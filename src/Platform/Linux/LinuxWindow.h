#ifndef WINDOWS

#pragma once
#include <X11/Xlib.h>
#include <stdexcept>

#include "../../Core/Window.h"

namespace MiniRenderer
{
	class LinuxWindow : public MiniWindow
	{
	public:
		LinuxWindow(const WindowProperties& windowProps);
		LinuxWindow(const LinuxWindow&) = delete;
		LinuxWindow& operator =(const LinuxWindow&) = delete;
		virtual ~LinuxWindow();

		virtual void OnUpdate() override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; }
		virtual uint32_t GetHeight() const override { return m_Data.Height; }

		// Window Attributes
		virtual void SetVSync(bool enabled) override { m_Data.VSync = enabled; }
		virtual bool IsVSync() const override { return m_Data.VSync; }
		inline virtual void* GetNativeWindow() const override { return (void*)this; }
	private:
		void Init(const WindowProperties& props);
		void Shutdown();
	private:
		Display* m_Display;
		int m_Screen;
		Window m_RootWindow;
		Window m_Window;
        XEvent m_Event;

		struct WindowData
		{
			const char* Title;
			unsigned int Width, Height;
			bool VSync;
		}m_Data;
	};
}
#endif