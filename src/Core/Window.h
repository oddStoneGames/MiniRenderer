// Base class for Windows.
#pragma once

#include <memory>

namespace MiniRenderer
{
	/// <summary>
	/// Common Window Properties
	/// </summary>
	struct WindowProperties
	{
		const char* Title;
		uint32_t Width;
		uint32_t Height;

		WindowProperties(const char* title = "Mini Renderer",
			uint32_t width = 800,
			uint32_t height = 600)
			: Title(title), Width(width), Height(height) {}
	};

	/// <summary>
	/// Interface representing a desktop system based Window
	/// </summary>
	class MiniWindow
	{
	public:
		virtual ~MiniWindow() {}

		virtual void OnUpdate() = 0;
		virtual void OnClose() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;

		// Window Attributes
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static std::unique_ptr<MiniWindow> Create(const WindowProperties& props = WindowProperties());
	};
}