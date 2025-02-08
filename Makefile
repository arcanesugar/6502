CC:=clang
CFLAGS:=-O0 -g
ASM:=./vasm6502
ASMFLAGS:= -dotdir -Fbin

#adding .bin to the bin names makes them easier to ignore in gitignore
COMPILER_BINNAME:=comp.bin

#.out files are bianarys for the 6502
ASM_BINNAMES:=$(patsubst %.s, %.out, $(wildcard ./*.s))

.PHONY:all
all:compiler assembly

.PHONY:assembly
assembly:$(ASM_BINNAMES)

.PHONY:compiler
compiler:$(COMPILER_BINNAME)

COMPILER_OBJ:=$(patsubst %.c, %.o, $(wildcard compiler/*.c))
$(COMPILER_BINNAME):$(COMPILER_OBJ)
	$(CC) $(CFLAGS) $^ -o $(COMPILER_BINNAME)

%.o:%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

%.out:%.s Makefile
	$(ASM) $(ASMFLAGS) -o $@ $< 

.PHONY:clean
clean:
	rm $(COMPILER_OBJ)
	rm $(COMPILER_BINNAME)
	rm $(ASM_BINNAMES)
