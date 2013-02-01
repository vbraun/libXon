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
class subprocess_pipe;
class subprocess_factory;



//! The factory for subprocesses
/*  This class encapsulates all the information for starting a new
 *  subprocess: command, arguments, environment.
 */
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
};



//! A simple subprocess
/*  This is the base class for all subprocesses. It just runs a
 *  subprocess.
 */
class subprocess
{
private:
  static const int WAIT_EXIT=10;
  pid_t pid;
  int status;
  //! Forbid copy constructor
  subprocess(const subprocess&) {};
protected:
  subprocess();
  //! Initialize variables
  virtual void init();
  //! Do the fork()
  virtual subprocess& exec(const subprocess_factory& factory);
  //! This is called after fork()
  virtual void child(const subprocess_factory& factory);
public:
  subprocess(const subprocess_factory& factory);
  virtual ~subprocess();
  bool is_running() const;
  int exit_status() const;
  void wait(double timeout = WAIT_EXIT);
  void kill();
};


//! A subprocess filter (connected to input/output pipes)
/*  This class implements a filter, it pipes fixed input trough the
 *  subprocess and collects all of its output.
 */
class subprocess_pipe : public subprocess
{
private:
  int infd[2], outfd[2], errfd[2];
  std::string out, err;
  //! Forbid copy constructor
  subprocess_pipe(const subprocess_pipe&) {};
protected:
  subprocess_pipe();
  virtual void init();
  virtual subprocess_pipe& exec(const subprocess_factory& factory);
  virtual void child(const subprocess_factory& factory);
public:
  subprocess_pipe(const subprocess_factory& factory);
  virtual ~subprocess_pipe();
  void operator << (const std::string& stdin);
  const std::string& stdout() const;
  const std::string& stderr() const;
};



//! A subprocess with continued input/output communication over pipes
/*  This class implements a subprocess that is driven via pipes. So
 *  you can send some input, get all the output up to some marker (the
 *  subprocess input prompt), send some more input, get some more
 *  output, etc. Usually that does not work because of buffering, but
 *  we use a LD_PRELOAD hack (see stdbuf_preload.cc) to make it work.
 */
class subprocess_communicate : public subprocess_pipe
{
}




} // end namespace xon

#endif /* XON__SUB_PROCESS__HH */
