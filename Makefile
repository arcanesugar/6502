ASM:=./vasm
ASMFLAGS:= -Fbin -dotdir

CC:=clang
CFLAGS:= -O1

.PHONY: hello-world 
hello-world:hello-world.s Makefile 
	$(ASM) $(ASMFLAGS) hello-world.s -o hello-world.bin

prep: squish.o
	$(CC) $(CFLAGS) $^ -o prep

%.o:%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.sqsh
	rm -f *.sqsh.*
