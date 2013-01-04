#ifndef SOCKET_COMMUNICATION__H
#define SOCKET_COMMUNICATION__H

#include <stddef.h>

int server_listen(void);
int server_accept(int sockfd);

int client_connect();

int socket_send_all(int sockfd, void *buffer, size_t size);
int socket_recv_all(int sockfd, void **buffer_ptr);




#endif /* SOCKET_COMMUNICATION__H */
