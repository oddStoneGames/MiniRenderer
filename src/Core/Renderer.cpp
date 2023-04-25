#include "Renderer.h"
#include "LineRenderer.h"
#include "TriangleRenderer.h"

#include <iostream>
#include <chrono>		// For Time related queries.
#include <stdlib.h>		// For EXIT_FAILURE & EXIT_SUCCESS.
#include <stdexcept>	// For std::runtime_error().

namespace MiniRenderer
{
	Renderer::Renderer(const WindowProperties& props, short unsigned int targetFPS, bool doubleBuffer)
		: m_Swapchain(props.Width, props.Height), m_TargetFPS(targetFPS), m_DoubleBuffer(doubleBuffer), 
		  m_TestModel(PROJECT_DIR"/src/Assets/pyramid.obj"), m_Camera(Vec3f(0.0f, 0.0f, 5.0f)), 
		  m_LastX(props.Width * 0.5f), m_LastY(props.Height * 0.5f), m_FirstMouse(true), m_RightClickHeld(false)
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

				// Update Camera Key Input.
				SendWSADInput();

				// Clear Backbuffer.
				m_Swapchain.backBuffer.Clear();

				// Get Time before rendering.
				auto timeBeforeRendering = std::chrono::high_resolution_clock::now();

				Render();

				// Get Time after rendering.
				auto timeAfterRendering = std::chrono::high_resolution_clock::now();

				// Get the Time it took to render this frame in miliseconds.
				long long start = std::chrono::time_point_cast<std::chrono::microseconds>(timeBeforeRendering).time_since_epoch().count();
				long long end = std::chrono::time_point_cast<std::chrono::microseconds>(timeAfterRendering).time_since_epoch().count();
				m_DeltaTime = (float)(end - start) / 1000.0f;

				// The Target Frame time.
				float targetFrameTime = (1.0f / m_TargetFPS) * 1000.0f;

				if (m_DeltaTime < targetFrameTime)
				{
					// We are faster than we are supposed to be so we should wait for some time.
					m_WaitTime = targetFrameTime - m_DeltaTime;
					m_TimeWhenWeStartedWaiting = end;
				}
				else
				{
					m_WaitTime = 0.0f;
				}
				
				// Show FPS
				// We waited for wait time to show this frame so the FPS should be calculated based upon (waittime + frametime)
				float frameTimeToDisplay = m_WaitTime + m_DeltaTime;
				float fpstoDisplay = (1.0f / frameTimeToDisplay) * 1000.0f;
				//printf("Frametime: %.2f ms\t FPS: %.2f\n", frameTimeToDisplay, fpstoDisplay);
			}
			else
			{
				// Uncapped Rendering.

				// Update Camera Key Input.
				SendWSADInput();

				// Clear Backbuffer.
				m_Swapchain.backBuffer.Clear();

				// Get Time before rendering.
				auto timeBeforeRendering = std::chrono::high_resolution_clock::now();

				Render();

				// Get Time after rendering.
				auto timeAfterRendering = std::chrono::high_resolution_clock::now();

				// Get the Time it took to render this frame in miliseconds.
				long long start = std::chrono::time_point_cast<std::chrono::microseconds>(timeBeforeRendering).time_since_epoch().count();
				long long end = std::chrono::time_point_cast<std::chrono::microseconds>(timeAfterRendering).time_since_epoch().count();
				m_DeltaTime = (float)(end - start) / 1000.0f;
				float fps = (1.0f / m_DeltaTime) * 1000.0f;

				//printf("Frametime: %.2f ms\t FPS: %.2f\n", frameTime, fps);
			}
		}
	}

	void Renderer::Render()
	{
		// Draw rectangle in the center of the screen.
		//DrawRectangle();

		// Render Line.
		//DrawLines();

		// Render a Triangle.
		//Vec2i points[3] = { Vec2i(40, 200), Vec2i(80, 120), Vec2i(120, 200) };
		//DrawTriangle(points, 0x069C4F, m_Swapchain.backBuffer);

		// Render model.
		m_TestModel.Draw(m_Swapchain.backBuffer, m_Camera);
		//m_TestModel.DrawWireframe(m_Swapchain.backBuffer);

		// The Swapchain swaps the buffer if only our backbuffer is completed which we set manually.
		m_Swapchain.SetBackbufferState(true);

		// Swap the Buffers.
		m_Swapchain.SwapBuffers(m_Window.get(), m_DoubleBuffer);
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
			//printf("New Width:%d\tNew Height:%d\n", wr.width, wr.height);
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

			// Only Rotate Camera when Right Click is held.
			if (m_RightClickHeld)
			{
				float xpos = (float)mm.x;
				float ypos = (float)mm.y;

				if (m_FirstMouse)
				{
					m_LastX = xpos;
					m_LastY = ypos;
					m_FirstMouse = false;
				}

				float xoffset = xpos - m_LastX;
				float yoffset = m_LastY - ypos; // reversed since y-coordinates go from bottom to top

				m_Camera.ProcessMouseInput(xoffset, yoffset);
			}
			
			m_LastX = (float)mm.x;
			m_LastY = (float)mm.y;

			//printf("Mouse Moved: %d\t%d\n", mm.x, mm.y);
		}else if(e.GetType() == MouseEvents::MouseButtonDown)
		{
			// Mouse Button Down Event.
			const MouseButtonDownEvent& mm = static_cast<const MouseButtonDownEvent&>(e);
			if (mm.button == 3)
			{
				m_RightClickHeld = true;
				m_Window->RenderCursor(false);	// Hide Cursor
				m_Window->ConfineCursor(true);  // Confine Cursor to this Window.
			}
		}else
		{
			// Mouse Button Up Event.
			const MouseButtonUpEvent& mm = static_cast<const MouseButtonUpEvent&>(e);
			if (mm.button == 3)
			{
				m_RightClickHeld = false;
				m_Window->RenderCursor(true);	// Show Cursor
				m_Window->ConfineCursor(false);  // Free the cursor.
			}
		}
    }

    void Renderer::OnKeyEvent(const Event<KeyEvents> &e)
    {
		if(e.GetType() == KeyEvents::KeyDown)
		{
			// Key Down Event.
			const KeyDownEvent& kd = static_cast<const KeyDownEvent&>(e);

			if(kd.keycode == 87) m_WSADheld.w = 1;
			else if(kd.keycode == 83) m_WSADheld.s = 1;
			else if (kd.keycode == 65) m_WSADheld.a = 1;
			else if (kd.keycode == 68) m_WSADheld.d = 1;
		}else
		{
			// Key Up Event.
			// Key Down Event.
			const KeyUpEvent& ku = static_cast<const KeyUpEvent&>(e);
			if (ku.keycode == 87) m_WSADheld.w = 0;
			else if (ku.keycode == 83) m_WSADheld.s = 0;
			else if (ku.keycode == 65) m_WSADheld.a = 0;
			else if (ku.keycode == 68) m_WSADheld.d = 0;
		}
    }

	void Renderer::SendWSADInput()
	{
		if (m_WSADheld.w == 1)
			m_Camera.ProcessKeyInput(CameraMovementDirection::Forward, m_DeltaTime);
		if (m_WSADheld.s == 1)
			m_Camera.ProcessKeyInput(CameraMovementDirection::Backward, m_DeltaTime);
		if (m_WSADheld.a == 1)
			m_Camera.ProcessKeyInput(CameraMovementDirection::Left, m_DeltaTime);
		if (m_WSADheld.d == 1)
			m_Camera.ProcessKeyInput(CameraMovementDirection::Right, m_DeltaTime);
	}

	void Renderer::DrawRectangle()
	{
		// Get the Middle of the Screen.
		int framebufferWidth = m_Swapchain.backBuffer.GetFramebufferWidth();
		int framebufferHeight = m_Swapchain.backBuffer.GetFramebufferHeight();
		int midX = framebufferWidth / 2;
		int midY = framebufferHeight / 2;

		// Rectangle size is 20% of the window width & 10% of the window height.
		int sizeX = framebufferWidth / 5, sizeY = framebufferHeight / 10;
		int startingPixelX = midX - sizeX / 2;
		int startingPixelY = midY - sizeY / 2;

		// Send draw command for every pixel.
		for (int x = startingPixelX; x <= startingPixelX + sizeX; x++)
			for (int y = startingPixelY; y <= startingPixelY + sizeY; y++)
				m_Swapchain.backBuffer.SetPixelColor(x, y, (y - x) * 0x00FFFF);
	}

	void Renderer::DrawLines()
	{
		int bufferWidth = m_Swapchain.backBuffer.GetFramebufferWidth();
		int bufferHeight = m_Swapchain.backBuffer.GetFramebufferHeight();
		int midPointX = bufferWidth / 2;
		int midPointY = bufferHeight / 2;

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
	renderer.EnableDoubleBuffers(true);	// You have the option to disable buffer swapping.
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