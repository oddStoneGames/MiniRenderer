#pragma once

#include <ostream>
#include <cmath>

namespace MiniRenderer
{
	template <class T> struct Vector2 
	{
		T x, y;
		Vector2() : x(0), y(0) {}
		Vector2(T x, T y) { this->x = x; this->y = y; }
		inline Vector2<T> operator +(const Vector2<T>& V) const { return Vector2<T>(x + V.x, y + V.y); }
		inline Vector2<T> operator -(const Vector2<T>& V) const { return Vector2<T>(x - V.x, y - V.y); }
		inline Vector2<T> operator *(float f)          const { return Vector2<T>(x * f, y * f); }
		float mag() const { return std::sqrt(x * x + y * y); }
		Vector2<T>& normalize(T l = 1) { *this = (*this) * (l / mag()); return *this; }
		template <class > friend std::ostream& operator<<(std::ostream& s, Vector2<T>& v);
	};

	template <class T>
	std::ostream& operator<<(std::ostream& s, Vector2<T>& v)
	{
		s << "(" << v.x << ", " << v.y << ")\n";
		return s;
	}

	template <class T> struct Vector3
	{
		T x, y, z;
		Vector3() : x(0), y(0), z(0) {}
		Vector3(T x, T y, T z) { this->x = x; this->y = y; this->z = z; }
		inline Vector3<T> operator +(const Vector3<T>& v) const { return Vector3<T>(x + v.x, y + v.y, z + v.z); }
		inline Vector3<T> operator -(const Vector3<T>& v) const { return Vector3<T>(x - v.x, y - v.y, z - v.z); }
		inline Vector3<T> operator *(float f)          const { return Vector3<T>(x * f, y * f, z * f); }
		float mag() const { return std::sqrt(x * x + y * y + z * z); }
		Vector3<T>& normalize(T l = 1) { *this = (*this) * (l / mag()); return *this; }
		template <class > friend std::ostream& operator<<(std::ostream& s, Vector3<T>& v);
	};

	template <class T> std::ostream& operator<<(std::ostream& s, Vector3<T>& v)
	{
		s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
		return s;
	}

	typedef Vector2<float> Vec2;
	typedef Vector2<int>   Vec2i;
	typedef Vector3<float> Vec3;
	typedef Vector3<int>   Vec3i;

	inline float Abs(float value)
	{ 
		return value < 0.0f ? value * -1.0f : value; 
	}

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
	inline Vector2<T>& Dot(Vector2<T>& v, Vector2<T>& u)
	{
		const Vector2<T>& dot = Vector2<T>(v.x * u.x, v.y * u.y);
		return dot;
	}

	template<class T>
	inline const Vector3<T>& Dot(const Vector3<T>& v, const Vector3<T>& u)
	{
		const Vector3<T> dot = Vector3<T>(v.x * u.x, v.y * u.y, v.z * u.z);
		return dot;
	}

	template<class T>
	inline const Vector3<T>& Cross(const Vector3<T>& v, const Vector3<T>& u)
	{
		const Vector3<T>& cross = Vector3<T>(v.y * u.z - v.z * u.y, v.z * u.x - v.x * u.z, v.x * u.y - v.y * u.x);
		return cross;
	}

}