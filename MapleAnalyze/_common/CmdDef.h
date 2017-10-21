#ifndef __CMDDEF_H__
#define __CMDDEF_H__

typedef struct COMM_BLOCK
{
	BYTE ucOpcode;
	BYTE pCommBuf[1024];
} COMM_BLOCK, *PCOMM_BLOCK;

#define		CMD_TERM			0x69
#define		CMD_BEGIN			0x70
#define     CMD_FAIL_AUTH		0x7E
#define		CMD_FOCUS			0x7F
#define		CMD_DUMP			0x80
#define		CMD_INIT			0x81
#define		CMD_READY			0x82
#define		CMD_LOOK_MEM		0x83
#define		CMD_RUN_ASM			0x84
#define		CMD_BREAK			0x85
#define		CMD_VTHOOK			0x86
#define		CMD_VTUNHOOK		0x87
#define		CMD_RELEASE			0x88
#define		CMD_INSTALLVEH		0x89
#define		CMD_UNINSTALLVEH	0x90


struct SHARE_INFO
{
	DWORD nCommand;
	DWORD dwData;
	DWORD dwBRBuf[80];
};

#endif
