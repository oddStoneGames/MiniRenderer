#ifndef WINDOWS

#include "LinuxWindow.h"

namespace MiniRenderer
{
    LinuxWindow::LinuxWindow(const WindowProperties &windowProps)
    {
        Init(windowProps);
    }

    std::unique_ptr<MiniWindow> MiniWindow::Create(const WindowProperties& props)
	{
		return std::unique_ptr<LinuxWindow>(new LinuxWindow(props));
	}

    LinuxWindow::~LinuxWindow()
    {
        Shutdown();
    }

    void LinuxWindow::OnUpdate()
    {
        while(XNextEvent(m_Display, &m_Event) == 0)
        {
            
        }
    }

    void LinuxWindow::Init(const WindowProperties &props)
    {
        if((m_Display = XOpenDisplay(NULL)) == nullptr)
            throw std::runtime_error("Can't Open Display!\n");

        // Get Default display & Root window.
        m_Screen = DefaultScreen(m_Display);
        m_RootWindow = RootWindow(m_Display, m_Screen);

        // Creating a simple window
        m_Window = XCreateSimpleWindow(m_Display, m_RootWindow, 500, 500, 
        props.Width, props.Height, 15, BlackPixel(m_Display, m_Screen), WhitePixel(m_Display, m_Screen));

        // Map window to display server
        XMapWindow(m_Display, m_Window);
    }

    void LinuxWindow::Shutdown()
    {
        printf("Getting Called!");
        XUnmapWindow(m_Display, m_Window);
        XDestroyWindow(m_Display, m_Window);
        XCloseDisplay(m_Display);
    }
}

#endif