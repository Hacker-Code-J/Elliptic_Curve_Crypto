#include "secp256k1_config.h"


#ifndef _SECP256K1_BIGINT_H
#define _SECP256K1_BIGINT_H

void add_core(word* result, const u32 op1[SIZE], const u32 op2[SIZE]);

#endif /* _SECP256K1_BIGINT_H */