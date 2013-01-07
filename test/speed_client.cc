#include <sys/time.h>
#include <iostream>
#include <string>

#include <xon/client.hh>

using std::cout;
using std::endl;


long microsec(struct timeval &start, struct timeval &finish)
{
  long usec = (finish.tv_sec-start.tv_sec)*1000*1000;
  usec += (finish.tv_usec-start.tv_usec);
  return usec;
}


int compute(const int x)
{
  return x*x;
};



int main(void)
{
  struct timeval t_begin, t_end;

  xon::obj_builder xb;
  xon::client client("./speed_server");

  gettimeofday(&t_begin, NULL);
  const int64_t N = 10000;
  for (int64_t x = 0; x < N; x++) {
    xb.add("quit", false);
    xb.add("x", x);
    const xon::object input = xb.get();
    client.send(input);
  
    xon::object result = client.receive();
    xon::obj_reader xr(result);
    const int y = xr.get_int64("result");
    if (y != compute(x)) // check result
      return 1;
  }
  gettimeofday(&t_end, NULL);

  xb.add("quit", true);
  const xon::object poison_pill = xb.get();
  client.send(poison_pill);
  client.wait(1);

  cout << "Time per message round-trip: " 
       << microsec(t_begin, t_end)/N << " us (microseconds)" << endl;

  return 0;
}
