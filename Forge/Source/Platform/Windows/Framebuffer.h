#ifndef FORGE_FRAMEBUFFER_H
#define FORGE_FRAMEBUFFER_H

#include "ForgeWindowsHeader.h"
#include <Forge/Source/Core/Forge.h>
#include <Forge/Source/Core/Color.h>

namespace forge
{
	// TODO: Need to chage this name from Framebuffer to Texture. Or maybe the name Framebuffer is better?
	class Framebuffer
	{
	public:

		void startup(Int pWidth, Int pHeight);

		void shutdown();

		Int getWidth() const;

		Int getHeight() const;

		void setWidthAndHeight(Int pWidth, Int pHeight);

		HDC getDeviceContext();

		void clear();

		void getPixelColor(Int pXCoord, Int pYCoord, Color &pColor);

		void setPixelColor(Int pXCoord, Int pYCoord, Color pColor);

		// Get a pointer to the array of pixels that make up this framebuffer
		UInt8* getPixelsMemoryPointer() const;

		// Get the size in bytes of the pixels memory
		SizeType getPixelsMemorySize() const;

		// Temporary function to draw text to the top left of the client area.
		// This can be used to display thr frame time or FPS until proper text rendering is implemented
		void tempDrawText(Int pXCoord, Int pYCoord, const char *pTextToDraw, Int pNumberOfCharacters);
		
	private:

		SizeType getMemoryIndexForPixelCoords(Int pXCoord, Int pYCoord);

		Int width = 0;
		Int height = 0;
		HGDIOBJ previousBitmapHandle = NULL;
		HBITMAP bitmapHandle = NULL;
		UInt8 *bitmapMemory = nullptr;
		HDC deviceContext = NULL;
		// The size in bytes of the bitmap memory
		SizeType bitmapMemorySize = 0;

		HGDIOBJ originalTemporaryFont = NULL;
		// Temporary font used for temporary text rendering
		HFONT temporaryFont = NULL;
	};

	inline
	void Framebuffer::shutdown()
	{
		this->setWidthAndHeight(0, 0);

		// TODO: Should this be deleted before or after the device context is destroyed?
		//       This is probably OK, as it is happening after the original font has been selected back in
		DeleteObject(this->temporaryFont);
	}

	inline
	Int Framebuffer::getWidth() const
	{
		return this->width;
	}

	inline
	Int Framebuffer::getHeight() const
	{
		return this->height;
	}

	inline
	HDC Framebuffer::getDeviceContext()
	{
		return this->deviceContext;
	}

	inline
	void Framebuffer::clear()
	{
		BitBlt(this->deviceContext, 0, 0, this->width, this->height, NULL, NULL, NULL, BLACKNESS);
	}

	inline
	void Framebuffer::getPixelColor(Int pXCoord, Int pYCoord, Color &pColor)
	{
		const SizeType index = this->getMemoryIndexForPixelCoords(pXCoord, pYCoord);

		pColor.setBlueUint8(this->bitmapMemory[index]);
		pColor.setGreenUint8(this->bitmapMemory[index + 1]);
		pColor.setRedUint8(this->bitmapMemory[index + 2]);
	}

	inline
	void Framebuffer::setPixelColor(Int pXCoord, Int pYCoord, Color pColor)
	{
		const SizeType index = this->getMemoryIndexForPixelCoords(pXCoord, pYCoord);

		// Set the colors for the pixel
		this->bitmapMemory[index] = pColor.getBlueUint8();
		this->bitmapMemory[index + 1] = pColor.getGreenUint8();
		this->bitmapMemory[index + 2] = pColor.getRedUint8();
	}

	inline
	UInt8* Framebuffer::getPixelsMemoryPointer() const
	{
		return this->bitmapMemory;
	}

	inline
	SizeType Framebuffer::getPixelsMemorySize() const
	{
		return this->bitmapMemorySize;
	}

	inline
	SizeType Framebuffer::getMemoryIndexForPixelCoords(Int pXCoord, Int pYCoord)
	{
		// Calculate the index of the first color value for the given pixel.
		// Casting all the integers to sizeType here, in order to avoid overflow
		return (static_cast<SizeType>(this->width) * static_cast<SizeType>(4) * static_cast<SizeType>(pYCoord)) +
			   (static_cast<SizeType>(4) * static_cast<SizeType>(pXCoord));
	}
}

#endif