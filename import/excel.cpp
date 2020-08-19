#include "excel.h"

LPWSTR CExcelUtil::sDriver = L"MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel安装驱动

int CExcelUtil::OpenEx (LPWSTR lpszFileName)
{
  CString sExcelFile; 
  CString sSql;

  sExcelFile = lpszFileName;
  if (!IsExcelDriverInstalled())
  {
		return 0;
  }
  // 创建进行存取的字符串
  sSql.Format(L"DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;DBQ=%s;CREATE_DB=\"%s\"",sDriver, sExcelFile, sExcelFile);
//FIRSTROWHASNAMES=1;CREATE_DB=\"%s\";
  // 创建数据库 (既Excel表格文件)
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

    // 获取已安装驱动的名称(涵数在odbcinst.h里)
    if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
        return false;
    
    // 检索已安装的驱动是否有Excel...
    do
    {
        if (wcsstr(pszBuf, L"Excel") != 0)
        {
            //发现 !
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
   // 执行查询语句
   recset.Open(CRecordset::forwardOnly, sSql, CRecordset::readOnly);
   return 0;
}



int CExcelUtil::Open (LPWSTR lpszFileName) 
{
    CString sDriver;
    CString sDsn;
    CString sFile;


	sFile = lpszFileName;

 
    // 创建进行存取的字符串
    sDsn.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s", sDriver, sFile);

    // 打开数据库(既Excel文件)
    database.Open(NULL, false, false, sDsn);

	return 0;
}

