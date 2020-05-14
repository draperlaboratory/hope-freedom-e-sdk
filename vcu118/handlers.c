#include <stdint.h>
#include "bsp.h"
#include "encoding.h"

typedef struct
{
  uintptr_t gpr[32];
  uintptr_t sr;
  uintptr_t epc;
  uintptr_t badvaddr;
  uintptr_t cause;
} trapframe_t;

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
    tf->gpr[10] = handle_ecall(&tf->gpr[10], tf->gpr[17]);
    tf->epc += 4; // tf->epc points to ecall
    break;
  default:
    bad_trap();
  }
  machine_pop_tf(tf);
}