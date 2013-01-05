#ifndef XON__SOCKET_COMMUNICATION__H
#define XON__SOCKET_COMMUNICATION__H

/** @file */ 


#include <stddef.h>

#include "xon/status.h"
#include "xon/object.h"
#include "buffer.h"


int server_listen(void);
int server_accept(int sockfd);

int client_connect();

/*! @brief Send over socket 
 */
xon_status socket_send(int sockfd, void *data, size_t size);

/*! @brief Receive from socket into existing memory area
 */
xon_status socket_recv_buffer(int sockfd, void *data, size_t size);

/*! @brief Allocate memory and receive from socket.
 */
xon_status socket_recv(int sockfd, void **data_ptr, size_t size);

/*! @brief Send xon_obj
 */
xon_status socket_send_obj(int sockfd, xon_obj obj);

/*! @brief Receive xon_obj
 */
xon_status socket_recv_obj(int sockfd, xon_obj *obj_ptr);




#endif /* XON__SOCKET_COMMUNICATION__H */
