//--------------------------------------------------------
//		통신 자료 해석/작성 관련 소스
//--------------------------------------------------------
#pragma once

//#include "winsock2.h"


#define MAX_IDPWD_LEN	(20)
// 오토계정의 유효기간 완료
#define		ERROR_AUTO_ACCOUNT_PAST				(-3)
// 오토계정이 등록되지 않음
#define		ERROR_AUTO_ACCOUNT_INVALID_ID		(-4)
// 오토계정의 비밀번호가 틀림
#define		ERROR_AUTO_ACCOUNT_INVALID_PWD		(-5)
// 일반 오류
#define		ERROR_GENERAL						(-1)
// 버전이 일치하지 않는다
#define		ERROR_VERSION_DIFF					(-6)
// 권한이 없습니다.
#define		ERROR_INVALID_DEGREE				(-7)
// 계정 추가에 실패.
#define		ERROR_ID_ADD_FAIL					(-8)
// 계정 삭제에 실패.
#define		ERROR_ID_REMOVE_FAIL				(-9)

#define		STR_CANT_CONNECT_SERVER				L"轟랬젯쌈돕륩蛟포."
#define		STR_DONT_RESPONS_SERVER				L"륩蛟포청唐捲壇."
#define		STR_INVALID_RESPONS_SERVER			L"륩蛟포돨捲壇角꼇攣횅돨."

#define		STR_CONFIRM_NETWORK_FIREWALL		L"렝삽퓰羸岺貢쭹."

#define		VERSION_MAJOR						0x0100
#define		VERSION_MINOR						0x0200

#define		PACKET_BLOCK_SIZE					(4096)
#define		PACKET_MAX_SIZE						(PACKET_BLOCK_SIZE * 16)

// 지정서버에 소켓을 창조하고 접속한다.
BOOL ConnectServer(IN CHAR* serverIP, WORD serverPort, OUT SOCKET &p_conn, OUT WCHAR* strMsg);
//소켓을 닫는다.
BOOL CloseConnect(IN SOCKET &p_conn);
//데이터를 암호화하여 송신한다.
BOOL SendData(IN SOCKET &p_conn, IN void* sendBuffer, IN int sendLen, OUT WCHAR* strMsg);
//데이터를 구조화하여 파켓송신부에 넘긴다.
BOOL SendOpData(IN SOCKET &p_conn, IN WORD opCode, IN void* sendBuffer, IN int sendLen, OUT WCHAR* strMsg = NULL);
//데이터를 수신하여 복호화하여 무결성을 판정한다.
BOOL RecvData(IN SOCKET &p_conn, IN void* recvBuffer, IN int& recvLen, OUT WCHAR* strMsg);
//데이터를 구조화하여 받는다.
BOOL RecvOpData(IN SOCKET &p_conn, OUT WORD& opCode, OUT void* recvBuffer, OUT int& recvLen, OUT WCHAR* strMsg);
//지정된 파일을 송신한다.
BOOL SendFile(SOCKET &sock, LPCTSTR fileName);
//파일데이터를 수신하여 지정된 파일로 쓴다.
BOOL WriteFile(SOCKET &sock, LPCTSTR fileName);

BOOL UpdatePortNo(OUT WORD *p_PortNo, IN WCHAR* strPath);
BOOL UpdateIpString(OUT CHAR *p_IpString, IN WCHAR* strPath);
