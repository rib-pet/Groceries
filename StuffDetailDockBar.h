#ifndef _STUFFDETAILDOCKBAR_H_
#define _STUFFDETAILDOCKBAR_H_

#pragma once
#include "afxpanedialog.h"
#include "Resource.h"
#include "SortingListCtrl.h"

class StuffDetailDockBar : public CPaneDialog
{
	DECLARE_DYNCREATE(StuffDetailDockBar)
	enum { IDD = IDD_STUFF_DLG };

	StuffDetailDockBar(void);
public:
	virtual ~StuffDetailDockBar(void);

public:
	static StuffDetailDockBar* CreateDockBar(
		LPCTSTR lpszWindowName,
		CWnd* pParentWnd,
		UINT nDockbarID = AFX_IDW_DIALOGBAR,
		UINT nCmdID = 0,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS 
		| WS_CLIPCHILDREN	|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
		|CBRS_HIDE_INPLACE);

	void SetObject(CObject* pObject);
	void DoDataExchange( CDataExchange* pDX );

	DECLARE_MESSAGE_MAP()

protected:
	void OnInitDialog();

protected:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRClickStuffList(NMHDR *pNMHDR, LRESULT *pResult);

protected:

	CObject* m_pObject;

	CSortingListCtrl m_ListCtrl;
public:
	afx_msg void OnStationeryAppend();
	afx_msg void OnUpdateStationeryAppend(CCmdUI *pCmdUI);
	afx_msg void OnStationeryDelete();
	afx_msg void OnUpdateStationeryDelete(CCmdUI *pCmdUI);
	afx_msg void OnLvnColumnclickStuffList(NMHDR *pNMHDR, LRESULT *pResult);
	bool IsActive();
	void UpdateTableCtrl();
};


#endif // _STUFFDETAILDOCKBAR_H_
