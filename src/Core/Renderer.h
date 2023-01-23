#include <stdlib.h>		// For EXIT_FAILURE & EXIT_SUCCESS
#include <stdexcept>	// For std::runtime_error()
#include <Config.h>		// Includes the Configuration for this project.

#ifdef WINDOWS
	#include <WindowsWindow.h>
#else
	#include <LinuxWindows.h>
#endif

namespace MiniRenderer
{
	class Renderer
	{
	public:
		Renderer(const WindowProperties& props);
		virtual ~Renderer();
		void Run();
	private:
		void Init();
		void RenderLoop();
		void Cleanup();
	private:
		bool m_Running = true;	// Tells if the Application is running.
	#ifdef WINDOWS
		std::unique_ptr<Window> m_Window;
	#else
		LinuxWindow m_Window;
	#endif
	};
}