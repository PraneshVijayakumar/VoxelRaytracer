#include "Camera.h"
#include <Forge/Source/Core/Math/MathUtility.h>
#include <Forge/Source/Core/Logging.h>
#include <Forge/Source/Core/NumericTypeInfo.h>

namespace forge
{
	Camera::Camera(Input *pInput) :
	input(pInput)
	{
		this->coordinateSpace.position = Vector3(0.0f, 80.0f, -200.0f);
		if(pInput->getIsShiftKeyPressed())
		{
			this->onShiftKeyPressed();
		}
		else
		{
			this->onShiftKeyReleased();
		}
		pInput->onShiftKeyPressed.addListener<Camera, &Camera::onShiftKeyPressed>(this);
		pInput->onShiftKeyReleased.addListener<Camera, &Camera::onShiftKeyReleased>(this);
		pInput->onMouseMoved.addListener<Camera, &Camera::onMouseMoved>(this);
	}

	Ray Camera::getCameraRayForPixel(Int pPixelXCoord, Int pPixelYCoord, Int pScreenResolutionWidth, Int pScreenResolutionHeight) const
	{
		const Float verticalViewExtent = std::tan(convertDegreesToRadians(this->verticalFieldOfViewInDegrees / 2.0f));
		const Float heightToWidthMultiplier = static_cast<Float>(pScreenResolutionWidth) / static_cast<Float>(pScreenResolutionHeight);
		const Float horizontalViewExtent = verticalViewExtent * heightToWidthMultiplier;

		Vector3 cameraSpaceVector;
		cameraSpaceVector.x = lerp<Float>(-horizontalViewExtent, horizontalViewExtent, static_cast<Float>(pPixelXCoord) / static_cast<Float>(pScreenResolutionWidth - 1));
		cameraSpaceVector.y = lerp<Float>(-verticalViewExtent, verticalViewExtent, static_cast<Float>(pPixelYCoord) / static_cast<Float>(pScreenResolutionHeight - 1));
		cameraSpaceVector.z = 1.0f;

		const Vector3 worldSpaceVector = cameraSpaceVector * this->coordinateSpace.orientation.getMatrix3x3();

		return Ray(this->coordinateSpace.position, worldSpaceVector.getNormalized(), NumericTypeInfo<Float>::maxValue());
	}

	void Camera::update(Float pDeltaTime)
	{
		this->coordinateSpace.orientation.resetToIdentity();
		this->coordinateSpace.orientation.rotateAboutObjectSpaceXAxis(this->currentAngleAboutLocalXAxis);
		this->coordinateSpace.orientation.rotateAboutWorldSpaceYAxis(this->currentAngleAboutGlobalYAxis);

		Vector3 movementDirection;
		if(this->input->getIsWKeyPressed())
		{
			movementDirection += this->coordinateSpace.orientation.getZAxis();
		}
		if(this->input->getIsSKeyPressed())
		{
			movementDirection -= this->coordinateSpace.orientation.getZAxis();
		}
		if(this->input->getIsAKeyPressed())
		{
			movementDirection -= this->coordinateSpace.orientation.getXAxis();
		}
		if(this->input->getIsDKeyPressed())
		{
			movementDirection += this->coordinateSpace.orientation.getXAxis();
		}

		Float movementDirectionMagnitude = movementDirection.getMagnitude();
		if(movementDirectionMagnitude > 0.0f)
		{
			movementDirection /= movementDirectionMagnitude;
			this->coordinateSpace.position += movementDirection * this->movementSpeed * pDeltaTime;
		}
	}

	void Camera::onShiftKeyPressed()
	{
		this->movementSpeed = 250.0f;
	}

	void Camera::onShiftKeyReleased()
	{
		this->movementSpeed = 10.0f;
	}

	void Camera::onMouseMoved(Int32 pXRelativeMovement, Int32 pYRelativeMovement)
	{
		if(!this->isMouseLookEnabled)
		{
			return;
		}

		if(abs(pXRelativeMovement) > 0)
		{
			// Update the horizontal look angle
			this->currentAngleAboutGlobalYAxis += 0.25f * static_cast<Float>(pXRelativeMovement);

			// Make sure the horizontal look angle is in the interval (-360.0f, 360.0f)
			{
				const Int64 numberOfSignedFullRevolutions = roundTowardsZeroFloatToInteger<Int64>(this->currentAngleAboutGlobalYAxis / 360.0f);
				
				if(abs<Int64>(numberOfSignedFullRevolutions) >= 1)
				{
					this->currentAngleAboutGlobalYAxis -= 360.0f * static_cast<Float>(numberOfSignedFullRevolutions);
				}
			}
		}

		if(abs(pYRelativeMovement) > 0)
		{
			// Update the vertical look angle
			this->currentAngleAboutLocalXAxis += 0.25f * static_cast<Float>(pYRelativeMovement);

			// Clamp the vertical look angle
			this->currentAngleAboutLocalXAxis = clamp<Float>(this->currentAngleAboutLocalXAxis, -89.0f, 89.0f);
		}
	}
}