// define_atmon.h

// kan 0621 클라이언트에서 오토감시서버(Atmon)에 올려보내는 요청
#define CLIENT_MON_AUTO_LOGIN			4001	// 오토계정 올려보내기
#define CLIENT_MON_GAME_LOGIN			4002	// 게임계정 올려보내기
#define CLIENT_MON_GAME_SERVER_ID		4003	// 실행중인 게임서버올려보내기
#define CLIENT_MON_GAME_CHARAC_ID		4004	// 게임 캐랙 네임 올려보내기
#define CLIENT_MON_GAME_DUNGEON_ID		4005	// 던전 ID
#define CLIENT_MON_GAME_CHARAC_INFO		4006	// 현재 캐랙의 골드, 피로도, 레벨 보내기
#define CLIENT_MON_GAME_STATE			4007	// 게임의 동작상태 올려보내기
#define CLIENT_MON_AUTO_LOGOUT			4008	// 오토계정 로그아웃
#define CLIENT_MON_GAME_LOGOUT			4009	// 게임계정 로그아웃

#define CLIENT_MON_GAME_CLEANPAD		4010	// 클린패드
#define CLIENT_MON_GAME_ABNORMAL		4011	// 비정상동작
#define CLIENT_MON_TERM_GAME_CHECK		4012	// 전체 강종 체크
#define CLIENT_MON_AUTO_START			4013	// 오토시작

// 게임 상태

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
	DWORD	dwReversed; // 종료형태: 비정상종료, 정상종료....
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
	DWORD dwReversed; // 종료형태: 비정상종료, 정상종료....
};


//

