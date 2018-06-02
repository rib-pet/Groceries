#include "stdafx.h"
#include "SortingListCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSortingListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CSortingListCtrl::OnLvnColumnclick)
END_MESSAGE_MAP()

CSortingListCtrl::CSortingListCtrl(void)
	: m_bSortOrder(TRUE)
{
}


CSortingListCtrl::~CSortingListCtrl(void)
{

}

void CSortingListCtrl::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	// TODO: Add your control notification handler code here

	for(int index=0; index<GetItemCount(); ++index)
	{
		SetItemData(index, index);
	}

	if(pNMLV->iSubItem == m_nSelCol)
	{
		m_bSortOrder = !m_bSortOrder;
	}
	else
	{
		m_bSortOrder = TRUE;
		m_nSelCol = pNMLV->iSubItem;
	}

	SortItems(ListCtrlCompare, (DWORD)this);
}

int CALLBACK CSortingListCtrl::ListCtrlCompare(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort)
{
	CSortingListCtrl* pV = (CSortingListCtrl*)lParamSort;

	if( pV->m_nSelCol > 1 )
	{
		return -1;
	}

	CString strComp1Name,strComp2Name,strComp1Sum,strCompp2Sum;
	int nSum1,nSum2;
	int nCompRes;
	switch (pV->m_nSelCol)
	{
	case 0:
		{
			for(int i=0; i<pV->GetItemCount(); ++i)
			{
				if(lParam1 == pV->GetItemData(i))
				{
					strComp1Name = pV->GetItemText(i, 0);
					nSum1 = _ttoi(strComp1Name);
				}
				if(lParam2 == pV->GetItemData(i))
				{
					strComp2Name = pV->GetItemText(i, 0);
					nSum2 = _ttoi(strComp2Name);
				}
			}
			
			if(nSum1 == nSum2)
				nCompRes = 0;
			else
				nCompRes=(nSum1<nSum2)?-1:1;
			break;
		}
	case 1:
	{
		for(int i=0; i<pV->GetItemCount(); ++i)
		{
			if(lParam1 == pV->GetItemData(i))
				strComp1Name = pV->GetItemText(i, 1);
			if(lParam2 == pV->GetItemData(i))
				strComp2Name = pV->GetItemText(i, 1);
		}

		nCompRes = strComp1Name.Compare(strComp2Name);
		break;
	}
	default:
		break;
	}

	if(pV->m_bSortOrder)
		return nCompRes;
	else
		return -1*nCompRes;
}