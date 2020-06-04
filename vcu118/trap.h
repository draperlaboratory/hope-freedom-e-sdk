#ifndef __TRAP_H__
#define __TRAP_H__

typedef struct
{
  uintptr_t gpr[32];
  uintptr_t sr;
  uintptr_t epc;
  uintptr_t badvaddr;
  uintptr_t cause;
} trapframe_t;

void bad_trap(void);
void machine_pop_tf(trapframe_t*);
void supervisor_pop_tf(trapframe_t*);
void supervisor_trap_entry(void);

#endif // __TRAP_H__