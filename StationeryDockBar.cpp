#include "stdafx.h"
#include "StationeryDockBar.h"
#include "Groceries\GroceriesDef.h"
#include "CGroceriesFrame.h"
#include "GroceriesView.h"
#include "GroceriesDoc.h"
#include "Groceries\GroceriesCatalog.h"
#include "Resource.h"
#include "Groceries\Stationery.h"
#include "StationeryOrStuffDlg.h"
#include "SortingListCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(StationeryDockBar, CDockablePane)

	BEGIN_MESSAGE_MAP(StationeryDockBar, CDockablePane)
		ON_WM_SIZE()
		ON_WM_DESTROY()
		ON_NOTIFY(NM_RCLICK, IDC_STATIONERY_LIST, &StationeryDockBar::OnNMRClickStationeryList)
		ON_COMMAND(ID_STATIONERY_APPEND, &StationeryDockBar::OnStationeryAdd)
		ON_UPDATE_COMMAND_UI(ID_STATIONERY_APPEND, &StationeryDockBar::OnUpdateStationeryAdd)
		ON_COMMAND(ID_STATIONERY_DELETE, &StationeryDockBar::OnStationeryDelete)
		ON_UPDATE_COMMAND_UI(ID_STATIONERY_DELETE, &StationeryDockBar::OnUpdateStationeryDelete)
		ON_NOTIFY(NM_DBLCLK, IDC_STATIONERY_LIST, &StationeryDockBar::OnNMDblclkStationeryList)
		
	END_MESSAGE_MAP()    

StationeryDockBar::StationeryDockBar(GroceriesCatalog* pGroceriesCatalog/* = NULL*/)
	: m_pGroceriesCatalog(pGroceriesCatalog)
	, m_pObject(NULL)
// 	, m_bCellFocus(FALSE)
// 	, m_strCellText(_T(""))
// 	, m_nRow(-1)
// 	, m_nColumn(-1)
{
	m_GroceriesControl = NULL;
}


StationeryDockBar::~StationeryDockBar(void)
{
	if(m_GroceriesControl)
		m_GroceriesControl = NULL;
}

StationeryDockBar* StationeryDockBar::CreateDockBar( LPCTSTR lpszWindowName, CWnd* pParentWnd, 
													UINT nDockbarID /*= AFX_IDW_DIALOGBAR*/, 
													UINT nCmdID /*= 0*/, GroceriesCatalog* pGroceriesCatalog,
													DWORD dwStyle /*= WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC |CBRS_HIDE_INPLACE*/ )
{

	StationeryDockBar * pNewDockBar =  new StationeryDockBar(pGroceriesCatalog);

	if(!pNewDockBar->Create(lpszWindowName, pParentWnd, TRUE, MAKEINTRESOURCE(IDD_STATIONERY_DLG), dwStyle, nCmdID))
	{
		delete pNewDockBar;
		pNewDockBar = NULL;
	}

	if(pNewDockBar->GetSafeHwnd())
	{
		pNewDockBar->OnInitDialog();
	}

	return pNewDockBar;
}

void StationeryDockBar::OnDestroy()
{
	__super::OnDestroy();
}

void StationeryDockBar::OnSize( UINT nType, int cx, int cy )
{
	if (IsWindow(m_GroceriesControl->GetSafeHwnd()))
	{
		CRect rc;
		GetClientRect(rc);

		int dx = rc.TopLeft().x;
		int dy = rc.TopLeft().y;
		m_GroceriesControl->SetWindowPos(NULL, dx, dy, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);
	}

	__super::OnSize(nType, cx, cy);
		// m_ListCtrl.SetWindowPos(NULL, dx, dy, cx, cy -30, SWP_NOACTIVATE | SWP_NOZORDER);

	/*	CRect rNew;
		GetDlgItem(IDC_BT_NEW)->GetClientRect(rNew);
		int dNewX = 0;
		int dNewY = rc.Height() - rNew.Height();
		GetDlgItem(IDC_BT_NEW)->SetWindowPos(NULL, dNewX + 2, dNewY -4, cx, cy, SWP_NOSIZE | SWP_NOZORDER );

		CRect rUpdate;
		GetDlgItem(IDC_BT_UPDATE)->GetClientRect(rUpdate);
		int nCenter = rc.Width() /2;
		int dUpdateX = nCenter - rUpdate.Width() /2;
		int dUpdateY = rc.Height() - rUpdate.Height();
		GetDlgItem(IDC_BT_UPDATE)->SetWindowPos(NULL, dUpdateX + 2, dUpdateY -4, cx, cy, SWP_NOSIZE | SWP_NOZORDER );

		CRect rDEL;
		GetDlgItem(IDC_BT_DEL)->GetClientRect(rDEL);
		int dDelX = rc.Width() - rDEL.Width();
		int dDelY = rc.Height() - rDEL.Height();
		GetDlgItem(IDC_BT_DEL)->SetWindowPos(NULL, dDelX + 2, dDelY -4, cx, cy, SWP_NOSIZE | SWP_NOZORDER );*/

}

void StationeryDockBar::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIONERY_LIST, m_ListCtrl);
}

void StationeryDockBar::OnInitDialog()
{
	if(m_GroceriesControl == NULL)
	{
		CRect rect;
		this->GetClientRect(&rect);
		ScreenToClient(&rect);
		m_GroceriesControl = GroceriesControl::GroceriesControlCreate(this, rect, 0);
	}

	m_ListCtrl.ShowWindow(SW_HIDE);
	/*if(m_ListCtrl.GetSafeHwnd())
	{

		CRect rc;
		GetClientRect(rc);

		int dx = rc.TopLeft().x;
		int dy = rc.TopLeft().y;
		int cx = (rc.Width() >= 2 * dx) ? rc.Width() - 2 * dx:0;
		int cy = (rc.Height() >= 2 * dy) ? rc.Height()- 2 * dy:0;
		m_ListCtrl.SetWindowPos(NULL, dx, dy, cx, cy -25, SWP_NOACTIVATE | SWP_NOZORDER);

		DWORD dwStyle = m_ListCtrl.GetExtendedStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		dwStyle |= LVS_EX_GRIDLINES;
		m_ListCtrl.SetExtendedStyle(dwStyle);

		m_ListCtrl.InsertColumn(0, STATIONERY_NUMBER,	LVCFMT_LEFT, 30);
		m_ListCtrl.InsertColumn(1, STATIONERY_NAME,		LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(2, STATIONERY_SPEC,		LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(3, STATIONERY_UP,		LVCFMT_LEFT, 80);
		m_ListCtrl.InsertColumn(4, STATIONERY_QTY,		LVCFMT_LEFT, 80);
		m_ListCtrl.InsertColumn(5, STATIONERY_CMT,		LVCFMT_LEFT, 160);

		UpdateTable();
	}*/
}

void StationeryDockBar::SetObject( CObject* pObject )
{
	if(m_pObject == pObject)
		return;

	m_pObject = pObject;
}


void StationeryDockBar::OnNMRClickStationeryList(NMHDR *pNMHDR, LRESULT *pResult)
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
}


void StationeryDockBar::OnStationeryAdd()
{
	// TODO: Add your command handler code here

	CGroceriesFrame* pFrame = dynamic_cast<CGroceriesFrame*>(GetParentFrame());
	if(pFrame)
	{
		m_pGroceriesCatalog = pFrame->GetRoot();
		CString Text;
		if(NULL != m_pGroceriesCatalog)
		{
			Stationery* pNewStationery = m_pGroceriesCatalog->CreateStationery();
			if(pNewStationery)
			{
				StationeryOrStuffDlg dlg(pNewStationery);

				CString Text;
				Text.Format(_T("New Stationery %d"), pNewStationery->GetID());
				dlg.SetTitle(Text);

				if(IDOK == dlg.DoModal())
				{

				}
				else
				{
					CString strID;
					strID.Format(_T("%d"), pNewStationery->GetID());
					if(!strID.IsEmpty())
						m_pGroceriesCatalog->DeleteStationery(strID);
				}
// 				// ID
// 				Text.Format(_T("%d"), pNewStationery->GetID());
// 				int index = m_ListCtrl.InsertItem(0, Text);
// 
// 				// Name
// 				Text = pNewStationery->GetName();
// 				m_ListCtrl.SetItemText(index, 1, Text);
// 
// 				// Specification
// 				Text = pNewStationery->GetDescription();
// 				m_ListCtrl.SetItemText(index, 2, Text);
// 
// 				// Unit price
// 				Text.Format(_T("%d"), pNewStationery->GetUnitRate());
// 				m_ListCtrl.SetItemText(index, 3, Text);
// 
// 				// Quantity
// 				Text.Format(_T("%d"), pNewStationery->GetUnitRate());
// 				m_ListCtrl.SetItemText(index, 3, Text);
// 
// 				// Comment
// 				Text = pNewStationery->GetComment();
// 				m_ListCtrl.SetItemText(index, 4, Text);

				UpdateTable();
			}
		}
	}
}


void StationeryDockBar::OnUpdateStationeryAdd(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}


void StationeryDockBar::OnStationeryDelete()
{
	// TODO: Add your command handler code here

	int nPos = m_ListCtrl.GetSelectionMark();
	if(nPos != -1)
	{
		Stationery* pStationery	= dynamic_cast<Stationery*>( (CObject*)m_ListCtrl.GetItemData(nPos));
		if(pStationery)
			pStationery->Delete();

		m_ListCtrl.DeleteItem(nPos);
	}
}


void StationeryDockBar::OnUpdateStationeryDelete(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = TRUE;

	int nPos = m_ListCtrl.GetSelectionMark();
	if(nPos == -1)
		bEnable = FALSE;

	pCmdUI->Enable(bEnable);
}

void StationeryDockBar::UpdateTable()
{
	int count = 0;
	if(m_pGroceriesCatalog)
	{

		count = m_pGroceriesCatalog->GetStationeryCount();
		m_ListCtrl.DeleteAllItems();

		for (int i=0; i < count; i++) 
		{
			Stationery* pNewStationery = m_pGroceriesCatalog->GetStationeryAt(i);
			if(pNewStationery)
			{
				CString Text;
				// ID
				Text.Format(_T("%d"), pNewStationery->GetID());
				int index = m_ListCtrl.InsertItem(i, Text);

				// Name
				Text = pNewStationery->GetName();
				m_ListCtrl.SetItemText(index, 1, Text);

				// Specification
				Text = pNewStationery->GetDescription();
				m_ListCtrl.SetItemText(index, 2, Text);

				// Unit price
				Text.Format(_T("%.2f"), pNewStationery->GetUnitRate());
				m_ListCtrl.SetItemText(index, 3, Text);

				// Quantity
				Text.Format(_T("%.2f"), pNewStationery->GetQuantity());
				m_ListCtrl.SetItemText(index, 4, Text);

				// Comment
				Text = pNewStationery->GetComment();
				m_ListCtrl.SetItemText(index, 5, Text);

				m_ListCtrl.SetItemData(i, (DWORD_PTR)pNewStationery);
			}
		}

	}
}


// void StationeryDockBar::OnNMDblclkStationeryList(NMHDR *pNMHDR, LRESULT *pResult)
// {
// 	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
// 	// TODO: Add your control notification handler code here
// 
// 	CRect rc;
// 	HWND listWnd = m_ListCtrl.GetSafeHwnd();
// 
// 	NMLISTVIEW* pNmList = (NMLISTVIEW*)pNMHDR;
// 
// 	m_nRow = pNmList->iItem;
// 	m_nColumn = pNmList->iSubItem;
// 
// 	if(m_nRow < 0 || m_nColumn < 0) return;
// 
// 	m_ListCtrl.GetSubItemRect(m_nRow, m_nColumn, LVIR_LABEL,rc);
// 
// 	m_strCellText = m_ListCtrl.GetItemText(m_nRow, m_nColumn);
// 	OnKillFocus(FromHandle(listWnd));
// 	
// 	m_pEdit =  new CEdit;
// 	m_pEdit->Create(WS_CHILD,rc,FromHandle(listWnd), 1);
// 	m_pEdit->ShowWindow(SW_SHOW);
// 	m_pEdit->SetFocus();
// 	m_bCellFocus = TRUE;
// 
// 	*pResult = 0;
// }
// 
// 
// void StationeryDockBar::OnNMSetfocusStationeryList(NMHDR *pNMHDR, LRESULT *pResult)
// {
// 	// TODO: Add your control notification handler code here
// 
// 	if(m_bCellFocus)
// 	{
// 		CString str;
// 		m_pEdit->GetWindowText(str);
// 		m_pEdit->ShowWindow(SW_HIDE);
// 
// 		HWND hEdit = m_pEdit->GetSafeHwnd();
// 
// 		OnKillFocus(FromHandle(hEdit));
// 		if(m_pEdit)
// 		{
// 			delete m_pEdit;
// 			m_pEdit = NULL;
// 		}
// 		
// 		str = (str != m_strCellText && !str.IsEmpty())? str : m_strCellText;
// 
// 		m_ListCtrl.SetItemText(m_nRow, m_nColumn, str);
// 	}
// 
// 	*pResult = 0;
// }


void StationeryDockBar::OnBnClickedBtNew()
{
	// TODO: Add your control notification handler code here
}


void StationeryDockBar::OnBnClickedBtUpdate()
{
	// TODO: Add your control notification handler code here
}


void StationeryDockBar::OnBnClickedBtDel()
{
	// TODO: Add your control notification handler code here
}

void StationeryDockBar::OnStationeryUpdate()
{

}

void StationeryDockBar::OnUpdateStationeryUpdate( CCmdUI *pCmdUI )
{
	pCmdUI->Enable(TRUE);
}


void StationeryDockBar::OnNMDblclkStationeryList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	int nRow = pNMItemActivate->iItem;
	if(nRow >= 0)
	{
		Stationery* pStationery	= dynamic_cast<Stationery*>( (CObject*)m_ListCtrl.GetItemData(nRow));
		if(pStationery)
		{
			CString strName = pStationery->GetName();
			CString strDesc = pStationery->GetDescription();
			double dUR = pStationery->GetUnitRate();
			double dQty = pStationery->GetQuantity();
			CString strComment = pStationery->GetComment();

			CString Text;
			StationeryOrStuffDlg dlg(pStationery);
			Text.Format(_T("Update Stationery %d"), pStationery->GetID());
			dlg.SetTitle(Text);

			if(IDOK == dlg.DoModal())
			{

			}
			else
			{
				pStationery->SetName(strName);
				pStationery->SetDescription(strDesc);
				pStationery->SetUnitRate(dUR);
				pStationery->SetQuantity(dQty);
				pStationery->SetComment(strComment);
			}

			UpdateTable();
		}
	}

	*pResult = 0;
}


bool StationeryDockBar::IsActive()
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

CSortingListCtrl* StationeryDockBar::getListControl()
{
	return &m_ListCtrl;

}
void StationeryDockBar::SetCatalog( GroceriesCatalog* pCat )
{
	if(m_pGroceriesCatalog != pCat)
		m_pGroceriesCatalog = pCat ;
}



