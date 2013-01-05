#ifndef XON__SUB_PROCESS__H
#define XON__SUB_PROCESS__H


#include <sys/types.h>


pid_t run_subprocess(const char *dst, const char *cookie);
void wait_for_subprocess(const pid_t pid);
void kill_subprocess(const pid_t pid);



#endif /* XON__SUB_PROCESS__H */

