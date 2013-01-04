#ifndef SUB_PROCESS__H
#define SUB_PROCESS__H


#include <sys/types.h>


pid_t run_subprocess(const char *dst);
void wait_for_subprocess(const pid_t pid);
void kill_subprocess(const pid_t pid);



#endif /* SUB_PROCESS__H */

