#pragma once

#include "Framebuffer.h"
#include "Window.h"
#include "Events/EventHandler.h"

namespace MiniRenderer
{
	/// @brief Swapchain manages the Framebuffers
	class Swapchain
	{
	public:
		Swapchain(int width, int height);
		~Swapchain() {}

		/// @brief If Double Buffers are enabled, then this function swaps the backbuffer with the buffer on the screen(frontbuffer).
		void SwapBuffers(MiniWindow* window, bool swap = true);

		/// @brief Buffer where rendering takes place.
		Framebuffer backBuffer;

		/// @brief Sets the backbuffer state if it is completed or not.
		/// If the backbuffer is completed then its content will be copied to front buffer and it will be shown to the screen.
		void SetBackbufferState(bool completed) { m_BackbufferComplete = completed; }
	private:
		void OnWindowEvent(const Event<WindowEvents>& event);
	private:
		/// @brief Width of Framebuffers.
		int m_Width;

		/// @brief Height of Framebuffers.
		int m_Height;

		/// @brief Buffer to Display.
		Framebuffer m_FrontBuffer;

		/// @brief If it is false & buffer swapping is enabled, then front buffer will be visible to the screen while back buffer is updating.
		bool m_BackbufferComplete = false;
	};
}