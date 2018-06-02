#pragma once
#include "afxwin.h"

class Stationery;
class Stuff;

// StationeryOrStuffDlg dialog

class StationeryOrStuffDlg : public CDialogEx
{
	DECLARE_DYNAMIC(StationeryOrStuffDlg)

public:
	StationeryOrStuffDlg(Stationery* pStationery,CWnd* pParent = NULL);   // standard constructor
	StationeryOrStuffDlg(Stuff* pStuff,CWnd* pParent = NULL);   // standard constructor
	virtual ~StationeryOrStuffDlg();

// Dialog Data
	enum { IDD = IDD_STAT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctrlID;
	CString m_ctrlName;
	CString m_strSpecification;
	double m_dUnitPrice;
	double m_dQty;
	CString m_strComment;
	Stationery* m_pStationery;

	CString m_strTitle;
	Stuff* m_pStuff;

protected:
	afx_msg void OnBnClickedDiscard();
	afx_msg void OnBnClickedSave();
public:
	virtual BOOL OnInitDialog();
	void SetTitle(const CString& Text );
	void UpdateControls( BOOL bReadOnly );
	afx_msg void OnEnKillfocusEditId();
	void GetData();
};
