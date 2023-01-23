#include "Renderer.h"

namespace MiniRenderer
{
	/// <summary>
	/// Create the Window & Context.
	/// </summary>
	/// <param name="props">Window Properties</param>
	Renderer::Renderer(const WindowProperties& props)
	{
	#ifdef WINDOWS
		m_Window = WindowsWindow::Create(props);
	#else
		m_Window = LinuxWindow::Create(props);
	#endif
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

	}

	void Renderer::RenderLoop()
	{
		while (m_Running)
		{
			// TEMP!
			if (!static_cast<WindowsWindow*>(m_Window->GetNativeWindow())->ProcessMessages())
			{
				std::cout << "Closing Window!\n";
				m_Running = false;
			}

			// Render
			Sleep(10);
		}
	}

	void Renderer::Cleanup()
	{

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
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}