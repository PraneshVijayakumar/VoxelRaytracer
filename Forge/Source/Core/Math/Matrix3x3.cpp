#include "Matrix3x3.h"
#include <cmath>
#include "MathUtility.h"

namespace forge
{
	void Matrix3x3::getRotateAboutXAxisMatrix(Matrix3x3 &pOutputMatrix, Float pAngleInDegrees)
	{
		Float cosOfAngle = std::cos(convertDegreesToRadians(pAngleInDegrees));
		Float sinOfAngle = std::sin(convertDegreesToRadians(pAngleInDegrees));

		// Set the elements for the first row
		pOutputMatrix.elements[0][0] = 1.0f;
		pOutputMatrix.elements[0][1] = 0.0f;
		pOutputMatrix.elements[0][2] = 0.0f;

		// Set the elements for the second row
		pOutputMatrix.elements[1][0] = 0.0f;
		pOutputMatrix.elements[1][1] = cosOfAngle;
		pOutputMatrix.elements[1][2] = sinOfAngle;

		// Set the elements for the third row
		pOutputMatrix.elements[2][0] = 0.0f;
		pOutputMatrix.elements[2][1] = -sinOfAngle;
		pOutputMatrix.elements[2][2] = cosOfAngle;
	}

	void Matrix3x3::getRotateAboutYAxisMatrix(Matrix3x3 &pOutputMatrix, Float pAngleInDegrees)
	{
		Float cosOfAngle = std::cos(convertDegreesToRadians(pAngleInDegrees));
		Float sinOfAngle = std::sin(convertDegreesToRadians(pAngleInDegrees));

		// Set the elements for the first row
		pOutputMatrix.elements[0][0] = cosOfAngle;
		pOutputMatrix.elements[0][1] = 0.0f;
		pOutputMatrix.elements[0][2] = -sinOfAngle;

		// Set the elements for the second row
		pOutputMatrix.elements[1][0] = 0.0f;
		pOutputMatrix.elements[1][1] = 1.0f;
		pOutputMatrix.elements[1][2] = 0.0f;

		// Set the elements for the third row
		pOutputMatrix.elements[2][0] = sinOfAngle;
		pOutputMatrix.elements[2][1] = 0.0f;
		pOutputMatrix.elements[2][2] = cosOfAngle;
	}

	Matrix3x3 operator*(const Matrix3x3 &pMatrix1, const Matrix3x3 &pMatrix2)
	{
		Matrix3x3 newMatrix3x3;

		// Calculate the elements for the first row
		newMatrix3x3.elements[0][0] = pMatrix1.elements[0][0] * pMatrix2.elements[0][0] + pMatrix1.elements[0][1] * pMatrix2.elements[1][0] + pMatrix1.elements[0][2] * pMatrix2.elements[2][0];
		newMatrix3x3.elements[0][1] = pMatrix1.elements[0][0] * pMatrix2.elements[0][1] + pMatrix1.elements[0][1] * pMatrix2.elements[1][1] + pMatrix1.elements[0][2] * pMatrix2.elements[2][1];
		newMatrix3x3.elements[0][2] = pMatrix1.elements[0][0] * pMatrix2.elements[0][2] + pMatrix1.elements[0][1] * pMatrix2.elements[1][2] + pMatrix1.elements[0][2] * pMatrix2.elements[2][2];

		// Calculate the elements for the second row
		newMatrix3x3.elements[1][0] = pMatrix1.elements[1][0] * pMatrix2.elements[0][0] + pMatrix1.elements[1][1] * pMatrix2.elements[1][0] + pMatrix1.elements[1][2] * pMatrix2.elements[2][0];
		newMatrix3x3.elements[1][1] = pMatrix1.elements[1][0] * pMatrix2.elements[0][1] + pMatrix1.elements[1][1] * pMatrix2.elements[1][1] + pMatrix1.elements[1][2] * pMatrix2.elements[2][1];
		newMatrix3x3.elements[1][2] = pMatrix1.elements[1][0] * pMatrix2.elements[0][2] + pMatrix1.elements[1][1] * pMatrix2.elements[1][2] + pMatrix1.elements[1][2] * pMatrix2.elements[2][2];

		// Calculate the elements for the third row	
		newMatrix3x3.elements[2][0] = pMatrix1.elements[2][0] * pMatrix2.elements[0][0] + pMatrix1.elements[2][1] * pMatrix2.elements[1][0] + pMatrix1.elements[2][2] * pMatrix2.elements[2][0];
		newMatrix3x3.elements[2][1] = pMatrix1.elements[2][0] * pMatrix2.elements[0][1] + pMatrix1.elements[2][1] * pMatrix2.elements[1][1] + pMatrix1.elements[2][2] * pMatrix2.elements[2][1];
		newMatrix3x3.elements[2][2] = pMatrix1.elements[2][0] * pMatrix2.elements[0][2] + pMatrix1.elements[2][1] * pMatrix2.elements[1][2] + pMatrix1.elements[2][2] * pMatrix2.elements[2][2];

		return newMatrix3x3;
	}
}