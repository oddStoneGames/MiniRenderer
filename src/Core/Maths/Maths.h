#ifndef MATHS_H
#define MATHS_H

#include "Vector.h"
#include "Matrix.h"

#include <cmath>

namespace MiniRenderer
{
	inline int Abs(int value) { return value < 0 ? value * -1 : value; }
	inline float Abs(float value) { return value < 0.0f ? value * -1.0f : value; }

	template<class T>
	inline T Min(T a, T b)
	{
		return a < b ? a : b;
	}

	template<class T>
	inline T Max(T a, T b)
	{
		return a > b ? a : b;
	}
}

#endif // !MATHS_H