# See LICENSE for license details.

ifndef _SIFIVE_MK_COMMON
_SIFIVE_MK_COMMON := # defined

include $(BSP_BASE)/libwrap/libwrap.mk

all:

ENV_DIR = $(BSP_BASE)/env
PLATFORM_DIR = $(ENV_DIR)/$(BOARD)

ASM_SRCS += $(ENV_DIR)/start.S
ASM_SRCS += $(ENV_DIR)/entry.S
C_SRCS += $(PLATFORM_DIR)/init.c

LINKER_SCRIPT := $(PLATFORM_DIR)/$(LINK_TARGET).lds

ISP_INCLUDES += -I$(BSP_BASE)/include
ISP_INCLUDES += -I$(BSP_BASE)/drivers/
ISP_INCLUDES += -I$(ENV_DIR)
ISP_INCLUDES += -I$(PLATFORM_DIR)

TOOL_DIR = $(BSP_BASE)/../toolchain/bin

ISP_LDFLAGS += -T $(LINKER_SCRIPT) -nostartfiles
ISP_LDFLAGS += -L$(ENV_DIR)

ASM_OBJS := $(ASM_SRCS:.S=.o)
C_OBJS := $(C_SRCS:.c=.o)

ISP_OBJECTS += $(ASM_OBJS) $(C_OBJS)
ISP_DEPS += $(LINKER_SCRIPT)

ISP_CLEAN += $(ISP_OBJECTS)

ISP_CFLAGS += -g
ISP_CFLAGS += -march=$(RISCV_ARCH)
ISP_CFLAGS += -mabi=$(RISCV_ABI)

$(ASM_OBJS): %.o: %.S $(ISP_HEADERS)
	$(CC) $(ISP_CFLAGS) $(ISP_INCLUDES) -c -o $@ $<

$(C_OBJS): %.o: %.c $(ISP_HEADERS)
	$(CC) $(ISP_CFLAGS) $(ISP_INCLUDES) -c -o $@ $<

endif # _SIFIVE_MK_COMMON
