#include "secp256r1_utils.h"

int main(void) {
    const char* string = "BD91C935C85617B079C6F2728B987CE488BB17B4644D5F8B9C23AF955AB74663";

    word data[SIZE];
    stringToWord(data, string);

    for (i32 i = SIZE-1; i >=0; i--)
#ifdef IS_32_BIT_ENV
        printf("%8X:", data[i]);
#else
        printf("%16lX:", data[i]);
#endif
    puts("");

    return 0;
}