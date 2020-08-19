#include <stdio.h>
#include <string.h>
#include "ifetcher.h"

//
//
//
//return the length of prefixes
//      F0:lock
//      F2-repe/F-repne
//      0x66-data size
//      0x67-address size
//
//
//
unsigned int CFetcherBase::fetch_prefix(prefix_t & prefix,bool & illegal)
{
	 prefix_t i;

	 prefix=NO_SEGOVR;
	 illegal=false;

	for(i=0;i<bytes;i++)
	{
		switch( buffer[i])
		{
		case 0xf0:
				if ( prefix& PREFIX_LOCK) illegal=true;
				else prefix|= PREFIX_LOCK;
				break;
		case 0xf2:
				if ( prefix& PREFIX_REPZ) illegal=true;
				else prefix|= PREFIX_REPNZ;
				break;
		case 0xf3:
				if ( prefix& PREFIX_REPNZ) illegal=true;
				else prefix|= PREFIX_REPZ;
				break;
		case 0x26://ES
				if( ( prefix& PREFIX_SEGS)!=NO_SEGOVR) illegal=true;
				else prefix&=0xf8; prefix |= ES;
				break;
		case 0x36://SS
				if( ( prefix& PREFIX_SEGS)!=NO_SEGOVR) illegal=true;
				else prefix&=0xf8; prefix |= SS;
				break;
		case 0x2E://CS
				if( ( prefix& PREFIX_SEGS)!=NO_SEGOVR) illegal=true;
				else prefix&=0xf8; prefix |= CS;
				break;
		case 0x3E://DS
				if( ( prefix& PREFIX_SEGS)!=NO_SEGOVR) illegal=true;
				else prefix&=0xf8; prefix |= DS;
				break;
		case 0x64://FS
				if( ( prefix& PREFIX_SEGS)!=NO_SEGOVR) illegal=true;
				else prefix&=0xf8; prefix |= FS;
				break;
		case 0x65://GS
				if( ( prefix& PREFIX_SEGS)!=NO_SEGOVR) illegal=true;
				else prefix&=0xf8; prefix |= GS;
				break;
		case 0x66://DSIZE
				if ( prefix& PREFIX_DSIZE) ;//illegal=true;
				else prefix|= PREFIX_DSIZE;
				break;
		case 0x67://ASIZE		
				if ( prefix& PREFIX_ASIZE) ;//illegal=true;
				else prefix|= PREFIX_ASIZE;
				break;
		default:
				return i;

		}
	}

	return i;
}



void CFetcherBase::set_fetch_address(bool mode,unsigned int _ip_)
{

	//
	// save pointer
	//
	address16_mode = mode;
	 ip = _ip_;
 

	if (address16_mode)
	{
	ip = _ip_ & 0xffff;
	}

}

//
//  return
//        0: illegal instruction
//        1: success
//        2: end
//
int CFetcherBase::fetch(PINSN_BASIC_INFO p_insn )
{
	unsigned char *code;
	unsigned int  prefix_bytes;
	unsigned int len;
	prefix_t  prefix;
	int data_size;
	int addr_size;
	bool illegal,eof;
	struct instr_rec* p_instr_desc;

	//
	// no matter of the result, fill fetch ip so that caller knows about it.
	//
	p_insn->eip=ip;

	//
	// end of insn stream or mmu fetch fault
	//
	if (!bytes ||!buffer) 
		return FETCH_EOF; // no data
	//
	// prefix
	//
	prefix_bytes=fetch_prefix(prefix,illegal);

	if(illegal) return FETCH_UD;// illegal instruction

	// a complete insn can't be seen
	if (bytes<=prefix_bytes 
		||(buffer[prefix_bytes]==0xf&&bytes<=prefix_bytes+1)) 
		return FETCH_EOF;
	
	code=buffer+prefix_bytes;
	p_instr_desc=get_instr_desc(code,bytes-prefix_bytes,prefix,eof);
	
	if(p_instr_desc==NULL) return eof?FETCH_EOF: FETCH_UD;// illegal instruction

	data_size=(address16_mode ^(0==(prefix&PREFIX_DSIZE)))?32:16;
	addr_size=(address16_mode ^(0==(prefix&PREFIX_ASIZE)))?32:16;

	unsigned int code_len = decode_length(
		code, 
		addr_size, 
		data_size,
		prefix,  // for rex.w?
		p_instr_desc);

	
	len=prefix_bytes+code_len;

	if (bytes<len) 
		return FETCH_EOF;

	p_insn->len=len;
	p_insn->prefix=prefix;
	p_insn->code=buffer;

	p_insn->addr_size=addr_size;
	p_insn->data_size=data_size;
	p_insn->prefix_bytes=prefix_bytes;
	p_insn->p_instr_desc=p_instr_desc;
	ip +=len;
	buffer+=len;
	bytes-=len;

	return FETCH_SUCCESS;

}

//
// defined in project import
//
#define ImmV   0x10      // db dw or dd , checking sext bit
#define Imm    0x20      // dw , don't check sext, add 2 if data size is 32
#define RorM   0x40
#define AddrA  0x80      // add 2 if addr size is 32,e.g.direct mem addressing,
#define AddrD  Imm    // address  depending on data size,e.g. jump ptr16:16/32

#define FIX_BYTES(x) (x&0xf)


 int decode_len_mm_noimm_16 (char*code); // old code

unsigned int CFetcherBase::decode_length(unsigned char  *code,int addr_size,unsigned  int data_size,prefix_t prefix,struct instr_rec* p_instr_desc)
{
	int len;

	if(code[0]==0x0f) code ++;
	len=FIX_BYTES(p_instr_desc->len_decode_info);
	if(p_instr_desc->len_decode_info&RorM)
	{


		len+= (addr_size==16)?decode_len_mm_noimm_16(code+1):
        	  (addr_size==32)?decode_len_mm_noimm_32(code+1): 0;//64 bits?
	}

	if(p_instr_desc->len_decode_info&(Imm|AddrD)
		&& data_size==32)
		len+=2;// two bytes are counted in fix_bytes.

	if((p_instr_desc->len_decode_info&AddrA)
		&& addr_size==32)
		len+=2;// two bytes are counted in fix_bytes.

	if(p_instr_desc->len_decode_info&ImmV)
	{
		// if word  bit  is 2,    //  imul and push imm
		// not set  or sext bit set, byte op
		if(p_instr_desc->w_mask
			&&(((2==p_instr_desc->w_mask)&&0!=(code[0]&2))// if w_mask==2, it is for imm size, only imul and push
		      ||((p_instr_desc->w_mask!=2) && 0==(p_instr_desc->w_mask&code[0]))
		      ||(p_instr_desc->sext_mask&&(p_instr_desc->sext_mask&code[0]))))
			len++;
		// word op, check data size
		else if(data_size==16) len+=2;
		else len+=4;
	}

	return len;
}