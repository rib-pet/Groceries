// StationeryOrStuffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Groceries.h"
#include "StationeryOrStuffDlg.h"
#include "afxdialogex.h"
#include "Groceries\Stationery.h"
#include "Groceries\Stuff.h"
#include "CGroceriesFrame.h"
#include "Groceries\GroceriesCatalog.h"
#include "Groceries\GroceriesItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// StationeryOrStuffDlg dialog

IMPLEMENT_DYNAMIC(StationeryOrStuffDlg, CDialogEx)

StationeryOrStuffDlg::StationeryOrStuffDlg( Stationery* pStationery, CWnd* pParent /*=NULL*/)
	: CDialogEx(StationeryOrStuffDlg::IDD, pParent)
	, m_ctrlName(_T(""))
	, m_strSpecification(_T(""))
	, m_dUnitPrice(0)
	, m_dQty(0)
	, m_strComment(_T(""))
	, m_pStationery(pStationery)
	, m_strTitle(_T(""))
{

}

StationeryOrStuffDlg::StationeryOrStuffDlg(Stuff* pStuff,CWnd* pParent)
	: CDialogEx(StationeryOrStuffDlg::IDD, pParent)
	, m_ctrlName(_T(""))
	, m_strSpecification(_T(""))
	, m_dUnitPrice(0)
	, m_dQty(0)
	, m_strComment(_T(""))
	, m_pStationery(NULL)
	, m_pStuff(pStuff)
	, m_strTitle(_T(""))
{

}


StationeryOrStuffDlg::~StationeryOrStuffDlg()
{
}

void StationeryOrStuffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ID, m_ctrlID);
	DDX_Text(pDX, IDC_EDIT_NAME, m_ctrlName);
	DDV_MaxChars(pDX, m_ctrlName, 20);
	DDX_Text(pDX, IDC_EDIT_DESC, m_strSpecification);
	DDV_MaxChars(pDX, m_strSpecification, 40);
	DDX_Text(pDX, IDC_EDIT_UP, m_dUnitPrice);
	DDX_Text(pDX, IDC_EDIT_QTY, m_dQty);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
	DDV_MaxChars(pDX, m_strComment, 50);
}


BEGIN_MESSAGE_MAP(StationeryOrStuffDlg, CDialogEx)
	ON_BN_CLICKED(IDC_DISCARD, &StationeryOrStuffDlg::OnBnClickedDiscard)
	ON_BN_CLICKED(IDC_SAVE, &StationeryOrStuffDlg::OnBnClickedSave)
	ON_EN_KILLFOCUS(IDC_EDIT_ID, &StationeryOrStuffDlg::OnEnKillfocusEditId)
END_MESSAGE_MAP()


// StationeryOrStuffDlg message handlers
void StationeryOrStuffDlg::OnBnClickedDiscard()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


void StationeryOrStuffDlg::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_pStationery)
	{
		m_pStationery->SetName(m_ctrlName);
		m_pStationery->SetDescription(m_strSpecification);
		m_pStationery->SetUnitRate(m_dUnitPrice);
		m_pStationery->SetQuantity(m_dQty);
		m_pStationery->SetComment(m_strComment);
	}

	if(m_pStuff)
	{
		m_pStuff->SetName(m_ctrlName);
		m_pStuff->SetQuantity(m_dQty);
		m_pStuff->SetDescription(m_strComment);
	}

	OnOK();
}


BOOL StationeryOrStuffDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	GetData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void StationeryOrStuffDlg::SetTitle(const CString& Text )
{
	m_strTitle = Text;
}

void StationeryOrStuffDlg::UpdateControls( BOOL bReadOnly )
{
	CEdit* idEdit = (CEdit*)GetDlgItem(IDC_EDIT_ID);
	if(idEdit)
		idEdit->SetReadOnly(!bReadOnly);

	CEdit* nameEdit = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
	if(nameEdit)
		nameEdit->SetReadOnly(bReadOnly);

	CEdit* descEdit = (CEdit*)GetDlgItem(IDC_EDIT_DESC);
	if(descEdit)
		descEdit->SetReadOnly(bReadOnly);

	CEdit* upEdit = (CEdit*)GetDlgItem(IDC_EDIT_UP);
	if(upEdit)
		upEdit->SetReadOnly(bReadOnly);

	//CEdit* qtyEdit = (CEdit*)GetDlgItem(IDC_EDIT_QTY);
	//if(qtyEdit)
	//	qtyEdit->SetReadOnly(bReadOnly);

	//CEdit* coomentEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMENT);
	//if(coomentEdit)
	//	coomentEdit->SetReadOnly(bReadOnly);
}


void StationeryOrStuffDlg::OnEnKillfocusEditId()
{
	CGroceriesFrame* pFrame = dynamic_cast<CGroceriesFrame*>(GetParentFrame());
	if(NULL == m_pStuff || NULL == pFrame)
		return;

	GroceriesCatalog* pCat = pFrame->GetRoot();

	if(NULL == pCat)
		return;

	CString strID;
	m_ctrlID.GetWindowText(strID);

	if(!strID.IsEmpty())
	{
		int nID = _wtoi(strID);

		if(m_pStuff)
		{
			Stationery* pStationery = pCat->FindStationeryByID(nID);
			if(pStationery)
			{
// 				GroceriesItem* pParent = dynamic_cast<GroceriesItem*>(m_pStuff->GetParent());
// 				if(pParent && pParent->IsUnique(pStationery))
// 				{
// 					m_pStuff->SetStationery(pStationery);
// 					GetData();
// 				}
				//annotate : m_pParent is always equal to NULL above
				
				
				m_pStuff->SetStationery(pStationery);
				GetData();
				
			}
			else
			{
				//AfxMessageBox(_T("Must give a Stationery option"));

				AfxMessageBox(_T("Specified ID of Stationery is not found"));
				
				//m_ctrlID.SetFocus();
				//m_ctrlID.SetWindowText(strID);
				return;
			}
		}
	}
}

void StationeryOrStuffDlg::GetData()
{
	if(m_pStationery)
	{
		UpdateControls(FALSE);
		CString Text;
		Text.Format(_T("%d"), m_pStationery->GetID());

		m_ctrlID.SetWindowText(Text);
		m_ctrlName = m_pStationery->GetName();
		m_strSpecification = m_pStationery->GetDescription();
		m_dUnitPrice = m_pStationery->GetUnitRate();
		m_dQty = m_pStationery->GetQuantity();
		m_strComment = m_pStationery->GetComment();

	}

	if(m_pStuff)
	{
		UpdateControls(TRUE);
		Stationery* pStationery = m_pStuff->GetStationery();
		if(pStationery)
		{
			CString Text;
			Text.Format(_T("%d"), pStationery->GetID());

			m_ctrlID.SetWindowText(Text);
			m_ctrlName = pStationery->GetName();
			m_strSpecification = pStationery->GetDescription();
			m_dUnitPrice = pStationery->GetUnitRate();
			m_dQty = m_pStuff->GetQuantity();
			m_strComment = pStationery->GetComment();
		}
	}

	UpdateData(FALSE);
	SetWindowText(m_strTitle);
}
