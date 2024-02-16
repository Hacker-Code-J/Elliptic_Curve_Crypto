#include "secp256r1_utils.h"
#include "secp256r1_bigint.h"

void printData(word* data) {
    for (i32 i = SIZE-1; i >=0; i--) {
#ifdef IS_32_BIT_ENV
        printf("%08X ", data[i]);
#else
        printf("%16lX:", data[i]);
#endif
    } puts("");
}

int main(void) {
    // printf("%ld\n", sizeof(bool*));

    const char* opA = "BD91C935C85617B079C6F2728B987CE488BB17B4644D5F8B9C23AF955AB74663";
    const char* opB = "4E272A7341569559F3E58053BE961728D67BF71EFBA44BF283DAA7ED9BF6DDA8";

    word src1[SIZE];
    word src2[SIZE];
    word dst[SIZE] = { 0x00, };

    stringToWord(src1, opA);
    stringToWord(src2, opB);

    printData(src1);
    printData(src2);

    addition_p256(dst, src1, src2);
    printData(dst);

    return 0;
}