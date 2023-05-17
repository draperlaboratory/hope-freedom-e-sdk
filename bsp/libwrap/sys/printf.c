/* See LICENSE of license details. */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#include "platform.h"
#include "stub.h"
#include "weak_under_alias.h"

extern int __wrap_puts(const char *s);

int __wrap_printf(const char *s, ...)
{
    char buf[256];
    va_list vl;

    const char *p = &buf[0];

    va_start(vl, s);
    vsnprintf(buf, sizeof buf, s, vl);
    va_end(vl);

    __wrap_puts(p);

    return 0;
}
weak_under_alias(printf);
