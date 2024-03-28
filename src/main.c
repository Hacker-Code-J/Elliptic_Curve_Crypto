#include "secp256r1_utils.h"
#include "secp256r1_bigint.h"
#include "secp256r1_validation.h"

int main(void) {
    const char* opA = "EAC8C20CE80D90B3BF6DC08AFD4C8B1D6274839279D9F1469BE87CCD85283FE5";
    const char* opB = "F54ECAE2F71FC58022DF670B74F41B07195BEEC269F85DDC1368137BC3DD50B3";

    // word src1[SIZE];
    // word src2[SIZE];
    // word dst[SIZE] = { 0x00, };
    field src1, src2;
    field dst;
    // field dst[2];

    stringToWord(src1, opA);
    stringToWord(src2, opB);

    printData(src1);
    printData(src2);
    puts("");

    // addition_p256(dst, src1, src2);
    // subtraction_p256(dst, src1, src2);
    
    // multiplication_p256(dst, src1, src2);
    // printData(dst);
    multiplication_montgomery_p256(dst, src1, src2);
    // multiplication_p256_test();

    // multiplication_ps2_test();
    // multiplication_ps2(dst, src1, src2);
    // printData(dst[1]);
    // printData(dst[0]);
    // for (int i = 0; i < 16; i++) {
	// 	if (i % 4 == 0) puts("");
	// 	printf("%08x, ", *dst[i]);
	// } puts("");

    // field data[2] = {
    //     {0xC, 0xCC, 0xCCC, 0xCCCC, 0xCCCCC, 0xCCCCCC, 0xCCCCCC, 0xCCCCCCC},
    //     {0xA, 0xAA, 0xAAA, 0xAAAA, 0xAAAAA, 0xAAAAAA, 0xAAAAAA, 0xAAAAAAA}
    // };
    // printData(data[1]);
    // printData(data[0]);
    // printf("%X\n", *data[0]);
    // printf("%X\n", *(data[0] + 1));
    // printf("%X\n", *(data[0] + 7));
    // // printf("%X\n", *(data[0] + 8));
    // printf("%X\n", *(data[1] + 0));


    // // *(data[1]+7) = *(data[1] + 6);
    // // *(data[1]+6) = *(data[1] + 5);
    // // *(data[1]+5) = *(data[1] + 4);
    // // *(data[1]+4) = *(data[1] + 3);
    // // *(data[1]+3) = *(data[1] + 2);
    // // *(data[1]+2) = *(data[1] + 1);
    // // *(data[1]+1) = *(data[1] + 0);
    // // *(data[1]+0) = *(data[0] + 7);
    // // *(data[0]+7) = *(data[0] + 6);
    // // *(data[0]+6) = *(data[0] + 5);
    // // *(data[0]+5) = *(data[0] + 4);
    // // *(data[0]+4) = *(data[0] + 3);
    // // *(data[0]+3) = *(data[0] + 2);
    // // *(data[0]+2) = *(data[0] + 1);
    // // *(data[0]+1) = *(data[0] + 0);
    // // *(data[0]+0) = 0;

    // shift_field_data(data, 8);

    // printData(data[1]);
    // printData(data[0]);


// /* === Test for Accuracy === */
    // addition_p256_test();
    // subtraction_p256_test();
    // multiplication_p256_test();
    // multiplication_ps_test();
    // multiplication_ps2_test();


/* === Measurement of Cycles === */
    // u64 cycles;
    // for (u32 i = 0; i < 120000; i++) {
    //     assign_rand(src1, SIZE);
    //     assign_rand(src2, SIZE);
    //     // cycles = measure_cycles(addition_p256, dst, src1, src2);
    //     // cycles = measure_cycles(subtraction_p256, dst, src1, src2);
    //     cycles = measure_cycles(multiplication_p256, dst, src1, src2);
    //     if (i >= 10000 && i < 110000) {
    //         printf("%" PRIu64 "\n", cycles);
    //         // printf("%" PRIu64 "\n", cycles / (u64)SIZE);
    //     }
    // }

    // u64 cycles;
    // for (u32 i = 0; i < 12000; i++) {
    //     assign_rand(src1, SIZE);
    //     assign_rand(src2, SIZE);
    //     // cycles = measure_cycles2(multiplication_single, dst, src1, src2);
    //     // cycles = measure_cycles2(multiplication_os, dst, src1, src2);
    //     cycles = measure_cycles2(multiplication_ps, dst, src1, src2);
    //     if (i >= 1000 && i < 11000) {
    //         printf("%" PRIu64 "\n", cycles);
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