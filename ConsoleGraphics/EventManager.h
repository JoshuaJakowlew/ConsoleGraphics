#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <queue>
#include <cassert>

#include <Windows.h>

#include "Event.h"

namespace cg
{
	class EventManager
	{
	public:
		EventManager() noexcept;
		EventManager(HANDLE handle) noexcept;

		void setHandle(HANDLE handle) noexcept;
		HANDLE getHandle() noexcept;

		[[nodiscard]] bool pollEvent(Event& e);
	private:
		HANDLE m_handle;
		std::queue<INPUT_RECORD> m_events;

		[[nodiscard]] bool sendEvent(Event& e) noexcept;
		[[nodiscard]] Event translateEvent(const INPUT_RECORD& e) noexcept;
		[[nodiscard]] Event translateKeyEvent(const INPUT_RECORD& e) noexcept;
		[[nodiscard]] Event translateMouseEvent(const INPUT_RECORD& e) noexcept;
		[[nodiscard]] bool getEvents();
	};

	inline EventManager::EventManager() noexcept
	{
	}

	inline EventManager::EventManager(HANDLE handle) noexcept :
		m_handle{ handle }
	{
		assert(INVALID_HANDLE_VALUE != m_handle);
	}

	inline void EventManager::setHandle(HANDLE handle) noexcept
	{
		assert(INVALID_HANDLE_VALUE != m_handle);
		m_handle = handle;
	}

	inline HANDLE EventManager::getHandle() noexcept
	{
		return m_handle;
	}

} // namespace cg

#endif
