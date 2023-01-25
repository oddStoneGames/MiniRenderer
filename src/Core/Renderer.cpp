#include "Renderer.h"

namespace MiniRenderer
{
	/// <summary>
	/// Create the Window & Context.
	/// </summary>
	/// <param name="props">Window Properties</param>
	Renderer::Renderer(const WindowProperties& props)
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

	/// <summary>
	/// Initialize!
	/// </summary>
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
			m_Window->OnUpdate();
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
			m_Running = false;
		}
    }

    void Renderer::OnMouseEvent(const Event<MouseEvents> &e)
    {
		if(e.GetType() == MouseEvents::MouseMoved)
		{
			// Mouse Moved Event.
			const MouseMovedEvent& mm = static_cast<const MouseMovedEvent&>(e);
			printf("Mouse Moved: %d\t%d\n", mm.x, mm.y);
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
}

int main()
{
	MiniRenderer::Renderer renderer(MiniRenderer::WindowProperties{});
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