//--------------------------------------------------------
//		��� �ڷ� �ؼ�/�ۼ� ���� �ҽ�
//--------------------------------------------------------

#include "stdafx.h"

#include "CommData.h"

#include "Encrypt.h"

char ProVer[6] ="DF100";

DWORD g_recvlen = 0;

// ������ �ְ� ��Ʈ��ȣ�� ��ȿ�� ��쿡�� �������ش�. 2009.3.24
BOOL UpdatePortNo(OUT WORD *p_PortNo, IN WCHAR* strPath)
{
	//��Ʈ ���
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

// ������ �����ϴ� ��� ù�ܾ�� ���� IP ���ڷ�(�ִ�16����Ʈ)�� ��´�. "192.168.0.199";
BOOL UpdateIpString(OUT CHAR *p_IpString, IN WCHAR* strPath)
{
	//IP ���ڷ� ���
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

�Լ�����: 

���������� ������ â���ϰ� �����Ѵ�.

�Ķ����: 

serverIP : �����ּ� : "192.168.0.111"

p_conn : ���ӵ� ����. ������ ��쿡�� ��ȿ�ϴ�.

strMsg : ���н� ��������� ��´�. �ּ� 200 ����Ʈ�� �ְų� NULL �� �ش�.

��ȯ��: 

TRUE/FALSE	

=============================================================================================== */
BOOL ConnectServer(IN CHAR* serverIP, WORD serverPort, OUT SOCKET &p_conn, OUT WCHAR* strMsg)
{
	WCHAR str[1024] = {0};
	WCHAR *strResult = str;

	// ������� ����&�ʱ�ȭ
	if(strMsg != NULL)
		strResult = strMsg;
	strResult[0] = 0;

	WSADATA wsaData;
	struct hostent *hp;
	unsigned int addr = 0;
	sockaddr_in serverAddr;
	BOOL r = FALSE;

	// �����Ѵ�.
	int err = 0, wsaRet;

	wsaRet = WSAStartup(0x101, &wsaData);

	if(wsaRet)	
	{
		wcscpy(strResult, STR_CANT_CONNECT_SERVER);
		wcscat(strResult, L"\n");
		wcscat(strResult, STR_CONFIRM_NETWORK_FIREWALL);
		return FALSE;
	}

	// ���� â��
	p_conn = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(p_conn == INVALID_SOCKET)
	{
		wcscpy(strResult, STR_CANT_CONNECT_SERVER);
		wcscat(strResult, L"\n");
		wcscat(strResult, STR_CONFIRM_NETWORK_FIREWALL);
		return FALSE;
	}

	// ȣ��Ʈ ���
	hp = gethostbyname(serverIP);
	if(hp == NULL)
	{
		addr = inet_addr(serverIP);
		hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	}

	// ȣ��Ʈ ��� 2��° ���
	if(hp == NULL)
	{
		wcscpy(strResult, STR_CANT_CONNECT_SERVER);
		wcscat(strResult, L"\n");
		wcscat(strResult, STR_CONFIRM_NETWORK_FIREWALL);
		goto loc_exit_fail;	
	}

	// ��Ÿ �������� ����
	serverAddr.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);

	// �����ϱ�
	if(connect(p_conn, (struct sockaddr*)&serverAddr, sizeof(serverAddr)))
	{
		wcscpy(strResult, STR_DONT_RESPONS_SERVER);
		wcscat(strResult, L"\n");
		wcscat(strResult, STR_CONFIRM_NETWORK_FIREWALL);
		goto loc_exit_fail;		
	}

	// ���������� ���� â���ǿ���.
	r = TRUE;

loc_exit_fail:

	if(r == FALSE && p_conn != NULL)
	{
		// ������ ��쿡�� ������ �ݴ´�.
		closesocket(p_conn);
		p_conn = NULL;
	}

	return r;

}

BOOL CloseConnect(IN SOCKET &p_conn)
{
	if(p_conn != NULL)
	{
		// ������ ��쿡�� ������ �ݴ´�.
		if(closesocket(p_conn) != 0)
		{
			return FALSE;
		}
		p_conn = NULL;
	}
	return TRUE;

}

/* ===============================================================================================

�Լ�����:

������ ���� ������ �����͸� �����Ѵ�.

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

	// ��ȣȭ //���� �������� �����͸� �����Ѵ�..
	for(kk = 0; kk< sendLen; kk++)
	{
		((BYTE *)sendBuffer)[kk] = ~((BYTE *)sendBuffer)[kk];
	}

	sendBuf = new BYTE[sendLen + 2 + 5]; //���� :�ĺ��� 5����Ʈ�� �� �ٴ´�. 
	*((WORD *)sendBuf) = (WORD)sendLen + 5;
	memcpy(&sendBuf[2], ProVer, 5);				//���� :�ĺ��� 5����Ʈ�� �� �ٴ´�.
	//���� �۽�.
	memcpy(&sendBuf[7], sendBuffer, sendLen);
	sendLen += 7;								//���� :�ĺ��� 5����Ʈ�� �� �ٴ´�.

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
		PrintLog(L"���Ͽ��� socket %d %d %d %d", p_conn, WSAGetLastError(), n, sendLen);
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

	if(sendLen > PACKET_MAX_SIZE - 6) sendLen = PACKET_MAX_SIZE - 6; //��Ŷ�� �ʹ� ũ�ٸ� �� �ڸ���..���н�ų��?(-2�� �����ڵ�, 4:���̿�)
	sendBuf = new BYTE[PACKET_BLOCK_SIZE]; 

	//ù ����Ʈ���� OP�ڵ�, ��Ŷ�� ����, �����͸� �ش�.
	//�� ���� ���� �����ʹ� �ô޾� �ش�. �̶����� �����͸� �۽��Ѵ�. 
	curPos = sendLen;
	if(sendLen > PACKET_BLOCK_SIZE - 6) curPos = (PACKET_BLOCK_SIZE - 6);

	*((WORD *)sendBuf) = (WORD)opCode;
	*((DWORD *)(&sendBuf[2])) = (DWORD)sendLen;

	if(sendBuffer == NULL && sendLen == 0)
	{
		//�̰��� op�ڵ常�� ����ϱ� ���ؼ��̴�, �Ϲ������� ���� �ڵ���� �׷��� ������.
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

// �۽��ϰ� ���Ź��۸� �Ҵ��Ͽ� �ǵ�����. ����� �������̶�� ����.
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

	// �����Ѵ�.
	// ���� 2����Ʈ �а� ���̸� ��´�. ���� �� ���̰� �� ������ ������ ���̸� �б��Ѵ�.
	
	
recvLoop1:
	n = recv(p_conn, t_buf, 2, 0); //���� 1����Ʈ�� �о����ٸ�..
	if (n == SOCKET_ERROR || n == 0)
	{
		PrintLog(L"���ſ��� -- 1 %d", WSAGetLastError());
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			Sleep(10);
			goto recvLoop1;
		}
		wcscpy(strResult, STR_INVALID_RESPONS_SERVER);
		goto loc_exit_fail;	
	}

	m_Length = *(WORD*)t_buf;

	// ���� ����⿡ ũ�⸸ŭ �������ְ� ���̸� �־��ش�..
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

	//�ĺ��� �˻�
	if(nLen < 5) goto loc_exit_fail;
	else nLen -= 5;

	if(memcmp(ProVer, buf, 5) != 0) //���� :�ĺ��� 5����Ʈ�� �� �ٴ´�.
	{
		wcscpy(strResult, STR_INVALID_RESPONS_SERVER);
		goto loc_exit_fail;	
	}

	// 	// ��ȣȭ //���� ���� �����͸� �����Ѵ�..���� ��ȣȭ�� �����ʹ� 5��° ����Ʈ���� �ִ�.
	for(kk = 0; kk< nLen; kk++)
	{
		((BYTE *)buf)[5 + kk] = ~((BYTE *)buf)[5 + kk];
	}

	//�޾ư� ������� ũ�⸦ ���غ��� �������ش�.??
	if(recvLen >= nLen)
	{
		memcpy(recvBuffer,&buf[5],nLen); //���� :�ĺ��� 5����Ʈ�� �� �ٴ´�.
		recvLen = nLen;
	}
	else
	{
		memcpy(recvBuffer,&buf[5],recvLen); //���� :�ĺ��� 5����Ʈ�� �� �ٴ´�.
	}
	//memcpy(recvBuffer,buf,n); 

	r = TRUE;

loc_exit_fail:

	if(buf) delete buf; buf = NULL;
	//	if(recvBuf) delete recvBuf; recvBuf = NULL;

	return r;

}

//�����͸� ����ȭ�Ͽ� �޴´�.
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

	// ��Ŷ�� �����Ѵ�. 
	//ù���� ���ſ��� op�ڵ�� ���۵� �������� ũ�⸦ ������ �̿� ���� �� ũ�Ⱑ �� �о��� ������ �б��Ѵ�.
	nLen = PACKET_BLOCK_SIZE;
	buf = new BYTE[nLen];

	if(!RecvData(p_conn, buf, nLen, strResult))
		goto loc_exit_fail;

	nLen = nLen - 6;//ù ��Ŷ���� 2����Ʈ OP�ڵ�, 4����Ʈ�� ���� �ڵ尡 �� ��������..
	if(nLen < 0) 
	{
		goto loc_exit_fail;
	}
	opCode = *(WORD*)(&buf[0]);
	n_recvLen = *(DWORD*)(&buf[2]);

	//�޾ư� ����Ⱑ ����� ū��? ���� ũ�� �ʴٸ�..//����� ���� ���ص� ������ ���ؿ����� ����� �Ѵ�.
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
		if(readedLength + nLen > PACKET_MAX_SIZE) //���� ��Ŷ�� �ʹ� ũ�ٸ�..//�̰��� �������ΰ��� �ƴѵ�..
			goto loc_exit_fail;
		//�޾ư� ����Ⱑ ����� ū��? ���� ũ�� �ʴٸ�...//����� ���� ���ص� ������ ���ؿ����� ����� �Ѵ�.
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

�Լ�����:

������ ���� ������ ������ �����Ѵ�.

=============================================================================================== */

// kan 0902 Ŭ������
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

		// ���� ������ ũ�⸦ �����Ѵ�.		
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
		//MyOutputDebugString(DBG_LING3, DBG_ERR, L"Server : �������ۿ��� ���� [%s]",errBuf);
		e->Delete();
	}

	if (totallen < flen)
	{
		//MyOutputDebugString(DBG_LING3, DBG_ERR, L"Server : �������ۿ��� �����ڷ��� ũ�Ⱑ ����ũ�⺸�� �۽��ϴ�.[%d/%d]",nSentSize , nFileSize);
		return FALSE;
	}
	return TRUE;
}

// kan 0902 Ŭ���е庸���� ����
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
			//MyOutputDebugString(DBG_LING3, DBG_ERR, L"Server : �������ۿ��� ���ϻ��� ����(%s)", fileName);
			return FALSE;
		}

		// ���� ������ ũ�⸦ �۽��Ѵ�.
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