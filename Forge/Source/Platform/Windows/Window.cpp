#include "Window.h"
#include <WindowsX.h>
#include <hidusage.h>
#include <Forge/Source/Core/Logging.h>
#include <Forge/Source/Core/Utility.h>
#include <Forge/Source/Core/Math/MathUtility.h>

namespace forge
{
	LRESULT CALLBACK Window::WndProc(HWND pHWnd, UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		Window *window = nullptr;

		if(pUMsg == WM_NCCREATE)
		{
			// The lParam parameter contains the CREATESTRUCTW structure. Cast it to the correct type
			CREATESTRUCTW *createStruct = reinterpret_cast<CREATESTRUCTW*>(pLParam);
			// Get the pointer to the Forge Window object that created this window. This must have been passed in the CreateWindowExW function
			window = reinterpret_cast<Window*>(createStruct->lpCreateParams);
			// Store a pointer to the Forge Window object in the instance data of the window. This can be retreived later using the function GetWindowLongPtr
			SetWindowLongPtrW(pHWnd, GWLP_USERDATA, (LONG_PTR)window);

			// Setting this pointer here even though the Forge Window object can use the return value from the CreateWindowExW function to set this.
			// This is done because the handleMessage function of the Forge Window object uses this member, and that function will be called before the CreateWindowExW function returns
			window->windowHandle = pHWnd;
		}
		else
		{
			// Get a pointer to the Forge Window object from the window
			LONG_PTR longPointer = GetWindowLongPtrW(pHWnd, GWLP_USERDATA);
			// Cast the pointer to the correct type
			window = reinterpret_cast<Window*>(longPointer);
		}

		if(window)
		{
			return window->handleMessage(pUMsg, pWParam, pLParam);
		}
		else
		{
			return DefWindowProcW(pHWnd, pUMsg, pWParam, pLParam);
		}
	}

	void Window::startup(HINSTANCE pHInstance, int pNCmdShow)
	{
		this->createWindow(pHInstance, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), true);

		this->registerRawMouseInput();

		ShowWindow(this->windowHandle, pNCmdShow);
	}

	void Window::startup(HINSTANCE pHInstance, int pNCmdShow, Int pResolutionWidth, Int pResolutionHeight)
	{
		this->createWindow(pHInstance, pResolutionWidth, pResolutionHeight, false);

		this->registerRawMouseInput();

		ShowWindow(this->windowHandle, pNCmdShow);
	}

	void Window::shutdown()
	{
		if(!DestroyWindow(this->windowHandle))
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("Failed to destroy window");
		}
	}

	bool Window::processMessages()
	{
		this->input.clear();

		MSG msg;
		// While a message is available
		while(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// If the message is WM_QUIT
			if(msg.message == WM_QUIT)
			{
				return false;
			}
			// If the message is something other than WM_QUIT
			else
			{
				TranslateMessage(&msg);

				// Calling this functions causes the window procedure function of the window class to be called
				DispatchMessageW(&msg);
			}
		}

		// Check if the mouse is in the client area
		{
			POINT cursorPosition;
			GetCursorPos(&cursorPosition);

			if(ScreenToClient(this->windowHandle, &cursorPosition) == 0)
			{
				FORGE_WINDOWS_ERROR_LOG("ClientToScreen function failed");
			}

			if(cursorPosition.x >= 0 &&
			   cursorPosition.y >= 0 &&
			   cursorPosition.x < this->resolutionWidth &&
			   cursorPosition.y < this->resolutionHeight)
			{
				this->input.isCursorInClientArea = true;
			}
			else
			{
				this->input.isCursorInClientArea = false;
			}
		}

		return true;
	}

	HWND Window::getWindowHandle()
	{
		return this->windowHandle;
	}

	void Window::blitFramebufferToScreen(Framebuffer &pFramebuffer)
	{
		HDC hdc = GetDC(this->windowHandle);

		if(pFramebuffer.getWidth() != this->resolutionWidth || this->resolutionHeight != pFramebuffer.getHeight())
		{
			if(!StretchBlt(hdc, 0, 0, this->resolutionWidth, this->resolutionHeight, pFramebuffer.getDeviceContext(), 0, 0, pFramebuffer.getWidth(), pFramebuffer.getHeight(), SRCCOPY))
			{
				FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("BitBlt function failed");
			}
		}
		else
		{
			if(!BitBlt(hdc, 0, 0, this->resolutionWidth, this->resolutionHeight, pFramebuffer.getDeviceContext(), 0, 0, SRCCOPY))
			{
				FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("BitBlt function failed");
			}
		}
		
		ReleaseDC(this->windowHandle, hdc);

		ValidateRect(this->windowHandle, NULL);
	}

	Float Window::remapAnalogAxisValue(SHORT pAxisValue)
	{
		if(pAxisValue > 0)
		{
			return static_cast<Float>(pAxisValue) / 32767.0f;
		}
		else
		{
			return static_cast<Float>(pAxisValue) / 32768.0f;
		}
	}

	LRESULT Window::handleMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		switch(pUMsg)
		{
		case WM_ACTIVATEAPP:
			return this->handleWM_ACTIVATEAPPMessage(pUMsg, pWParam, pLParam);

		case WM_KEYDOWN:
			return this->handleWM_KEYDOWNMessage(pUMsg, pWParam, pLParam);

		case WM_KEYUP:
			return this->handleWM_KEYUPMessage(pUMsg, pWParam, pLParam);

		case WM_LBUTTONDOWN:
			return this->handleWM_LBUTTONDOWNMessage(pUMsg, pWParam, pLParam);

		case WM_LBUTTONUP:
			return this->handleWM_LBUTTONUPMessage(pUMsg, pWParam, pLParam);

		case WM_MBUTTONDOWN:
			return this->handleWM_MBUTTONDOWNMessage(pUMsg, pWParam, pLParam);

		case WM_MBUTTONUP:
			return this->handleWM_MBUTTONUPMessage(pUMsg, pWParam, pLParam);

		case WM_RBUTTONDOWN:
			return this->handleWM_RBUTTONDOWNMessage(pUMsg, pWParam, pLParam);

		case WM_RBUTTONUP:
			return this->handleWM_RBUTTONUPMessage(pUMsg, pWParam, pLParam);

		case WM_MOUSEMOVE:
			return this->handleWM_MOUSEMOVEMessage(pUMsg, pWParam, pLParam);

		case WM_MOUSEWHEEL:
			return this->handleWM_MOUSEWHEELMessage(pUMsg, pWParam, pLParam);

		case WM_INPUT:
			return this->handleWM_INPUTMessage(pUMsg, pWParam, pLParam);

		case WM_CLOSE:
			return this->handleWM_CLOSEMessage(pUMsg, pWParam, pLParam);
		}

		return DefWindowProcW(this->windowHandle, pUMsg, pWParam, pLParam);
	}

	LRESULT Window::handleWM_ACTIVATEAPPMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		if(this->isFullscreen == false)
		{
			return 0;
		}

		switch(pWParam)
		{
		// This window became active
		case TRUE:
		{
			this->confineCursorToClientArea();
			return 0;
		}
		// This window became inactive
		case FALSE:
		{
			this->releaseCursorFromClientArea();
			return 0;
		}
		}

		return 0;
	}

	LRESULT Window::handleWM_KEYDOWNMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		switch(pWParam)
		{
		// 'W' key
		case 0x57:
		{
			this->input.isWKeyPressed = true;
			this->input.onWKeyPressed.invoke();
			return 0;
		}
		// 'A' key
		case 0x41:
		{
			this->input.isAKeyPressed = true;
			this->input.onAKeyPressed.invoke();
			return 0;
		}
		// 'H' key
		case 0x48:
		{
			this->input.isHKeyPressed = true;
			this->input.onHKeyPressed.invoke();
			return 0;
		}
		// 'S' key
		case 0x53:
		{
			this->input.isSKeyPressed = true;
			this->input.onSKeyPressed.invoke();
			return 0;
		}
		// 'D' key
		case 0x44:
		{
			this->input.isDKeyPressed = true;
			this->input.onDKeyPressed.invoke();
			return 0;
		}
		// 'R' key
		case 0x52:
		{
			this->input.isRKeyPressed = true;
			this->input.onRKeyPressed.invoke();
			return 0;
		}
		// 'Z' key
		case 0x5A:
		{
			this->input.isZKeyPressed = true;
			this->input.onZKeyPressed.invoke();
			return 0;
		}
		// 'G' key
		case 0x47:
		{
			this->input.isGKeyPressed = true;
			this->input.onGKeyPressed.invoke();
			return 0;
		}
		// 'B' key
		case 0x42:
		{
			this->input.isBKeyPressed = true;
			this->input.onBKeyPressed.invoke();
			return 0;
		}
		// 'V' key
		case 0x56:
		{
			this->input.isVKeyPressed = true;
			this->input.onVKeyPressed.invoke();
			return 0;
		}
		// '1' key
		case 0x31:
		{
			this->input.is1KeyPressed = true;
			this->input.on1KeyPressed.invoke();
			return 0;
		}
		// '2' key
		case 0x32:
		{
			this->input.is2KeyPressed = true;
			this->input.on2KeyPressed.invoke();
			return 0;
		}
		// Shift key
		case VK_SHIFT:
		{
			this->input.isShiftKeyPressed = true;
			this->input.onShiftKeyPressed.invoke();
			return 0;
		}
		// Control key
		case VK_CONTROL:
		{
			this->input.isControlKeyPressed = true;
			this->input.onControlKeyPressed.invoke();
			return 0;
		}
		}

		return 0;
	}

	LRESULT Window::handleWM_KEYUPMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		switch(pWParam)
		{
		// Escape key
		case VK_ESCAPE:
		{
			PostQuitMessage(0);
			return 0;
		}
		// 'W' key
		case 0x57:
		{
			this->input.isWKeyPressed = false;
			this->input.onWKeyReleased.invoke();
			return 0;
		}
		// 'A' key
		case 0x41:
		{
			this->input.isAKeyPressed = false;
			this->input.onAKeyReleased.invoke();
			return 0;
		}
		// 'H' key
		case 0x48:
		{
			this->input.isHKeyPressed = false;
			this->input.onHKeyReleased.invoke();
			return 0;
		}
		// 'S' key
		case 0x53:
		{
			this->input.isSKeyPressed = false;
			this->input.onSKeyReleased.invoke();
			return 0;
		}
		// 'D' key
		case 0x44:
		{
			this->input.isDKeyPressed = false;
			this->input.onDKeyReleased.invoke();
			return 0;
		}
		// 'R' key
		case 0x52:
		{
			this->input.isRKeyPressed = false;
			this->input.onRKeyReleased.invoke();
			return 0;
		}
		// 'Z' key
		case 0x5A:
		{
			this->input.isZKeyPressed = false;
			this->input.onZKeyReleased.invoke();
			return 0;
		}
		// 'G' key
		case 0x47:
		{
			this->input.isGKeyPressed = false;
			this->input.onGKeyReleased.invoke();
			return 0;
		}
		// 'B' key
		case 0x42:
		{
			this->input.isBKeyPressed = false;
			this->input.onBKeyReleased.invoke();
			return 0;
		}
		// 'V' key
		case 0x56:
		{
			this->input.isVKeyPressed = false;
			this->input.onVKeyReleased.invoke();
			return 0;
		}
		// '1' key
		case 0x31:
		{
			this->input.is1KeyPressed = false;
			this->input.on1KeyReleased.invoke();
			return 0;
		}
		// '2' key
		case 0x32:
		{
			this->input.is2KeyPressed = false;
			this->input.on2KeyReleased.invoke();
			return 0;
		}
		// Shift key
		case VK_SHIFT:
		{
			this->input.isShiftKeyPressed = false;
			this->input.onShiftKeyReleased.invoke();
			return 0;
		}
		// Control key
		case VK_CONTROL:
		{
			this->input.isControlKeyPressed = false;
			this->input.onControlKeyReleased.invoke();
			return 0;
		}
		}

		return 0;
	}

	LRESULT Window::handleWM_LBUTTONDOWNMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		this->input.isLeftMouseButtonPressed = true;
		this->input.onMouseLeftButtonDown.invoke(GET_X_LPARAM(pLParam), this->resolutionHeight - GET_Y_LPARAM(pLParam) - 1);
		return 0;
	}

	LRESULT Window::handleWM_LBUTTONUPMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		this->input.isLeftMouseButtonPressed = false;
		this->input.onMouseLeftButtonUp.invoke(GET_X_LPARAM(pLParam), this->resolutionHeight - GET_Y_LPARAM(pLParam) - 1);
		return 0;
	}

	LRESULT Window::handleWM_MBUTTONDOWNMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		this->input.isMiddleMouseButtonPressed = true;
		this->input.onMouseMiddleButtonDown.invoke(GET_X_LPARAM(pLParam), this->resolutionHeight - GET_Y_LPARAM(pLParam) - 1);
		return 0;
	}

	LRESULT Window::handleWM_MBUTTONUPMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		this->input.isMiddleMouseButtonPressed = false;
		this->input.onMouseMiddleButtonUp.invoke(GET_X_LPARAM(pLParam), this->resolutionHeight - GET_Y_LPARAM(pLParam) - 1);
		return 0;
	}

	LRESULT Window::handleWM_RBUTTONDOWNMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		SetCapture(this->windowHandle);
		this->input.isRightMouseButtonPressed = true;
		this->input.onMouseRightButtonDown.invoke(GET_X_LPARAM(pLParam), this->resolutionHeight - GET_Y_LPARAM(pLParam) - 1);
		return 0;
	}

	LRESULT Window::handleWM_RBUTTONUPMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		if(ReleaseCapture() == 0)
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("ReleaseCapture function failed");
		}
		this->input.isRightMouseButtonPressed = false;
		this->input.onMouseRightButtonUp.invoke(GET_X_LPARAM(pLParam), this->resolutionHeight - GET_Y_LPARAM(pLParam) - 1);
		return 0;
	}

	LRESULT Window::handleWM_MOUSEMOVEMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		this->input.currentCursorXCoord = static_cast<Int>(GET_X_LPARAM(pLParam));
		this->input.currentCursorYCoord = static_cast<Int>(this->resolutionHeight - GET_Y_LPARAM(pLParam) - 1);
		this->input.onMouseCursorMoved.invoke(GET_X_LPARAM(pLParam), this->resolutionHeight - GET_Y_LPARAM(pLParam) - 1);
		return 0;
	}

	LRESULT Window::handleWM_MOUSEWHEELMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		// Store the progress made towards the next mouse delta
		this->input.currentMouseScrollWheelDeltaCounter += GET_WHEEL_DELTA_WPARAM(pWParam);

		// Find the number of units the scroll wheel has been rotated and in what direction
		Int numberOfWheelDeltasCompleted = this->input.currentMouseScrollWheelDeltaCounter / WHEEL_DELTA;

		// If the units the scroll wheel has been rotated is greater than zero.
		// Taking the absolute value here because the delta can be negative
		if(abs(numberOfWheelDeltasCompleted) > 0)
		{
			this->input.onMouseScrollWheelRotated.invoke(numberOfWheelDeltasCompleted);

			// Remove the units of scroll wheel rotation that have been counted from the counter
			this->input.currentMouseScrollWheelDeltaCounter %= WHEEL_DELTA;
		}

		return 0;
	}

	LRESULT Window::handleWM_INPUTMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		UINT dataSize = sizeof(RAWINPUT);
		BYTE data[sizeof(RAWINPUT)];

		GetRawInputData((HRAWINPUT)pLParam, RID_INPUT, data, &dataSize, sizeof(RAWINPUTHEADER));

		RAWINPUT *rawInput = (RAWINPUT*)data;

		if(rawInput->header.dwType == RIM_TYPEMOUSE)
		{
			this->input.relativeMouseMovementXCoord += rawInput->data.mouse.lLastX;
			this->input.relativeMouseMovementYCoord += rawInput->data.mouse.lLastY;
			this->input.onMouseMoved.invoke(rawInput->data.mouse.lLastX, rawInput->data.mouse.lLastY);
		}

		return 0;
	}

	LRESULT Window::handleWM_CLOSEMessage(UINT pUMsg, WPARAM pWParam, LPARAM pLParam)
	{
		PostQuitMessage(0);
		return 0;
	}

	void Window::createWindow(HINSTANCE pHInstance, Int pResolutionWidth, Int pResolutionHeight, bool pIsFullscreen)
	{
		WNDCLASSEXW windowClass;

		// Fill the WNDCLASSEXW structure
		{
			windowClass.cbSize = sizeof(WNDCLASSEXW);
			windowClass.style = 0;
			windowClass.lpfnWndProc = WndProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = pHInstance;

			windowClass.hIcon = LoadIconW(NULL, IDI_APPLICATION);
			if(windowClass.hIcon == NULL)
			{
				FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("LoadIconW function failed");
			}

			windowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
			if(windowClass.hCursor == NULL)
			{
				FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("LoadCursorW function failed");
			}

			windowClass.hbrBackground = NULL;
			windowClass.lpszMenuName = NULL;
			windowClass.lpszClassName = this->windowClassName;

			windowClass.hIconSm = LoadIconW(NULL, IDI_APPLICATION);
			if(windowClass.hIconSm == NULL)
			{
				FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("LoadIconW function failed");
			}
		}

		// Register the window class.
		// It will be unregistered automatically when the window terminates, so there is no need to manually unregister it anywhere
		if(!RegisterClassExW(&windowClass))
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("Failed to register window class");
		}

		// A rectangle that specifies the required size of the client area
		RECT windowRectangle;
		windowRectangle.left = 0;
		windowRectangle.top = 0;
		windowRectangle.right = pResolutionWidth;
		windowRectangle.bottom = pResolutionHeight;

		DWORD selectedWindowStyle;
		DWORD selectedExtendedWindowStyle;
		if(pIsFullscreen)
		{
			selectedWindowStyle = WS_POPUP;
			selectedExtendedWindowStyle = 0;
		}
		else
		{
			selectedWindowStyle = WS_OVERLAPPEDWINDOW;
			selectedExtendedWindowStyle = 0;
		}

		// Get the rectangle that is the size of a window that has the desired client area
		if(!AdjustWindowRectEx(&windowRectangle, selectedWindowStyle, FALSE, selectedExtendedWindowStyle))
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("AdjustWindowRectEx function failed");
		}

		// The width of the entire window, including the client and non-client areas
		int windowWidth = static_cast<int>(windowRectangle.right - windowRectangle.left);
		// The height of the entire window, including the client and non-client areas
		int windowHeight = static_cast<int>(windowRectangle.bottom - windowRectangle.top);

		// The width of the resolution of the primary monitor
		const int primaryMonitorResolutionWidth = GetSystemMetrics(SM_CXSCREEN);
		// The height of the resolution of the primary monitor
		const int primaryMonitorResolutionHeight = GetSystemMetrics(SM_CYSCREEN);

		// Clamp the width and height of the window to make sure it will fit in the primary monitor
		{
			windowWidth = clamp<int>(windowWidth, 1, primaryMonitorResolutionWidth);
			windowHeight = clamp<int>(windowHeight, 1, primaryMonitorResolutionHeight);
		}

		// The x position of the window. This is set to a value that will put the window in the center of the screen
		const int windowPositionX = (primaryMonitorResolutionWidth - windowWidth) / 2;
		// The y position of the window. This is set to a value that will put the window in the center of the screen
		const int windowPositionY = (primaryMonitorResolutionHeight - windowHeight) / 2;

		// Create the window
		this->windowHandle = CreateWindowExW(selectedExtendedWindowStyle,
											 this->windowClassName,
											 L"Forge",
											 selectedWindowStyle,
											 windowPositionX,
											 windowPositionY,
											 windowWidth,
											 windowHeight,
											 NULL,
											 NULL,
											 pHInstance,
											 this);

		// If window creation failed
		if(this->windowHandle == NULL)
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("Failed to create window");
			return;
		}

		this->resolutionWidth = pResolutionWidth;
		this->resolutionHeight = pResolutionHeight;
		this->isFullscreen = pIsFullscreen;

		// Move the mouse cursor to the center of the client area if fullscreen mode is enabled
		if(pIsFullscreen)
		{
			this->setCursorCoordinates(this->resolutionWidth / 2 - 1, this->resolutionHeight / 2 - 1);
		}
	}

	void Window::registerRawMouseInput()
	{
		RAWINPUTDEVICE rawInputDevices[1];
		rawInputDevices[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		rawInputDevices[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		rawInputDevices[0].dwFlags = RIDEV_INPUTSINK;
		rawInputDevices[0].hwndTarget = this->windowHandle;

		if(RegisterRawInputDevices(rawInputDevices, 1, sizeof(rawInputDevices[0])) == FALSE)
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("Failed to register raw mouse input");
		}
	}

	void Window::confineCursorToClientArea()
	{
		// Get the current clip cursor rect and store it, so that it can be used to restore the clip cursor rect later
		if(!GetClipCursor(&this->previousClipCursorRect))
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("GetClipCursor function failed");
		}

		// Get the window's client area rect in client coordinates
		RECT clientRect;
		if(!GetClientRect(this->windowHandle, &clientRect))
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("GetClientRect function failed");
		}

		// Convert the client rect coordinates to screen coordinates
		POINT topLeftScreenCoords;
		topLeftScreenCoords.x = clientRect.left;
		topLeftScreenCoords.y = clientRect.top;
		POINT bottomRightScreenCoords;
		bottomRightScreenCoords.x = clientRect.right;
		bottomRightScreenCoords.y = clientRect.bottom;
		if(!ClientToScreen(this->windowHandle, &topLeftScreenCoords))
		{
			return;
		}
		if(!ClientToScreen(this->windowHandle, &bottomRightScreenCoords))
		{
			return;
		}

		// Create a new rect that holds the screen coordinates of the client area
		RECT screenRect;
		screenRect.left = topLeftScreenCoords.x;
		screenRect.top = topLeftScreenCoords.y;
		screenRect.right = bottomRightScreenCoords.x;
		screenRect.bottom = bottomRightScreenCoords.y;

		// Confine the cursor to the client area
		if(!ClipCursor(&screenRect))
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("ClipCursor function failed");
		}
	}

	void Window::releaseCursorFromClientArea()
	{
		// Restore the cursor area to the previous value
		ClipCursor(&this->previousClipCursorRect);
	}
}