#include "StdAfx.h"
#include "DbManager.h"
#include "AppDebug.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef _DEBUG
#define DebugPrint
#endif
//////////////////////////////////////////////////////////////////////////
/* ===============================================================================================

함수설명:

	지적한 MDB파일을 창조하고 CDaoDatabase 인스턴스를 귀환한다.
	파일이 이미 존재하면 삭제한다.

파라메터:

	p_MdbName	: MDB파일의 완전경로

귀환값:

	CDaoDatabase 인스턴스 지적자. 실패하면 NULL

=============================================================================================== */

CDaoDatabase *DbManager::CreateDatabase(LPCTSTR p_MdbName)
{
	if (GetFileAttributes(p_MdbName) != INVALID_FILE_ATTRIBUTES)
	{
		// 파일이 이미 존재
		DebugPrint(L"%s 가 이미 존재");
		if (DeleteFile(p_MdbName) == FALSE)
		{
			DebugPrint(L"삭제처리에 실패");
			return NULL;
		}
	}

	CDaoDatabase *pDb = new CDaoDatabase;
	ASSERT(pDb != NULL);
	try
	{
		pDb->Create(p_MdbName);
	}
	catch (CDaoException* e)
	{
		delete pDb;
		pDb = NULL;

		TCHAR szCause[255];
		e->GetErrorMessage(szCause, 255);
		DebugPrint(L"DB 창조에서 예외 : %s", szCause);
		e->Delete();
		return NULL;
	}

	return pDb;
}


/* ===============================================================================================

함수설명:

	지적한 MDB파일을 오픈하고 CDaoDatabase 인스턴스를 창조하여 귀환한다.

파라메터:

	p_MdbName	: MDB파일의 완전경로

귀환값:

	CDaoDatabase 인스턴스 지적자. 실패하면 NULL

=============================================================================================== */
CDaoDatabase *DbManager::OpenDatabase(LPCTSTR p_MdbName)
{
	CDaoDatabase *pDb = new CDaoDatabase;
	ASSERT(pDb != NULL);
	try
	{
		pDb->Open(p_MdbName);
	}
	catch (CDaoException* e)
	{
		delete pDb;
		pDb = NULL;

		TCHAR szCause[255];
		e->GetErrorMessage(szCause, 255);
		DebugPrint(L"DB 오픈에서 예외 : %s", szCause);
		e->Delete();
		return NULL;
	}

	return pDb;
}

/* ===============================================================================================

함수설명:

    데이터베이스 인스턴스를 닫고 메모리를 해방한다.

=============================================================================================== */
void DbManager::CloseDatabase(IN CDaoDatabase *p_Database)
{
	if (!p_Database)
	{
		return;
	}

	p_Database->Close();
	delete p_Database;
}

//////////////////////////////////////////////////////////////////////////

DbManager::DbManager(void)
{
	m_pDatabase = NULL;
}

DbManager::~DbManager(void)
{
	Close();
}

/* ===============================================================================================

함수설명:

    MDB 파일을 오픈한다.

=============================================================================================== */
BOOL DbManager::Open(LPCTSTR p_MdbFile)
{
//	Close();

	m_pDatabase = OpenDatabase(p_MdbFile);

	return m_pDatabase != NULL;
}

/* ===============================================================================================

함수설명:

    MDB 파일을 닫는다.

=============================================================================================== */
void DbManager::Close()
{
	CloseDatabase(m_pDatabase);
	m_pDatabase = NULL;
}

void DbManager::PrintDbException(CDaoException* e)
{
	TCHAR szCause[255];
	e->GetErrorMessage(szCause, 255);
	DebugPrint(L"DB 예외 : %s", szCause);
	e->Delete();
}

/* ===============================================================================================

함수설명:

    T_LIST_EMP 테이블을 오픈하여 레코드셋을 귀환 합니다.

=============================================================================================== */
CDaoRecordset* DbManager::QuerySql(LPCTSTR p_SqlText)
{
	if (!IsOpend())
	{
		return NULL;
	}

	DebugPrint(p_SqlText);

	CDaoRecordset* pRecordset = NULL;
	try
	{
		//CRsBase *pRecSet = new CRsBase(m_pDatabase);
		//pRecSet->Open(dbOpenDynaset, L"SELECT * FROM T_LIST_EMP");

		pRecordset = new CDaoRecordset(m_pDatabase);
		//pRecordset->Open();
		
		pRecordset->Open(dbOpenDynaset/*AFX_DAO_USE_DEFAULT_TYPE*/, p_SqlText);
		return pRecordset;
	}
	catch (CDaoException* e)
	{
		PrintDbException(e);

		if (pRecordset) delete pRecordset;
	}
	return NULL;
}

/* ===============================================================================================

함수설명:

    SQL문을 실행하고 오류코드 귀환

=============================================================================================== */
UINT DbManager::ExecuteSql(LPCTSTR p_SqlText)
{
	DebugPrint(p_SqlText);

	try
	{
		m_pDatabase->Execute(p_SqlText);
		return DBERR_NOERROR;
	}
	catch (CDaoException* e)
	{
		PrintDbException(e);
		return DBERR_EXCEPTION;
	}
}

/* ===============================================================================================

함수설명:

    지적한 테이블의 마당의 최대값을 얻는다.
	마당은 옹근수형이여야 한다.

귀환값:
	DB 오유값

=============================================================================================== */
UINT DbManager::GetIntMaxValue(LPCTSTR p_TblName, LPCTSTR p_FieldName, OUT int &p_Value)
{
	UINT ret = DBERR_NOERROR;

	CString sql;
	sql.Format(L"select max(%s) from %s;", p_FieldName, p_TblName);

	CDaoRecordset *pRS = QuerySql(sql);
	if (pRS == NULL)
	{
		return DBERR_QUERY;
	}
	try
	{
		if (pRS->IsEOF())
		{
			p_Value = 0;
			goto L_EXIT;
		}

		COleVariant val;
		pRS->MoveFirst();
		val = pRS->GetFieldValue(0);

		if(val.vt == VT_NULL)
		{
			p_Value = 0;
			goto L_EXIT;
		}

		p_Value = val.intVal;
	}
	catch (CDaoException* e)
	{
		PrintDbException(e);

		ret = DBERR_EXCEPTION;
	}

L_EXIT:
	pRS->Close();
	delete pRS;

	return ret;
}

// 
// 지적한 조회 결과가 빈모임인가를 얻는다.
// 
BOOL DbManager::IsNotNull(LPCTSTR p_Sql)
{
	CDaoRecordset *pRS = QuerySql(p_Sql);
	if (pRS == NULL)
	{
		return FALSE;
	}

	BOOL ret;

	if (pRS->IsEOF())
		ret = FALSE;
	else
		ret = TRUE;

	pRS->Close();
	delete pRS;

	return ret;
}

BOOL DbManager::GetFieldBool(CDaoRecordset *pRS, int num)
{
	CDaoFieldInfo fieldInfo;
	COleVariant val;

	int nCols = pRS->GetFieldCount();
	if(nCols < num) return FALSE;
	pRS->GetFieldInfo(num, fieldInfo);

	if(fieldInfo.m_nType != dbBoolean) return FALSE;

	val = pRS->GetFieldValue(num);
	if(val.vt == VT_NULL)
	{
		return FALSE;
	}
	
	return val.boolVal;
}

DWORD DbManager::GetFieldInt(CDaoRecordset *pRS, int num)
{
	CDaoFieldInfo fieldInfo;
	COleVariant val;
	DWORD rVal = 0;

	int nCols = pRS->GetFieldCount();
	if(nCols < num) return 0;
	pRS->GetFieldInfo(num, fieldInfo);

	if(fieldInfo.m_nType != dbByte && fieldInfo.m_nType != dbInteger && fieldInfo.m_nType != dbLong) 
		return rVal;

	val = pRS->GetFieldValue(num);
	if(val.vt == VT_NULL)
	{
		return rVal;
	}

	if(fieldInfo.m_nType == dbByte)				rVal = val.bVal;
	else if(fieldInfo.m_nType == dbInteger)		rVal = val.iVal;
	else 	rVal = val.lVal;

	return rVal;
}

DOUBLE DbManager::GetFieldDouble(CDaoRecordset *pRS, int num)
{
	CDaoFieldInfo fieldInfo;
	COleVariant val;
	DOUBLE rVal = 0;

	int nCols = pRS->GetFieldCount();
	if(nCols < num) return 0;
	pRS->GetFieldInfo(num, fieldInfo);

	if(fieldInfo.m_nType != dbSingle && fieldInfo.m_nType != dbDouble) 
		return rVal;

	val = pRS->GetFieldValue(num);
	if(val.vt == VT_NULL)
	{
		return rVal;
	}

	if(fieldInfo.m_nType == dbSingle)				rVal = val.fltVal;
	else 	rVal = val.dblVal;

	return rVal;
}

CString DbManager::GetFieldStr(CDaoRecordset *pRS, int num)
{
	CDaoFieldInfo fieldInfo;
	COleVariant val;
	CString rStr = L"";

	int nCols = pRS->GetFieldCount();
	if(nCols < num) return 0;
	pRS->GetFieldInfo(num, fieldInfo);

	if(fieldInfo.m_nType != dbText && fieldInfo.m_nType != dbMemo) 
		return rStr;

	val = pRS->GetFieldValue(num);
	if(val.vt == VT_NULL)
	{
		return rStr;
	}

	rStr = (CString)val;
	
	return rStr;
}

SYSTEMTIME DbManager::GetFieldDate(CDaoRecordset *pRS, int num)
{
	CDaoFieldInfo fieldInfo;
	COleVariant val;
	SYSTEMTIME rDate;GetLocalTime(&rDate);

	int nCols = pRS->GetFieldCount();
	if(nCols < num) return rDate;
	pRS->GetFieldInfo(num, fieldInfo);

	if(fieldInfo.m_nType != dbDate) 
		return rDate;

	val = pRS->GetFieldValue(num);
	if(val.vt == VT_NULL)
	{
		return rDate;
	}

	COleDateTime daveVal = val.date;
	daveVal.GetAsSystemTime(rDate);
	return rDate;
}

