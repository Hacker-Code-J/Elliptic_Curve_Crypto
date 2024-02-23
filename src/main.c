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
    // multiplication_os(dst, src1, src2);
    multiplication_ps(dst, src1, src2);

    // multiplication_os_test();
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
    //         printf("%" PRIu64 "\n", cycles / (u64)SIZE);
    //     }
    // }

    // u64 cycles;
    // for (u32 i = 0; i < 12000; i++) {
    //     assign_rand(src1, SIZE);
    //     assign_rand(src2, SIZE);
    //     cycles = measure_cycles2(multiplication_os, dst, src1, src2);
    //     // cycles = measure_cycles(subtraction_p256, dst, src1, src2);
    //     if (i >= 1000 && i < 11000) {
    //         printf("%" PRIu64 "\n", cycles);
    //         // printf("%" PRIu64 "\n", cycles / (u64)(SIZE * 2));
    //     }
    // }

    // field t0, t1; // Define two fields, each with SIZE elements
    // word *ptr_t0, *ptr_t1; // Define pointers to elements of type u32

    // memset(t0, 0, sizeof(field));
    // memset(t1, 0, sizeof(field));

    // // Initialize fields for demonstration (Optional)
    // for(int i = 0; i < SIZE; ++i) {
    //     t0[i] = i;        // Assign values for demonstration
    //     t1[i] = SIZE - i; // Just example values
    // }
    // printData(t0);
    // printData(t1);
    // // Point to the third element of each array (i.e., index 2)
    // ptr_t0 = &t0[2];
    // ptr_t1 = &t1[2];

    // // Directly change values using the pointers
    // *ptr_t0 = 88; // Changes the value of t0[2] to 88
    // *ptr_t1 = 99; // Changes the value of t1[2] to 99

    // // Print the entire arrays to see the change
    // printf("Modified t0: ");
    // for (int i = 0; i < SIZE; i++) {
    //     printf("%X ", t0[i]);
    // }
    // printf("\n");

    // printf("Modified t1: ");
    // for (int i = 0; i < SIZE; i++) {
    //     printf("%X ", t1[i]);
    // }
    // printf("\n");

    // printData(t0);
    // printData(t1);

    return 0;
}