# See LICENSE for license details.

ifndef _SIFIVE_MK_LIBWRAP
_SIFIVE_MK_LIBWRAP := # defined

LIBWRAP_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
LIBWRAP_DIR := $(LIBWRAP_DIR:/=)

LIBWRAP_SRCS := \
	stdlib/malloc.c \
	sys/open.c \
	sys/lseek.c \
	sys/read.c \
	sys/write.c \
	sys/fstat.c \
	sys/stat.c \
	sys/close.c \
	sys/link.c \
	sys/unlink.c \
	sys/execve.c \
	sys/fork.c \
	sys/getpid.c \
	sys/kill.c \
	sys/wait.c \
	sys/isatty.c \
	sys/times.c \
	sys/sbrk.c \
	sys/_exit.c \
	sys/puts.c \
	sys/printf.c \
	misc/write_hex.c

LIBWRAP_SRCS := $(foreach f,$(LIBWRAP_SRCS),$(LIBWRAP_DIR)/$(f))
LIBWRAP_OBJS := $(LIBWRAP_SRCS:.c=.o)

LIBWRAP_SYMS := malloc free \
	open lseek read write fstat stat close link unlink \
	execve fork getpid kill wait \
	isatty times sbrk _exit puts printf

LIBWRAP := $(LIBWRAP_DIR)/libwrap.a

ISP_DEPS += $(LIBWRAP)

ISP_CLEAN += $(LIBWRAP_OBJS) $(LIBWRAP)

all:

$(LIBWRAP_OBJS): %.o: %.c $(ISP_HEADERS)
	$(CC) $(CFLAGS) $(ISP_INCLUDES) -c -o $@ $<

$(LIBWRAP): $(LIBWRAP_OBJS)
	$(AR) rcs $@ $^

endif # _SIFIVE_MK_LIBWRAP
