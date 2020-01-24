#include "time.h"
#include "bsp.h"

/**
 * Capture the current 64-bit cycle count.
 */
uint64_t get_cycle_count(void)
{
#if __riscv_xlen == 64
	uint64_t mcycle;
	asm volatile(
		"%=:\n\t"
		"csrr %0, mcycle"
		: "=r"(mcycle));
  return mcycle;
#else
	uint32_t cycle_lo, cycle_hi;
	asm volatile(
		"%=:\n\t"
		"csrr %1, mcycleh\n\t"
		"csrr %0, mcycle\n\t"
		"csrr t1, mcycleh\n\t"
		"bne  %1, t1, %=b"
		: "=r"(cycle_lo), "=r"(cycle_hi)
		: // No inputs.
		: "t1");
	return (((uint64_t)cycle_hi) << 32) | (uint64_t)cycle_lo;
#endif
}

/**
 * Use `mcycle` counter to get usec resolution.
 * On RV32 only, reads of the mcycle CSR return the low 32 bits,
 * while reads of the mcycleh CSR return bits 63–32 of the corresponding
 * counter.
 * We convert the 64-bit read into usec. The counter overflows in roughly an hour
 * and 20 minutes. Probably not a big issue though.
 * At 50HMz clock rate, 1 us = 50 ticks
 */
uint32_t get_current_mtime_usec(void)
{
	return (uint32_t)(get_cycle_count() / (CPU_CLOCK_HZ / 1000000));
}
