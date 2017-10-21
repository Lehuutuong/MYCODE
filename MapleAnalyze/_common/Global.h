#ifndef __GLOBAL_H__
#define __GLOBAL_H__


#define		SERVERNUM		28
#define		DUNGEONNUM		25
#define		MULTI_NUM		3
#define		MAX_LEVEL		70  //pepsi_dungeon

struct  SUBSERVER
{
	int		SubServerIndex;
	TCHAR	pszChannelName[63][50];
	int		nChannelCount;
};
struct	SERVERINFO 
{
	TCHAR		szServerName[50];
	SUBSERVER   subServer[15];
	int			nSubServer;
};

extern	SERVERINFO		g_pInfoServer[SERVERNUM];

struct	IDINFO 
{
	WCHAR	szID[20];
	WCHAR	szPS[20];
//	WCHAR	szPadNum[5];
	WCHAR	szReceiver[100];
	WCHAR	szLimitCoin[10];
	WCHAR	szServer1[100];
	WCHAR	szServer2[100];
	WCHAR	szChannel[100];
	WCHAR	szStartChar[10];
};


typedef struct _tagGameSettings
{
	BYTE bySetting1;	// 0 : �������ޱ�, 1 : ī�弱��, 2 : �������ȱ�, 3 : �����ޱ� 4 : ������ 5: ������ ���� 6 : �۱ݹ�� (0:���� 1: ���)
	BYTE bySetting2;	// 0 : Ŀ��, 1 : ��Ŀ��, 2 : ����, 3 : ����ũ, 4 : ����, 5: �������� �Ǹ�
	WORD bySetting3;    // 0 : ī�� 1: ������ 2: �÷��� 3: ������ 4: ī�Ǿ߽� 5: ���� 6:���� 7 : ��Į 8 :���
	BYTE bySetting4;	// 0 : HP 1 : MP 2 : ���� 3: ������ 
	BYTE bySetting5;    // 0 : �Ƿε�
	BYTE bySetting6;	// 0 : ĳ������ð���� 1 : Ŭ���е嶸���� 2:������ 3:ĳ����ü 4:�Ƿε����� kan 0718
	BYTE bySetting7;	// �Ҹ�ǰ Ŀ��, 1 : ��Ŀ��, 2 : ����, 3 : ����ũ, 4 : ����, 5: �������� �Ǹ�
	BYTE bySetting8;    // ��� Ŀ��, 1 : ��Ŀ��, 2 : ����, 3 : ����ũ, 4 : ����, 5: �������� �Ǹ�
	BYTE bySlotIndex1;	// �Ҹ�ǰ ���� �Ѱ�
	BYTE bySlotIndex2;	// ��� ���� �Ѱ�
	BYTE byCardIndex;	// ī�� �ε���
	BYTE byTire;		// �Ƿε�
	BYTE byMultiNum;
	BYTE byHP;			// ����MHP
	BYTE byMP;			// ����MP
	BYTE byAttack;		// Attack Count
	BYTE byJBStength;	// Jangbi Strength
	BYTE byMulGong;		// 
	BYTE byTownMove;	// �����̵���� 0: �ɾ �̵� 1: �����̵�
	int  nCoin;			// ���μ�
	int  nRCoin;			// ���μ�
	int   nRcvMSrv;
	int   nRcvSSrv;
	WCHAR RcvName[50];
	
	WCHAR managerServerIP[16];
	char  MgServerIP[16];
	WCHAR szPath[MAX_PATH];
	int   StartTime;	// ���۽ð�
	int   nTimeInterVal;		// ���� ���ð�
	int   nAuctionItem; // ��Ź�ǰ

	// for manager server kan 1007
	DWORD comNumber;
	BOOL  bCleanpad;

	//{ pepsi_dungeon. ������ �������ÿ� ���� �������. 
	int	nDungeonIndexForLevel[MAX_LEVEL];
	//}
	//{ pepsi_ip 
	int nIpTimeInterval ;  //������ �ð��������� ������ ��ü . pepsi_ip 
	WCHAR szIP1[MAX_PATH];
	WCHAR szIP2[MAX_PATH];
	WCHAR szIP3[MAX_PATH];
	//}
} GAMESETTINGS, *PGAMESETTING;

struct LOGIN_INFO 
{
	BOOL			bEnable;
	DWORD			PID;
	WCHAR			GameID[MAX_PATH];
	SOCKET			sock;
	// kus 0503
	BYTE			byServer;
	BYTE			byChannel;
	BYTE			byCharac;
	BYTE			byPadNum[4];
	// kus 0419
	WCHAR			szReceiver[100];
	DWORD			nLmitCoin;
	GAMESETTINGS	gameSettings;
};

// kus 0910 ��Ƽ
struct MAC_INFO 
{
//	WCHAR		DnfPath[MAX_PATH];  // ���İ��	
	BYTE		MacAddr[6];			// MAC �ּ�
};


extern IDINFO		g_pInfoID[100];
extern	int			g_nIDNum;
extern GAMESETTINGS	g_gameSettings;
extern WCHAR		g_szDnFPath[MAX_PATH];
extern WCHAR		g_szDoumiPath[MAX_PATH];

extern int			g_nNoSellItem;
extern WCHAR		g_wzNoSellItemList[1200][40];

extern int			g_nAucItem;
extern WCHAR		g_wzAucItemList[100][40];
extern HANDLE		g_hLogOut;

extern MAC_INFO	g_MacInfo[MULTI_NUM]; 

extern	WCHAR		g_strAutoID[20];
extern	WCHAR		g_strAutoPwd[20];


#define AuthReq					0x10   
#define REQ_LOGIN				0x11
#define REQ_LOGOUT		   		0x12
#define REQ_PWD_CHANGE			0x13
#define REQ_LOGIN_ALEADY		0x14
#define REQ_AUTH_CLIENT			0x15
#define REQ_AUTO_STATE			0x16
#define REQ_AUTO_CLIENT			0x17
#define REQ_CLIENT_COUNT		0x18
#define REQ_UPDATE_GAME			0x19
#define RES_AUTH_SUCC			0x21
#define RES_AUTH_FAIL			0x22
#define RES_AUTH_NO				0x23
#define RES_OTHER_USED			0x24
#define RES_AUTO_VERSION		0x25
#define RES_AUTH_CLIENT			0x26
#define RES_PACKET_FAIL			0x27
#define RES_PW_CHANGE_SUCCESS	0x28
#define RES_PW_CHANGE_FAIL	    0x29
#define RES_CLIENT_COUNT		0x2A
#define RES_AUTH_CLIENT_FAIL	0x30
#define CTRL_SEVER_DIED			0x31
#define SERVICE_STOP   			0x33
#define SERVICE_ADD				0x34
#define RES_AUTO_STATE			0x56
#define REQ_USER_STATE			0x57

#endif