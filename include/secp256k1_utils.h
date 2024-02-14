#include "secp256k1_config.h"

#ifndef _SECP256K1_UTILS_H
#define _SECP256K1_UTILS_H

#if PLATFORM == 32
void stringToWord(u32* wordArray, const char hexString[]);
#else
void stringToWord(u64* wordArray, const char* hexString);
#endif


#endif /* _SECP256K1_UTILS */