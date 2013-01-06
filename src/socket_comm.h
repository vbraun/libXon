#ifndef XON__SOCKET_COMMUNICATION__H
#define XON__SOCKET_COMMUNICATION__H

/** @file */ 


#include <stddef.h>

#include "xon/status.h"
#include "xon/object.h"
#include "buffer.h"


/*! @brief Listen on port
 * 
 *  @param port the initial port. If bind fails, try higher ports.
 *  @return the socket file descriptor
 */
int server_listen_net(int *port);

/*! @brief Accept connection on socket
 * 
 *  @param sockfd the bound port
 *  @param cookie to verify that the client is authorized
 *  @return the socket file descriptor to communicate on
 * 
 *  This function closes the original file descriptor sockfd (and,
 *  therefore, accepts no further connections).
 */
int server_accept(int sockfd, const char *cookie);

int client_connect(const char *cookie);

/*! @brief Send over socket 
 */
xon_status socket_send(int sockfd, const void *data, size_t size);

/*! @brief Receive from socket into existing memory area
 */
xon_status socket_recv_buffer(int sockfd, void *data, size_t size);

/*! @brief Allocate memory and receive from socket.
 */
xon_status socket_recv(int sockfd, void **data_ptr, size_t size);

/*! @brief Send xon_obj
 */
xon_status socket_send_obj(int sockfd, const xon_obj obj);

/*! @brief Receive xon_obj
 */
xon_status socket_recv_obj(int sockfd, xon_obj *obj_ptr);




#endif /* XON__SOCKET_COMMUNICATION__H */
