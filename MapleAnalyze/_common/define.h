
//

#pragma once
//#include "stdafx.h"

// 기타 정의들
#define DnFSERVER_DB_FILE		L"DnFServer.mdb"	// 자료기지파일

//에로코드와 에로문들.
#define	ERRCODE_OK						0		//에로없음
#define	ERRCODE_INVALID_ID				101		//무효한 아이디입니다.
#define	ERRCODE_INVALID_PWD				102		//패스워드가 정확하지 않습니다.
#define	ERRCODE_INVALID_LEVEL			103		//권한이 없습니다.로그인을 다시 하여 주십시오.
#define	ERRCODE_INVALID_DAY				104		//기한이 완료되였습니다. 
#define	ERRCODE_SAME_ID					105		//이미 같은 아이디가 있습니다.
#define	ERRCODE_SAME_USER				106		//이미 같은 아이디로 로그인 되여있습니다.\n\r중복로그인에 의해 로그아웃되였습니다.
#define	ERRCODE_GAME_LOGIN				107		//현재 게임에 로그인할수 없습니다.
#define	ERRCODE_GAME_GUARD				108		//현재 게임에 오유가 있습니다..
#define	ERRCODE_NO_LIST					109		//검색결과가 없습니다..
#define	ERRCODE_NO_CONIP				110		//접속할수 없는 아이피입니다..
#define	ERRCODE_UNKNOWN					120		//확인되지 않은 오유가 발생하였습니다..
#define	ERRCODE_INVALID_USE				111		//사용정지인 계정

#define	ERRSTR_OK						L"확인이 제바로 되였습니다."
#define	ERRSTR_INVALID_ID				L"ID错误."
#define	ERRSTR_INVALID_PWD				L"PW错误."
#define	ERRSTR_INVALID_LEVEL			L"权限错误."
#define	ERRSTR_INVALID_DAY				L"失效日期."
#define	ERRSTR_SAME_ID					L"相同的ID已经登录了."
#define	ERRSTR_SAME_USER				L"相同的用户."
#define	ERRSTR_GAME_LOGIN				L"현재 서버가드업데이트중입니다. 1분후 재시도 해주십시오"
#define	ERRSTR_GAME_GUARD				ERRSTR_GAME_LOGIN
#define	ERRSTR_NO_LIST					L"검색결과가 없습니다."
#define	ERRSTR_NO_CONIP					L"접속할수 없는 해외아이피입니다. VPN을 확인하십시오."
#define	ERRSTR_UNKNOWN					L"不知道的错误"
#define ERRSTR_INVALID_USE				L"사용정지된 계정입니다"


//요청의 종류
//클라이언트에서 올라오는 요청의 종류
#define	CLIENT_REQUEST_IDPWD			1001	//아이디/패스워드 인증
#define	CLIENT_REQUEST_TOKEN			1002	//실시간 인증
#define	CLIENT_REQUEST_FILEINFO			1003	//파일업데이트 정보 확인
#define	CLIENT_REQUEST_FILEDATA			1004	//파일을 전송해달라.
#define	CLIENT_REQUEST_NOTICE			1005	//공지 정보를 보내라.
#define	CLIENT_REQUEST_GUARD			1006	//가드정보를 보내라.
#define	CLIENT_REQUEST_LOGOUT			1007	//로그아웃
#define	CLIENT_REQUEST_GUARDUPDATE		1008	//가드업데이트하라..

// kan 110608
#define CLIENT_REQUEST_GOLD				1009	//메일로 전송되는 골드량 오토서버에 보내기 kan 110601
#define CLIENT_REQUEST_LOG_IMPORT		1010	//오토서버로부터 저장된 골드량 보내달라 kan 110603
#define	CLIENT_REQUEST_ACCOUNT			1011	//오토서버에 계정보내기  kan 110603
//

//kan 0707
#define CLIENT_REQUEST_CLEANPAD			1012

// kan 0803
#define CLIENT_REQUEST_ILLEGALUSER		1013 // 불법사용자를 오토서버에 보내기


#define CLIENT_REQUEST_UPDATEDOWN		1014 // kan 0807 다운로드파일 불러오기 요청


//관리프로그람에서 올라오는 요청의 종류
#define	MANAGE_REQUEST_IDPWD			2001	//아이디/패스워드 인증
#define	MANAGE_REQUEST_MANAGERLIST		2002	//관리자 정보를 보내라.
#define	MANAGE_REQUEST_MANAGERADD		2003	//관리자 정보를 추가하라.
#define	MANAGE_REQUEST_MANAGERCHANGE	2004	//관리자 정보를 변경하라.
#define	MANAGE_REQUEST_MANAGERDELETE	2005	//관리자 정보를 삭제하라.
#define	MANAGE_REQUEST_USERLIST			2006	//유저 정보를 보내라.
#define	MANAGE_REQUEST_USERADD			2007	//유저 정보를 추가하라.
#define	MANAGE_REQUEST_USERCHANGE		2008	//유저 정보를 변경하라.
#define	MANAGE_REQUEST_USERDELETE		2009	//유저 정보를 삭제하라.
#define	MANAGE_REQUEST_NOTICELIST		2010	//공지 정보를 보내라.
#define	MANAGE_REQUEST_NOTICEADD		2011	//공지 정보를 추가하라.
#define	MANAGE_REQUEST_NOTICECHANGE		2012	//공지 정보를 변경하라.
#define	MANAGE_REQUEST_NOTICEDELETE		2013	//공지 정보를 삭제하라.
#define	MANAGE_REQUEST_LOGOUT			2014	//로그아웃
#define	MANAGE_REQUEST_USERINFO			2015	//유저의 구체적인 정보를 보내라.
#define	MANAGE_REQUEST_USERLOGOUT		2016	//유저를 강제로그아웃하라.


//업데이트프로그람에서 올라오는 요청의 종류
#define	UPDATE_REQUEST_FILEINFO			3001	//파일업데이트 정보 확인
#define	UPDATE_REQUEST_FILEDATA			3002	//파일을 전송하라.
#define UPDATE_REQUEST_FILEINFO_END		3003	// 복사파일을 지워라 kan 0808
#define	UPDATE_REQUEST_FILEINFO_NEW		3004	//파일업데이트 정보 확인


#define	CUR_PROGRAM_IDENTIFYER			L"DnFUpdate.ini" //프로그람식별자를 하나 만들어놓고 리용하자..

struct FILE_INFO 
{
	WCHAR	filename[128];
	DWORD	size;
	BYTE	hash[20];
};
#define MAX_FILE_NUM				(300) //파일 무결성 검사 개수
#define	CUR_FILE_LIST				L"DnFUpdate.ini" //무결성 검사 파일 리스트.

//유저정보 구조체
struct USER_INFO 
{
	WCHAR	id[20];
	WCHAR	pwd[20];
	WCHAR	GameID[20];
	DWORD   dwMoney;
	WORD	number;
	WORD	status;
	WCHAR	addrIp[20];
	bool	bUse;
};

// 유저목록을 위한 개별 구조체.
struct USER_INFOEX
{
	WCHAR	id[20];
	WCHAR	pwd[20];
	WCHAR	master[20];
	WORD	number;
	WORD	status;
};

//게임유저정보 구조체
struct GAMEUSER_INFO 
{
	WCHAR	id[20];
	WCHAR	master[20];
	SYSTEMTIME	lastTime;
	WORD	status;
};

//로그인된 유저정보 구조체
struct LOGIN_USER_INFO 
{
	WCHAR		id[20];
	WCHAR		pwd[20];
	WORD		level;	//권한
	WORD		day;	//남은 인증일수(시간수)
	SYSTEMTIME	loginTime;	//로그인 한 시간
	SYSTEMTIME	minusedTime;//(마지막으로 덜어버린 시간..1시간에 한번씩 던다.) 
	SYSTEMTIME	lastTime;	//마지막으로 통신한 시간
	SYSTEMTIME	regTime;	//마지막으로 로그인한 시간
	BYTE		token[20];	//토큰
	BOOL		dbFlag;		//디비조작이 성공하였는가 하는 기발
	WORD		errCode;	//디비조작의 결과
	CDaoRecordset * pRecordset;//디비조작의 결과를 받는 레코드셋. 스레드함수에서 호출되기 떄문,,
	WORD		status;		
	WORD		userNumber;		
};

#define  NOTICE_MAX_SIZE	1000
//공지정보 구조체
struct NOTICE_INFO 
{
	DWORD		id;
	WCHAR		managerId[20];
	WCHAR		title[50];
	WCHAR		content[NOTICE_MAX_SIZE];
	SYSTEMTIME	time;
	WORD		status;
};

//관리자의 권한을 구분하자
#define		ADMIN_USER			(1)		//유저: 이때에는 인증일자이다.
#define		ADMIN_MANAGEUSER	(2)		//유저를 관리할수 있는 관리자.
#define		ADMIN_MANAGEMANAGER	(4)		//유저와 관리자, 공지를 관리할수 잇는 관리자. 
#define		ADMIN_MASTER		(8)		//최고권한의 관리자.

#define		USER_STATE_LOGOUT	(0)		//유저가 로그아웃 되여있다.
#define		USER_STATE_LOGIN	(1)		//유저가 로그인 되여있다.

//디비관련 메시지들..
#define		DB_USER_LOGIN		(WM_USER + 1)//유저가 로그인 하려고 한다는 메시지..
#define		DB_USER_QUERY		(WM_USER + 2)//유저가 디비에서 조회 하려고 한다는 메시지..
#define		DB_USER_QUERY_END	(WM_USER + 3)//유저가 디비에서 조회 하려고 한다는 메시지..
#define		DB_USER_EXEQSL		(WM_USER + 4)//유저가 디비에서 실행 하려고 한다는 메시지..
#define		DB_GAMEUSER_LOGIN	(WM_USER + 5)//유저가 로그인 하려고 한다는 메시지..
#define		DB_GET_GAMEUSER_INFO	(WM_USER + 6)//유저가 로그인 하려고 한다는 메시지..
#define		DB_GAMEUSER_GAMEID_LOGIN	(WM_USER + 7)//클라이언트에서 리니지 계정이 로그인 하였다는 메시지..

#define		DB_USER_GETUSERINFO		(WM_USER + 11)//레코드셋에서 정보를 얻는 함수, 스레드로부터 호출....
#define		DB_USER_GETNOTICEINFO	(WM_USER + 12)//레코드셋에서 정보를 얻는 함수, 스레드로부터 호출....
#define		DB_GAMEUSER_GETUSERINFO	(WM_USER + 13)//레코드셋에서 게임유저정보를 얻는 함수, 스레드로부터 호출....
#define		DB_USER_GETMANAGEINFO	(WM_USER + 14)//레코드셋에서 정보를 얻는 함수, 스레드로부터 호출....

#define		DB_USER_MONEYINFO		(WM_USER + 15)//현재 게임 진행중인 유저의 Money 상태를 DB에 반영
#define		DB_USER_COAHINFO		(WM_USER + 16)//코치 생성시 정보등록