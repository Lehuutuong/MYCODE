#include "stdafx.h"




void SortUintArray ( IN OUT CUIntArray &p_Array) 
{ 
	int indexOfMin; 
	int pass; 
	int j; 
	int size = (int)p_Array.GetSize(); 
	UINT temp; 

	for ( pass = 0; pass < size - 1; pass++ ) 
	{ 
		indexOfMin = pass; 

		for ( j = pass + 1; j < size; j++ ) 
			if ( p_Array[j] < p_Array[pass] ) 
				indexOfMin = j; 

		temp = p_Array[pass]; 
		p_Array[pass] = p_Array[indexOfMin]; 
		p_Array[indexOfMin] = temp; 
	} 
} 

/* ===============================================================================================

�Լ�����:

    ���� ���μ����̹��� ������ �ִ� ������ ��θ� ��ȯ�մϴ�.

=============================================================================================== */
CString GetCurrentProcessDir()
{
	// 
	TCHAR path[MAX_PATH];
	TCHAR *pos;

	GetModuleFileName(NULL, path, MAX_PATH);
	pos = _tcsrchr(path, '\\');
	pos[0] = 0;

	return path;
}