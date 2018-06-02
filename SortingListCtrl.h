#pragma once
#include "afxcmn.h"
class CSortingListCtrl :
	public CListCtrl
{
public:
	CSortingListCtrl(void);
	~CSortingListCtrl(void);

public:
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);

public:
	int static CALLBACK ListCtrlCompare(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort);

public:
	BOOL m_bSortOrder;
	int m_nSelCol;
	DECLARE_MESSAGE_MAP()

};

