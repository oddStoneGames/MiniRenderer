#ifdef WINDOWS
#include "WindowsWindow.h"
#include <tchar.h>

namespace MiniRenderer
{
	LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& props)
		: m_hInstance(GetModuleHandle(nullptr))
	{
		Init(props);
		const TCHAR class_name[] = TEXT("My Window");

		WNDCLASS wndClass = {};
		wndClass.lpszClassName = class_name;
		wndClass.hInstance = m_hInstance;
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.lpfnWndProc = WindowProc;

		RegisterClass(&wndClass);

		DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

		RECT rect;
		rect.left = 250;
		rect.bottom = 250;
		rect.right = rect.left + m_Data.Width;
		rect.top = rect.bottom + m_Data.Height;

		// To make sure that our window size is the whole canvas and not the outer border.
		AdjustWindowRect(&rect, style, false);

		m_hWnd = CreateWindowEx(0, class_name, TEXT(m_Data.Title), style, rect.left, rect.bottom, rect.right - rect.left, rect.top - rect.bottom, NULL, NULL, m_hInstance, NULL);

		ShowWindow(m_hWnd, SW_SHOW);
	}

	WindowsWindow::~WindowsWindow()
	{
		const TCHAR class_name[] = TEXT("My Window");
		UnregisterClass(class_name, m_hInstance);
	}

	void WindowsWindow::OnUpdate()
	{

	}

	std::unique_ptr<MiniWindow> MiniWindow::Create(const WindowProperties& props)
	{
		return std::unique_ptr<WindowsWindow>(new WindowsWindow(props));
	}

	bool WindowsWindow::ProcessMessages()
	{
		MSG msg = {};
		while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)	return false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return true;
	}

	void WindowsWindow::Init(const WindowProperties& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.VSync = true;
	}

	void WindowsWindow::Shutdown()
	{

	}
}
#endif