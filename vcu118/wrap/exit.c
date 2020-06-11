#include "encoding.h"
#include "syscall.h"
#include "weak_under_alias.h"

void __wrap_exit(int status)
{
    syscall(status, 0, 0, 0, 0, 0, 0, SYSCALL_EXIT);
}
weak_under_alias(exit);