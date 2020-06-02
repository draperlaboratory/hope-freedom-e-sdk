#include <stdint.h>
#include <unistd.h>
#include "encoding.h"
#include "trap.h"
#include "uart.h"

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

uintptr_t handle_ecall(uintptr_t args[6], int n)
{
  switch (n) {
  case SYSCALL_WRITE:
    return (uintptr_t)(do_write(args[0], (const void*)args[1], (size_t)args[2]));
  }
}

void handle_machine_trap(trapframe_t* tf)
{
  switch (tf->cause) {
  case CAUSE_SUPERVISOR_ECALL:
  case CAUSE_MACHINE_ECALL:
    tf->gpr[10] = handle_ecall(&tf->gpr[10], tf->gpr[17]);
    tf->epc += 4; // tf->epc points to ecall
    break;
  default:
    bad_trap();
  }
  machine_pop_tf(tf);
}