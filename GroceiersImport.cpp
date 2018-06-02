#include "stdafx.h"
#include "..\Groceries\GroceiersImport.h"
#include "io.h"
//#include <unistd.h>
#include <fcntl.h>
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>
#include <msclr/marshal_atl.h>

#pragma warning(disable:4273)

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace System;
//using namespace System::Core;
using namespace System::Data;
using namespace System::Xml;
using namespace System::Reflection;
using namespace Microsoft::Office::Interop::Excel;
using namespace System::Collections;
using namespace System::IO;
//using namespace WindowsBase;

using namespace System::Runtime::InteropServices;
using namespace System::Data::OleDb;
using namespace msclr::interop;
using namespace ClientLib;

class GroceriesImportWrapper::GroceriesImport
{
public:
	GroceriesImport();
	~GroceriesImport();

	gcroot<XmlWriter^> m_pXmlWriter;
	gcroot<XmlDocument^> m_pXmlDoc;
public:
	GroceriesImport(CString importPath,GroceriesCatalog* pCatalog);
	int ReadExcel();
	void WriteExcel();

	int ReadXml();
	int WriteXml();

	//void ReadExcelEx();

private:
	CString m_szImportPath;
	GroceriesCatalog* m_pCatalog;
	
};

int GroceriesImportWrapper::GroceriesImport::ReadExcel()
{

	try
	{
		String^ _excelPath = gcnew String(m_szImportPath.operator LPCTSTR());

		String^ strConn = "Provider=Microsoft.Jet.OLEDB.4.0;";
		strConn += "Data Source=";
		strConn += _excelPath;
		strConn += ";";
		strConn += "Extended Properties=Excel 8.0;";
		OleDbConnection^ conn = gcnew OleDbConnection(strConn);

		conn->Open();
		String^ strExcel = nullptr;
		OleDbDataAdapter^ myCommand = nullptr;
		DataSet^ ds = nullptr;
		strExcel = "select * from [HR$]";

		myCommand = gcnew OleDbDataAdapter(strExcel, strConn);

		System::Data::OleDb::OleDbCommandBuilder^ builder = gcnew OleDbCommandBuilder(myCommand);
		builder->QuotePrefix = "[";
		builder->QuoteSuffix = "]";

		ds = gcnew DataSet();
		myCommand->Fill(ds, "Table1");

		int nRows = ds->Tables[0]->Rows->Count;
		int nColumn = ds->Tables[0]->Columns->Count;

		std::vector<CString> elementRow;
		std::vector<vector<CString>> elements;	

		for (int i = 0; i<31;i++)
		{
			elementRow.clear();
			for (int j = 0;j<4;j++)
			{
				String^ strCell = ds->Tables[0]->Rows[3+i][j]->ToString()->Trim();
				if (!String::IsNullOrEmpty(strCell))
				{
					CString cells = marshal_as<CString>(strCell);
					elementRow.push_back(cells);
				}
				else
				{
					elementRow.push_back(L"");
				}
			}
			if (elementRow.at(1).GetLength()!=0)
			{
				elements.push_back(elementRow);
			}

		}

		for (int i = 0; i<31;i++)
		{
			elementRow.clear();
			for (int j = 7;j<11;j++)
			{
				String^ strCell = ds->Tables[0]->Rows[3+i][j]->ToString()->Trim();
				if (!String::IsNullOrEmpty(strCell))
				{
					CString cells = marshal_as<CString>(strCell);
					elementRow.push_back(cells);
				}
				else
				{
					elementRow.push_back(L"");
				}
			}
			if (elementRow.at(1).GetLength()!=0)
			{
				elements.push_back(elementRow);
			}
		}

		conn->Close();

		int sizeRows = elements.size();
		int sizeColumn = (elements.at(0)).size();

		//create the node
		GroceriesGroup* pNewGroup = m_pCatalog->CreateGroup();
		std::vector<Stationery*> m_rgpStationery;

		UINT nID = 0;
		double m_dUnitPrice = 0;
		double m_dQuantity = 0;
		CString m_strName = _T("");
		CString m_strDescription = _T("");
		CString m_strComment = _T("");

		for (int index = 0;index< sizeRows ;index++)
		{
			nID = _wtoi((elements.at(index)).at(0));
			m_strName = (elements.at(index)).at(1);
			m_strDescription = (elements.at(index)).at(2);
			m_dUnitPrice = _wtof((elements.at(index)).at(3));

			Stationery* pNewStationery = NULL;
			pNewStationery = m_pCatalog->CreateStationery();

			pNewStationery->SetID(nID);
			pNewStationery->SetUnitRate(m_dUnitPrice);
			pNewStationery->SetQuantity(m_dQuantity);
			pNewStationery->SetName(m_strName);
			pNewStationery->SetComment(m_strComment);
			pNewStationery->SetDescription(m_strDescription);

			m_rgpStationery.push_back(pNewStationery);
		}

		int stationerycount = m_rgpStationery.size();

	}
	catch(Exception^ e)
	{
		CString strError(e->Message);
		AfxMessageBox(strError);
	}
	
	return 0;
}

void GroceriesImportWrapper::GroceriesImport::WriteExcel()
{

}

GroceriesImportWrapper::GroceriesImport::GroceriesImport()
	: m_pXmlWriter(nullptr),
	  m_pXmlDoc(nullptr),
	  m_pCatalog(NULL)
{
	
}

GroceriesImportWrapper::GroceriesImport::~GroceriesImport()
{
	if(m_pXmlWriter)
	{
		delete m_pXmlWriter;
		m_pXmlWriter = nullptr;
	}
	if(m_pXmlDoc)
	{
		delete m_pXmlDoc;
		m_pXmlDoc = nullptr;
	}
}


GroceriesImportWrapper::GroceriesImport::GroceriesImport(CString importPath,GroceriesCatalog* pCatalog):
	m_szImportPath(importPath),
	m_pCatalog(pCatalog)
{
	if (!m_pXmlDoc)
	{
		m_pXmlDoc = gcnew XmlDocument();
	}
}

GroceriesImportWrapper::GroceriesImportWrapper()
{

}

GroceriesImportWrapper::~GroceriesImportWrapper()
{
	if (m_pExport!=NULL)
	{
		delete m_pExport;
		m_pExport = NULL;
	}
}

GroceriesImportWrapper::GroceriesImportWrapper(CString excelPath,GroceriesCatalog* pCatalog)
{
	m_pExport = new GroceriesImport(excelPath,pCatalog);
}

int GroceriesImportWrapper::ReadExcel()
{
	return m_pExport->ReadExcel();
}

void GroceriesImportWrapper::WriteExcel()
{
	m_pExport->WriteExcel();
}

int GroceriesImportWrapper::GroceriesImport::ReadXml()
{
	////
	//test
// 	String^ value = "";
// 	StationeryOperate^ stat = gcnew StationeryOperate();
// 	stat->XmlToString();
// 	String^ result = stat->DataTableToXmlStr(value);
// 	stat->Close();

	////
	String^ strExcelPath = gcnew String(m_szImportPath.operator LPCTSTR());
	string _XMLPath = marshal_as<string>(strExcelPath);
	size_t found = _XMLPath.rfind("\\");
	if (found != std::string::npos)
		_XMLPath.erase(found,std::string::npos);
	_XMLPath += "\\stationery.xml";
	String^ strXMLPathname = gcnew String(_XMLPath.c_str());
	m_pXmlDoc->RemoveAll();
	m_pXmlDoc->Load(strXMLPathname);


	//create the node
	GroceriesGroup* pNewGroup = m_pCatalog->CreateGroup();
	std::vector<Stationery*> m_rgpStationery;

	UINT nID = 0;
	double m_dUnitPrice = 0;
	double m_dQuantity = 0;
	CString m_strName = _T("");
	CString m_strDescription = _T("");
	CString m_strComment = _T("");

	String^ localName = m_pXmlDoc->DocumentElement->LocalName;

// 	XmlNode^ xn = m_pXmlDoc->SelectSingleNode(localName);// 得到根节点bookstore
// 	XmlNodeList^ xnlist = xn->ChildNodes;

	//TODO if more than one child(group)
	XmlNode^ xNode = m_pXmlDoc->DocumentElement->FirstChild;
	XmlNodeList^ xnList = xNode->ChildNodes;

	//XmlNodeList^ table_valude_nodes;

	int nodecount = xnList->Count;
	for (int i = 0; i < nodecount; i++)
	{
		//table_valude_nodes = xnList[i]->SelectNodes("Name");
		//table_valude_nodes = xNode->SelectNodes("Stationery");
		XmlNodeList^ xnStationery = xnList[i]->ChildNodes;

		nID = atoi((marshal_as<string>(xnStationery[0]->InnerText)).c_str());
		m_strName = marshal_as<CString>(xnStationery[1]->InnerText);
		m_strDescription = marshal_as<CString>(xnStationery[2]->InnerText);
		m_dUnitPrice = atof((marshal_as<string>(xnStationery[3]->InnerText)).c_str());

		Stationery* pNewStationery = NULL;
		pNewStationery = m_pCatalog->CreateStationery();

		pNewStationery->SetID(nID);
		pNewStationery->SetUnitRate(m_dUnitPrice);
		pNewStationery->SetQuantity(m_dQuantity);
		pNewStationery->SetName(m_strName);
		pNewStationery->SetComment(m_strComment);
		pNewStationery->SetDescription(m_strDescription);

		m_rgpStationery.push_back(pNewStationery);
	}

	int stationerycount = m_rgpStationery.size();



	return true;
}

int GroceriesImportWrapper::GroceriesImport::WriteXml()
{
	if (m_pCatalog == NULL)
	{
		return -1;
	}

	CString m_CatName = m_pCatalog->GetName();
	CString m_CatDesc = m_pCatalog->GetDescription();

	//add xml node
	m_pXmlDoc->LoadXml(L"<GroceriesCatalog></GroceriesCatalog>");
	XmlNode^ xn = m_pXmlDoc->SelectSingleNode("GroceriesCatalog");
	//XmlNodeList^ xnl = xn.ChildNodes;

	int nCount = m_pCatalog->GetChildrenCount();
	if(nCount > 0)
	{
		for(int i=0; i<nCount; ++i)
		{
			GroceriesGroup* pTempGroup = m_pCatalog->GetGroupAt(i);
			if(pTempGroup)
				;
				//ar << pTempGroup;
		}
	}

	nCount = m_pCatalog->GetStationeryCount();
	if( nCount > 0)
	{
		XmlElement^ xelGroup = m_pXmlDoc->CreateElement("Group");
		xn->AppendChild(xelGroup);
		for(int i=0; i<nCount; ++i)
		{
			Stationery* pTempStationery = m_pCatalog->GetStationeryAt(i);
			if(pTempStationery)
			{
				//create a new node
				XmlElement^ xelKey = m_pXmlDoc->CreateElement("Stationery");
// 				XmlAttribute^ xelType = m_pXmlDoc->CreateAttribute("Type");
// 				xelType->InnerText = "Science";
// 				xelKey->SetAttributeNode(xelType);

				XmlElement^ xelStationery = m_pXmlDoc->CreateElement("ID");
				char szID[16] = {0x0};
				itoa(pTempStationery->GetID(),szID,10);
				String^ strXMLID = gcnew String(szID);
				xelStationery->InnerText = strXMLID;
				xelKey->AppendChild(xelStationery);

				xelStationery = m_pXmlDoc->CreateElement("Name");
				String^ strXMLName = gcnew String(pTempStationery->GetName());
				xelStationery->InnerText = strXMLName;
				xelKey->AppendChild(xelStationery);

				xelStationery = m_pXmlDoc->CreateElement("Description");
				String^ strXMLDesc = gcnew String(pTempStationery->GetDescription());
				xelStationery->InnerText = strXMLDesc;
				xelKey->AppendChild(xelStationery);

				xelStationery = m_pXmlDoc->CreateElement("UnitPrice");
				double^ dUnitPrice = pTempStationery->GetUnitRate();
				xelStationery->InnerText = dUnitPrice->ToString("f2");
				xelKey->AppendChild(xelStationery);


				xelGroup->AppendChild(xelKey);

			}
		}
		String^ strExcelPath = gcnew String(m_szImportPath.operator LPCTSTR());
		string _XMLPath = marshal_as<string>(strExcelPath);
		size_t found = _XMLPath.rfind("\\");
		if (found != std::string::npos)
		    _XMLPath.erase(found,std::string::npos);
		_XMLPath += "\\stationery.xml";
		String^ strXMLPath = gcnew String(_XMLPath.c_str());
		
		m_pXmlDoc->Save(strXMLPath);
		
	}


	return 0;
}

int GroceriesImportWrapper::ReadXml()
{
	return m_pExport->ReadXml();
}

int GroceriesImportWrapper::WriteXml()
{
	return m_pExport->WriteXml();
}

void GroceriesImportWrapper::SendToSevice()
{
	String^ value = "";
	StationeryOperate^ stat = gcnew StationeryOperate();
	stat->XmlToString();
	String^ result = stat->DataTableToXmlStr(value);
	stat->Close();
}
