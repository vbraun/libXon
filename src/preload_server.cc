#include <cassert>





int main(int argc, char *argv[])
{
  if (argc == 3)
    return test_open_explicit(argv[1], argv[2]);
  else if (argc == 2)
    return test_mathlink(argv[1]);
  else
    return run_server();
}
