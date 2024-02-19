#include "secp256r1_utils.h"
#include "secp256r1_bigint.h"
#include "secp256r1_validation.h"

int main(void) {
    const char* opA = "BD91C935C85617B079C6F2728B987CE488BB17B4644D5F8B9C23AF955AB74663";
    const char* opB = "4E272A7341569559F3E58053BE961728D67BF71EFBA44BF283DAA7ED9BF6DDA8";

    word src1[SIZE];
    word src2[SIZE];
    word dst[SIZE] = { 0x00, };

    stringToWord(src1, opA);
    stringToWord(src2, opB);

    // printData(src1);
    // printData(src2);

    addition_p256(dst, src1, src2);
    // printData(dst);

    addition_p256_test();

    // u64 cycles;
    // for (u32 i = 0; i < 12000; i++) {
    //     assign_rand(src1, SIZE);
    //     assign_rand(src2, SIZE);
    //     cycles = measure_cycles(addition_p256, dst, src1, src2);
    //     if (i >= 1000 && i < 11000) {
    //         printf("%" PRIu64 "\n", cycles);
    //         printf("%llu\n", (long long)cycles / (long long)SIZE);
    //     }
    // }
    return 0;
}