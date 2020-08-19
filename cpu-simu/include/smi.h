/*
Added to SMBASE + 8000H) Register Writable?
7FFCH CR0 No
7FF8H CR3 No
7FF4H EFLAGS Yes
7FF0H EIP Yes
7FECH EDI Yes
7FE8H ESI Yes
7FE4H EBP Yes
7FE0H ESP Yes
7FDCH EBX Yes
7FD8H EDX Yes
7FD4H ECX Yes
7FD0H EAX Yes
7FCCH DR6 No
7FC8H DR7 No
7FC4H TR* No
7FC0H LDT Base* No
7FBCH GS* No
7FB8H FS* No
7FB4H DS* No
7FB0H SS* No
7FACH CS* No
7FA8H ES* No
7FA7H - 7F04H Reserved No
7F02H Auto HALT Restart Field (Word) Yes
7F00H I/O Instruction Restart Field (Word) Yes
7EFCH SMM Revision Identifier Field (Doubleword) No
7EF8H SMBASE Field (Doubleword) Yes
*/

typedef struct _SMRAM_State_Save_Map
{
BYTE reserved7e00_7ef7[0xf8];  
DWORD smbase;
DWORD smm_revision_id;
WORD  io_restart;
WORD  hlt_restart;
BYTE reserved7f04_7fa8[0xa8-0x04];
DWORD es;
DWORD cs;
DWORD ss;
DWORD ds;
DWORD fs;
DWORD gs;
DWORD ldt;
DWORD tr;
DWORD dr7;
DWORD dr6;
DWORD eax;
DWORD ecx;
DWORD edx;
DWORD ebx;
DWORD esp;
DWORD ebp;
DWORD esi;
DWORD edi;
DWORD eip;
DWORD eflags;
DWORD cr3;
DWORD cr0;
}SMM_STATE_SAVE_MAP;