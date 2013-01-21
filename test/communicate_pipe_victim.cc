#include <stdio.h>

#include <string>

/*********************************************************************

Imagine this is your average closed-source interpreter. You don't have
access to the source code so you can't change it and link against
libXon.
   
The next-best thing is to connect stdin/stdout to pipes; Pipe in
commands and pipe out the result. This usually does not work because
of buffering. It is much more efficient to read larger chunks that
individual bytes at a time, so libc switches the stdin/stdout streams
to buffering mode. You can check this with the test/buffering program;
Compare this:

    [vbraun@volker-desktop test]$ ./buffering 
    Press enter to initialise stdin

    Initialising stdout
    Initialising stderr
    stdin:     tty,  line buffer mode, buffer size: 1024
    stdout:    tty,  line buffer mode, buffer size: 1024
    stderr:    tty, block buffer mode, buffer size: 1

to naively equivalent pipe version:

    [vbraun@volker-desktop test]$ echo "anykey" | ./buffering | cat
    Initialising stderr
    Initialising stdout
    stdin:    pipe, block buffer mode, buffer size: 4096
    stdout:   pipe, block buffer mode, buffer size: 4096
    stderr:    tty, block buffer mode, buffer size: 1

The problem is, in block buffer mode any half-filled stdin buffer will
not be delivered to the program's fread(). And anything written with
fwrite() to stdout will not be delivered to the next program in the
pipe until a whole 4096-byte long buffer is full. If you could only
stick in a fflush() in there!

EVIL HACK: we override C isatty/setbuf/... commands that are used to
set and change the buffer. This we can do to a dynamically linked
program via LD_PRELOAD.
   
*********************************************************************/


#define size 4096
char buf[size];

void screw_up_buffering()
{
  /* libc does this automatically if stdout is a pipe */
  setvbuf(stdout, buf, _IOFBF, size);
}


void read_eval_write()
{
  char ch[128];
  int matched = fscanf(stdin, "%127s", ch);
  if (matched != 1)
    return;

  std::string out = std::string("(stdout) input is >>>") + ch + "<<<\n";
  std::string err = "(stderr) no error\n";

  fwrite(out.c_str(), 1, out.length()+1, stdout);
  fwrite(err.c_str(), 1, err.length()+1, stderr);
}


int main(void)
{
  screw_up_buffering();
  while (!feof(stdin))
    read_eval_write();
  return 0;
}
