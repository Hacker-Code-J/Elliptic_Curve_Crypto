#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifndef _ECC_CONFIG_H
#define _ECC_CONFIG_H

typedef int8_t i8;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

#ifdef _64BIT_SYSTEM
typedef u64 field_element[4]; // For 64-bit systems
#else
typedef u32 field_element[8]; // For 32-bit systems
#endif

#endif /* _ECC_CONFIG_H */