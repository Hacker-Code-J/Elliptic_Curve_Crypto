#include "secp256r1_config.h"
#include "secp256r1_utils.h"

#ifndef _SECP256R1_BIGINT_H
#define _SECP256R1_BIGINT_H

#ifdef IS_32_BIT_ENV
static const field PRIME = {
	0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0x00000000U,
	0x00000000U, 0x00000000U, 0x00000001U, 0xFFFFFFFFU
};
static const field PRIME_INVERSE = {
	0x00000001U, 0x00000000U, 0x00000000U, 0xFFFFFFFFU,
	0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFEU, 0x00000000U
};
#else
static const field PRIME = {
	0xFFFFFFFFFFFFFFFFU, 0x00000000FFFFFFFFU,
	0x0000000000000000U, 0xFFFFFFFF00000001U
};
static const field PRIME_INVERSE = {
	0x0000000000000001U, 0xFFFFFFFF00000000U,
	0xFFFFFFFFFFFFFFFFU, 0x00000000FFFFFFFEU
};
#endif

static inline void shiftField(field dst[2], u8 shiftAmount) {
    if (shiftAmount >= 2 * SIZE) {
        // If the shift amount is twice greater than or equal to SIZE, all data will be shifted out. So, fill with zeros.
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                dst[i][j] = 0;
            }
        }
    } else {
        // For shifts between 7 and 15, inclusive
        if (shiftAmount > 0) {
            // Handle shifting from dst[0] to dst[1]
            if (shiftAmount >= SIZE) {
                for (int j = 0; j < SIZE; ++j) {  // Shift within dst[1]
                    int new_index = j + shiftAmount - SIZE;
                    if (new_index < SIZE) {
                        dst[1][new_index] = dst[0][j];  // Shift from dst[0] to dst[1]
                    }
                }
            } else if (shiftAmount > 0) {  // Normal shifting within dst[0]
                for (int j = SIZE - 1; j >= 0; --j) {
                    int new_index = j + shiftAmount;
                    if (new_index < SIZE) {
                        dst[0][new_index] = dst[0][j];  // Shift within dst[0]
                    } else {
                        dst[1][new_index - SIZE] = dst[0][j];  // Shift from dst[0] to dst[1]
                    }
                }
            }
            // Fill the vacated positions with zeros
            for (int j = 0; j < (SIZE < shiftAmount ? SIZE : shiftAmount); ++j) {
                dst[0][j] = 0;  // Clear moved elements in dst[0]
            }
            if (shiftAmount >= SIZE) {
                for (int j = 0; j < shiftAmount - SIZE; ++j) {
                    dst[1][j] = 0;  // Clear old elements in dst[1] when shiftAmount exceeds SIZE
                }
            }
        }
    }
    // If shiftAmount is 0, do nothing
}

void addition_single(word* epsilon, field dst, const word src1, const word src2);
void addition_core(word* epsilon, field dst, const field src1, const field src2);
void addition_p256(field dst, const field src1, const field src2);

void subtraction_single(word* epsilon, field dst, const word src1, const word src2);
void subtraction_core(word* epsilon, field dst, const field src1, const field src2);
void subtraction_p256(field dst, const field src1, const field src2);

void multiplication_single(field dst[2], const word src1, const word src2);

void multiplication_os(field dst[2], const field src1, const field src2);

#endif /* _SECP256R1_BIGINT_H */