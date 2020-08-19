#define  _CRT_SECURE_NO_WARNINGS 1

#include <assert.h>
#include "stdafx.h"
#include "string.h"
#include "excel.h"
int		   wcs2id(  LPCWSTR wcs_fmt);

extern CExcelUtil ExcelUtil;

#undef OPER_FMT_REC
#define OPER_FMT_REC(x) x,
//
// If new oper type is to be defined, go to oper_type.h
//
enum id_fmt {
	
#include "oper_fmt_def.h"
N_FMT
};
void gen_fpu_id()
{
	CString sLast,sSql;
	FILE *fp = fopen("fpu_id.h","wt");

	try {

		sSql.Format(L"Select MNEM from [fpu$] order by MNEM");

		ExcelUtil.OpenRecordSet (sSql);
		while (!ExcelUtil.IsEOF())
		{
			CString name = ExcelUtil.GetFieldValue(L"MNEM");
			ExcelUtil.NextRecord();

			if( name!= sLast)
			{
			fwprintf(fp,L"	INSTR_REC( %s )\n",(LPCWSTR)name	);
			sLast=name;
			}
		}
	}
	catch(...)
	{
	}

	fclose (fp);
}

void gen_fpu_exec()
{
	FILE * fp = fopen("fpu-exec.cpp","wt");

	CString sLast,sSql;

	fwprintf (fp,
L"#include <stdio.h>\n"
L"#include \"condition.h\"\n"
L"#include \"cpu.h\"\n\n"
L"void CCPU:: fpu_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)\n"
L"{\n"
L"\tunion {unsigned long data;char buffer[512];};\n"
L"\tREG_CODE reg;\n"
L"\tunsigned short cflags;\n\n"
L"\tfpu87.save_opcode(p_insn);\n"
L"\tswitch(p_insn->p_instr_desc->index)\n"
L"\t{\n");


	try {

		sSql.Format(L"Select MNEM ,FMT,IO from [fpu$] order by MNEM");

		ExcelUtil.OpenRecordSet (sSql);
		while (!ExcelUtil.IsEOF())
		{
			CString index = ExcelUtil.GetFieldValue(L"MNEM");
			CString fmt = ExcelUtil.GetFieldValue(L"FMT");
			CString io = ExcelUtil.GetFieldValue(L"IO");
			int i= wcs2id(fmt);
			ExcelUtil.NextRecord();

			if( index!= sLast)
			{
				if(!sLast.IsEmpty())
				fwprintf(fp,L"\t}\n\tbreak;\n"	);
				
				fwprintf(fp,L"\tcase %s:\n",index.GetString());

				//
				//FCMOVNU,FCMOVU,FCMOVNE,FCMOVE,FCMOVNBE,FCMOVBE,FCMOVNB,FCMOVB
				//
				if((index[1])==L'C'&&(index[2])==L'M')
				{
					LPCWSTR wcs_cond;
					if(wcscmp((LPCWSTR)index+5,L"NU")==0)
						wcs_cond=L"PE";
					else if(wcscmp((LPCWSTR)index+5,L"U")==0)
						wcs_cond=L"PO";
					else if(wcscmp((LPCWSTR)index+5,L"NBE")==0)
						wcs_cond=L"G";
					else if(wcscmp((LPCWSTR)index+5,L"BE")==0)
						wcs_cond=L"NG";
					else /*if(wcscmp((LPCWSTR)index,L"NE")==0)
						wcs_cond=L"NE";
					else if(wcscmp((LPCWSTR)index,L"E")==0)
						wcs_cond=L"E";
					else if(wcscmp((LPCWSTR)index,L"NB")==0)
						wcs_cond=L"NB";
					else if(wcscmp((LPCWSTR)index,L"B")==0)
						wcs_cond=L"B";*/
						wcs_cond=(LPCWSTR)index+5;

					fwprintf(fp,L"\tcflags=__CFLAGS__;\n"
						L"\tif((eval_cond(cond_%s,cflags))) {\n\t\tfpu87.FLD_STi(p_insn->code[p_insn->prefix_bytes+1]&7);\n",wcs_cond);
					goto l_next;
				}

				fwprintf(fp,L"\tswitch (p_insn->p_instr_desc->op_fmt)\n\t{\n"	);
			}	
			fwprintf(fp,L"\tcase %s:\n",fmt.GetString());
			switch(i)
			{
			case fmt_AX:
				fwprintf(fp,L"\treg.reg_cat=RC_REGG16;\n\treg.reg_no=0;\n");

				if(io[0]=='O')
				{
				fwprintf(fp,L"\tdata=fpu87.%s_%s();\n\tupdate_reg(reg,data);\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				}
				else
				{
				fwprintf(fp,L"\tdata=reg_file.read_reg((reg);\n\t\tfpu87.%s_%s(data);\n\tupdate_reg(reg,data);\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				}
			break;

			case mem2i:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\tdata=fpu87.%s_%s();\n\n",index.GetString(),fmt.GetString()	);
				fwprintf(fp,
				L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tfpu87.%s_%s((unsigned short)data);\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				}
			break;

			case mem4i:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\tdata=fpu87.%s_%s();\n\n",index.GetString(),fmt.GetString()	);
				fwprintf(fp,
				L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tfpu87.%s_%s(data);\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				}
			break;
			case mem8i:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\t\tfpu87.%s_%s((unsigned long *)buffer);\n\n",index.GetString(),fmt.GetString()	);
				fwprintf(fp,
				L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tfpu87.%s_%s((unsigned long *)buffer);\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				}
			break;
			case mem14:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\t\tfpu87.%s_%s(buffer);\n\n",index.GetString(),fmt.GetString()	);
				fwprintf(fp,
				L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,14,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,14,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tfpu87.%s_%s(buffer);\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				}
			break;
			case mem108:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\t\tfpu87.%s_%s(p_insn->data_size,buffer);\n\n",index.GetString(),fmt.GetString()	);
				fwprintf(fp,
					L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tfpu87.%s_%s(p_insn->data_size,buffer);\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				}
			break;
			/*
			case mem512:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\t\tfpu87.%s_%s(p_insn->data_size,buffer);\n\n",index.GetString(),fmt.GetString()	);
				fwprintf(fp,
					L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,512,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,512,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tfpu87.%s_%s(p_insn->data_size,buffer);\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				}
			break;
			*/
			case mem4r:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\t\tfpu87.%s_%s(buffer);\n\n",index.GetString(),fmt.GetString()	);
				fwprintf(fp,
					L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tfpu87.%s_%s(buffer);\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				}
			break;
			case mem8r:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\t\tfpu87.%s_%s(buffer);\n\n",index.GetString(),fmt.GetString()	);
				fwprintf(fp,
					L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tfpu87.%s_%s(buffer);\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				}
			break;
			case mem10r:
			case mem10d:
				if(io[0]=='O')
				{
				fwprintf(fp,L"\t\tfpu87.%s_%s(buffer);\n\n",index.GetString(),fmt.GetString()	);
				fwprintf(fp,
					L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tfpu87.%s_%s(buffer);\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				}
			break;
			case ST0i:
				if(io[0]=='C')
				fwprintf(fp,L"\t\tcflags=__CFLAGS__;\n\t\tfpu87.%s_%s(cflags,p_insn->code[p_insn->prefix_bytes+1]&7);\n\t\t__CFLAGS__=cflags;\n\n\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				else
				fwprintf(fp,L"\t\tfpu87.%s_%s(p_insn->code[p_insn->prefix_bytes+1]&7);\n\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				break;
			case STi0:	
			case STi:
				fwprintf(fp,L"\t\tfpu87.%s_%s(p_insn->code[p_insn->prefix_bytes+1]&7);\n\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);
				break;
			case fmt_none:
				fwprintf(fp,L"\t\tfpu87.%s_();\n\n\t\tbreak;\n",index.GetString());
				break;
			default:
				fwprintf(fp,L"\t\tfpu87.%s_%s();\n\n\t\tbreak;\n",index.GetString(),fmt.GetString()	);

				break;
			}
l_next:
			sLast=index;
			
		}
	}
	catch(...)
	{
	}
	fwprintf(fp,L"\n\t}\n\t}\n}\n");

	fclose (fp);
}

void gen_fpu_inc()
{
	FILE * fp = fopen("fpu-inc.h","wt");

	CString sLast,sSql;



	try {

		sSql.Format(L"Select MNEM ,FMT,IO from [fpu$] order by MNEM");

		ExcelUtil.OpenRecordSet (sSql);
		while (!ExcelUtil.IsEOF())
		{
			CString index = ExcelUtil.GetFieldValue(L"MNEM");
			CString fmt = ExcelUtil.GetFieldValue(L"FMT");
			CString io = ExcelUtil.GetFieldValue(L"IO");
			int i= wcs2id(fmt);
			ExcelUtil.NextRecord();

			switch(i)
			{
			case fmt_AX:

				if(io[0]=='O')
				{
				fwprintf(fp,L"unsigned short %s_%s();\n",index.GetString(),fmt.GetString()	);
				}
				else
				{
				fwprintf(fp,L"void %s_%s(unsigned short data);\n",index.GetString(),fmt.GetString()	);
				}
			break;

			case mem2i:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"unsigned short %s_%s();\n",index.GetString(),fmt.GetString()	);
				}
				else
				{
				fwprintf(fp,L"void %s_%s(unsigned short data);\n",index.GetString(),fmt.GetString()	);
				}
			break;

			case mem4i:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"unsigned long %s_%s();\n\n",index.GetString(),fmt.GetString()	);
				}
				else
				{
				fwprintf(fp,L"void %s_%s( unsigned long  data);\n",index.GetString(),fmt.GetString()	);
				}
			break;
			case mem8i:
				fwprintf(fp,L"void %s_%s( unsigned long * data);\n",index.GetString(),fmt.GetString()	);
				
			break;
			case mem14:
			case mem4r:
			case mem8r:
			case mem10r:
			case mem10d:
				fwprintf(fp,L"void %s_%s(char * buffer);\n\n",index.GetString(),fmt.GetString()	);
			break;
			case mem108:
	
			//case mem512:
				fwprintf(fp,L"void %s_%s(int OperandSize,char * buffer);\n\n",index.GetString(),fmt.GetString()	);
			break;
			
			case ST0i:
				if(io[0]=='C')
				fwprintf(fp,L"void %s_%s(unsigned short &cflags,int i);\n\n",index.GetString(),fmt.GetString()	);
				else
				fwprintf(fp,L"void %s_%s(int i);\n\n",index.GetString(),fmt.GetString()	);
				break;
			case STi0:	
			case STi:
				fwprintf(fp,L"void %s_%s(int i);\n\n",index.GetString(),fmt.GetString()	);
				break;
			case fmt_none:
				fwprintf(fp,L"void %s_();\n\n",index.GetString());
				break;
			default:
				fwprintf(fp,L"void %s_%s();\n\n",index.GetString(),fmt.GetString()	);
			break;
			}

			sLast=index;
			
		}
	}
	catch(...)
	{
	}

	fclose (fp);
}