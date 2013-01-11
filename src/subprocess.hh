#ifndef XON__SUB_PROCESS__HH
#define XON__SUB_PROCESS__HH

/** @file */ 

#include <string>
#include <vector>

#include "xon/exceptions.hh"
#include "subprocess.h"

namespace xon {





class subprocess
{
private:
  std::string cmd;
  std::vector<std::string> args;
  std::string out, err;
  int status;
public:
  subprocess(const std::string& command);
  subprocess add_arg(const std::string& cmdline_argument);
  void operator << (const std::string& stdin);
  const std::string& stdout() const;
  const std::string& stderr() const;
  int exit_status() const;
};




} // end namespace xon

#endif /* XON__SUB_PROCESS__HH */
