CC:=clang
CFLAGS:=-O0 -g

ASM:=./vasm6502
ASMFLAGS:= -dotdir -Fbin
ASM_PROGRAMS := hello-world.s

#unnamed compiler programs 
COMPILER_BINNAME:=comp.bin
UC:=./$(COMPILER_BINNAME)
UC_PROGRAMS:= source.txt


#-dont modify anything below here-
#adding .bin to the bin names makes them easier to ignore in gitignore

#.out files are bianarys for the 6502
ASM_BINNAMES:=$(patsubst %.s, %.out, $(ASM_PROGRAMS))
UC_BINNAMES:=$(patsubst %.txt, %.out, $(UC_PROGRAMS))
COMPILER_OBJ:=$(patsubst %.c, %.o, $(wildcard compiler/*.c))

.PHONY:all
all: assembly compiler unnamed

.PHONY:assembly
assembly:$(ASM_BINNAMES)

.PHONY:compiler
compiler:$(COMPILER_BINNAME)

.PHONY:unnamed
unnamed:$(UC_BINNAMES)

$(COMPILER_BINNAME):$(COMPILER_OBJ)
	$(CC) $(CFLAGS) $^ -o $(COMPILER_BINNAME)

%.o:%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

%.s:%.txt $(COMPILER_BINNAME) Makefile
	$(UC)

%.out:%.s Makefile
	$(ASM) $(ASMFLAGS) -o $@ $< 

.PHONY:clean
clean:
	rm $(COMPILER_BINNAME)
	rm $(COMPILER_OBJ)
	rm $(ASM_BINNAMES)
	rm $(UC_BINNAMES)
