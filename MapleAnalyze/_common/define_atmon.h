// define_atmon.h

// kan 0621 Ŭ���̾�Ʈ���� ���䰨�ü���(Atmon)�� �÷������� ��û
#define CLIENT_MON_AUTO_LOGIN			4001	// ������� �÷�������
#define CLIENT_MON_GAME_LOGIN			4002	// ���Ӱ��� �÷�������
#define CLIENT_MON_GAME_SERVER_ID		4003	// �������� ���Ӽ����÷�������
#define CLIENT_MON_GAME_CHARAC_ID		4004	// ���� ĳ�� ���� �÷�������
#define CLIENT_MON_GAME_DUNGEON_ID		4005	// ���� ID
#define CLIENT_MON_GAME_CHARAC_INFO		4006	// ���� ĳ���� ���, �Ƿε�, ���� ������
#define CLIENT_MON_GAME_STATE			4007	// ������ ���ۻ��� �÷�������
#define CLIENT_MON_AUTO_LOGOUT			4008	// ������� �α׾ƿ�
#define CLIENT_MON_GAME_LOGOUT			4009	// ���Ӱ��� �α׾ƿ�

#define CLIENT_MON_GAME_CLEANPAD		4010	// Ŭ���е�
#define CLIENT_MON_GAME_ABNORMAL		4011	// ��������
#define CLIENT_MON_TERM_GAME_CHECK		4012	// ��ü ���� üũ
#define CLIENT_MON_AUTO_START			4013	// �������

// ���� ����

//

/*
struct ATMONINFO
{
	WCHAR	wszIpAddr[50];
	WCHAR	wszAutoID[50];
	WCHAR	wszGameID[50];
	WCHAR	wszStartTime[50];
	WCHAR	wszEndTime[50];
	WCHAR	wszServerID[50];
	WCHAR	wszCharacID[400];
	WCHAR	wszDungeonID[50];
	DWORD	nLevel;
	DWORD	nGold;
	DWORD	nTire;
	WCHAR	wszState[400];
	DWORD	dwReversed; // ��������: ����������, ��������....
};
*/

struct ATMONINFO
{
	DWORD comNumber;
	WCHAR wszIpAddr[50];
	WCHAR wszAutoID[50];
	WCHAR wszGameID[50];
	WCHAR wszStartTime[50];
	WCHAR wszEndTime[50];
	WCHAR wszServerID[50];
	WCHAR wszCharacID[50];
	WCHAR wszDungeonID[50];
	DWORD nLevel;
	DWORD nGold;
	DWORD nTire;
	WCHAR wszState[100];
	DWORD dwReversed; // ��������: ����������, ��������....
};


//

