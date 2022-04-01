TARGET_EXEC := hackrf_sweep_svc

BUILD_DIR := ./build
SRC_DIR := ./src
ODIR := ./obj
IDIR := /Users/ed/dev/blockchainbpi/cryptreserve/plugins/hackrf/include

_SRCS = hackrf.c hackrf_info.c hackrf_sweep.c sweepsvc.c
SRCS = $(patsubst %,$(SRC_DIR)/%,$(_SRCS))

_DEPS = hackrf.h hackrf_info.h hackrf_sweep.h sweepsvc.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = hackrf.o hackrf_info.o hackrf_sweep.o sweepsvc.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJ))

INC_DIRS=/Users/ed/dev/blockchainbpi/cryptreserve/plugins/hackrf/include /opt/local/include /opt/local/include/libusb-1.0 /Library/Developer/CommandLineTools/SDKs/MacOSX12.1.sdk/usr/include
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

LIB_DIRS=/Users/ed/.local/lib /usr/lib /usr/local/lib /opt/local/lib
LIB_FLAGS := $(addprefix -L,$(LIB_DIRS))

LDFLAGS=-lm -lrfftw -lfftw $(LIB_FLAGS)
THREADS=-pthread

CC=/Users/ed/.local/bin/gcc
# -Wextra -Werror 
CFLAGS=$(THREADS) -std=c99 -pedantic -Wall -ggdb3 -O0 -I$(IDIR) $(INC_FLAGS)

ifdef DEBUG
CFLAGS += -g3
endif

all: $(BUILD_DIR)/$(TARGET_EXEC)

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/hackrf_info: $(ODIR)/hackrf_info.o $(ODIR)/hackrf.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/hackrf_sweep: $(ODIR)/hackrf_sweep.o $(ODIR)/hackrf.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS)
 
# Build step for C source

$(ODIR)/hackrf.o: $(SRC_DIR)/hackrf.c
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR)/hackrf_info.o: $(SRC_DIR)/hackrf_info.c
	$(CC) $(CFLAGS) -c $< -o $@
 
$(ODIR)/hackrf_sweep.o: $(SRC_DIR)/hackrf_sweep.c
	$(CC) $(CFLAGS) -c $< -o $@
 
$(ODIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
 
.PHONY: clean 
clean:
	rm -f $(ODIR) $(BUILD_DIR)/$(TARGET_EXEC)  $(BUILD_DIR)/hackrf_info $(BUILD_DIR)/hackrf_sweep

