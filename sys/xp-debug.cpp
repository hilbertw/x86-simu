#define  _CRT_SECURE_NO_WARNINGS 1
#include <conio.h>
#include <stdio.h>
#include "cpu.h"
#include "util.h"
#include "log.h"
extern CMCH mch;

extern CCPU cpu[CFG_N_CPUS];
struct field_def
{
	unsigned long offset;
	unsigned long size;
	char * desc;
	struct field_def *def;
};

struct field_def  _KAPC_STATE[]=
{
	{0,0x10,"ApclistHead"}, //+0x000 ApcListHead      : [2] _LIST_ENTRY
	{0x10,4,"Process"},  //+0x010 Process          : Ptr32 _KPROCESS
	{0x14,1,"KernelApcInProgress"},  //+0x014 KernelApcInProgress : UChar
	{0x15,1,"KernelApcPending"},  //+0x015 KernelApcPending : UChar
	{0x16,1,"UserApcPending"},  //+0x016 UserApcPending   : UChar
	{-1,-1,NULL}
};

struct field_def  _DISPATCHER_HEADER32[]=
{
    {0,1,"Type"},     //UCHAR Type;
    {1,1,"Absolute"}, //UCHAR Absolute;
    {2,1,"Size"},     //UCHAR Size;
    {3,1,"Inserted"}, //UCHAR Inserted;
    {4,4,"SignalState"}, //LONG SignalState;
    {8,8,"WaitListHead"}, //LIST_ENTRY32 WaitListHead;
	{-1,-1,NULL}
};

struct field_def  _ULARGE_INTEGER[]=
{
    {0,4,"LowPart"},     
    {4,4,"HighPart"},
	{-1,-1,NULL}
};

struct field_def  _KTIMER[]=
{
	{0,0x10,"Header",_DISPATCHER_HEADER32},		   //DISPATCHER_HEADER32  Header;
	{0x10,8,"DueTime",_ULARGE_INTEGER},		   //ULARGE_INTEGER DueTime;
	{0x18,8,"TimerListEntry"}, //LIST_ENTRY TimerListEntry;
	{0x20,4,"Dpc"},		       //struct _KDPC *Dpc;
	{0x24,4,"Period"},		   //LONG Period;
	{-1,-1,NULL}
};
struct field_def  _KTRAP_FRAME[]=
{
	{0x000,4,"DbgEbp"},			//   +0x000 DbgEbp           : Uint4B
	{0x004,4,"DbgEip"},			//   +0x004 DbgEip           : Uint4B
	{0x008,4,"DbgArgMark"},     //   +0x008 DbgArgMark       : Uint4B
	{0x00c,4,"DbgArgPointer"},  //   +0x00c DbgArgPointer    : Uint4B
	{0x010,4,"TempSegCs"},		//   +0x010 TempSegCs        : Uint4B
	{0x014,4,"TempEsp"},  		//   +0x014 TempEsp          : Uint4B
	{0x018,4,"Dr0"},			//   +0x018 Dr0              : Uint4B
	{0x01c,4,"Dr1"},			//   +0x01c Dr1              : Uint4B
	{0x020,4,"Dr2"},			//   +0x020 Dr2              : Uint4B
	{0x024,4,"Dr3"},			//   +0x024 Dr3              : Uint4B
	{0x028,4,"Dr6"},			//   +0x028 Dr6              : Uint4B
	{0x02c,4,"Dr7"},			//   +0x02c Dr7              : Uint4B
	{0x030,4,"SegGs"},			//   +0x030 SegGs            : Uint4B
	{0x034,4,"SegEs"},			//   +0x034 SegEs            : Uint4B
	{0x038,4,"SegDs"},			//   +0x038 SegDs            : Uint4B
	{0x03c,4,"Edx"},			//   +0x03c Edx              : Uint4B
	{0x040,4,"Ecx"},			//   +0x040 Ecx              : Uint4B
	{0x044,4,"Eax"},			//   +0x044 Eax              : Uint4B
	{0x048,4,"PreviousMode"},   //   +0x048 PreviousMode    : Uint4B
	{0x04c,4,"ExceptionList"},  //   +0x04c ExceptionList    : Ptr32 _EXCEPTION_REGISTRATION_RECORD
	{0x050,4,"SegFs"},          //   +0x050 SegFs            : Uint4B
	{0x054,4,"Edi"},			//   +0x054 Edi              : Uint4B
	{0x058,4,"Esi"},			//   +0x058 Esi              : Uint4B
	{0x05c,4,"Ebx"},			//   +0x05c Ebx              : Uint4B
	{0x060,4,"Ebp"},			//   +0x060 Ebp              : Uint4B
	{0x064,4,"ErrCode"},		//   +0x064 ErrCode          : Uint4B
	{0x068,4,"Eip"},    		//   +0x068 Eip              : Uint4B
	{0x06c,4,"SegCs"},  		//   +0x06c SegCs            : Uint4B
	{0x070,4,"EFlags"}, 		//   +0x070 EFlags           : Uint4B
	{0x074,4,"HardwareEsp"},  	//   +0x074 HardwareEsp      : Uint4B
	{0x078,4,"HardwareSegSs"},	//   +0x078 HardwareSegSs    : Uint4B
	{0x07c,4,"V86Es"},			//   +0x07c V86Es            : Uint4B
	{0x080,4,"V86Ds"},			//   +0x080 V86Ds            : Uint4B
	{0x084,4,"V86Fs"},			//   +0x084 V86Fs            : Uint4B
	{0x088,4,"V86Gs"},			//   +0x088 V86Gs            : Uint4B
	{-1,-1,NULL}
};

struct field_def  _KTHREAD[]=
{

	{0x000,0x10,"Header"},         //+0x000 Header : _DISPATCHER_HEADER
	{0x010,8,"MutantListHead"}, //+0x010 MutantListHead : _LIST_ENTRY
	{0x018,4,"InitialStack"},	//+0x018 InitialStack : Ptr32 Void
	{0x01c,4,"StackLimit"},  	//+0x01c StackLimit : Ptr32 Void
	{0x020,4,"Teb"},     		//+0x020 Teb : Ptr32 Void
	{0x024,4,"TlsArray"},		//+0x024 TlsArray : Ptr32 Void
	{0x028,4,"KernelStack"},	//+0x028 KernelStack : Ptr32 Void
	{0x02c,1,"DebugActive"},	//+0x02c DebugActive : UChar
	{0x02d,1,"State"},  		//+0x02d State : UChar
	{0x02e,2,"Alerted"},		//+0x02e Alerted : [2] UChar
	{0x030,1,"Iopl"},   		//+0x030 Iopl : UChar
	{0x031,1,"NexState"},       //+0x031 NexState : UChar
	{0x032,1,"Saturation"},     //+0x032 Saturation : Char
	{0x033,1,"Priority"},		//+0x033 Priority : Char
	{0x034,0x18,"ApcState"},		//+0x034 ApcState : _KAPC_STATE
	{0x04c,4,"ContextSwitches"},//+0x04c ContextSwitches : Uint4B
	{0x050,1,"IdleSwapBlock"},  //+0x050 IdleSwapBlock : UChar
	{0x051,3,"Spare0"},    		//+0x051 Spare0 : [3] UChar
	{0x054,4,"WaitStatus"},		//+0x054 WaitStatus : Int4B
	{0x058,1,"WaitIrql"},		//+0x058 WaitIrql : UChar
	{0x059,1,"WaitMode"},		//+0x059 WaitMode : Char
	{0x05a,1,"WaitNext"},		//+0x05a WaitNext : UChar
	{0x05b,1,"WaitReason"},   	//+0x05b WaitReason : UChar
	{0x05c,4,"WaitBlockList"},	//+0x05c WaitBlockList : Ptr32 _KWAIT_BLOCK
	{0x060,4,"WaitListEntry"},	//+0x060 WaitListEntry : _LIST_ENTRY
	{0x060,4,"SwapListEntry"},	//+0x060 SwapListEntry : _SINGLE_LIST_ENTRY
	{0x068,4,"WaitTime"},     	//+0x068 WaitTime : Uint4B
	{0x06c,1,"BasePriority"},     	//+0x06c BasePriority : Char
	{0x06d,1,"DecrementCount"},   	//+0x06d DecrementCount : UChar
	{0x06e,1,"PriorityDecrement"},	//+0x06e PriorityDecrement : Char
	{0x06f,1,"Quantum"},  			//+0x06f Quantum : Char
	{0x070,4,"WaitBlock"},			//+0x070 WaitBlock : [4] _KWAIT_BLOCK
	{0x0d0,4,"LegoData"}, 			//+0x0d0 LegoData : Ptr32 Void
	{0x0d4,4,"KernelApcDisable"},	//+0x0d4 KernelApcDisable : Uint4B
	{0x0d8,4,"UserAffinity"},    	//+0x0d8 UserAffinity : Uint4B
	{0x0dc,1,"SystemAffinityActive"},//+0x0dc SystemAffinityActive : UChar
	{0x0dd,1,"PowerState"}, 		//+0x0dd PowerState : UChar
	{0x0de,1,"NpxIrql"},    		//+0x0de NpxIrql : UChar
	{0x0df,1,"InitialNode"},		//+0x0df InitialNode : UChar
	{0x0e0,4,"ServiceTable"},		//+0x0e0 ServiceTable : Ptr32 Void
	{0x0e4,4,"Queue"},       		//+0x0e4 Queue : Ptr32 _KQUEUE
	{0x0e8,4,"ApcQueueLock"},		//+0x0e8 ApcQueueLock : Uint4B
	{0x0f0,0x28,"Timer"},       	//+0x0f0 Timer : _KTIMER
	{0x118,4,"QueueListEntry"},		//+0x118 QueueListEntry : _LIST_ENTRY
	{0x120,4,"SoftAffinity"},  		//+0x120 SoftAffinity : Uint4B
	{0x124,4,"Affinity"}, 			//+0x124 Affinity : Uint4B
	{0x128,1,"Preempted"},			//+0x128 Preempted : UChar
	{0x129,1,"ProcessReadyQueue"},  //+0x129 ProcessReadyQueue : UChar
	{0x12a,1,"KernelStackResident"},//+0x12a KernelStackResident : UChar
	{0x12b,1,"NextProcessor"},		//+0x12b NextProcessor : UChar
	{0x12c,4,"CallbackStack"},		//+0x12c CallbackStack : Ptr32 Void
	{0x130,4,"Win32Thread"},		//+0x130 Win32Thread : Ptr32 Void
	{0x134,4,"TrapFrame"},  		//+0x134 TrapFrame : Ptr32 _KTRAP_FRAME
	{0x138,8,"ApcStatePointer"},	//+0x138 ApcStatePointer : [2] Ptr32 _KAPC_STATE
	{0x140,1,"PreviousMode"},   	//+0x140 PreviousMode : Char
	{0x141,1,"EnableStackSwap"},	//+0x141 EnableStackSwap : UChar
	{0x142,1,"LargeStack"},   		//+0x142 LargeStack : UChar
	{0x143,1,"ResourceIndex"},		//+0x143 ResourceIndex : UChar
	{0x144,4,"KernelTime"},			//+0x144 KernelTime : Uint4B
	{0x148,4,"UserTime"},  			//+0x148 UserTime : Uint4B
	{0x14c,0x18,"SavedApcState"},	//+0x14c SavedApcState : _KAPC_STATE
	{0x164,1,"Alertable"},    		//+0x164 Alertable : UChar
	{0x165,1,"ApcStateIndex"},		//+0x165 ApcStateIndex : UChar
	{0x166,1,"ApcQueueable"}, 		//+0x166 ApcQueueable : UChar
	{0x167,1,"AutoAlignment"},		//+0x167 AutoAlignment : UChar
	{0x168,4,"StackBase"}, 			//+0x168 StackBase : Ptr32 Void
	{0x16c,0x30,"SuspendApc"},		//+0x16c SuspendApc : _KAPC
	{0x19c,0x14,"SuspendSemaphore"},//+0x19c SuspendSemaphore : _KSEMAPHORE
	{0x1b0,4,"ThreadListEntry"},	//+0x1b0 ThreadListEntry : _LIST_ENTRY
	{0x1b8,1,"FreezeCount"},   		//+0x1b8 FreezeCount : Char
	{0x1b9,1,"SuspendCount"},  		//+0x1b9 SuspendCount : Char
	{0x1ba,1,"IdealProcessor"},		//+0x1ba IdealProcessor : UChar
	{0x1bb,1,"DisableBoost"},  		//+0x1bb DisableBoost : UChar
 	{-1,-1,NULL}
};
struct field_def  _KUSER_SHARED_DATA[]=
{

	{0x000,4,"TickCountLow"},               //+0x000 TickCountLow     : Uint4B
	{0x004,4,"TickCountMultiplier"},        //+0x004 TickCountMultiplier : Uint4B
	{0x008,4,"InterruptTime"},              //+0x008 InterruptTime    : _KSYSTEM_TIME
	{0x014,4,"SystemTime"},  				//+0x014 SystemTime       : _KSYSTEM_TIME
	{0x020,4,"TimeZoneBias"},				//+0x020 TimeZoneBias     : _KSYSTEM_TIME
	{0x02c,2,"ImageNumberLow"},             //+0x02c ImageNumberLow   : Uint2B
	{0x02e,2,"ImageNumberHigh"},   			//+0x02e ImageNumberHigh  : Uint2B
	{0x030,4,"NtSystemRoot"},      			//+0x030 NtSystemRoot     : [260] Uint2B
	{0x238,4,"MaxStackTraceDepth"},			//+0x238 MaxStackTraceDepth : Uint4B
	{0x23c,4,"CryptoExponent"},    			//+0x23c CryptoExponent   : Uint4B
	{0x240,4,"TimeZoneId"},					//+0x240 TimeZoneId       : Uint4B
	{0x244,0x20,"Reserved2"}, 					//+0x244 Reserved2        : [8] Uint4B
	{0x264,4,"NtProductType"},     			//+0x264 NtProductType    : _NT_PRODUCT_TYPE
	{0x268,1,"ProductTypeIsValid"},			//+0x268 ProductTypeIsValid : UChar
	{0x26c,4,"NtMajorVersion "},			//+0x26c NtMajorVersion   : Uint4B
	{0x270,4,"NtMinorVersion "},			//+0x270 NtMinorVersion   : Uint4B
	{0x274,0x40,"ProcessorFeatures"},          //+0x274 ProcessorFeatures : [64] UChar
	{0x2b4,4,"Reserved1"},					//+0x2b4 Reserved1        : Uint4B
	{0x2b8,4,"Reserved3"},					//+0x2b8 Reserved3        : Uint4B
	{0x2bc,4,"TimeSlip"}, 					//+0x2bc TimeSlip         : Uint4B
	{0x2c0,8,"AlternativeArchitecture"},	//+0x2c0 AlternativeArchitecture : _ALTERNATIVE_ARCHITECTURE_TYPE
	{0x2c8,8,"SystemExpirationDate"},   	//+0x2c8 SystemExpirationDate : _LARGE_INTEGER
	{0x2d0,4,"SuiteMask"},                  //+0x2d0 SuiteMask        : Uint4B
	{0x2d4,1,"KdDebuggerEnabled"},          //+0x2d4 KdDebuggerEnabled : UChar
	{0x2d5,1,"NXSupportPolicy"},			//+0x2d5 NXSupportPolicy  : UChar
	{0x2d8,4,"ActiveConsoleId"},			//+0x2d8 ActiveConsoleId  : Uint4B
	{0x2dc,4,"DismountCount"},  			//+0x2dc DismountCount    : Uint4B
	{0x2e0,4,"ComPlusPackage"}, 			//+0x2e0 ComPlusPackage   : Uint4B
	{0x2e4,4,"LastSystemRITEventTickCount"},//+0x2e4 LastSystemRITEventTickCount : Uint4B
	{0x2e8,4,"NumberOfPhysicalPages"},      //+0x2e8 NumberOfPhysicalPages : Uint4B
	{0x2ec,4,"SafeBootMode"},				//+0x2ec SafeBootMode     : UChar
	{0x2f0,4,"TraceLogging"},				//+0x2f0 TraceLogging     : Uint4B
	{0x2f8,4,"TestRetInstruction"}, 		//+0x2f8 TestRetInstruction : Uint8B
	{0x300,4,"SystemCall"},         		//+0x300 SystemCall       : Uint4B
	{0x304,4,"SystemCallReturn"},   		//+0x304 SystemCallReturn : Uint4B
	{0x308,0x18,"SystemCallPad"},				//+0x308 SystemCallPad    : [3] Uint8B
	{0x320,8,"TickCount"},    				//+0x320 TickCount        : _KSYSTEM_TIME
	{0x320,8,"TickCountQuad"},				//+0x320 TickCountQuad    : Uint8B
	{0x330,4,"Cookie"},       				//+0x330 Cookie           : Uint4B
 	{-1,-1,NULL}
};
struct field_def  _KPRCB[]=
{

	{0x000,4,"MinorVersion"}, 			//+0x000 MinorVersion  : Uint2B
	{0x002,4,"MajorVersion"}, 			//+0x002 MajorVersion  : Uint2B
	{0x004,4,"CurrentThread"},			//+0x004 CurrentThread : Ptr32 _KTHREAD
	{0x008,4,"NextThread"},				//+0x008 NextThread    : Ptr32 _KTHREAD
	{0x00c,4,"IdleThread"},				//+0x00c IdleThread    : Ptr32 _KTHREAD
	{0x010,4,"Number"},  				//+0x010 Number        : Char
	{0x011,4,"Reserved"},				//+0x011 Reserved      : Char
	{0x012,4,"BuildType"},				//+0x012 BuildType     : Uint2B
	{0x014,4,"SetMember"},				//+0x014 SetMember     : Uint4B
	{0x018,4,"CpuType"},				//+0x018 CpuType       : Char
	{0x019,4,"CpuID"},  				//+0x019 CpuID         : Char
	{0x01a,4,"CpuStep"},				//+0x01a CpuStep       : Uint2B
	{0x01c,4,"ProcessorState"},			//+0x01c ProcessorState : _KPROCESSOR_STATE
	{0x33c,4,"KernelReserved"},			//+0x33c KernelReserved : [16] Uint4B
	{0x37c,4,"HalReserved"},			//+0x37c HalReserved    : [16] Uint4B
	{0x3bc,4,"PrcbPad0"}, 				//+0x3bc PrcbPad0       : [92] UChar
	{0x418,4,"LockQueue"},				//+0x418 LockQueue      : [16] _KSPIN_LOCK_QUEUE
	{0x498,4,"PrcbPad1"}, 				//+0x498 PrcbPad1       : [8] UChar
	{0x4a0,4,"NexThread"},				//+0x4a0 NexThread      : Ptr32 _KTHREAD
	{0x4a4,4,"InterruptCount"},         //+0x4a4 InterruptCount : Uint4B
	{0x4a8,4,"KernelTime"},				//+0x4a8 KernelTime     : Uint4B
	{0x4ac,4,"UserTime"},  				//+0x4ac UserTime       : Uint4B
	{0x4b0,4,"DpcTime"},   				//+0x4b0 DpcTime        : Uint4B
	{0x4b4,4,"DebugDpcTime"}, 			//+0x4b4 DebugDpcTime   : Uint4B
	{0x4b8,4,"InterruptTime"},			//+0x4b8 InterruptTime  : Uint4B
	{0x4bc,4,"AdjustDpcThreshold"}, 	//+0x4bc AdjustDpcThreshold : Uint4B
	{0x4c0,4,"PageColor"},				//+0x4c0 PageColor          : Uint4B
	{0x4c4,4,"SkipTick"}, 				//+0x4c4 SkipTick           : Uint4B
	{0x4c8,4,"MultiThreadSetBusy"}, 	//+0x4c8 MultiThreadSetBusy : UChar
	{0x4c9,4,"Spare2"},    				//+0x4c9 Spare2             : [3] UChar
	{0x4cc,4,"ParentNode"},				//+0x4cc ParentNode         : Ptr32 _KNODE
	{0x4d0,4,"MultiThreadProcessorSet"},//+0x4d0 MultiThreadProcessorSet : Uint4B
	{0x4d4,4,"MultiThreadSetMaster"},   //+0x4d4 MultiThreadSetMaster : Ptr32 _KPRCB
	{0x4d8,4,"ThreadStartCount"},		//+0x4d8 ThreadStartCount : [2] Uint4B
	{0x4e0,4,"CcFastReadNoWait"},		//+0x4e0 CcFastReadNoWait : Uint4B
	{0x4e4,4,"CcFastReadWait"},  		//+0x4e4 CcFastReadWait : Uint4B
	{0x4e8,4,"CcFastReadNotPossible"},  //+0x4e8 CcFastReadNotPossible : Uint4B
	{0x4ec,4,"CcCopyReadNoWait"},		//+0x4ec CcCopyReadNoWait : Uint4B
	{0x4f0,4,"CcCopyReadWait"},  		//+0x4f0 CcCopyReadWait : Uint4B
	{0x4f4,4,"CcCopyReadNoWaitMiss"}, 	//+0x4f4 CcCopyReadNoWaitMiss : Uint4B
	{0x4f8,4,"KeAlignmentFixupCount"},	//+0x4f8 KeAlignmentFixupCount : Uint4B
	{0x4fc,4,"KeContextSwitches"}, 		//+0x4fc KeContextSwitches : Uint4B
	{0x500,4,"KeDcacheFlushCount"},		//+0x500 KeDcacheFlushCount : Uint4B
	{0x504,4,"KeExceptionDispatchCount"},//+0x504 KeExceptionDispatchCount : Uint4B
	{0x508,4,"KeFirstLevelTbFills"},     //+0x508 KeFirstLevelTbFills : Uint4B
	{0x50c,4,"KeFloatingEmulationCount"},//+0x50c KeFloatingEmulationCount : Uint4B
	{0x510,4,"KeIcacheFlushCount"},  	//+0x510 KeIcacheFlushCount : Uint4B
	{0x514,4,"KeSecondLevelTbFills"},	//+0x514 KeSecondLevelTbFills : Uint4B
	{0x518,4,"KeSystemCalls"},			//+0x518 KeSystemCalls : Uint4B
	{0x51c,4,"SpareCounter0"},			//+0x51c SpareCounter0 : [1] Uint4B
	{0x520,4,"PPLookasideList"},        //+0x520 PPLookasideList : [16] _PP_LOOKASIDE_LIST
	{0x5a0,4,"PPNPagedLookasideList"},  //+0x5a0 PPNPagedLookasideList : [32] _PP_LOOKASIDE_LIST
	{0x6a0,4,"PPPagedLookasideList"},   //+0x6a0 PPPagedLookasideList : [32] _PP_LOOKASIDE_LIST
	{0x7a0,4,"PacketBarrier"},			//+0x7a0 PacketBarrier : Uint4B
	{0x7a4,4,"ReverseStall"}, 			//+0x7a4 ReverseStall : Uint4B
	{0x7a8,4,"IpiFrame"},				//+0x7a8 IpiFrame : Ptr32 Void
	{0x7ac,4,"PrcbPad2"},				//+0x7ac PrcbPad2 : [52] UChar
	{0x7e0,4,"CurrentPacket"},			//+0x7e0 CurrentPacket : [3] Ptr32 Void
	{0x7ec,4,"TargetSet"},    			//+0x7ec TargetSet : Uint4B
	{0x7f0,4,"WorkerRoutine"},			//+0x7f0 WorkerRoutine : Ptr32 void
	{0x7f4,4,"IpiFrozen"},				//+0x7f4 IpiFrozen : Uint4B
	{0x7f8,4,"PrcbPad3"}, 				//+0x7f8 PrcbPad3 : [40] UChar
	{0x820,4,"RequestSummary"},         //+0x820 RequestSummary : Uint4B
	{0x824,4,"SignalDone"},				//+0x824 SignalDone : Ptr32 _KPRCB
	{0x828,4,"PrcbPad4"},  				//+0x828 PrcbPad4 : [56] UChar
	{0x860,4,"DpcListHead"},            //+0x860 DpcListHead : _LIST_ENTRY
	{0x868,4,"DpcStack"},				//+0x868 DpcStack : Ptr32 Void
	{0x86c,4,"DpcCount"},				//+0x86c DpcCount : Uint4B
	{0x870,4,"DpcQueueDepth"},        	//+0x870 DpcQueueDepth : Uint4B
	{0x874,4,"DpcRoutineActive"},     	//+0x874 DpcRoutineActive : Uint4B
	{0x878,4,"DpcInterruptRequested"},	//+0x878 DpcInterruptRequested : Uint4B
	{0x87c,4,"DpcLastCount"},  			//+0x87c DpcLastCount : Uint4B
	{0x880,4,"DpcRequestRate"},			//+0x880 DpcRequestRate : Uint4B
	{0x884,4,"MaximumDpcQueueDepth"},   //+0x884 MaximumDpcQueueDepth : Uint4B
	{0x888,4,"MinimumDpcRate"},			//+0x888 MinimumDpcRate : Uint4B
	{0x88c,4,"QuantumEnd"},    			//+0x88c QuantumEnd : Uint4B
	{0x890,4,"PrcbPad5"},				//+0x890 PrcbPad5 : [16] UChar
	{0x8a0,4,"DpcLock"}, 				//+0x8a0 DpcLock : Uint4B
	{0x8a4,4,"PrcbPad6"},				//+0x8a4 PrcbPad6 : [28] UChar
	{0x8c0,4,"CallDpc"}, 				//+0x8c0 CallDpc : _KDPC
	{0x8e0,4,"ChainedInterruptList"},	//+0x8e0 ChainedInterruptList : Ptr32 Void
	{0x8e4,4,"LookasideIrpFloat"},   	//+0x8e4 LookasideIrpFloat : Int4B
	{0x8e8,4,"SpareFields0"}, 			//+0x8e8 SpareFields0 : [6] Uint4B
	{0x900,4,"VendorString"}, 			//+0x900 VendorString : [13] UChar
	{0x90d,4,"InitialApicId"},			//+0x90d InitialApicId : UChar
	{0x90e,4,"LogicalProcessorsPerPhysicalProcessor"},//+0x90e LogicalProcessorsPerPhysicalProcessor : UChar
	{0x910,4,"MHz"},            		//+0x910 MHz : Uint4B
	{0x914,4,"FeatureBits"},    		//+0x914 FeatureBits : Uint4B
	{0x918,4,"UpdateSignature"},		//+0x918 UpdateSignature : _LARGE_INTEGER
	{0x920,4,"NpxSaveArea"},    		//+0x920 NpxSaveArea : _FX_SAVE_AREA
	{0xb30,4,"PowerState"},     		//+0xb30 PowerState : _PROCESSOR_POWER_STATE
	{-1,-1,NULL}
}; 




struct field_def _TIB[]=
{
	{0,4,"ExceptionList"},          //+0x000 ExceptionList : Ptr32 _EXCEPTION_REGISTRATION_RECORD
	{4,4,"StackBase"},              //+0x004 StackBase : Ptr32 Void
	{8,4,"StackLimit"},             //+0x008 StackLimit : Ptr32 Void
	{0xc,4,"SubSystemTib"},         //+0x00c SubSystemTib : Ptr32 Void
	{0x10,4,"FiberData"},           //+0x010 FiberData : Ptr32 Void
	{0x10,4,"Version"},             //+0x010 Version : Uint4B
	{0x14,4,"ArbitraryUserPointer"},//+0x014 ArbitraryUserPointer : Ptr32 Void
	{0x18,4,"Self"},                //+0x018 Self : Ptr32 _NT_TIB
	{-1,-1,NULL}
};

struct field_def _KPCR[]=
{
	{0,0x1c,"NtTib",_TIB},           //+0x000 NtTib            : _NT_TIB
	{0x1c,4,"SelfPcr"},             //+0x01c SelfPcr          : 0xffdff000 _KPCR
	{0x20,4,"Prcb"},                //+0x020 Prcb             : 0xffdff120 _KPRCB
	{0x24,4,"Irql"},                //+0x024 Irql             : 0 ''
	{0x28,4,"IRR"},                 //+0x028 IRR              : 0
	{0x2c,4,"IrrActive"},           //+0x02c IrrActive        : 0
	{0x30,4,"IDR"},                 //+0x030 IDR              : 0xffffffff
	{0x34,4,"KdVersionBlock"},      //+0x034 KdVersionBlock   : 0x805562b8
	{0x38,4,"IDT"},                 //+0x038 IDT              : 0x8003f400 _KIDTENTRY
	{0x3c,4,"GDT"},                 //+0x03c GDT              : 0x8003f000 _KGDTENTRY
	{0x40,4,"TSS"},                 //+0x040 TSS              : 0x80042000 _KTSS
	{0x44,2,"MajorVersion"},        //+0x044 MajorVersion    
	{0x46,2,"MinorVersion"},        //+0x046 MinorVersion 
	{0x48,4,"SetMember"},           //+0x048 SetMember        : 1
	{0x4c,4,"StallScaleFactor"},    //+0x04c StallScaleFactor : 0x6bb
	{0x50,4,"DebugActive"},         //+0x050 DebugActive number Spare0 SecondLevelCacheAssociativity 
	{0x54,4,"VdmAlert"},            //+0x054 VdmAlert         : 0
	{0x58,0x38,"KernelReserved"},    //+0x058 KernelReserved   : [14] 0
	{0x90,4,"SecondLevelCacheS"},   //+0x090 SecondLevelCacheSize : 0x80000
	{0x94,0x40,"HalReserved"},      //+0x094 HalReserved      : [16] 0
	{0xd4,4,"InterruptMode"},       //+0x0d4 InterruptMode    : 0
	{0xd8,4,"Spare1"},              //+0x0d8 Spare1           : 0 ''
	{0xdc,0x44,"KernelReserved2"},     //+0x0dc KernelReserved2  : [17] 0
	{0x120,0,"PrcbData"},           //+0x120 PrcbData         : _KPRCB
	{-1,-1,NULL}
};


void  dump_xp_threads()
{
	//
	// ffdff124:current
	// ffdff128 next:
	//
}
//
// xp/sp3:8088F5A0
//
void  dump_xp_timers()
{
	//
	// ffdff124:current
	// ffdff128 next:
	//
}