#include "include/wginfo.h"
#include "include/config.h"
#include "include/ping.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "%s <config.txt> [-debug]\n", argv[0]);
    return 1;
  }

  struct WireGuardConfigs *config = parse_config(argv[1]);
  if (!config)
  {
    fprintf(stderr, "could not read config or failed to parse\n");
    return 2;
  }

  if (config->count == 0)
  {
    fprintf(stderr, "empty config, nothing to do.\n");
    return 3;
  }

  if (argc >= 3 && strcmp("-debug", argv[2]) == 0)
  {
    fprintf(stderr, "found %d %s:\n", config->count, (config->count == 1) ? "entry" : "entries");
    for (int i = 0; i < config->count; i++)
    {
      printf("%s: %s\n", config->p[i].iface, config->p[i].gateway);
    }
  }

  ping_nonstop(config);

  return 0;
}
