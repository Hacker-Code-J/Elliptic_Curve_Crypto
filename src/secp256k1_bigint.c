#include "secp256k1_bigint.h"

void add_core(word* result, const u32 op1[SIZE], const u32 op2[SIZE]) {
    u64 carry = 0;
	for (i32 i = 0; i < SIZE; i++) { // Assuming 64-bit system
		u64 temp = (u64)(op1[i] + op2[i] + carry);
		result[i] = temp & 0xFFFFFFFFFFFFFFFF; // Keep only 64 bits
		carry = temp >> 64; // Carry for the next iteration
	}
	
	// // Modular reduction if necessary
	// if (carry || is_greater_or_equal(result, p256)) {
	// 	// Subtract p256 if result >= p256
	// 	subtract_p256(result);
	// }
}