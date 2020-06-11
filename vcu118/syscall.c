#include <stdint.h>
#include <unistd.h>
#include "encoding.h"
#include "trap.h"
#include "uart.h"

uintptr_t syscall(uintptr_t arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3, uintptr_t arg4, uintptr_t arg5, uintptr_t arg6, int n)
{
    uintptr_t retval;
    asm volatile("ecall; mv %0, a0" : "=r" (retval));
    return retval;
}

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