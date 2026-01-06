CC := gcc
EXE :=
RM := rm -f

ifeq ($(OS),Windows_NT)
    EXE := .exe
    RM := del
    CC := x86_64-w64-mingw32-gcc
endif

SRC = \
    src/max.c \
    src/module/parser/parser.c \
    src/module/cmdopts/cmdopts.c \
    src/module/version/version.c \
	src/module/usage/usage.c

BIN = dist/max$(EXE)

all: $(BIN)

$(BIN): $(SRC)
	$(CC) -o $@ $(SRC)

clean:
	$(RM) $(BIN)
