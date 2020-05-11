#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include "encoding.h"
#include "weak_under_alias.h"
#include "uart.h"

ssize_t __wrap_write(int fd, const void* ptr, size_t len)
{
    uintptr_t satp;
    asm volatile("csrr %0, satp" : "=r"(satp));
#if __riscv_xlen == 64
    if (satp & SATP32_MODE) {
#else
    if (satp & SATP64_MODE) {
#endif
        ssize_t retval;
        asm volatile("mv a0, %1;"
                     "mv a1, %2;"
                     "mv a2, %3;"
                     "li a3, 0;"
                     "li a4, 0;"
                     "li a5, 0;"
                     "li a7, %4;"
                     "ecall;"
                     "mv %0, a0;"
                     : "=r" (retval)
                     : "r" (fd), "r" (ptr), "r" (len), "i" (SYSCALL_WRITE)
                     : "a0", "a1", "a2", "a3", "a4", "a5", "a7");
        return retval;
    } else {
        return do_write(fd, ptr, len);
    }
}
weak_under_alias(write);

ssize_t do_write(int fd, const void* ptr, size_t len)
{
    const uint8_t* current = (const uint8_t*)ptr;

    if (isatty(fd)) {
        for (size_t jj = 0; jj < len; jj++) {
            uart0_txchar(current[jj]);
            if (current[jj] == '\n') {
              uart0_txchar('\r');
            }
        }
        return len;
    }

    return -1;
}