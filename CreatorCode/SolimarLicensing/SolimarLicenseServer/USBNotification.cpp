#include "USBNotification.h"
#include <dbt.h>

const wchar_t* USBNotification::USBNotifyWindowClassName = L"USB Device Insert/Remove Notification Window";

HWND USBNotification::m_USBNotifyWindow = 0;

USBNotification::USBNotification(LPVOID pContext) : 
	m_USBNotifyThreadTerminateEvent(CreateEvent(0,TRUE,FALSE,0)),
	m_USBNotifyThread(0)
{
}

//CR.FIX.14360 - Delay the usb notification until after the class member finish initializing.
void USBNotification::StartUSBNotification()
{
	// launch a thread to drive the message pump for the window
	if(!m_USBNotifyThread)
		m_USBNotifyThread = CreateThread(0, 0, USBNotifyThreadProc, this, 0, 0);
}

USBNotification::~USBNotification()
{
	// wait for thread termination
	if (m_USBNotifyThread)
	{
		SetEvent(m_USBNotifyThreadTerminateEvent);
		WaitForSingleObject(m_USBNotifyThread, 500);
		TerminateThread(m_USBNotifyThread,0);
		CloseHandle(m_USBNotifyThread);
		CloseHandle(m_USBNotifyThreadTerminateEvent);
	}	
}

LRESULT CALLBACK USBNotification::USBNotifyCallback(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		case WM_DEVICECHANGE:
		{
			switch(wp)
			{
				case DBT_DEVNODES_CHANGED:
				{
					// call the user provided callback
					((USBNotification*)(LONG_PTR)GetWindowLongPtr(hwnd, 0*sizeof(LONG_PTR)))->USBEventCallback((LPVOID)(LONG_PTR)GetWindowLongPtr(hwnd, 1*sizeof(LONG_PTR)));
					return 0;
				}
			}
			break;
		}
	}
	return DefWindowProc(hwnd,msg,wp,lp);
}


DWORD WINAPI USBNotification::USBNotifyThreadProc(LPVOID pContext)
{
	HRESULT hr = S_OK;

	// create the window class responsible for usb insert/remove notifications
	WNDCLASS wndclass;
	memset(&wndclass,0,sizeof(wndclass));
	wndclass.lpfnWndProc = USBNotifyCallback;
	wndclass.lpszClassName = USBNotifyWindowClassName;
	wndclass.cbWndExtra	= 2*sizeof(LONG_PTR);	// allocate some space for the 'this' pointer
	
	// register the window class, ignore the case that the class already exists
	if (!RegisterClass(&wndclass) && ERROR_CLASS_ALREADY_EXISTS!=GetLastError())
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
		return 0;
	}
	
	//
	// create a new window
	m_USBNotifyWindow = CreateWindow(USBNotifyWindowClassName, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (!m_USBNotifyWindow) 
	{
		return 0;
	}
	
	// add the 'this' pointer to the window so it is available in the callback function
	SetWindowLongPtr(m_USBNotifyWindow, 0*sizeof(LONG_PTR), (LONG_PTR)pContext);
	// add the user provided context pointer
	SetWindowLongPtr(m_USBNotifyWindow, 1*sizeof(LONG_PTR), (LONG_PTR)pContext);
	
	// register the window for usb notifications
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;

	// for USB Raw Device
	//GUID_DEVINTERFACE_USB_DEVICE
	static const GUID usb_class_guid = {0xa5dcbf10, 0x6530, 0x11d2, {0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed}};	

	// for USB hubs, not needed.  Disconnecting / Connecting the hub will notify each individual device
	//static const GUID usb_class_guid = {0x36fc9e60, 0xc465, 0x11cf, {0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};

	ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
	NotificationFilter.dbcc_size		= sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype	= DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid	= usb_class_guid;
	
	HDEVNOTIFY hdevnotify = RegisterDeviceNotification(m_USBNotifyWindow, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
	if(hdevnotify == NULL)
		return 0;

	hr = HRESULT_FROM_WIN32(::GetLastError());
	if(FAILED(hr))
	{
		return 0;
	}

	// Setup Message Pump
	MSG msg;
	HWND hwnd;
	USBNotification* pThis = (USBNotification*)pContext;
	hwnd = pThis->m_USBNotifyWindow;
	while(GetMessage(&msg, hwnd, 0, 0))
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_USBNotifyThreadTerminateEvent,0))
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
