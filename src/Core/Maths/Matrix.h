#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"

#include <iostream>
#include <iomanip>
#include <memory>

namespace MiniRenderer
{
	template <size_t rows, size_t columns>
	class Matrix
	{
	public:
		Matrix() : m_Rows(rows), m_Columns(columns)
		{
			AllocateSpace();
		}

		Matrix(float identityScale) : m_Rows(rows), m_Columns(columns)
		{
			AllocateSpace();

			if (m_Rows != m_Columns) return;

			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					m_Buffer[i][j] = (i == j) ? identityScale : 0;
		}

		Matrix(const Matrix<rows, columns>& other) : m_Rows(rows), m_Columns(columns)
		{
			AllocateSpace();

			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					m_Buffer[i][j] = other.m_Buffer[i][j];
		}

		Matrix<rows, columns>& operator=(const Matrix<rows, columns>& other)
		{
			if (this == &other) return *this;

			if (m_Rows != other.m_Rows || m_Columns != other.m_Columns)
			{
				m_Buffer.release();

				m_Rows = other.m_Rows;
				m_Columns = other.m_Columns;
				AllocateSpace();
			}

			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					m_Buffer[i][j] = other.m_Buffer[i][j];

			return *this;
		}

		~Matrix() {}

		inline float& operator()(int x, int y) { return m_Buffer[x][y]; }
		inline const float operator()(int x, int y) const { return m_Buffer[x][y]; }

		Matrix<rows, columns>& operator+=(const Matrix<rows, columns>& other)
		{
			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					m_Buffer[i][j] += other.m_Buffer[i][j];

			return *this;
		}
		Matrix<rows, columns> operator+(const Matrix<rows, columns>& other)
		{
			Matrix<rows, columns> res(*this);

			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					res.m_Buffer[i][j] += other.m_Buffer[i][j];

			return res;
		}

		Matrix<rows, columns>& operator-=(const Matrix<rows, columns>& other)
		{
			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					m_Buffer[i][j] -= other.m_Buffer[i][j];

			return *this;
		}
		Matrix<rows, columns> operator-(const Matrix<rows, columns>& other)
		{
			Matrix<rows, columns> res(*this);

			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					res.m_Buffer[i][j] -= other.m_Buffer[i][j];

			return res;
		}

		template <size_t otherColumns>
		Matrix<rows, otherColumns>& operator*=(const Matrix<rows, otherColumns>& other)
		{
			Matrix<rows, otherColumns> mult;

			// Multiplying matrix a and b and storing in array mult.
			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < other.m_Columns; ++j)
					for (int k = 0; k < m_Columns; ++k)
						mult.m_Buffer[i][j] += m_Buffer[i][k] * other.m_Buffer[k][j];

			return (*this = mult);
		}

		template <size_t otherColumns>
		Matrix<rows, otherColumns> operator*(const Matrix<rows, otherColumns>& other)
		{
			Matrix<rows, otherColumns> mult;

			// Multiplying matrix a and b and storing in array mult.
			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < other.m_Columns; ++j)
					for (int k = 0; k < m_Columns; ++k)
						mult.m_Buffer[i][j] += m_Buffer[i][k] * other.m_Buffer[k][j];

			return mult;
		}

		Vec3f& operator*(Vec3f& v)
		{
			Vec4f temp(v.x, v.y, v.z, 1.0f);
			Vec4f mult(0);

			if (rows < 3) return v;

			// Multiplying matrix a and vector v and storing in array mult.
			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					mult[i] += m_Buffer[i][j] * temp[j];

			return Vec3f(mult.x, mult.y, mult.z);
		}

		Vec4f& operator*(Vec4f& v)
		{
			if (rows < 3) return v;

			Vec4f mult(0.0f);

			// Multiplying matrix a and vector v and storing in array mult.
			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					mult[i] += m_Buffer[i][j] * v[j];

			return mult;
		}

		Matrix<rows, columns>& operator*=(float)
		{
			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					m_Buffer[i][j] *= value;

			return *this;
		}
		Matrix<rows, columns> operator*(float)
		{
			Matrix res(*this);

			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					res.m_Buffer[i][j] *= value;

			return res;
		}

		Matrix<rows, columns>& operator/=(float)
		{
			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					m_Buffer[i][j] /= value;

			return *this;
		}
		Matrix<rows, columns> operator/(float)
		{
			Matrix res(*this);

			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					res.m_Buffer[i][j] /= value;

			return res;
		}

		float Determinant(const Matrix<rows, columns>& a, size_t n) const
		{
			// This operation is applicable only on square matrices.
			if (rows != columns) return 0.0f;

			float det = 0.0f;

			//  Base case : if matrix contains single element
			if (n == 1)
				return a.m_Buffer[0][0];

			Matrix<rows, columns> cofactors; // To store cofactors

			int sign = 1; // To store sign multiplier

			// Iterate for each element of first row
			for (int f = 0; f < n; f++)
			{
				// Getting Cofactor of a[0][f]
				Cofactor(a, cofactors, 0, f, n);

				det += sign * a.m_Buffer[0][f] * Determinant(a, n - 1);

				// terms are to be added with alternate sign
				sign = -sign;
			}

			return det;
		}

		void Zero()
		{
			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					m_Buffer[i][j] = 0.0f;
		}

		void Randomize(int max = 1024)
		{
			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					m_Buffer[i][j] = std::rand() % max;
		}

		Matrix<columns, rows> Transpose() const
		{
			Matrix<columns, rows> transpose;
			for (int i = 0; i < m_Rows; ++i) {
				for (int j = 0; j < m_Columns; ++j) {
					transpose.m_Buffer[j][i] = m_Buffer[i][j];
				}
			}
			return transpose;
		}
		Matrix<columns, rows>& Transpose()
		{
			Matrix<columns, rows> transpose;
			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					transpose.m_Buffer[j][i] = m_Buffer[i][j];

			return (*this = transpose);
		}

		void Cofactor(const Matrix<rows, columns>& soruce, Matrix<rows, columns>& cofactor, size_t p, size_t q, size_t n) const
		{
			// This operation is applicable only on square matrices.
			if (rows != columns)
				return;

			int i = 0, j = 0;

			// Looping for each element of the matrix
			for (int row = 0; row < n; row++)
			{
				for (int col = 0; col < n; col++)
				{
					//  Copying into cofactor matrix only those
					//  element which are not in given row and
					//  column
					if (row != p && col != q)
					{
						cofactor.m_Buffer[i][j++] = source.m_Buffer[row][col];

						// Row is filled, so increase row index and
						// reset col index
						if (j == n - 1)
						{
							j = 0;
							i++;
						}
					}
				}
			}

			return;
		}

		Matrix<rows, columns> Adjoint() const
		{
			// This operation is applicable only on square matrices.
			if (rows != columns) return *this;

			Matrix<rows, columns> adjoint;

			if (m_Rows == 1)
			{
				adjoint.m_Buffer[0][0] = 1;
				return adjoint;
			}

			int sign = 1;
			Matrix<rows, columns> cofactors;

			for (int i = 0; i < m_Rows; i++)
			{
				for (int j = 0; j < m_Columns; j++)
				{
					// Get cofactor of (*this)[i][j]
					Cofactor(*this, cofactors, i, j, m_Rows);

					// sign of adj[j][i] positive if sum of row
					// and column indexes is even.
					sign = ((i + j) % 2 == 0) ? 1 : -1;

					// Interchanging rows and columns to get the
					// transpose of the cofactor matrix
					adjoint.m_Buffer[j][i] = (sign) * (Determinant(cofactors, m_Rows - 1));
				}
			}

			return adjoint;
		}

		Matrix<rows, columns> Inverse() const
		{
			// This operation is applicable only on square matrices.
			if (rows != columns) return *this;

			// Find determinant of (*this)[][]
			float det = Determinant(*this, m_Rows);

			// There is no inverse for Singular Matrix.
			if (det == 0) return *this;

			// Find adjoint.
			Matrix<rows, columns> adjoint = Adjoint();

			Matrix<rows, columns> inverse;

			// Find Inverse using formula "inverse(A) =
			// adjoint(A)/determinant(A)"
			for (int i = 0; i < m_Rows; i++)
				for (int j = 0; j < m_Columns; j++)
					inverse.m_Buffer[i][j] = adjoint.m_Buffer[i][j] / det;

			return inverse;
		}

		Matrix<rows, columns>& Identity()
		{
			if (m_Rows != m_Columns) return *this;

			for (int i = 0; i < m_Rows; ++i)
				for (int j = 0; j < m_Columns; ++j)
					m_Buffer[i][j] = (i == j) ? 1 : 0;

			return *this;
		}

	private:
		size_t m_Rows, m_Columns;
		std::unique_ptr<std::unique_ptr<float[]>[]> m_Buffer;

		void AllocateSpace()
		{
			m_Buffer = std::make_unique<std::unique_ptr<float[]>[]>(m_Rows);
			for (int i = 0; i < m_Rows; i++)
				m_Buffer[i] = std::make_unique<float[]>(m_Columns);
		}
	};

	template <size_t rows, size_t columns>
	std::istream& operator>>(std::istream& is, Matrix<rows, columns>& m)
	{
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < columns; ++j)
				is >> m(i, j);

		return is;
	}

	template <size_t rows, size_t columns>
	std::ostream& operator<<(std::ostream& os, const Matrix<rows, columns>& m)
	{
		for (int i = 0; i < rows; ++i)
		{
			if (m(i, 0) >= 0.0f) os << " ";
			os << m(i, 0);
			for (int j = 1; j < columns; ++j)
			{
				float p = m(i, j);
				if (p >= 0.0f) os << " ";
				os << " " << std::fixed << std::setprecision(6) << p;
			}
			os << std::endl;
		}

		return os;
	}

	typedef Matrix<1, 2> Mat1x2;

	typedef Matrix<2, 1> Mat2x1;
	typedef Matrix<2, 2> Mat2;
	typedef Matrix<2, 2> Mat2x2;
	typedef Matrix<2, 3> Mat2x3;
	typedef Matrix<2, 4> Mat2x4;

	typedef Matrix<3, 1> Mat3x1;
	typedef Matrix<3, 2> Mat3x2;
	typedef Matrix<3, 3> Mat3;
	typedef Matrix<3, 3> Mat3x3;
	typedef Matrix<3, 4> Mat3x4;

	typedef Matrix<4, 1> Mat4x1;
	typedef Matrix<4, 2> Mat4x2;
	typedef Matrix<4, 3> Mat4x3;
	typedef Matrix<4, 4> Mat4;
	typedef Matrix<4, 4> Mat4x4;
}

#endif // !MATRIX_H
