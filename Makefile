# TODO: find out how to execute other commands

all:
	$(MAKE) stack
	$(MAKE) exploit
	./exploit

stack: stack.c
	echo "Note: need sudo"
	sudo gcc -o stack -fno-stack-protector stack.c -g
	sudo chmod 4755 stack
	sudo execstack -s stack

exploit: exploit.c
	gcc -o exploit exploit.c

obfuscated: obfuscated.c
	gcc -o obfuscated obfuscated.c

shellcode: shellcode.c
	gcc -o call_shellcode shellcode.c
	execstack -s shellcode

break_env:
	sudo sysctl -w kernel.randomize_va_space=0

fix_env:
	sudo sysctl -w kernel.randomize_va_space=2

clean:
	rm -f ./exploit ./stack ./badfile ./shellcode
