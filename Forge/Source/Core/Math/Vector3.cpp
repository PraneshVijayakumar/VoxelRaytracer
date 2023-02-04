#include "Vector3.h"
#include "Matrix3x3.h"

namespace forge
{
	Vector3& Vector3::operator*=(const Matrix3x3 &pMatrix3x3)
	{
		// Implementing this in terms of the * operator, and not the other way around, because this compound assignment operator needs to make a copy of itself.
		// It needs to make a copy because the x, y and z components are all used to calculate the new values for the x, y and z components.
		// If the components are assigned to as the values are calculated, then the new values will be used to calculate the values for the subsequent components.
		// That would yeild an incorrect result
		(*this) = (*this) * pMatrix3x3;
		return *this;
	}

	Vector3 operator*(const Vector3 &pVector3, const Matrix3x3 &pMatrix3x3)
	{
		Vector3 newVector3 = pVector3;
		newVector3.x = pVector3.x * pMatrix3x3[0][0] + pVector3.y * pMatrix3x3[1][0] + pVector3.z * pMatrix3x3[2][0];
		newVector3.y = pVector3.x * pMatrix3x3[0][1] + pVector3.y * pMatrix3x3[1][1] + pVector3.z * pMatrix3x3[2][1];
		newVector3.z = pVector3.x * pMatrix3x3[0][2] + pVector3.y * pMatrix3x3[1][2] + pVector3.z * pMatrix3x3[2][2];

		return newVector3;
	}
}