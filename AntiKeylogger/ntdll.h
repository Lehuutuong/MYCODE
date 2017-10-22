#ifndef _NTDLL_H_
#define _NTDLL_H_

#pragma comment (linker, "/defaultlib:ntdll.lib")

//
// Valid values for the Attributes field
//

#define OBJ_INHERIT             0x00000002L
#define OBJ_PERMANENT           0x00000010L
#define OBJ_EXCLUSIVE           0x00000020L
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define OBJ_OPENIF              0x00000080L
#define OBJ_OPENLINK            0x00000100L
#define OBJ_KERNEL_HANDLE       0x00000200L
#define OBJ_VALID_ATTRIBUTES    0x000003F2L

//
// Define LPC Message Types
//
#define LPC_REQUEST                     0x0001
#define LPC_REPLY                       0x0002
#define LPC_DATAGRAM                    0x0003
#define LPC_LOST_REPLY                  0x0004
#define LPC_PORT_CLOSED                 0x0005
#define LPC_CLIENT_DIED                 0x0006
#define LPC_EXCEPTION                   0x0007
#define LPC_DEBUG_EVENT                 0x0008
#define LPC_ERROR_EVENT                 0x0009
#define LPC_CONNECTION_REQUEST          0x000A

typedef LONG	NTSTATUS;

#define NT_SUCCESS(Status)				((LONG)(Status) >= 0)
#define NT_ERROR(Status)				((ULONG)(Status) >> 30 == 3)

#define ULONG_PTR unsigned long

typedef CONST char *PCSZ;

typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR Buffer;
} STRING;
typedef STRING *PSTRING;

typedef STRING ANSI_STRING;
typedef PSTRING PANSI_STRING;
typedef PSTRING PCANSI_STRING;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef struct _CLIENT_ID {
    DWORD	UniqueProcess;	//	0
    DWORD	UniqueThread;	//	4
} CLIENT_ID, *PCLIENT_ID;

typedef struct _USER_STACK {
	PVOID FixedStackBase;
	PVOID FixedStackLimit;
	PVOID ExpandableStackBase;
	PVOID ExpandableStackLimit;
	PVOID ExpandableStackBottom;
} USER_STACK, *PUSER_STACK;

typedef struct _PORT_MESSAGE {
	USHORT DataSize;// 0
	USHORT MessageSize;// 2
	USHORT MessageType;// 4
	USHORT VirtualRangesOffset;// 6
	CLIENT_ID CallerId;// 8
	ULONG MessageId;// 0x10
	ULONG SectionSize;// 0x14
	ULONG DebugEventCode;// 0x18
	ULONG Status;// 0x1c
	PVOID BufferLimit;// 0x20
	PVOID BufferBase;// 0x24
	ULONG BufferLength;// 0x28
	CLIENT_ID DebuggerId;// 0x2C
	ULONG Unknown;// 0x34
	DWORD Code;// 0x38
	LPVOID ImageBase;// 0x3C
} PORT_MESSAGE, *PPORT_MESSAGE;

//
//
//
typedef struct _PORT_SECTION_WRITE {
	ULONG	Length;
	HANDLE	SectionHandle;
	ULONG	SectionOffset;
	ULONG	ViewSize;
	PVOID	ViewBase;
	PVOID	TargetViewBase;
} PORT_SECTION_WRITE, *PPORT_SECTION_WRITE;

//
// The peer of a process that creates a port section learns about the base address and 
// view size of the section from the members of the PORT_SECTION_READ structure.
//
typedef struct _PORT_SECTION_READ {
	ULONG	Length;			// The size, in bytes, of the PORT_SECTION_READ structure.
	ULONG	ViewSize;		// The size, in bytes, of the view.
	PVOID	ViewBase;		// The base address of the view.
} PORT_SECTION_READ, *PPORT_SECTION_READ;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };
    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;	//	π§ª˛º≥∏Û  ÷À¡
} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

//
// Define structure about shared memory initialized from the other side of port
//
typedef struct _LPC_OTHER_SIDE_MEMORY {
    ULONG Length;			//in only - always must be initialized
    ULONG ViewSize;			//out only
    PVOID ViewBase;			//out only
} LPC_OTHER_SIDE_MEMORY, *PLPC_OTHER_SIDE_MEMORY;

typedef struct _LPC_THIS_SIDE_MEMORY {
    ULONG Length;				//in only - always must be initialized
    HANDLE SectionHandle;		//in only
    ULONG OffsetInSection;		//in, out
    ULONG ViewSize;				//in, out
    PVOID ViewBase;				//out only
    PVOID OtherSideViewBase;	//out only
} LPC_THIS_SIDE_MEMORY, *PLPC_THIS_SIDE_MEMORY;

typedef struct {
	WORD Architecture;
	WORD Level;
	WORD Revision;       /* combination of CPU model and stepping */
	WORD Reserved;       /* always zero */
	DWORD FeatureSet;    /* see bit flags below */
} SYSTEM_CPU_INFORMATION;

typedef enum {
    StateInitialized,	// 0
    StateReady,			// 1
    StateRunning,		// 2
    StateStandby,		// 3
    StateTerminated,	// 4
    StateWait,			// 5
    StateTransition,	// 6
	StateUnknown		// 7
}THREAD_STATE;

//
// Wait reasons
//

typedef enum _KWAIT_REASON {
    Executive,	
    FreePage,
    PageIn,
    PoolAllocation,
    DelayExecution,
    Suspended,
    UserRequest,
    WrExecutive,
    WrFreePage,
    WrPageIn,
    WrPoolAllocation,
    WrDelayExecution,
    WrSuspended,
    WrUserRequest,
    WrEventPair,
    WrQueue,
    WrLpcReceive,
    WrLpcReply,
    WrVirtualMemory,
    WrPageOut,
    WrRendezvous,
    Spare2,
    Spare3,
    Spare4,
    Spare5,
    Spare6,
    WrKernel,
    MaximumWaitReason
} KWAIT_REASON;

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation, // 0 Y N
	SystemProcessorInformation, // 1 Y N
	SystemPerformanceInformation, // 2 Y N
	SystemTimeOfDayInformation, // 3 Y N
	SystemNotImplemented1, // 4 Y N
	SystemProcessesAndThreadsInformation, // 5 Y N
	SystemCallCounts, // 6 Y N
	SystemConfigurationInformation, // 7 Y N
	SystemProcessorTimes, // 8 Y N
	SystemGlobalFlag, // 9 Y Y
	SystemNotImplemented2, // 10 Y N
	SystemModuleInformation, // 11 Y N
	SystemLockInformation, // 12 Y N
	SystemNotImplemented3, // 13 Y N
	SystemNotImplemented4, // 14 Y N
	SystemNotImplemented5, // 15 Y N
	SystemHandleInformation, // 16 Y N
	SystemObjectInformation, // 17 Y N
	SystemPagefileInformation, // 18 Y N
	SystemInstructionEmulationCounts, // 19 Y N
	SystemInvalidInfoClass1, // 20
	SystemCacheInformation, // 21 Y Y
	SystemPoolTagInformation, // 22 Y N
	SystemProcessorStatistics, // 23 Y N
	SystemDpcInformation, // 24 Y Y
	SystemNotImplemented6, // 25 Y N
	SystemLoadImage, // 26 N Y
	SystemUnloadImage, // 27 N Y
	SystemTimeAdjustment, // 28 Y Y
	SystemNotImplemented7, // 29 Y N
	SystemNotImplemented8, // 30 Y N
	SystemNotImplemented9, // 31 Y N
	SystemCrashDumpInformation, // 32 Y N
	SystemExceptionInformation, // 33 Y N
	SystemCrashDumpStateInformation, // 34 Y Y/N
	SystemKernelDebuggerInformation, // 35 Y N
	SystemContextSwitchInformation, // 36 Y N
	SystemRegistryQuotaInformation, // 37 Y Y
	SystemLoadAndCallImage, // 38 N Y
	SystemPrioritySeparation, // 39 N Y
	SystemNotImplemented10, // 40 Y N
	SystemNotImplemented11, // 41 Y N
	SystemInvalidInfoClass2, // 42
	SystemInvalidInfoClass3, // 43
	SystemTimeZoneInformation, // 44 Y N
	SystemLookasideInformation, // 45 Y N
	SystemSetTimeSlipEvent, // 46 N Y
	SystemCreateSession, // 47 N Y
	SystemDeleteSession, // 48 N Y
	SystemInvalidInfoClass4, // 49
	SystemRangeStartInformation, // 50 Y N
	SystemVerifierInformation, // 51 Y Y
	SystemAddVerifier, // 52 N Y
	SystemSessionProcessesInformation // 53 Y N
} SYSTEM_INFORMATION_CLASS;

typedef LONG KPRIORITY;

typedef struct _SYSTEM_THREADS {
    LARGE_INTEGER   KernelTime;
    LARGE_INTEGER   UserTime;
    LARGE_INTEGER   CreateTime;
    ULONG			WaitTime;
    PVOID			StartAddress;
    CLIENT_ID	    ClientId;
    KPRIORITY	    Priority;
    KPRIORITY	    BasePriority;
    ULONG			ContextSwitchCount;
    THREAD_STATE	State;
    KWAIT_REASON	WaitReason;
} SYSTEM_THREADS, * PSYSTEM_THREADS;

typedef struct _VM_COUNTERS {
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
} VM_COUNTERS;

/*
typedef struct _IO_COUNTERS {
    ULONGLONG  ReadOperationCount;
    ULONGLONG  WriteOperationCount;
    ULONGLONG  OtherOperationCount;
    ULONGLONG ReadTransferCount;
    ULONGLONG WriteTransferCount;
    ULONGLONG OtherTransferCount;
} IO_COUNTERS;
*/

typedef struct _SYSTEM_PROCESSES {
    ULONG			NextEntryDelta;
    ULONG			ThreadCount;
    ULONG			Reserved1[6];
    LARGE_INTEGER   CreateTime;
    LARGE_INTEGER   UserTime;
    LARGE_INTEGER   KernelTime;
    UNICODE_STRING  ProcessName;
    KPRIORITY	    BasePriority;
    ULONG			ProcessId;
    ULONG			InheritedFromProcessId;
    ULONG			HandleCount;
    ULONG			Reserved2[2];
    VM_COUNTERS	    VmCounters;
    IO_COUNTERS	    IoCounters;
    SYSTEM_THREADS  Threads[1];
} SYSTEM_PROCESSES, * PSYSTEM_PROCESSES;

typedef struct _PEB_LDR_DATA
{ 
	ULONG Length;
	UCHAR Initialized;
	PVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID EntryInProgress;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _LDR_MODULE {
	LIST_ENTRY              InLoadOrderModuleList;// 0
	LIST_ENTRY              InMemoryOrderModuleList;// 8
	LIST_ENTRY              InInitializationOrderModuleList;// 0x10
	PVOID                   BaseAddress;// 0x18
	PVOID                   EntryPoint;// 0x1C
	ULONG                   SizeOfImage;// 0x20
	UNICODE_STRING          FullDllName;// 0x24
	UNICODE_STRING          BaseDllName;// 0x2C
	ULONG                   Flags;// 0x34
	SHORT                   LoadCount;// 0x38
	SHORT                   TlsIndex;// 0x3A
	LIST_ENTRY              HashTableEntry;// 0x3C
	ULONG                   TimeDateStamp;// 0x44
} LDR_MODULE, *PLDR_MODULE;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	ULONG                   MaximumLength;
	ULONG                   Length;
	ULONG                   Flags;
	ULONG                   DebugFlags;
	PVOID                   ConsoleHandle;
	ULONG                   ConsoleFlags;
	HANDLE                  StdInputHandle;
	HANDLE                  StdOutputHandle;
	HANDLE                  StdErrorHandle;
	UNICODE_STRING          CurrentDirectoryPath;
	HANDLE                  CurrentDirectoryHandle;
	UNICODE_STRING          DllPath;
	UNICODE_STRING          ImagePathName;
	UNICODE_STRING          CommandLine;
	PVOID                   Environment;
	ULONG                   StartingPositionLeft;
	ULONG                   StartingPositionTop;
	ULONG                   Width;
	ULONG                   Height;
	ULONG                   CharWidth;
	ULONG                   CharHeight;
	ULONG                   ConsoleTextAttributes;
	ULONG                   WindowFlags;
	ULONG                   ShowWindowFlags;
	UNICODE_STRING          WindowTitle;
	UNICODE_STRING          DesktopName;
	UNICODE_STRING          ShellInfo;
	UNICODE_STRING          RuntimeData;
	//	RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB {
	BOOLEAN                 InheritedAddressSpace;
	BOOLEAN                 ReadImageFileExecOptions;
	BOOLEAN                 BeingDebugged;
	BOOLEAN                 Spare;
	HANDLE                  Mutant;
	PVOID                   ImageBaseAddress;
	PPEB_LDR_DATA           LoaderData;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	PVOID                   SubSystemData;
	PVOID                   ProcessHeap;
	PVOID                   FastPebLock;
	PVOID					FastPebLockRoutine;
	PVOID					FastPebUnlockRoutine;
	ULONG                   EnvironmentUpdateCount;
	PVOID                   KernelCallbackTable;
	PVOID                   EventLogSection;
	PVOID                   EventLog;
	PVOID					FreeList;
	ULONG                   TlsExpansionCounter;
	PVOID                   TlsBitmap;
	ULONG                   TlsBitmapBits[0x2];
	PVOID                   ReadOnlySharedMemoryBase;
	PVOID                   ReadOnlySharedMemoryHeap;
	PVOID					ReadOnlyStaticServerData;
	PVOID                   AnsiCodePageData;
	PVOID                   OemCodePageData;
	PVOID                   UnicodeCaseTableData;
	ULONG                   NumberOfProcessors;
	ULONG                   NtGlobalFlag;
	BYTE                    Spare2[0x4];
	LARGE_INTEGER           CriticalSectionTimeout;
	ULONG                   HeapSegmentReserve;
	ULONG                   HeapSegmentCommit;
	ULONG                   HeapDeCommitTotalFreeThreshold;
	ULONG                   HeapDeCommitFreeBlockThreshold;
	ULONG                   NumberOfHeaps;
	ULONG                   MaximumNumberOfHeaps;
	PVOID*					ProcessHeaps;
	PVOID                   GdiSharedHandleTable;
	PVOID                   ProcessStarterHelper;
	PVOID                   GdiDCAttributeList;
	PVOID                   LoaderLock;
	ULONG                   OSMajorVersion;
	ULONG                   OSMinorVersion;
	ULONG                   OSBuildNumber;
	ULONG                   OSPlatformId;
	ULONG                   ImageSubSystem;
	ULONG                   ImageSubSystemMajorVersion;
	ULONG                   ImageSubSystemMinorVersion;
	ULONG                   GdiHandleBuffer[0x22];
	ULONG                   PostProcessInitRoutine;
	ULONG                   TlsExpansionBitmap;
	BYTE                    TlsExpansionBitmapBits[0x80];
	ULONG                   SessionId;
} PEB, *PPEB;


//
//
//
extern "C"
NTSYSAPI
NTSTATUS
NTAPI
RtlRunDecodeUnicodeString(
	BYTE, 
	PUNICODE_STRING
	); 

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwLoadDriver(
	IN PUNICODE_STRING DriverServiceName
	);

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwUnloadDriver(
	IN PUNICODE_STRING DriverServiceName
	);

//
//	Opens a file.
//
extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwOpenFile(
    OUT PHANDLE  FileHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN ULONG  ShareAccess,
    IN ULONG  OpenOptions
    );

extern "C"
//
// Completes the port connection process.
//
NTSYSAPI
NTSTATUS
NTAPI
ZwCompleteConnectPort (
    IN HANDLE PortHandle
    );

extern "C"
//
//	Accepts or rejects a connection request.
//
NTSYSAPI
NTSTATUS
NTAPI
ZwAcceptConnectPort (
    OUT PHANDLE ServerPortHandle,	// Points to a variable that will receive the port object handle
    IN ULONG PortIdentifier,		// A numeric identifier to be associated with the port.
    IN PPORT_MESSAGE Message,		// Points to a caller-allocated buffer or variable that identifies 
									// the connection request and contains any connect data 
									// that should be returned to requestor of the connection.
    IN BOOLEAN Accept,				// Specifies whether the connection should be accepted or not.
    IN OUT PPORT_SECTION_WRITE WriteSection OPTIONAL,	// Points to a structure describing the shared memory region 
														// used to send large amounts of data to the requestor; 
														// if the call is successful, this will be updated.
    IN OUT PPORT_SECTION_READ ReadSection OPTIONAL		// Points to a caller-allocated buffer or variable 
														// that receives information on the shared memory region used 
														// by the requestor to send large amounts of data to the caller.
    );

extern "C"
//
//	Creates or opens an event object.
//
NTSYSAPI
NTSTATUS
NTAPI
ZwCreateEvent (
	OUT	PHANDLE				EventHandle,
	IN	ACCESS_MASK			DesiredAccess,
	IN	POBJECT_ATTRIBUTES	ObjectAttributes,
	IN	BOOLEAN				ManualReset,
	IN	BOOLEAN				InitialState
	);

//
// Duplicates the handle to an object.
//
extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwDuplicateObject(
    IN HANDLE SourceProcessHandle,
    IN HANDLE SourceHandle,
    IN HANDLE TargetProcessHandle OPTIONAL,
    OUT PHANDLE TargetHandle OPTIONAL,
    IN ACCESS_MASK DesiredAccess,
    IN ULONG HandleAttributes,
    IN ULONG Options
    );

//
//
//
extern "C"
NTSYSAPI
VOID
NTAPI
RtlInitUnicodeString(
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
    );

extern "C"
//
//	Creates a port connected a named port.
//
NTSYSAPI
NTSTATUS
NTAPI
ZwConnectPort (
    OUT PHANDLE ClientPortHandle,
    IN PUNICODE_STRING ServerPortName,
    IN PSECURITY_QUALITY_OF_SERVICE SecurityQos,
    IN OUT PLPC_THIS_SIDE_MEMORY ClientSharedMemory OPTIONAL,
    IN OUT PLPC_OTHER_SIDE_MEMORY ServerSharedMemory OPTIONAL,
    OUT PULONG MaximumMessageLength OPTIONAL,
    IN OUT PVOID ConnectionInfo OPTIONAL,
    IN OUT PULONG ConnectionInfoLength OPTIONAL
    );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwCreatePort (
    OUT PHANDLE PortHandle,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN ULONG MaximumConnectionInfoLength,
    IN ULONG MaximumMessageLength,
    IN ULONG Reserved
    );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwRequestPort (
    IN HANDLE PortHandle,
    IN PPORT_MESSAGE Request
    );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwReplyWaitReceivePort (
    IN HANDLE			PortHandle,
    OUT PULONG			ReceivePortHandle OPTIONAL,
    IN PPORT_MESSAGE		Reply OPTIONAL,
    OUT PPORT_MESSAGE	Request
    );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwRequestWaitReplyPort(
    IN HANDLE PortHandle,
    IN PPORT_MESSAGE Request,
    OUT PPORT_MESSAGE Reply
    );

extern "C"
//
// Creates a section object.
//
NTSYSAPI
NTSTATUS
NTAPI
ZwCreateSection(
	OUT PHANDLE  SectionHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes, // Optional
	IN PLARGE_INTEGER MaximumSize, // Optional
	IN ULONG  SectionPageProtection,
	IN ULONG  AllocationAttributes,
	IN HANDLE  FileHandle // Optional
	);

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwCreateMutant (
    IN PHANDLE MutantHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN BOOLEAN InitialOwner
    );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwCreateSemaphore (
    OUT PHANDLE SemaphoreHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN LONG InitialCount,
    IN LONG MaximumCount
    );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation (
    IN SYSTEM_INFORMATION_CLASS	SystemInformationClass,
	IN OUT PVOID	SystemInformation,
	IN ULONG	SystemInformationLength,
	OUT	PULONG	ReturnLength OPTIONAL
	);

/*
typedef enum _COMPUTER_NAME_FORMAT {
    ComputerNameNetBIOS,
    ComputerNameDnsHostname,
    ComputerNameDnsDomain,
    ComputerNameDnsFullyQualified,
    ComputerNamePhysicalNetBIOS,
    ComputerNamePhysicalDnsHostname,
    ComputerNamePhysicalDnsDomain,
    ComputerNamePhysicalDnsFullyQualified,
    ComputerNameMax
} COMPUTER_NAME_FORMAT ;
*/

extern "C"
WINBASEAPI
BOOL
WINAPI
GetComputerNameExW (
    IN COMPUTER_NAME_FORMAT NameType,
    OUT LPWSTR lpBuffer,
    IN OUT LPDWORD nSize
    );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwClose(
    IN HANDLE Handle
    );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwWaitForSingleObject(
	IN HANDLE hObject,
	IN BOOLEAN bAlertable,
	IN PLARGE_INTEGER Timeout
);

typedef enum _WAIT_TYPE {
	WaitAll,				// Wait for any handle to be signaled
	WaitAny					// Wait for all handles to be signaled
} WAIT_TYPE, *PWAIT_TYPE;

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwWaitForMultipleObjects(
	IN ULONG nWaitObjectHandles,
	IN PHANDLE WaitObjectHandlesArray,
	IN WAIT_TYPE WaitType,
	IN BOOLEAN bAlertable,
	IN PLARGE_INTEGER Timeout
);

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwOpenEvent(
	OUT PHANDLE hEvent,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes
	);

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwSetEvent(
	IN HANDLE hEvent,
	OUT OPTIONAL PULONG PreviousState
	);

extern "C"
NTSYSAPI
LONG
NTAPI
RtlCompareUnicodeString(
    PUNICODE_STRING String1,
    PUNICODE_STRING String2,
    BOOLEAN CaseInSensitive
    );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwOpenProcess(
	OUT PHANDLE phProcess,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PCLIENT_ID pClientId
);

//ProcessSessionInformation
typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation,
	ProcessQuotaLimits,
	ProcessIoCounters,
	ProcessVmCounters,
	ProcessTimes,
	ProcessBasePriority,
	ProcessRaisePriority,
	ProcessDebugPort,
	ProcessExceptionPort,
	ProcessAccessToken,
	ProcessLdtInformation,
	ProcessLdtSize,
	ProcessDefaultHardErrorMode,
	ProcessIoPortHandlers,
	ProcessPooledUsageAndLimits,
	ProcessWorkingSetWatch,
	ProcessUserModeIOPL,
	ProcessEnableAlignmentFaultFixup,
	ProcessPriorityClass,
	ProcessWx86Information,
	ProcessHandleCount,
	ProcessAffinityMask,
	ProcessPriorityBoost,
	ProcessDeviceMap,
	ProcessSessionInformation,
	ProcessForegroundInformation,
	ProcessWow64Information
} PROCESSINFOCLASS;

typedef struct _PROCESS_SESSION_INFORMATION {
    ULONG SessionId;
} PROCESS_SESSION_INFORMATION, *PPROCESS_SESSION_INFORMATION;

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationProcess(
	IN HANDLE hProcess,
	IN PROCESSINFOCLASS ProcessInfoClass,
	OUT PVOID ProcessInfoBuffer,
	IN ULONG ProcessInfoBufferLength,
	OUT PULONG BytesReturned OPTIONAL
);

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwOpenProcessToken(
	IN HANDLE hProcess,
	IN ACCESS_MASK DesiredAccess,
	OUT PHANDLE phToken
);

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationToken(
	IN HANDLE hToken,
	IN TOKEN_INFORMATION_CLASS TokenInfoClass,
	OUT PVOID TokenInfoBuffer,
	IN ULONG TokenInfoBufferLength,
	OUT PULONG BytesReturned
);

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationToken(
	IN HANDLE hToken,
	IN TOKEN_INFORMATION_CLASS TokenInfoClass,
	IN PVOID TokenInfoBuffer,
	IN ULONG TokenInfoBufferLength
);

extern "C"
NTSTATUS
NTAPI
ZwAdjustPrivilegesToken(
	IN HANDLE hToken,
    IN BOOLEAN DisableAllPrivileges,
    IN PTOKEN_PRIVILEGES pNewPrivlegeSet,
    IN ULONG PreviousPrivilegeSetBufferLength OPTIONAL,
    PTOKEN_PRIVILEGES pPreviousPrivlegeSet OPTIONAL,
    PULONG PreviousPrivlegeSetReturnLength OPTIONAL
);

#define SE_SHUTDOWN_PRIVILEGE             (19L)

typedef enum _EVENT_TYPE {
    NotificationEvent,
    SynchronizationEvent
    } EVENT_TYPE;

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
RtlInitializeCriticalSection (
							  RTL_CRITICAL_SECTION* crit
							  );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
RtlEnterCriticalSection (
						 RTL_CRITICAL_SECTION* crit
						 );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
RtlLeaveCriticalSection (
						 RTL_CRITICAL_SECTION* crit
						 );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
RtlDeleteCriticalSection (
						 RTL_CRITICAL_SECTION* crit
						 );

extern "C"
NTSYSAPI
VOID
NTAPI
RtlFreeAnsiString (
    PANSI_STRING AnsiString
    );

extern "C"
NTSYSAPI
VOID
NTAPI
RtlFreeUnicodeString (
    PUNICODE_STRING UnicodeString
    );

extern "C"
NTSYSAPI
VOID
NTAPI
RtlInitString (
    PSTRING DestinationString,
    PCSZ SourceString
    );

extern "C"
NTSYSAPI
VOID
NTAPI
RtlInitAnsiString (
    PANSI_STRING DestinationString,
    PCSZ SourceString
    );

extern "C"
NTSYSAPI
VOID
NTAPI
RtlInitUnicodeString (
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
    );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
RtlAnsiStringToUnicodeString (
    PUNICODE_STRING DestinationString,
    PCANSI_STRING SourceString,
    BOOLEAN AllocateDestinationString
    );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeStringToAnsiString (
    PANSI_STRING DestinationString,
    PCUNICODE_STRING SourceString,
    BOOLEAN AllocateDestinationString
    );

extern "C"
NTSYSAPI
PVOID
NTAPI
RtlAllocateHeap(
				HANDLE heap,
				ULONG  flags,
				SIZE_T size
				);

extern "C"
NTSYSAPI
BOOLEAN 
NTAPI
RtlFreeHeap(
			HANDLE heap,
			ULONG  flags,
			PVOID  ptr
			);

extern "C"
NTSYSAPI
NTSTATUS
NTAPI 
ZwAllocateVirtualMemory(
						IN HANDLE  ProcessHandle,
						IN OUT PVOID  *BaseAddress,
						IN ULONG  ZeroBits,
						IN OUT PSIZE_T  RegionSize,
						IN ULONG  AllocationType,
						IN ULONG  Protect
						);

extern "C"
NTSYSAPI
NTSTATUS
NTAPI
ZwFreeVirtualMemory (
					 IN HANDLE ProcessHandle,
					 IN OUT PVOID* BaseAddress,
					 IN OUT PULONG FreeSize,
					 IN ULONG FreeType
					 );

// Process functions
#define NtCurrentProcess()				((HANDLE) -1)

extern "C"
NTSYSAPI
NTSTATUS
NTAPI 
ZwCreateThread(
			   OUT HANDLE  ThreadHandle,
			   IN  ACCESS_MASK  DesiredAccess,
			   IN  POBJECT_ATTRIBUTES  ObjectAttributes,
			   IN  HANDLE  ProcessHandle,
			   OUT PCLIENT_ID  ClientId,
			   IN  PCONTEXT  ThreadContext,
			   IN  PUSER_STACK UserStack,
			   IN  BOOLEAN  CreateSuspended
			   );

extern "C"
NTSYSAPI
NTSTATUS
NTAPI 
ZwTerminateThread(
				  IN HANDLE ThreadHandle OPTIONAL,
				  IN NTSTATUS ExitStatus
				  );

extern "C"
NTSYSAPI 
NTSTATUS
NTAPI
LdrLoadDll(
		   IN PWCHAR               PathToFile OPTIONAL,
		   IN ULONG                Flags OPTIONAL,
		   IN PUNICODE_STRING      ModuleFileName,
		   OUT PHANDLE             ModuleHandle 
		   );

extern "C"
NTSYSAPI 
NTSTATUS
NTAPI
LdrUnloadDll(
			 IN HANDLE	ModuleHandle
			 );

extern "C"
NTSYSAPI 
NTSTATUS
NTAPI
NtTerminateProcess(
				   IN HANDLE               ProcessHandle OPTIONAL,
				   IN NTSTATUS             ExitStatus 
				   );

#endif // _NTDLL_H_