#ifndef FORGE_MATRIX3X3_H
#define FORGE_MATRIX3X3_H

#include <Forge/Source/Core/Forge.h>
#include "Vector3.h"

namespace forge
{
	// A 3x3 square matrix
	class Matrix3x3
	{
		friend Matrix3x3 operator*(const Matrix3x3 &pMatrix1, const Matrix3x3 &pMatrix2);

	public:

		// This class represents a row from the matrix. It is used as the return value for the subscript operator.
		// Subscripting an object of this type will return an element from the row that this class represents
		class NonConstMatrixRow
		{
		public:

			NonConstMatrixRow(Float *pColumnParam) :
			column(pColumnParam)
			{
			}

			Float& operator[](SizeType pColumnIndex)
			{
				return this->column[pColumnIndex];
			}

		private:

			Float *column;
		};

		// This class represents a row from the matrix. It is used as the return value for the const subscript operator.
		// Subscripting an object of this type will return an element from the row that this class represents
		class ConstMatrixRow
		{
		public:

			ConstMatrixRow(const Float *pColumnParam) :
			column(pColumnParam)
			{
			}

			const Float& operator[](SizeType pColumnIndex) const
			{
				return this->column[pColumnIndex];
			}

		private:

			const Float *column;
		};

		static void getRotateAboutXAxisMatrix(Matrix3x3 &pOutputMatrix, Float pAngleInDegrees);

		static void getRotateAboutYAxisMatrix(Matrix3x3 &pOutputMatrix, Float pAngleInDegrees);

		Matrix3x3();

		/// <summary>
		/// Returns a class representing a row from the matrix. Subscripting the return value will return an element from that row.
		/// This is the non const version
		/// </summary>
		/// <param name="rowIndex">The index of the row to return</param>
		/// <returns>A class that represents a row from the matrix</returns>
		NonConstMatrixRow operator[](SizeType pRowIndex);

		/// <summary>
		/// Returns a class representing a row from the matrix. Subscripting the return value will return an element from that row.
		/// This is the const version
		/// </summary>
		/// <param name="rowIndex">The index of the row to return</param>
		/// <returns>A class that represents a row from the matrix</returns>
		ConstMatrixRow operator[](SizeType pRowIndex) const;

		Matrix3x3& operator*=(const Matrix3x3 &pOtherMatrix);

		// Gets the first row of the matrix as a 3 component vector
		Vector3 getFirstRowVector3() const;

		// Gets the second row of the matrix as a 3 component vector
		Vector3 getSecondRowVector3() const;

		// Gets the third row of the matrix as a 3 component vector
		Vector3 getThirdRowVector3() const;

		void resetToIdentity();

		void orthogonalize();

	private:

		Float elements[3][3];
	};

	inline
	Matrix3x3::Matrix3x3()
	{
		this->resetToIdentity();
	}

	inline
	Vector3 Matrix3x3::getFirstRowVector3() const
	{
		return Vector3(this->elements[0][0], this->elements[0][1], this->elements[0][2]);
	}

	inline
	Vector3 Matrix3x3::getSecondRowVector3() const
	{
		return Vector3(this->elements[1][0], this->elements[1][1], this->elements[1][2]);
	}

	inline
	Vector3 Matrix3x3::getThirdRowVector3() const
	{
		return Vector3(this->elements[2][0], this->elements[2][1], this->elements[2][2]);
	}

	inline
	void Matrix3x3::resetToIdentity()
	{
		// Set the elements for the first row
		this->elements[0][0] = 1.0f;
		this->elements[0][1] = 0.0f;
		this->elements[0][2] = 0.0f;

		// Set the elements for the second row
		this->elements[1][0] = 0.0f;
		this->elements[1][1] = 1.0f;
		this->elements[1][2] = 0.0f;

		// Set the elements for the third row
		this->elements[2][0] = 0.0f;
		this->elements[2][1] = 0.0f;
		this->elements[2][2] = 1.0f;
	}

	inline
	Matrix3x3::NonConstMatrixRow Matrix3x3::operator[](SizeType pRowIndex)
	{
		return NonConstMatrixRow(this->elements[pRowIndex]);
	}

	inline
	Matrix3x3::ConstMatrixRow Matrix3x3::operator[](SizeType pRowIndex) const
	{
		return ConstMatrixRow(this->elements[pRowIndex]);
	}

	inline
	Matrix3x3& Matrix3x3::operator*=(const Matrix3x3 &pOtherMatrix)
	{
		// Implementing this in terms of the * operator, and not the other way around,
		// because this compound assignment operator needs to make a copy of the object being assigned to before assigning
		(*this) = (*this) * pOtherMatrix;
		return *this;
	}

	Matrix3x3 operator*(const Matrix3x3 &pMatrix1, const Matrix3x3 &pMatrix2);

	inline
	void Matrix3x3::orthogonalize()
	{
		Vector3 basisX = this->getFirstRowVector3();
		Vector3 basisY = this->getSecondRowVector3();
		Vector3 basisZ = this->getThirdRowVector3();

		basisX.normalize();

		basisY = basisY - basisX * Vector3::dot(basisY, basisX);

		basisY.normalize();

		basisZ = Vector3::cross(basisX, basisY);

		basisZ.normalize();

		(*this)[0][0] = basisX.x;
		(*this)[0][1] = basisX.y;
		(*this)[0][2] = basisX.z;

		(*this)[1][0] = basisY.x;
		(*this)[1][1] = basisY.y;
		(*this)[1][2] = basisY.z;

		(*this)[2][0] = basisZ.x;
		(*this)[2][1] = basisZ.y;
		(*this)[2][2] = basisZ.z;
	}
}

#endif