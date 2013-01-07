#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sched.h>
extern char **environ;

#include "xon/support.h"
#include "subprocess.h"
#include "cookie.h"
#include "debug.h"

NAMESPACE_XON_C_API_BEGIN


static
char **new_environment(const char *cookie) 
{
  const int size = 128;
  char* cookie_var = (char*)malloc(size);
  snprintf(cookie_var, size, "%s=%s", COOKIE_ENV_VAR, cookie);

  // duplicate environment
  size_t n = 0;
  for (char **e = environ; *e != NULL; e++)
    n += 1;
  char ** newenv = (char**)malloc((n+2) * sizeof(char*));
  char **ne = newenv;
  *(ne++) = cookie_var;
  for (char **e = environ; *e != NULL; e++) {
    if (strncmp(*e, COOKIE_ENV_VAR, strlen(COOKIE_ENV_VAR)) == 0)
      continue;
    *(ne++) = strdup(*e);
    // printf("%s\n", *e);
  }
  *(ne++) = NULL;
  return newenv;
}


pid_t run_subprocess(const char *dst, const char *cookie)
{
  pid_t pid = fork();
  if (pid == 0) { // child
    char ** env = new_environment(cookie);
    printf("env %s\n", *env);
#ifdef XON_DEBUG
    // execle("valgrind", "valgrind", dst, NULL, env);
    execle("/usr/bin/libtool", "libtool", "--mode=execute", 
           "valgrind", "--leak-check=full", dst, NULL, env);
#else
    execle(dst, dst, NULL, env);
#endif
    error_printf("Failed to execute %s: %s\n", dst, strerror(errno));
    exit(EXIT_FAILURE);
  } else if (pid < 0) {  // failed to fork
    error_printf("Failed to fork (out of memory?).\n");
    return 0;
  }
  return pid;
}


xon_status wait_for_subprocess(const pid_t pid, double timeout) 
{
  time_t start = time(NULL);
  while (true) {
    if (waitpid(pid, NULL, WNOHANG) == pid)
      return XON_OK;
    time_t now = time(NULL);
    double elapsed = difftime(now, start);
    if (elapsed > timeout)
      return XON_TIMEOUT;
    sched_yield();
  }
}


void kill_subprocess(const pid_t pid)
{
  kill(pid, SIGKILL);
  waitpid(pid, NULL, 0);
}




NAMESPACE_XON_C_API_END
