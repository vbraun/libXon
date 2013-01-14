#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>  // for getenv

/* Imagine this is your average closed-source interpreter. You don't
   have access to the source code so you can't change it and link
   against libXon. 
   
   But, being a reasonably complete interpreted language, you can open
   files and read/write to them. Perhaps even binary data if you are
   lucky.

   EVIL HACK: we override file read from filenames starting with
   "/xon/input/" and file write to filenames starting with
   "/xon/output/". This works by overriding the libc open(), read(),
   write(), and close() calls with a shared library that we can force
   in via LD_PRELOAD.
   
*/


std::string read_from_file(const std::string& filename)
{
  std::fstream fs(filename.c_str(), std::fstream::in);
  std::string data;
  fs >> data;
  if (fs.fail())
    std::cout << "reading from " << filename << " failed." << std::endl;
  fs.close();
  return data;
}


void write_to_file(const std::string& filename, const std::string& data)
{
  std::fstream fs(filename.c_str(), std::fstream::out);
  fs << data;
  if (fs.fail())
    std::cout << "writing to " << filename << " failed." << std::endl;
  fs.close();
}


int main(void)
{
  std::cout << "LD_PRELOAD = " << getenv("LD_PRELOAD") << std::endl;
  
  // Using your closed-source interpreter to load a file ends up doing
  // this
  std::string input = read_from_file("/xon/input/key");

  // And using your closed-source interpreter to write to a file does
  // something like this
  write_to_file("/xon/output/result", 
                std::string("I got your message: ")+input);

  // If you run this program straight-away, you will get read/write
  // errors since you clearly can't read/write to /xon. But if you 

  return 0;
}
