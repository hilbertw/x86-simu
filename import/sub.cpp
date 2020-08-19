#define  _CRT_SECURE_NO_WARNINGS 1

#include <assert.h>
#include "stdafx.h"
#include "string.h"
#include "excel.h"

extern CExcelUtil ExcelUtil;

void gen_fmt_id(LPCWSTR wcstbl)
{
	CString sLast,sSql,sFile,sFmt_none=L"fmt_none";
	sFile.Format(L"%s-fmt-def.h",wcstbl);
	FILE *fp = _wfopen((LPCWSTR)sFile,L"wt");

	try {

		sSql.Format(L"Select FMT from [%s$] order by FMT",wcstbl);

		ExcelUtil.OpenRecordSet (sSql);
		while (!ExcelUtil.IsEOF())
		{
			CString fmt = ExcelUtil.GetFieldValue(L"FMT");
			ExcelUtil.NextRecord();

			if( fmt!= sLast&&fmt!= sFmt_none)
			{
			fwprintf(fp,L"	OPER_FMT_REC( %s )\n",(LPCWSTR)fmt	);
	
			}
			sLast=fmt;
		}
	}
	catch(...)
	{
	}

	fclose (fp);
}

void gen_insn_id(LPCWSTR wcstbl)
{
	CString sLast,sSql,sFile;

	sFile.Format(L"%s-id-def.h",wcstbl);
	FILE *fp = _wfopen((LPCWSTR)sFile,L"wt");

	try {

		sSql.Format(L"Select MNEM from [%s$] order by MNEM",wcstbl);

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