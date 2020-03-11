#include <iostream>
#include "yasio/xxsocket.hpp"
#include "yasio/yasio.hpp"
#include "yasio/obstream.hpp"

using namespace yasio;
using namespace yasio::inet;

static int count = 0;

#ifndef _WIN32
static struct itimerval oldtv;
void set_timer()
{
  struct itimerval itv;
  itv.it_interval.tv_sec  = 0;
  itv.it_interval.tv_usec = 100;
  itv.it_value.tv_sec     = 0;
  itv.it_value.tv_usec    = 100;
  setitimer(ITIMER_REAL, &itv, &oldtv);
}

void signal_handler(int m) { count++; }
#endif

int main()
{
#ifndef _WIN32
  signal(SIGALRM, signal_handler);
  set_timer();
#endif
  xxsocket sock;
  auto wtimeout = std::chrono::milliseconds(5000);
  if (0 != sock.pconnect_n("www.ip138.com", 80, wtimeout))
  {
    printf("connect error\n");
    return -2;
  }

  obstream obs;
  obs.write_bytes("GET /index.htm HTTP/1.1\r\n");

  obs.write_bytes("Host: www.ip138.com\r\n");

  obs.write_bytes("User-Agent: Mozilla/5.0 (Windows NT 10.0; "
                  "WOW64) AppleWebKit/537.36 (KHTML, like Gecko) "
                  "Chrome/79.0.3945.117 Safari/537.36\r\n");
  obs.write_bytes("Accept: */*;q=0.8\r\n");
  obs.write_bytes("Connection: Close\r\n\r\n");
  auto k = obs.data();
  if (obs.length() != sock.send(obs.data(), obs.length()))
  {
    printf("send error\n");
    return -4;
  }

  getchar();

  return 0;
}