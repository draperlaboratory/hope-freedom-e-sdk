#include <stdio.h>
#include "encoding.h"
#include "syscall.h"
#include "trap.h"

void machine_pop_tf(trapframe_t*);

void bad_trap()
{
    uintptr_t mcause, mepc, mtval;
    asm volatile("csrr %0, mcause;"
                 "csrr %1, mepc;"
                 "csrr %2, mtval;"
                 : "=r" (mcause), "=r" (mepc), "=r" (mtval));

    // Can't use printf here because can't use ecall in an M-mode trap handler
    char buf[64];
    snprintf(buf, sizeof(buf), "Bad trap %x at address %p: %x\n", mcause, mepc, mtval);
    do_write(1, buf, strlen(buf));
    do_exit(mcause);
}

static uintptr_t handle_ecall(uintptr_t args[7], int n)
{
  switch (n) {
   case SYSCALL_WRITE:
    return (uintptr_t)(do_write(args[0], (const void*)args[1], args[2]));
   case SYSCALL_EXIT:
    do_exit(args[0]);
   default:
    asm volatile("csrw mtval, %0" :: "r"(n));
    bad_trap();
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