//--------------------------------------------------------
//		��� �ڷ� �ؼ�/�ۼ� ���� �ҽ�
//--------------------------------------------------------
#pragma once

//#include "winsock2.h"


#define MAX_IDPWD_LEN	(20)
// ��������� ��ȿ�Ⱓ �Ϸ�
#define		ERROR_AUTO_ACCOUNT_PAST				(-3)
// ��������� ��ϵ��� ����
#define		ERROR_AUTO_ACCOUNT_INVALID_ID		(-4)
// ��������� ��й�ȣ�� Ʋ��
#define		ERROR_AUTO_ACCOUNT_INVALID_PWD		(-5)
// �Ϲ� ����
#define		ERROR_GENERAL						(-1)
// ������ ��ġ���� �ʴ´�
#define		ERROR_VERSION_DIFF					(-6)
// ������ �����ϴ�.
#define		ERROR_INVALID_DEGREE				(-7)
// ���� �߰��� ����.
#define		ERROR_ID_ADD_FAIL					(-8)
// ���� ������ ����.
#define		ERROR_ID_REMOVE_FAIL				(-9)

#define		STR_CANT_CONNECT_SERVER				L"�޷����ӵ�������."
#define		STR_DONT_RESPONS_SERVER				L"������û����Ӧ."
#define		STR_INVALID_RESPONS_SERVER			L"����������Ӧ�ǲ���ȷ��."

#define		STR_CONFIRM_NETWORK_FIREWALL		L"����ǽ��ֹ����."

#define		VERSION_MAJOR						0x0100
#define		VERSION_MINOR						0x0200

#define		PACKET_BLOCK_SIZE					(4096)
#define		PACKET_MAX_SIZE						(PACKET_BLOCK_SIZE * 16)

// ���������� ������ â���ϰ� �����Ѵ�.
BOOL ConnectServer(IN CHAR* serverIP, WORD serverPort, OUT SOCKET &p_conn, OUT WCHAR* strMsg);
//������ �ݴ´�.
BOOL CloseConnect(IN SOCKET &p_conn);
//�����͸� ��ȣȭ�Ͽ� �۽��Ѵ�.
BOOL SendData(IN SOCKET &p_conn, IN void* sendBuffer, IN int sendLen, OUT WCHAR* strMsg);
//�����͸� ����ȭ�Ͽ� ���ϼ۽źο� �ѱ��.
BOOL SendOpData(IN SOCKET &p_conn, IN WORD opCode, IN void* sendBuffer, IN int sendLen, OUT WCHAR* strMsg = NULL);
//�����͸� �����Ͽ� ��ȣȭ�Ͽ� ���Ἲ�� �����Ѵ�.
BOOL RecvData(IN SOCKET &p_conn, IN void* recvBuffer, IN int& recvLen, OUT WCHAR* strMsg);
//�����͸� ����ȭ�Ͽ� �޴´�.
BOOL RecvOpData(IN SOCKET &p_conn, OUT WORD& opCode, OUT void* recvBuffer, OUT int& recvLen, OUT WCHAR* strMsg);
//������ ������ �۽��Ѵ�.
BOOL SendFile(SOCKET &sock, LPCTSTR fileName);
//���ϵ����͸� �����Ͽ� ������ ���Ϸ� ����.
BOOL WriteFile(SOCKET &sock, LPCTSTR fileName);

BOOL UpdatePortNo(OUT WORD *p_PortNo, IN WCHAR* strPath);
BOOL UpdateIpString(OUT CHAR *p_IpString, IN WCHAR* strPath);
