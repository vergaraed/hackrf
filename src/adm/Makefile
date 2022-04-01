MAK_HOM:=/Users/ed/dev/blockchainbpi/cryptreserve/plugins/hackrf
BLD_DIR := $(MAK_HOM)/build
SRC_DIR := $(MAK_HOM)/src
OBJ_DIR := $(MAK_HOM)/obj
INC_DIR := $(MAK_HOM)/include /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include /Users/ed/.local/lib/gcc/x86_64-apple-darwin21.3.0/12.0.1/include /opt/local/include/libusb-1.0
LIB_DIR := /opt/local/lib $(MAK_HOM)/lib

_SRCS = hackrf_admin.c
SRCS = $(patsubst %,$(SRC_DIR)/%,$(_SRCS))

_DEPS = hackrf_admin.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJS = hackrf_admin.o
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

INC_FLAGS := $(addprefix -I,$(INC_DIR))

LIB_FLAGS := $(addprefix -L,$(LIB_DIR))
LDFLAGS=-lm -lrfftw -lfftw $(LIB_FLAGS)

THREADS=

CC=/Users/ed/.local/bin/gcc
CFLAGS=$(THREADS) -std=c99 -pedantic -Wall -ggdb3 -O0 $(INC_FLAGS)

ifdef DEBUG
CFLAGS += -g3
endif

$(_OBJS): $(_SRCS)
	$(CC) $(CFLAGS) -c $< -o $@ 	

%.o: $(SRCS) 
	$(CC) $(CFLAGS) -c $< -o $@

hackrfadm: $(_OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

all: hackrfadm

.PHONY: clean 
clean:
	rm -f $(OBJ_DIR) 

