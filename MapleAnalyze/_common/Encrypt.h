// CIDPwdDlg �޽��� ó�����Դϴ�.
#pragma once 

#define EVENTNAME_R2CLOCAL_ORG L"R2CLOCAL_ORG_Z"//���� 1.62 ����
#define EVENTNAME_R2CLOCAL_CR L"R2CLOCAL_CR_Z"//���� 1.62 ũ����������

// ���߽��� ����� ����ϴ°��� ���
#define ALLOW_MULTI_RUN
//#undef ALLOW_MULTI_RUN

#define MULTI_CRACK		// �⺻����: 2009.1.9 ũ�� ���������� ������ �����ϴ� ���. ���⼭�� ���μ��� ��ȣ ó���� ���� �ʴ´�.
//#undef MULTI_CRACK


int DecryptData(BYTE *dest, BYTE *src, int nSrcLen);
int EncryptData(BYTE *dest, BYTE *src, int nSrcLen);
