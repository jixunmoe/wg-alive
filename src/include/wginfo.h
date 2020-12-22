#pragma once

#include <arpa/inet.h>

struct WireGuardInfo
{
  char iface[16];
  char gateway[INET6_ADDRSTRLEN + 1];
};

struct WireGuardConfigs
{
  int count;
  int capacity;
  struct WireGuardInfo *p;
};
