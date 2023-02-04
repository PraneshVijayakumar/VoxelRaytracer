#ifndef FORGE_INPUT_H
#define FORGE_INPUT_H

#include <Forge/Source/Core/Event.h>

namespace forge
{
	class Window;

	// Class that acts as a container for all the input data and events. The only instance of this object is inside the Window class.
	// All the events in this class will only be invoked inside the processMessages function of the Window class
	class Input
	{
		friend class Window;

	public:

		bool getIsWKeyPressed() const;
		bool getIsAKeyPressed() const;
		bool getIsSKeyPressed() const;
		bool getIsDKeyPressed() const;
		bool getIsRKeyPressed() const;
		bool getIsZKeyPressed() const;
		bool getIsVKeyPressed() const;
		bool getIs1KeyPressed() const;
		bool getIsShiftKeyPressed() const;
		bool getIsControlKeyPressed() const;
		bool getIsLeftMouseButtonPressed() const;
		bool getIsMiddleMouseButtonPressed() const;
		bool getIsRightMouseButtonPressed() const;
		Int getCurrentCursorXCoord() const;
		Int getCurrentCursorYCoord() const;
		Int32 getCurrentRelativeMouseMovementXCoord() const;
		Int32 getCurrentRelativeMouseMovementYCoord() const;

		Float getGamepadLeftAnalogXAxis() const;
		Float getGamepadLeftAnalogYAxis() const;
		Float getGamepadRightAnalogXAxis() const;
		Float getGamepadRightAnalogYAxis() const;

		bool getIsCursorInClientArea() const;

		void clear();

		// Invoked when the 'H' key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> onHKeyPressed;
		// Invoked when the 'H' key is released
		Event<> onHKeyReleased;

		// Invoked when the 'W' key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> onWKeyPressed;
		// Invoked when the 'W' key is released
		Event<> onWKeyReleased;

		// Invoked when the 'A' key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> onAKeyPressed;
		// Invoked when the 'A' key is released
		Event<> onAKeyReleased;

		// Invoked when the 'S' key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> onSKeyPressed;
		// Invoked when the 'S' key is released
		Event<> onSKeyReleased;

		// Invoked when the 'D' key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> onDKeyPressed;
		// Invoked when the 'D' key is released
		Event<> onDKeyReleased;

		// Invoked when the 'R' key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> onRKeyPressed;
		// Invoked when the 'R' key is released
		Event<> onRKeyReleased;

		// Invoked when the 'Z' key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> onZKeyPressed;
		// Invoked when the 'Z' key is released
		Event<> onZKeyReleased;

		// Invoked when the 'G' key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> onGKeyPressed;
		// Invoked when the 'G' key is released
		Event<> onGKeyReleased;

		// Invoked when the 'B' key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> onBKeyPressed;
		// Invoked when the 'B' key is released
		Event<> onBKeyReleased;

		// Invoked when the 'V' key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> onVKeyPressed;
		// Invoked when the 'V' key is released
		Event<> onVKeyReleased;

		// Invoked when the '1' key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> on1KeyPressed;
		// Invoked when the '1' key is released
		Event<> on1KeyReleased;

		// Invoked when the '2' key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> on2KeyPressed;
		// Invoked when the '2' key is released
		Event<> on2KeyReleased;

		// Invoked when the shift key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> onShiftKeyPressed;
		// Invoked when the shift key is released
		Event<> onShiftKeyReleased;

		// Invoked when the control key is pressed. If the key is held down for long enough, this event will start repeating
		Event<> onControlKeyPressed;
		// Invoked when the control key is released
		Event<> onControlKeyReleased;

		// Invoked when the left mouse button is pressed. Parameters are as follows.
		// 1. x coordinate of the cursor position
		// 2. y coordinate of the cursor position
		Event<Int, Int> onMouseLeftButtonDown;

		// Invoked when the left mouse button is released. Parameters are as follows.
		// 1. x coordinate of the cursor position
		// 2. y coordinate of the cursor position
		Event<Int, Int> onMouseLeftButtonUp;

		// Invoked when the middle mouse button is pressed. Parameters are as follows.
		// 1. x coordinate of the cursor position
		// 2. y coordinate of the cursor position
		Event<Int, Int> onMouseMiddleButtonDown;

		// Invoked when the middle mouse button is released. Parameters are as follows.
		// 1. x coordinate of the cursor position
		// 2. y coordinate of the cursor position
		Event<Int, Int> onMouseMiddleButtonUp;

		// Invoked when the right mouse button is pressed. Parameters are as follows.
		// 1. x coordinate of the cursor position
		// 2. y coordinate of the cursor position
		Event<Int, Int> onMouseRightButtonDown;

		// Invoked when the right mouse button is released. Parameters are as follows.
		// 1. x coordinate of the cursor position
		// 2. y coordinate of the cursor position
		Event<Int, Int> onMouseRightButtonUp;

		// Invoked when the mouse cursor is moved. Parameters are as follows.
		// 1. Absolute x coordinate of the mouse movement
		// 2. Absolute y coordinate of the mouse movement
		Event<Int, Int> onMouseCursorMoved;

		// Invoked when the mouse is moved. Parameters are as follows.
		// 1. Relative x coordinate of the mouse movement
		// 2. Relative y coordinate of the mouse movement
		Event<Int32, Int32> onMouseMoved;

		// Invoked when the mouse scroll wheel is rotated. Parameters are as follows.
		// 1. The amount of units the scroll wheel was rotated.
		//	  The value is positive if the scroll wheel was rotated forward, and negative if the scroll wheel was rotated backward
		Event<Int> onMouseScrollWheelRotated;

	private:

		bool isHKeyPressed = false;
		bool isWKeyPressed = false;
		bool isAKeyPressed = false;
		bool isSKeyPressed = false;
		bool isDKeyPressed = false;
		bool isRKeyPressed = false;
		bool isZKeyPressed = false;
		bool isGKeyPressed = false;
		bool isBKeyPressed = false;
		bool isVKeyPressed = false;
		bool is1KeyPressed = false;
		bool is2KeyPressed = false;
		bool isControlKeyPressed = false;
		bool isShiftKeyPressed = false;
		bool isLeftMouseButtonPressed = false;
		bool isMiddleMouseButtonPressed = false;
		bool isRightMouseButtonPressed = false;
		Int currentCursorXCoord;
		Int currentCursorYCoord;
		Int32 relativeMouseMovementXCoord;
		Int32 relativeMouseMovementYCoord;
		Int currentMouseScrollWheelDeltaCounter = 0;
		bool isCursorInClientArea = false;
	};

	inline
	bool Input::getIsWKeyPressed() const
	{
		return this->isWKeyPressed;
	}

	inline
	bool Input::getIsAKeyPressed() const
	{
		return this->isAKeyPressed;
	}

	inline
	bool Input::getIsSKeyPressed() const
	{
		return this->isSKeyPressed;
	}

	inline
	bool Input::getIsDKeyPressed() const
	{
		return this->isDKeyPressed;
	}

	inline
	bool Input::getIsRKeyPressed() const
	{
		return this->isRKeyPressed;
	}

	inline
	bool Input::getIsZKeyPressed() const
	{
		return this->isZKeyPressed;
	}

	inline
	bool Input::getIsVKeyPressed() const
	{
		return this->isVKeyPressed;
	}

	inline
	bool Input::getIs1KeyPressed() const
	{
		return this->is1KeyPressed;
	}

	inline
	bool Input::getIsShiftKeyPressed() const
	{
		return this->isShiftKeyPressed;
	}

	inline
	bool Input::getIsControlKeyPressed() const
	{
		return this->isControlKeyPressed;
	}

	inline
	bool Input::getIsLeftMouseButtonPressed() const
	{
		return this->isLeftMouseButtonPressed;
	}

	inline
	bool Input::getIsMiddleMouseButtonPressed() const
	{
		return this->isMiddleMouseButtonPressed;
	}

	inline
	bool Input::getIsRightMouseButtonPressed() const
	{
		return this->isRightMouseButtonPressed;
	}

	inline
	Int Input::getCurrentCursorXCoord() const
	{
		return this->currentCursorXCoord;
	}

	inline
	Int Input::getCurrentCursorYCoord() const
	{
		return this->currentCursorYCoord;
	}

	inline
	Int32 Input::getCurrentRelativeMouseMovementXCoord() const
	{
		return this->relativeMouseMovementXCoord;
	}

	inline
	Int32 Input::getCurrentRelativeMouseMovementYCoord() const
	{
		return this->relativeMouseMovementYCoord;
	}

	inline
	bool Input::getIsCursorInClientArea() const
	{
		return this->isCursorInClientArea;
	}

	inline
	void Input::clear()
	{
		this->relativeMouseMovementXCoord = 0;
		this->relativeMouseMovementYCoord = 0;
	}
}

#endif