#include "secp256r1_config.h"

#ifndef _SECP256R1_UTILS_H
#define _SECP256R1_UTILS_H

static inline void assign_rand(word* dst, size_t wordLen) {
    // Cast the destination pointer to a pointer to u8 (unsigned 8-bit integer)
    u8* p = (u8*)dst;
    
    // Calculate the total number of bytes to be filled
    u32 cnt = wordLen * sizeof(word);

    // Loop until all bytes are filled
    while (cnt > 0) {
        *p = rand() & 0xff; // Assign a random byte (0 to 255) to the current location pointed by p
        p++;                // Move to the next byte
        cnt--;
    }
}

void stringToWord(word* wordArray, const char* hexString);
void printData(word* data);
void printTestData(word* data);

u64 measure_cycles(void (*func)(word*, const word*, const word*),
                                  word* dst, const word* src1, const word* src2);

#endif /* _SECP256R1_UTILS */