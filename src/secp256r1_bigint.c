#include "secp256r1_bigint.h"

void addition_single(word* epsilon, word* dst, const word src1, const word src2) {
	word tmp = *epsilon;
	word sum = src1 + src2;
	*epsilon = (sum < src1); // Set epsilon to 1 if there was a carry-out, otherwise 0
    sum += tmp; // Add epsilon (this will be either 0 or 1)
	*epsilon += (sum < *epsilon); // Adjust epsilon if there was a carry-out from this addition
    *dst = sum; // Set the destination word
}

void addition_core(word* epsilon, word* dst, const word* src1, const word* src2) {
	for(u8 i = 0; i < SIZE; i++) {
		addition_single(epsilon, dst + i, src1[i] , src2[i]);
	}
}

void addition_p256(word* dst, const word* src1, const word* src2) {
	word epsilon = 0;
	word buffer[SIZE];
	addition_core(&epsilon, buffer, src1, src2);
	if (epsilon) {
		epsilon = 0;
		addition_core(&epsilon, dst, buffer, PRIME_INVERSE);
		return;
	}
	memcpy(dst, buffer, SIZE * 4);
}