#include <cassert>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

#include <unistd.h>
#include <sys/wait.h>
#include <poll.h>

#include "subprocess.hh"
#include "subprocess.c" 


namespace xon {



subprocess_factory::subprocess_factory(const std::string& command)
  : cmd(command)
{
}

subprocess_factory 
subprocess_factory::add_arg(const std::string& cmdline_argument)
{
  arg.push_back(cmdline_argument);
}

subprocess_factory 
subprocess_factory::add_env(const std::string& environment_variable, 
                           const std::string& value)
{
}

bool subprocess_factory::delete_env(const std::string& environment_variable)
{
}

const std::string 
subprocess_factory::get_command() const
{
  return cmd;
}

const std::vector<std::string>& 
subprocess_factory::get_env() const
{
  return env;
}

const std::vector<std::string>& 
subprocess_factory::get_args() const
{
  return arg;
}

char ** subprocess_factory::dup_env() const
{
}

char ** subprocess_factory::dup_argv() const
{
}

subprocess subprocess_factory::exec() const
{
  return subprocess().exec(*this);
}

subprocess_pipe subprocess_factory::exec_pipe() const
{
  return subprocess_pipe().exec(*this);
}




namespace { 

//! Communicate with pipes
void read_write_all(int infd, const std::string& stdin, 
                    int outfd, std::stringstream& stdout_stream, 
                    int errfd, std::stringstream& stderr_stream)
{
  const size_t size = 4096;
  char buf[size];
  
  size_t stdin_remaining = stdin.size();
  const char *stdin_ptr = stdin.c_str();

  struct pollfd fds[3];
  fds[0].fd = infd;
  fds[0].events = POLLOUT;
  fds[1].fd = outfd;
  fds[1].events = POLLIN;
  fds[2].fd = errfd;
  fds[2].events = POLLIN;

  while (fds[0].fd > 0 || fds[1].fd > 0 || fds[2].fd > 0) {
    int rc = poll(fds, 3, -1);  // -1 == no timeout

    if (rc == 0)
      debug_printf("timeout\n");

    if (rc < 0) {
      error_printf("piping to child process: %s\n", strerror(errno));
      return;
    }

    if ((fds[0].revents & POLLOUT) != 0) {
      int n = write(fds[0].fd, stdin_ptr, std::min(size, stdin_remaining));
      // std::cout << "writing to stdin " << n << std::endl;
      stdin_ptr += n;
      stdin_remaining -= n;
      if (stdin_remaining == 0) {   // done
        close(fds[0].fd);
        fds[0].fd = -1;
        fds[0].revents = 0;  
      }
    }
    
    if ((fds[1].revents & POLLIN) != 0) {
      int n = read(fds[1].fd, buf, size);
      // std::cout << "reading from stdout " << n << std::endl;
      assert(n >= 0); // do not set O_NONBLOCK
      if (n == 0) {   // done
        close(fds[1].fd);
        fds[1].fd = -1;
        fds[1].revents = POLLHUP;
      } else
        stdout_stream.write(buf, n);
    }

    if ((fds[2].revents & POLLIN) != 0) {
      int n = read(fds[2].fd, buf, size);
      // std::cout << "reading from stderr " << n << std::endl;
      assert(n >= 0); // do not set O_NONBLOCK
      if (n == 0) {   // done
        close(fds[2].fd);
        fds[2].fd = -1;
        fds[2].revents = POLLHUP;
      } else
      stderr_stream.write(buf, n);
    }

    if ((fds[0].revents & (POLLERR|POLLNVAL)) != 0) {
      error_printf("stdin error\n");
      fds[0].fd = -1;
      fds[0].revents = 0;
    }

    if ((fds[1].revents & (POLLERR|POLLNVAL)) != 0) {
      error_printf("stdout error\n");
      fds[1].fd = -1;
      fds[1].revents = POLLHUP;
    }

    if ((fds[2].revents & (POLLERR|POLLNVAL)) != 0) {
      error_printf("stdout error\n");
      fds[2].fd = -1;
      fds[2].revents = POLLHUP;
    }

    if (fds[0].revents == 0 && 
        fds[1].revents == POLLHUP && 
        fds[2].revents == POLLHUP) 
      // nothing to do?? Remote end has closed pipes and we read
      // everything that remained buffered.
      break;
  }
}


int communicate(const std::string& command, 
                const std::vector<std::string> &args,
                const std::string& stdin, 
                std::stringstream& stdout_stream, 
                std::stringstream& stderr_stream)
{
  int infd[2], outfd[2], errfd[2];
  pipe(infd);
  pipe(outfd);
  pipe(errfd);

  fflush(STDOUT_FILENO);
  fflush(STDERR_FILENO);
  pid_t pid = fork();
  if (pid == 0) { // child
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    dup2(infd[0], STDIN_FILENO);
    dup2(outfd[1], STDOUT_FILENO);
    dup2(errfd[1], STDERR_FILENO);
    close(outfd[0]);   close(outfd[1]);
    close(errfd[0]);   close(errfd[1]);
    close(infd[0]);    close(infd[1]);
    execlp(command.c_str(), command.c_str(), NULL);
    std::cerr << "Failed to execute \"" << command << "\": " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    throw subprocess_exception("Failed to fork (out of memory?).\n");
  }

  close(infd[0]);
  close(outfd[1]);
  close(errfd[1]);

  read_write_all(infd[1], stdin,
                 outfd[0], stdout_stream, 
                 errfd[0], stderr_stream);
  
  close(infd[1]);
  close(outfd[0]);
  close(errfd[0]);

  int status;
  waitpid(pid, &status, 0);
  return WEXITSTATUS(status);
}

} // end anonymous namespace


//////////////////////////////////////////////////////
///
/// Implementation of class subprocess
///
//////////////////////////////////////////////////////


subprocess::subprocess()
  : pid(-1), status(-1)
{
}

subprocess::~subprocess()
{
  if (is_running())
    wait(WAIT_EXIT);  
  if (is_running())
    kill();
}

void subprocess::exec(const subprocess_factory& factory)
{
  cmd = factory.get_command();

  fflush(STDOUT_FILENO);
  fflush(STDERR_FILENO);
  pid = fork();
  if (pid == 0) { // child
    child();
    std::cerr << "Failed to execute \"" << cmd << "\": " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    throw subprocess_exception("Failed to fork (out of memory?).\n");
  }
}

void subprocess::child(const subprocess_factory& factory)
{
  execlp(cmd.c_str(), cmd.c_str(), NULL);
}

bool subprocess::is_running() const
{
  return pid < 0;
}

int subprocess::exit_status() const
{
  assert(!is_running());
  return WEXITSTATUS(status);
}

void subprocess::kill() const
{
  c_api::kill_subprocess_status(pid, &status);
  pid = -1;
}

void subprocess::wait(double timeout = 0) const
{
  xon::c_api::xon_status rc = 
    c_api::wait_for_subprocess_status(pid, timeout, &status);
  if (rc == XON_OK)
    pid = -1;
}


//////////////////////////////////////////////////////
///
/// Implementation of class subprocess
///
//////////////////////////////////////////////////////


subprocess_pipe::subprocess_pipe(const subprocess_factory& factory)
  : subprocess(factory)
{
  pipe(infd);
  pipe(outfd);
  pipe(errfd);
}


subprocess_pipe::~subprocess_pipe()
{
  close(infd[1]);
  close(outfd[0]);
  close(errfd[0]);
}

void subprocess_pipe::exec(const subprocess_factory& factory)
{
  subprocess::exec(factory);

  close(infd[0]);
  close(outfd[1]);
  close(errfd[1]);
}

void subprocess_pipe::child(const subprocess_factory& factory)
{
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  dup2(infd[0], STDIN_FILENO);
  dup2(outfd[1], STDOUT_FILENO);
  dup2(errfd[1], STDERR_FILENO);
  close(outfd[0]);   close(outfd[1]);
  close(errfd[0]);   close(errfd[1]);
  close(infd[0]);    close(infd[1]);
  subprocess::child(factory);
}

void subprocess_pipe::operator << (const std::string& stdin)
{
  std::stringstream stdout_stream, stderr_stream;
  read_write_all(infd[1], stdin,
                 outfd[0], stdout_stream, 
                 errfd[0], stderr_stream);
  out = stdout_stream.str();
  err = stderr_stream.str();
}

const std::string& subprocess_pipe::stdout() const
{
  return out;
}

const std::string& subprocess_pipe::stderr() const
{
  return err;
}



} // end namespace xon
