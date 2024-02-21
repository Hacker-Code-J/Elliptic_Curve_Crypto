#include "secp256r1_utils.h"
#include "secp256r1_bigint.h"
#include "secp256r1_validation.h"

int main(void) {
    const char* opA = "7E781AF075DF34221947833F570DC5EEC844DE0DE81A7071CDB374444857A994";
    const char* opB = "9C3980B6BFA0B75993BE648924B9D298101E46FEF0AC62E19D6BCABB8FF8D324";

    // word src1[SIZE];
    // word src2[SIZE];
    // word dst[SIZE] = { 0x00, };
    field src1, src2;
    // field dst;
    field dst[2];

    stringToWord(src1, opA);
    stringToWord(src2, opB);

    printData(src1);
    printData(src2);
    
    // addition_p256(dst, src1, src2);
    // subtraction_p256(dst, src1, src2);
    multiplication_os(dst, src1, src2);
    printData(dst[1]);
    printData(dst[0]);
    // for (int i = 0; i < 16; i++) {
	// 	if (i % 4 == 0) puts("");
	// 	printf("%08x, ", *dst[i]);
	// } puts("");

    // addition_p256_test();
    // subtraction_p256_test();

    // u64 cycles;
    // for (u32 i = 0; i < 12000; i++) {
    //     assign_rand(src1, SIZE);
    //     assign_rand(src2, SIZE);
    //     cycles = measure_cycles(addition_p256, dst, src1, src2);
    //     // cycles = measure_cycles(subtraction_p256, dst, src1, src2);
    //     if (i >= 1000 && i < 11000) {
    //         printf("%" PRIu64 "\n", cycles);
    //         printf("%llu\n", (long long)cycles / (long long)SIZE);
    //     }
    // }
    return 0;
}