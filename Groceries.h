
// Groceries.h : main header file for the Groceries application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGroceriesApp:
// See Groceries.cpp for the implementation of this class
//

class CGroceriesApp : public CWinAppEx
{
public:
	CGroceriesApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGroceriesApp theApp;
