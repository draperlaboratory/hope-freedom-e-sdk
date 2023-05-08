ISP_PREFIX ?= $(HOME)/.local/isp

BASE_DIR = $(abspath .)
INSTALL_DIR ?= $(ISP_PREFIX)/bsp/ssith-p1/ap

CFLAGS := -DALIGN_UART -DFPGA_GFE

CC := $(ISP_PREFIX)/bin/clang

RISCV_ARCH ?= rv32ima
RISCV_ABI ?= ilp32

CFLAGS += --target=riscv32-unknown-elf
CFLAGS += -DCPU_CLOCK_HZ=50000000
CFLAGS += -DRV32
CFLAGS += -O0 -g3 -Wall -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI)

LIB_TARGET = libbsp.a

CFLAGS += -I$(ISP_PREFIX)/include -I$(ISP_PREFIX)/local/include -I$(ISP_PREFIX)/bsp/ssith-p2/ap/include \
          -I$(BASE_DIR) \
          -I$(BASE_DIR)/wrap \
          -ffunction-sections \
          -fno-builtin-printf

C_SRCS = init.c \
				 uart.c \
				 wrap/isatty.c \
				 wrap/malloc.c \
				 wrap/printf.c \
				 wrap/puts.c \
				 wrap/read.c \
				 wrap/write.c

ASM_SRCS = boot.S

OBJS   = $(patsubst %.c,%.o,$(C_SRCS))
OBJS   += $(patsubst %.S,%.o,$(ASM_SRCS))

.PHONY: all clean install

all: $(LIB_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_TARGET): $(OBJS)
	ar rcs $@ $(OBJS)

install: $(LIB_TARGET) link.ld
	mkdir -p $(INSTALL_DIR)/lib
	cp $(LIB_TARGET) $(INSTALL_DIR)/lib
	cp link.ld $(INSTALL_DIR)

clean:
	rm -rf $(OBJS) $(LIB_TARGET)
