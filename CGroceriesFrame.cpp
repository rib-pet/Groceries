
// MainFrm.cpp : implementation of the CGroceriesFrame class
//

#include "stdafx.h"
#include "Groceries.h"

#include "CGroceriesFrame.h"
#include "Resource.h"
#include "StationeryDockBar.h"
#include "GroceriesView.h"
#include "GroceriesBasicPage.h"
#include "StuffDetailDockBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGroceriesFrame

IMPLEMENT_DYNCREATE(CGroceriesFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CGroceriesFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_STATINERY, OnShowStioneryDockBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATINERY, OnUpdatetioneryDockBar)
	ON_COMMAND(ID_VIEW_PROPERTY, OnShowPage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTY, OnUpdateOnShowPage)

	ON_COMMAND(ID_VIEW_STUFFDETAILS, OnShowStuffDetails)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STUFFDETAILS, OnUpdateStuffDetails)

	ON_COMMAND(ID_REFRESH, &CGroceriesFrame::OnRefresh)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CGroceriesFrame construction/destruction

CGroceriesFrame::CGroceriesFrame()
	: m_pStationeryDockBar(NULL)
	, m_pBasicPage(NULL)
	, m_pStuffDetailDockBar(NULL)
{
	// TODO: add member initialization code here
}

CGroceriesFrame::~CGroceriesFrame()
{
	if(m_pStationeryDockBar)
	{
		delete m_pStationeryDockBar;
		m_pStationeryDockBar = NULL;
	}

	if(m_pBasicPage)
	{
		delete m_pBasicPage;
		m_pBasicPage = NULL;
	}

	if(m_pStuffDetailDockBar)
	{
		delete m_pStuffDetailDockBar;
		m_pStuffDetailDockBar = NULL;
	}
}

int CGroceriesFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) 
	//	)
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}
	//} 

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	EnableDocking(CBRS_ALIGN_ANY);

	CString strTitle;
	if(NULL == m_pStationeryDockBar)
	{
		strTitle.LoadString(IDS_STATIONERY_TITLE);
		m_pStationeryDockBar = StationeryDockBar::CreateDockBar(strTitle, this, ID_VIEW_STATINERY, ID_VIEW_STATINERY);
		m_pStationeryDockBar->EnableDocking(CBRS_ALIGN_ANY);
		m_pStationeryDockBar->DockToFrameWindow(CBRS_ALIGN_BOTTOM);
	}

	if(NULL == m_pBasicPage)
	{
		strTitle.LoadString(IDS_BASE_PAGE);
		m_pBasicPage = GroceriesBasicPage::CreateDockBar(strTitle, this, ID_VIEW_PROPERTY, ID_VIEW_PROPERTY);
		m_pBasicPage->EnableDocking(CBRS_ALIGN_ANY);
		m_pBasicPage->DockToFrameWindow(CBRS_ALIGN_RIGHT);
	}

	if(NULL == m_pStuffDetailDockBar)
	{
		strTitle.LoadString(IDS_STUFF_DETAIL);
		m_pStuffDetailDockBar = StuffDetailDockBar::CreateDockBar(strTitle, this, ID_VIEW_STUFFDETAILS, ID_VIEW_STUFFDETAILS);
		m_pStuffDetailDockBar->EnableDocking(CBRS_ALIGN_ANY);
		m_pStuffDetailDockBar->DockToFrameWindow(CBRS_ALIGN_BOTTOM);
	}

	CDockingManager::SetDockingMode(DT_SMART);
	
	// TODO: Delete these three lines if you don't want the toolbar to be dockable
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);
//	
	return 0;
}

BOOL CGroceriesFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CGroceriesFrame diagnostics

#ifdef _DEBUG
void CGroceriesFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CGroceriesFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CGroceriesFrame message handlers



void CGroceriesFrame::OnShowStioneryDockBar()
{
	if(m_pStationeryDockBar )
	{
		bool bVisible = ( m_pStationeryDockBar->IsVisible() == TRUE);
		m_pStationeryDockBar->ShowPane(!bVisible, false, false);
	}

}

void CGroceriesFrame::OnUpdatetioneryDockBar(CCmdUI* pCmdUI)
{
	if(m_pStationeryDockBar)
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(m_pStationeryDockBar->IsVisible());
	}
	else
	{
		pCmdUI->Enable(FALSE);
		pCmdUI->SetCheck(FALSE);
	}
}


void CGroceriesFrame::OnRefresh()
{
	if(m_pStationeryDockBar)
	{
		GroceriesCatalog* pCat = GetRoot();
		m_pStationeryDockBar->SetCatalog(pCat);
		m_pStationeryDockBar->UpdateTable();
	}

	CGroceriesView* pView = dynamic_cast<CGroceriesView*>(GetActiveView());
	if(pView)
	{
		//pView->OnRefresh();
	}
}

void CGroceriesFrame::OnShowPage()
{
	if(m_pBasicPage )
	{
		bool bVisible = ( m_pBasicPage->IsVisible() == TRUE);
		m_pBasicPage->ShowPane(!bVisible, false, false);
	}
}

void CGroceriesFrame::OnUpdateOnShowPage( CCmdUI* pCmdUI )
{
	if(m_pBasicPage)
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(m_pBasicPage->IsVisible());
	}
	else
	{
		pCmdUI->Enable(FALSE);
		pCmdUI->SetCheck(FALSE);
	}
}

GroceriesBasicPage* CGroceriesFrame::GetPage()
{
	return m_pBasicPage;
}

StationeryDockBar* CGroceriesFrame::GetStationeryDockBar()
{
	return m_pStationeryDockBar;
}

StuffDetailDockBar* CGroceriesFrame::GetStuffDetailDockBar()
{
	return m_pStuffDetailDockBar;
}

void CGroceriesFrame::OnShowStuffDetails()
{
	if(m_pStuffDetailDockBar )
	{
		bool bVisible = ( m_pStuffDetailDockBar->IsVisible() == TRUE);
		m_pStuffDetailDockBar->ShowPane(!bVisible, false, false);
	}
}

void CGroceriesFrame::OnUpdateStuffDetails( CCmdUI* pCmdUI )
{
	if(m_pStuffDetailDockBar)
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(m_pStuffDetailDockBar->IsVisible());
	}
	else
	{
		pCmdUI->Enable(FALSE);
		pCmdUI->SetCheck(FALSE);
	}
}

GroceriesCatalog* CGroceriesFrame::GetRoot()
{
	CGroceriesView* pView = dynamic_cast<CGroceriesView*>(GetActiveView());
	if(pView && pView->GetDocument())
	{
		return	pView->GetDocument()->GetRoot();
	}

	return NULL;
}

BOOL CGroceriesFrame::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext )
{
	return __super::OnCreateClient(lpcs, pContext);
}


BOOL CGroceriesFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class

	BOOL bResult = FALSE;
	if (m_pStationeryDockBar != NULL)
	{
		if (m_pStationeryDockBar->IsActive() && 
			m_pStationeryDockBar->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			bResult = TRUE;
	}

	if(m_pBasicPage != NULL)
	{
		if (m_pBasicPage->IsActive() && 
			m_pBasicPage->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			bResult = TRUE;
	}

	if(m_pStuffDetailDockBar != NULL)
	{
		if (m_pStuffDetailDockBar->IsActive() &&
			m_pStuffDetailDockBar->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			bResult = TRUE;
	}

	if (!bResult)
		return __super::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	return bResult;
}
