#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
extern char **environ;

#include "subprocess.h"
#include "cookie.h"




static
char **new_environment(const char *cookie) 
{
  const int size = 128;
  char* cookie_var = (char*)malloc(size);
  snprintf(cookie_var, size, "%s=%s", COOKIE_ENV_VAR, cookie);

  // duplicate environment
  size_t n = 0;
  for (char *e = *environ; e!=NULL; e++)
    n += 1;
  char ** newenv = (char**)malloc((n+1) * sizeof(char*));
  char **ne = newenv;
  *(ne++) = cookie_var;
  for (char *e = *environ; e!=NULL; e++) {
    if (strncmp(e, COOKIE_ENV_VAR, strlen(COOKIE_ENV_VAR)) == 0)
      continue;
    *(ne++) = e;
  }
  *(ne++) = NULL;
  return newenv;
}


pid_t run_subprocess(const char *dst, const char *cookie)
{
  pid_t pid = fork();
  if (pid == 0) { // child
    execle(dst, dst, NULL, new_environment(cookie));
    const int size = 1024;
    char err[size];
    snprintf(err, size, "Failed to execute %s", dst);
    perror(err);
    exit(2);
  } else if (pid < 0) {  // failed to fork
    fprintf(stderr, "Failed to fork (out of memory?).\n");
    exit(2);
  }
  return pid;
}


void wait_for_subprocess(const pid_t pid) 
{
  waitpid(pid, NULL, 0);
}


void kill_subprocess(const pid_t pid)
{
  kill(pid, SIGKILL);
}
