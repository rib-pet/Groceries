
#include "stdafx.h"
#include "GroceriesControlWrapper.h"

using namespace Groceries;
using namespace System::Windows::Interop;
using namespace WPFControls;


GroceriesControlWrapper::GroceriesControlWrapper()
{
}


GroceriesControlWrapper::~GroceriesControlWrapper()
{
	if(!disposed)
	{		
		// Dispose of managed resources.
		if(_control)
		{
			delete _control;
			_control = nullptr;
		}

		if(_hws)
		{
			delete _hws;
			_hws = nullptr;
		}

		// Call C++ finalizer to clean up unmanaged resources.
		this->!GroceriesControlWrapper();
	}
	// Mark the class as disposed. This flag allows you to throw an
	// exception if a disposed object is accessed.
	disposed = true;
}

GroceriesControlWrapper::!GroceriesControlWrapper(void)
{
}

HWND GroceriesControlWrapper::GetHwnd( HWND parent, int x, int y, int width, int height)
{
	HwndSource^ source = gcnew HwndSource(
		0, // class style
		WS_VISIBLE | WS_CHILD, // style
		0, // exstyle
		x, y, width, height,
		"RIBAnswersCompareControl", // NAME
		System::IntPtr(parent)        // parent window 
		);

	source->AddHook(gcnew HwndSourceHook(GroceriesControlWrapper::ChildHwndSourceHook));

	WCHAR filepath[MAX_PATH] = {0x0};
	GetCurrentDirectory(sizeof(filepath), filepath);
	CString pathOfExcel(filepath);
	pathOfExcel += L"\\document\\stationery.xml";

	WPFControls::WPFControl^ _dockControl = gcnew WPFControls::WPFControl();
	source->RootVisual = _dockControl;
	GroceriesControlWrapper^ pControlWraper = GroceriesControlWrapper::GetInstance((unsigned long)parent);
	if(pControlWraper)
	{
		pControlWraper->_control =_dockControl;
		pControlWraper->_hws = source;
	}

	return (HWND) source->Handle.ToPointer();
}

GroceriesControlWrapper^ GroceriesControlWrapper::GetInstance( unsigned long key )
{
	GroceriesControlWrapper^ ctrlWrapper;
	_instances.TryGetValue(key, ctrlWrapper);

	if (!ctrlWrapper)
	{
		ctrlWrapper = gcnew GroceriesControlWrapper;
		_instances.Add(key, ctrlWrapper);
	}

	return ctrlWrapper;
}

void GroceriesControlWrapper::RemoveInstance( unsigned long key )
{
	if (_instances.ContainsKey(key))
	{
		GroceriesControlWrapper^ ctrlWrapper;
		_instances.TryGetValue(key, ctrlWrapper);

		_instances.Remove(key);

		if (ctrlWrapper)
		{			
			delete ctrlWrapper;
			ctrlWrapper = nullptr;
		}
	}
}

System::IntPtr GroceriesControlWrapper::ChildHwndSourceHook( System::IntPtr hwnd, int msg, System::IntPtr wParam, System::IntPtr lParam, bool% handled )
{
	UNREFERENCED_PARAMETER(hwnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if(msg == WM_GETDLGCODE)
	{
		handled = true;

		return System::IntPtr(DLGC_WANTCHARS);
	}

	return System::IntPtr(0);
}
