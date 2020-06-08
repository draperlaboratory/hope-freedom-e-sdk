#ifndef __TRAP_H__
#define __TRAP_H__

#include <stdint.h>

typedef struct
{
  uintptr_t gpr[32];
  uintptr_t sr;
  uintptr_t epc;
  uintptr_t badvaddr;
  uintptr_t cause;
} trapframe_t;

#endif // __TRAP_H__