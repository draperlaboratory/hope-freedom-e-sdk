#include <stdint.h>

#include "weak_under_alias.h"

extern uintptr_t _heap_end;
static void* program_break = &_heap_end;

void* __wrap_sbrk(intptr_t increment)
{
  void* temp = program_break;
  program_break += increment;
  return temp;
}

void* __wrap__sbrk(intptr_t increment)
{
  return __wrap_sbrk(increment);
}