#ifndef FORGE_COLOR_H
#define FORGE_COLOR_H

#include "Forge.h"
#include <Forge/Source/Core/Math/Vector3.h>
#include <Forge/Source/Core/Utility.h>

namespace forge
{
	// Represents a color
	class Color
	{
	public:

		Color() = default;

		Color(UInt8 pRed, UInt8 pGreen, UInt8 pBlue);

		UInt8 getRedUint8() const;

		void setRedUint8(UInt8 pRed);

		UInt8 getGreenUint8() const;

		void setGreenUint8(UInt8 pGreen);

		UInt8 getBlueUint8() const;

		void setBlueUint8(UInt8 pBlue);

		void setRGBUint8(UInt8 pRed, UInt8 pGreen, UInt8 pBlue);

		Vector3 getRGBVector3() const;

		UInt32 getUInt32() const;

	private:

		UInt8 red = 0;
		UInt8 green = 0;
		UInt8 blue = 0;
	};

	inline
	Color::Color(UInt8 pRed, UInt8 pGreen, UInt8 pBlue)
	{
		this->setRGBUint8(pRed, pGreen, pBlue);
	}

	inline
	UInt8 Color::getRedUint8() const
	{
		return this->red;
	}

	inline
	void Color::setRedUint8(UInt8 pRed)
	{
		this->red = pRed;
	}

	inline
	UInt8 Color::getGreenUint8() const
	{
		return this->green;
	}

	inline
	void Color::setGreenUint8(UInt8 pGreen)
	{
		this->green = pGreen;
	}

	inline
	UInt8 Color::getBlueUint8() const
	{
		return this->blue;
	}

	inline
	void Color::setBlueUint8(UInt8 pBlue)
	{
		this->blue = pBlue;
	}

	inline
	void Color::setRGBUint8(UInt8 pRed, UInt8 pGreen, UInt8 pBlue)
	{
		this->red = pRed;
		this->green = pGreen;
		this->blue = pBlue;
	}

	inline
	Vector3 Color::getRGBVector3() const
	{
		return Vector3(static_cast<Float>(this->red) / 255.0f, static_cast<Float>(this->green) / 255.0f, static_cast<Float>(this->blue) / 255.0f);
	}

	inline
	UInt32 Color::getUInt32() const
	{
		UInt32 colorUInt32 = bitwiseLeftShiftInteger<UInt32>(this->blue, 8);
		colorUInt32 |= bitwiseLeftShiftInteger<UInt32>(this->green, 16);
		colorUInt32 |= bitwiseLeftShiftInteger<UInt32>(this->red, 24);

		return colorUInt32;
	}
}

#endif