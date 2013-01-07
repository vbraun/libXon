#include <iostream>

#include <xon/client.hh>



void test_bc() 
{
  using namespace std;

  const string command = "bc";
  const string stdin = "3^2^10\n";
  const string command = "cat";
  const vector<string> args;
 
  stringstream stdout;
  stringstream stderr;
  int rc = communicate(command, args, stdin, stdout, stderr);

  cout << "Reply:" << endl 
       << "stdin: "  << stdin << endl 
       << "stdout: " << stdout.str() << endl 
       << "stderr: " << stderr.str() << endl
       << "Return code = " << rc << endl;
}



void test_long() 
{
  using namespace std;

  const string command = "cat";
  vector<string> args;
  stringstream ss;
  for (int i=0; i<100000; i++)
    ss << "0123456789\n";
  const string stdin = ss.str();
 
  stringstream stdout;
  stringstream stderr;
  int rc = communicate(command, args, stdin, stdout, stderr);

  cout << "Reply:" << endl 
       << "stdin: "  << stdin.size() << " bytes" << endl 
       << "stdout: " << stdout.str().size() << " bytes" << endl 
       << "stderr: " << stderr.str().size() << " bytes" << endl
       << "Return code = " << rc << endl;
}




int main(void)
{
  test_bc();
  test_long();
  return 0;
}
