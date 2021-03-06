// See LICENSE for license details.
#include <sifive/smp.h>
#include <encoding.h>

/* This is defined in sifive/platform.h, but that can't be included from
 * assembly. */
#define CLINT_CTRL_ADDR 0x02000000

	.section .init
	.globl _start
	.type _start,@function

_start:
	.cfi_startproc
	.cfi_undefined ra
.option push
.option norelax
1:
	//la gp, _global_pointer$
	auipc gp, %pcrel_hi(__global_pointer$)
	addi gp, gp, %pcrel_lo(1b)
.option pop
	//la sp, _sp
1:
	auipc sp, %pcrel_hi(_sp)
	addi sp, sp, %pcrel_lo(1b)

#if defined(ENABLE_SMP)
	smp_pause(t0, t1)
#endif

	/* Load data section */
	//la a0, _data_lma
1:
	auipc a0, %pcrel_hi(_data_lma)
	addi a0, a0, %pcrel_lo(1b)
	//la a1, _data
1:
	auipc a1, %pcrel_hi(_data)
	addi a1, a1, %pcrel_lo(1b)
	//la a2, _edata
1:
	auipc a2, %pcrel_hi(_edata)
	addi a2, a2, %pcrel_lo(1b)
	bgeu a1, a2, 2f
1:
	lw t0, 0(a0)
	sw t0, 0(a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
2:

	/* Clear bss section */
	//la a0, __bss_start
1:
	auipc a0, %pcrel_hi(__bss_start)
	addi a0, a0, %pcrel_lo(1b)
	//la a1, _end
1:
	auipc a1, %pcrel_hi(_end)
	addi a1, a1, %pcrel_lo(1b)
	bgeu a0, a1, 2f
1:
	sw zero, 0(a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:

	/* Call global constructors */
	//la a0, __libc_fini_array
1:
/*
	auipc a0, %pcrel_hi(__libc_fini_array)
	addi a0, a0, %pcrel_lo(1b)
	call atexit
	call __libc_init_array
*/

#ifndef __riscv_float_abi_soft
	/* Enable FPU */
	li t0, MSTATUS_FS
	csrs mstatus, t0
	csrr t1, mstatus
	and t1, t1, t0
	beqz t1, 1f
	fssr x0
1:
#endif

#if defined(ENABLE_SMP)
	smp_resume(t0, t1)

	csrr a0, mhartid
	bnez a0, 2f
#endif

	auipc ra, 0
	addi sp, sp, -16
#if __riscv_xlen == 32
	sw ra, 8(sp)
#else
	sd ra, 8(sp)
#endif

	/* argc = argv = 0 */
	li a0, 0
	li a1, 0
	call main
	tail exit
1:
	j 1b

#if defined(ENABLE_SMP)
2:
	la t0, trap_entry
	csrw mtvec, t0

	csrr a0, mhartid
	la t1, _sp
	slli t0, a0, 10
	sub sp, t1, t0

	auipc ra, 0
	addi sp, sp, -16
#if __riscv_xlen == 32
	sw ra, 8(sp)
#else
	sd ra, 8(sp)
#endif

	call secondary_main
	tail exit

1:
	j 1b
#endif
	.cfi_endproc
