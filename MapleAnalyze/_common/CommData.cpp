//--------------------------------------------------------
//		통신 자료 해석/작성 관련 소스
//--------------------------------------------------------

#include "stdafx.h"

#include "CommData.h"

#include "Encrypt.h"

char ProVer[6] ="DF100";

DWORD g_recvlen = 0;

// 파일이 있고 포트번호가 유효한 경우에만 설정해준다. 2009.3.24
BOOL UpdatePortNo(OUT WORD *p_PortNo, IN WCHAR* strPath)
{
	//포트 얻기
	BOOL r = FALSE;
	FILE		*fp;
	if(strPath == NULL || strPath[0] == 0) return FALSE;
	fp = _wfopen(strPath, L"rb");
	if(fp!=NULL)
	{	
		char buf[100] = {0};
		DWORD port = 0;
		fscanf(fp,"%d",&port);
		if(port > 0 && port < 0xFFFF)
		{
			*p_PortNo = (WORD)port;
			r = TRUE;
		}
		fclose(fp);fp=NULL;
	}
	return r;

}

// 파일이 존재하는 경우 첫단어에서 서버 IP 문자렬(최대16바이트)을 얻는다. "192.168.0.199";
BOOL UpdateIpString(OUT CHAR *p_IpString, IN WCHAR* strPath)
{
	//IP 문자렬 얻기
	BOOL r = FALSE;
	FILE		*fp;
	if(strPath == NULL || strPath[0] == 0) return FALSE;
	fp = _wfopen(strPath, L"rb");
	if(fp!=NULL)
	{
		char buf[100] = {0};
		fscanf(fp,"%s",buf);
		memcpy(p_IpString, buf, 16);
		//OutputDebugStringA(p_IpString);
		fclose(fp);fp=NULL;
	}
	return r;
}

/* =============================================================================================== 

함수설명: 

지정서버에 소켓을 창조하고 접속한다.

파라메터: 

serverIP : 서버주소 : "192.168.0.111"

p_conn : 접속된 소켓. 성공한 경우에만 유효하다.

strMsg : 실패시 출력정보를 얻는다. 최소 200 바이트로 주거나 NULL 로 준다.

귀환값: 

TRUE/FALSE	

=============================================================================================== */
BOOL ConnectServer(IN CHAR* serverIP, WORD serverPort, OUT SOCKET &p_conn, OUT WCHAR* strMsg)
{
	WCHAR str[1024] = {0};
	WCHAR *strResult = str;

	// 출력정보 설정&초기화
	if(strMsg != NULL)
		strResult = strMsg;
	strResult[0] = 0;

	WSADATA wsaData;
	struct hostent *hp;
	unsigned int addr = 0;
	sockaddr_in serverAddr;
	BOOL r = FALSE;

	// 접속한다.
	int err = 0, wsaRet;

	wsaRet = WSAStartup(0x101, &wsaData);

	if(wsaRet)	
	{
		wcscpy(strResult, STR_CANT_CONNECT_SERVER);
		wcscat(strResult, L"\n");
		wcscat(strResult, STR_CONFIRM_NETWORK_FIREWALL);
		return FALSE;
	}

	// 소켓 창조
	p_conn = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(p_conn == INVALID_SOCKET)
	{
		wcscpy(strResult, STR_CANT_CONNECT_SERVER);
		wcscat(strResult, L"\n");
		wcscat(strResult, STR_CONFIRM_NETWORK_FIREWALL);
		return FALSE;
	}

	// 호스트 얻기
	hp = gethostbyname(serverIP);
	if(hp == NULL)
	{
		addr = inet_addr(serverIP);
		hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	}

	// 호스트 얻기 2번째 방법
	if(hp == NULL)
	{
		wcscpy(strResult, STR_CANT_CONNECT_SERVER);
		wcscat(strResult, L"\n");
		wcscat(strResult, STR_CONFIRM_NETWORK_FIREWALL);
		goto loc_exit_fail;	
	}

	// 기타 정보들의 설정
	serverAddr.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);

	// 접속하기
	if(connect(p_conn, (struct sockaddr*)&serverAddr, sizeof(serverAddr)))
	{
		wcscpy(strResult, STR_DONT_RESPONS_SERVER);
		wcscat(strResult, L"\n");
		wcscat(strResult, STR_CONFIRM_NETWORK_FIREWALL);
		goto loc_exit_fail;		
	}

	// 성공적으로 소켓 창조되였다.
	r = TRUE;

loc_exit_fail:

	if(r == FALSE && p_conn != NULL)
	{
		// 실패한 경우에는 소켓을 닫는다.
		closesocket(p_conn);
		p_conn = NULL;
	}

	return r;

}

BOOL CloseConnect(IN SOCKET &p_conn)
{
	if(p_conn != NULL)
	{
		// 실패한 경우에는 소켓을 닫는다.
		if(closesocket(p_conn) != 0)
		{
			return FALSE;
		}
		p_conn = NULL;
	}
	return TRUE;

}

/* ===============================================================================================

함수설명:

소켓을 통해 지정한 데이터를 전송한다.

=============================================================================================== */

BOOL SendData(IN SOCKET &p_conn, IN void* sendBuffer, IN int sendLen, OUT WCHAR* strMsg)
{
	// 	WCHAR str[1024] = {0};
	// 	WCHAR *strResult = str;
	// 
	// 
	// 	if(strMsg != NULL)
	// 		strResult = strMsg;
	// 	strResult[0] = 0;
	// 

	BYTE *buf = NULL; int nLen = 0, n = 0;
	BOOL r = FALSE;
	BYTE *sendBuf;
	int kk;

	if(p_conn == NULL) goto loc_exit_fail;	

	// 암호화 //현재 보내려는 데이터를 반전한다..
	for(kk = 0; kk< sendLen; kk++)
	{
		((BYTE *)sendBuffer)[kk] = ~((BYTE *)sendBuffer)[kk];
	}

	sendBuf = new BYTE[sendLen + 2 + 5]; //주의 :식별자 5바이트가 더 붙는다. 
	*((WORD *)sendBuf) = (WORD)sendLen + 5;
	memcpy(&sendBuf[2], ProVer, 5);				//주의 :식별자 5바이트가 더 붙는다.
	//길이 송신.
	memcpy(&sendBuf[7], sendBuffer, sendLen);
	sendLen += 7;								//주의 :식별자 5바이트가 더 붙는다.

retry_loop:
	n = send(p_conn, (char*)sendBuf, sendLen, 0);
	//	n = send(p_conn, (char*)sendBuffer, sendLen, 0);
	if (n == SOCKET_ERROR || n != sendLen)
	{
		// 		wcscpy(strResult, STR_DONT_RESPONS_SERVER);
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			Sleep(10);
			goto retry_loop;
		}
		PrintLog(L"소켓에러 socket %d %d %d %d", p_conn, WSAGetLastError(), n, sendLen);
		goto loc_exit_fail;		
	}

	r = TRUE;

loc_exit_fail:

	// 	if(buf) delete buf; buf = NULL;
	if(sendBuf) delete sendBuf; sendBuf = NULL;

	return r;

}

BOOL SendOpData(IN SOCKET &p_conn, IN WORD opCode, IN void* sendBuffer, IN int sendLen, OUT WCHAR* strMsg)
{
	
	WCHAR str[1024] = {0};
	WCHAR *strResult = str;
	BYTE *sendBuf;
	int curPos;
	if(strMsg != NULL)
		strResult = strMsg;
	strResult[0] = 0;

	BOOL r = FALSE;
	if(p_conn == NULL) goto loc_exit_fail;	

	if(sendLen > PACKET_MAX_SIZE - 6) sendLen = PACKET_MAX_SIZE - 6; //패킷이 너무 크다면 좀 자른다..실패시킬가?(-2는 오피코드, 4:길이용)
	sendBuf = new BYTE[PACKET_BLOCK_SIZE]; 

	//첫 파케트에는 OP코드, 패킷의 길이, 데이터를 준다.
	//다 주지 못한 데이터는 련달아 준다. 이때에는 데이터만 송신한다. 
	curPos = sendLen;
	if(sendLen > PACKET_BLOCK_SIZE - 6) curPos = (PACKET_BLOCK_SIZE - 6);

	*((WORD *)sendBuf) = (WORD)opCode;
	*((DWORD *)(&sendBuf[2])) = (DWORD)sendLen;

	if(sendBuffer == NULL && sendLen == 0)
	{
		//이경우는 op코드만을 통신하기 위해서이다, 일반적으로 오류 코드들을 그렇게 보낸다.
		if(!SendData(p_conn, sendBuf, curPos + 6, strResult))
			goto loc_exit_fail;
	}
	else
	{
		memcpy(&sendBuf[6], sendBuffer, curPos);
		if(!SendData(p_conn, sendBuf, curPos + 6, strResult))
			goto loc_exit_fail;

		while(sendLen > curPos)
		{
			int len;
			if(sendLen - curPos > PACKET_BLOCK_SIZE) len = PACKET_BLOCK_SIZE;
			else len = sendLen - curPos;
			memcpy(sendBuf, &(((BYTE *)sendBuffer)[curPos]), len);
			if(!SendData(p_conn, sendBuf, len, strResult))
				goto loc_exit_fail;
			curPos += len;
		}
	}

	r = TRUE;

loc_exit_fail:

	if(sendBuf) delete sendBuf; sendBuf = NULL;

	
	return r;

}

// 송신하고 수신버퍼를 할당하여 되돌린다. 현재는 고정길이라고 본다.
BOOL RecvData(IN SOCKET &p_conn, IN void* recvBuffer, IN int& recvLen, OUT WCHAR* strMsg)
{

	WCHAR str[1024] = {0};
	WCHAR *strResult = str;
	int kk;
	char t_buf[200] = {0};
	WORD m_Length = 0;
	if(strMsg != NULL)
		strResult = strMsg;
	strResult[0] = 0;

	BYTE *buf = NULL; int nLen = 0, n = 0;
	BOOL r = FALSE;

	if(p_conn == NULL) goto loc_exit_fail;	
	if(recvBuffer == NULL) goto loc_exit_fail;	

	// 수신한다.
	// 먼저 2바이트 읽고 길이를 얻는다. 다음 그 길이가 될 때까지 나머지 길이를 읽기한다.
	
	
recvLoop1:
	n = recv(p_conn, t_buf, 2, 0); //만일 1바이트가 읽어졌다면..
	if (n == SOCKET_ERROR || n == 0)
	{
		PrintLog(L"수신에러 -- 1 %d", WSAGetLastError());
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			Sleep(10);
			goto recvLoop1;
		}
		wcscpy(strResult, STR_INVALID_RESPONS_SERVER);
		goto loc_exit_fail;	
	}

	m_Length = *(WORD*)t_buf;

	// 다음 완충기에 크기만큼 복사해주고 길이를 넣어준다..
	nLen = 0;
	buf = new BYTE[m_Length + 2];
	while((int)m_Length > nLen)
	{
		n = recv(p_conn, (char*)(&buf[nLen]), (int)m_Length - nLen, 0);
		if (n == SOCKET_ERROR || n == 0)
		{
			wcscpy(strResult, STR_INVALID_RESPONS_SERVER);
			int sdd = WSAGetLastError();
			sdd += 10;
			goto loc_exit_fail;	
		}
		nLen += n;
	}

	//식별자 검사
	if(nLen < 5) goto loc_exit_fail;
	else nLen -= 5;

	if(memcmp(ProVer, buf, 5) != 0) //주의 :식별자 5바이트가 더 붙는다.
	{
		wcscpy(strResult, STR_INVALID_RESPONS_SERVER);
		goto loc_exit_fail;	
	}

	// 	// 복호화 //현재 받은 데이터를 반전한다..실제 암호화된 데이터는 5번째 바이트부터 있다.
	for(kk = 0; kk< nLen; kk++)
	{
		((BYTE *)buf)[5 + kk] = ~((BYTE *)buf)[5 + kk];
	}

	//받아갈 완충기의 크기를 비교해보고 복사해준다.??
	if(recvLen >= nLen)
	{
		memcpy(recvBuffer,&buf[5],nLen); //주의 :식별자 5바이트가 더 붙는다.
		recvLen = nLen;
	}
	else
	{
		memcpy(recvBuffer,&buf[5],recvLen); //주의 :식별자 5바이트가 더 붙는다.
	}
	//memcpy(recvBuffer,buf,n); 

	r = TRUE;

loc_exit_fail:

	if(buf) delete buf; buf = NULL;
	//	if(recvBuf) delete recvBuf; recvBuf = NULL;

	return r;

}

//데이터를 구조화하여 받는다.
BOOL RecvOpData(IN SOCKET &p_conn, OUT WORD& opCode, OUT void* recvBuffer, OUT int& recvLen, OUT WCHAR* strMsg)
{
	WCHAR str[1024] = {0};
	WCHAR *strResult = str;
	DWORD n_recvLen;
	if(strMsg != NULL)
		strResult = strMsg;
	strResult[0] = 0;

	BYTE *buf = NULL; int nLen = 0, curPos = 0, readedLength = 0;
	BOOL r = FALSE;

	if(p_conn == NULL) goto loc_exit_fail;	
	if(recvBuffer == NULL) goto loc_exit_fail;	

	// 패킷을 수신한다. 
	//첫번쨰 수신에서 op코드와 전송될 데이터의 크기를 얻으며 이에 따라 그 크기가 다 읽어질 때까지 읽기한다.
	nLen = PACKET_BLOCK_SIZE;
	buf = new BYTE[nLen];

	if(!RecvData(p_conn, buf, nLen, strResult))
		goto loc_exit_fail;

	nLen = nLen - 6;//첫 패킷에는 2바이트 OP코드, 4바이트는 길이 코드가 들어가 있음으로..
	if(nLen < 0) 
	{
		goto loc_exit_fail;
	}
	opCode = *(WORD*)(&buf[0]);
	n_recvLen = *(DWORD*)(&buf[2]);

	//받아갈 완충기가 충분히 큰가? 만일 크지 않다면..//복사는 하지 못해도 다음을 위해완충기는 비워야 한다.
	if(recvLen >= curPos + nLen)
	{
		memcpy(&((BYTE*)recvBuffer)[curPos],&buf[6],nLen);
		curPos = nLen ;
	}
	else if(recvLen > curPos) 
	{
		memcpy(&((BYTE*)recvBuffer)[curPos],&buf[6], recvLen - curPos);
		curPos = recvLen - curPos;
	}
	readedLength = nLen;

	while(n_recvLen > readedLength)
	{
		nLen = PACKET_BLOCK_SIZE;
		if(!RecvData(p_conn, buf, nLen, strResult))
			goto loc_exit_fail;
		if(readedLength + nLen > PACKET_MAX_SIZE) //들어온 패킷이 너무 크다면..//이것은 규정적인것은 아닌데..
			goto loc_exit_fail;
		//받아갈 완충기가 충분히 큰가? 만일 크지 않다면...//복사는 하지 못해도 다음을 위해완충기는 비워야 한다.
		if(recvLen >= curPos + nLen)
		{
			memcpy(&((BYTE*)recvBuffer)[curPos],buf,nLen);
			curPos += nLen ;
		}
		else if(recvLen > curPos) 
		{
			memcpy(&((BYTE*)recvBuffer)[curPos],buf, recvLen - curPos);
			curPos += recvLen - curPos;
		}
		readedLength += nLen;
	}

	recvLen = curPos;
	r = TRUE;

loc_exit_fail:

	if(buf) delete buf; buf = NULL;

	return r;

}

/* ===============================================================================================

함수설명:

소켓을 통해 지정한 파일을 전송한다.

=============================================================================================== */

// kan 0902 클린수정
BOOL SendFile(SOCKET &sock, LPCTSTR fileName)
{
	CFile file;
	char buf[PACKET_BLOCK_SIZE];
	int sendlen;
	int n;	
	DWORD flen;
	int totallen = 0;

	try
	{
		if(!file.Open(fileName, CFile::modeRead))
		{			
			return FALSE;
		}

		flen = file.GetLength();

		// 먼저 파일의 크기를 전송한다.		
		sendlen = sizeof(flen);
retry_sendfile_loop1:
		n = send(sock, (char*)&flen, sendlen, 0);
		if(n == SOCKET_ERROR || n != sendlen)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				Sleep(10);
				goto retry_sendfile_loop1;
			}
			return FALSE;
		}

		while((sendlen = file.Read(buf, PACKET_BLOCK_SIZE)) > 0)
		{
retry_sendfile_loop2:
			n = send(sock, buf, sendlen, 0);
			if(n == SOCKET_ERROR || n != sendlen)
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					Sleep(10);
					goto retry_sendfile_loop2;
				}
				break;
			}
			totallen += sendlen;
		}
		file.Close();
	}
	catch (CException* e)
	{
		WCHAR errBuf[1024];
		e->GetErrorMessage(errBuf, 1024);
		//MyOutputDebugString(DBG_LING3, DBG_ERR, L"Server : 파일전송에서 예외 [%s]",errBuf);
		e->Delete();
	}

	if (totallen < flen)
	{
		//MyOutputDebugString(DBG_LING3, DBG_ERR, L"Server : 파일전송에서 전송자료의 크기가 파일크기보다 작습니다.[%d/%d]",nSentSize , nFileSize);
		return FALSE;
	}
	return TRUE;
}

// kan 0902 클린패드보내기 수정
BOOL WriteFile(SOCKET &sock, LPCTSTR fileName)
{
	CFile file;
	char buf[PACKET_BLOCK_SIZE];

	int recvlen = 0;
	int n;	
	DWORD flen;	

	try
	{
		if(!file.Open(fileName, CFile::modeCreate | CFile::modeWrite))
		{
			//MyOutputDebugString(DBG_LING3, DBG_ERR, L"Server : 파일전송에서 파일생성 실패(%s)", fileName);
			return FALSE;
		}

		// 먼저 파일의 크기를 송신한다.
retry_writefile_loop1:
		n = recv(sock, (char*)&flen, sizeof(flen), 0);
		if(n == SOCKET_ERROR || n != sizeof(flen))
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				Sleep(10);
				goto retry_writefile_loop1;
			}
			return FALSE;
		}

		while(recvlen < flen)
		{
retry_writefile_loop2:
			n = recv(sock, buf, PACKET_BLOCK_SIZE, 0);
			if(n == SOCKET_ERROR || n == 0)
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					Sleep(10);
					goto retry_writefile_loop2;
				}
				break;
			}
			file.Write(buf, n);
			recvlen += n;
		}
		file.Close();
	}
	catch (CException* e)
	{
		WCHAR errBuf[1024];
		e->GetErrorMessage(errBuf, 1024);
		e->Delete();
	}

	if (recvlen < flen)
	{
		return FALSE;
	}
	return TRUE;
}