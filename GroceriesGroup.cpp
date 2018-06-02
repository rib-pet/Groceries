#include "stdafx.h"
#include "Groceries\GroceriesGroup.h"

GroceriesGroup::GroceriesGroup(void)
{
}


GroceriesGroup::~GroceriesGroup(void)
{
}

void GroceriesGroup::SetName( const CString& strKey )
{
	m_strName = strKey;
}

CString GroceriesGroup::GetName() const
{
	return m_strName;
}
