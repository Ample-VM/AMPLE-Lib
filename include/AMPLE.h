#pragma once

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#if  (  defined(_WIN32)    || \
        defined(__WIN32__) || \
        defined(__NT__)    || \
        defined(WIN32)     || \
        defined(_WIN64)   )
    #define __WINDOWS__
    #if (!defined(_MSC_VER))
        #warning AMPLE is tested in the MS Compiler If any problem arises \
                 you will have to fix them by yourself
    #endif
#elif ( defined(__linux__))
    #define __LINUX__
    #if (!defined(__GNUC__))
        #warning AMPLE is tested in the GNU C Compiler. If any problem arises \
                 you will have to fix them by yourself
    #endif
#elif ( defined(__AMPLE__))
    #if (!defined(__GNUC__))
        #warning AMPLE is tested in the GNU C Compiler. If any problem arises \
                 you will have to fix them by yourself
    #endif
#elif (!defined(__APPLE__))
    #warning AMPLE does not support your platform. If any problem arises \
             you will have to fix them by yourself
#endif


#if   (defined(__amd64)    || \
       defined(__amd64__)  || \
       defined(__x86_64)   || \
       defined(__x86_64__) || \
       defined(_M_AMD64)   || \
       defined(_M_X64))
    #if (!defined(__x86_64__))
        #define __x86_64__
    #endif
#elif ( defined(i386)     || \
        defined(__i386)   || \
        defined(__i386__) || \
        defined(_M_IX86))
    #if (!defined(__x86__))
        #define __x86__
    #endif
#elif ((defined(_M_ARM) && defined(_M_ARM64)) && \
       !defined(__aarch64) && \
       defined(__arm__))
    #if (!defined(__ARM32__))
        #define __ARM32__
    #endif
#elif ( defined(_M_ARM) || \
        defined(__aarch64__))
    #if (!defined(__ARM64__))
        #define __ARM64__
    #endif
#else
    #warning AMPLE does not support your architecture. If any problem arises \
             you will have to fix them by yourself
#endif


#if   ( defined(__WINDOWS__)                      && defined(_MSC_VER))
    #define EXPORT __declspec(dllexport)
	#define IMPORT __declspec(dllimport)
#elif ((defined(__LINUX__) || defined(__APPLE__)) && defined(__GNUC__))
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    #define EXPORT
    #define IMPORT
#endif


#if   ( defined(__WINDOWS__) )
    #define INVALID_ARGUMENT 10022
#elif ( defined(__LINUX__) )
    #define INVALID_ARGUMENT EINVAL
#elif ( defined(__APPLE__) )
    #define INVALID_ARGUMENT EINVAL
#else
    #define INVALID_ARGUMENT 1
#endif


#if (!defined(__WINDOWS__))
    #define TEXT(...) __VA_ARGS__
#endif

#if   ( defined(_MSC_VER) && !defined(INLINE))
    #define INLINE inline __forceinline
#elif ( defined(__GNUC__) && !defined(INLINE))
    #define INLINE inline __attribute__((always_inline))
#elif ( !defined(INLINE) )
    #define INLINE inline
#endif

#if   ( defined(__WINDOWS__) )
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif


#define BIG_ENDIAN_D 0
#define LITTLE_ENDIAN_D 1
#define VOODOO_ENDIAN_D 2


INLINE uint16_t ConvertEndianU16(uint16_t input) {
    return (input << 8) | (input >> 8);
}

INLINE int16_t ConvertEndian16(int16_t input) {
    return (input << 8) | ((input >> 8) & 0xFF);
}


INLINE uint32_t ConvertEndianU32(uint32_t input) {
    uint32_t intermediateVal = ((input << 8) & 0xFF00FF00) | ((input >> 8) & 0xFF00FF);
    return (intermediateVal << 16) | (intermediateVal >> 16);
}

INLINE int32_t ConvertEndian32(int32_t input) {
    int32_t intermediateVal = ((input << 8) & 0xFF00FF00) | ((input >> 8) & 0xFF00FF);
    return (intermediateVal << 16) | ((intermediateVal >> 16) & 0xFFFF);
}


INLINE uint64_t ConvertEndianU64(uint64_t input) {
    uint64_t intermediateVal1 = ((input << 8) & 0xFF00FF00FF00FF00ULL) | ((input >> 8) & 0x00FF00FF00FF00FFULL);
    uint64_t intermediateVal2 = ((intermediateVal1 << 16) & 0xFFFF0000FFFF0000ULL) | ((intermediateVal1 >> 16) & 0x0000FFFF0000FFFFULL);
    return (intermediateVal2 << 32) | (intermediateVal2 >> 32);
}

INLINE int64_t ConvertEndian64(int64_t input) {
    int64_t intermediateVal1 = ((input << 8) & 0xFF00FF00FF00FF00ULL) | ((input >> 8) & 0x00FF00FF00FF00FFULL);
    int64_t intermediateVal2 = ((intermediateVal1 << 16) & 0xFFFF0000FFFF0000ULL) | ((intermediateVal1 >> 16) & 0x0000FFFF0000FFFFULL);
    return (intermediateVal2 << 32) | ((intermediateVal2 >> 32) & 0xFFFFFFFFULL);
}

INLINE uint8_t GetEndianness() {
    char fNumber[] = { 0x01, 0x02, 0x03, 0x04,
                       0x05, 0x06, 0x07, 0x08 };
    uint64_t sNumber = 0x0102030405060708;

    if (*((uint64_t*)fNumber) == sNumber)
        return BIG_ENDIAN_D;
    else if (*((uint64_t*)fNumber) == ConvertEndianU64(sNumber))
        return LITTLE_ENDIAN_D;
    else
        return VOODOO_ENDIAN_D;
}


/**
 * AMPLE_BIN_STRUCT is applied to the pointer of the first byte
 * AMPLE_MAGIC must contain 0x41 0x4D 0x50 0x4C 0x45 0x53 0x55 0x00 or or the decimal 24016930118585665
 * AMPLE_BIN_STRUCT_VERSION contains the struct version of the binary. The first two always contain the Magic number and version
 * AMPLE_MIN_MEMORY contains the minimum number of bytes to reserve
 * AMPLE_NUM_OF_TABLE_CONTENT
 */
typedef struct {
		uint64_t AMPLE_MAGIC;
		uint64_t AMPLE_BIN_STRUCT_VERSION;
		uint64_t AMPLE_INITIAL_VERSION;
		uint64_t AMPLE_MIN_MEMORY;
		uint64_t AMPLE_NUM_OF_TABLE_CONTENT;
		uint64_t AMPLE_TABLE_CONTENT;
		uint64_t AMPLE_NUM_OF_INSTRUCTIONS;
		uint64_t AMPLE_TABLE_OF_INSTRUCTIONS;
} AMPLE_BIN;


typedef struct {
    uint64_t CONTENT_TYPE;
    uint64_t CONTENT_MODE;
    uint64_t CONTENT_VALUE_00;
    uint64_t CONTENT_VALUE_01;
    uint64_t CONTENT_VALUE_02;
    uint64_t CONTENT_VALUE_03;
    uint64_t CONTENT_VALUE_04;
    uint64_t CONTENT_VALUE_05;
    uint64_t CONTENT_VALUE_06;
    uint64_t CONTENT_VALUE_07;
    uint64_t CONTENT_VALUE_08;
    uint64_t CONTENT_VALUE_09;
    uint64_t CONTENT_VALUE_10;
    uint64_t CONTENT_VALUE_11;
    uint64_t CONTENT_VALUE_12;
    uint64_t CONTENT_VALUE_13;
} AMPLE_CONTENT;

typedef struct {
    uint32_t INSTRUCTION;
    uint32_t MODE;
    uint64_t ARG1;
    uint64_t ARG2;
    uint64_t ARG3;
} AMPLE_INSTRUCTION;

typedef struct {
    const char* opcode;
    uint32_t opcodeLength;
    const char** arguments;
    uint32_t* argumentsLength;
    uint32_t argumentLength;
} AMPLE_ASSEMBLER_LINE;