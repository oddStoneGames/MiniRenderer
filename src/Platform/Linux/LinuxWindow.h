#ifndef PLATFORM_WINDOWS

#pragma once
#include <X11/Xlib.h>

#include "../../Core/Window.h"

// For Sending Events.
#include "../../Core/Events/EventHandler.h"

namespace MiniRenderer
{
	class LinuxWindow : public MiniWindow
	{
	public:
		LinuxWindow(const WindowProperties& windowProps);
		LinuxWindow(const LinuxWindow&) = delete;
		LinuxWindow& operator =(const LinuxWindow&) = delete;
		virtual ~LinuxWindow() { }

		virtual void OnUpdate() override;
		virtual void Draw(const Framebuffer& framebuffer) override;
		virtual void OnClose() override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; }
		virtual uint32_t GetHeight() const override { return m_Data.Height; }
	private:
		void Init(const WindowProperties& props);
		XImage* ImageFromBuffer(const Framebuffer& buffer, Visual* visual);
	private:
		Display* m_Display;
		int m_Screen;
		Window m_RootWindow;
		Window m_Window;
        XEvent m_Event;
		XImage* m_Image;

		/// @brief Color Buffer made from Framebuffer's Color buffer.
		unsigned char* m_ColorBuffer;

		// Width & Height of the color buffer that will be shown to the screen.
		int m_BufferWidth, m_BufferHeight;

		struct WindowData
		{
			const char* Title;
			unsigned int Width, Height;
		}m_Data;
	};
}
#endif