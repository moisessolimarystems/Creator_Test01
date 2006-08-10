#ifndef __USB_Notification_E6401DB5_18EE_433c_BC4F_44F4C2CDE8B2__
#define __USB_Notification_E6401DB5_18EE_433c_BC4F_44F4C2CDE8B2__

#include <windows.h>

class USBNotification
{
public:
	USBNotification(LPVOID pContext);
	virtual ~USBNotification();
	
	virtual void USBEventCallback(LPVOID pContext) = 0;
	
private:
	// detects usb key insertion removal and updates the key list accordingly
	HWND m_USBNotifyWindow;
	HANDLE m_USBNotifyThreadTerminateEvent;
	HANDLE m_USBNotifyThread;
	LPVOID m_UserCallbackContext;
	static const wchar_t* USBNotifyWindowClassName;
	static LRESULT CALLBACK USBNotifyCallback(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	static DWORD WINAPI USBNotifyThreadProc(LPVOID pContext);
};

#endif
