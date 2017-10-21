/////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 PSJDC.
//
//						strmgr.cpp	
//
//	This file is created [3/31/2009 by LMC]
//	comment is written in Korean.
//
//	Main purpose
//
//	���ڿ�&�������� ó�� �Լ����� �����մϴ�.
//	
//	Modified History:
//
//		
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "strmgr.h"

/* ===============================================================================================

�Լ�����:	-----------------	SplitString

	���ڿ��� �־��� �и� ���ڿ��� �и��Ͽ� ���ڿ� �迭�� �����մϴ�.

�Ķ����:

	p_strText	- �и��� ��õ ���ڿ�.

	p_strSplit	- �и� ��ȣ��.

	p_strArray	- �и��� ��ȣ������ �迭�� ��Ʈ�� �迭 ���� �Ķ����.

���� ��:

	����.

=============================================================================================== */
void SplitString(
			   IN	CString		p_strText,
			   IN	CString		p_strSplit,
			   OUT	CStringArray&	p_strArray
			   )
{
	int		w_oldPos = 0;
	int		w_newPos;
	CString	w_str;

	p_strArray.RemoveAll();
	if (p_strText == L"")
	{
		return;
	}

	//
	//. �и� ��ȣ���� ã�� �ⱸ �迭�� �߰��Ѵ�.
	//
	while ((w_newPos = p_strText.Find(p_strSplit, w_oldPos)) >= 0)
	{
		w_str = p_strText.Mid(w_oldPos, w_newPos - w_oldPos);
		p_strArray.Add(w_str);

		w_oldPos = w_newPos + p_strSplit.GetLength();
	}

	//
	//. ������ ���� ��ȣ���� �߰��Ѵ�.
	//
	w_str = p_strText.Mid(w_oldPos, p_strText.GetLength() - w_oldPos);
	p_strArray.Add(w_str);
}

/* ===============================================================================================

�Լ�����:	-----------------	IsInvalidPhoneNumber

	��ȭ��ȣ�� ������ ��Ȯ�Ѱ��� �˻��Ѵ�.

	��ȿ�ϸ� TRUE

=============================================================================================== */
BOOL IsInvalidPhoneNumber(LPCTSTR p_PhoneNumber)
{
	//ù ���ڰ� ���ڰ� �ƴϸ�
	if(p_PhoneNumber[0] < '0' || p_PhoneNumber[0] > '9')
		return TRUE;

	//���������ڰ� ���ڰ� �ƴϸ�
	int Num = (int)wcslen(p_PhoneNumber);
	if(p_PhoneNumber[Num-1] < '0' || p_PhoneNumber[Num-1] > '9')
		return TRUE;

	for(int i = 1; i<Num-1; i++)
	{
		if(p_PhoneNumber[Num-1] < '0' || p_PhoneNumber[Num-1] > '9')
			if(p_PhoneNumber[Num-1] != '-' )
				return TRUE;
	}
	return FALSE;
}
/* ===============================================================================================

�Լ�����:	-----------------	IsInvalidEMail

	���ڿ��� ������ ��Ȯ�Ѱ��� �˻��Ѵ�.


	��ȿ�ϸ� TRUE

=============================================================================================== */
BOOL IsInvalidEMail(LPCTSTR p_Email)
{
	LPCTSTR str1;
	LPCTSTR str2;
	int result;
	
	str1 = wcschr(p_Email,'@');
	result = (int)(str1-p_Email);
	if(str1 == NULL || result==0 || wcslen(str1) == 1)
		return TRUE;
	
	str2 = wcschr(str1,'.');
	result = (int)(str2-str1);
	if(str2 == NULL ||result <= 1 || wcslen(str2) == 1)
		return TRUE;
	
	return FALSE;
}

/* =============================================================================================== 

�Լ�����:	-----------------	ConvertStringToInt

	 ���ڿ��� Int ������ ��ȯ�ϱ�.

=============================================================================================== */
int ConvertStringToInt(LPCTSTR p_Str)
{
	char w_depstr[256] = {0};
	int w_ret = -1;
 	WideCharToMultiByte(CP_ACP, 0, p_Str, -1, 
	w_depstr, (int)wcslen(p_Str) * 2 + 1, NULL, NULL);
	for( int i = 0; i < (int)wcslen(p_Str); i++)
	{
		if( w_depstr[i] <'0' || w_depstr[i] >'9')
			goto go_exit;
	}
	w_ret = atoi(w_depstr);	

go_exit:
	return w_ret;
}

/* =============================================================================================== 

�Լ�����:	-----------------	ConvertIntToCstring

	 Int�� ���ڿ��� ��ȯ�ϱ�.

=============================================================================================== */
CString ConvertIntToCstring(int p_Number)
{
	CString w_ret ;
	w_ret.Format(L"%d", p_Number);
	return w_ret;
}

/* ===============================================================================================

�Լ�����:	-----------------	GetStringFromAnsiChars

    char* �κ��� CString�� ����ϴ�.

�Ķ����:

	p_Ansi	: �Ա����ڿ�

���� ��:

    �ⱸ���ڿ�

=============================================================================================== */
CString GetStringFromAnsiChars( const char* p_Ansi )
{
	CString strRet;
	size_t converted;
	size_t len = strlen(p_Ansi);
	WCHAR *buf = new WCHAR[len + 2];
	mbstowcs_s( &converted, buf, len + 1,  p_Ansi, len + 1);

	strRet = buf;

	delete []buf;

	return strRet;
}

/* ===============================================================================================

�Լ�����:	-----------------	ConvertStringToBynary

	16���� ǥ�� ������ �Է¹��ڿ��� ���̳ʸ� �ķ� ��ȯ�Ѵ�.


�Ķ����:

	p_srcData		: �Ա� ���ڿ�(������ ����.)

	p_nSrcChars		: ��ȯ �� ����Ʈ�� ����

	p_dstNumbers	: ��� ����Ʈ �迭�� �޾ư� �����

���� ��:

	����

=============================================================================================== */
void ConvertStringToBynary(LPCTSTR p_srcData, UINT p_nCount, OUT BYTE* p_dstBynary)
{

	WCHAR *pKey = (WCHAR *)p_srcData;
	int n;
/*
	// �Է��ڷῡ ������ �ִ� ���
	for ( UINT i = 0; i < p_nCount; i++ )
	{
		wchar_t szTmp2[3], *ptr;
		szTmp2[0] = pKey[i * 3 + 0];
		szTmp2[1] = pKey[i * 3 + 1];
		szTmp2[2] = 0;

		n = (int)_wcstoi64( szTmp2, &ptr, 16);
		p_dstBynary[i] = (BYTE) n;
	}
*/
	for ( UINT i = 0; i < p_nCount; i++ )
	{
		wchar_t szTmp2[3], *ptr;
		szTmp2[0] = pKey[i * 2 + 0];
		szTmp2[1] = pKey[i * 2 + 1];
		szTmp2[2] = 0;

		n = (int)_wcstoi64( szTmp2, &ptr, 16);
		p_dstBynary[i] = (BYTE) n;
	}

}

/* ===============================================================================================

�Լ�����:	-----------------	GetBinaryString

	����Ʈ���� ������ �ִ� ���̳ʸ� �ڷ��� ���ڿ�ǥ���� ����ϴ�.

�Ķ����:

	p_pData	: ���̳ʸ� �ڷ�

	p_nSize	: �Ա��ڷ��� ũ��

	pBinaryString : �Ա��ڷ��� ���ڿ� ǥ���� �޾ư� ������̴�.

���� ��:

	����

=============================================================================================== */
void GetBinaryString(const unsigned char* p_pData, unsigned int p_nSize, OUT wchar_t* pBinaryString)
{
	wchar_t sample[16] = {
		L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', 
		L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F'
	};

	for(unsigned int i = 0; i < p_nSize; i++)
	{
		pBinaryString[i * 3 + 0] = sample[(p_pData[i] & 0xF0) >> 4];
		pBinaryString[i * 3 + 1] = sample[p_pData[i] & 0x0F];
		pBinaryString[i * 3 + 2] = ' ';
	}
	pBinaryString[p_nSize * 3] = 0;

}

/* =============================================================================================== 

�Լ�����:	-----------------	GetBinaryStringNoSpace

 	������ ���� ���̳ʸ� �ڷ��� ���ڿ�ǥ���� ����ϴ�.

=============================================================================================== */
void GetBinaryStringNoSpace(const unsigned char* p_pData, unsigned int p_nSize, OUT wchar_t* pBinaryString)
{
	wchar_t sample[16] = {
		L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', 
		L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F'
	};

	for(unsigned int i = 0; i < p_nSize; i++)
	{
		pBinaryString[i * 2 + 0] = sample[(p_pData[i] & 0xF0) >> 4];
		pBinaryString[i * 2 + 1] = sample[p_pData[i] & 0x0F];
	}
	pBinaryString[p_nSize * 2] = 0;

}


/* ===============================================================================================

â���ð� :	2007�� 8�� 18��   2��17��

�Լ�����:	-----------------	ToANSI 

    �����ڵ� ���ڿ��� ANSI ���ڿ��� ��ȯ.

=============================================================================================== */
char* ToANSI (OUT char* lpszNewText, int nLen, const wchar_t* lpszText)
{
	// ��ȯ�� ���ڿ��� ���ڰ����� ��´�.
	int wideLen = (int)wcslen (lpszText);
	int chars;
	if (lpszNewText == 0)
	{
		lpszNewText = (char*) malloc (wideLen + 1);
		nLen = wideLen + 1;
	}
	if (wideLen == 0)
	{
		lpszNewText[0] = 0;
		return lpszNewText;
	}
	else
	{
		if (nLen > 0 && wideLen > nLen - 1)
		{
			wideLen = nLen - 1;
		}
		chars = WideCharToMultiByte (CP_ACP, 0L, lpszText, -1, lpszNewText, nLen, NULL, NULL);
	}
	return lpszNewText;
}


/* ===============================================================================================

â���ð� :	2007�� 8�� 18��   2��17��

�Լ�����:	-----------------	ANSI2UNICODE 

    ANSI���ڿ��� �����ڵ� ���ڿ��� ��ȯ.

=============================================================================================== */
wchar_t* ANSI2UNICODE (OUT wchar_t* lpszNewText, int nLen, const char* lpszText)
{
	int j = (int)strlen (lpszText);
	if (lpszNewText == 0)
	{
		lpszNewText = (wchar_t*) malloc ((j + 1) * 2);
		nLen = (j + 1) * 2;// 2009.2.12 22 zc(test). 
	}
	if (j == 0)
	{
		lpszNewText[0] = 0;
		return lpszNewText;
	}
	else
	{
		if (nLen > 0 && j > nLen - 1)
		{
			j = nLen - 1;
		}
		j = MultiByteToWideChar (CP_ACP, 0L, lpszText, -1, lpszNewText, j + 1);
	}
	return lpszNewText;	
}

/* ===============================================================================================

â���ð� :	2007�� 8�� 19��   11��18��

�Լ�����:	-----------------	strcpy_X

    ���ڿ� ������ �����ڵ�.

���� ��:

    ������ ���ڿ��� ����.

=============================================================================================== */
int strcpy_X(OUT char* dst, int dstLen, const char* src)
{
	// �ִ� dstLen-1 ������ ���ڸ� �����Ҽ� �ִ�.
	if (dstLen <= 0) return 0;
	if (src == 0 || dst == 0) return 0;
	int nLen = (int)strlen(src);
	if(nLen < dstLen)
	{
		// ��õ�� ���� ���� ���
		memcpy(dst, src, nLen + 1);
	}
	else
	{
		// ��õ�� ���̰� �� ��� �ںκ��� �߶������.
		memcpy(dst, src, dstLen-1);
		dst[dstLen-1] = 0;
		nLen = dstLen-1;
	}
	return nLen;
}
/* ===============================================================================================

â���ð� :	2007�� 8�� 19��   11��18��

�Լ�����:	-----------------	wcscpy_X

    ���ڿ� ������ �����ڵ�.

���� ��:

    ������ ���ڿ��� ����.

=============================================================================================== */
int wcscpy_X(OUT wchar_t *dst, int dstLen, const wchar_t* src)
{
	// �ִ� dstLen-1 ������ ���ڸ� �����Ҽ� �ִ�.
	if (dstLen <= 0) return 0;
	if (src == 0 || dst == 0) return 0;
	int nLen = (int)wcslen(src);
	if(nLen < dstLen)
	{
		// ��õ�� ���� ���� ���
		memcpy(dst, src, (nLen + 1)*sizeof(wchar_t));
	}
	else
	{
		// ��õ�� ���̰� �� ��� �ںκ��� �߶������.
		memcpy(dst, src, (dstLen-1)*sizeof(wchar_t));
		dst[dstLen-1] = 0;
		nLen = dstLen-1;
	}
	return nLen;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetSystemTimeString

	 �ð� ���ڿ��� ���.

=============================================================================================== */
void GetSystemTimeString(OUT wchar_t* strTime, int bufChLen, IN SYSTEMTIME *st)
{
	wchar_t buf[MAX_PATH];
	GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, st, L"ddd dd/MM/yyyy", buf, 20);
	wcscpy_s(strTime, bufChLen, buf);
	GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0, st, L" hh:mm:ss tt", buf, 20);
	wcscat_s(strTime, bufChLen, buf);

}

/* ===============================================================================================

â���ð� :	2007�� 8�� 22��   13��25��

�Լ�����:	-----------------	GetAlphaNumericString 

	�־��� ��Ʈ���� �����ڷķ� �ٲپ��ִ� �Լ�.
	
	�񰡿��Լ��̴�. ��ȯ�ÿ� ���ڰ��� ���̴� ������ �ʴ´�.

�Ķ����:

	len			- ������ ����Ʈ��(���� ���� ����)

	wcDefault	- �����ڰ� �ƴ� ���ڸ� ġȯ�� ������.
					�̰��� �����ڰ� �ƴϸ� ������ �����ڸ� ���ش�.

���� ��:

	����� ���ڿ��� ����.

=============================================================================================== */
int GetAlphaNumericString(OUT wchar_t* dst, IN wchar_t* src, int len, wchar_t wcDefault)
{
	int i;
	wchar_t buf;

	// 
	// ġȯ ������ �缳��
	// 
	if (wcDefault > 128)
	{
		wcDefault = 0;
	}
	if(len <= 0)
	{
		len = (int)wcslen(src) + 1;
	}

	// 
	// �� �����ڵ鿡 ���� ó���� �ϸ鼭 �����Ѵ�.
	// 
	for(i = 0; i < len - 1; i++)
	{
		buf = (src[i]+0x10000) % 128;
		if( IsCharAlphaNumericW(buf) == TRUE)
		{
			dst[i] = buf;
		}
		else
		{
			if (wcDefault == 0)
			{
				dst[i] = (buf % 26) + 'A';
			}
			else
			{
				dst[i] = wcDefault;
			}
		}
	}
	dst[i] = 0;

	return i;

}

/* ===============================================================================================

�Լ�����:	-----------------	GetAlphaNumericString 

    ����Ʈ ��� �ڷḦ ��ȿ�� �ƽ�Ű ���ڿ��� ��ȯ�Ѵ�.

	������ ��ȯ�� �ƴϴ�.

=============================================================================================== */
int GetAlphaNumericString(OUT char* p_dst, int p_DstMaxLen, const BYTE* p_src, int p_SrcLen, char cDefault)
{
	int i;
	char buf;

	// 
	// ġȯ ������ �缳��
	// 
	if (cDefault > 128)
	{
		cDefault = 0;
	}
	if(p_SrcLen <= 0)
	{
		p_SrcLen = (int)strlen((char*)p_src) + 1;
	}

	if (p_SrcLen > p_DstMaxLen)
	{
		p_SrcLen = p_DstMaxLen;
	}

	// 
	// �� �����ڵ鿡 ���� ó���� �ϸ鼭 �����Ѵ�.
	// 
	for(i = 0; i < p_SrcLen - 1; i++)
	{
		buf = (p_src[i]+0x100) % 128;
		if( IsCharAlphaNumericA(buf) == TRUE)
		{
			p_dst[i] = buf;
		}
		else
		{
			if (cDefault == 0)
			{
				p_dst[i] = (buf % 26) + 'A';
			}
			else
			{
				p_dst[i] = cDefault;
			}
		}
	}
	p_dst[i] = 0;

	return i;
}

/* ===============================================================================================

â���ð� :	2007�� 9�� 28��   0��17��

�Լ�����:	-----------------	ClearCString 

	CString �� ������ �ִ� ���ڿ��� ������ ����� �Լ�.    


=============================================================================================== */
void ClearCString(OUT CString &p_strText) 
{
	memset(p_strText.GetBuffer(0), 0, p_strText.GetLength() * sizeof(TCHAR));
}

/* ===============================================================================================

�Լ�����:	-----------------	XFormatNumber 

	����ȭ�� ����ǥ���� ��´�. ��) 10000 => 10,000

�Ķ����

	nFracDigits = -1 : use no. of frac. digits from string.

=============================================================================================== */
CString XFormatNumber(LPCTSTR lpszNumber, int nFracDigits)
{
	ASSERT(lpszNumber);
	ASSERT(lpszNumber[0] != 0);

	CString str = lpszNumber;

	NUMBERFMT nf;
	memset(&nf, 0, sizeof(nf));


	_TCHAR szBuffer[10];
	_TCHAR szDecSep[10];
	_TCHAR szThousandsSep[10];

	// set decimal separator string from locale default
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL,
		szDecSep, sizeof(szDecSep)-1);
	nf.lpDecimalSep = szDecSep;

	// set thousand separator string from locale default
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND,
		szThousandsSep, sizeof(szThousandsSep)-1);
	nf.lpThousandSep = szThousandsSep;

	// set leading zeroes from locale default
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ILZERO,
		szBuffer, sizeof(szBuffer)-1);
	nf.LeadingZero = _ttoi(szBuffer);

	// set grouping of digits from locale default
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SGROUPING,
		szBuffer, sizeof(szBuffer)-1);
	nf.Grouping = _ttoi(szBuffer);

	// set negative number mode from locale default
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER,
		szBuffer, sizeof(szBuffer)-1);
	nf.NegativeOrder = _ttoi(szBuffer);

	int dp = nFracDigits;

	if (dp == -1)
	{
		// set number of fractional digits from input string
		CString strDecSep = szDecSep;
		dp = str.Find(szDecSep);
		dp = (dp == -1) ? 0 : str.GetLength() - dp - strDecSep.GetLength();
	}
	nf.NumDigits = dp;

	int nSize = (int)(_tcslen(lpszNumber) * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];
	ASSERT(pszFormattedNumber);

	if (GetNumberFormat(LOCALE_USER_DEFAULT, 
		0, 
		lpszNumber, 
		&nf, 
		pszFormattedNumber, 
		nSize-1))
	{
		str = pszFormattedNumber;
	}

	if (pszFormattedNumber)
		delete [] pszFormattedNumber;

	return str;
}

/* =============================================================================================== 

�Լ�����:	-----------------	XFormatNumber

	 Int64 �� ����ȭ�� ���ڿ��� ����ϱ�.

=============================================================================================== */
CString XFormatNumber(__int64 number)
{
	CString strNum;
	strNum.Format(L"%d", number);

	return XFormatNumber(strNum, -1);
}

/* ===============================================================================================

�Լ�����:	-----------------	GetSizeString

	����Ʈ������ ũ��� ���� KB������ ũ�� ���ڿ��� ��´�.

=============================================================================================== */
CString GetSizeString(__int64 nSize)
{
	if (nSize == 0)
	{
		return L"0 KB";
	}

	int k_size = (int)((nSize + 1023) / 1024);

	CString str, strRet;
	str.Format(L"%d", k_size);

	str = XFormatNumber(str, -1);
	strRet.Format(L"%s KB", str);

	return strRet;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetSizeByteString

 	����Ʈ������ ũ��� ���� ����Ʈ ������ ũ�� ���ڿ��� ��´�.

=============================================================================================== */
CString GetSizeByteString(__int64 nSize)
{
	if (nSize == 0)
	{
		return L"0 Byte";
	}

	CString str, strRet;
	str.Format(L"%I64u", nSize);

	str = XFormatNumber(str, -1);
	strRet.Format(L"%s Byte", str);

	return strRet;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetMByteString

	 ����Ʈ������ ũ��� ���� MByte ������ ũ�� ���ڿ��� ��´�.

=============================================================================================== */
CString GetMByteString(__int64 nSize)
{
	if (nSize == 0)
	{
		return L"0 MB";
	}

	int k_size = (int)((nSize + 1023) / 1024);
	int m_size = (int)((k_size + 1023) / 1024);

	CString str, strRet;
	str.Format(L"%d", m_size);

	str = XFormatNumber(str, -1);
	strRet.Format(L"%s MB", str);

	return strRet;
}

/* =============================================================================================== 

�Լ�����:	-----------------	MyGetSizeString

	 InfMByte ���� ũ�� MB ������ SupByte ���� ������ ����Ʈ ������ ũ�⸦ ���ڿ��� �ǵ�����.

=============================================================================================== */
CString MyGetSizeString(INT64 nSize, int InfMByte, int SupByte)
{
	CString strSIze = L"";
	if(nSize > InfMByte)
		strSIze = GetMByteString(nSize);	//20M �̻��� ���. ����
	else if(nSize < SupByte)
		strSIze = GetSizeByteString(nSize);	//9MB ������ ���. ����
	else
		strSIze = GetSizeString(nSize);
	return strSIze;

}

/* ===============================================================================================

�Լ�����:	-----------------	GetValue

	Ư�� ���ڵ��� �����ϸ鼭 ���ڿ��κ��� int �� ������ ��� �Լ�

=============================================================================================== */
int GetValue(LPCTSTR p_str, LPCTSTR p_ExceptChars)
{
	CString str_tmp = p_str;
	int nRet = 0;

	if(p_ExceptChars != NULL)
	{
		for(int i = 0; i < (int)wcslen(p_ExceptChars); i++)
		{
			str_tmp.Remove(p_ExceptChars[i]);
		}
	}

	nRet = _wtoi(str_tmp);

	return nRet;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetString

	 ���ҽ� �����ϱ�

=============================================================================================== */
CString  GetString(UINT nID)
{
	CString strMsg;
	strMsg.LoadString(nID);
	return strMsg;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetVerValue

	 �������ڿ��κ��� BYTE* ���� ���� ��´�. 99.99.99.99

=============================================================================================== */
BOOL GetVerValue(LPCTSTR p_strVer, OUT BYTE* ver)
{
	memset(ver, 0, 4);
	if(p_strVer == NULL) return FALSE;

	CString item;
	CStringArray strArray;
	SplitString(p_strVer, L".", strArray);
	// 4�� �̻��̸� �����Ѵ�. �������� �ƴϸ� �����Ұ��̴�.
	int nCount = (int)strArray.GetCount();
	if(nCount > 4 ) nCount = 4;
	int i; 
	WCHAR *ptr;
	for(i = 0; i < nCount; i++)
	{
		item = strArray[i];
		ver[3-i] = (BYTE)wcstoul(item, &ptr, 10);// ����2.5 ���� 2008�� 11�� 19�� ����.
	}

	return TRUE;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetVerValue

	�������ڿ��κ��� DWORD ���� ���� ��´�. 99.99.99.99

=============================================================================================== */
DWORD GetVerValue(LPCTSTR p_strVer)
{

	DWORD Ver = 0;
	if(GetVerValue(p_strVer, (BYTE*)&Ver) == FALSE) return 0;

	return Ver;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetVerString

	 ���� �����κ��� ���� ���ڿ��� ��´�.

=============================================================================================== */
CString GetVerString(DWORD p_Ver)
{
	CString strVer;

	strVer.Format(L"%d.%d.%d.%d", 0xFF & (p_Ver >> 24), 0xFF & (p_Ver >> 16)
		, 0xFF & (p_Ver >> 8), 0xFF & (p_Ver));

	return strVer;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetWebPath

	���� ���� �ڷḦ �÷����������� "\\"������ ������ ����. L"A:\\folder1\\folder2\\folderLast"

	�ڿ� �齽���ø� �䱸�Ҽ��� ���Ҽ��� �ִ�.
	 

=============================================================================================== */
CString GetWebPath(LPCTSTR path, LPCTSTR p_splitChar/* = NULL */, BOOL withBSlash /* = FALSE */)
{
	if(path == NULL || path[0] == 0) return	 L"";
	CString webPath = L"";

	CString splitChar;//���� ���� ��ȣ
	if(p_splitChar == NULL)
		splitChar = L"\\\\";
	else
		splitChar = p_splitChar;

	BOOL backSlash = FALSE;
	for(int i = 0; i < (int)wcslen(path); i++)
	{
		if(path[i] == '/' || path[i] == '\\')
		{
			backSlash = TRUE;
		}
		else
		{
			if(backSlash == TRUE)
			{
				webPath += splitChar;
			}
			backSlash = FALSE;
			webPath += path[i];
		}
	}

	// �ڿ� ���̴°�?
	if(withBSlash == TRUE)
	{
		webPath += splitChar;
	}

	return webPath;

}

/* =============================================================================================== 

�Լ�����:	-----------------	GetSystemErrorMessage

	 ü������� �뺸���� ���.

=============================================================================================== */
BOOL GetSystemErrorMessage(int nErr, OUT wchar_t *errMsg, int nMaxLen) 
{ 

	wchar_t* lpMsgBuf;
	BOOL bResult;

	int n = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, nErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(wchar_t*) &lpMsgBuf, 0, NULL );


	bResult = n == 0 ? FALSE : TRUE;

	if (bResult == TRUE)
	{
		swprintf_s(errMsg, nMaxLen, L"%s", lpMsgBuf); 
	}

	LocalFree(lpMsgBuf);

	return bResult;

}