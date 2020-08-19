#define  _CRT_SECURE_NO_WARNINGS 1

#include <assert.h>

#include "stdafx.h"
#include "excel.h"

#undef OPER_FMT_REC
#define OPER_FMT_REC(x) x,
//
// If new oper type is to be defined, go to oper_type.h
//
enum id_fmt {
	
#include "oper_fmt_def.h"
N_FMT
};

void		Import_basic(int no);
void		Import_fpu();
void		Import_sse();

void		gen_fpu();
void		gen_code(WCHAR *wcstblname, WCHAR *wcsclassname);
unsigned char	   get_len_decode_sig(int id);
int		   wcs2id(  LPCWSTR wcs_fmt);
void gen_fpu_inc();
void gen_fpu_id();
void gen_fpu_exec();
void gen_sse_inc();
void gen_sse_id();
void gen_sse_exec();
void gen_insn_id(LPCWSTR wcstbl);
void gen_fmt_id(LPCWSTR wcstbl);


unsigned char hex(char c)
{

	if (c>='A') c=(c&0xf)+9; else c&=0xf;
	return c;
}
char to_hex(CString wcs)
{
	char x;
	x=hex((char)wcs[2])<<4;
	x|=hex((char)wcs[3]);
	return x;
}
FILE *fp;

CExcelUtil ExcelUtil;
const _TCHAR* Zero=L"0x00";
const _TCHAR* False=L"false";

FILE *fpc,*fph,*fpc1,*fph1;
//
// gen instr-dat.h
//
int _tmain(int argc, _TCHAR* argv[])
{


	ExcelUtil.OpenEx((argc<2)?L"x86-inst.xls":argv[1]);

	fp = fopen("instr-dat.h","wt");
	if(fp)
	{
		fwprintf(fp,L"#ifndef __INSTR_DAT_H__\n"
			L"#define __INSTR_DAT_H__\n\n"
			L"\n\n"
			L"#include \"common.h\"\n");

		Import_basic(1);
		Import_basic(2);
		Import_fpu();
		Import_sse();

		//fpc1 = fopen("fpu-exec.cpp","wt");
		//fpc = fopen("fpu.cpp","wt");
		//fph = fopen("fpu.h","wt");
		//fph1 = fopen("fpu_id.h","wt");
		//gen_code(L"fpu",L"CFPU");

		//fpc1 = fopen("sse-exec.cpp","wt");
		//fpc = fopen("sse.cpp","wt");
		//fph = fopen("sse.h","wt");
		//fph1 = fopen("sse_id.h","wt");
		//gen_code(L"sse",L"CSSE");

		fwprintf(fp,L"#endif    /*__INSTR_DAT_H__*/");
		fclose(fp);
	}

	gen_fmt_id(L"fpu");
	gen_insn_id(L"fpu");

	//gen_fpu_id();
	gen_fpu_exec();
	gen_fpu_inc();

	gen_fmt_id(L"sse");
	gen_insn_id(L"sse");
	gen_sse_exec();
	gen_sse_inc();
	ExcelUtil.Close();
	return 0;
}


void		Import_basic(int no)
{
	CString sSql;
	assert(fp);

	unsigned char rec_index[256];
	unsigned int rec_no=0,i;

	//
	// init index
	//
	memset (rec_index,0xff,256);


	//fwprintf (fp,L"struct instr_struct_basic basic_%cbyte [] = {\n",L'0'+no);
	fwprintf (fp,L"struct  instr_rec %sinstr_recs [] = {\n",no==2?L"ext_":L"");
	try {

		sSql.Format(L"Select * from [basic_%cbyte$] order by ID1 ASC",L'0'+no);

		ExcelUtil.OpenRecordSet (sSql);
		while (!ExcelUtil.IsEOF())
		{
			CString id1 = ExcelUtil.GetFieldValue(L"ID1"); 
			CString id2 = ExcelUtil.GetFieldValue(L"ID2");
			CString id1_mask = ExcelUtil.GetFieldValue(L"ID1MASK"); 
			CString id2_mask = ExcelUtil.GetFieldValue(L"ID2MASK"); 
			CString w_mask = ExcelUtil.GetFieldValue(L"WMASK");
			CString sext_mask = ExcelUtil.GetFieldValue(L"SMASK");
			CString dir_mask =ExcelUtil. GetFieldValue(L"DMASK");
			CString mnemonics = ExcelUtil.GetFieldValue(L"MNEM");
			CString index = ExcelUtil.GetFieldValue(L"NAME");
			CString fmt = ExcelUtil.GetFieldValue(L"FMT");
			CString funit = ExcelUtil.GetFieldValue(L"FUNIT");
			CString repeatable = no==2?L"":ExcelUtil.GetFieldValue(L"repeatable");
			CString lockable = ExcelUtil.GetFieldValue(L"lockable");
			CString RMConstraint = ExcelUtil.GetFieldValue(L"RMConstraint");
			CString eflags_m = ExcelUtil.GetFieldValue(L"eflags_m");
			CString eflags_t = ExcelUtil.GetFieldValue(L"eflags_t");
			CString opt_prefix=Zero;
			CString  aux_prefix=Zero;
			if(RMConstraint.IsEmpty()) RMConstraint=Zero;
			if(id2_mask.IsEmpty()) id2_mask=Zero;
			if(id2.IsEmpty()) id2=Zero;
			if(repeatable.IsEmpty()) repeatable=False;
			if(lockable.IsEmpty()) lockable=False;

			unsigned  char len_decode_sig=get_len_decode_sig(wcs2id((LPCWSTR)fmt));
			if(len_decode_sig==-1)
				wprintf(L"Unknown format:%s\n",fmt.GetString());


			if(id2_mask!=Zero
				&&(len_decode_sig&0xf0)==0)
			{
				len_decode_sig++;	
			}
			if( no==2) len_decode_sig++;  // include 0x0f
			ExcelUtil.NextRecord();
			fwprintf(fp,L"\t{\"%10s\", %5s, %5s, %5s, %5s, %5s, %5s, %8s, %8s, %8s, %8s,LOCKABLE_%s, %8s , 0x%02x,%8s,%s,%s  }%c\n", 
				mnemonics.GetString(),
				id1.GetString(),
				id1_mask.GetString(),
				id2.GetString(),
				id2_mask.GetString(),
				w_mask.GetString(),
				dir_mask.GetString(),
				sext_mask.GetString(),
				fmt.GetString(),
				index.GetString(),
				funit.GetString(),
				lockable.GetString(),
				repeatable.GetString(),

				len_decode_sig,

				RMConstraint.GetString(),
				eflags_m.GetString(),
				eflags_t.GetString(),

				ExcelUtil.IsEOF() ? L' ':L',');

			unsigned char id,mask;
			id=to_hex(id1);
			mask=to_hex(id1_mask);
			for(i=0;i<0x100;i++)
			{
				if((i&mask)==id)
				{
					if(rec_index[i]==0xff)				
					rec_index[i]=rec_no;
				}
			}


		rec_no++;

		}
	}
	catch(...)
	{
	}
	fwprintf (fp,L"};\n\n");
	//
	// output indexing
	//
	//fwprintf (fp,L"struct instr_struct_basic basic_%cbyte [] = {\n",L'0'+no);
	fwprintf (fp,L"int %sinstr_index [] = {\n",no==2?L"ext_":L"");
	for(i=0;i<0x100;i++)
	{
		fwprintf (fp,L"0x%x%c",rec_index[i],i==0xff?' ':',');
	}
	fwprintf (fp,L"};\n\n");
}


void		Import_fpu()
{
	CString sSql;


	assert(fp);


	fwprintf (fp,L"struct instr_rec x87_instr_recs [] = {\n");

	try {

		sSql.Format(L"Select * from [fpu$] order by ID1 ASC");

		ExcelUtil.OpenRecordSet (sSql);
		while (!ExcelUtil.IsEOF())
		{
			CString id1 = ExcelUtil.GetFieldValue(L"ID1"); 
			CString id2 = ExcelUtil.GetFieldValue(L"ID2");
			CString id1_mask = ExcelUtil.GetFieldValue(L"ID1MASK"); 
			CString id2_mask = ExcelUtil.GetFieldValue(L"ID2MASK"); 
			CString mnemonics = ExcelUtil.GetFieldValue(L"MNEM");
			CString fmt = ExcelUtil.GetFieldValue(L"FMT");
			CString  opt_prefix=Zero;
			CString  aux_prefix=Zero;

			if(id2_mask.IsEmpty()) id2_mask=Zero;
			if(id2.IsEmpty()) id2=Zero;
			
			 int fmt_id=wcs2id(fmt);
			unsigned char len_decode_sig=get_len_decode_sig(fmt_id);
			if(len_decode_sig==-1)
				wprintf(L"Unknown format:%s\n",fmt.GetString());

			CString RMConstraint=Zero;
			if (fmt_id==mem2i
				||fmt_id==mem4i
				||fmt_id==mem8i
				||fmt_id==mem10d
				||fmt_id==mem4r
				||fmt_id==mem8r
				||fmt_id==mem10r
				||fmt_id==mem108
				//||fmt_id==mem512
				||fmt_id==mem14
				)
				RMConstraint=L"Monly";
			//
			// insns with two byte id,but no r/m byte to be counted in length decode
			//
			if(id2_mask!=Zero
				&&(len_decode_sig&0xf0)==0)
			{
				len_decode_sig++;	
			}

			ExcelUtil.NextRecord();

			fwprintf(fp,L"\t{ \"%10s\", %5s, %5s, %5s, %5s, 0x00, 0x00, 0x00, %8s, %8s,fpu, LOCKABLE_NEVER,false,0x%02x, %8s   }%c\n", 
				mnemonics.GetString(),
				id1.GetString(),
				id1_mask.GetString(),
				id2.GetString(),
				id2_mask.GetString(),
				fmt.GetString(),
				mnemonics.GetString(),
				len_decode_sig,
				RMConstraint.GetString(),
				ExcelUtil.IsEOF() ? L' ':L',');
		}
	}
	catch(...)
	{
	}
	fwprintf (fp,L"};\n\n");
}

void		Import_sse()
{
	CString sSql;


	assert(fp);
	unsigned char rec_index[256];
	int rec_no=0,i;

	//
	// init index
	//
	memset (rec_index,0xff,256);
	fwprintf (fp,L"struct instr_rec sse_instr_recs [] = {\n");

	try {

		sSql.Format(L"Select * from [sse$] order by ID1 ASC");

		ExcelUtil.OpenRecordSet (sSql);
		while (!ExcelUtil.IsEOF())
		{
			CString id1 = ExcelUtil.GetFieldValue(L"ID1"); 
			CString id2 = ExcelUtil.GetFieldValue(L"ID2");
			CString id1_mask = ExcelUtil.GetFieldValue(L"ID1MASK"); 
			CString id2_mask = ExcelUtil.GetFieldValue(L"ID2MASK"); 
			CString aux_prefix = ExcelUtil.GetFieldValue(L"aux_prefix");
			CString opt_prefix = ExcelUtil.GetFieldValue(L"opt_prefix");
			CString dir_mask =ExcelUtil. GetFieldValue(L"DMASK");
			CString mnemonics = ExcelUtil.GetFieldValue(L"MNEM");
			//CString index = ExcelUtil.GetFieldValue(L"NAME");
			CString fmt = ExcelUtil.GetFieldValue(L"FMT");
			CString RMConstraint = ExcelUtil.GetFieldValue(L"RMConstraint");
			if(opt_prefix.IsEmpty())   opt_prefix=Zero;
			if(aux_prefix.IsEmpty())   aux_prefix=Zero;
			if(RMConstraint.IsEmpty()) RMConstraint=Zero;
			if(id2.IsEmpty()) id2=Zero;
			if(id2_mask.IsEmpty()) id2_mask=Zero;
			if(dir_mask.IsEmpty()) dir_mask=Zero;

			unsigned char len_decode_sig=get_len_decode_sig(wcs2id((LPCWSTR)fmt));
			if(len_decode_sig==-1)
			wprintf(L"Unknown format:%s\n",fmt.GetString());
			if(id2_mask!=Zero
				&&(len_decode_sig&0xf0)==0)
			{
				len_decode_sig++;	
			}
			// count in 0f
			len_decode_sig++;	
			ExcelUtil.NextRecord();
			fwprintf(fp,L"\t{\"%10s\", %5s, %5s, %5s,%5s,0x00, %5s, 0x00,%8s,  %8s, sse, LOCKABLE_NEVER,false,0x%02x, %8s,0,0,%8s ,%8s  }%c\n", 
				mnemonics.GetString(),
				id1.GetString(),
				id1_mask.GetString(),
				id2.GetString(),
				id2_mask.GetString(),
				dir_mask.GetString(),
				fmt.GetString(),
				mnemonics.GetString(),//index,
				len_decode_sig,
				RMConstraint.GetString(),
				aux_prefix.GetString(),
				opt_prefix.GetString(),

				ExcelUtil.IsEOF() ? L' ':L',');

			unsigned char id,mask;
			id=to_hex(id1);
			mask=to_hex(id1_mask);
			for(i=0;i<0x100;i++)
			{
				if((i&mask)==id)
				{
					if(rec_index[i]==0xff)				
					rec_index[i]=rec_no;
				}
			}


			rec_no++;
		}
	}
	catch(...)
	{
	}
	fwprintf (fp,L"};\n\n");
	//
	// output indexing
	//
	//fwprintf (fp,L"struct instr_struct_basic basic_%cbyte [] = {\n",L'0'+no);
	fwprintf (fp,L"int sse_instr_index [] = {\n");
	for(i=0;i<0x100;i++)
	{
		fwprintf (fp,L"0x%x%c",rec_index[i],i==0xff?' ':',');
	}
	fwprintf (fp,L"};\n\n");
}

void		gen_code(WCHAR *wcstblname, WCHAR *wcsclassname)
{
	CString sSql,sLast=Zero;

	//fwprintf (fph,L"class %s {\npublic:\n",wcsclassname);
	fwprintf (fpc,
L"#include <stdio.h>\n"
L"#include \"cpu.h\"\n\n" );

	fwprintf (fpc1,
L"#include <stdio.h>\n"
L"#include \"cpu.h\"\n\n"
L"void CCPU:: %s_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)\n"
L"{\n"
L"\tswitch(p_insn->p_instr_desc->index)\n"
L"\t{\n",wcstblname);


	try {

		sSql.Format(L"Select MNEM from [%s$] order by MNEM",wcstblname);

		ExcelUtil.OpenRecordSet (sSql);
		while (!ExcelUtil.IsEOF())
		{
			CString index = ExcelUtil.GetFieldValue(L"MNEM");

			ExcelUtil.NextRecord();

			if( index!= sLast)
			{
			fwprintf(fph,L"void %s_(PINSN_EXEC_INFO p_insn_exec_info);\n",index.GetString());
			fwprintf(fpc,L"void CCPU::%s_(PINSN_EXEC_INFO p_insn_exec_info)\n{\n\tassert(0);\n}\n",index.GetString());
			fwprintf(fpc1,L"\tcase %s:\n\t %s_(p_insn_exec_info);\n\tbreak;\n",index.GetString(),index.GetString());
			fwprintf(fph1,L"	INSTR_REC( %s )\n",index.GetString());
			sLast=index;
			}
		}
	}
	catch(...)
	{
	}
	//fwprintf(fph,L"\n};\n");
	fwprintf(fpc1,L"\n\t}\n}\n");

	fclose (fph);
	fclose (fph1);
	fclose (fpc);
	fclose (fpc1);
}
#define CF 1
#define AF 2
#define PF 4
#define ZF 8
#define SF 16
#define OF 32

unsigned char flags_str_to_hex(const char * str)
{
	unsigned char flag = 0;
	while (*str)
	{
		switch (*str)
		{
		case 'o': case 'O':flag |= OF;break;
		case 's': case 'S':flag |= SF;break;
		case 'z': case 'Z':flag |= ZF;break;
		case 'p': case 'P':flag |= PF;break;
		case 'a': case 'A':flag |= AF;break;
		case 'c': case 'C':flag |= CF;break;
		}
	}
	return flag;
}

void flags_bin_to_str( unsigned char flag, char str[7])
{
	str[6]=0;
	str[5] = (flag&CF) ? 'c' :'-';
	str[4] = (flag&AF) ? 'a' :'-';
	str[3] = (flag&PF) ? 'p' :'-';
	str[2] = (flag&ZF) ? 'z' :'-';
	str[1] = (flag&SF) ? 's' :'-';
	str[0] = (flag&OF) ? 'o' :'-';
}