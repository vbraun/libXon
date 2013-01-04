#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#include "socket_comm.h"


#define PORT "13490"



union sockaddr_any {
  struct sockaddr sa;
  struct sockaddr_in sa_in;
  struct sockaddr_in6 sa_in6;
  struct sockaddr_storage sa_storage;
};




int server_listen(void)
{
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  
  struct addrinfo *res;
  int rv = getaddrinfo(NULL, PORT, &hints, &res);
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
    fprintf(stderr, "server: failed to bind\n");
    return -1;
  }
  
  printf("server: waiting for connections...\n");
  if (listen(sockfd, 1) == -1) {
    perror("server: listen");
    return -1;
  }
  return sockfd;
}


int server_accept(int sockfd)
{
  struct sockaddr_storage addr;
  socklen_t sin_size = sizeof(union sockaddr_any);

  int new_fd = accept(sockfd, (struct sockaddr *)&addr, &sin_size);
  if (new_fd == -1) {
    perror("server: accept");
    return -1;
  }
    
  printf("server: got connection\n");
  
  close(sockfd);
  return new_fd;
}




int client_connect()
{
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  
  struct addrinfo *res;
  int rv = getaddrinfo(NULL, PORT, &hints, &res);
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

  printf("client: connecting\n");
  
  return sockfd;
}


int socket_send_all(int sockfd, void *buffer, size_t size) 
{
  char *buf = (char*)buffer;
  int bytes_sent = 0;
  int bytes_left = size;
  while(bytes_sent < size) {
    int n = send(sockfd, buf + bytes_sent, bytes_left, 0);
    if (n == -1) 
      return -1;
    bytes_sent += n;
    bytes_left -= n;
  }
}


int socket_recv_all(int sockfd, void **buffer_ptr)
{
  const int bufsize = 1024;
  char buf[bufsize];
  int n = recv(sockfd, buf, bufsize, 0);
  if (n == -1)
    return -1;
  *buffer_ptr = malloc(n);
  memcpy(*buffer_ptr, buf, n);
  return n;
}
