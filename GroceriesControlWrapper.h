


#ifndef _GROCERIESCONTROLWRAPPER_H_
#define _GROCERIESCONTROLWRAPPER_H_

#include "stdafx.h"
#include "Groceries\GroceriesCatalog.h"

using namespace System::Windows::Interop;
using namespace WPFControls;
using namespace System::Collections::Generic;

namespace Groceries
{
	ref class GroceriesControlWrapper
	{
	public:
		GroceriesControlWrapper();
		virtual ~GroceriesControlWrapper(void);

		static HWND GetHwnd(HWND parent, int x, int y, int width, int height/*, System::String ^ instanceKey*/);

		static GroceriesControlWrapper^ GetInstance(unsigned long key);
		static void RemoveInstance(unsigned long key);

		::HwndSource^ _hws;

		WPFControl^ _control;

	protected:
		!GroceriesControlWrapper();

	private:
		static Dictionary<unsigned long, GroceriesControlWrapper^> _instances;

		static System::IntPtr ChildHwndSourceHook(System::IntPtr hwnd, int msg, System::IntPtr wParam, System::IntPtr lParam, bool% handled);

		// Track whether Dispose has been called.
		bool disposed;

	};
}
#endif
