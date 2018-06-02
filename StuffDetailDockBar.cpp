#include "stdafx.h"
#include "StuffDetailDockBar.h"
#include "Groceries\GroceriesDef.h"
#include "Groceries\GroceriesItem.h"
#include "Groceries\Stuff.h"
#include "Groceries\Stationery.h"
#include "StationeryOrStuffDlg.h"
#include "CGroceriesFrame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(StuffDetailDockBar, CPaneDialog)

	BEGIN_MESSAGE_MAP(StuffDetailDockBar, CPaneDialog)
		ON_WM_SIZE()
		ON_WM_DESTROY()
		ON_NOTIFY(NM_RCLICK, IDC_STUFF_LIST, &StuffDetailDockBar::OnNMRClickStuffList)
		ON_COMMAND(ID_STATIONERY_APPEND, &StuffDetailDockBar::OnStationeryAppend)
		ON_UPDATE_COMMAND_UI(ID_STATIONERY_APPEND, &StuffDetailDockBar::OnUpdateStationeryAppend)
		ON_COMMAND(ID_STATIONERY_DELETE, &StuffDetailDockBar::OnStationeryDelete)
		ON_UPDATE_COMMAND_UI(ID_STATIONERY_DELETE, &StuffDetailDockBar::OnUpdateStationeryDelete)
		ON_NOTIFY(LVN_COLUMNCLICK, IDC_STUFF_LIST, &StuffDetailDockBar::OnLvnColumnclickStuffList)
	END_MESSAGE_MAP()    


StuffDetailDockBar::StuffDetailDockBar(void)
{
}


StuffDetailDockBar::~StuffDetailDockBar(void)
{
}

StuffDetailDockBar* StuffDetailDockBar::CreateDockBar( LPCTSTR lpszWindowName, CWnd* pParentWnd, UINT nDockbarID /*= AFX_IDW_DIALOGBAR*/, UINT nCmdID /*= 0*/, DWORD dwStyle /*= WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC |CBRS_HIDE_INPLACE*/ )
{
	StuffDetailDockBar* pStuffDockBar = new StuffDetailDockBar();

	if(!pStuffDockBar->Create(lpszWindowName, pParentWnd, TRUE, MAKEINTRESOURCE(IDD_STUFF_DLG), dwStyle, nCmdID))
	{
		delete pStuffDockBar;
		pStuffDockBar = NULL;
	}

	if(pStuffDockBar->GetSafeHwnd())
	{
		pStuffDockBar->OnInitDialog();
	}

	return pStuffDockBar;
}

void StuffDetailDockBar::SetObject( CObject* pObject )
{
	if(pObject != m_pObject)
	{
		m_pObject = pObject;
	}

	UpdateTableCtrl();
	
}

void StuffDetailDockBar::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STUFF_LIST, m_ListCtrl);
}

void StuffDetailDockBar::OnSize( UINT nType, int cx, int cy )
{
	__super::OnSize(nType,cx, cy);

	if(m_ListCtrl.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(rc);

		int dx = rc.TopLeft().x;
		int dy = rc.TopLeft().y;
		int cx = (rc.Width() >= 2 * dx) ? rc.Width() - 2 * dx:0;
		int cy = (rc.Height() >= 2 * dy) ? rc.Height()- 2 * dy:0;
		m_ListCtrl.SetWindowPos(NULL, dx, dy, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void StuffDetailDockBar::OnDestroy()
{

}

void StuffDetailDockBar::OnInitDialog()
{
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_ListCtrl.SetExtendedStyle(dwStyle);

	// initialize table header
	m_ListCtrl.InsertColumn(0, STATIONERY_NUMBER,	LVCFMT_LEFT, 30);
	m_ListCtrl.InsertColumn(1, STATIONERY_NAME,		LVCFMT_LEFT, 60);
	m_ListCtrl.InsertColumn(2, STATIONERY_SPEC,		LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(3, STATIONERY_UP,		LVCFMT_LEFT, 80);
	m_ListCtrl.InsertColumn(4, STATIONERY_QTY,		LVCFMT_LEFT, 80);
	m_ListCtrl.InsertColumn(5, STATIONERY_CMT,		LVCFMT_LEFT, 160);
}


void StuffDetailDockBar::OnNMRClickStuffList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	*pResult = 0;

	CPoint point;
	GetCursorPos(&point);

	CPoint pt = point;
	m_ListCtrl.ScreenToClient(&pt);


	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_STATIONERY));

	CMenu* pPopup = menu.GetSubMenu(0);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{
		pWndPopupOwner = pWndPopupOwner->GetParent();
	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);

	*pResult = 0;
}


void StuffDetailDockBar::OnStationeryAppend()
{
	// TODO: Add your command handler code here

	CGroceriesFrame* pFrame = dynamic_cast<CGroceriesFrame*>(GetParentFrame());
	if(pFrame && m_pObject)
	{
		CString Text;
		GroceriesItem* pItem = dynamic_cast<GroceriesItem* >(m_pObject);
		if(pItem)
		{
			Stuff* pNewStuff = pItem->CreateStuff();
			if(pNewStuff)
			{
				StationeryOrStuffDlg dlg(pNewStuff);

				CString Text;
				Text.Format(_T("New Stationery %s"), pNewStuff->GetName());
				dlg.SetTitle(Text);

				if(IDOK == dlg.DoModal())
				{

				}
				else
				{
					CString strID;
					strID.Format(_T("%s"), pNewStuff->GetName());
					if(!strID.IsEmpty())
						pItem->DeleteStuff(strID);
				}

				UpdateTableCtrl();
			}
		}
	}
}


void StuffDetailDockBar::OnUpdateStationeryAppend(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = FALSE;

	if(m_pObject)
	{
		GroceriesItem* pItem = dynamic_cast<GroceriesItem* >(m_pObject);
		if(pItem)
			bEnable = TRUE;
	}

	pCmdUI->Enable(bEnable);
}


void StuffDetailDockBar::OnStationeryDelete()
{
	// TODO: Add your command handler code here
}


void StuffDetailDockBar::OnUpdateStationeryDelete(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = FALSE;

	if(m_pObject)
	{
		GroceriesItem* pItem = dynamic_cast<GroceriesItem* >(m_pObject);
		if(pItem)
		{
			std::vector<Stuff*> rgpStuffs;
			int nCount = pItem->GetAllStuffs(rgpStuffs);

			bEnable = !rgpStuffs.empty();
		}
	}

	pCmdUI->Enable(bEnable);
}


void StuffDetailDockBar::OnLvnColumnclickStuffList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

}

bool StuffDetailDockBar::IsActive()
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

void StuffDetailDockBar::UpdateTableCtrl()
{
	// clear all items
	m_ListCtrl.DeleteAllItems();

	if(m_pObject)
	{
		GroceriesItem* pItem = dynamic_cast<GroceriesItem* >(m_pObject);
		if(pItem)
		{
			std::vector<Stuff*> rgpStuffs;
			int nCount = pItem->GetAllStuffs(rgpStuffs);

			for (int i=0; i< nCount; ++i)
			{
				Stuff* pStuff = rgpStuffs.at(i);

				if(!pStuff)
					continue;

				Stationery* pStationery = pStuff->GetStationery();
				if(NULL == pStationery)
					continue;

				// ID
				CString Text;
				Text.Format(_T("%d"), pStationery->GetID());
				int index = m_ListCtrl.InsertItem(i, Text);

				// Name
				Text = pStuff->GetName();
				m_ListCtrl.SetItemText(index, 1, Text);

				// Specification
				Text = pStationery->GetDescription();
				m_ListCtrl.SetItemText(index, 2, Text);

				// Unit price
				Text.Format(_T("%.2f"), pStationery->GetUnitRate());
				m_ListCtrl.SetItemText(index, 3, Text);

				// Quantity
				Text.Format(_T("%.2f"), pStuff->GetQuantity());
				m_ListCtrl.SetItemText(index, 4, Text);

				// Comment ==  Description of Stuff
				Text = pStuff->GetDescription();
				m_ListCtrl.SetItemText(index, 5, Text);

				m_ListCtrl.SetItemData(i, (DWORD_PTR)pStuff);
			}
		}
	}
}
