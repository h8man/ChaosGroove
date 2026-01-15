# ==========================================================
# Makefile: Raylib + Auto-Detect Static/Shared Libraries
# ==========================================================

# Compiler
CXX = g++

# Output binary
TARGET = game

# Directories
SRC_DIR = ./
LIB_DIR = linux/raylib/lib
BIN_DIR = bin
OBJ_DIR = bin/obj
INC_DIR = linux/raylib/include

# Build type: Debug or Release
BUILD ?= Release

# Compiler flags
ifeq ($(BUILD),Debug)
    CXXFLAGS = -Wall -std=c++17 -g -I$(INC_DIR)
else
    CXXFLAGS = -O3 -Wall -std=c++17 -I$(INC_DIR)
endif

OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Auto-detect C source files
SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Auto-detect static libs: lib/*.a
STATIC_LIBS = $(wildcard $(LIB_DIR)/*.a)

# Auto-detect dynamic libs: lib/*.so
#DYNAMIC_LIBS = $(wildcard $(LIB_DIR)/*.so)

# Convert lib/libxyz.a → -lxyz
STATIC_LIB_NAMES = $(patsubst $(LIB_DIR)/lib%.a,-l%,$(STATIC_LIBS))

# Convert lib/libxyz.so → -lxyz
DYNAMIC_LIB_NAMES = $(patsubst $(LIB_DIR)/lib%.so,-l%,$(DYNAMIC_LIBS))

# Compiler flags
CXXFLAGS = -Wall -march=x86-64 -std=c++17 -g -D_DEBUG -O0 -I $(INC_DIR)

# Library search paths + rpath for dynamic libs
LDFLAGS = -L $(LIB_DIR) -Wl,-rpath,'$$ORIGIN/../$(LIB_DIR)'

# System libs needed by Raylib
SYS_LIBS = -lGL -lm -lpthread -ldl -lrt -lX11

# Output path
OUT = $(BIN_DIR)/$(TARGET)

# Build
all: $(OUT)

$(OUT): $(SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) \
		$(STATIC_LIB_NAMES) $(SYS_LIBS)

clean:
	rm -rf $(BIN_DIR)

run: all
	./$(OUT)
