#include "secp256r1_config.h"
#include "secp256r1_utils.h"

#ifndef _SECP256R1_BIGINT_H
#define _SECP256R1_BIGINT_H

#ifdef IS_32_BIT_ENV
// 0x ffffffff 00000001 00000000 00000000 00000000 ffffffff ffffffff ffffffff
static const field P256 = {
	0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0x00000000U,
	0x00000000U, 0x00000000U, 0x00000001U, 0xFFFFFFFFU
};
// 0x 00000000 fffffffe ffffffff ffffffff ffffffff 00000000 00000000 00000001
static const field P256_INVERSE = {
	0x00000001U, 0x00000000U, 0x00000000U, 0xFFFFFFFFU,
	0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFEU, 0x00000000U
};
// 0x fffffffe 00000003 fffffffd 00000002 00000001 fffffffe 00000003 00000000
static const field MONT_RADIX_INVERSE = {
    0x00000000U, 0x00000003U, 0xFFFFFFFEU, 0x00000001U,
    0x00000002U, 0xFFFFFFFDU, 0x00000003U, 0xFFFFFFFEU
};
#else
// 0x ffffffff00000001 0000000000000000 00000000ffffffff ffffffffffffffff
static const field P256 = {
	0xFFFFFFFFFFFFFFFFU, 0x00000000FFFFFFFFU,
	0x0000000000000000U, 0xFFFFFFFF00000001U
};
// 0x 00000000fffffffe ffffffffffffffff ffffffff00000000 0000000000000001
static const field P256_INVERSE = {
	0x0000000000000001U, 0xFFFFFFFF00000000U,
	0xFFFFFFFFFFFFFFFFU, 0x00000000FFFFFFFEU
};
// 0x fffffffe00000003 fffffffd00000002 00000001fffffffe 0000000300000000
static const field MONT_RADIX_INVERSE = {
    0x0000000300000000U, 0x00000001FFFFFFFEU,
    0xFFFFFFFD00000002U, 0xFFFFFFFE00000003U
};
#endif

static inline void shift_field_data(field data[2], u32 n) {
    word temp[SIZE * 2]; // Temporary storage for both fields combined

    // Calculate actual shift amount within the total size of the two fields
    n = n % (SIZE * 2); // Ensure n is within bounds [0, SIZE * 2)

    // If n is 0, no need to shift
    if (n == 0) {
        return;
    }

    // Backup the entire data structure
    memcpy(temp, data[0], SIZE * sizeof(word)); // Backup first field
    memcpy(temp + SIZE, data[1], SIZE * sizeof(word)); // Backup second field

    // Shift data within the entire structure
    memmove(temp + n, temp, (SIZE * 2 - n) * sizeof(word));

    // Clear the vacated positions by setting them to zero
    memset(temp, 0, n * sizeof(word));

    // Copy back the shifted and cleared data into the original structure
    memcpy(data[0], temp, SIZE * sizeof(word)); // Restore first field
    memcpy(data[1], temp + SIZE, SIZE * sizeof(word)); // Restore second field
}

void addition_single(word* epsilon, word* dst, const word src1, const word src2);
void addition_core(word* epsilon, field dst, const field src1, const field src2);
void addition_p256(field dst, const field src1, const field src2);

void subtraction_single(word* epsilon, word* dst, const word src1, const word src2);
void subtraction_core(word* epsilon, field dst, const field src1, const field src2);
void subtraction_p256(field dst, const field src1, const field src2);

void multiplication_single(word* dst, const word src1, const word src2);
void multiplication_ps(field* dst, const field src1, const field src2);
void multiplication_ps2(field* dst, const field src1, const field src2);

void squaring_single(word* dst, const word src);

void multiplication_p256(field dst, const field src1, const field src2);

void fast_red(field dst, const field* src);
void mont_red(field dst, const field* src);

/* === Rubbish === */
// static inline void shiftField(field dst[2], u8 shiftAmount) {
//     if (shiftAmount >= 2 * SIZE) {
//         // If the shift amount is twice greater than or equal to SIZE, all data will be shifted out. So, fill with zeros.
//         for (int i = 0; i < 2; ++i) {
//             for (int j = 0; j < SIZE; ++j) {
//                 dst[i][j] = 0;
//             }
//         }
//     } else {
//         // For shifts between 7 and 15, inclusive
//         if (shiftAmount > 0) {
//             // Handle shifting from dst[0] to dst[1]
//             if (shiftAmount >= SIZE) {
//                 for (int j = 0; j < SIZE; ++j) {  // Shift within dst[1]
//                     int new_index = j + shiftAmount - SIZE;
//                     if (new_index < SIZE) {
//                         dst[1][new_index] = dst[0][j];  // Shift from dst[0] to dst[1]
//                     }
//                 }
//             } else if (shiftAmount > 0) {  // Normal shifting within dst[0]
//                 for (int j = SIZE - 1; j >= 0; --j) {
//                     int new_index = j + shiftAmount;
//                     if (new_index < SIZE) {
//                         dst[0][new_index] = dst[0][j];  // Shift within dst[0]
//                     } else {
//                         dst[1][new_index - SIZE] = dst[0][j];  // Shift from dst[0] to dst[1]
//                     }
//                 }
//             }
//             // Fill the vacated positions with zeros
//             for (int j = 0; j < (SIZE < shiftAmount ? SIZE : shiftAmount); ++j) {
//                 dst[0][j] = 0;  // Clear moved elements in dst[0]
//             }
//             if (shiftAmount >= SIZE) {
//                 for (int j = 0; j < shiftAmount - SIZE; ++j) {
//                     dst[1][j] = 0;  // Clear old elements in dst[1] when shiftAmount exceeds SIZE
//                 }
//             }
//         }
//     }
//     // If shiftAmount is 0, do nothing
// }

#endif /* _SECP256R1_BIGINT_H */