// log.h

void		PrintError(DWORD dwErrorNo);
void		PrintLog(const WCHAR *format,...);
void		PrintLogA(const char *format,...);
void		PrintBufToFile(BYTE * pBuf, int nSize, char * pszFileName);
void		PrintBuf(BYTE * pBuf, int nSize);
void		WCHARToFile(WCHAR * pszStr, char * pszFileName);
char*		WCharToChar(const wchar_t* pwstrSrc);