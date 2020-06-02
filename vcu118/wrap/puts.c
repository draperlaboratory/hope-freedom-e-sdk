#include <unistd.h>
#include <string.h>
#include "weak_under_alias.h"

int __wrap_puts(const char *s)
{
  return write(STDOUT_FILENO, s, strlen(s));
}
weak_under_alias(puts);
