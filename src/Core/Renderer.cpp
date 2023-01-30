#include "Renderer.h"
#include "LineRenderer.h"

#include <chrono>		// For Time related queries.
#include <stdlib.h>		// For EXIT_FAILURE & EXIT_SUCCESS.
#include <stdexcept>	// For std::runtime_error().

namespace MiniRenderer
{
	Renderer::Renderer(const WindowProperties& props, short unsigned int targetFPS, bool doubleBuffer)
		: m_Swapchain(props.Width, props.Height), m_TargetFPS(targetFPS), m_DoubleBuffer(doubleBuffer)
	{
		m_Window = MiniWindow::Create(props);
	}

	Renderer::~Renderer()
	{
		
	}

	void Renderer::Run()
	{
		Init();
		RenderLoop();
		Cleanup();
	}

	void Renderer::Init()
	{
		// Add Listener of Events.
		EventHandler::GetInstance()->WindowEventDispatcher.AddListener(WindowEvents::WindowResize, std::bind(&Renderer::OnWindowEvent, this, std::placeholders::_1));
		EventHandler::GetInstance()->WindowEventDispatcher.AddListener(WindowEvents::WindowClose, std::bind(&Renderer::OnWindowEvent, this, std::placeholders::_1));
		
		EventHandler::GetInstance()->MouseEventDispatcher.AddListener(MouseEvents::MouseMoved, std::bind(&Renderer::OnMouseEvent, this, std::placeholders::_1));
		EventHandler::GetInstance()->MouseEventDispatcher.AddListener(MouseEvents::MouseButtonDown, std::bind(&Renderer::OnMouseEvent, this, std::placeholders::_1));
		EventHandler::GetInstance()->MouseEventDispatcher.AddListener(MouseEvents::MouseButtonUp, std::bind(&Renderer::OnMouseEvent, this, std::placeholders::_1));

		EventHandler::GetInstance()->KeyEventDispatcher.AddListener(KeyEvents::KeyDown, std::bind(&Renderer::OnKeyEvent, this, std::placeholders::_1));
		EventHandler::GetInstance()->KeyEventDispatcher.AddListener(KeyEvents::KeyUp, std::bind(&Renderer::OnKeyEvent, this, std::placeholders::_1));
	}

	void Renderer::RenderLoop()
	{
		while (m_Running)
		{
			// Window Update
			m_Window->OnUpdate();

			// It can be so that after window processes its update, we are not in running state.
			if(!m_Running) break;

			// Rendering.
			if (m_TargetFPS > 0)
			{
				// Capped Rendering
				if (m_WaitTime > 0.0f)
				{
					// Wait & Reduce the time we have waited for and then return.
					long long timeNow = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
					float timeSinceWaiting = (float)(timeNow - m_TimeWhenWeStartedWaiting) / 1000.0f;

					if (timeSinceWaiting >= m_WaitTime)
					{
						// Wait over we can start rendering again.
						m_WaitTime = 0.0f;
					}
					else
					{
						// Wait.
						continue;
					}
				}

				// Clear Backbuffer.
				m_Swapchain.backBuffer.Clear();

				// Get Time before rendering.
				auto timeBeforeRendering = std::chrono::high_resolution_clock::now();

				// Render a Rectangle for example.
				DrawRectangle();

				// Render a line.
				DrawLines();

				// The Swapchain swaps the buffer if only our backbuffer is completed which we set manually.
				m_Swapchain.SetBackbufferState(true);

				// Swap the Buffers.
				m_Swapchain.SwapBuffers(m_Window.get(), m_DoubleBuffer);

				// Get Time after rendering.
				auto timeAfterRendering = std::chrono::high_resolution_clock::now();

				// Get the Time it took to render this frame in miliseconds.
				long long start = std::chrono::time_point_cast<std::chrono::microseconds>(timeBeforeRendering).time_since_epoch().count();
				long long end = std::chrono::time_point_cast<std::chrono::microseconds>(timeAfterRendering).time_since_epoch().count();
				float frameTime = (float)(end - start) / 1000.0f;

				// The Target Frame time.
				float targetFrameTime = (1.0f / m_TargetFPS) * 1000.0f;

				if (frameTime < targetFrameTime)
				{
					// We are faster than we are supposed to be so we should wait for some time.
					m_WaitTime = targetFrameTime - frameTime;
					m_TimeWhenWeStartedWaiting = end;
				}
				else
				{
					m_WaitTime = 0.0f;
				}
				
				// Show FPS
				// We waited for wait time to show this frame so the FPS should be calculated based upon (waittime + frametime)
				float frameTimeToDisplay = m_WaitTime + frameTime;
				float fpstoDisplay = (1.0f / frameTimeToDisplay) * 1000.0f;
				printf("Frametime: %.2f ms\t FPS: %.2f\n", frameTimeToDisplay, fpstoDisplay);
			}
			else
			{
				// Uncapped Rendering.

				// Clear Backbuffer.
				m_Swapchain.backBuffer.Clear();

				// Get Time before rendering.
				auto timeBeforeRendering = std::chrono::high_resolution_clock::now();

				// Draw rectangle in the center of the screen.
				DrawRectangle();

				// Render Line.
				DrawLines();

				// The Swapchain swaps the buffer if only our backbuffer is completed which we set manually.
				m_Swapchain.SetBackbufferState(true);

				// Swap Buffers and Show the End Result to the screen.
				m_Swapchain.SwapBuffers(m_Window.get(), m_DoubleBuffer);

				// Get Time after rendering.
				auto timeAfterRendering = std::chrono::high_resolution_clock::now();

				// Get the Time it took to render this frame in miliseconds.
				long long start = std::chrono::time_point_cast<std::chrono::microseconds>(timeBeforeRendering).time_since_epoch().count();
				long long end = std::chrono::time_point_cast<std::chrono::microseconds>(timeAfterRendering).time_since_epoch().count();
				float frameTime = (float)(end - start) / 1000.0f;
				float fps = (1.0f / frameTime) * 1000.0f;

				printf("Frametime: %.2f ms\t FPS: %.2f\n", frameTime, fps);
			}
		}
	}

	void Renderer::Cleanup()
	{
		
	}

    void Renderer::OnWindowEvent(const Event<WindowEvents>& e)
    {
		if(e.GetType() == WindowEvents::WindowResize)
		{
			// Window Resize Event.
			const WindowResizeEvent& wr = static_cast<const WindowResizeEvent&>(e);
			printf("New Width:%d\tNew Height:%d\n", wr.width, wr.height);
		}else
		{
			// Window Close Event.
			m_Window->OnClose();
			// Set Running State to false.
			m_Running = false;
		}
    }

    void Renderer::OnMouseEvent(const Event<MouseEvents> &e)
    {
		if(e.GetType() == MouseEvents::MouseMoved)
		{
			// Mouse Moved Event.
			const MouseMovedEvent& mm = static_cast<const MouseMovedEvent&>(e);
			//printf("Mouse Moved: %d\t%d\n", mm.x, mm.y);
		}else if(e.GetType() == MouseEvents::MouseButtonDown)
		{
			// Mouse Button Down Event.
			const MouseButtonDownEvent& mm = static_cast<const MouseButtonDownEvent&>(e);
			printf("Mouse Button Down: %d\n", mm.button);
		}else
		{
			// Mouse Button Up Event.
			const MouseButtonUpEvent& mm = static_cast<const MouseButtonUpEvent&>(e);
			printf("Mouse Button Up: %d\n", mm.button);
		}
    }

    void Renderer::OnKeyEvent(const Event<KeyEvents> &e)
    {
		if(e.GetType() == KeyEvents::KeyDown)
		{
			// Key Down Event.
			const KeyDownEvent& kd = static_cast<const KeyDownEvent&>(e);
			printf("Key Down: %c\n", kd.keycode);
		}else
		{
			// Key Up Event.
			// Key Down Event.
			const KeyUpEvent& ku = static_cast<const KeyUpEvent&>(e);
			printf("Key Up: %c\n", ku.keycode);
		}
    }

	void Renderer::DrawRectangle()
	{
		// Get the Middle of the Screen.
		int framebufferWidth = m_Swapchain.backBuffer.GetFramebufferWidth();
		int framebufferHeight = m_Swapchain.backBuffer.GetFramebufferHeight();
		int midX = framebufferWidth * 0.5f;
		int midY = framebufferHeight * 0.5f;

		// Rectangle size is 20% of the window width & 10% of the window height.
		int sizeX = framebufferWidth * 0.2f, sizeY = framebufferHeight * 0.1f;
		int startingPixelX = midX - sizeX * 0.5f;
		int startingPixelY = midY - sizeY * 0.5f;

		// Send draw command for every pixel.
		for (int x = startingPixelX; x <= startingPixelX + sizeX; x++)
			for (int y = startingPixelY; y <= startingPixelY + sizeY; y++)
				m_Swapchain.backBuffer.SetPixelColor(x, y, (y - x) * 0x00FFFF);
	}

	void Renderer::DrawLines()
	{
		int bufferWidth = m_Swapchain.backBuffer.GetFramebufferWidth();
		int bufferHeight = m_Swapchain.backBuffer.GetFramebufferHeight();
		int midPointX = bufferWidth * 0.5f;
		int midPointY = bufferHeight * 0.5f;

		DrawLine(0, 0, midPointX, midPointY, 0xFF5516, m_Swapchain.backBuffer);
		DrawLine(bufferWidth, 0, midPointX, midPointY, 0x40EA76, m_Swapchain.backBuffer);
		DrawLine(0, bufferHeight, midPointX, midPointY, 0x40DBEA, m_Swapchain.backBuffer);
		DrawLine(bufferWidth, bufferHeight, midPointX, midPointY, 0xC0AA0B, m_Swapchain.backBuffer);
	}
}

int main()
{
	MiniRenderer::Renderer renderer(MiniRenderer::WindowProperties{});
	renderer.SetTargetFPS(60);	// Cap the FPS at 60 for testing.
	renderer.EnableDoubleBuffers(false);	// You have the option to disable buffer swapping.
	try
	{
		renderer.Run();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}