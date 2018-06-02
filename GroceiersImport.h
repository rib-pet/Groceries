#ifndef _PDC_GROCEIERSIMPORT_H
#define _PDC_GROCEIERSIMPORT_H
#pragma once

#include "Groceries\GroceriesCatalog.h"

class GroceriesCatalog;

#include <string>
#include <vector>
using namespace std;

#include ".\Groceries\PDCCore.h"
#include "afx.h"

class PDC_CLASS_DECL GroceriesImportWrapper 
{
public: 
	GroceriesImportWrapper();
	~GroceriesImportWrapper();

	GroceriesImportWrapper(CString excelPath , GroceriesCatalog* pCatalog);

	int ReadExcel();
	void WriteExcel();

	int ReadXml();
	int WriteXml();
	void SendToSevice();
protected:
	class GroceriesImport;
	GroceriesImport* m_pExport;

};


#endif // !_PDC_GROCEIERSIMPORT_H
