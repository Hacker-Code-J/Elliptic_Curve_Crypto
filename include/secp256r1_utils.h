#include "secp256r1_config.h"

#ifndef _SECP256R1_UTILS_H
#define _SECP256R1_UTILS_H

void stringToWord(word* wordArray, const char* hexString);
void printData(word* data);
void printTestData(word* data);

#endif /* _SECP256R1_UTILS */