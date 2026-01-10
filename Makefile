CC := gcc
EXE :=
RM := rm -f
MKDIR := mkdir -p

ifeq ($(OS),Windows_NT)
    EXE := .exe
    RM := del
    MKDIR := mkdir
    CC := x86_64-w64-mingw32-gcc
endif

SRC = \
    src/max.c \
    src/module/parser/parser.c \
    src/module/cmdopts/cmdopts.c \
    src/module/version/version.c \
    src/module/usage/usage.c

BIN_DIR := dist
BIN := $(BIN_DIR)/max$(EXE)

all: $(BIN)

$(BIN): $(SRC) | $(BIN_DIR)
	$(CC) -o $@ $(SRC)

$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)

clean:
	$(RM) $(BIN)
