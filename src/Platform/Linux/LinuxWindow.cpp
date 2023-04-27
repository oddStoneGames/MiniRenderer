#ifndef PLATFORM_WINDOWS

#include "LinuxWindow.h"
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <cstring>
#include <stdexcept>

namespace MiniRenderer
{
    LinuxWindow::LinuxWindow(const WindowProperties &windowProps)
    {
        Init(windowProps);
    }

    std::unique_ptr<MiniWindow> MiniWindow::Create(const WindowProperties& props)
	{
		return std::make_unique<LinuxWindow>(props);
	}

    void LinuxWindow::OnUpdate()
    {
        Atom wmDeleteMessage = XInternAtom(m_Display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(m_Display, m_Window, &wmDeleteMessage, 1);

        if(XNextEvent(m_Display, &m_Event) == 0)
        {
            switch(m_Event.type)
            {
                case ClientMessage:
                {
                    if (m_Event.xclient.data.l[0] == wmDeleteMessage)
                    {
                        // Send Window Close Event.
                        //OnClose();
                        WindowCloseEvent wc;
                        EventHandler::GetInstance()->WindowEventDispatcher.SendEvent(wc);
                    }
                    break;
                }
                case ConfigureNotify:
                {
                    XConfigureEvent xce = m_Event.xconfigure;
                    /* This event type is generated for a variety of
                    happenings, so check whether the window has been
                    resized. */
                    if (xce.width != m_Data.Width || xce.height != m_Data.Height)
                    {
                        m_Data.Width = xce.width;
                        m_Data.Height = xce.height;
                        
                        // Send Window Resize Event.
                        WindowResizeEvent wr;
                        wr.width = m_Data.Width;
                        wr.height = m_Data.Height;
                        EventHandler::GetInstance()->WindowEventDispatcher.SendEvent(wr);
                    }
                    break;
                }
                case MotionNotify:
                {
                    MouseMovedEvent mm;
                    mm.x = m_Event.xmotion.x;
                    mm.y = m_Event.xmotion.y;
                    EventHandler::GetInstance()->MouseEventDispatcher.SendEvent(mm);
                    break;
                }
                case ButtonPress:
                {
                    MouseButtonDownEvent mb;
                    mb.button = m_Event.xbutton.button;
                    EventHandler::GetInstance()->MouseEventDispatcher.SendEvent(mb);
                    break;
                }
                case ButtonRelease:
                {
                    MouseButtonUpEvent mu;
                    mu.button = m_Event.xbutton.button;
                    EventHandler::GetInstance()->MouseEventDispatcher.SendEvent(mu);
                    break;
                }
                case KeyPress:
                {
                    KeySym key;
                    char text;
                    if(XLookupString(&m_Event.xkey, &text, 1, &key,0) == 1)
                    {
                        KeyDownEvent kd;
                        kd.keycode = text;
                        EventHandler::GetInstance()->KeyEventDispatcher.SendEvent(kd);
                    }
                    break;
                }
                case KeyRelease:
                {
                    KeySym key;
                    char text;
                    if(XLookupString(&m_Event.xkey, &text, 1, &key,0) == 1)
                    {
                        KeyUpEvent ku;
                        ku.keycode = text;
                        EventHandler::GetInstance()->KeyEventDispatcher.SendEvent(ku);
                    }
                    break;
                }
            }
        }
    }

    void LinuxWindow::Draw(const Framebuffer& framebuffer)
    {
        // Get the Image from our Framebuffer.
        m_Image = ImageFromBuffer(framebuffer, DefaultVisual(m_Display, 0));

        // Show it to window.
        XPutImage(m_Display, m_Window, DefaultGC(m_Display, 0), m_Image, 0, 0, 0, 0, m_BufferWidth, m_BufferHeight);
    }

    void LinuxWindow::OnClose()
    {
        //Reset Repeat State to be true.
        XAutoRepeatOn(m_Display);
        // Free Color Buffer.
        free(m_ColorBuffer);
        XUnmapWindow(m_Display, m_Window);
        XDestroyWindow(m_Display, m_Window);
        XCloseDisplay(m_Display);
    }

    void LinuxWindow::SetCursorPosition(int x, int y)
    {
        
    }

    void LinuxWindow::RenderCursor(bool show)
    {
        Cursor cursor;
        // if(show)
        //     XDefineCursor(m_Display, m_Window, cursor);
        // else
        //     XUndefineCursor(m_Display, m_Window);
    }

    void LinuxWindow::ConfineCursor(bool confine)
    {

    }

    void LinuxWindow::Init(const WindowProperties &props)
    {
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        if((m_Display = XOpenDisplay(NULL)) == nullptr)
            throw std::runtime_error("Can't Open Display!\n");

        // Don't Repeat Key.
        XAutoRepeatOff(m_Display);

        // Get Default display & Root window.
        m_Screen = DefaultScreen(m_Display);
        m_RootWindow = RootWindow(m_Display, m_Screen);
        
        // Creating a simple window
        m_Window = XCreateSimpleWindow(m_Display, m_RootWindow, 0, 0, 
        props.Width, props.Height, 15, BlackPixel(m_Display, m_Screen), WhitePixel(m_Display, m_Screen));
        XSetStandardProperties(m_Display, m_Window, m_Data.Title, m_Data.Title, None, NULL, 0, NULL);
        XSelectInput(m_Display, m_Window, ExposureMask | StructureNotifyMask
                     | PointerMotionMask | ButtonPressMask | ButtonReleaseMask
                     | KeyPressMask | KeyReleaseMask);

        XClearWindow(m_Display, m_Window);

        // Map window to display server
        XMapWindow(m_Display, m_Window);

        // Allocate Memory for our Color Buffer.
        m_BufferWidth = m_Data.Width;
        m_BufferHeight = m_Data.Height;
        m_ColorBuffer = (unsigned char*)malloc(m_BufferWidth * m_BufferHeight * sizeof(uint32_t));
    }

    XImage* LinuxWindow::ImageFromBuffer(const Framebuffer& buffer, Visual *visual)
    {
        int framebufferWidth = buffer.GetFramebufferWidth();
        int framebufferHeight = buffer.GetFramebufferHeight();

        if(framebufferWidth != m_BufferWidth || framebufferHeight != m_BufferHeight)
        {
            // Update our color buffer size.
            m_BufferWidth = framebufferWidth;
            m_BufferHeight = framebufferHeight;
            unsigned char* colorBuffer = (unsigned char*)realloc(m_ColorBuffer, m_BufferWidth * m_BufferHeight * sizeof(uint32_t));
            if(colorBuffer != nullptr)
                m_ColorBuffer = colorBuffer;
            else
                throw std::runtime_error("Failed to resize Linux window's color buffer.\n");
        }

        // Copy memory of Framebuffer's color buffer to our color buffer.
        std::memcpy(m_ColorBuffer, buffer.colorBuffer, m_BufferWidth * m_BufferHeight * sizeof(uint32_t));
        return XCreateImage(m_Display, visual, DefaultDepth(m_Display, m_Screen), ZPixmap, 0, (char *)m_ColorBuffer, m_BufferWidth, m_BufferHeight, 32, 0);
    }
}

#endif