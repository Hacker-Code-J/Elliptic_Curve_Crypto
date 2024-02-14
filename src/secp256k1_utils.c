#include "secp256k1_utils.h"


#if PLATFORM == 32
void stringToWord(u32* wordArray, const char hexString[]) {
    size_t length = strlen(hexString) / 8; // 64 / 8 = 8
    if (length != 8) { // 8 * 8 * 4 = 256-bit
        printf("Invaild 128-bit Hexa-string Length!\n");
        return;
    }
    for (size_t i = 0; i < length; i++)
        sscanf(&hexString[i], "%8X", &wordArray[(length - 1) - i]);
}
#else
void stringToWord(u64* wordArray, const char* hexString) {
    size_t length = strlen(hexString) / 16; // 64 / 16 = 4
    if (length != 4) { // 8 * 8 * 4 = 256-bit
        printf("Invaild 128-bit Hexa-string Length!\n");
        return;
    }
    for (size_t i = 0; i < length; i++)
        sscanf(&hexString[i], "%16lX", &wordArray[(length - 1) - i]);
}
#endif