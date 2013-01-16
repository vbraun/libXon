#include <stdio_ext.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>

using std::cout;
using std::cerr; 
using std::cin;
using std::endl;
using std::left;


void status(std::string name, FILE* stream, int fd)
{
  cout.width(10);
  cout.fill(' ');
  cout << left << name + ":"
       << (isatty(fd) ? " tty," : "pipe,");

  if (__flbf(stream))
    cout << "  line buffer mode,";
  else
    cout << " block buffer mode,";

  cout << " buffer size: " << __fbufsize(stream);

  cout << endl;
}



void print_status()
{
  if (isatty(STDIN_FILENO)) {
    cout << "Press enter to initialise stdin" << endl;
  } 
  char ch;
  fread(&ch,1,1,stdin);

  fprintf(stdout, "Initialising stdout\n");
  fprintf(stderr, "Initialising stderr\n");
  fflush(NULL);

  status("stdin", stdin,  STDIN_FILENO);
  status("stdout", stdout, STDOUT_FILENO);
  status("stderr", stderr, STDERR_FILENO);
}


// the least efficient "cat" implementation 
void cat() 
{
  char ch;
  while (!feof(stdin)) {
    fread(&ch, 1, 1, stdin);
    fwrite(&ch, 1, 1, stdout);
  }
}



int main(int argc, char *argv[])
{
  if (argc == 1) {
    print_status();
    return EXIT_SUCCESS;
  }
  
  if (argc == 2) {
    std::string arg = argv[1];
    int rcin, rcout;
    if (arg == "no") {
      rcin  = setvbuf(stdin,  NULL, _IONBF, 0);
      rcout = setvbuf(stdout, NULL, _IONBF, 0);
    } else if (arg == "line") {
      rcin  = setvbuf(stdin,  NULL, _IOLBF, 0);
      rcout = setvbuf(stdout, NULL, _IOLBF, 0);
    } else if (arg == "block") {
      rcin  = setvbuf(stdin,  NULL, _IOFBF, 4096);
      rcout = setvbuf(stdout, NULL, _IOFBF, 4096);
    } else {
      cerr << 
        "Argument must be 'no' (no buffering), 'line' (line buffering), " 
        "or 'block' (block buffering)." << endl;
    }
    if (rcin != 0 || rcout != 0)
      cerr << "Error: input=" << rcin << ", output=" << rcout << endl;
    cat();
    return EXIT_SUCCESS;
  }
  
  cout << "Usage: buffering [no|line|block]" << endl;

  return EXIT_FAILURE;
}
