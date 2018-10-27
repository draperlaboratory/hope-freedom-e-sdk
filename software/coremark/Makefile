TARGET := coremark

C_SRCS := \
	core_list_join.c \
	core_main.c \
	core_matrix.c \
	core_state.c \
	core_util.c \
	core_portme.c \
	test_wrapper.c \
	test_status.c \
	hifive.c

HEADERS := \
	coremark.h \
	core_portme.h \

CFLAGS := -O2 -fno-common -funroll-loops -finline-functions --param max-inline-insns-auto=20 -falign-functions=4 -falign-jumps=4 -falign-loops=4
CFLAGS += -DFLAGS_STR=\""$(CFLAGS)"\"
#CFLAGS += -DITERATIONS=10000 -DPERFORMANCE_RUN=1
#XXX: hard-coding while debugging
CFLAGS += -DITERATIONS=1 -DPERFORMANCE_RUN=1

BSP_BASE = ../../bsp
include $(BSP_BASE)/env/common.mk
