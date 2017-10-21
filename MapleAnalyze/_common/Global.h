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
	BYTE bySetting1;	// 0 : 아이템줏기, 1 : 카드선택, 2 : 아이템팔기, 3 : 전직받기 4 : 장비수리 5: 내구도 고정 6 : 송금방식 (0:우편 1: 경매)
	BYTE bySetting2;	// 0 : 커먼, 1 : 언커먼, 2 : 레어, 3 : 유니크, 4 : 에픽, 5: 모든아이템 판매
	WORD bySetting3;    // 0 : 카인 1: 디레지에 2: 시로코 3: 프레이 4: 카실야스 5: 힐더 6:안톤 7 : 바칼 8 :오즈마
	BYTE bySetting4;	// 0 : HP 1 : MP 2 : 무적 3: 공격핵 
	BYTE bySetting5;    // 0 : 피로도
	BYTE bySetting6;	// 0 : 캐릭사망시경고음 1 : 클린패드떳을시 2:비정상 3:캐릭교체 4:피로도부족 kan 0718
	BYTE bySetting7;	// 소모품 커먼, 1 : 언커먼, 2 : 레어, 3 : 유니크, 4 : 에픽, 5: 모든아이템 판매
	BYTE bySetting8;    // 재료 커먼, 1 : 언커먼, 2 : 레어, 3 : 유니크, 4 : 에픽, 5: 모든아이템 판매
	BYTE bySlotIndex1;	// 소모품 슬롯 한계
	BYTE bySlotIndex2;	// 재료 슬롯 한계
	BYTE byCardIndex;	// 카드 인덱스
	BYTE byTire;		// 피로도
	BYTE byMultiNum;
	BYTE byHP;			// 고정MHP
	BYTE byMP;			// 고정MP
	BYTE byAttack;		// Attack Count
	BYTE byJBStength;	// Jangbi Strength
	BYTE byMulGong;		// 
	BYTE byTownMove;	// 마을이동방식 0: 걸어서 이동 1: 순간이동
	int  nCoin;			// 코인수
	int  nRCoin;			// 코인수
	int   nRcvMSrv;
	int   nRcvSSrv;
	WCHAR RcvName[50];
	
	WCHAR managerServerIP[16];
	char  MgServerIP[16];
	WCHAR szPath[MAX_PATH];
	int   StartTime;	// 시작시간
	int   nTimeInterVal;		// 정지 대기시간
	int   nAuctionItem; // 경매물품

	// for manager server kan 1007
	DWORD comNumber;
	BOOL  bCleanpad;

	//{ pepsi_dungeon. 레벨별 던전선택에 대한 정보기록. 
	int	nDungeonIndexForLevel[MAX_LEVEL];
	//}
	//{ pepsi_ip 
	int nIpTimeInterval ;  //일정한 시간간격으로 아이피 교체 . pepsi_ip 
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

// kus 0910 멀티
struct MAC_INFO 
{
//	WCHAR		DnfPath[MAX_PATH];  // 던파경로	
	BYTE		MacAddr[6];			// MAC 주소
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