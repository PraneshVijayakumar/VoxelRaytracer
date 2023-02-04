#ifndef FORGE_WINDOW_H
#define FORGE_WINDOW_H

#include "ForgeWindowsHeader.h"
#include <Forge/Source/Core/Forge.h>
#include "Framebuffer.h"
#include "Input.h"
#include "WindowsLogging.h"

namespace forge
{
	/// <summary>
	/// This class represents one window
	/// </summary>
	class Window final
	{
	public:

		/// <summary>
		/// The windows procedure for the window class
		/// </summary>
		static LRESULT CALLBACK WndProc(HWND pHWnd, UINT pUMsg, WPARAM pWParam, LPARAM pLParam);

		/// <summary>
		/// Creates the window and all the resources it needs, and then shows the window
		/// </summary>
		/// <param name="hInstance">Handle to the instance</param>
		/// <param name="nCmdShow">Controls how the windows is to be shown right after it is created</param>
		void startup(HINSTANCE pHInstance, int pNCmdShow);

		void startup(HINSTANCE pHInstance, int pNCmdShow, Int pResolutionWidth, Int pResolutionHeight);

		/// <summary>
		/// Destroys the window
		/// </summary>
		void shutdown();

		/// <summary>
		/// Handles all the available window events. Must be called exactly once every frame
		/// </summary>
		/// <returns>A value of true if a quit message was not received. A value of false if it was</returns>
		bool processMessages();

		HWND getWindowHandle();

		// Get the width of the client area resolution in pixels
		Int getResolutionWidth() const;

		// Get the height of the client area resolution in pixels
		Int getResolutionHeight() const;

		// Get a pointer to the Input object of this window.
		// The Input object contains all the input events and data
		Input* getInput();

		/// <summary>
		/// Blits a framebuffer to the screen
		/// </summary>
		void blitFramebufferToScreen(Framebuffer &pFramebuffer);

		// Show the mouse cursor
		void showCursor();
		// Hide the mouse cursor
		void hideCursor();

		/// <summary>
		/// Set the coordinates of the mouse cursor
		/// </summary>
		/// <param name="pXCoord">The new x coordinate</param>
		/// <param name="pYCoord">The new y coordinate</param>
		void setCursorCoordinates(Int pXCoord, Int pYCoord);

	private:

		static Float remapAnalogAxisValue(SHORT pAxisValue);

		// Handles a message from the window procedure
		LRESULT handleMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);

		// Called when the active window has changed
		LRESULT handleWM_ACTIVATEAPPMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);
		// Called when a key is pressed
		LRESULT handleWM_KEYDOWNMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);
		// Called when a key is released
		LRESULT handleWM_KEYUPMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);
		// Called when the left mouse button is pressed
		LRESULT handleWM_LBUTTONDOWNMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);
		// Called when the left mouse button is released
		LRESULT handleWM_LBUTTONUPMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);
		// Called when the middle mouse button is pressed
		LRESULT handleWM_MBUTTONDOWNMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);
		// Called when the middle mouse button is released
		LRESULT handleWM_MBUTTONUPMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);
		// Called when the right mouse button is pressed
		LRESULT handleWM_RBUTTONDOWNMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);
		// Called when the right mouse button is released
		LRESULT handleWM_RBUTTONUPMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);
		// Called when the mouse cursor moves
		LRESULT handleWM_MOUSEMOVEMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);
		// Called when the mouse wheel is rotated
		LRESULT handleWM_MOUSEWHEELMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);
		// Called when raw input is received
		LRESULT handleWM_INPUTMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);
		// Called when the window is closed
		LRESULT handleWM_CLOSEMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam);

		/// <summary>
		/// Creates the window
		/// </summary>
		/// <param name="pHInstance">The handle to the instance</param>
		void createWindow(HINSTANCE pHInstance, Int pResolutionWidth, Int pResolutionHeight, bool pIsFullscreen);

		void registerRawMouseInput();

		// Restricts the cursor from moving outside the client area of the window
		void confineCursorToClientArea();
		// Allows the cursor to move outside the client area of the window
		void releaseCursorFromClientArea();

		Int resolutionWidth;
		Int resolutionHeight;
		bool isFullscreen;
		LPCWSTR windowClassName = L"ForgeWindow";
		HWND windowHandle = NULL;

		// Used to store the previous clip cursor rect, so that it can be restored when the mouse cursor is released
		RECT previousClipCursorRect;

		Input input;
	};

	inline
	Int Window::getResolutionWidth() const
	{
		return this->resolutionWidth;
	}

	inline
	Int Window::getResolutionHeight() const
	{
		return this->resolutionHeight;
	}

	inline
	Input* Window::getInput()
	{
		return &this->input;
	}

	inline
	void Window::showCursor()
	{
		// A loop is needed here because the ShowCursor(TRUE) function call only increments the display count
		while(ShowCursor(TRUE) < 0)
		{
			continue;
		}
	}

	inline
	void Window::hideCursor()
	{
		// A loop is needed here because the ShowCursor(TRUE) function call only decrements the display count
		while(ShowCursor(FALSE) >= 0)
		{
			continue;
		}
	}

	inline
	void Window::setCursorCoordinates(Int pXCoord, Int pYCoord)
	{
		POINT cursorScreenCoords;
		cursorScreenCoords.x = pXCoord;
		// Need to flip the y coordinate here before passing it to ClientToScreen
		cursorScreenCoords.y = this->resolutionHeight - 1 - pYCoord;

		if(ClientToScreen(this->windowHandle, &cursorScreenCoords) == 0)
		{
			FORGE_WINDOWS_ERROR_LOG("ScreenToClient function failed");
		}

		if(!SetCursorPos(cursorScreenCoords.x, cursorScreenCoords.y))
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("SetCursorPos function failed");
		}
	}
}

#endif