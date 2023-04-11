FILE := template

SRC_DIR := ./
OBJ_DIR := ./obj
BIN_DIR := ./bin

CC_HOME := 
CC_NAME := 

CC  := gcc
CXX := g++
OBJDUMP := $(CC_HOME)$(CC_NAME)objdump
OBJCPY := $(CC_HOME)$(CC_NAME)objcopy
SIZE := $(CC_HOME)$(CC_NAME)size

ifeq ($(OS),Windows_NT)
BUILD_FILE := $(FILE).exe
OBJ_DIR := ./obj/win32
BIN_DIR := ./bin/win32
CC_HOME := 
CC_NAME := gcc
CC_DEF += 
CC_OPT += -O3 -Wall -lSDL2 -lSDL2_image -lSDL2_mixer -lm -lshell32 -luser32 -lkernel32 -I Z:\git\mingw_dev_lib\x86_64-w64-mingw32\include\SDL2 -L Z:\git\mingw_dev_lib\x86_64-w64-mingw32\lib -L C:\\Windows\\System32

CXX_HOME := 
CXX_NAME := g++
CXX_DEF += 
CXX_OPT += -O3
else
BUILD_FILE := $(FILE).elf
OBJ_DIR := ./obj/lnx
BIN_DIR := ./bin/lnx
CC_HOME := 
CC_NAME := gcc
CC_DEF += 
CC_OPT += -g3 -O0 -Wall -lSDL2 -lSDL2_image -lSDL2_mixer -lm

CXX_HOME := 
CXX_NAME := g++
CXX_DEF += 
CXX_OPT += -g3 -O0
endif

CC  := $(CC_HOME)$(CC_NAME)
CXX := $(CXX_HOME)$(CXX_NAME)

LIB_DIR :=lib

# Find .c & .cpp
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# The final build step.
$(BIN_DIR)/$(BUILD_FILE): $(OBJS)
	$(CC) -Wall $(OBJS) $(CC_OPT) $(CC_DEF) -o  $@ $(LDFLAGS)

# Build C source
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) -Wall $(CPPFLAGS) $(CC_OPT) $(CC_DEF) -c $< -o $@ 

# Build C++ source
$(OBJ_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) -Wall $(CPPFLAGS) $(CXX_OPT) $(CXX_DEF) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(OBJ_DIR)/*

# Do not remove
-include $(DEPS)