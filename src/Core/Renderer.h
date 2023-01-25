#include <stdlib.h>		// For EXIT_FAILURE & EXIT_SUCCESS
#include <stdexcept>	// For std::runtime_error()

#ifdef WINDOWS
	#include <WindowsWindow.h>
#else
	#include <LinuxWindow.h>
#endif

#include "Events/EventHandler.h"

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
		void OnWindowEvent(const Event<WindowEvents>& e);
		void OnMouseEvent(const Event<MouseEvents>& e);
		void OnKeyEvent(const Event<KeyEvents>& e);
	private:
		bool m_Running = true;	// Tells if the Application is running.
		std::unique_ptr<MiniWindow> m_Window;
	};
}
