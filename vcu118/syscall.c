#include <stdint.h>
#include <unistd.h>
#include "encoding.h"
#include "trap.h"
#include "uart.h"

extern volatile uint64_t tohost;
extern volatile uint64_t fromhost;

static void do_tohost(uint64_t tohost_value)
{
  while (tohost)
    fromhost = 0;
  tohost = tohost_value;
}

void do_exit(int code)
{
  do_tohost((code << 1) | 1);
  while (1);
}

ssize_t do_write(int fd, const void* ptr, size_t len)
{
  const uint8_t* current = (const uint8_t*)ptr;

  if (isatty(fd)) {
    for (size_t jj = 0; jj < len; jj++) {
      uart0_txchar(current[jj]);
      if (current[jj] == '\n') {
        uart0_txchar('\r');
      }
    }
    return len;
  }

  return -1;
}