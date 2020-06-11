#ifndef __HANDLERS_H__
#define __HANDLERS_H__

#include <stdint.h>
#include <unistd.h>

uintptr_t syscall(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, int);

void do_exit(int);
ssize_t do_write(int, const void*, size_t);

#endif // __HANDLERS_H__