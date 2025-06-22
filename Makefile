ASM:=./vasm
ASMFLAGS:= -Fbin -dotdir

CC:=clang
CFLAGS:= -O1

.PHONY: hello-world 
hello-world:hello-world.s Makefile prep
	./prep hello-world.s -o hello-world.prep
	$(ASM) $(ASMFLAGS) hello-world.prep -o hello-world.bin

prep: squish.o
	$(CC) $(CFLAGS) $^ -o prep

%.o:%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.prep

# also removes binarys
.PHONY: deep_clean 
deep_clean:clean
	rm -f prep
	rm -f *.bin
