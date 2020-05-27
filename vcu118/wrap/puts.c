#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include "uart.h"
#include "weak_under_alias.h"

int __wrap_puts(const char *s)
{
  char* str = s;
  int len = 0;
  while (*str != '\0') {
    len++;
    str++;
  }
  return write(STDOUT_FILENO, s, len);
}
weak_under_alias(puts);
