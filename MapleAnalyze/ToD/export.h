///  Export. h
#if !defined(_EXPORT_INCLUDE_FILE_)
#define _EXPORT_INCLUDE_FILE_



int WINAPI ST(); // ���� �����Լ�
void GetCallStackInfo(DWORD Ebp);
void GetAllModArrInCurProc();
BOOL GetProcessNameFromPID(LPWSTR lpProcessName, DWORD PID);

#endif // _EXPORT_INCLUDE_FILE_