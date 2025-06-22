ASM:=./vasm
ASMFLAGS:= -Fbin -dotdir

CC:=clang
CFLAGS:= -O1

hello-world:hello-world.s
	$(ASM) $(ASMFLAGS) hello-world.s -o hello-world.bin

prep: squish.o
	$(CC) $(CFLAGS) $^ -o prep

%.o:%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@
