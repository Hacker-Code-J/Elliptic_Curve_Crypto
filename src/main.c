#include "secp256k1_utils.h"

int main(void) {
    const char string[] = "BD91C935C85617B079C6F2728B987CE488BB17B4644D5F8B9C23AF955AB74663";

    u32 word[8];
    stringToWord(word, string);

    for (i32 i = 7; i >=0; i--)
        printf("%8X:", word[i]);
    puts("");
    // printf("Test!\n");
    return 0;
}