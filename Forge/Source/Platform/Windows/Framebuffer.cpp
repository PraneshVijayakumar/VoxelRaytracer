#include "Framebuffer.h"
#include "WindowsLogging.h"

namespace forge
{
	void Framebuffer::startup(Int pWidth, Int pHeight)
	{
		// Create the temporary font
		this->temporaryFont = CreateFontA(50,
										  0,
										  0,
										  0,
										  FW_DONTCARE,
										  FALSE,
										  FALSE,
										  FALSE,
										  ANSI_CHARSET,
										  OUT_DEFAULT_PRECIS,
										  CLIP_DEFAULT_PRECIS,
										  DEFAULT_QUALITY,
										  DEFAULT_PITCH | FF_DONTCARE,
										  NULL);

		this->setWidthAndHeight(pWidth, pHeight);
	}

	void Framebuffer::setWidthAndHeight(Int pWidth, Int pHeight)
	{
		// If the new width and height are the same as the current width and height, nothing needs to be done
		if(this->width == pWidth && this->height == pHeight)
		{
			return;
		}

		// Delete the existing bitmap and device context, if they exist
		if(this->bitmapHandle != NULL)
		{
			SelectObject(this->deviceContext, this->previousBitmapHandle);
			SelectObject(this->deviceContext, this->originalTemporaryFont);
			this->previousBitmapHandle = NULL;
			DeleteDC(this->deviceContext);
			this->deviceContext = NULL;
			DeleteObject(this->bitmapHandle);
			this->bitmapHandle = NULL;
			this->bitmapMemory = nullptr;
		}

		// If the new width and height are zero, nothing needs to be done
		if(pWidth == 0 && pHeight == 0)
		{
			return;
		}

		this->deviceContext = CreateCompatibleDC(NULL);
		if(this->deviceContext == NULL)
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("Failed to create backbuffer device context");
		}

		BITMAPINFO backbufferBitmapInfo;
		backbufferBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		backbufferBitmapInfo.bmiHeader.biWidth = pWidth;
		backbufferBitmapInfo.bmiHeader.biHeight = pHeight;
		backbufferBitmapInfo.bmiHeader.biPlanes = 1;
		backbufferBitmapInfo.bmiHeader.biBitCount = 32;
		backbufferBitmapInfo.bmiHeader.biCompression = BI_RGB;
		backbufferBitmapInfo.bmiHeader.biSizeImage = 0;
		backbufferBitmapInfo.bmiHeader.biXPelsPerMeter = 0;
		backbufferBitmapInfo.bmiHeader.biYPelsPerMeter = 0;
		backbufferBitmapInfo.bmiHeader.biClrUsed = 0;
		backbufferBitmapInfo.bmiHeader.biClrImportant = 0;

		void *tempBackbufferMemoryPointer = NULL;
		this->bitmapHandle = CreateDIBSection(this->deviceContext, &backbufferBitmapInfo, DIB_RGB_COLORS, &tempBackbufferMemoryPointer, NULL, 0);
		if(this->bitmapHandle == NULL)
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("CreateDIBSection function failed");
		}
		
		this->bitmapMemory = static_cast<UInt8*>(tempBackbufferMemoryPointer);

		this->previousBitmapHandle = SelectObject(this->deviceContext, this->bitmapHandle);

		this->width = pWidth;
		this->height = pHeight;
		this->bitmapMemorySize = pWidth * pHeight * sizeof(UInt32);

		if(this->temporaryFont != NULL)
		{
			this->originalTemporaryFont = SelectObject(this->deviceContext, this->temporaryFont);
		}
	}

	void Framebuffer::tempDrawText(Int pXCoord, Int pYCoord, const char *pTextToDraw, Int pNumberOfCharacters)
	{
		TextOutA(this->deviceContext, pXCoord, pYCoord, pTextToDraw, pNumberOfCharacters);
	}
}