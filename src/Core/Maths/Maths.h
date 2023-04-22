#ifndef MATHS_H
#define MATHS_H

#include "Vector.h"
#include "Matrix.h"

#include <math.h>

namespace MiniRenderer
{
	const float pie = 3.14159f;

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

	template<class T>
	inline T ToRadians(T degrees)
	{
		return degrees * pie / static_cast<T>(180);
	}

	template<class T>
	inline T ToDegrees(T radians)
	{
		return radians * static_cast<T>(180) / pie;
	}

	inline void Translate(Mat4& m, const Vec3f& translation)
	{
		for (int i = 0; i < 3; i++)
			m(i, 3) += translation[i];
	}

	inline void Scale(Mat4& m, const Vec3f& scale)
	{
		m(0, 0) *= scale[0];
		m(1, 1) *= scale[1];
		m(2, 2) *= scale[2];
	}

	template<typename T>
	inline void Rotate(Mat4& m, T angle, Vec3f& axis)
	{
		T const a = angle;
		T const c = std::cos(a);
		T const s = std::sin(a);

		axis.normalize();

		Vec3f temp = axis * (1.0f - (float)c);

		Mat4 rotate(m);

		rotate(0, 0) = c + temp[0] * axis[0];
		rotate(0, 1) = temp[1] * axis[0] - axis[2] * s;
		rotate(0, 2) = temp[2] * axis[0] + axis[1] * s;

		rotate(1, 0) = temp[0] * axis[1] + axis[2] * s;
		rotate(1, 1) = c + temp[1] * axis[1];
		rotate(1, 2) = temp[1] * axis[2] - axis[0] * s;

		rotate(2, 0) = temp[0] * axis[2] - axis[1] * s;
		rotate(2, 1) = temp[1] * axis[2] + axis[0] * s;
		rotate(2, 2) = c + temp[2] * axis[2];

		m = m * rotate;
	}

	template<typename T>
	inline void Orthographic(Mat4& ortho, T right, T left, T top, T bottom, T near, T far)
	{
		T rightminusleft = right - left;
		T topminusbottom = top - bottom;
		T farminusnear = far - near;

		ortho.Identity();

		ortho(0, 0) = static_cast<T>(2) / rightminusleft;
		ortho(0, 3) = -(right + left) / rightminusleft;

		ortho(1, 1) = static_cast<T>(2) / topminusbottom;
		ortho(1, 3) = -(top + bottom) / topminusbottom;

		ortho(2, 2) = -static_cast<T>(2) / farminusnear;
		ortho(2, 3) = -(far + near) / farminusnear;
	}

	template<typename T>
	inline void Perspective(Mat4& perspective, T fovy, T aspect, T zNear, T zFar)
	{
		T const tanHalfFovy = tan(fovy / static_cast<T>(2));

		perspective.Identity();

		perspective(0, 0) = static_cast<T>(1) / (aspect * tanHalfFovy);
		perspective(1, 1) = static_cast<T>(1) / tanHalfFovy;
		perspective(2, 3) = -static_cast<T>(1);

		perspective(2, 2) = -(zFar + zNear) / (zFar - zNear);
		perspective(3, 2) = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
	}
}

#endif // !MATHS_H