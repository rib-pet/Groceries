
// GroceriesDoc.cpp : implementation of the CGroceriesDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Groceries.h"
#endif

#include "GroceriesDoc.h"

#include <propkey.h>
#include "Groceries\GroceriesCatalog.h"
#include <io.h>
#include "GroceiersImport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGroceriesDoc

IMPLEMENT_DYNCREATE(CGroceriesDoc, CDocument)

BEGIN_MESSAGE_MAP(CGroceriesDoc, CDocument)
END_MESSAGE_MAP()


// CGroceriesDoc construction/destruction

CGroceriesDoc::CGroceriesDoc()
	: m_pCatalog(NULL)
	, m_strCGroceriesXMLFile(_T(""))
{
	// TODO: add one-time construction code here
	m_pCatalog = new GroceriesCatalog(_T("Team"));
	ImportStationeryData();
}

CGroceriesDoc::~CGroceriesDoc()
{
	if(m_pCatalog)
	{
		delete m_pCatalog;
		m_pCatalog = NULL;
	}
}

BOOL CGroceriesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



// CGroceriesDoc serialization

void CGroceriesDoc::Serialize(CArchive& ar)
{
	//if (ar.IsStoring())
	//{
	//	// TODO: add storing code here
	//	if(m_pCatalog)
	//		ar << m_pCatalog;
	//}
	//else
	//{
	//	// TODO: add loading code here
	//	ar >> m_pCatalog;
	//}
	if(m_pCatalog)
		m_pCatalog->Serialize(ar);
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CGroceriesDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CGroceriesDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGroceriesDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGroceriesDoc diagnostics

#ifdef _DEBUG
void CGroceriesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGroceriesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGroceriesDoc commands

GroceriesCatalog* CGroceriesDoc::GetRoot()
{
	return m_pCatalog;
}

void CGroceriesDoc::ImportStationeryData()
{
	if(m_pCatalog)
	{
		//import data from ".xls" file
		WCHAR filepath[MAX_PATH] = {0x0};
		//GetModuleFileName(NULL,filepath,sizeof(filepath));
		//PathRemoveFileSpec(filepath);
		GetCurrentDirectory(sizeof(filepath),filepath);
		CString pathOfExcel(filepath);
		pathOfExcel += L"\\document\\stationery.xls";

		if ( _waccess(pathOfExcel.operator LPCTSTR(),0) == -1 )
		{
			AfxMessageBox(L"Can not open file :" + pathOfExcel);
			return ;
		}

		m_strCGroceriesXMLFile = pathOfExcel;

		GroceriesImportWrapper importdata(m_strCGroceriesXMLFile, m_pCatalog);
		int iRet = 0;
		//iRet = importdata.ReadExcel();
		//iRet = importdata.WriteXml();
		iRet = importdata.ReadXml();
	}
}

void CGroceriesDoc::SendToService()
{
	GroceriesImportWrapper importdata(m_strCGroceriesXMLFile, m_pCatalog);
	int iRet = importdata.WriteXml();
	importdata.SendToSevice();
}
