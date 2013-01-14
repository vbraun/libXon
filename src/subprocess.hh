#ifndef XON__SUB_PROCESS__HH
#define XON__SUB_PROCESS__HH

/** @file */ 

#include <string>
#include <vector>

#include <sys/types.h>

#include "xon/exceptions.hh"
#include "subprocess.h"

namespace xon {


class subprocess;
class subprocess_factory;


class subprocess_factory
{
private:
  std::string cmd;
  std::vector<std::string> env;
  std::vector<std::string> args;
public:
  subprocess_factory(const std::string& command);
  subprocess_factory add_arg(const std::string& cmdline_argument);
  subprocess_factory add_env(const std::string& environment_variable, 
                             const std::string& value);
  const std::string get_command() const;
  //! Remove environment variable
  /*  @param environment_variable 
   *  @return whether the environment variable was actually present
   *          before deletion.
   *  It is not an error to call this with a variable that is not
   *  present.
   */
  bool delete_env(const std::string& environment_variable);
  const std::vector<std::string>& get_env() const;
  const std::vector<std::string>& get_args() const;
  //! Allocate env suitable for calling execle, execve
  char ** dup_env() const;
  //! Allocate argv suitable for calling execv, execvp, execvpe
  char ** dup_argv() const;
  //! Create new process
  subprocess exec() const;
  subprocess_pipe exec_pipe() const;
}




class subprocess
{
  friend class subprocess_factory;
protected:
  const int WAIT_EXIT=10;
  std::string cmd;
  pid_t pid;
  int status;
  subprocess();
  //! Do the fork()
  virtual subprocess& exec(const subprocess_factory& factory);
  //! This is called after fork()
  virtual void child(const subprocess_factory& factory);
public:
  virtual ~subprocess();
  bool is_running() const;
  int exit_status() const;
  void wait(double timeout = 0);
  void kill();
};


class subprocess_pipe : public subprocess
{
  friend class subprocess_factory;
private:
  int infd[2], outfd[2], errfd[2];
  std::string out, err;
  subprocess_pipe();
  virtual subprocess_pipe& exec(const subprocess_factory& factory);
  virtual void child(const subprocess_factory& factory);
protected:
  virtual 
public:
  virtual ~subprocess_pipe();
  void operator << (const std::string& stdin);
  const std::string& stdout() const;
  const std::string& stderr() const;
};




} // end namespace xon

#endif /* XON__SUB_PROCESS__HH */
