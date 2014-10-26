/*----------------------------------------------------------------------------*\
  MODULE NAME: types.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Collection of basic types
\*----------------------------------------------------------------------------*/

#ifndef F3DB_TYPES_H
#define F3DB_TYPES_H


typedef char int8;
typedef short int16;
typedef long int32;
typedef __int64 int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned __int64 uint64;

#ifndef byte
#define byte uint8
#endif

typedef char *str;
typedef const char *cstr;



#define INT8_MIN -127
#define INT8_MAX 127
#define INT16_MIN -32767
#define INT16_MAX 32767
#define INT32_MIN -2147483647
#define INT32_MAX 2147483647
#define INT64_MIN -9223372036854775807
#define INT64_MAX 9223372036854775807
#define UINT8_MIN 0
#define UINT8_MAX 255
#define UINT16_MIN 0
#define UINT16_MAX 65535
#define UINT32_MIN 0
#define UINT32_MAX 4294967295
#define UINT64_MIN 0
#define UINT64_MAX 18446744073709551615



#ifndef NULL
#define NULL 0
#endif
#ifndef null
#define null NULL
#endif



#endif // #ifndef F3DB_TYPES_H