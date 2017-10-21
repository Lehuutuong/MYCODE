/////////////////  d3d9.h ///////////////////////////////////////////////////////////////
#if !defined(_D3D9_INCLUDE_FILE_)
#define _D3D9_INCLUDE_FILE_

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |       \
	((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */


#define D3DLOCK_READONLY           0x00000010L
#define D3DLOCK_DISCARD            0x00002000L
#define D3DLOCK_NOOVERWRITE        0x00001000L
#define D3DLOCK_NOSYSLOCK          0x00000800L
#define D3DLOCK_DONOTWAIT          0x00004000L

#define D3DLOCK_NO_DIRTY_UPDATE     0x00008000L

typedef struct _D3DLOCKED_RECT
{
	INT                 Pitch;
	void*               pBits;
} D3DLOCKED_RECT;

typedef enum _D3DFORMAT
{
	D3DFMT_UNKNOWN              =  0,

	D3DFMT_R8G8B8               = 20,
	D3DFMT_A8R8G8B8             = 21,
	D3DFMT_X8R8G8B8             = 22,
	D3DFMT_R5G6B5               = 23,
	D3DFMT_X1R5G5B5             = 24,
	D3DFMT_A1R5G5B5             = 25,
	D3DFMT_A4R4G4B4             = 26,
	D3DFMT_R3G3B2               = 27,
	D3DFMT_A8                   = 28,
	D3DFMT_A8R3G3B2             = 29,
	D3DFMT_X4R4G4B4             = 30,
	D3DFMT_A2B10G10R10          = 31,
	D3DFMT_A8B8G8R8             = 32,
	D3DFMT_X8B8G8R8             = 33,
	D3DFMT_G16R16               = 34,
	D3DFMT_A2R10G10B10          = 35,
	D3DFMT_A16B16G16R16         = 36,

	D3DFMT_A8P8                 = 40,
	D3DFMT_P8                   = 41,

	D3DFMT_L8                   = 50,
	D3DFMT_A8L8                 = 51,
	D3DFMT_A4L4                 = 52,

	D3DFMT_V8U8                 = 60,
	D3DFMT_L6V5U5               = 61,
	D3DFMT_X8L8V8U8             = 62,
	D3DFMT_Q8W8V8U8             = 63,
	D3DFMT_V16U16               = 64,
	D3DFMT_A2W10V10U10          = 67,

	D3DFMT_UYVY                 = MAKEFOURCC('U', 'Y', 'V', 'Y'),
	D3DFMT_R8G8_B8G8            = MAKEFOURCC('R', 'G', 'B', 'G'),
	D3DFMT_YUY2                 = MAKEFOURCC('Y', 'U', 'Y', '2'),
	D3DFMT_G8R8_G8B8            = MAKEFOURCC('G', 'R', 'G', 'B'),
	D3DFMT_DXT1                 = MAKEFOURCC('D', 'X', 'T', '1'),
	D3DFMT_DXT2                 = MAKEFOURCC('D', 'X', 'T', '2'),
	D3DFMT_DXT3                 = MAKEFOURCC('D', 'X', 'T', '3'),
	D3DFMT_DXT4                 = MAKEFOURCC('D', 'X', 'T', '4'),
	D3DFMT_DXT5                 = MAKEFOURCC('D', 'X', 'T', '5'),

	D3DFMT_D16_LOCKABLE         = 70,
	D3DFMT_D32                  = 71,
	D3DFMT_D15S1                = 73,
	D3DFMT_D24S8                = 75,
	D3DFMT_D24X8                = 77,
	D3DFMT_D24X4S4              = 79,
	D3DFMT_D16                  = 80,

	D3DFMT_D32F_LOCKABLE        = 82,
	D3DFMT_D24FS8               = 83,

	/* Z-Stencil formats valid for CPU access */
	D3DFMT_D32_LOCKABLE         = 84,
	D3DFMT_S8_LOCKABLE          = 85,



	D3DFMT_L16                  = 81,

	D3DFMT_VERTEXDATA           =100,
	D3DFMT_INDEX16              =101,
	D3DFMT_INDEX32              =102,

	D3DFMT_Q16W16V16U16         =110,

	D3DFMT_MULTI2_ARGB8         = MAKEFOURCC('M','E','T','1'),

	// Floating point surface formats

	// s10e5 formats (16-bits per channel)
	D3DFMT_R16F                 = 111,
	D3DFMT_G16R16F              = 112,
	D3DFMT_A16B16G16R16F        = 113,

	// IEEE s23e8 formats (32-bits per channel)
	D3DFMT_R32F                 = 114,
	D3DFMT_G32R32F              = 115,
	D3DFMT_A32B32G32R32F        = 116,

	D3DFMT_CxV8U8               = 117,

	// Monochrome 1 bit per pixel format
	D3DFMT_A1                   = 118,


	// Binary format indicating that the data has no inherent type
	D3DFMT_BINARYBUFFER            = 199,                                 


	D3DFMT_FORCE_DWORD          =0x7fffffff
} D3DFORMAT;


typedef enum _D3DRESOURCETYPE {
	D3DRTYPE_SURFACE                =  1,
	D3DRTYPE_VOLUME                 =  2,
	D3DRTYPE_TEXTURE                =  3,
	D3DRTYPE_VOLUMETEXTURE          =  4,
	D3DRTYPE_CUBETEXTURE            =  5,
	D3DRTYPE_VERTEXBUFFER           =  6,
	D3DRTYPE_INDEXBUFFER            =  7,           //if this changes, change _D3DDEVINFO_RESOURCEMANAGER definition


	D3DRTYPE_FORCE_DWORD            = 0x7fffffff
} D3DRESOURCETYPE;

typedef enum _D3DPOOL {
	D3DPOOL_DEFAULT                 = 0,
	D3DPOOL_MANAGED                 = 1,
	D3DPOOL_SYSTEMMEM               = 2,
	D3DPOOL_SCRATCH                 = 3,

	D3DPOOL_FORCE_DWORD             = 0x7fffffff
} D3DPOOL;


typedef enum _D3DMULTISAMPLE_TYPE
{
	D3DMULTISAMPLE_NONE            =  0,
	D3DMULTISAMPLE_NONMASKABLE     =  1,
	D3DMULTISAMPLE_2_SAMPLES       =  2,
	D3DMULTISAMPLE_3_SAMPLES       =  3,
	D3DMULTISAMPLE_4_SAMPLES       =  4,
	D3DMULTISAMPLE_5_SAMPLES       =  5,
	D3DMULTISAMPLE_6_SAMPLES       =  6,
	D3DMULTISAMPLE_7_SAMPLES       =  7,
	D3DMULTISAMPLE_8_SAMPLES       =  8,
	D3DMULTISAMPLE_9_SAMPLES       =  9,
	D3DMULTISAMPLE_10_SAMPLES      = 10,
	D3DMULTISAMPLE_11_SAMPLES      = 11,
	D3DMULTISAMPLE_12_SAMPLES      = 12,
	D3DMULTISAMPLE_13_SAMPLES      = 13,
	D3DMULTISAMPLE_14_SAMPLES      = 14,
	D3DMULTISAMPLE_15_SAMPLES      = 15,
	D3DMULTISAMPLE_16_SAMPLES      = 16,

	D3DMULTISAMPLE_FORCE_DWORD     = 0x7fffffff
} D3DMULTISAMPLE_TYPE;

typedef struct _D3DSURFACE_DESC
{
	D3DFORMAT           Format;
	D3DRESOURCETYPE     Type;
	DWORD               Usage;
	D3DPOOL             Pool;

	D3DMULTISAMPLE_TYPE MultiSampleType;
	DWORD               MultiSampleQuality;
	UINT                Width;
	UINT                Height;
} D3DSURFACE_DESC;

#endif // _D3D9_INCLUDE_FILE_