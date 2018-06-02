#include "stdafx.h"
#include "GroceriesControl.h"
#include "GroceriesControlWrapper.h"

using namespace Groceries;

GroceriesControl::GroceriesControl(/*CString instanceKey*/)	
{
	//m_instanceKey = instanceKey;
}


GroceriesControl::~GroceriesControl(void)
{
}

GroceriesControl* GroceriesControl::GroceriesControlCreate( CWnd* pParent, CRect &rect, UINT nID/*, CString instanceKey*/)
{
	ASSERT(pParent);

	GroceriesControl* pGroceriesControl = new GroceriesControl(/*instanceKey*/);

	if(!pGroceriesControl->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, pParent, nID))
	{
		delete pGroceriesControl;
		pGroceriesControl = NULL;
	}

	return pGroceriesControl;
}

BEGIN_MESSAGE_MAP(GroceriesControl, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


int GroceriesControl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the wpf control and the hwnd source
	HWND hwnd = GroceriesControlWrapper::GetHwnd(GetSafeHwnd(), 
		0, 
		0, 
		0,
		0/*,
		gcnew System::String(m_instanceKey)*/);

	System::Windows::Interop::HwndSource^ hws = System::Windows::Interop::HwndSource::FromHwnd(System::IntPtr(hwnd));
	hws->SizeToContent = System::Windows::SizeToContent::WidthAndHeight;
	
	/*if(m_pEventHandler == NULL)
	{
		m_pEventHandler = new RIBWPFUserControlEvents();
		m_pEventHandler->RegisterEvents();
	}*/
	return 0;
}

void GroceriesControl::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize(nType, cx, cy);

	intptr_t tmphwnd = (intptr_t)GetSafeHwnd();
	// get the current WPF control from the dictionary
	GroceriesControlWrapper ^pGroceriesControlWrapper = GroceriesControlWrapper::GetInstance((unsigned long)tmphwnd);
	if (pGroceriesControlWrapper->_control)
	{
		// WPF unit is 1/96" 
		// convert pixel to 1/96" units

		HDC hdc = ::GetDC(NULL);
		ASSERT(hdc);
		double height = (cy * 96.) / ::GetDeviceCaps(hdc, LOGPIXELSY);
		double width = (cx * 96.) / ::GetDeviceCaps(hdc, LOGPIXELSX);

		::ReleaseDC(NULL, hdc);

		pGroceriesControlWrapper->_control->Width = width;
		pGroceriesControlWrapper->_control->Height = height;
	}
}

void GroceriesControl::OnDestroy()
{
	intptr_t tmphwnd = (intptr_t)GetSafeHwnd();
	// removed the current WPF control from the dictionary
	GroceriesControlWrapper::RemoveInstance((unsigned long)tmphwnd);

	CWnd::OnDestroy();
}


BOOL GroceriesControl::PreTranslateMessage(::MSG* pMsg)
{
	if (pMsg != NULL && (VK_TAB == pMsg->wParam || VK_RETURN == pMsg->wParam || VK_LEFT == pMsg->wParam || VK_RIGHT == pMsg->wParam))
	{
		intptr_t tmphwnd = (intptr_t)GetSafeHwnd();
		GroceriesControlWrapper ^pGroceriesControlWrapper = GroceriesControlWrapper::GetInstance((unsigned long)tmphwnd);
		if (pGroceriesControlWrapper != nullptr)
		{
			if (pGroceriesControlWrapper->_control != nullptr)
			{
				HwndSource^ hwndSource = pGroceriesControlWrapper->_hws;

				if (hwndSource != nullptr)
				{
					::SendMessage((HWND)hwndSource->Handle.ToPointer(), pMsg->message, pMsg->wParam, pMsg->lParam);
					return true;
				}
			}
		}
	}
	return __super::PreTranslateMessage(pMsg);
}
