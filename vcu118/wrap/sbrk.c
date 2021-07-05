#include <stdint.h>
#include "bsp.h"

#include "weak_under_alias.h"

#define SBRK_REQ 1

extern uintptr_t _heap_end;
static void* program_break = &_heap_end;
static const volatile uintptr_t sbrk_zero = 0;

void* __wrap_sbrk(intptr_t increment)
{
  // If increment is zero, just return the fake program break
  if (increment == 0)
    return program_break;

  // Ensure the new program break always aligns to pointer size
  increment = (increment + sizeof(uintptr_t) - 1)/sizeof(uintptr_t)*sizeof(uintptr_t);

  // Move the fake program break
  AP_MB[1] = (uint32_t)program_break;
  void* temp = program_break;
  program_break += increment;
  AP_MB[2] = (uint32_t)program_break;

  if (increment > 0) {
    // Interrupt the PEX to tell it the AP might want to allocate a new memory region
    AP_MB[0] = SBRK_REQ;
    uint32_t pex = PEX_MB[0];
    *MB_IRQ = 1;
    while (PEX_MB[0] == pex); // Wait for the PEX to finish processing the request
  }

  // Update tags for newly-allocated or deallocated memory
  uintptr_t size = increment > 0 ? increment : -increment;
  volatile uintptr_t* to_tag = (uintptr_t*)(increment > 0 ? temp : program_break);
  for (intptr_t i = 0; i < size/sizeof(uintptr_t); i++)
    to_tag[i] = sbrk_zero;

  return temp;
}

void* __wrap__sbrk(intptr_t increment)
{
  return __wrap_sbrk(increment);
}