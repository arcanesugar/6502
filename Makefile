CC:=clang
CFLAGS:=-O0 -g
#adding .bin to the bin names makes them easyer to ignore in gitignore
LINKER_BINNAME:=link.bin

.PHONY:all
all:$(LINKER_BINNAME)

LINKER_OBJ:=$(patsubst %.c, %.o, $(wildcard linker/*.c))

$(LINKER_BINNAME):$(LINKER_OBJ)
	$(CC) $(CFLAGS) $^ -o $(LINKER_BINNAME)

%.o:%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	rm $(LINKER_OBJ)
	rm $(LINKER_BINNAME)
