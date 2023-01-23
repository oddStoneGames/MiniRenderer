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
			m_Window->OnUpdate();

			// // TEMP!
			// if (!static_cast<WindowsWindow*>(m_Window->GetNativeWindow())->ProcessMessages())
			// {
			// 	std::cout << "Closing Window!\n";
			// 	m_Running = false;
			// }

			// // Render
			// Sleep(10);
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
		throw std::runtime_error(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}