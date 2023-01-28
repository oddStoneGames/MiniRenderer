#include "Swapchain.h"

namespace MiniRenderer
{
	Swapchain::Swapchain(int width, int height)
		: m_Width(width), m_Height(height), m_FrontBuffer(0, 0, width, height), backBuffer(0, 0, width, height)
	{
		// Add OnWindowEvent function as a listener for window events.
		EventHandler::GetInstance()->WindowEventDispatcher.AddListener(WindowEvents::WindowResize, std::bind(&Swapchain::OnWindowEvent, this, std::placeholders::_1));
	}

	/// @brief Copies the data of Back buffer into Front buffer & shows it to the window.
	/// If swap is set to false, then backbuffer is shown directly to the window & doesn't wait for the backbuffer to complete.
	void Swapchain::SwapBuffers(MiniWindow* window, bool swap)
	{
		if (swap)
		{
			// Use Double Buffers.
			if (m_BackbufferComplete)
				// Copy the contents of Back buffer to front buffer.
				m_FrontBuffer.CopyBuffers(backBuffer);

			// Show Front Buffer to the window.
			window->Draw(m_FrontBuffer);
		}
		else
		{
			// Show Backbuffer to the window.
			window->Draw(backBuffer);
		}
	}

	void Swapchain::OnWindowEvent(const Event<WindowEvents>& event)
	{
		if (event.GetType() == WindowEvents::WindowResize)
		{
			// Window Resize Event.
			const WindowResizeEvent& wr = static_cast<const WindowResizeEvent&>(event);

			// Resize our framebuffers to match the current size of our window.
			m_FrontBuffer.SetFramebufferSize(0, 0, wr.width, wr.height);
			backBuffer.SetFramebufferSize(0, 0, wr.width, wr.height);
		}
	}

}