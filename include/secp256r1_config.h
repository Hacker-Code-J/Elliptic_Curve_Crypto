#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h> // For sleep function

#include <inttypes.h> // for PRIu64 macro

#ifdef __i386
#include <x86intrin.h> // Use this on x86 platforms
#elif defined(__x86_64)
#include <x86intrin.h> // Use this on x86_64 platforms
#endif

/**
 * Standard for Efficient Cryptography
 * p     Field type = Prime field
 * 256   Key size = 256
 * r     Curve type = Verifiably Random
 * 1     Sequence = 1
*/

#ifndef _SECP256R1_CONFIG_H
#define _SECP256R1_CONFIG_H

#ifdef _WIN32
// Windows-specific definitions
#include <windows.h>

// For Windows, use standard types where applicable. Since Windows
// does not define int8_t, int32_t, etc., explicitly, we use
// the types from stdint.h which are also available in Windows
// when using a C99 or newer compiler.
#include <stdint.h> // Ensure this is included for fixed-width types

typedef int8_t      i8;
typedef int32_t     i32;
typedef int64_t     i64;

typedef uint8_t     u8;
typedef uint16_t    u16;
// Use Windows-specific types for u32 and u64 for demonstration, though
// uint32_t and uint64_t could also be used directly.
typedef DWORD       u32;
typedef DWORDLONG   u64;
#elif defined(__linux__)
// Linux-specific definitions
#include <stdint.h>

typedef int8_t      i8;
typedef int32_t     i32;
typedef int64_t     i64;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;
#else
#error "Unsupported platform"
#endif

// Define this to force 32-bit mode in development
#define FORCE_32_BIT

// Simplified check for 32-bit or forced 32-bit mode
#if defined(FORCE_32_BIT) || !defined(_WIN64) && !defined(__x86_64__) && !defined(__ppc64__)
#define IS_32_BIT_ENV
#endif

#ifdef IS_32_BIT_ENV
// 32-bit specific settings
#define ONE     0x01
#define SIZE    8
typedef u32     word;
typedef word    field[SIZE];
#else
// 64-bit specific settings
#define ONE     0x01LL
#define SIZE    4
typedef u64     word;
typedef word    field[SIZE];
#endif

#endif /* _SECP256R1_CONFIG_H */