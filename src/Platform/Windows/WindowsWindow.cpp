#ifdef PLATFORM_WINDOWS
#include "WindowsWindow.h"
#include <Windowsx.h>
#include <tchar.h>

namespace MiniRenderer
{
	LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_KEYDOWN:
		{
			if ((HIWORD(lParam) & KF_REPEAT) != KF_REPEAT)
			{
				// Key Down Event without repeating.
				KeyDownEvent kd;
				kd.keycode = (int)wParam;
				EventHandler::GetInstance()->KeyEventDispatcher.SendEvent(kd);
			}
			break;
		}
		case WM_KEYUP:
		{
			// Key Up Event.
			KeyUpEvent ku;
			ku.keycode = (int)wParam;
			EventHandler::GetInstance()->KeyEventDispatcher.SendEvent(ku);
			break;
		}
		case WM_MOUSEMOVE:
		{
			// Mouse Move Event.
			MouseMovedEvent me;
			me.x = GET_X_LPARAM(lParam);
			me.y = GET_Y_LPARAM(lParam);
			EventHandler::GetInstance()->MouseEventDispatcher.SendEvent(me);
			break;
		}
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		{
			// Mouse Button Down Event.
			MouseButtonDownEvent md;
			// To make sure the input is synonymous in both linux & windows.
			// 1 - LMB, 2 - MMB, 3 - RMB
			md.button = (uMsg == WM_RBUTTONDOWN) ? 3 : (uMsg == WM_MBUTTONDOWN ? 2 : 1);
			EventHandler::GetInstance()->MouseEventDispatcher.SendEvent(md);
			break;
		}
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		{
			// Mouse Button Up Event.
			MouseButtonUpEvent mu;
			mu.button = (uMsg == WM_RBUTTONUP) ? 3 : (uMsg == WM_MBUTTONUP ? 2 : 1);
			EventHandler::GetInstance()->MouseEventDispatcher.SendEvent(mu);
			break;
		}
		case WM_SIZE:
		{
			// Window Resize Event.
			WindowResizeEvent wr;
			wr.width = LOWORD(lParam);	// Macro to get the low-order word.
			wr.height = HIWORD(lParam); // Macro to get the high-order word.

			// Send Resize Event.
			EventHandler::GetInstance()->WindowEventDispatcher.SendEvent(wr);
			break;
		}
		case WM_PAINT:
		{
			// Paint all the area again.
			//PAINTSTRUCT ps;
			//HDC hdc = BeginPaint(hWnd, &ps);
			//// All painting occurs here, between BeginPaint and EndPaint.
			//FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			//EndPaint(hWnd, &ps);
			break;
		}
		case WM_CLOSE:
		{
			// Window Close Event.
			WindowCloseEvent wc;
			EventHandler::GetInstance()->WindowEventDispatcher.SendEvent(wc);
			break;
		}
		case WM_DESTROY:
			return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& props)
		: m_hInstance(GetModuleHandle(nullptr))
	{
		Init(props);
	}

	void WindowsWindow::OnUpdate()
	{
		if (!ProcessMessages())	return;
		return;
	}

	void WindowsWindow::Draw(const Framebuffer& framebuffer)
	{
		// Update Bitmap Width & Height to match screen height.
		if (framebuffer.GetFramebufferWidth() != m_BitmapInfo.bmiHeader.biWidth || -framebuffer.GetFramebufferHeight() != m_BitmapInfo.bmiHeader.biHeight)
		{
			m_BitmapInfo.bmiHeader.biWidth = framebuffer.GetFramebufferWidth();
			m_BitmapInfo.bmiHeader.biHeight = -framebuffer.GetFramebufferHeight();
		}

		int linesCopied = StretchDIBits(m_DeviceContext, 
					  0, 0, framebuffer.GetFramebufferWidth(), framebuffer.GetFramebufferHeight(), // Destination Size.
					  0, 0, framebuffer.GetFramebufferWidth(), framebuffer.GetFramebufferHeight(), // Source Size.
					  (void *)framebuffer.colorBuffer, &m_BitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		//printf("Lines Copied: %d\n", linesCopied);
	}

	void WindowsWindow::OnClose()
	{
		// Free the resources.
		DestroyWindow(m_hWnd);
		PostQuitMessage(0);
		UnregisterClass(m_Classname, m_hInstance);
	}

	std::unique_ptr<MiniWindow> MiniWindow::Create(const WindowProperties& props)
	{
		return std::make_unique<WindowsWindow>(props);
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

	int WindowsWindow::Init(const WindowProperties& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		WNDCLASS wndClass = {};
		wndClass.lpszClassName = m_Classname;
		wndClass.hInstance = m_hInstance;
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.lpfnWndProc = WindowProc;

		DWORD style = WS_OVERLAPPEDWINDOW;

		RECT rect;
		rect.left = 250;
		rect.bottom = 250;
		rect.right = rect.left + m_Data.Width;
		rect.top = rect.bottom + m_Data.Height;

		// To make sure that our window size is the whole canvas and not the outer border.
		AdjustWindowRect(&rect, style, false);

		// TODO: Move Everything to a FUNCTION
		if (!RegisterClass(&wndClass))
			return GetLastError();

		m_Data.Width = rect.right - rect.left;
		m_Data.Height = rect.top - rect.bottom;
		printf("WIDTH & HEIGHT of Rect: %d\t%d\n", m_Data.Width, m_Data.Height);

		m_hWnd = CreateWindowEx(0, m_Classname, TEXT(m_Data.Title), style, rect.left, rect.bottom, m_Data.Width, m_Data.Height, NULL, NULL, m_hInstance, NULL);

		if (!m_hWnd)
			return GetLastError();

		// Set Bitmap info.
		m_BitmapInfo.bmiHeader.biSize = sizeof(m_BitmapInfo.bmiHeader);
		m_BitmapInfo.bmiHeader.biWidth = m_Data.Width;
		// Negative height makes top left as the coordinate system origin for the DrawPixel function, otherwise its bottom left
		m_BitmapInfo.bmiHeader.biHeight = -m_Data.Height;
		m_BitmapInfo.bmiHeader.biPlanes = 1;
		m_BitmapInfo.bmiHeader.biBitCount = 32;
		m_BitmapInfo.bmiHeader.biCompression = BI_RGB;

		m_DeviceContext = GetDC(m_hWnd);
		ShowWindow(m_hWnd, SW_SHOW);

		return 0;
	}
}
#endif