// Base class for Windows.
#pragma once

#include <memory>
#include "Framebuffer.h"

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
			uint32_t width = 640,
			uint32_t height = 460)
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
		virtual void Draw(const Framebuffer &framebuffer) = 0;
		virtual void OnClose() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		static std::unique_ptr<MiniWindow> Create(const WindowProperties& props = WindowProperties());
	};
}