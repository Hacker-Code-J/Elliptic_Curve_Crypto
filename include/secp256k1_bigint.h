#include "secp256k1_config.h"

#ifndef _SECP256K1_BIGINT_H
#define _SECP256K1_BIGINT_H

#ifdef IS_32_BIT_ENV
static const prime_field PRIME = {
	0xFFFFFC2FU, 0xFFFFFFFEU, 0xFFFFFFFFU, 0xFFFFFFFFU,
	0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU
};
static const prime_field PRIME_COMPLEMENT = {
	0x000003D0U, 0x00000001U, 0x00000000U, 0x00000000U,
	0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U
};
#else
static const prime_field PRIME = {
	0xFFFFFFFEFFFFFC2FU, 0xFFFFFFFFFFFFFFFFU,
	0xFFFFFFFFFFFFFFFFU, 0xFFFFFFFFFFFFFFFFU
};
static const prime_field PRIME_COMPLEMENT = {
	0x00000001000003D0U, 0x0000000000000000U,
	0x0000000000000000U, 0x0000000000000000U
};
#endif

void add_core(bool* pEpsilon, word result[SIZE], const word op1[SIZE], const word op2[SIZE]);
void addition_p256(word result[SIZE], const word op1[SIZE], const word op2[SIZE]);


#endif /* _SECP256K1_BIGINT_H */