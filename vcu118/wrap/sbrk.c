#include <stdint.h>

#include "weak_under_alias.h"

extern uintptr_t _heap_end;
static void* program_break = &_heap_end;
static uint32_t sbrk_zero = 0;

void* __wrap_sbrk(intptr_t increment)
{
  // If increment is zero, just return the fake program break
  if (increment == 0)
    return program_break;

  // Ensure the new program break always aligns to pointer size
  increment = (increment + sizeof(uintptr_t) - 1)/sizeof(uintptr_t)*sizeof(uintptr_t);

  // Move the fake program break
  void* temp = program_break;
  program_break += increment;

  // Update tags for newly-allocated or deallocated memory
  uintptr_t size = increment > 0 ? increment : -increment;
  uint32_t* to_tag = (uint32_t*)(increment > 0 ? temp : program_break);
  for (intptr_t i = 0; i < size/sizeof(uint32_t); i++)
    to_tag[i] = sbrk_zero;

  return temp;
}

void* __wrap__sbrk(intptr_t increment)
{
  return __wrap_sbrk(increment);
}