#include "secp256r1_config.h"

#ifndef _SECP256R1_UTILS_H
#define _SECP256R1_UTILS_H

void stringToWord(word* wordArray, const char* hexString);
void printData(word* data);
void printTestData(word* data);

u64 measure_cycles(void (*func)(word*, const word*, const word*),
                                  word* dst, const word* src1, const word* src2);

#endif /* _SECP256R1_UTILS */