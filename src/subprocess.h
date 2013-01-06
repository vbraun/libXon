#ifndef XON__SUB_PROCESS__H
#define XON__SUB_PROCESS__H

/** @file */ 

#include <sys/types.h>

#include "xon/support.h"
#include "xon/status.h"

NAMESPACE_XON_C_API_BEGIN




pid_t run_subprocess(const char *dst, const char *cookie);

/*! @brief Wait for subprocess to complete
 * 
 *  @param pid the pid of the subprocess
 *  @param timeout a timeout in seconds. 
 *  @return either XON_OK if the process finished or XON_TIMEOUT.
 */
xon_status wait_for_subprocess(const pid_t pid, double timeout);


void kill_subprocess(const pid_t pid);



NAMESPACE_XON_C_API_END

#endif /* XON__SUB_PROCESS__H */

