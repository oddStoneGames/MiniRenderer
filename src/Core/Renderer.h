#include <stdlib.h>		// For EXIT_FAILURE & EXIT_SUCCESS
#include <stdexcept>	// For std::runtime_error()
#include <Config.h>		// Includes the Configuration for this project.

#ifdef WINDOWS
	#include <WindowsWindow.h>
#else
	#include <LinuxWindow.h>
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
		std::unique_ptr<MiniWindow> m_Window;
	};
}
