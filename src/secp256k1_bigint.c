#include "secp256k1_bigint.h"

void add_core(bool* pEpsilon, word result[SIZE], const word op1[SIZE], const word op2[SIZE]) {
    result[0] = op1[0] + op2[0];
	bool carry = (op1[0] + op2[0]) < op1[0];
	for (u8 i = 1; i < SIZE; i++) {
		result[i] = op1[i] + op2[i] + carry;
		carry = (op1[i] + op2[i]) < op1[i];
	}
	*pEpsilon = carry;


	
	// u64 carry = 0;
	// for (i32 i = 0; i < SIZE; i++) { // Assuming 64-bit system
	// 	u64 temp = (u64)(op1[i] + op2[i] + carry);
	// 	result[i] = temp & 0xFFFFFFFFFFFFFFFF; // Keep only 64 bits
	// 	carry = temp >> 64; // Carry for the next iteration
	// }
	
	// // Modular reduction if necessary
	// if (carry || is_greater_or_equal(result, p256)) {
	// 	// Subtract p256 if result >= p256
	// 	subtract_p256(result);
	// }
}

void addition_p256(word result[SIZE], const word op1[SIZE], const word op2[SIZE]) {
	exit(1);
}