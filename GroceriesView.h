
// GroceriesView.h : interface of the CGroceriesView class
//

#pragma once
#include <afxcview.h>
#include <vector>
#include "GroceriesDoc.h"

class GroceriesCatalog;
class GroceriesDoc;

using namespace std;

class CGroceriesView : public CFormView
{
protected: // create from serialization only
	CGroceriesView();
	DECLARE_DYNCREATE(CGroceriesView)

// Attributes
public:
	CGroceriesDoc* GetDocument() const;

// Operations
public:
	void CreateImageList( CImageList &il, UINT nBitmap4Bit, UINT nBitmap24Bit, 
		int cx = 16, int cy = 16, 
		int nInitial = 2, int nGrow = 1, 
		COLORREF crMask = RGB( 0, 128, 128 ) );

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGroceriesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif



	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	CTreeCtrl& GetTreeCtrl() {return m_Tree;};

public:
	virtual void OnInitialUpdate();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNewGroup();
	afx_msg void OnUpdateNewGroup(CCmdUI* pCmdUI);

	afx_msg void OnNewSubGroup();
	afx_msg void OnUpdateNewSubGroup(CCmdUI* pCmdUI);

	afx_msg void OnNewItem();
	afx_msg void OnUpdateItem(CCmdUI* pCmdUI);

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnDelete();
	afx_msg void OnUpdateDelete(CCmdUI *pCmdUI);

	void OnRefresh();
	BOOL CanDelete();
protected:
	//
	// Image list
	// 
	CImageList m_ImageList;

	//
	// Root node
	// 
	GroceriesCatalog* m_pCatalog;

	//
	// Tree control
	// 
	CTreeCtrl m_Tree;

	//
	// socket
	// 
	SOCKET m_sk;
public:
//	afx_msg void OnNMTVStateImageChanging(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTvnSelchangedGroTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickGroTree(NMHDR *pNMHDR, LRESULT *pResult);
	void SetSelectedObj( CObject* pObject );
	afx_msg void OnActionImportstationery();
	afx_msg void OnUpdateActionImportstationery(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in GroceriesView.cpp
inline CGroceriesDoc* CGroceriesView::GetDocument() const
   { return reinterpret_cast<CGroceriesDoc*>(m_pDocument); }
#endif

