#ifndef EVENT_H
#define EVENT_H

#include <Windows.h>

namespace cg
{
	enum class EventType
	{
		KeyPressed,
		KeyReleased,
		Mouse,
		Raw
	};

	struct RawEvent
	{
		INPUT_RECORD data;
	};

	struct KeyEvent
	{
		DWORD controlKeyState;
		wchar_t key;
	};

	struct MouseEvent
	{
		COORD position;
		DWORD buttonState;
		DWORD eventFlags;
	};

	struct Event
	{
		EventType type;
		union
		{
			RawEvent raw;
			KeyEvent key;
			MouseEvent mouse;
		};		
	};


} // namespace cg

#endif EVENT_H
