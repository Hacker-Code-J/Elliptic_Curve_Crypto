#include "secp256r1_config.h"
#include "secp256r1_utils.h"

#ifndef _SECP256R1_VALIDATION_H
#define _SECP256R1_VALIDATION_H

#define MAX_LINE_LENGTH 1024

void printProgressBar(i32 current, i32 total);

bool is_blank_line(const char* line);

void create_p256_rspFile(const char* rspFileName,
                         const char* reqFileName1, const char* reqFileName2,
                         u8 option);

void addition_p256_test();
void subtraction_p256_test();

void create_mul_squ_rspFile(const char* rspFileName,
                            const char* reqFileName1, const char* reqFileName2,
                            u8 option);
                            
void multiplication_ps_test();
void multiplication_ps2_test();

void multiplication_p256_test();

#endif /* _SECP256R1_VALIDATION_H */