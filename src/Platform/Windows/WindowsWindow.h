#pragma once

#include <iostream>
#include "src/Core/Window.h"

#include <Windows.h>

namespace MiniRenderer
{
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& windowProps);
		WindowsWindow(const WindowsWindow&) = delete;
		WindowsWindow& operator =(const WindowsWindow&) = delete;
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; }
		virtual uint32_t GetHeight() const override { return m_Data.Height; }

		// Window Attributes
		virtual void SetVSync(bool enabled) override { m_Data.VSync = enabled; }
		virtual bool IsVSync() const override { return m_Data.VSync; }
		inline virtual void* GetNativeWindow() const override { return (void*)this; }

		bool ProcessMessages();
	private:
		void Init(const WindowProperties& props);
		void Shutdown();
	private:
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		struct WindowData
		{
			const char* Title;
			unsigned int Width, Height;
			bool VSync;
		};

		WindowData m_Data;
	};
}