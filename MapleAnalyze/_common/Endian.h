#pragma once



#	ifndef LITTLE_ENDIAN
#		define LITTLE_ENDIAN 1234
#	endif
#	ifndef BYTE_ORDER
#		define BYTE_ORDER LITTLE_ENDIAN
#	endif


/* Macros to read and write 16, 32, and 64-bit quantities in a portable manner.
   These functions are implemented as macros rather than true functions as
   the need to adjust the memory pointers makes them somewhat painful to call
   in user code */

#define mputInt64(memPtr,data) \
	*memPtr++ = ( unsigned char ) ( ( ( data ) >> 56 ) & 0xFF ), \
	*memPtr++ = ( unsigned char ) ( ( ( data ) >> 48 ) & 0xFF ), \
	*memPtr++ = ( unsigned char ) ( ( ( data ) >> 40 ) & 0xFF ), \
	*memPtr++ = ( unsigned char ) ( ( ( data ) >> 32 ) & 0xFF ), \
	*memPtr++ = ( unsigned char ) ( ( ( data ) >> 24 ) & 0xFF ), \
	*memPtr++ = ( unsigned char ) ( ( ( data ) >> 16 ) & 0xFF ), \
	*memPtr++ = ( unsigned char ) ( ( ( data ) >> 8 ) & 0xFF ), \
	*memPtr++ = ( unsigned char ) ( ( data ) & 0xFF )

#define mputLong(memPtr,data) \
	*memPtr++ = ( unsigned char ) ( ( ( data ) >> 24 ) & 0xFF ), \
	*memPtr++ = ( unsigned char ) ( ( ( data ) >> 16 ) & 0xFF ), \
	*memPtr++ = ( unsigned char ) ( ( ( data ) >> 8 ) & 0xFF ), \
	*memPtr++ = ( unsigned char ) ( ( data ) & 0xFF )

#define mputWord(memPtr,data) \
	*memPtr++ = ( unsigned char ) ( ( ( data ) >> 8 ) & 0xFF ), \
	*memPtr++ = ( unsigned char ) ( ( data ) & 0xFF )

#define mputByte(memPtr,data)	\
	*memPtr++ = ( unsigned char ) data

#define mputBytes(memPtr,data,len)  \
	memcpy (memPtr,data,len); \
	memPtr += len;

#define mgetInt64(memPtr) 		\
	( memPtr += 8, ( ( unsigned __int64 ) memPtr[ -8 ] << 56 ) | ( ( unsigned __int64 ) memPtr[ -7 ] << 48 ) | \
	( ( unsigned __int64 ) memPtr[ -6 ] << 40 ) | ( ( unsigned __int64 ) memPtr[ -5 ] << 32 ) | \
	( ( unsigned __int64 ) memPtr[ -4 ] << 24 ) | ( ( unsigned __int64 ) memPtr[ -3 ] << 16 ) | \
	  ( ( unsigned __int64 ) memPtr[ -2 ] << 8 ) | ( unsigned __int64 ) memPtr[ -1 ] )

#define mgetLong(memPtr) 		\
	( memPtr += 4, ( ( unsigned __int32 ) memPtr[ -4 ] << 24 ) | ( ( unsigned __int32 ) memPtr[ -3 ] << 16 ) | \
	  ( ( unsigned __int32 ) memPtr[ -2 ] << 8 ) | ( unsigned __int32 ) memPtr[ -1 ] )

#define mgetWord(memPtr) 		\
	( memPtr += 2, ( unsigned short ) memPtr[ -2 ] << 8 ) | ( ( unsigned short ) memPtr[ -1 ] ) 

#define mgetByte(memPtr)		\
	( ( unsigned char ) *memPtr++ )

#if BYTE_ORDER == BIG_ENDIAN
#	define LE16(x) MirrorBytes16(x)
#	define LE32(x) MirrorBytes32(x)
#	define LE64(x) MirrorBytes64(x)
#else
#	define LE16(x) (x)
#	define LE32(x) (x)
#	define LE64(x) (x)
#endif

#if BYTE_ORDER == LITTLE_ENDIAN
#	define BE16(x) MirrorBytes16(x)
#	define BE32(x) MirrorBytes32(x)
#	define BE64(x) MirrorBytes64(x)
#else
#	define BE16(x) (x)
#	define BE32(x) (x)
#	define BE64(x) (x)
#endif

unsigned __int16 MirrorBytes16 (unsigned __int16 x);
unsigned __int32 MirrorBytes32 (unsigned __int32 x);
unsigned __int64 MirrorBytes64 (unsigned __int64 x);
void LongReverse ( unsigned __int32 *buffer , unsigned byteCount );

