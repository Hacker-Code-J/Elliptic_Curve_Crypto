#include "secp256r1_bigint.h"

void addition_single(word* epsilon, field dst, const word src1, const word src2) {
	word tmp = *epsilon;
	word sum = src1 + src2;
	*epsilon = (sum < src1); // Set epsilon to 1 if there was a carry-out, otherwise 0
    sum += tmp; // Add epsilon (this will be either 0 or 1)
	*epsilon += (sum < *epsilon); // Adjust epsilon if there was a carry-out from this addition
    *dst = sum; // Set the destination word
}

void addition_core(word* epsilon, field dst, const field src1, const field src2) {
	for(u8 i = 0; i < SIZE; i++)
		addition_single(epsilon, dst + i, src1[i] , src2[i]);
}

void addition_p256(field dst, const field src1, const field src2) {
	word epsilon = 0;
	word buffer[SIZE] = { 0x00, };
	addition_core(&epsilon, buffer, src1, src2);
	if (epsilon) {
		epsilon = 0;
		addition_core(&epsilon, dst, buffer, PRIME_INVERSE);
	} else {
		memcpy(dst, buffer, SIZE * sizeof(word));
	}
}

void subtraction_single(word* epsilon, field dst, const word src1, const word src2) {
	word tmp = src1 - *epsilon;
	*epsilon = (src1 < tmp);
	*epsilon += (tmp < src2);
	tmp -= src2;
	*dst = tmp;
}

void subtraction_core(word* epsilon, field dst, const field src1, const field src2) {
	for(u8 i = 0; i < SIZE; i++)
		subtraction_single(epsilon, dst + i, src1[i] , src2[i]);
}

void subtraction_p256(field dst, const field src1, const field src2) {
	word epsilon = 0;
	word buffer[SIZE] = { 0x00, };
	subtraction_core(&epsilon, buffer, src1, src2);
	if (epsilon) {
		epsilon = 0;
		subtraction_core(&epsilon, dst, buffer, PRIME_INVERSE);
	} else {
		memcpy(dst, buffer, SIZE * sizeof(word));
	}
}

void multiplication_single(field dst[2], const word src1, const word src2) {
	// word = 32 -> half_w = 4 * 4 = 16
	// word = 64 -> half_w = 4 * 8 = 32
	const u8 half_w = 4 * sizeof(word);

	// word = 32 -> MASK = 0x0000FFFF
	// word = 32 -> MASK = 0x00000000FFFFFFFF
	const word MASK = (ONE << half_w) - 1;

	word u0 = src1 & MASK;
	word v0 = src2 & MASK;
	word u1 = src1 >> half_w;
	word v1 = src2 >> half_w;

	// Cross Multiplication
	word t0 = u0 * v1;
	word t1 = u1 * v0;
	t0 += t1;
	t1 = t0 < t1;

	// Direct Multiplication
	word w0 = u0 * v0;
	word w1 = u1 * v1;

	word t = w0;
	w0 += (t0 << half_w);
	w1 += (t1 << half_w) + (t0 >> half_w) + (w0 < t);

	*dst[0] = w0;
	*(dst[0] + 1) = w1;
}

void multiplication_os(field dst[2], const field src1, const field src2) {
	memset(dst, 0, sizeof(field) * 2);
	field tmp[2];
	field buffer[2];

	for (u8 i = 0; i < SIZE; i++) {
		for (u8 j = 0; j < SIZE; j++) {
			memset(tmp, 0, sizeof(field) * 2);
			// printf("i: %u, j: %u\n", i, j);
			// printf("%08X x %08X\n", src1[i], src2[j]);
			multiplication_single(tmp, src1[i], src2[j]);
    		// printData(tmp[1]);
			// printData(tmp[0]);
			// puts("");

			shiftField(tmp, i+j);
			// printData(tmp[1]);
			// printData(tmp[0]);
			// puts("");
			
			word epsilon = 0;
			for (u8 k = 0; k < 2; k++) {
				addition_core(&epsilon, buffer[k], dst[k], tmp[k]);
			}
			memcpy(dst[0], buffer[0], sizeof(field));
    		memcpy(dst[1], buffer[1], sizeof(field));
			// printData(dst[1]);
			// printData(dst[0]);
			// puts("");
		}
	}
}