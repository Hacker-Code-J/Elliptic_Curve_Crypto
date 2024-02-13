#include "bigint_utils.h"

void stringToWord(u32* wordArray, const char* hexString) {
    size_t length = strlen(hexString);
    for (size_t i = 0; i < length; i += 8) {
        sscanf(&hexString[i], "%8x", &wordArray[i / 8]);
    }
}