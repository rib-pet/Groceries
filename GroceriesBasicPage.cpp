#include "stdafx.h"
#include "GroceriesBasicPage.h"
#include "Groceries\GroceriesCatalog.h"
#include "Groceries\GroceriesGroup.h"
#include "Groceries\GroceriesItem.h"
#include "GroerierFinder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(GroceriesBasicPage, CPaneDialog)

	BEGIN_MESSAGE_MAP(GroceriesBasicPage, CPaneDialog)
		ON_EN_KILLFOCUS(IDC_KEY, &GroceriesBasicPage::OnEnKillfocusKey)
		ON_EN_KILLFOCUS(IDC_DESC, &GroceriesBasicPage::OnEnKillfocusDesc)
		ON_WM_SIZE()
	END_MESSAGE_MAP()    

GroceriesBasicPage::GroceriesBasicPage(void)
	: m_pObject(NULL)
{
}


GroceriesBasicPage::~GroceriesBasicPage(void)
{
}

void GroceriesBasicPage::SetObject( CObject* pObject)
{
	if(m_pObject == pObject)
		return;

	CString strTitle;
	strTitle.LoadString(IDS_BASE_PAGE);
	SetWindowText(strTitle);

	m_pObject = pObject;

	if(m_pObject)
	{
		CString strName = _T("");
		CString strDesc = _T("");
		GroceriesCatalog* pCat = dynamic_cast<GroceriesCatalog*>(m_pObject);
		GroceriesGroup* pGroup = dynamic_cast<GroceriesGroup*>(m_pObject);
		GroceriesItem* pItem = dynamic_cast<GroceriesItem*>(m_pObject);
		if(pCat)
		{
			strName = pCat->GetName();
			strDesc = pCat->GetDescription();
		}
		else if(pGroup)
		{
			strName = pGroup->GetName();
			strDesc = pGroup->GetDescription();
		}
		else if(pItem)
		{
			strName = pItem->GetName();
			strDesc = pItem->GetDescription();
		}

		m_ctrKey.SetWindowText(strName);
		m_ctrDesc.SetWindowText(strDesc);
	}
}

void GroceriesBasicPage::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_KEY, m_ctrKey);
	DDX_Control(pDX, IDC_DESC, m_ctrDesc);
}

GroceriesBasicPage* GroceriesBasicPage::CreateDockBar( LPCTSTR lpszWindowName, CWnd* pParentWnd,  UINT nDockbarID ,
													  UINT nCmdID /*= 0*/, DWORD dwStyle )
{
	GroceriesBasicPage* pBasicPage = new GroceriesBasicPage();

	if(!pBasicPage->Create(lpszWindowName, pParentWnd, TRUE, MAKEINTRESOURCE(IDD_GRO_BASIC_PAGE), dwStyle, nCmdID))
	{
		delete pBasicPage;
		pBasicPage = NULL;
	}

	return pBasicPage;
}


void GroceriesBasicPage::OnEnKillfocusKey()
{
	// TODO: Add your control notification handler code here
	CString strKey;
	m_ctrKey.GetWindowText(strKey);

	GroceriesCatalog* pCat = dynamic_cast<GroceriesCatalog*>(m_pObject);
	GroceriesGroup* pGroup = dynamic_cast<GroceriesGroup*>(m_pObject);
	GroceriesItem* pItem = dynamic_cast<GroceriesItem*>(m_pObject);
	
	if(strKey.IsEmpty() || !IsKeyUnique(m_pObject, strKey))
	{
		CString strName = _T("");
		if(pCat)
			strName = pCat->GetName();
		else if(pGroup)
			strName = pGroup->GetName();
		else if(pItem)
			strName = pItem->GetName();

//		AfxMessageBox(_T("Please input valid name"));
		m_ctrKey.SetWindowText(strName);
		return;
	}

	if(pCat)
		pCat->SetName(strKey);
	else if(pGroup)
		pGroup->SetName(strKey);
	else if(pItem)
		pItem->SetName(strKey);
}


void GroceriesBasicPage::OnEnKillfocusDesc()
{
	CString strDesc;
	m_ctrDesc.GetWindowText(strDesc);

	GroceriesCatalog* pCat = dynamic_cast<GroceriesCatalog*>(m_pObject);
	GroceriesGroup* pGroup = dynamic_cast<GroceriesGroup*>(m_pObject);
	GroceriesItem* pItem = dynamic_cast<GroceriesItem*>(m_pObject);


	if(strDesc.IsEmpty() || !IsDescUnique(m_pObject, strDesc))
	{
		CString str = _T("");
		if(pCat)
			str = pCat->GetDescription();
		else if(pGroup)
			str = pGroup->GetDescription();
		else if(pItem)
			str = pItem->GetDescription();

//		AfxMessageBox(_T("Please input valid description"));
		m_ctrDesc.SetWindowText(str);
		return;
	}

	if(pCat)
		pCat->SetDescription(strDesc);
	else if(pGroup)
		pGroup->SetDescription(strDesc);
	else if(pItem)
		pItem->SetDescription(strDesc);
}

bool GroceriesBasicPage::IsKeyUnique( CObject* m_pObject, CString strKey )
{
	GroceriesCatalog* pCat = dynamic_cast<GroceriesCatalog*>(m_pObject);
	GroceriesGroup* pGroup = dynamic_cast<GroceriesGroup*>(m_pObject);
	GroceriesItem* pItem = dynamic_cast<GroceriesItem*>(m_pObject);

	GroerierFinder finder(strKey, true);

	if(pCat)
		pCat->Accept(&finder);
	else if(pGroup)
		pGroup->Accept(&finder);
	else if(pItem)
		pItem->Accept(&finder);

	return finder.IsUnique();
}

bool GroceriesBasicPage::IsDescUnique( CObject* m_pObject, CString strDesc )
{
	GroceriesCatalog* pCat = dynamic_cast<GroceriesCatalog*>(m_pObject);
	GroceriesGroup* pGroup = dynamic_cast<GroceriesGroup*>(m_pObject);
	GroceriesItem* pItem = dynamic_cast<GroceriesItem*>(m_pObject);

	GroerierFinder finder(strDesc, false);

	if(pCat)
		pCat->Accept(&finder);
	else if(pGroup)
		pGroup->Accept(&finder);
	else if(pItem)
		pItem->Accept(&finder);

	return finder.IsUnique();
}


void GroceriesBasicPage::OnSize(UINT nType, int cx, int cy)
{
	CPaneDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if(IsWindow(m_ctrKey.m_hWnd) && IsWindow(m_ctrDesc.m_hWnd))
	{
		m_ctrKey.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE);
		m_ctrDesc.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE);
	}
}

bool GroceriesBasicPage::IsActive()
{
	if(IsAutoHideMode() && IsHideInAutoHideMode())
	{
		if(!m_pAutoHideBar || !::IsWindow(m_pAutoHideBar->m_hWnd) || !m_pAutoHideBar->IsVisible())
			return FALSE;
	}

	if(::IsWindow(m_hWnd))
	{
		CWnd* pWnd = CWnd::GetFocus();
		while(pWnd != NULL)
		{
			if(pWnd == this)
				return true;

			if(pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
				break;

			pWnd = pWnd->GetParent();
		}
	}
	return false;
}
