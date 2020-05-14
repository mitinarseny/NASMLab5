#include <stdio.h>
#include <stdlib.h>

void print_int8_binary(int8_t x) {
	static char ZERO = '0';
	printf("0b");
	for (int i = 0; i < 8; x >>= 1, i++) {
		putchar(ZERO + (x & 1));
	}
}

void print_int8_binary_arr(int8_t A[], size_t N) {
	putchar('[');
	for (size_t i = 0; i < N; i++) {
		if (i > 0)
			printf(", ");
		print_int8_binary(A[i]);
	}
	putchar(']');
}

void f(int8_t A[], size_t N) {   
	__asm__ (
		"1:                       # loop: iterate over A\n\t"
		"mov (%[A]), %%al         # AL := A[i]\n\t"       
		"mov %%al, %%bl           # BL := A[i]\n\t"
		"mov %%al, %%cl           # CL := A[i]\n\t"
		"and $0b00000110, %%al    # AL := A[i][2:1]\n\t"
		"and $0b00011000, %%bl    # BL := A[i][4:3]\n\t"
		"shr $2, %%bl             # Bl := A[i][4:3] >> 2\n\t"
		"and $0b01100000, %%cl    # CL := A[i][6:5]\n\t"
		"shr $4, %%cl             # CL := A[i][6:5] >> 4\n\t"
		"or %%bl, %%cl            # CL := (A[i][4:3] >> 2) | (A[i][6:5] >> 4)\n\t"
		"cmp %%al, %%cl           # if (CL != A[i][2:1])\n\t"
		"jne 2f                   #     goto 2\n\t"
		"xorb $0b10000001, (%[A]) # A[i] := A[i] ^ 0b10000001\n"

		"2:\n\t"
		"inc %[A]                 # next A\n\t"
		"inc %[i]                 # next i\n\t"
		"cmp %[N], %[i]           # if (i < N)\n\t"
		"jl 1b                    #     goto 1\n"    
	:: [A] "S" (A),
	[N] "d" (N),
	[i] "r" ((size_t)0)
	: "al", "bl", "cl"
	);
	print_int8_binary_arr(A, N);
}

int main() {
	int8_t AA[][6] = {
		{0},
		{0b01100110, 0b01110010, 0b00011100, 0b01111000, 0b10000001, 0b10101010},
		{0b11100110, 0b01111010, 0b01011100, 0b11111111, 0b11000001, 0b11101110},
		{0b11110000, 0b11110110, 0b00011100, 0b01111011, 0b10100001, 0b11101011},
	};

	for (int i = 0; i < sizeof(AA) / sizeof(*AA); i++) {
		printf("was: ");
		print_int8_binary_arr(AA[i], sizeof(*AA) / sizeof(**AA));
		printf("\nnow: ");
		f(AA[i], sizeof(*AA) / sizeof(**AA));
		printf("\n\n");
    }
}
