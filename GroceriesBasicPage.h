#ifndef _GROCERIESBASICPAGE_H_
#define _GROCERIESBASICPAGE_H_

#pragma once
#include "afxpanedialog.h"
#include "Resource.h"

class GroceriesBasicPage : public CPaneDialog
{
 	DECLARE_DYNCREATE(GroceriesBasicPage)
 	enum { IDD = IDD_GRO_BASIC_PAGE };

public:
	virtual ~GroceriesBasicPage(void);
protected:
	GroceriesBasicPage(void);

	virtual void DoDataExchange(CDataExchange* pDX);

public:
	static GroceriesBasicPage* CreateDockBar(
		LPCTSTR lpszWindowName,
		CWnd* pParentWnd,
		UINT nDockbarID = AFX_IDW_DIALOGBAR,
		UINT nCmdID = 0,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS 
		| WS_CLIPCHILDREN	|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
		|CBRS_HIDE_INPLACE);

	void SetObject(CObject* pObject);

	DECLARE_MESSAGE_MAP()

protected:

	CObject* m_pObject;

	CEdit m_ctrKey;
	CEdit m_ctrDesc;
public:
	afx_msg void OnEnKillfocusKey();
	afx_msg void OnEnKillfocusDesc();
	bool IsKeyUnique( CObject* m_pObject, CString strKey );
	bool IsDescUnique( CObject* m_pObject, CString strDesc );
	afx_msg void OnSize(UINT nType, int cx, int cy);
	bool IsActive();
};

#endif //_GROCERIESBASICPAGE_H_
