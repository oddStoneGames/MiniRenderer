#pragma once
#include <cstdint>

namespace MiniRenderer
{
	/// @brief Buffer/Memory used to Hold Color & Alpha Values.
	class Framebuffer
	{
	public:
		Framebuffer(int x, int y, int width, int height);
		~Framebuffer();
		Framebuffer& operator =(const Framebuffer& other) = delete;

		/// @brief Copies the Color & Alpha buffers from the given framebuffer to this framebuffer.
		void CopyBuffers(const Framebuffer& from);

		/// @brief Makes a Frambuffer starting from Screen Coordinate (x,y) to (x+width,y+height).
		void SetFramebufferSize(int x, int y, int width, int height);

		/// @brief Width of Framebuffer.
		int GetFramebufferWidth() const { return m_Width; }
		/// @brief Height of Framebuffer.
		int GetFramebufferHeight() const { return m_Height; }

		/// @brief Sets the Clear Color & Alpha of this framebuffer.
		void SetClearColor(uint32_t clearColor, unsigned char clearAlpha = 255);
		/// @brief Clears the Framebuffer with the color used in last SetClearColor() call.
		void Clear();
		
		/// @brief Sets the Color of the Pixel at the coord (x,y) with the desired color & alpha value.
		void SetPixelColor(int x, int y, uint32_t color, unsigned char alpha = 255);

		/* All The Pixels on the Screen & their colors.
		   A Single Pixel Contains Color data in this Format
			   R  G  B  / Padding
		   0x 00 00 00 / 00
		   where every color value ranges from 0 to 255.
		*/
		uint32_t* colorBuffer;

		/* The Alpha Value of all the pixels on the screen.
		   This value does not go to the window directly, this is used between two framebuffers for blending purposes.
		   Each Alpha Value in this buffer Ranges from 0 to 255.
		*/
		unsigned char* alphaBuffer;
	private:

		/// @brief Width of the Framebuffer.
		int m_Width;

		/// @brief Height of the Framebuffer.
		int m_Height;
		
		/// @brief Clear Color, Default is Black.
		uint32_t m_ClearColor = 0x000000;

		/// @brief Clear Alpha, Default is 255.
		unsigned char m_ClearAlpha = 0xFF;

		/// @brief Tells if this Framebuffer has initialized.
		bool m_Initialized;
	};
}