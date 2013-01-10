#ifndef XON__SUB_PROCESS__HH
#define XON__SUB_PROCESS__HH

/** @file */ 

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "subprocess.h"

namespace xon {




void read_write_all(int infd, const std::string& stdin, 
                    int outfd, std::stringstream& stdout_stream, 
                    int errfd, std::stringstream& stderr_stream);

int communicate(const std::string& command, 
                const std::vector<std::string> &args,
                const std::string& stdin, 
                std::stringstream& stdout_stream, 
                std::stringstream& stderr_stream);



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
