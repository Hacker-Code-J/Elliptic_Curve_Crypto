#include "secp256r1_utils.h"


void stringToWord(word* wordArray, const char* hexString) {
    size_t length = strlen(hexString) / (SIZE == 8 ? 8 : 16);
    if (length != SIZE) {
        printf("Invaild 128-bit Hexa-string Length!\n");
        return;
    }
    for (size_t i = 0; i < length; i++)
#ifdef IS_32_BIT_ENV
        sscanf(&hexString[i * 8], "%8X", &wordArray[(length - 1) - i]);
#else
        sscanf(&hexString[i * 16], "%16lX", &wordArray[(length - 1) - i]);
#endif
}

void printData(word* data) {
    for (i32 i = SIZE-1; i >=0; i--) {
#ifdef IS_32_BIT_ENV
        printf("%08X:", data[i]);
#else
        printf("%016lX:", data[i]);
#endif
    } puts("");
}

void printTestData(word* data) {
    for (i32 i = SIZE-1; i >=0; i--) {
#ifdef IS_32_BIT_ENV
        printf("%08X", data[i]);
#else
        printf("%016lX", data[i]);
#endif
    } puts("");
}