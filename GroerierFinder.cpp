#include "stdafx.h"
#include "GroerierFinder.h"
#include "Groceries\GroceriesGroup.h"
#include "Groceries\GroceriesItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GroerierFinder::GroerierFinder(const CString& strMatchKey, bool bKey)
	: m_strMatchKey(strMatchKey)
	, m_bUnique(true)
	, m_bKey(bKey)
{
}


GroerierFinder::~GroerierFinder(void)
{
}

void GroerierFinder::VisitGroceriesCatalog( GroceriesCatalog* pCatalog )
{
	__super::VisitGroceriesCatalog(pCatalog);
}

void GroerierFinder::VisitGroceriesGroup( GroceriesGroup* pGroceriesGroup )
{
	if(pGroceriesGroup && !m_strMatchKey.IsEmpty())
	{
		if(m_bKey)
		{
			CString strName = pGroceriesGroup->GetName();
			m_bUnique = (strName.CompareNoCase(m_strMatchKey) != 0);
		}
		else
		{
			CString strDesc = pGroceriesGroup->GetDescription();
			m_bUnique = (strDesc.CompareNoCase(m_strMatchKey) != 0);
		}
		

		if(!m_bUnique)
			return;
	}

	__super::VisitGroceriesGroup(pGroceriesGroup);
}

void GroerierFinder::VisitGroceriesItem( GroceriesItem* pItem )
{
	if(pItem && !m_strMatchKey.IsEmpty())
	{
		if(m_bKey)
		{
			CString strName = pItem->GetName();
			m_bUnique = (strName.CompareNoCase(m_strMatchKey) != 0);
		}
		else
		{
			CString strDesc = pItem->GetDescription();
			m_bUnique = (strDesc.CompareNoCase(m_strMatchKey) != 0);
		}

		if(!m_bUnique)
			return;
	}

	__super::VisitGroceriesItem(pItem);
}

bool GroerierFinder::IsUnique()
{
	return m_bUnique;
}
