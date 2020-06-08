#include "encoding.h"
#include "syscall.h"
#include "trap.h"

void machine_pop_tf(trapframe_t*);

static uintptr_t handle_ecall(uintptr_t args[6], int n)
{
  switch (n) {
   case SYSCALL_WRITE:
    return (uintptr_t)(do_write(args[0], (const void*)args[1], args[2]));
   case SYSCALL_EXIT:
    do_exit(args[0]);
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