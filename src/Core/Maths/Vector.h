#ifndef VECTOR_H
#define VECTOR_H

// SSE 4.2
#include <nmmintrin.h>
#include <assert.h>
#include <sstream>
#include <ostream>

#include <math.h>

namespace MiniRenderer
{
#pragma region Vector2

	// Vector 2 Class
	// Since __m64 is deprecated in x64 program we use scalar.
	template <class T> struct Vector2
	{
		// Constructors
		Vector2() : x(0), y(0) {}
		Vector2(T _x) : x(_x), y(_x) {}
		Vector2(T _x, T _y) : x(_x), y(_y) {}

		// Arithmetic Operators with float
		inline Vector2<T> operator *(float f) const { return Vector2<T>(x * f, y * f); }
		inline Vector2<T> operator /(float f) const { assert(f == 0.0f); return Vector2<T>(x / f, y / f); }

		// Arithmetic Assignment Operators with float
		inline const Vector2<T>& operator *=(float f) { x *= f; y *= f; return *this; }
		inline const Vector2<T>& operator /=(float f) { assert(f == 0.0f); x /= f; y /= f; return *this; }

		// Arithmetic Operators
		inline Vector2<T> operator +(const Vector2<T>& v) const { return Vector2<T>(x + v.x, y + v.y); }
		inline Vector2<T> operator -(const Vector2<T>& v) const { return Vector2<T>(x - v.x, y - v.y); }
		inline Vector2<T> operator *(const Vector2<T>& v) const { return Vector2<T>(x * v.x, y * v.y); }
		inline Vector2<T> operator /(const Vector2<T>& v) const { assert(v.length() == 0.0f); return Vector2<T>(x / v.x, y / v.y); }

		// Arithmetic Assignment Operators
		inline const Vector2<T>& operator +=(const Vector2<T>& v) { x += v.x; y += v.y; return *this; }
		inline const Vector2<T>& operator -=(const Vector2<T>& v) { x -= v.x; y -= v.y; return *this; }
		inline const Vector2<T>& operator *=(const Vector2<T>& v) { x *= v.x; y *= v.y; return *this; }
		inline const Vector2<T>& operator /=(const Vector2<T>& v) { assert(v.length() == 0.0f); x /= v.x; y /= v.y; return *this; }

		// Subscript/Array Index Operator
		inline T& operator[](int index) const { assert(index < 0 || index >= 2); return (&x)[index]; }

		/// @brief Returns the Length of this Vector2.
		inline float length() const { return std::sqrt(x * x + y * y); }

		/// @brief Scales this vector to unit length and then returns it as reference.
		inline Vector2<T>& normalize(T l = 1) { *this = (*this) * (l / length()); return *this; }

		/// @brief Returns the contents of this vector as a string.
		const inline std::string toString() const
		{
			std::ostringstream ss;
			ss << "(" << x << ", " << y << ")\n";
			return ss.str();
		}

		// Member Variables
		T x, y;
	};

	template <class T>
	std::ostream& operator<<(std::ostream& s, Vector2<T>& v)
	{
		s << "(" << v.x << ", " << v.y << ")\n";
		return s;
	}

	/// @brief Dot Product of two Vector2.
	template<class T>
	inline float Dot(Vector2<T>& v, Vector2<T>& u)
	{
		return (float)(v.x * u.x + v.y * u.y);
	}

	/// @brief Returns the given vector to scaled to unit length.
	template<class T>
	inline Vector2<T> Normalize(Vector2<T>& v)
	{
		return v.normalize();
	}

	/// @brief Returns the given vector to scaled to unit length.
	template<class T>
	inline Vector2<T> Normalize(const Vector2<T> v)
	{
		Vector2<T> res(v);
		v *= static_cast<T>(1) / v.length();
		return v;
	}

	/// @brief Two floating point values.
	typedef Vector2<float> Vec2f;

	/// @brief Two integer values.
	typedef Vector2<int>   Vec2i;

#pragma endregion

#pragma region Vec3i

	// SSE 4.2 Vector3 Integer
#ifdef __GNUC__
	struct __attribute__((aligned(16))) Vec3i
#else
	_MM_ALIGN16 struct Vec3i		//__declspec(align(16))
#endif
	{
		// Constructor
		Vec3i() : _mValue(_mm_setzero_si128()) {}
		Vec3i(int x) : _mValue(_mm_set_epi32(0, x, x, x)) {}
		Vec3i(int x, int y, int z) : _mValue(_mm_set_epi32(0, z, y, x)) {}
		Vec3i(__m128i m) : _mValue(m) {}

		// Arithmetic Operators with float
		inline Vec3i operator*(const int b) const { return _mm_mul_epi32(_mValue, _mm_set1_epi32(b)); }
		inline Vec3i operator/(const int b) const { assert(b == 0); return _mm_castps_si128(_mm_div_ps(_mm_castsi128_ps(_mValue), _mm_set1_ps((float)b))); }

		// Arithmetic Assignment Operators with float
		inline const Vec3i& operator*=(const int b) { _mValue = _mm_mul_epi32(_mValue, _mm_set1_epi32(b)); return *this; }
		inline const Vec3i& operator/=(const int b) { assert(b == 0); _mValue = _mm_castps_si128(_mm_div_ps(_mm_castsi128_ps(_mValue), _mm_set1_ps((float)b))); return *this; }

		// Arithmetic Operators
		inline Vec3i operator+(const Vec3i& b) const { return _mm_add_epi32(_mValue, b._mValue); }
		inline Vec3i operator-(const Vec3i& b) const { return _mm_sub_epi32(_mValue, b._mValue); }
		inline Vec3i operator*(const Vec3i& b) const { return _mm_mul_epi32(_mValue, b._mValue); }
		inline Vec3i operator/(const Vec3i& b) const { assert(b.length() == 0); return _mm_castps_si128(_mm_div_ps(_mm_castsi128_ps(_mValue), _mm_castsi128_ps(b._mValue))); }

		// Arithmetic Assignment Operators
		inline const Vec3i& operator+=(const Vec3i& b) { _mValue = _mm_add_epi32(_mValue, b._mValue); return *this; }
		inline const Vec3i& operator-=(const Vec3i& b) { _mValue = _mm_sub_epi32(_mValue, b._mValue); return *this; }
		inline const Vec3i& operator*=(const Vec3i& b) { _mValue = _mm_mul_epi32(_mValue, b._mValue); return *this; }
		inline const Vec3i& operator/=(const Vec3i& b) { assert(b.length() == 0); _mValue = _mm_castps_si128(_mm_div_ps(_mm_castsi128_ps(_mValue), _mm_castsi128_ps(b._mValue))); return *this; }

		// Subscript/Array Index Operator
		inline int& operator[](int index) { assert(index < 0 || index >= 3); return (&x)[index]; }

		// Returns the Length of this Vector.
		inline float length() const { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(_mm_castsi128_ps(_mValue), _mm_castsi128_ps(_mValue), 0x71))); }

		// Scales this vector to unit length and then returns it as reference.
		inline const Vec3i& normalize()
		{
			_mValue = _mm_castps_si128(_mm_mul_ps(_mm_castsi128_ps(_mValue), _mm_rsqrt_ps(_mm_dp_ps(_mm_castsi128_ps(_mValue), _mm_castsi128_ps(_mValue), 0x7F))));
			return *this;
		}

		/// @brief Returns the contents of this vector as a string.
		const inline std::string toString() const
		{
			std::ostringstream ss;
			ss << "(" << x << ", " << y << ", " << z << ")\n";
			return ss.str();
		}

		// Overload operators for enforcing correct alignment.
#ifdef PLATFORM_WINDOWS
		inline void* operator new[](size_t x) { return _aligned_malloc(x, 16); }
		inline void operator delete[](void* x) { if (x) _aligned_free(x); }
#endif
		// Member Variables
		union
		{
			struct { int x, y, z; };
			__m128i _mValue;
		};
	};

	/// @brief Returns the given vector to scaled to unit length.
	inline Vec3i Normalize(Vec3i& v) { return v.normalize(); }

	/// @brief Returns the given vector to scaled to unit length.
	inline Vec3i Normalize(const Vec3i v)
	{
		Vec3i res(v);
		res._mValue = _mm_castps_si128(_mm_mul_ps(_mm_castsi128_ps(res._mValue), _mm_rsqrt_ps(_mm_dp_ps(_mm_castsi128_ps(res._mValue), _mm_castsi128_ps(res._mValue), 0x7F))));
		return res;
	}

	/// @brief Returns the Dot Product of integer vectors a & b.
	inline float Dot(const Vec3i& a, const Vec3i& b)
	{
		return _mm_cvtss_f32(_mm_dp_ps(_mm_castsi128_ps(a._mValue), _mm_castsi128_ps(b._mValue), 0x71));
	}

	/// @brief Returns the Cross Product of integer vectors a & b.
	inline Vec3i Cross(const Vec3i& a, const Vec3i& b)
	{
		// Returns a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x

		/*__m128 _mm_shuffle_ps(__m128 lo, __m128 hi, _MM_SHUFFLE(hi3, hi2, lo1, lo0))
		* Interleave inputs into low 2 floats and high 2 floats of output. Basically
		*   out[0]=lo[lo0];
		*   out[1]=lo[lo1];
		*   out[2]=hi[hi2];
		*   out[3]=hi[hi3];
		* For example, _mm_shuffle_ps(a,a,_MM_SHUFFLE(i,i,i,i)) copies the float a[i] into all 4 output floats.
		*/

		// _mm_castps_si128 - Reinterprets the four single precision floating point values in a as four 32-bit integers, and vice versa. 
		// No conversion is performed.Does not map to any assembly instructions.

		return _mm_castps_si128(_mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(_mm_castsi128_ps(a._mValue), _mm_castsi128_ps(a._mValue), _MM_SHUFFLE(3, 0, 2, 1)),
			_mm_shuffle_ps(_mm_castsi128_ps(b._mValue), _mm_castsi128_ps(b._mValue), _MM_SHUFFLE(3, 1, 0, 2))),
			_mm_mul_ps(_mm_shuffle_ps(_mm_castsi128_ps(a._mValue), _mm_castsi128_ps(a._mValue), _MM_SHUFFLE(3, 1, 0, 2)),
				_mm_shuffle_ps(_mm_castsi128_ps(b._mValue), _mm_castsi128_ps(b._mValue), _MM_SHUFFLE(3, 0, 2, 1)))));
	}

#pragma endregion

#pragma region Vec3f

	// SSE 4.2 Vector3 Float
#ifdef __GNUC__
	struct __attribute__((aligned(16))) Vec3f
#else
	_MM_ALIGN16 struct Vec3f		//__declspec(align(16))
#endif
	{
		// Constructor
		Vec3f() : _mValue(_mm_setzero_ps()) {}
		Vec3f(float x) : _mValue(_mm_set_ps(0, x, x, x)) {}
		Vec3f(float x, float y, float z) : _mValue(_mm_set_ps(0, z, y, x)) {}
		Vec3f(__m128 m) : _mValue(m) {}

		// Arithmetic Operators with float
		inline Vec3f operator*(const float b) const { return _mm_mul_ps(_mValue, _mm_set1_ps(b)); }
		inline Vec3f operator/(const float b) const { assert(b != 0.0f); return _mm_div_ps(_mValue, _mm_set1_ps(b)); }

		// Arithmetic Assignment Operators with float
		inline const Vec3f& operator*=(const float b) { _mValue = _mm_mul_ps(_mValue, _mm_set1_ps(b)); return *this; }
		inline const Vec3f& operator/=(const float b) { assert(b != 0.0f); _mValue = _mm_div_ps(_mValue, _mm_set1_ps(b)); return *this; }

		// Arithmetic Operators
		inline Vec3f operator+(const Vec3f& b) const { return _mm_add_ps(_mValue, b._mValue); }
		inline Vec3f operator-(const Vec3f& b) const { return _mm_sub_ps(_mValue, b._mValue); }
		inline Vec3f operator*(const Vec3f& b) const { return _mm_mul_ps(_mValue, b._mValue); }
		inline Vec3f operator/(const Vec3f& b) const { assert(b.length() != 0.0f); return _mm_div_ps(_mValue, b._mValue); }

		// Arithmetic Assignment Operators
		inline const Vec3f& operator+=(const Vec3f& b) { _mValue = _mm_add_ps(_mValue, b._mValue); return *this; }
		inline const Vec3f& operator-=(const Vec3f& b) { _mValue = _mm_sub_ps(_mValue, b._mValue); return *this; }
		inline const Vec3f& operator*=(const Vec3f& b) { _mValue = _mm_mul_ps(_mValue, b._mValue); return *this; }
		inline const Vec3f& operator/=(const Vec3f& b) { assert(b.length() != 0.0f); _mValue = _mm_div_ps(_mValue, b._mValue); return *this; }

		// Subscript/Array Index Operator
		inline float& operator[](int index) { assert(index > 0 || index < 3); return (&x)[index]; }
		inline const float& operator[](int index) const { assert(index > 0 || index < 3); return (&x)[index]; }

		// Returns the Length of this Vector.
		inline float length() const { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(_mValue, _mValue, 0x71))); }

		// Scales this vector to unit length and then returns it as reference.
		inline const Vec3f& normalize() { _mValue = _mm_mul_ps(_mValue, _mm_rsqrt_ps(_mm_dp_ps(_mValue, _mValue, 0x7F))); return *this; }

		/// @brief Returns the contents of this vector as a string.
		const inline std::string toString() const
		{
			std::ostringstream ss;
			ss << "(" << x << ", " << y << ", " << z << ")\n";
			return ss.str();
		}

		// Overload operators for enforcing correct alignment.
#ifdef PLATFORM_WINDOWS
		inline void* operator new[](size_t x) { return _aligned_malloc(x, 16); }
		inline void operator delete[](void* x) { if (x) _aligned_free(x); }
#endif
		// Member Variables
		union
		{
			struct { float x, y, z; };
			__m128 _mValue;
		};
	};

	/// @brief Returns the given vector to scaled to unit length.
	inline Vec3f Normalize(Vec3f& v) { return v.normalize(); }

	/// @brief Returns the given vector to scaled to unit length.
	inline Vec3f Normalize(const Vec3f v)
	{
		Vec3f res(v);
		res._mValue = _mm_mul_ps(res._mValue, _mm_rsqrt_ps(_mm_dp_ps(res._mValue, res._mValue, 0x7F)));
		return res;
	}

	/// @brief Returns the Dot Product of two floating point vectors a & b.
	inline float Dot(const Vec3f& a, const Vec3f& b)
	{
		return _mm_cvtss_f32(_mm_dp_ps(a._mValue, b._mValue, 0x71));
	}

	/// @brief Returns the Cross Product of floating point vectors a & b.
	inline Vec3f Cross(const Vec3f& a, const Vec3f& b)
	{
		// Returns a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x

		/*__m128 _mm_shuffle_ps(__m128 lo, __m128 hi, _MM_SHUFFLE(hi3, hi2, lo1, lo0))
		* Interleave inputs into low 2 floats and high 2 floats of output. Basically
		*   out[0]=lo[lo0];
		*   out[1]=lo[lo1];
		*   out[2]=hi[hi2];
		*   out[3]=hi[hi3];
		* For example, _mm_shuffle_ps(a,a,_MM_SHUFFLE(i,i,i,i)) copies the float a[i] into all 4 output floats.
		*/
		return _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(a._mValue, a._mValue, _MM_SHUFFLE(3, 0, 2, 1)),
					  _mm_shuffle_ps(b._mValue, b._mValue, _MM_SHUFFLE(3, 1, 0, 2))), _mm_mul_ps(_mm_shuffle_ps(a._mValue, a._mValue, _MM_SHUFFLE(3, 1, 0, 2)),
					  _mm_shuffle_ps(b._mValue, b._mValue, _MM_SHUFFLE(3, 0, 2, 1))));
	}

#pragma endregion

#pragma region Vec4f

	// SSE 4.2 Vector4 Float
#ifdef __GNUC__
	struct __attribute__((aligned(16))) Vec4f
#else
	_MM_ALIGN16 struct Vec4f		//__declspec(align(16))
#endif
	{
		// Constructor
		Vec4f() : _mValue(_mm_setzero_ps()) {}
		Vec4f(float x) : _mValue(_mm_set_ps(x, x, x, x)) {}
		Vec4f(float x, float y, float z, float w) : _mValue(_mm_set_ps(w, z, y, x)) {}
		Vec4f(__m128 m) : _mValue(m) {}

		// Arithmetic Operators with float
		inline Vec4f operator*(const float b) const { return _mm_mul_ps(_mValue, _mm_set1_ps(b)); }
		inline Vec4f operator/(const float b) const { assert(b != 0.0f); return _mm_div_ps(_mValue, _mm_set1_ps(b)); }

		// Arithmetic Assignment Operators with float
		inline const Vec4f& operator*=(const float b) { _mValue = _mm_mul_ps(_mValue, _mm_set1_ps(b)); return *this; }
		inline const Vec4f& operator/=(const float b) { assert(b != 0.0f); _mValue = _mm_div_ps(_mValue, _mm_set1_ps(b)); return *this; }

		// Arithmetic Operators
		inline Vec4f operator+(const Vec4f& b) const { return _mm_add_ps(_mValue, b._mValue); }
		inline Vec4f operator-(const Vec4f& b) const { return _mm_sub_ps(_mValue, b._mValue); }
		inline Vec4f operator*(const Vec4f& b) const { return _mm_mul_ps(_mValue, b._mValue); }
		inline Vec4f operator/(const Vec4f& b) const { assert(b.length() != 0.0f); return _mm_div_ps(_mValue, b._mValue); }

		// Arithmetic Assignment Operators
		inline const Vec4f& operator+=(const Vec4f& b) { _mValue = _mm_add_ps(_mValue, b._mValue); return *this; }
		inline const Vec4f& operator-=(const Vec4f& b) { _mValue = _mm_sub_ps(_mValue, b._mValue); return *this; }
		inline const Vec4f& operator*=(const Vec4f& b) { _mValue = _mm_mul_ps(_mValue, b._mValue); return *this; }
		inline const Vec4f& operator/=(const Vec4f& b) { assert(b.length() != 0.0f); _mValue = _mm_div_ps(_mValue, b._mValue); return *this; }

		// Subscript/Array Index Operator
		inline float& operator[](int index) { assert(index > 0 || index < 4); return (&x)[index]; }

		// Returns the Length of this Vector.
		inline float length() const { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(_mValue, _mValue, 0xF1))); }

		// Scales this vector to unit length and then returns it as reference.
		inline const Vec4f& normalize() { _mValue = _mm_mul_ps(_mValue, _mm_rsqrt_ps(_mm_dp_ps(_mValue, _mValue, 0xFF))); return *this; }

		/// @brief Returns the contents of this vector as a string.
		const inline std::string toString() const
		{
			std::ostringstream ss;
			ss << "(" << x << ", " << y << ", " << z << ", " << w << ")\n";
			return ss.str();
		}

		// Overload operators for enforcing correct alignment.
#ifdef PLATFORM_WINDOWS
		inline void* operator new[](size_t x) { return _aligned_malloc(x, 16); }
		inline void operator delete[](void* x) { if (x) _aligned_free(x); }
#endif

			// Member Variables
			union
		{
			struct { float x, y, z, w; };
			__m128 _mValue;
		};
	};

	/// @brief Returns the given vector to scaled to unit length.
	inline Vec4f Normalize(Vec4f& v) { return v.normalize(); }

	/// @brief Returns the given vector to scaled to unit length.
	inline Vec4f Normalize(const Vec4f v)
	{
		Vec4f res(v);
		res._mValue = _mm_mul_ps(res._mValue, _mm_rsqrt_ps(_mm_dp_ps(res._mValue, res._mValue, 0x7F)));
		return res;
	}

	/// @brief Returns the Dot Product of two floating point vectors a & b.
	inline float Dot(const Vec4f& a, const Vec4f& b)
	{
		return _mm_cvtss_f32(_mm_dp_ps(a._mValue, b._mValue, 0xF1));
	}

#pragma endregion
}

#endif // VECTOR_H