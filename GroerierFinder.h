#ifndef _GROERIERFINDER_H_
#define _GROERIERFINDER_H_

#pragma once
#include "Groceries\GroceriesVisitor.h"

class GroerierFinder : public GroceriesVisitor
{
public:
	GroerierFinder(const CString& strMatchKey, bool bKey = true);
	virtual ~GroerierFinder(void);

	bool IsUnique();
protected:
	virtual void VisitGroceriesCatalog(GroceriesCatalog* pCatalog);
	virtual void VisitGroceriesGroup(GroceriesGroup* pGroceriesGroup);
	virtual void VisitGroceriesItem(GroceriesItem* pItem);

protected:
	bool m_bUnique;
	bool m_bKey;

	CString m_strMatchKey;
};

#endif // _GROERIERFINDER_H_
