#ifdef PLATFORM_WINDOWS

#pragma once
#include "../../Core/Window.h"
#include "../../Core/Events/EventHandler.h"
#include <Windows.h>

namespace MiniRenderer
{
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	class WindowsWindow : public MiniWindow
	{
	public:
		WindowsWindow(const WindowProperties& windowProps);
		WindowsWindow(const WindowsWindow&) = delete;
		WindowsWindow& operator =(const WindowsWindow&) = delete;
		virtual ~WindowsWindow() {}

		virtual void OnUpdate() override;
		virtual void Draw(const Framebuffer& framebuffer) override;
		virtual void OnClose() override;

		virtual void SetCursorPosition(int x, int y) override;
		virtual void RenderCursor(bool show) override;
		virtual void ConfineCursor(bool confine) override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; }
		virtual uint32_t GetHeight() const override { return m_Data.Height; }
	private:
		int Init(const WindowProperties& props);
		bool ProcessMessages();
		RECT GetLocalCoordinates() const;
	private:
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		RECT m_ScreenSize;
		HDC m_DeviceContext;
		BITMAPINFO m_BitmapInfo;	// Struct for StretchDIBits.
		const TCHAR* m_Classname = TEXT("My Window");
		struct WindowData
		{
			const char* Title;
			unsigned int Width, Height;
		}m_Data;
	};
}
#endif