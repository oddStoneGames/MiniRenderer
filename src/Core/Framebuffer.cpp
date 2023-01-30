#include "Framebuffer.h"
#include <memory>
#include <stdexcept>
#include <cstring>

namespace MiniRenderer
{
	Framebuffer::Framebuffer(int x, int y, int width, int height)
		: m_Width(width), m_Height(height), m_Initialized(false)
	{
		SetFramebufferSize(x, y, m_Width, m_Height);
	}

	Framebuffer::~Framebuffer()
	{
		// Free the Memory allocated.
		free(colorBuffer);
		free(alphaBuffer);
	}

	void Framebuffer::CopyBuffers(const Framebuffer& from)
	{
		std::memcpy(colorBuffer, from.colorBuffer, m_Width * m_Height * sizeof(uint32_t));
		std::memcpy(alphaBuffer, from.alphaBuffer, m_Width * m_Height * sizeof(unsigned char));
	}

	void Framebuffer::SetFramebufferSize(int x, int y, int width, int height)
	{
		if (width <= 0 || height <= 0) return;

		// Initialize Pixels to hold this many colors.
		m_Width = width;
		m_Height = height;
		
		if (m_Initialized)
		{
			// If Buffers were already initialized once, we only need to resize them.
			uint32_t* cB = (uint32_t*)realloc(colorBuffer, m_Width * m_Height * sizeof(uint32_t));
			if (cB == nullptr)
				throw std::runtime_error("Failed to resize color buffer.");
			else
				colorBuffer = cB;

			unsigned char* aB = (unsigned char*)realloc(alphaBuffer, m_Width * m_Height * sizeof(unsigned char));
			if (aB == nullptr)
				throw std::runtime_error("Failed to resize alpha buffer.");
			else
				alphaBuffer = aB;
		}
		else
		{
			// Allocate Memory.
			colorBuffer = (uint32_t*)malloc(m_Width * m_Height * sizeof(uint32_t));
			alphaBuffer = (unsigned char*)malloc(m_Width * m_Height * sizeof(unsigned char));
			if (colorBuffer != nullptr && alphaBuffer != nullptr)
				m_Initialized = true;
			else
				throw std::runtime_error("Failed to allocate buffer.");
		}

		// Set Default Values.
		Clear();
	}

	void Framebuffer::SetClearColor(uint32_t clearColor, unsigned char clearAlpha)
	{
		m_ClearColor = clearColor;
		m_ClearAlpha = clearAlpha;
	}

	void Framebuffer::Clear()
	{
		uint32_t* pixel = colorBuffer; // Get the first pixel's Color.
		unsigned char* alpha = alphaBuffer;	// Get the first pixel's alpha value.

		for (int pixelno = 0; pixelno < m_Width * m_Height; pixelno++)
		{
			*pixel++ = m_ClearColor;
			*alpha++ = m_ClearAlpha;
		}
	}

	void Framebuffer::SetPixelColor(int x, int y, uint32_t color, unsigned char alpha)
	{
		// Return if the x & y values are out of bounds.
		if (x > m_Width || x < 0 || y < 0 || y >= m_Height) return;

		// Pixel at (x,y).
		int position = y * m_Width + x;

		// If Position is out of bounds, then return.
		if (position > m_Width * m_Height) return;

		uint32_t* pixel = colorBuffer; // Get Color of the pixel at (0,0).
		unsigned char* alphaValue = alphaBuffer; // Get Alpha value of the pixel at (0,0).

		// Goto the pixel at (x,y) in our color & alpha buffer and set the values to the given one.
		pixel += position;
		alphaValue += position;
		*pixel = color;
		*alphaValue = alpha;
	}
}