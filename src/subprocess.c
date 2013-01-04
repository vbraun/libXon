#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "subprocess.h"


pid_t run_subprocess(const char *dst)
{
  pid_t parent_pid = getpid();
  pid_t pid = fork();
  if (pid == 0) { // child
    execlp(dst, NULL);
    const int size = 1024;
    char err[size];
    snprintf(err, size, "Failed to execute %s", dst);
    perror(err);
    exit(2);
  } else if (pid < 0) {  // failed to fork
    fprintf(stderr, "Failed to fork (out of memory?).\n");
    exit(2);
  }
}


void wait_for_subprocess(const pid_t pid) 
{
  waitpid(pid, NULL, 0);
}


void kill_subprocess(const pid_t pid)
{
  kill(pid, SIGKILL);
}
