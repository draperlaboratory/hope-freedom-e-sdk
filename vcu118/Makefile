BASE_DIR = $(abspath .)

CFLAGS := -DALIGN_UART -DFPGA_GFE

CC 							 := $(ISP_PREFIX)/bin/clang
ifneq ($(ARCH), rv64)
RISCV_ARCH 			 ?= rv32ima
RISCV_ABI        ?= ilp32
CFLAGS += --target=riscv32-unknown-elf
CFLAGS += -DCPU_CLOCK_HZ=50000000
else
RISCV_ARCH ?= rv64imafd
RISCV_ABI ?= lp64d

CFLAGS += --target=riscv64-unknown-elf
CFLAGS += -mcmodel=medany
CFLAGS += -DCPU_CLOCK_HZ=100000000
endif

USE_VM ?= 0
CFLAGS += -O0 -g3 -Wall -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -DUSE_VM=$(USE_VM)

LIB_TARGET = libvcu118.a

CFLAGS += -I$(ISP_PREFIX)/include \
          -I$(BASE_DIR) \
          -I$(BASE_DIR)/wrap \
          -I$(BASE_DIR)/xilinx/common \
          -I$(BASE_DIR)/xilinx/uartns550 \
          -ffunction-sections \
          -fno-builtin-printf

C_SRCS = $(wildcard *.c) $(wildcard wrap/*.c) \
		 $(wildcard xilinx/common/*.c) $(wildcard xilinx/uartns550/*.c)

ASM_SRCS = $(wildcard *.S)

OBJS   = $(patsubst %.c,%.o,$(C_SRCS))
OBJS   += $(patsubst %.S,%.o,$(ASM_SRCS))

all: $(LIB_TARGET)
.PHONY: all clean
.PRECIOUS: $(BUILD_DIR)/ $(BUILD_DIR)%/
.SECONDEXPANSION:

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_TARGET): $(OBJS)
	ar rcs $@ $(OBJS)

clean:
	rm -rf $(OBJS) $(LIB_TARGET)
