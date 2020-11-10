#ifndef ATLAS_LIBC_STDINT_H
#define ATLAS_LIBC_STDINT_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef uint32_t size_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long int64_t;
#define NULL ((void*)0x0)

#define INT8_MIN		(-128)
#define INT16_MIN		(-32768)
#define INT32_MIN		(-2147483647 - 1)
#define INT64_MIN		(-9223372036854775807LL - 1)

#define INT8_MAX		127
#define INT16_MAX		32767
#define INT32_MAX		2147483647
#define INT64_MAX		9223372036854775807LL

#define UINT8_MAX		0xFFU
#define UINT16_MAX		0xFFFFU
#define UINT32_MAX		0xFFFFFFFFUL
#define UINT64_MAX		0xFFFFFFFFFFFFFFFFFULL

#endif
