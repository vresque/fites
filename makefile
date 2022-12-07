CC = gcc
LD = gcc
PREFIX=/usr/bin

CFLAGS = -O3 -I./include -Wall -Wextra -pedantic -std=c2x -Werror -Wno-sign-compare
LDFLAGS = 

SRCDIR := sources
BUILD_DIR := build
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

OUT = $(BUILD_DIR)/fites

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.c)          
OBJS = $(patsubst $(SRCDIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
DIRS = $(wildcard $(SRCDIR)/*)

build: $(OBJS) link

$(BUILD_DIR)/%.o: $(SRCDIR)/%.c
	@ echo Compiling $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

link:
	@ echo Linking...
	$(LD) $(LDFLAGS) -o $(OUT) $(OBJS)

clean:
	rm $(OBJS) $(OUT)

install: clean build
	cp $(OUT) $(PREFIX)/fites