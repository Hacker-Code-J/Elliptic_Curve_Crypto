#include "secp256r1_bigint.h"

void addition_single(word* epsilon, word* dst, const word src1, const word src2) {
	word sum = src1 + src2;
    *epsilon = (sum < src1); // Set epsilon to 1 if there was a carry-out, otherwise 0
    *epsilon += (sum < *epsilon); // Adjust epsilon if there was a carry-out from this addition
	sum += *epsilon; // Add epsilon (this will be either 0 or 1)
    *dst = sum; // Set the destination word
}

void addition_core(word* epsilon, word* dst, const word* src1, const word* src2) {
	word buffer = 0;
	word carry = 0;
	
	for(u8 i = 0; i < SIZE; i++) {
		addition_single(epsilon, dst + i, src1[i] , src2[i]);
	}
}

void addition_p256(word* dst, const word* src1, const word* src2) {
	word epsilon = 0;
	word buffer[SIZE];

	addition_core(&epsilon, buffer, src1, src2);

    printf("Carry= %d\n", epsilon);
	if (epsilon) {
		epsilon = 0;
		addition_core(&epsilon, dst, buffer, PRIME_INVERSE);
		// for (u8 i = 0; i < SIZE; i++) {
		// 	dst[i] += (PRIME_COMPLEMENT[i]);
		// }
		return;
	}
	memcpy(dst, buffer, SIZE);
}

// 0BB8F3AA 09ACAD09 6DAC72C6 4A2E940D 5F370ED2 5FF1AB7E 1FFE5782 F6AE240C
// 0BB8F3A9 09ACAD09 6DAC72C6 4A2E940D 5F370ED3 5FF1AB7E 1FFE5783 F6AE240C

// 0BB8F3AA 09ACAD0A 6DAC72C6 4A2E940D 5F370ED3 5FF1AB7F 1FFE5784 F6AE240C 

// 0BB8F3A9 09ACAD0A 6DAC72C6 4A2E940D 5F370ED3 5FF1AB7E 1FFE5783 F6AE240B
// 0BB8F3A9 09ACAD08 6DAC72C5 4A2E940C 5F370ED2 5FF1AB7E 1FFE5783 F6AE240B