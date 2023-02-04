#ifndef FORGE_ORIENTATION_H
#define FORGE_ORIENTATION_H

#include "Matrix3x3.h"

namespace forge
{
	// Represents an orientation in 3D
	class Orientation
	{
	public:

		// Get the normalized x axis basis vector of the orientation
		Vector3 getXAxis() const;

		// Get the normalized y axis basis vector of the orientation
		Vector3 getYAxis() const;

		// Get the normalized z axis basis vector of the orientation
		Vector3 getZAxis() const;

		// Get a rotation matrix that represents this orientation
		Matrix3x3 getMatrix3x3() const;

		void setMatrix3x3(const Matrix3x3 &pMatrix3x3);

		void resetToIdentity();

		/// <summary>
		/// Rotate the orientation about the object space x axis
		/// </summary>
		/// <param name="pAngleInDegrees">The angle that specifies the amount of rotation in degrees</param>
		void rotateAboutObjectSpaceXAxis(Float pAngleInDegrees);

		/// <summary>
		/// Rotate the orientation about the world space y axis
		/// </summary>
		/// <param name="pAngleInDegrees">The angle that specifies the amount of rotation in degrees</param>
		void rotateAboutWorldSpaceYAxis(Float pAngleInDegrees);

	private:

		// A rotation matrix that represents this orientation
		Matrix3x3 orientationMatrix;
	};

	inline
	Vector3 Orientation::getXAxis() const
	{
		return this->orientationMatrix.getFirstRowVector3();
	}

	inline
	Vector3 Orientation::getYAxis() const
	{
		return this->orientationMatrix.getSecondRowVector3();
	}

	inline
	Vector3 Orientation::getZAxis() const
	{
		return this->orientationMatrix.getThirdRowVector3();
	}

	inline
	Matrix3x3 Orientation::getMatrix3x3() const
	{
		return this->orientationMatrix;
	}

	inline
	void Orientation::setMatrix3x3(const Matrix3x3 &pMatrix3x3)
	{
		this->orientationMatrix = pMatrix3x3;

		this->orientationMatrix.orthogonalize();
	}

	inline
	void Orientation::resetToIdentity()
	{
		this->orientationMatrix.resetToIdentity();
	}

	inline
	void Orientation::rotateAboutObjectSpaceXAxis(Float pAngleInDegrees)
	{
		Matrix3x3 rotateAboutXAxisMatrix;
		Matrix3x3::getRotateAboutXAxisMatrix(rotateAboutXAxisMatrix, pAngleInDegrees);

		// Apply the object space rotation first
		this->orientationMatrix = rotateAboutXAxisMatrix * this->orientationMatrix;

		this->orientationMatrix.orthogonalize();
	}

	inline
	void Orientation::rotateAboutWorldSpaceYAxis(Float pAngleInDegrees)
	{
		Matrix3x3 rotateAboutYAxisMatrix;
		Matrix3x3::getRotateAboutYAxisMatrix(rotateAboutYAxisMatrix, pAngleInDegrees);

		this->orientationMatrix *= rotateAboutYAxisMatrix;

		this->orientationMatrix.orthogonalize();
	}
}

#endif