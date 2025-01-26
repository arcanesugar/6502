CC:=clang
CFLAGS:=-O0 -g
#adding .bin to the bin names makes them easyer to ignore in gitignore
COMPILER_BINNAME:=comp.bin

.PHONY:all
all:$(COMPILER_BINNAME)

COMPILER_OBJ:=$(patsubst %.c, %.o, $(wildcard compiler/*.c))

$(COMPILER_BINNAME):$(COMPILER_OBJ)
	$(CC) $(CFLAGS) $^ -o $(COMPILER_BINNAME)

%.o:%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	rm $(COMPILER_OBJ)
	rm $(COMPILER_BINNAME)
