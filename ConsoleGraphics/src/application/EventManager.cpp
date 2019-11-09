#include "application/EventManager.h"

namespace cg
{
	bool EventManager::pollEvent(Event& e)
	{
		if (!sendEvent(e))
		{
			if (!getEvents())
				return false;

			return sendEvent(e);
		}

		return true;
	}

	bool EventManager::sendEvent(Event& e) noexcept
	{
		if (m_events.size())
		{
			e = translateEvent(m_events.front());
			m_events.pop();

			return true;
		}

		return false;
	}

	Event EventManager::translateEvent(const INPUT_RECORD& e) noexcept
	{
		Event event = {};

		if (KEY_EVENT == e.EventType)
			return translateKeyEvent(e);
		else if (MOUSE_EVENT == e.EventType)
			return translateMouseEvent(e);
		else
		{
			event.type = EventType::Raw;
			event.raw = RawEvent{ e };
		}

		return event;
	}

	Event EventManager::translateKeyEvent(const INPUT_RECORD& e) noexcept
	{
		Event event = {};
		if (e.Event.KeyEvent.bKeyDown)
			event.type = EventType::KeyPressed;
		else
			event.type = EventType::KeyReleased;

		KeyEvent key = {};
		key.key = e.Event.KeyEvent.uChar.UnicodeChar;
		key.scanCode = e.Event.KeyEvent.wVirtualScanCode;
		key.controlKeyState = e.Event.KeyEvent.dwControlKeyState;

		event.key = key;

		return event;
	}

	// TODO: Check all cases
	Event EventManager::translateMouseEvent(const INPUT_RECORD& e) noexcept
	{
		Event event = {};

		const auto buttonState = e.Event.MouseEvent.dwButtonState;
		if (buttonState)
		{
			event.type = EventType::MouseClick;

			MouseClickEvent click;
			if (FROM_LEFT_1ST_BUTTON_PRESSED & buttonState)
				click.button = MouseButton::Left;
			else if (RIGHTMOST_BUTTON_PRESSED & buttonState)
				click.button = MouseButton::Right;

			click.position = e.Event.MouseEvent.dwMousePosition;
			click.doubleClick = DOUBLE_CLICK & e.Event.MouseEvent.dwEventFlags;
			event.mouseClick = click;
		}
		else if (MOUSE_MOVED & e.Event.MouseEvent.dwEventFlags)
		{
			event.type = EventType::MouseMove;

			MouseMoveEvent move;
			move.position = e.Event.MouseEvent.dwMousePosition;

			event.mouseMove = move;
		}


		return event;
	}

	bool EventManager::getEvents()
	{
		assert(INVALID_HANDLE_VALUE != m_handle);

		DWORD nEvents;
		[[unlikely]]
		if (!::GetNumberOfConsoleInputEvents(m_handle, &nEvents))
			return false;

		if (nEvents > 0)
		{
			std::vector<INPUT_RECORD> events(nEvents);
			DWORD eventsRead;


			[[unlikely]]
			if (!::ReadConsoleInput(m_handle, events.data(), nEvents, &eventsRead))
				return false;

			[[unlikely]]
			if (0 == eventsRead)
				return false;

			for (auto e : events)
				m_events.push(e);

			return true;
		}

		return false;
	}

} // namespace cg
