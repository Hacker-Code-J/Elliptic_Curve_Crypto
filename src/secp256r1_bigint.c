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

void multiplication_txtbk(field* dst, const field src1, const field src2) {
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
			lshift_field_data(tmp, i+j);
			// printData(tmp[1]);
			// printData(tmp[0]);
			// puts("");
			
			word epsilon = 0;
			addition_core(&epsilon, buffer[0], dst[0], tmp[0]);
			addition_core(&epsilon, buffer[1], dst[1], tmp[1]);
			// for (u8 k = 0; k < 2; k++) {
			// 	addition_core(&epsilon, buffer[k], dst[k], tmp[k]);
			// }
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

void multiplication_imptxtbk(field* dst, const field src1, const field src2) {
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
		lshift_field_data(tmp, j);
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

void multiplication_p256(field dst, const field src1, const field src2) {
	field buffer[2];
	multiplication_imptxtbk(buffer, src1, src2);
	fast_red(dst, buffer);
// 	word* p = &buffer[0][0];

// #ifdef IS_32_BIT_ENV
// 	field s1 = { p[ 0], p[ 1], p[ 2], p[ 3], p[ 4], p[ 5], p[ 6], p[ 7] };
// 	field s2 = { 	 0, 	0, 	   0, p[11], p[12], p[13], p[14], p[15] };
// 	field s3 = { 	 0, 	0, 	   0, p[12], p[13], p[14], p[15], 	  0 };
// 	field s4 = { p[ 8], p[ 9], p[10], 	  0, 	 0, 	0, p[14], p[15] };
// 	field s5 = { p[ 9], p[10], p[11], p[13], p[14], p[15], p[13], p[ 8] };
// 	field s6 = { p[11], p[12], p[13], 	  0, 	 0, 	0, p[ 8], p[10] };
// 	field s7 = { p[12], p[13], p[14], p[15], 	 0, 	0, p[ 9], p[11] };
// 	field s8 = { p[13], p[14], p[15], p[ 8], p[ 9], p[10], 	   0, p[12] };
// 	field s9 = { p[14], p[15], 	   0, p[ 9], p[10], p[11], 	   0, p[13] };
// #else
// 	field s1 = { p[0], p[1], p[2], p[3] };
// 	field s2 = { 0, p[5] & 0xFFFFFFFF00000000ULL, p[6], p[7] };
// 	field s3 = {
// 		0,
// 		p[6] << 32,
// 		((p[7] & 0xFFFFFFFFU) << 32) | (p[6] >> 32),
// 		p[7] >> 32
// 	};
// 	field s4 = { p[4], p[5] & 0x00000000FFFFFFFFULL, 0, p[7] };
// 	field s5 = {
// 		(p[5] << 32) | (p[4] >> 32),
// 		(p[6] & 0xFFFFFFFF00000000ULL) | (p[5] >> 32),
// 		p[7],
// 		((p[4] & 0xFFFFFFFF) << 32) | (p[6] >> 32)
// 	};
// 	field s6 = {
// 		(p[6] << 32) | (p[5] >> 32),
// 		p[6] >> 32,
// 		0,
// 		(p[5] << 32) | (p[4] & 0xFFFFFFFFU)
// 	};
// 	field s7 = {
// 		p[6],
// 		p[7],
// 		0,
// 		(p[5] & 0xFFFFFFFF00000000ULL) | (p[4] >> 32)
// 	};
// 	field s8 = {
// 		(p[7] << 32) | (p[6] >> 32),
// 		(p[4] << 32) | (p[7] >> 32),
// 		(p[5] << 32) | (p[4] >> 32),
// 		p[6] << 32
// 	};
// 	field s9 = {
// 		p[7],
// 		p[4] & 0xFFFFFFFF00000000ULL,
// 		p[5],
// 		p[6] & 0xFFFFFFFF00000000ULL
// 	};
// #endif

// 	memset(dst, 0, sizeof(field));
// 	addition_p256(dst, dst, s1);
// 	addition_p256(dst, dst, s2);
// 	addition_p256(dst, dst, s2);
// 	addition_p256(dst, dst, s3);
// 	addition_p256(dst, dst, s3);
// 	addition_p256(dst, dst, s4);
// 	addition_p256(dst, dst, s5);
// 	subtraction_p256(dst, dst, s6);
// 	subtraction_p256(dst, dst, s7);
// 	subtraction_p256(dst, dst, s8);
// 	subtraction_p256(dst, dst, s9);
}

void multiplication_montgomery_p256(field dst, const field src1, const field src2) {
	field t = { 0, };
	
	for (u8 i = 0; i < SIZE-7; i++) {
		field u = { 0, };
		u[0] = *(word*)t + (src1[i] * src2[0]);
		field _u[2];
		
		multiplication_imptxtbk(_u, u, P256);

		printData(_u[1]);
		printData(_u[0]);
		puts("");

		field v = { 0, };
		v[0] = src1[i];
		field _v[2];
		multiplication_imptxtbk(_v, v, src2);
		
		printData(_v[1]);
		printData(_v[0]);
		puts("");

		field buffer = { 0, };
		word epsilon = 0;
		for(u8 i = 0; i < SIZE; i++)
			addition_single(&epsilon, buffer + (i), src1[i] , src2[i]);

		printData(buffer);
		puts("");
		// word epsilon = 0;
		// word buffer[SIZE] = { 0x00, };
		// addition_core(&epsilon, buffer, src1, src2);
		// if (epsilon) {
		// 	epsilon = 0;
		// 	addition_core(&epsilon, dst, buffer, P256_INVERSE);
		// } else {
		// 	memcpy(dst, buffer, SIZE * sizeof(word));
		// }
	}
}

void fast_red(field dst, const field* src) {
	field buffer[2];
	memcpy(buffer, src, sizeof(field) * 2);
	word* p = &buffer[0][0];
	
	#ifdef IS_32_BIT_ENV
	field s1 = { p[ 0], p[ 1], p[ 2], p[ 3], p[ 4], p[ 5], p[ 6], p[ 7] };
	field s2 = { 	 0, 	0, 	   0, p[11], p[12], p[13], p[14], p[15] };
	field s3 = { 	 0, 	0, 	   0, p[12], p[13], p[14], p[15], 	  0 };
	field s4 = { p[ 8], p[ 9], p[10], 	  0, 	 0, 	0, p[14], p[15] };
	field s5 = { p[ 9], p[10], p[11], p[13], p[14], p[15], p[13], p[ 8] };
	field s6 = { p[11], p[12], p[13], 	  0, 	 0, 	0, p[ 8], p[10] };
	field s7 = { p[12], p[13], p[14], p[15], 	 0, 	0, p[ 9], p[11] };
	field s8 = { p[13], p[14], p[15], p[ 8], p[ 9], p[10], 	   0, p[12] };
	field s9 = { p[14], p[15], 	   0, p[ 9], p[10], p[11], 	   0, p[13] };
#else
	field s1 = { p[0], p[1], p[2], p[3] };
	field s2 = { 0, p[5] & 0xFFFFFFFF00000000ULL, p[6], p[7] };
	field s3 = {
		0,
		p[6] << 32,
		((p[7] & 0xFFFFFFFFU) << 32) | (p[6] >> 32),
		p[7] >> 32
	};
	field s4 = { p[4], p[5] & 0x00000000FFFFFFFFULL, 0, p[7] };
	field s5 = {
		(p[5] << 32) | (p[4] >> 32),
		(p[6] & 0xFFFFFFFF00000000ULL) | (p[5] >> 32),
		p[7],
		((p[4] & 0xFFFFFFFF) << 32) | (p[6] >> 32)
	};
	field s6 = {
		(p[6] << 32) | (p[5] >> 32),
		p[6] >> 32,
		0,
		(p[5] << 32) | (p[4] & 0xFFFFFFFFU)
	};
	field s7 = {
		p[6],
		p[7],
		0,
		(p[5] & 0xFFFFFFFF00000000ULL) | (p[4] >> 32)
	};
	field s8 = {
		(p[7] << 32) | (p[6] >> 32),
		(p[4] << 32) | (p[7] >> 32),
		(p[5] << 32) | (p[4] >> 32),
		p[6] << 32
	};
	field s9 = {
		p[7],
		p[4] & 0xFFFFFFFF00000000ULL,
		p[5],
		p[6] & 0xFFFFFFFF00000000ULL
	};
#endif

	memset(dst, 0, sizeof(field));
	addition_p256(dst, dst, s1);
	addition_p256(dst, dst, s2);
	addition_p256(dst, dst, s2);
	addition_p256(dst, dst, s3);
	addition_p256(dst, dst, s3);
	addition_p256(dst, dst, s4);
	addition_p256(dst, dst, s5);
	subtraction_p256(dst, dst, s6);
	subtraction_p256(dst, dst, s7);
	subtraction_p256(dst, dst, s8);
	subtraction_p256(dst, dst, s9);
}
// void mont_red(field dst, const field* src) {
// 	field buffer[2];
// 	field temp[2];
// 	field temp_buffer[2];
// 	memcpy(temp, src, sizeof(field) * 2);

// 	// for (u8 i = 0; i < SIZE; i++) {
// 	// 	memcpy(temp_buffer, temp, sizeof(field) * 2);
// 	// 	multiplication_imptxtbk(buffer, src[i], P256);
// 	// 	lshift_field_data(buffer, i);
// 	// 	addition_p256(temp, temp_buffer, buffer);
// 	// }
// 	// rshift_field_data(temp, 8);
// }