#include "include/wginfo.h"
#include "include/utils.h"

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

const char *skip_whitespace(const char *str)
{
  const char *p = str;
  while (isspace(*p))
  {
    p++;
  }
  return *p ? p : NULL;
}

const char *find_whitespace(const char *str)
{
  const char *p = str;
  while (!isspace(*p))
  {
    if (!*p)
      return NULL;
    p++;
  }
  return p;
}

#define sizeof_gateway (sizeof_field(struct WireGuardInfo, gateway))
#define sizeof_iface (sizeof_field(struct WireGuardInfo, iface))
#define MIN(a, b) (a < b ? a : b)

void push_config(struct WireGuardConfigs *root, const char *iface, int n_iface, const char *gateway, int n_gateway)
{
  if (root->count >= root->capacity)
  {
    size_t size_now = root->capacity * sizeof(struct WireGuardInfo);
    root->capacity *= 2;
    root->p = (struct WireGuardInfo *)realloc(root->p, size_now * 2);
  }
  struct WireGuardInfo *p = &root->p[root->count++];
  strncpy(p->iface, iface, MIN(n_iface, sizeof_iface));
  p->iface[sizeof_iface] = '\x00';
  strncpy(p->gateway, gateway, MIN(n_gateway, sizeof_gateway));
  p->gateway[sizeof_gateway] = '\x00';
}

struct WireGuardConfigs *parse_config(const char *path)
{
  FILE *fp_config = fopen(path, "r");
  if (fp_config == NULL)
  {
    return NULL;
  }

  struct WireGuardConfigs *config = (struct WireGuardConfigs *)calloc(1, sizeof(struct WireGuardConfigs));
  config->capacity = 1;
  config->p = (struct WireGuardInfo *)malloc(sizeof(struct WireGuardInfo));

  char line[4096] = {};
  while (fgets(line, sizeof(line), fp_config) != 0)
  {
    const char *p = skip_whitespace(line);

    // Comment
    if (!p || *p == '#')
      continue;

    const char *p_sep = find_whitespace(p);
    int n_iface = p_sep - p;
    if (!p_sep || n_iface > sizeof_field(struct WireGuardInfo, iface))
      continue;

    const char *p_ip_start = skip_whitespace(p_sep);
    if (!p_sep)
      continue;

    const char *p_ip_end = find_whitespace(p_ip_start);
    int n_ip = p_ip_end - p_ip_start;
    if (!p_ip_end || n_ip > sizeof_field(struct WireGuardInfo, gateway))
      continue;

    push_config(config, p, n_iface, p_ip_start, n_ip);
  }
  fclose(fp_config);

  return config;
}
