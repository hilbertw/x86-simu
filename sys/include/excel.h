#include <afxdb.h> 
#include <odbcinst.h>

class CExcelUtil
{
	static LPWSTR sDriver; // Excel��װ����

	CDatabase database;
	CRecordset recset;

public:
	CExcelUtil() :    recset(CRecordset(&database))
	{}

	~CExcelUtil()
	{
		Close();
	}

	bool IsEOF()
	{
		return(recset.IsEOF()!=0);
	}

	int Close () 
	{
		recset.Close();
		database.Close();
		return 0;
	}

	CString GetFieldValue(LPWSTR lpszField)
	{
		CString sValue;
		//��ȡExcel�ڲ���ֵ
		recset.GetFieldValue(lpszField, sValue);

		return sValue;
	}

	void NextRecord()
	{
		recset.MoveNext();
	}

	int ExecSql (CString sSql);
	int OpenEx (LPWSTR lpszFileName);
	int Open (LPWSTR lpszFileName);
	int OpenRecordSet (CString sSql);
	static bool IsExcelDriverInstalled();
};