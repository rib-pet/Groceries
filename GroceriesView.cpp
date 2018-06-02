
// GroceriesView.cpp : implementation of the CGroceriesView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Groceries.h"
#endif

#include "io.h"

#pragma warning(disable:4217)

#include "GroceriesView.h"
#include "Groceries\GroceriesCatalog.h"
#include "Groceries\GroceriesGroup.h"
#include "CGroceriesFrame.h"
#include "GroceriesBasicPage.h"
#include "Groceries\GroceriesItem.h"
#include "StationeryDockBar.h"
#include "Resource.h"
#include "StuffDetailDockBar.h"
#include "GroerierFinder.h"
#include "..\Groceries\GroceiersImport.h"
#include ".\Groceries\GroceriesVisitor.h"
#include <windows.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGroceriesView

IMPLEMENT_DYNCREATE(CGroceriesView, CFormView)

BEGIN_MESSAGE_MAP(CGroceriesView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_NEW_GROUP, &CGroceriesView::OnNewGroup)
	ON_UPDATE_COMMAND_UI(ID_NEW_GROUP, &CGroceriesView::OnUpdateNewGroup)

	ON_COMMAND(ID_NEW_SUB_GROUP, &CGroceriesView::OnNewSubGroup)
	ON_UPDATE_COMMAND_UI(ID_NEW_SUB_GROUP, &CGroceriesView::OnUpdateNewSubGroup)

	ON_COMMAND(ID_NEW_ITEM, &CGroceriesView::OnNewItem)
	ON_UPDATE_COMMAND_UI(ID_NEW_ITEM, &CGroceriesView::OnUpdateItem)

	ON_COMMAND(ID_DELETE, &CGroceriesView::OnDelete)
	ON_UPDATE_COMMAND_UI(ID_DELETE, &CGroceriesView::OnUpdateDelete)

	//ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK, &CGroceriesView::OnNMClick)
	ON_WM_HSCROLL()
	ON_NOTIFY(TVN_SELCHANGED, IDC_GRO_TREE, &CGroceriesView::OnTvnSelchangedGroTree)
	ON_NOTIFY(NM_RCLICK, IDC_GRO_TREE, &CGroceriesView::OnNMRClickGroTree)
	ON_COMMAND(ID_ACTION_IMPORTSTATIONERY, &CGroceriesView::OnActionImportstationery)
	ON_UPDATE_COMMAND_UI(ID_ACTION_IMPORTSTATIONERY, &CGroceriesView::OnUpdateActionImportstationery)
END_MESSAGE_MAP()

// CGroceriesView construction/destruction

CGroceriesView::CGroceriesView()
	: CFormView(IDD_GRO_DLG)
	, m_pCatalog(NULL)
{
	// TODO: add construction code here

}

CGroceriesView::~CGroceriesView()
{
}

BOOL CGroceriesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

// CGroceriesView drawing

void CGroceriesView::OnDraw(CDC* /*pDC*/)
{
	CGroceriesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CGroceriesView printing

BOOL CGroceriesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGroceriesView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGroceriesView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGroceriesView diagnostics

#ifdef _DEBUG
void CGroceriesView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGroceriesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CGroceriesDoc* CGroceriesView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGroceriesDoc)));
	return (CGroceriesDoc*)m_pDocument;
}

void CGroceriesView::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_GRO_TREE, m_Tree);
}

#endif //_DEBUG

void CGroceriesView::OnSize( UINT nType, int cx, int cy )
{
	__super::OnSize(nType,cx,cy);

	if (IsWindow(m_Tree.m_hWnd))
	{
		CRect rc;
		GetClientRect(rc);

		int dx = rc.TopLeft().x;
		int dy = rc.TopLeft().y;
		int cx = (rc.Width() >= 2 * dx) ? rc.Width() - 2 * dx:0;
		int cy = (rc.Height() >= 2 * dy) ? rc.Height()- 2 * dy:0;
		m_Tree.SetWindowPos(NULL, dx, dy, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void CGroceriesView::CreateImageList( CImageList &il, UINT nBitmap4Bit, UINT nBitmap24Bit, int cx, int cy, int nInitial, int nGrow, COLORREF crMask)
{
	HDC iDCHandle = ::GetDC(m_hWnd);
	int nBitsPerPixel = GetDeviceCaps(iDCHandle, BITSPIXEL);
	::ReleaseDC(NULL, iDCHandle);

	UINT nFlags = 0;

	if (nBitsPerPixel > 8 && nBitsPerPixel < 24)
	{
		OSVERSIONINFO info;
		memset(&info, 0, sizeof(OSVERSIONINFO));
		info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	//	GetVersionEx(&info);

		if (info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) // Win9x/ME
			nFlags = ILC_COLOR;
		else
			nFlags = ILC_COLOR24;
	} 
	else if (nBitsPerPixel >= 24)
	{
		nFlags = ILC_COLOR24;
	} else if (nBitsPerPixel == 8)
	{
		nFlags = ILC_COLOR8;
	} else {
		nFlags = ILC_COLOR;
	} //endif

	il.Create(cx, cy, nFlags | ILC_MASK, nInitial, nGrow);

	if ( nBitmap24Bit != 0 || nBitmap4Bit != 0 )
	{
		CBitmap bmp;
		bmp.LoadBitmap((nFlags == ILC_COLOR24) ? nBitmap24Bit : nBitmap4Bit);
		il.Add(&bmp, crMask);
	}
}


void CGroceriesView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	if (IsWindow(GetTreeCtrl().m_hWnd))
	{
		//
		// Initialize Image
		// 
		if(!m_ImageList.GetSafeHandle())
		{
			CreateImageList(m_ImageList, IDB_GROS, IDB_GROS);
			GetTreeCtrl().SetImageList(&m_ImageList, LVSIL_NORMAL);
			//GetTreeCtrl().SetExtendedStyle(TVS_EX_DOUBLEBUFFER , 0);
		}

		

		if (IsWindow(m_Tree.m_hWnd))
		{
			CRect rc;
			GetClientRect(rc);

			int dx = rc.TopLeft().x;
			int dy = rc.TopLeft().y;
			int cx = (rc.Width() >= 2 * dx) ? rc.Width() - 2 * dx:0;
			int cy = (rc.Height() >= 2 * dy) ? rc.Height()- 2 * dy:0;
			m_Tree.SetWindowPos(NULL, dx, dy, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);
		}

		CGroceriesDoc* pDoc = GetDocument();
		if(pDoc)
		{
			m_pCatalog = pDoc->GetRoot();
			if(m_pCatalog)
			{
				TVINSERTSTRUCT tvInsert;
				tvInsert.hParent = NULL;
				tvInsert.hInsertAfter = NULL;
				tvInsert.item.mask = TVIF_TEXT;
				CString strName = m_pCatalog->GetName();
				
				tvInsert.item.pszText = strName.GetBuffer();
				tvInsert.item.iSelectedImage = 1;

				//
				// Initialize Tree control
				// 
				HTREEITEM rootItem = GetTreeCtrl().InsertItem(&tvInsert);
				GetTreeCtrl().SetItemData(rootItem, (DWORD_PTR)m_pCatalog);

				CGroceriesFrame* pFrame = dynamic_cast<CGroceriesFrame*>(GetParentFrame());
				if(pFrame)
				{
					GroceriesBasicPage* pPage = pFrame->GetPage();
					if(pPage)
						pPage->SetObject(m_pCatalog);
				}
			}
		}
	}
}

void CGroceriesView::OnNewGroup()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	DWORD_PTR pdata = GetTreeCtrl().GetItemData(item);

	CString str = GetTreeCtrl().GetItemText(item);

	if(pdata )
	{
		GroceriesGroup* pNewGroup = NULL;

		GroceriesCatalog* pCat = dynamic_cast<GroceriesCatalog*>((CObject*)pdata);
		GroceriesGroup* pGroup = dynamic_cast<GroceriesGroup*>((CObject*)pdata);

		if(pCat)
		{
			pNewGroup = pCat->CreateGroup();
			if(pNewGroup)
			{
				CString strName = pNewGroup->GetName();
				HTREEITEM subItem = GetTreeCtrl().InsertItem(strName,item);
				GetTreeCtrl().SetItemImage(subItem, 1,1);
				GetTreeCtrl().SetItemData(subItem, (DWORD_PTR)pNewGroup);
				GetTreeCtrl().Expand(item, TVE_EXPAND );

				GetTreeCtrl().SelectItem(subItem);
				SetSelectedObj(pNewGroup);
			}
		}
		else if(pGroup)
		{
			CObject* pParentObject = pGroup->GetParentObject();
			if(pParentObject)
			{
				GroceriesCatalog* pCatalog = dynamic_cast<GroceriesCatalog*>(pParentObject);
				GroceriesGroup* pTmpGroup = dynamic_cast<GroceriesGroup*>(pParentObject);

				if(pCatalog)
					pNewGroup = pCatalog->CreateGroup();
				else if(pTmpGroup)
					pNewGroup = pTmpGroup->CreateGroup();
					
				if(pNewGroup)
				{
					CString strName = pNewGroup->GetName();
					HTREEITEM parentItem = GetTreeCtrl().GetParentItem(item);
					if(parentItem)
					{
						HTREEITEM subItem = GetTreeCtrl().InsertItem(strName,parentItem);
						GetTreeCtrl().SetItemImage(subItem, 1,1);
						GetTreeCtrl().SetItemData(subItem, (DWORD_PTR)pNewGroup);
						GetTreeCtrl().Expand(item, TVE_EXPAND );

						GetTreeCtrl().SelectItem(subItem);
						SetSelectedObj(pNewGroup);
					}
				}
			}
		}
	}
}

void CGroceriesView::OnUpdateNewGroup( CCmdUI* pCmdUI )
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	DWORD_PTR pdata = GetTreeCtrl().GetItemData(item);

	BOOL bEnable = FALSE;

	if(pdata)
	{
		GroceriesCatalog* pCat = dynamic_cast<GroceriesCatalog*>((CObject*)pdata);
		GroceriesGroup* pGroup = dynamic_cast<GroceriesGroup*>((CObject*)pdata);

		if(pCat || pGroup)
		{
			bEnable = TRUE;
		}
	}

	if(pCmdUI)
		pCmdUI->Enable(bEnable);
}


void CGroceriesView::OnRButtonDown( UINT nFlags, CPoint p )
{
	CPoint point;
	GetCursorPos(&point);

	CPoint pt = point;
	GetTreeCtrl().ScreenToClient(&pt);

	HTREEITEM item;
	UINT flag = TVHT_ONITEM;

	item = GetTreeCtrl().HitTest(pt, &flag);

	if(item != NULL)
	{
		GetTreeCtrl().SelectItem(item);

		CMenu menu;
		menu.LoadMenu(IDR_MAINFRAME);

		menu.GetSubMenu(4)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
	}
}

void CGroceriesView::OnRefresh()
{
	CGroceriesDoc* pDoc = GetDocument();
	if(pDoc)
	{
		pDoc->SendToService();
	}
}


void CGroceriesView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CPoint point;
	GetCursorPos(&point);

	CPoint pt = point;
	GetTreeCtrl().ScreenToClient(&pt);

	HTREEITEM item;
	UINT flag = TVHT_ONITEM;

	item = GetTreeCtrl().HitTest(pt, &flag);

	if(item != NULL)
	{
		GetTreeCtrl().SelectItem(item);
		DWORD_PTR pdata = GetTreeCtrl().GetItemData(item);

		if(pdata)
		{
			CGroceriesFrame* pFrame = dynamic_cast<CGroceriesFrame*>(GetParentFrame());
			CObject* pObject = (CObject*)(pdata);
			if(pObject && pFrame)
			{
				GroceriesBasicPage* pPage = pFrame->GetPage();
				StationeryDockBar* pDockBar = pFrame->GetStationeryDockBar();

				if(pPage)
				{
					pPage->SetObject(pObject);
				}

				if(pDockBar)
				{
					pDockBar->SetObject(pObject);
				}
			}
		}
	}
}

void CGroceriesView::OnNewSubGroup()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	DWORD_PTR pdata = GetTreeCtrl().GetItemData(item);

	CString str = GetTreeCtrl().GetItemText(item);

	if(pdata )
	{
		GroceriesGroup* pNewGroup = NULL;

		GroceriesCatalog* pCat = dynamic_cast<GroceriesCatalog*>((CObject*)pdata);
		GroceriesGroup* pGroup = dynamic_cast<GroceriesGroup*>((CObject*)pdata);

		if(pCat)
			pNewGroup = pCat->CreateGroup();
		else if(pGroup)
			pNewGroup = pGroup->CreateGroup();

		if(pNewGroup)
		{
			CString strName = pNewGroup->GetName();
			HTREEITEM subItem = GetTreeCtrl().InsertItem(strName,item);
			GetTreeCtrl().SetItemImage(subItem, 1,1);
			GetTreeCtrl().SetItemData(subItem, (DWORD_PTR)pNewGroup);
			GetTreeCtrl().Expand(item, TVE_EXPAND );

			GetTreeCtrl().SelectItem(subItem);
			SetSelectedObj(pNewGroup);
		}
	}
}

void CGroceriesView::OnUpdateNewSubGroup( CCmdUI* pCmdUI )
{
	BOOL bEnable = FALSE;

	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	DWORD_PTR pdata = GetTreeCtrl().GetItemData(item);

	CString str = GetTreeCtrl().GetItemText(item);

	if(pdata )
	{
		GroceriesGroup* pNewGroup = NULL;

		GroceriesCatalog* pCat = dynamic_cast<GroceriesCatalog*>((CObject*)pdata);
		GroceriesGroup* pGroup = dynamic_cast<GroceriesGroup*>((CObject*)pdata);

		if(pCat || pGroup)
			bEnable = TRUE;
	}

	pCmdUI->Enable(bEnable);
}

void CGroceriesView::OnNewItem()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	DWORD_PTR pdata = GetTreeCtrl().GetItemData(item);

	CString str = GetTreeCtrl().GetItemText(item);

	if(pdata )
	{
		GroceriesItem* pNewItem = NULL;
		GroceriesGroup* pGroup = dynamic_cast<GroceriesGroup*>((CObject*)pdata);

		if(pGroup)
			pNewItem = pGroup->CreateItem();

		if(pNewItem)
		{
			CString strName = pNewItem->GetName();
			HTREEITEM subItem = GetTreeCtrl().InsertItem(strName,item);
			GetTreeCtrl().SetItemImage(subItem, 2,2);

			GetTreeCtrl().SetItemData(subItem, (DWORD_PTR)pNewItem);
			GetTreeCtrl().Expand(item, TVE_EXPAND );

			GetTreeCtrl().SelectItem(subItem);

			SetSelectedObj(pNewItem);
		}
	}
}

void CGroceriesView::OnUpdateItem( CCmdUI* pCmdUI )
{
	BOOL bEnable = TRUE;

	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	DWORD_PTR pdata = GetTreeCtrl().GetItemData(item);

	CString str = GetTreeCtrl().GetItemText(item);

	if(pdata )
	{
		GroceriesGroup* pNewGroup = NULL;

		GroceriesCatalog* pCat = dynamic_cast<GroceriesCatalog*>((CObject*)pdata);
		GroceriesGroup* pGroup = dynamic_cast<GroceriesGroup*>((CObject*)pdata);
		GroceriesItem* pItem = dynamic_cast<GroceriesItem*>((CObject*)pdata);

		if(pCat || pItem)
			bEnable = FALSE;
	}

	pCmdUI->Enable(bEnable);
}

BOOL CGroceriesView::CanDelete()
{
	BOOL bEnable = TRUE;
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	DWORD_PTR pdata = GetTreeCtrl().GetItemData(item);

	CString str = GetTreeCtrl().GetItemText(item);

	if(pdata)
	{
		GroceriesCatalog* pCat = dynamic_cast<GroceriesCatalog*>((CObject*)pdata);
		if(pCat)
			bEnable = FALSE;
	}

	return bEnable;
}

void CGroceriesView::OnDelete()
{
	// TODO: Add your command handler code here


	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	DWORD_PTR pdata = GetTreeCtrl().GetItemData(item);

	CString str = GetTreeCtrl().GetItemText(item);

	if(pdata)
	{
		GroceriesGroup* pGroup = dynamic_cast<GroceriesGroup*>((CObject*)pdata);
		GroceriesItem* pItem = dynamic_cast<GroceriesItem*>((CObject*)pdata);

		if(pGroup || pItem)
		{
			CString sMsg ;
			AfxFormatString1(sMsg, IDS_DELETE_ERROR, str);
			if(IDYES == AfxMessageBox(sMsg, MB_YESNO))
			{
				if(pGroup)
					pGroup->Delete();
				else if(pItem)
					pItem->Delete();

				// delete item
				GetTreeCtrl().DeleteItem(item);
			}
		}
	}
}


void CGroceriesView::OnUpdateDelete(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	BOOL bEnable = FALSE;
	bEnable = CanDelete();

	pCmdUI->Enable(bEnable);
}


void CGroceriesView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CGroceriesView::OnTvnSelchangedGroTree(NMHDR *, LRESULT *)
{
	CPoint point;
	GetCursorPos(&point);

	CPoint pt = point;
	GetTreeCtrl().ScreenToClient(&pt);

	HTREEITEM item;
	UINT flag = TVHT_ONITEM;

	item = GetTreeCtrl().HitTest(pt, &flag);

	if(item != NULL)
	{
		GetTreeCtrl().SelectItem(item);
		DWORD_PTR pdata = GetTreeCtrl().GetItemData(item);

		if(pdata)
		{
			CGroceriesFrame* pFrame = dynamic_cast<CGroceriesFrame*>(GetParentFrame());
			CObject* pObject = (CObject*)(pdata);
			if(pObject && pFrame)
			{
				GroceriesBasicPage* pPage = pFrame->GetPage();
				if(pPage)
					pPage->SetObject(pObject);

				StuffDetailDockBar* pDetailDockBar = pFrame->GetStuffDetailDockBar();
				if(pDetailDockBar)
					pDetailDockBar->SetObject(pObject);
			}
		}
	}
}


void CGroceriesView::OnNMRClickGroTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CPoint point;
	GetCursorPos(&point);

	CPoint pt = point;
	GetTreeCtrl().ScreenToClient(&pt);


	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MAINFRAME));

	CMenu* pPopup = menu.GetSubMenu(4);
	CWnd* pWndPopupOwner = this;
	pWndPopupOwner = pWndPopupOwner->GetParent();
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);

}

void CGroceriesView::SetSelectedObj( CObject* pObject )
{
	CGroceriesFrame* pFrame = dynamic_cast<CGroceriesFrame*>(GetParentFrame());
	if(pFrame && pObject)
	{
		StuffDetailDockBar* pDetailDockBar = pFrame->GetStuffDetailDockBar();
		if(pDetailDockBar)
			pDetailDockBar->SetObject(pObject);
	}
}


void CGroceriesView::OnActionImportstationery()
{
	CGroceriesDoc* pDoc = GetDocument();
	if(pDoc)
	{
		m_pCatalog = pDoc->GetRoot();
		if(m_pCatalog)
		{
			//import data from ".xls" file
			
			WCHAR filepath[MAX_PATH] = {0x0};
			//GetModuleFileName(NULL,filepath,sizeof(filepath));
			//PathRemoveFileSpec(filepath);
			GetCurrentDirectory(sizeof(filepath), filepath);
			CString pathOfExcel(filepath);
			pathOfExcel += L"\\document\\stationery.xls";

			if ( _waccess(pathOfExcel.operator LPCTSTR(),0) == -1 )
			{
				return ;
			}

			GroceriesImportWrapper importdata(pathOfExcel,m_pCatalog);
			int iRet = importdata.ReadExcel();



		}
	}


}


void CGroceriesView::OnUpdateActionImportstationery(CCmdUI *pCmdUI)
{

}
