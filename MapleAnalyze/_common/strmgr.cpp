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
//	문자열&본문관련 처리 함수들을 실현합니다.
//	
//	Modified History:
//
//		
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "strmgr.h"

/* ===============================================================================================

함수설명:	-----------------	SplitString

	문자열을 주어진 분리 문자열로 분리하여 문자열 배열을 리턴합니다.

파라미터:

	p_strText	- 분리할 원천 문자열.

	p_strSplit	- 분리 기호열.

	p_strArray	- 분리된 기호열들을 배열한 스트링 배열 참조 파라미터.

리턴 값:

	없음.

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
	//. 분리 기호열을 찾아 출구 배열에 추가한다.
	//
	while ((w_newPos = p_strText.Find(p_strSplit, w_oldPos)) >= 0)
	{
		w_str = p_strText.Mid(w_oldPos, w_newPos - w_oldPos);
		p_strArray.Add(w_str);

		w_oldPos = w_newPos + p_strSplit.GetLength();
	}

	//
	//. 마지막 남은 기호열을 추가한다.
	//
	w_str = p_strText.Mid(w_oldPos, p_strText.GetLength() - w_oldPos);
	p_strArray.Add(w_str);
}

/* ===============================================================================================

함수설명:	-----------------	IsInvalidPhoneNumber

	전화번호의 형식이 정확한가를 검사한다.

	무효하면 TRUE

=============================================================================================== */
BOOL IsInvalidPhoneNumber(LPCTSTR p_PhoneNumber)
{
	//첫 문자가 수자가 아니면
	if(p_PhoneNumber[0] < '0' || p_PhoneNumber[0] > '9')
		return TRUE;

	//마지막문자가 수자가 아니면
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

함수설명:	-----------------	IsInvalidEMail

	전자우편 형식이 정확한가를 검사한다.


	무효하면 TRUE

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

함수설명:	-----------------	ConvertStringToInt

	 문자열을 Int 형으로 변환하기.

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

함수설명:	-----------------	ConvertIntToCstring

	 Int를 문자열로 변환하기.

=============================================================================================== */
CString ConvertIntToCstring(int p_Number)
{
	CString w_ret ;
	w_ret.Format(L"%d", p_Number);
	return w_ret;
}

/* ===============================================================================================

함수설명:	-----------------	GetStringFromAnsiChars

    char* 로부터 CString을 얻습니다.

파라미터:

	p_Ansi	: 입구문자열

리턴 값:

    출구문자열

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

함수설명:	-----------------	ConvertStringToBynary

	16진수 표현 형식의 입력문자열을 바이너리 렬로 변환한다.


파라미터:

	p_srcData		: 입구 문자열(공백이 없다.)

	p_nSrcChars		: 변환 할 바이트의 개수

	p_dstNumbers	: 결과 바이트 배열을 받아갈 완충기

리턴 값:

	없음

=============================================================================================== */
void ConvertStringToBynary(LPCTSTR p_srcData, UINT p_nCount, OUT BYTE* p_dstBynary)
{

	WCHAR *pKey = (WCHAR *)p_srcData;
	int n;
/*
	// 입력자료에 공백이 있는 경우
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

함수설명:	-----------------	GetBinaryString

	바이트별로 공백이 있는 바이너리 자료의 문자열표현을 얻습니다.

파라미터:

	p_pData	: 바이너리 자료

	p_nSize	: 입구자료의 크기

	pBinaryString : 입구자료의 문자열 표현을 받아갈 완충기이다.

리턴 값:

	없음

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

함수설명:	-----------------	GetBinaryStringNoSpace

 	공백이 없는 바이너리 자료의 문자열표현을 얻습니다.

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

창조시간 :	2007년 8월 18일   2시17분

함수설명:	-----------------	ToANSI 

    유니코드 문자열을 ANSI 문자열로 변환.

=============================================================================================== */
char* ToANSI (OUT char* lpszNewText, int nLen, const wchar_t* lpszText)
{
	// 변환할 문자열의 문자개수를 얻는다.
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

창조시간 :	2007년 8월 18일   2시17분

함수설명:	-----------------	ANSI2UNICODE 

    ANSI문자열을 유니코드 문자열로 변환.

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

창조시간 :	2007년 8월 19일   11시18분

함수설명:	-----------------	strcpy_X

    문자열 복사의 안전코드.

리턴 값:

    복사한 문자열의 길이.

=============================================================================================== */
int strcpy_X(OUT char* dst, int dstLen, const char* src)
{
	// 최대 dstLen-1 길이의 문자를 복사할수 있다.
	if (dstLen <= 0) return 0;
	if (src == 0 || dst == 0) return 0;
	int nLen = (int)strlen(src);
	if(nLen < dstLen)
	{
		// 원천의 길이 작은 경우
		memcpy(dst, src, nLen + 1);
	}
	else
	{
		// 원천의 길이가 긴 경우 뒤부분을 잘라버린다.
		memcpy(dst, src, dstLen-1);
		dst[dstLen-1] = 0;
		nLen = dstLen-1;
	}
	return nLen;
}
/* ===============================================================================================

창조시간 :	2007년 8월 19일   11시18분

함수설명:	-----------------	wcscpy_X

    문자열 복사의 안전코드.

리턴 값:

    복사한 문자열의 길이.

=============================================================================================== */
int wcscpy_X(OUT wchar_t *dst, int dstLen, const wchar_t* src)
{
	// 최대 dstLen-1 길이의 문자를 복사할수 있다.
	if (dstLen <= 0) return 0;
	if (src == 0 || dst == 0) return 0;
	int nLen = (int)wcslen(src);
	if(nLen < dstLen)
	{
		// 원천의 길이 작은 경우
		memcpy(dst, src, (nLen + 1)*sizeof(wchar_t));
	}
	else
	{
		// 원천의 길이가 긴 경우 뒤부분을 잘라버린다.
		memcpy(dst, src, (dstLen-1)*sizeof(wchar_t));
		dst[dstLen-1] = 0;
		nLen = dstLen-1;
	}
	return nLen;
}

/* =============================================================================================== 

함수설명:	-----------------	GetSystemTimeString

	 시간 문자열을 얻기.

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

창조시간 :	2007년 8월 22일   13시25분

함수설명:	-----------------	GetAlphaNumericString 

	주어진 스트림을 영숫자렬로 바꾸어주는 함수.
	
	비가역함수이다. 변환시에 문자결의 길이는 변하지 않는다.

파라미터:

	len			- 복사할 바이트수(끝의 령을 포함)

	wcDefault	- 영숫자가 아닌 문자를 치환할 영수자.
					이것이 영숫자가 아니면 적당한 영숫자를 값준다.

리턴 값:

	복사된 문자열의 길이.

=============================================================================================== */
int GetAlphaNumericString(OUT wchar_t* dst, IN wchar_t* src, int len, wchar_t wcDefault)
{
	int i;
	wchar_t buf;

	// 
	// 치환 문자의 재설정
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
	// 비 영숫자들에 대한 처리를 하면서 복하한다.
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

함수설명:	-----------------	GetAlphaNumericString 

    바이트 배렬 자료를 유효한 아스키 문자열로 변환한다.

	가역의 변환은 아니다.

=============================================================================================== */
int GetAlphaNumericString(OUT char* p_dst, int p_DstMaxLen, const BYTE* p_src, int p_SrcLen, char cDefault)
{
	int i;
	char buf;

	// 
	// 치환 문자의 재설정
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
	// 비 영숫자들에 대한 처리를 하면서 복하한다.
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

창조시간 :	2007년 9월 28일   0시17분

함수설명:	-----------------	ClearCString 

	CString 형 변수에 있는 문자열의 내용을 지우는 함수.    


=============================================================================================== */
void ClearCString(OUT CString &p_strText) 
{
	memset(p_strText.GetBuffer(0), 0, p_strText.GetLength() * sizeof(TCHAR));
}

/* ===============================================================================================

함수설명:	-----------------	XFormatNumber 

	형식화된 수자표현을 얻는다. 례) 10000 => 10,000

파라미터

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

함수설명:	-----------------	XFormatNumber

	 Int64 를 형식화된 문자열로 출력하기.

=============================================================================================== */
CString XFormatNumber(__int64 number)
{
	CString strNum;
	strNum.Format(L"%d", number);

	return XFormatNumber(strNum, -1);
}

/* ===============================================================================================

함수설명:	-----------------	GetSizeString

	바이트단위의 크기로 부터 KB단위의 크기 문자열을 얻는다.

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

함수설명:	-----------------	GetSizeByteString

 	바이트단위의 크기로 부터 바이트 단위의 크기 문자열을 얻는다.

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

함수설명:	-----------------	GetMByteString

	 바이트단위의 크기로 부터 MByte 단위의 크기 문자열을 얻는다.

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

함수설명:	-----------------	MyGetSizeString

	 InfMByte 보다 크면 MB 단위로 SupByte 보다 작으면 바이트 단위로 크기를 문자열로 되돌린다.

=============================================================================================== */
CString MyGetSizeString(INT64 nSize, int InfMByte, int SupByte)
{
	CString strSIze = L"";
	if(nSize > InfMByte)
		strSIze = GetMByteString(nSize);	//20M 이상인 경우. 기정
	else if(nSize < SupByte)
		strSIze = GetSizeByteString(nSize);	//9MB 이하인 경우. 기정
	else
		strSIze = GetSizeString(nSize);
	return strSIze;

}

/* ===============================================================================================

함수설명:	-----------------	GetValue

	특정 문자들을 제외하면서 문자열로부터 int 형 수값을 얻는 함수

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

함수설명:	-----------------	GetString

	 리소스 오픈하기

=============================================================================================== */
CString  GetString(UINT nID)
{
	CString strMsg;
	strMsg.LoadString(nID);
	return strMsg;
}

/* =============================================================================================== 

함수설명:	-----------------	GetVerValue

	 버전문자열로부터 BYTE* 버전 값을 얻는다. 99.99.99.99

=============================================================================================== */
BOOL GetVerValue(LPCTSTR p_strVer, OUT BYTE* ver)
{
	memset(ver, 0, 4);
	if(p_strVer == NULL) return FALSE;

	CString item;
	CStringArray strArray;
	SplitString(p_strVer, L".", strArray);
	// 4개 이상이면 무시한다. 십진수가 아니면 실패할것이다.
	int nCount = (int)strArray.GetCount();
	if(nCount > 4 ) nCount = 4;
	int i; 
	WCHAR *ptr;
	for(i = 0; i < nCount; i++)
	{
		item = strArray[i];
		ver[3-i] = (BYTE)wcstoul(item, &ptr, 10);// 버전2.5 관련 2008년 11월 19일 수정.
	}

	return TRUE;
}

/* =============================================================================================== 

함수설명:	-----------------	GetVerValue

	버전문자열로부터 DWORD 버전 값을 얻는다. 99.99.99.99

=============================================================================================== */
DWORD GetVerValue(LPCTSTR p_strVer)
{

	DWORD Ver = 0;
	if(GetVerValue(p_strVer, (BYTE*)&Ver) == FALSE) return 0;

	return Ver;
}

/* =============================================================================================== 

함수설명:	-----------------	GetVerString

	 버전 값으로부터 버전 문자열을 얻는다.

=============================================================================================== */
CString GetVerString(DWORD p_Ver)
{
	CString strVer;

	strVer.Format(L"%d.%d.%d.%d", 0xFF & (p_Ver >> 24), 0xFF & (p_Ver >> 16)
		, 0xFF & (p_Ver >> 8), 0xFF & (p_Ver));

	return strVer;
}

/* =============================================================================================== 

함수설명:	-----------------	GetWebPath

	웹에 폴더 자료를 올려보낼때에는 "\\"형식은 다음과 같다. L"A:\\folder1\\folder2\\folderLast"

	뒤에 백슬레시를 요구할수도 안할수도 있다.
	 

=============================================================================================== */
CString GetWebPath(LPCTSTR path, LPCTSTR p_splitChar/* = NULL */, BOOL withBSlash /* = FALSE */)
{
	if(path == NULL || path[0] == 0) return	 L"";
	CString webPath = L"";

	CString splitChar;//폴더 구분 기호
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

	// 뒤에 붙이는가?
	if(withBSlash == TRUE)
	{
		webPath += splitChar;
	}

	return webPath;

}

/* =============================================================================================== 

함수설명:	-----------------	GetSystemErrorMessage

	 체계오류의 통보문을 얻기.

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