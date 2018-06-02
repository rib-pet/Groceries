#ifndef _STATIONERYDOCKBAR_H_
#define _STATIONERYDOCKBAR_H_

#pragma once
#include "afxpane.h"
#include "Resource.h"
#include "SortingListCtrl.h"
#include "GroceriesControl.h"

using namespace Groceries;

class GroceriesCatalog;


class StationeryDockBar : public CPaneDialog
{
	DECLARE_DYNCREATE(StationeryDockBar)
	enum { IDD = IDD_STATIONERY_DLG };

protected:
	StationeryDockBar(GroceriesCatalog* pGroceriesCatalog = NULL);

public:
	virtual ~StationeryDockBar(void);
	void SetObject( CObject* pObject );
	void SetCatalog( GroceriesCatalog* pCat );

public:
	static StationeryDockBar* CreateDockBar(
		LPCTSTR lpszWindowName,
		CWnd* pParentWnd,
		UINT nDockbarID = AFX_IDW_DIALOGBAR,
		UINT nCmdID = 0,
		GroceriesCatalog* pGroceriesCatalog = NULL,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS 
		| WS_CLIPCHILDREN	|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
		|CBRS_HIDE_INPLACE);

	DECLARE_MESSAGE_MAP()

	// virtual override methods
protected:
	virtual void OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	// root
	GroceriesCatalog* m_pGroceriesCatalog;

	// List control
	CSortingListCtrl m_ListCtrl;

	CObject* m_pObject;

	GroceriesControl* m_GroceriesControl;

// 	BOOL m_bCellFocus;
// 	CEdit* m_pEdit;
// 	CString m_strCellText;
// 
// 	int m_nRow;
// 	int m_nColumn;

public:
	afx_msg void OnNMRClickStationeryList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStationeryAdd();
	afx_msg void OnUpdateStationeryAdd(CCmdUI *pCmdUI);
	afx_msg void OnStationeryDelete();
	afx_msg void OnUpdateStationeryDelete(CCmdUI *pCmdUI);

	afx_msg void OnStationeryUpdate();
	afx_msg void OnUpdateStationeryUpdate(CCmdUI *pCmdUI);
	void InitializeData();
	void UpdateTable();
	bool IsActive();
	CSortingListCtrl* getListControl();

	// 	afx_msg void OnNMDblclkStationeryList(NMHDR *pNMHDR, LRESULT *pResult);
// 	afx_msg void OnNMSetfocusStationeryList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtNew();
	afx_msg void OnBnClickedBtUpdate();
	afx_msg void OnBnClickedBtDel();
	afx_msg void OnNMDblclkStationeryList(NMHDR *pNMHDR, LRESULT *pResult);


};


#endif //_STATIONERYDOCKBAR_H_
