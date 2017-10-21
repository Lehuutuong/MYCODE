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
	// 데이터베이스 인스턴스의 지적자를 얻는다.
	// 
	CDaoDatabase *GetDatabase(){return m_pDatabase;}

	// 
	// MDB 파일을 오픈한다.
	// 
	BOOL Open(LPCTSTR p_MdbFile);

	// 
	// MDB 파일을 닫는다.
	// 
	void Close();

	// 
	// 자료기지가 오픈되였는가를 얻는다.
	// 
	BOOL IsOpend(){return m_pDatabase != NULL;}

	// 
	// SQL문을 실행하고 레코드셋을 귀환 합니다.
	// 
	CDaoRecordset* QuerySql(LPCTSTR p_SqlText);

	// 
	// SQL문을 실행하고 오류코드 귀환
	// 
	UINT ExecuteSql(LPCTSTR p_SqlText);

	// 
	// 지적한 테이블의 지적한 마당의 최대값을 얻는다.
	// 
	UINT GetIntMaxValue(LPCTSTR p_TblName, LPCTSTR p_FieldName, OUT int &p_Value);

	// 
	// 지적한 조회 결과가 빈모임이 아닌지 여부를 얻는다.
	// 
	BOOL IsNotNull(LPCTSTR p_Sql);

	//지적한 마당값을 형변환하여 돌려주는 함수들..
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
	// 데이터베이스 인스턴스
	// 
	CDaoDatabase *m_pDatabase;



};
