// CIDPwdDlg 메시지 처리기입니다.
#pragma once 

#define EVENTNAME_R2CLOCAL_ORG L"R2CLOCAL_ORG_Z"//버전 1.62 원본
#define EVENTNAME_R2CLOCAL_CR L"R2CLOCAL_CR_Z"//버전 1.62 크랙실행파일

// 다중실행 방식을 허용하는가의 기발
#define ALLOW_MULTI_RUN
//#undef ALLOW_MULTI_RUN

#define MULTI_CRACK		// 기본설명: 2009.1.9 크랙 실행파일을 여러개 실행하는 방식. 여기서는 프로세스 번호 처리를 하지 않는다.
//#undef MULTI_CRACK


int DecryptData(BYTE *dest, BYTE *src, int nSrcLen);
int EncryptData(BYTE *dest, BYTE *src, int nSrcLen);
