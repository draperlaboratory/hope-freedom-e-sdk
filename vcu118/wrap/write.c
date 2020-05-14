#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include "encoding.h"
#include "weak_under_alias.h"
#include "uart.h"

ssize_t __wrap_write(int fd, const void* ptr, size_t len)
{
    ssize_t retval;
    asm volatile("mv a0, %1\n"
                 "mv a1, %2\n"
                 "mv a2, %3\n"
                 "li a3, 0\n"
                 "li a4, 0\n"
                 "li a5, 0\n"
                 "li a7, %4\n"
                 "ecall\n"
                 "mv %0, a0\n"
                 : "=r" (retval)
                 : "r" (fd), "r" (ptr), "r" (len), "i" (SYSCALL_WRITE)
                 : "a0", "a1", "a2", "a3", "a4", "a5", "a7");
    return retval;
}
weak_under_alias(write);