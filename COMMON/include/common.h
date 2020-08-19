#ifndef __COMMON_H__
#define __COMMON_H__

#include "instr_db.h"

#include "uop.h"


enum REG8  { AL,    CL,    DL,    BL,    AH,    CH,    DH,    BH,R8_NONE};
enum REG16 { AX,    CX,    DX,    BX,    SP,    BP,    SI,    DI,R16_NONE};
enum REG32 { EAX,   ECX,   EDX,   EBX,   ESP,   EBP,   ESI,   EDI,R32_NONE};
typedef enum e_sreg   { ES,CS,SS,DS,FS,GS,NO_SEGOVR=7,DEF_SEG=NO_SEGOVR,MAX_SEG=NO_SEGOVR}SEGREG;// usedin overriden segment macro PREFIX_SEG

typedef short  OFFSET16;
typedef long   OFFSET32;
typedef unsigned int address_t;

int  mm_len16( char* mmrm_byte);
int  mm_len32( char* mmrm_byte);
int is_stack16(unsigned char  *code);
int is_stack32(unsigned char  *code);
int Mv_display32( unsigned char  *code, char* buffer);
int Mv_display16( unsigned char  *code, char* buffer);

 int decode_len_mm_imm_16_16 (unsigned char  *code);
 int decode_len_mm_imm_16_16x(unsigned char  *code);
 int decode_len_mm_imm_32_16 (unsigned char  *code);
 int decode_len_mm_imm_32_16x(unsigned char  *code);
 int decode_len_mm_imm_16_32 (unsigned char  *code);
 int decode_len_mm_imm_16_32x(unsigned char  *code);
 int decode_len_mm_imm_32_32 (unsigned char  *code);
 int decode_len_mm_imm_32_32x(unsigned char  *code);
 int decode_len_mm_noimm_32 (unsigned char  *code);
 int decode_len_mm_noimm_32x(unsigned char  *code);
 int decode_len_mm_imm8_32 (unsigned char  *code);
 int decode_len_mm_imm8_32x(unsigned char  *code);

 int decode_len_mm_noimm_16 (unsigned char  *code);
 int decode_len_mm_noimm_16x(unsigned char  *code);
 int decode_len_mm_imm8_16 (unsigned char  *code);
 int decode_len_mm_imm8_16x(unsigned char  *code);


// prefix 
//
//

#define PREFIX_REXR  0x200
#define PREFIX_REXW  0x100


#define PREFIX_REPZ  0x80
#define PREFIX_LOCK  0x40
#define PREFIX_REPNZ 0x20
#define PREFIX_DSIZE 0x10
#define PREFIX_ASIZE 0x8
#define PREFIX_SEGS  0x7
#define PREFIX_SEG(a) ( a&7)


#define PREFIX_F2  PREFIX_REPNZ
#define PREFIX_F3  PREFIX_REPZ
#define PREFIX_66  PREFIX_DSIZE
#define PREFIX_67  PREFIX_ASIZE

#define PREFIX_RREX (PREFIX_REXW|PREFIX_REXR)
#define PREFIX_REPZNZ (PREFIX_REPNZ|PREFIX_REPZ)
#define PREFIX_REP(a) ( a&PREFIX_REPZNZ)

typedef unsigned short prefix_t;

__inline bool TEST_PREFIX(prefix_t prefix,unsigned long mask)
{
  return  (prefix &mask)!=0;
}



#define TEST(a,b) (a&b)
//
// for disassemblydisplay
//
//
int display_operand(
	unsigned char  *code,  // not including 0x0f if two bytes opcode instructions
	int next_ip,
	enum OP_FMT fmt,
	char * buffer,
	int  buf_len,
	int address_size,
	int data_size,
	prefix_t prefix,
	int flag );


int display_Mv(
	unsigned char  *code, 
	char * buffer,
	int address_size,
	int data_size_num,
	int override_seg );

int display_Ev(
	unsigned char  *code, 
	char * buffer,
	int address_size,
	int data_size,
	bool word,
	int override_seg,
	bool disp_dsize=false );

int display_Rv(
	int reg,
	char * buffer,
	int data_size,
	bool word);



int display_EvGv(
	unsigned char  *code, 
	char * buffer,
	int address_size,
	int data_size,
	bool word,
	bool dir,
	int override_seg );

struct instr_rec;

int display_line(	
	unsigned char * code,
	int next_ip,
	char * buffer,
	prefix_t prefix,
	int data_size,
	int addr_size,
	 struct instr_rec* instr_desc);
//
// SSE display	 
//
int display_RorM_sse(
	unsigned char  *code, 
	char * buffer,
	int address_size,
	int override_seg,
	bool xmm);

int display_R_sse(
	int reg,
	char * buffer,
	bool xmm);

int display_Ex(
	unsigned char  *code, 
	char * buffer,
	int address_size,
	bool xmm,
	int override_seg );

int display_R_RorM_sse(
	unsigned char  *code, 
	char * buffer,
	int address_size,
	bool dir,
	bool xmm,
	int override_seg );



 //
 // for decoding
 //
 typedef struct reg_code
 {
	 unsigned char reg_cat:4; 
	 unsigned char reg_no:4;
 }  REG_CODE;

 __inline bool reg_eBu( REG_CODE reg_code1,REG_CODE reg_code2)
{
	return reg_code1.reg_cat==reg_code2.reg_cat&&reg_code1.reg_no==reg_code2.reg_no;
}
 //
 // register category
 //
//#undef RC_NONE

#define RC_REGG8 0
#define RC_REGG16 1
#define RC_REGG32 2
#define RC_REGSEG 3
#define RC_REGCR 4
#define RC_REGDR 5
#define RC_REGTR 6
#define RC_REGFP 7
#define RC_REGMMX 8
#define RC_REGXMM 9
#define RC_REGG64 10
#define RC_REGG128 11
#define RC_NOTHING 15


typedef enum _address_fmt
{
	AT_DIRECT,// 0
	AT_BASE  ,// 1
	AT_INDEX ,// 2
	AT_FULL =AT_BASE|AT_INDEX //3
} ADDRESS_FORMAT;

 typedef struct mem_desc
 {
	unsigned char scale:2;
	unsigned char seg:3;
	unsigned char fmt:2; // ADDRESS_FORMAT:no base, no index,direct addressing
	unsigned char base_reg_no;
	unsigned char index_reg_no;
	unsigned long  disp;
 } MEM_DESC,*PMEM_DESC;

 
#define OT_REG 3
#define OT_MEM 2
#define OT_NONE 0
 typedef struct operand_r_m_i
 {
	unsigned char type:2;  // reg or mem, /rm present ? a sub set of struct _op_type_flag  ;
	union operand_desc{
		REG_CODE reg;
		MEM_DESC mem;
	} operand_desc;
 }OPERAND_R_M,*POPERAND_R_M;

 //
 // indicates existence of fields in insn_exec_info
 //
struct _op_type_flag { 
 unsigned char rm_type:1;   // type rm
 unsigned char rm_present:1;//existence of rm
 unsigned char imm_present:1;
 unsigned char s_imm_present:1; // secondary imm for seg addr in jmp/call ptr16:16, imm8 in enter imm16:imm8
 unsigned char reg_present:1;
 unsigned char s_reg_present:1;


};
	struct _secondary_imm {
		struct _op_type_flag type;//eBual to r_m.type
		char padding;
		unsigned long imm;  // for imm8, imm16, mask off high order bits.
         };		
	struct _secondry_reg {
		struct _op_type_flag type;//eBual to r_m.type
		char padding;
		REG_CODE reg; // for in al,dx, out dx,al
	} ;
//
//  grneral formats
// 1)imul r,r/m,imm
// 2) r,r/m
// 3) r/m,imm
// 4) r,imm
 typedef struct insn_exec_info
{
	//
	// union-ed so that secondary_imm.type is  eBual to  type and r_m.type
	//
	union 
	{
		OPERAND_R_M r_m;// union-ed so that r_m.type is  be eBual to  type
		struct _op_type_flag type;//eBual to r_m.type
		unsigned short  op_presence;
		struct _secondry_reg secondary_reg; 
                struct _secondary_imm  secondary_imm;
	};
	REG_CODE reg;
	unsigned long imm;  // for imm8, imm16, mask off high order bits.
	char  flag;   // dir,sext,word
//	struct instr_rec *p_instr_desc;
}  INSN_EXEC_INFO,*PINSN_EXEC_INFO;
//
// insn_exec_info defines three flags:
//
#define EXEC_F_DIR 1
#define EXEC_F_WORD 2
#define EXEC_F_SEXT 4
//
// flags for operand presence
//
#define  OP_PRESENCE_RM 2
#define  OP_PRESENCE_RM_R 3
#define  OP_PRESENCE_IMM 4
#define  OP_PRESENCE_IMM_SEC 8
#define  OP_PRESENCE_REG 0x10
#define  OP_PRESENCE_REG_SEC 0x20

#pragma pack(push,1)
typedef struct _insn_basic_info
{
	unsigned int eip;
	unsigned char * code;
	struct instr_rec *p_instr_desc;
	prefix_t  prefix;
	unsigned char addr_size;
	unsigned char data_size;
	unsigned char  len;
	unsigned char  prefix_bytes;

}INSN_BASIC_INFO,*PINSN_BASIC_INFO;

#pragma pack(pop)
//
// address gen uop_addrg
//
void    uop_addrg_addr16 (
	unsigned char * code,
	UOP &uop, // uop_addrg to gen address
	RoV & r_v,      //direct address or tmp reg number
	bool & stack,
	int & imm);

//
// address gen uop_addrg
//
void    uop_addrg_addr32 (
	unsigned char * code,
	UOP &uop, // uop_addrg to gen address
	RoV & r_v,      //direct address or tmp reg number
	bool & stack,
	int & imm);

 int decode_length(int addr_size, unsigned int data_size,unsigned char  *code,bool pmode,bool &illegal,char prefix);

struct instr_rec * get_instr_desc( unsigned char * opcode,unsigned  int bytes,prefix_t prefix,bool & eof);
void predecode_insn(PINSN_BASIC_INFO insn,PINSN_EXEC_INFO p_insn_exec_info);
//int disasm_line(char line[100],unsigned int ip,
//				unsigned char *buffer,int prefix_bytes,int len,prefix_t prefix,
//				int data_size,int addr_size);

int disasm_line(char line[100],PINSN_BASIC_INFO p_insn);

void  decode(int &n_uop,int addr_size, int data_size ,unsigned char * code,bool pmode,bool &illegal,prefix_t prefix,UOP uop[5],unsigned int next_ip);
char decode_flag(unsigned char *code,struct instr_rec *p_instr_desc);

void exec_uop(int n_uop,UOP uop[]);
void branch_exec(UOP uop);
void alu_exec(UOP uop);
void mrom_exec(UOP uop);

int sys_init();
int sys_run();


#endif
