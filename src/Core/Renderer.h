#ifdef PLATFORM_WINDOWS
	#include <WindowsWindow.h>
#else
	#include <LinuxWindow.h>
#endif

#include "Events/EventHandler.h"
#include "Swapchain.h"
#include "Model.h"

namespace MiniRenderer
{
	class Renderer
	{
	public:
		/// @brief Create the Window & Context with the given Window Properties.
		Renderer(const WindowProperties& props, short unsigned int targetFPS = 0, bool doubleBuffer = true);
		virtual ~Renderer();

		/// @brief Caps the FPS at the given value. Default value is 0 which means FPS is uncapped.
		void SetTargetFPS(short unsigned int targetFPS) { m_TargetFPS = targetFPS; }

		/// @brief If set to true, then 2 buffers are used for drawing on screen which heavily lowers screen tearing.
		/// Else, Window shows the buffer while its rendering.
		void EnableDoubleBuffers(bool doubleBuffer) { m_DoubleBuffer = doubleBuffer; }

		/// @brief Runs the renderer.
		void Run();
	private:
		/// @brief Adds Listeners to event.
		void Init();

		/// @brief Render Loop.
		void RenderLoop();

		/// @brief Frees up the taken memory and closes all the operations.
		void Cleanup();

		/// @brief Called when window events(resize or close) happen.
		void OnWindowEvent(const Event<WindowEvents>& e);

		/// @brief Called when mouse events(move, button down or button up) happen.
		void OnMouseEvent(const Event<MouseEvents>& e);

		/// @brief Called when keyboard events(key down or key up) happen.
		void OnKeyEvent(const Event<KeyEvents>& e);

		/// @brief Draws a Rectangle to the backbuffer.
		void DrawRectangle();

		/// @brief Draws some lines.
		void DrawLines();
	private:
		/// @brief Tells if the Application is running.
		bool m_Running = true;

		/// @brief Caps the FPS at this value, if 0, then FPS is uncapped.
		short unsigned int m_TargetFPS = 0;

		/// @brief If True, then 2 buffers are used for drawing on screen which heavily lowers screen tearing.
		bool m_DoubleBuffer = true;

		/// @brief Window where everything that renders is shown.
		std::unique_ptr<MiniWindow> m_Window;

		/// @brief Used for swapping the buffers to avoid screen tearing.
		Swapchain m_Swapchain;

		/// @brief To ensure that we don't render another frame instantly if we are capping FPS.
		float m_WaitTime = 0.0f;

		/// @brief The Time at which we started waiting.
		long long m_TimeWhenWeStartedWaiting = 0;

		/// @brief Test Model.
		Model m_TestModel;
	};
}
