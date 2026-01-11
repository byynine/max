CC := gcc
EXE :=
RM := rm -f
MKDIR := mkdir -p

SRC = \
    src/max.c \
    src/module/parser/parser.c \
    src/module/cmdopts/cmdopts.c \
    src/module/version/version.c \
    src/module/usage/usage.c

ifeq ($(OS),Windows_NT)
    EXE := .exe
    RM := del
    MKDIR := mkdir
    CC := gcc
	SRC = \
		  srcwin/max.c \
		  src/win/module/parser/parser.c \
		  src/win/module/cmdopts/cmdopts.c \
		  src/win/module/version/version.c \
		  src/win/module/usage/usage.c
endif


BIN_DIR := dist
BIN := $(BIN_DIR)/max$(EXE)

all: $(BIN)

$(BIN): $(SRC) | $(BIN_DIR)
	$(CC) -o $@ $(SRC)

$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)

clean:
	$(RM) $(BIN)
