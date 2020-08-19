#define  _CRT_SECURE_NO_WARNINGS 1

#include <assert.h>
#include "stdafx.h"
#include "string.h"
#include "excel.h"
int		   wcs2id(  LPCWSTR wcs_fmt);
#undef OPER_FMT_REC
#define OPER_FMT_REC(x) x,
//
// If new oper type is to be defined, go to oper_type.h
//
enum id_fmt {
	
#include "oper_fmt_def.h"
N_FMT
};
extern CExcelUtil ExcelUtil;

void gen_sse_id()
{
	CString sLast,sSql;
	FILE *fp = fopen("sse_id.h","wt");

	try {

		sSql.Format(L"Select MNEM from [sse$] order by MNEM");

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

void gen_sse_exec()
{
	FILE * fp = fopen("sse-exec.cpp","wt");

	CString sLast,sSql;
	int n=32;

	fwprintf (fp,
L"#include <stdio.h>\n"
L"#include \"condition.h\"\n"
L"#include \"cpu.h\"\n\n"
L"void CCPU:: fpu_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)\n"
L"{\n"
L"\tunion {unsigned long data;char buffer[128];unsigned long l[4];};\n"
L"\tREG_CODE reg;\n"
L"\tunsigned short cflags;\n\n"
L"\tswitch(p_insn->p_instr_desc->index)\n"
L"\t{\n");


	try {

		sSql.Format(L"Select MNEM ,FMT,IO from [sse$] order by MNEM");

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

				fwprintf(fp,L"\tswitch (p_insn->p_instr_desc->op_fmt)\n\t{\n"	);
			}	
			fwprintf(fp,L"\tcase %s:\n",fmt.GetString());
			switch(i)
			{
			case xmm_xm64Ib:
				fwprintf(fp,L"\treg.reg_cat=RC_REGG16;\n\treg.reg_no=0;\n");

				if(io[0]=='O')
				{
				fwprintf(fp,L"\tdata=sse_core.%s_%s();\n\tupdate_reg(reg,data);\n\t\tbreak;\n",index.GetString(),fmt.GetString());
				}
				else
				{
				fwprintf(fp,L"\tdata=reg_file.read_reg((reg);\n\t\tsse_core.%s_%s(data);\n\tupdate_reg(reg,data);\n\t\tbreak;\n",index.GetString(),fmt.GetString());
				}
			break;

			case xmm_xm128Ib:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\tdata=sse_core.%s_%s();\n\n",index.GetString(),fmt.GetString());
				fwprintf(fp,
				L"\t\tif( !write_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tsse_core.%s_%s((unsigned short)data);\n\t\tbreak;\n",index.GetString(),fmt.GetString());
				}
			break;

			case xmm_xm32:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\tdata=sse_core.%s_%s();\n\n",index.GetString(),fmt.GetString());
				fwprintf(fp,
				L"\t\tif( !write_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tsse_core.%s_%s(data);\n\t\tbreak;\n",index.GetString(),fmt.GetString());
				}
			break;
			case xmm_xm64:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\t\tsse_core.%s_%s(buffer);\n\n",index.GetString(),fmt.GetString());
				fwprintf(fp,
				L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tsse_core.%s_%s(buffer);\n\t\tbreak;\n",index.GetString(),fmt.GetString());
				}
			break;
			case xmm_xm128:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\t\tsse_core.%s_%s(p_insn->data_size,buffer);\n\n",index.GetString(),fmt.GetString());
				fwprintf(fp,
					L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tsse_core.%s_%s(p_insn->data_size,buffer);\n\t\tbreak;\n",index.GetString(),fmt.GetString());
				}
			break;
			case xmm_xm32Ib:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\t\tsse_core.%s_%s(buffer);\n\n",index.GetString(),fmt.GetString());
				fwprintf(fp,
					L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tsse_core.%s_%s(buffer);\n\t\tbreak;\n",index.GetString(),fmt.GetString());
				}
			break;
			case xmm_mmm64:
	
				if(io[0]=='O')
				{
				fwprintf(fp,L"\t\tsse_core.%s_%s(buffer);\n\n",index.GetString(),fmt.GetString());
				fwprintf(fp,
					L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tsse_core.%s_%s(buffer);\n\t\tbreak;\n",index.GetString(),fmt.GetString());
				}
			break;
			case mm_mm64:
			case xmm_xm128v:
				if(io[0]=='O')
				{
				fwprintf(fp,L"\t\tsse_core.%s_%s(buffer);\n\n",index.GetString(),fmt.GetString());
				fwprintf(fp,
					L"\t\tif( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tbreak;\n");
				}
				else
				{
				fwprintf(fp,
				L"\t\tif( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))\n"
				L"\t\t\treturn;\n\t\tsse_core.%s_%s(buffer);\n\t\tbreak;\n",index.GetString(),fmt.GetString());
				}
			break;
			case r32_xm32:
			case mm_rm32:	
			case mm_mm32:
				fwprintf(fp,L"\t\tsse_core.%s_%s(p_insn->code[p_insn->prefix_bytes+1]&7);\n\n\t\tbreak;\n",index.GetString(),fmt.GetString());
				break;
			case fmt_none:
				fwprintf(fp,L"\t\tsse_core.%s_();\n\n\t\tbreak;\n",index.GetString());
				break;
			default:
				fwprintf(fp,L"\t\tsse_core.%s_%s();\n\n\t\tbreak;\n",index.GetString(),fmt.GetString());

				break;
			}

			sLast=index;
			
		}
	}
	catch(...)
	{
	}
	fwprintf(fp,L"\n\t}\n\t}\n}\n");

	fclose (fp);
}

void gen_sse_inc()
{
	FILE * fp = fopen("sse-inc.h","wt");

	CString sLast,sSql;



	try {

		sSql.Format(L"Select MNEM ,FMT,IO from [sse$] order by MNEM");

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
			case xmm_xm64Ib:
			case xmm_xm128Ib:
			case xmm_xm32Ib:
				fwprintf(fp,L"void %s_%s_r(int r_xmm1,int r_xmm2, int Ib);\n",index.GetString(),fmt.GetString());
				fwprintf(fp,L"void %s_%s_m(int r_xmm,unsigned long *data, int Ib);\n",index.GetString(),fmt.GetString());		
			break;

			case xmm_xm32:
			case xmm_xm64:
			case xmm_xm128:
				fwprintf(fp,L"void %s_%s_r(int r_xmm1,int r_xmm2);\n",index.GetString(),fmt.GetString());
				fwprintf(fp,L"void %s_%s_m(int r_xmm,unsigned long *data);\n",index.GetString(),fmt.GetString());	
			break;
			case xmm_mmm64:


			case xmm_xm128v:
				fwprintf(fp,L"void %s_%s(char * buffer);\n\n",index.GetString(),fmt.GetString());
			break;

			break;
			case r32_xm32:
			case mm_rm32:
			case mm_mm64:
			case mm_mm32:
				fwprintf(fp,L"void %s_%s(int i);\n\n",index.GetString(),fmt.GetString());
				break;
			default:
				fwprintf(fp,L"void %s_%s();\n\n",index.GetString(),fmt.GetString());
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

//
//case  m128_xmm :
//	break;
//case  m32_r32 :
//	break;
//case  mm_mmm64 :
//	break;
//case  mm_rm32 :
//	break;
//case  mm_rm64 :
//	break;
//case  mm_xm64 :
//	break;
//case  r32_xm32 :
//	break;
//case  r32_xmm :
//	break;
//case  r32_xmm32 :
//	break;
//case  r32_xmm64 :
//	break;
//case  xm64_xmm :
//	break;
//case  xmm_mmm64 :
//	break;
//case  xmm_rm32 :
//	break;
//case  xmm_rm64 :
//	break;
//case  xmm_xm128 :
//	break;
//case  xmm_xm128Ib :
//	break;
//case  xmm_xm128v :
//	break;
//case  xmm_xm32 :
//	break;
//case  xmm_xm32Ib :
//	break;
//case  xmm_xm64 :
//	break;
//case  xmm_xm64Ib :
//	break;