#include "excel.h"

LPWSTR CExcelUtil::sDriver = L"MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����

int CExcelUtil::OpenEx (LPWSTR lpszFileName)
{
  CString sExcelFile; 
  CString sSql;

  sExcelFile = lpszFileName;
  if (!IsExcelDriverInstalled())
  {
		return 0;
  }
  // �������д�ȡ���ַ���
  sSql.Format(L"DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;DBQ=%s;CREATE_DB=\"%s\"",sDriver, sExcelFile, sExcelFile);
//FIRSTROWHASNAMES=1;CREATE_DB=\"%s\";
  // �������ݿ� (��Excel����ļ�)
  try {
	  database.OpenEx(sSql,CDatabase::noOdbcDialog);
  } catch ( CDBException& ex )
  {
	  wprintf(L"%s\n",(LPCWSTR)ex.m_strError);
	  return 0;
  }

  return 1;
}

int CExcelUtil::ExecSql (CString sSql)
{
    database.ExecuteSQL(sSql);

	return 0;
}

bool CExcelUtil::IsExcelDriverInstalled()
{
    WCHAR szBuf[2001];
    WORD cbBufMax = 2000;
    WORD cbBufOut;
    LPWSTR pszBuf = szBuf;

    // ��ȡ�Ѱ�װ����������(������odbcinst.h��)
    if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
        return false;
    
    // �����Ѱ�װ�������Ƿ���Excel...
    do
    {
        if (wcsstr(pszBuf, L"Excel") != 0)
        {
            //���� !
            sDriver = _wcsdup(pszBuf);
            return true;
        }
        pszBuf = wcschr(pszBuf, L'\0') + 1;
    }
    while (pszBuf[1] !=L'\0');

    return false;
}

int CExcelUtil::OpenRecordSet (CString sSql) 
{
	recset.Close();
   // ִ�в�ѯ���
   recset.Open(CRecordset::forwardOnly, sSql, CRecordset::readOnly);
   return 0;
}



int CExcelUtil::Open (LPWSTR lpszFileName) 
{
    CString sDriver;
    CString sDsn;
    CString sFile;


	sFile = lpszFileName;

 
    // �������д�ȡ���ַ���
    sDsn.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s", sDriver, sFile);

    // �����ݿ�(��Excel�ļ�)
    database.Open(NULL, false, false, sDsn);

	return 0;
}

