#ifndef FORGE_CAMERA_H
#define FORGE_CAMERA_H

#include <Forge/Source/Core/Math/Ray.h>
#include <Forge/Source/Platform/Windows/Window.h>
#include <Forge/Source/Core/Math/CoordinateSpace.h>

namespace forge
{
	class Camera
	{
	public:

		Camera(Input *pInput);

		void setVerticalFieldOfView(Float pVerticalFieldOfViewInDegrees);

		Ray getCameraRayForPixel(Int pPixelXCoord, Int pPixelYCoord, Int pScreenResolutionWidth, Int pScreenResolutionHeight) const;

		bool getIsMouseLookEnabled() const;

		void setIsMouseLookEnabled(bool pIsMouseLookEnabled);

		Float getVerticalFieldOfViewInDegrees() const;

		void update(Float pDeltaTime);

		CoordinateSpace coordinateSpace;

	private:

		void onShiftKeyPressed();

		void onShiftKeyReleased();

		void onMouseMoved(Int32 pXRelativeMovement, Int32 pYRelativeMovement);

		// The vertical field of view, specified in degrees
		Float verticalFieldOfViewInDegrees = 60.0f;
		bool isMouseLookEnabled = false;
		Input *input = nullptr;
		Float movementSpeed = 0.0f;
		Float currentAngleAboutGlobalYAxis = 0.0f;
		Float currentAngleAboutLocalXAxis = 0.0f;
	};

	inline
	void Camera::setVerticalFieldOfView(Float pVerticalFieldOfViewInDegrees)
	{
		this->verticalFieldOfViewInDegrees = pVerticalFieldOfViewInDegrees;
	}

	inline
	bool Camera::getIsMouseLookEnabled() const
	{
		return this->isMouseLookEnabled;
	}

	inline
	void Camera::setIsMouseLookEnabled(bool pIsMouseLookEnabled)
	{
		this->isMouseLookEnabled = pIsMouseLookEnabled;
	}

	inline
	Float Camera::getVerticalFieldOfViewInDegrees() const
	{
		return this->verticalFieldOfViewInDegrees;
	}
}

#endif