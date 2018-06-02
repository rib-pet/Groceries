#ifndef _GROCERIESCONTROL_H_
#define _GROCERIESCONTROL_H_

#include "GroceriesControlWrapper.h"

namespace Groceries
{
	class GroceriesControl : public CWnd
	{
	public:
		GroceriesControl(/*CString instanceKey*/);
		virtual ~GroceriesControl(void);
		static GroceriesControl* GroceriesControlCreate(CWnd* pParent, CRect &rect, UINT nID/*, CString instanceKey*/);

	protected:
		DECLARE_MESSAGE_MAP()
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnDestroy();

		virtual BOOL PreTranslateMessage(::MSG* pMsg);

	protected:
		
	};
}
#endif