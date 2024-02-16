#include "secp256r1_config.h"

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
	0x0000000000000000U, 0xFFFFFFFF00000000U,
	0xFFFFFFFFFFFFFFFFU, 0x00000000FFFFFFFFU
};
#endif

void addition_single(word* epsilon, word* dst, const word src1, const word src2);
void addition_core(word* epsilon, word* dst, const word* src1, const word* src2);
void addition_p256(word* dst, const word* src1, const word* src2);


#endif /* _SECP256R1_BIGINT_H */