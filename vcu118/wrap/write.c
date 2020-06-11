#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include "encoding.h"
#include "weak_under_alias.h"
#include "uart.h"

ssize_t __wrap_write(int fd, const void* ptr, size_t len)
{
    return syscall(fd, (uintptr_t)ptr, len, 0, 0, 0, 0, SYSCALL_WRITE);
}
weak_under_alias(write);