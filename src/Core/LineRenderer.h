// Renders a line based on Bresenham's Line Rendering algorithm
#pragma once
#include <stdint.h>
#include <cmath>
#include <utility>
#include "Framebuffer.h"
#include <chrono>

namespace MiniRenderer
{
	/// @brief Draws a Line from the points x0, y0 to x1, y1 with the specified color in the supplied framebuffer.
	inline void DrawLine(int x0, int y0, int x1, int y1, uint32_t color, Framebuffer& buffer)
	{
		bool steep = false;
		if (std::abs(x0 - x1) < std::abs(y0 - y1))
		{
			std::swap(x0, y0);
			std::swap(x1, y1);
			steep = true;
		}

		if (x0 > x1)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		int dx = x1 - x0;
		int dy = y1 - y0;
		int derror = std::abs(dy) * 2;
		int error = 0;
		int y = y0;

		auto timeBeforeDrawingLine = std::chrono::high_resolution_clock::now();

		if (steep)
		{
			for (int x = x0; x <= x1; x++)
			{
				buffer.SetPixelColor(y, x, color);
				error += derror;
				if (error > dx)
				{
					y += y1 > y0 ? 1 : -1;
					error -= dx * 2;
				}
			}
		}
		else
		{
			for (int x = x0; x <= x1; x++)
			{
				buffer.SetPixelColor(x, y, color);
				error += derror;
				if (error > dx)
				{
					y += y1 > y0 ? 1 : -1;
					error -= dx * 2;
				}
			}
		}

		auto timeAfterDrawingLine = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(timeBeforeDrawingLine).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(timeAfterDrawingLine).time_since_epoch().count();
		float timeTaken = (float)(end - start) / 1000.0f;
	}
}