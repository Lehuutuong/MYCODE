#pragma once

//#include "RsListEmp.h"

#define DBERR_NOERROR			0
#define DBERR_ERROR				1
#define DBERR_UNK_ERROR			-1
#define DBERR_NOT_EXIST			2
#define DBERR_EXIST				3
#define DBERR_EXCEPTION			4
#define DBERR_QUERY				5

class DbManager
{
public:
	DbManager(void);
	~DbManager(void);

	// 
	// �����ͺ��̽� �ν��Ͻ��� �����ڸ� ��´�.
	// 
	CDaoDatabase *GetDatabase(){return m_pDatabase;}

	// 
	// MDB ������ �����Ѵ�.
	// 
	BOOL Open(LPCTSTR p_MdbFile);

	// 
	// MDB ������ �ݴ´�.
	// 
	void Close();

	// 
	// �ڷ������ ���µǿ��°��� ��´�.
	// 
	BOOL IsOpend(){return m_pDatabase != NULL;}

	// 
	// SQL���� �����ϰ� ���ڵ���� ��ȯ �մϴ�.
	// 
	CDaoRecordset* QuerySql(LPCTSTR p_SqlText);

	// 
	// SQL���� �����ϰ� �����ڵ� ��ȯ
	// 
	UINT ExecuteSql(LPCTSTR p_SqlText);

	// 
	// ������ ���̺��� ������ ������ �ִ밪�� ��´�.
	// 
	UINT GetIntMaxValue(LPCTSTR p_TblName, LPCTSTR p_FieldName, OUT int &p_Value);

	// 
	// ������ ��ȸ ����� ������� �ƴ��� ���θ� ��´�.
	// 
	BOOL IsNotNull(LPCTSTR p_Sql);

	//������ ���簪�� ����ȯ�Ͽ� �����ִ� �Լ���..
	BOOL GetFieldBool(CDaoRecordset *pRS, int num);
	DWORD GetFieldInt(CDaoRecordset *pRS, int num);
	DOUBLE GetFieldDouble(CDaoRecordset *pRS, int num);
	CString GetFieldStr(CDaoRecordset *pRS, int num);
	SYSTEMTIME GetFieldDate(CDaoRecordset *pRS, int num);
private:

	CDaoDatabase *CreateDatabase(LPCTSTR p_MdbName);
	CDaoDatabase *OpenDatabase(LPCTSTR p_MdbName);
	void CloseDatabase(IN CDaoDatabase *p_Database);

	void PrintDbException(CDaoException* e);

	// 
	// �����ͺ��̽� �ν��Ͻ�
	// 
	CDaoDatabase *m_pDatabase;



};
