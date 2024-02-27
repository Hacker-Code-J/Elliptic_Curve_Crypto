#include "secp256r1_bigint.h"

void addition_single(word* epsilon, word* dst, const word src1, const word src2) {
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
		addition_core(&epsilon, dst, buffer, P256_INVERSE);
	} else {
		memcpy(dst, buffer, SIZE * sizeof(word));
	}
}

void subtraction_single(word* epsilon, word* dst, const word src1, const word src2) {
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
		subtraction_core(&epsilon, dst, buffer, P256_INVERSE);
	} else {
		memcpy(dst, buffer, SIZE * sizeof(word));
	}
}

void multiplication_single(word* dst, const word src1, const word src2) {
	// word = 32 -> half_w = 4 * 4 = 16
	// word = 64 -> half_w = 4 * 8 = 32
	const u8 half_w = 4 * sizeof(word);

	// word = 32 -> MASK = 0x0000FFFF
	// word = 64 -> MASK = 0x00000000FFFFFFFF
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

	*(dst + 0) = w0;
	*(dst + 1) = w1;
	// **(dst + 0) = w0;
	// *(*(dst + 0) + 1) = w1;
}

void multiplication_ps(field* dst, const field src1, const field src2) {
	memset(dst, 0, sizeof(field) * 2);
	field tmp[2];
	field buffer[2];

	for (u8 i = 0; i < SIZE; i++) {
		for (u8 j = 0; j < SIZE; j++) {
			memset(tmp, 0, sizeof(field) * 2);
			// printf("i: %u, j: %u\n", i, j);
			// printf("%08X x %08X\n", src1[i], src2[j]);
			multiplication_single((word*)tmp, src1[i], src2[j]);
    		// printData(tmp[1]);
			// printData(tmp[0]);
			// puts("");

			// shiftField(tmp, i+j);
			shift_field_data(tmp, i+j);
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

/*
x7 || x6 || x5 || x4 || x3 || x2 || x1 || x0
y7 || y6 || y5 || y4 || y3 || y2 || y1 || y0

for j = 0
	for i = 0;
	T0 = x0y0 		 (T0 in 2^{2w})
	T1 = x1y0 || 0^w (T1 in 2^{3w})
	for i = 1;
	T0 = x2y0 		 (T0 in 2^{2w})
	T1 = x3y0 || 0^w (T1 in 2^{3w})
	for i = 2;
	T0 = x4y0 		 (T0 in 2^{2w})
	T1 = x5y0 || 0^w (T1 in 2^{3w})
	for i = 3;
	T0 = x6y0 		 (T0 in 2^{2w})
	T1 = x7y0 || 0^w (T1 in 2^{3w})
	
	T <- T0 + T1
	T << w * 0
	Z += T

for j = 1
	for i = 0;
	T0 = x0y1 		 (T0 in 2^{2w})
	T1 = x1y1 || 0^w (T1 in 2^{3w})
	for i = 1;
	T0 = x2y1 		 (T0 in 2^{2w})
	T1 = x3y1 || 0^w (T1 in 2^{3w})
	for i = 2;
	T0 = x4y1 		 (T0 in 2^{2w})
	T1 = x5y1 || 0^w (T1 in 2^{3w})
	for i = 3;
	T0 = x6y1 		 (T0 in 2^{2w})
	T1 = x7y1 || 0^w (T1 in 2^{3w})
	
	T <- T0 + T1
	T << w * 1
	Z += T
*/

void multiplication_ps2(field* dst, const field src1, const field src2) {
	memset(dst, 0, sizeof(field) * 2);
	
	field buffer[2];
	memset(buffer, 0, sizeof(field) * 2);

	for (u8 j = 0; j < SIZE; j++) {
		field tmp[2], t0[2], t1[2];
		memset(tmp, 0, sizeof(field) * 2);
		memset(t0 , 0, sizeof(field) * 2);
		memset(t1 , 0, sizeof(field) * 2);
		for (u8 i = 0; i < SIZE / 2; i++) {
			word* p0 = &t0[0][0];
			word* p1 = &t1[0][0];
			// printf("\nj: %u, i: %u\n", j, i);
			// printf("t0: %08X x %08X\n", src1[2 * i    ], src2[j]);
			// printf("t1: %08X x %08X\n", src1[2 * i + 1], src2[j]);
			multiplication_single((p0 + 2 * i    ), src1[2 * i    ], src2[j]);
			multiplication_single((p1 + 2 * i + 1), src1[2 * i + 1], src2[j]);
		// 	printf("t0: \n"); printData(t0[1]); printData(t0[0]);
		// 	printf("t1: \n"); printData(t1[1]); printData(t1[0]);
		}
		word epsilon = 0;
		addition_core(&epsilon, tmp[0], t0[0], t1[0]);
		addition_core(&epsilon, tmp[1], t0[1], t1[1]);
		// printf("t0+t1: \n");
		// printData(tmp[1]);
		// printData(tmp[0]);
		// printf("T << %d: \n", j);
		shift_field_data(tmp, j);
		if (epsilon) {
			*(*(tmp+1)+j) += 1;
		}
		// printData(tmp[1]);
		// printData(tmp[0]);

		// printf("Before dst:\n");
		// printData(dst[1]);
		// printData(dst[0]);
		
		epsilon = 0;
		addition_core(&epsilon, dst[0], tmp[0], *(buffer + 0));
		addition_core(&epsilon, dst[1], tmp[1], *(buffer + 1));
		
		// printf(" After dst:\n");
		// printData(dst[1]);
		// printData(dst[0]);
		memcpy(buffer[0], dst[0], sizeof(field));
		memcpy(buffer[1], dst[1], sizeof(field));	
	}

	// for (u8 i = 0; i < SIZE; i++) {
	// 	for (u8 j = 0; j < SIZE; j++) {
	// 		memset(tmp, 0, sizeof(field) * 2);
	// 		multiplication_single(tmp, src1[i], src2[j]);
    // 		shiftField(tmp, i+j);
	// 		word epsilon = 0;
	// 		for (u8 k = 0; k < 2; k++) {
	// 			addition_core(&epsilon, buffer[k], dst[k], tmp[k]);
	// 		}
	// 		memcpy(dst[0], buffer[0], sizeof(field));
    // 		memtmpcpy(dst[1], buffer[1], sizeof(field));
	// 	}
	// }
}
#if 0
void squaring_single(word* dst, const word src) {
	// word = 32 -> half_w = 4 * 4 = 16
	// word = 64 -> half_w = 4 * 8 = 32
	const u8 half_w = 4 * sizeof(word);

	// word = 32 -> MASK = 0x0000FFFF
	// word = 64 -> MASK = 0x00000000FFFFFFFF
	const word MASK = (ONE << half_w) - 1;

	word u0 = src & MASK;
	word u1 = src >> half_w;

	word z0 = u0 * u0;
	word z1 = u1 * u1;
	
	*(dst + 0) = z0;
	*(dst + 1) = z1;

	word tmp = u0 * u1;
}
#endif