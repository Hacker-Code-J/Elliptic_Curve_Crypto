#include "secp256r1_config.h"
#include "secp256r1_utils.h"

#ifndef _SECP256R1_VALIDATION_H
#define _SECP256R1_VALIDATION_H

#define MAX_LINE_LENGTH 128

void printProgressBar(i32 current, i32 total);
void parseHexLine(word* dst, const char* src);

bool is_blank_line(const char* line);
void create_rspFile(const char* rspFileName, const char* reqFileName1, const char* reqFileName2);

bool arrays_are_equal(const word* data1, const word* data2, size_t size);

void addition_test();

#endif /* _SECP256R1_VALIDATION_H */