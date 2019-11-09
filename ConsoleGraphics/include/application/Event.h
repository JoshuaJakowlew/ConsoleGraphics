#ifndef EVENT_H
#define EVENT_H

#include <Windows.h>

namespace cg
{
	enum class EventType
	{
		KeyPressed,
		KeyReleased,
		MouseMove,
		MouseClick,
		Raw
	};

	struct RawEvent
	{
		INPUT_RECORD data;
	};

	struct KeyEvent
	{
		DWORD controlKeyState;
		WORD scanCode;
		wchar_t key;
	};

	enum class MouseButton
	{
		Left,
		Right
	};

	struct MouseClickEvent
	{
		COORD position;
		MouseButton button;
		bool doubleClick;
	};

	struct MouseMoveEvent
	{
		COORD position;
	};

	struct Event
	{
		EventType type;
		union
		{
			RawEvent raw;
			KeyEvent key;
			MouseMoveEvent mouseMove;
			MouseClickEvent mouseClick;
		};		
	};


} // namespace cg

#endif EVENT_H
