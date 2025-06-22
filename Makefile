ASM:=./vasm
ASMFLAGS:= -Fbin -dotdir

hello-world:
	$(ASM) $(ASMFLAGS) hello-world.s -o hello-world.bin
