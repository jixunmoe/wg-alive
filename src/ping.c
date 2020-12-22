#include "include/ping.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#define SLEEP_TIME (25 * 1000)

int to_ms(struct timespec *tp)
{
  return (int)tp->tv_sec * 1000 + (int)round(tp->tv_nsec / 1.0e6);
}

void sleep_ms(long ms)
{
  struct timespec tp = {};
  tp.tv_sec = (int)floor(ms / 1000);
  tp.tv_nsec = (ms % 1000) * 1000;
  nanosleep(&tp, NULL);
}

void ping_gateway(struct WireGuardInfo *wg)
{
  pid_t cpid = fork();
  if (cpid == -1)
  {
    // failed to fork
    return;
  }
  else if (cpid != 0)
  {
    fprintf(stderr, "ping %s (%s)...", wg->iface, wg->gateway);
    fflush(stderr);
    // parent
    int wstatus;
    waitpid(cpid, &wstatus, 0);
    fprintf(stderr, "%s\n", (wstatus == 0) ? "ok" : "bad");
    return;
  }

  int fd_null = open("/dev/null", O_WRONLY);

  dup2(fd_null, STDOUT_FILENO);
  dup2(fd_null, STDERR_FILENO);

  close(fd_null);

  execlp("ping", "ping", "-W", "1", "-c", "1", wg->gateway, NULL);
}

void ping_nonstop(struct WireGuardConfigs *config)
{
  struct timespec tp = {};
  while (true)
  {
    clock_gettime(CLOCK_REALTIME, &tp);
    int now = to_ms(&tp);

    for (int i = 0; i < config->count; i++)
    {
      ping_gateway(&config->p[i]);
    }

    clock_gettime(CLOCK_REALTIME, &tp);
    long diff = now - to_ms(&tp);
    if (diff < SLEEP_TIME)
    {
      sleep_ms(SLEEP_TIME - diff);
    }
  }
}
