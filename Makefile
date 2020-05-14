.PHONY: build
build: lab5

lab5: main.c
	gcc --std=c11 -Wall -O0 -o $@ -g $^

.PHONY: asm
asm: main.c
	$(CC) $(CFLAGS) -S -o /dev/stdout $^ | less
