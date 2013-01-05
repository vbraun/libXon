#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <endian.h>

#include "socket_comm.h"
#include "cookie.h"
#include "align.h"
#include "debug.h"



union sockaddr_any {
  struct sockaddr sa;
  struct sockaddr_in sa_in;
  struct sockaddr_in6 sa_in6;
  struct sockaddr_storage sa_storage;
};



int try_bind_port(int port)
{
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  
  char port_str[10];
  snprintf(port_str, 10, "%d", port);

  struct addrinfo *res;
  int rv = getaddrinfo(NULL, port_str, &hints, &res);
  if (rv != 0) {
    fprintf(stderr, "server: getaddrinfo: %s\n", gai_strerror(rv));
    return -1;
  }

  int sockfd;
  struct addrinfo *p;
  for(p = res; p != NULL; p = p->ai_next) {
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      perror("server: socket");
      continue;
    }
    int yes=1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      close(sockfd);
      perror("server: setsockopt");
      continue;
    }
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }
    break;
  }
  freeaddrinfo(res);
  
  if (p == NULL)  {
    fprintf(stderr, "server: failed to bind port %d\n", port);
    return -1;
  }
  return sockfd;
}


int server_listen_net(int *port)
{
  int sockfd = -1;
  for (int good_port = *port; good_port < (*port)+1024; good_port++) {
    sockfd = try_bind_port(good_port);
    if (sockfd != -1) {
      *port = good_port;
      break;
    }
  }

  printf("server: waiting for connections on port %d\n", *port);
  if (listen(sockfd, 1) == -1) {
    perror("server: listen");
    return -1;
  }

  return sockfd;
}


int server_accept(int sockfd, const char *cookie)
{
  struct sockaddr_storage addr;
  socklen_t sin_size = sizeof(union sockaddr_any);

  int new_fd = accept(sockfd, (struct sockaddr *)&addr, &sin_size);
  if (new_fd == -1) {
    perror("server: accept");
    return -1;
  }
    
  debug_printf("server: got connection\n");
  
  close(sockfd);
  return new_fd;
}



int client_connect(const char*cookie)
{
  int port;
  parse_cookie(cookie, &port);
  
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  char port_str[10];
  snprintf(port_str, 10, "%d", port);
  
  struct addrinfo *res;
  int rv = getaddrinfo(NULL, port_str, &hints, &res);
  if (rv != 0) {
    fprintf(stderr, "client: getaddrinfo: %s\n", gai_strerror(rv));
    return -1;
  }

  int sockfd;  
  struct addrinfo *p;
  for(p = res; p != NULL; p = p->ai_next) {
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      perror("client: socket");
      continue;
    }
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    }
    break;
  }
  freeaddrinfo(res);
  
  if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    return -1;
  }

  debug_printf("client: connecting\n");
  
  return sockfd;
}


xon_status socket_send(int sockfd, void *data, size_t size) 
{
  char *buf = (char*)data;
  int bytes_sent = 0;
  int bytes_left = size;
  while (bytes_sent < size) {
    int n = send(sockfd, buf + bytes_sent, bytes_left, 0);
    if (n == -1) 
      return XON_ERROR_SEND;
    bytes_sent += n;
    bytes_left -= n;
  }
  return XON_OK;
}


xon_status socket_recv_buffer(int sockfd, void *data, size_t size)
{
  char *buf = (char*)data;
  int bytes_recd = 0;
  int bytes_left = size;
  while (bytes_recd < size) {
    int n = recv(sockfd, buf + bytes_recd, bytes_left, 0);
    if (n == -1) {
      return XON_ERROR_RECV;
    }
    bytes_recd += n;
    bytes_left -= n;
  }
  return XON_OK;
}
  

xon_status socket_recv(int sockfd, void **data_ptr, size_t size)
{
  void *data = malloc(size);
  if (data == NULL)
    return XON_ERROR_MALLOC;
  xon_status rc = socket_recv_buffer(sockfd, data, size);
  if (rc == XON_OK)
    *data_ptr = data;
  else
    free(data);
  return rc;
}
  

xon_status socket_send_obj(int sockfd, xon_obj obj)
{
  size_t size = xon_obj_size(obj);
  return socket_send(sockfd, obj, size);
}

xon_status socket_recv_obj(int sockfd, xon_obj *obj_ptr)
{
  int32_t start;
  xon_status rc;
  rc = socket_recv_buffer(sockfd, &start, sizeof(int32_t));
  //debug_printf("socket_recv_obj: 0x%x (little endian: 0x%x) (magic: 0x%x)\n", 
  //             start, le32toh(start), XON_MAGIC);
  if (rc != XON_OK)
    return rc;
  if (!xon_obj_verify_architecture((xon_obj)(&start)))
    return XON_ERROR_ARCH_MISMATCH;
  char *data;
  if (xon_obj_is_bson((xon_obj)(&start))) {
    int32_t size = le32toh(start);
    data = (char*)malloc(size + sizeof(int32_t));
    if (data == NULL)
      return XON_ERROR_MALLOC;
    *(int32_t*)data = start;
    socket_recv_buffer(sockfd, data+sizeof(int32_t), size-sizeof(int32_t));
  } else {
    char hdr[2*ALIGN_BYTES];
    socket_recv_buffer(sockfd, hdr+sizeof(int32_t), 2*ALIGN_BYTES-sizeof(int32_t));
    int size = xon_obj_size((xon_obj)hdr);
    data = (char*)malloc(size + sizeof(int32_t));
    if (data == NULL)
      return XON_ERROR_MALLOC;
    *(int32_t*)data = start;
    *(int*)(data + ALIGN_BYTES) = size;
    socket_recv_buffer(sockfd, data+2*ALIGN_BYTES, size-2*ALIGN_BYTES);
  }
  *obj_ptr = (xon_obj)data;
  return XON_OK;
}


