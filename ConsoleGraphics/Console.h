#ifndef CONSOLE_H
#define CONSOLE_H

#include <type_traits>
#include <cassert>

#include <Windows.h>

#include "Vec2.h"

namespace cg
{
	template <typename T>
	class Console
	{
	public:
		explicit Console(Vec2u resolution, Vec2u font_size);
		Console(const Console&) = delete;
		Console& operator=(const Console&) = delete;
		~Console() noexcept;

		[[nodiscard]] bool create() noexcept;
		[[nodiscard]] bool display() noexcept;

		[[nodiscard]] inline Vec2u getResolution() const noexcept;
		[[nodiscard]] inline Vec2u getMaxResolution() const noexcept;
	protected:
		struct Handles
		{
			HANDLE original;
			HANDLE out;
			HANDLE in;
		};

		Handles m_handles;

		Vec2u m_resolution;
		Vec2u m_maxResolution;
		Vec2u m_fontSize;

		[[nodiscard]] Vec2u getMaxScreenBufferSize();
		[[nodiscard]] bool getStdHandles();
		[[nodiscard]] bool createScreenBuffer();
		[[nodiscard]] bool configureOutput();
		[[nodiscard]] bool assignScreenBuffer();
		[[nodiscard]] bool disableCursor();
		[[nodiscard]] bool setFontSize(Vec2u fontSize);
	};

} // namespace cg

namespace cg
{
	template <typename T>
	inline Vec2u Console<T>::getResolution() const noexcept
	{
		return m_resolution;
	}

	template <typename T>
	inline Vec2u Console<T>::getMaxResolution() const noexcept
	{
		return m_maxResolution;
	}

	template <typename T>
	Console<T>::Console(Vec2u resolution, Vec2u font_size) :
		m_resolution(resolution),
		m_fontSize(font_size)
	{}

	template <typename T>
	Console<T>::~Console() noexcept
	{
		::SetConsoleActiveScreenBuffer(m_handles.original);
		::CloseHandle(m_handles.out);
	}

	template <typename T>
	bool Console<T>::create() noexcept
	{
		[[unlikely]]
		if (!getStdHandles())
			return false;

		[[unlikely]]
		if (!createScreenBuffer())
			return false;

		[[unlikely]]
		if (!configureOutput())
			return false;

		[[unlikely]]
		if (!assignScreenBuffer())
			return false;

		[[unlikely]]
		if (!disableCursor())
			return false;

		return true; // Everithing is OK
	}

	template <typename T>
	bool Console<T>::display() noexcept
	{
		static_assert(std::is_same_v<T*, decltype(this)>);
		static_cast<T*>(this)->display();
	}

	template <typename T>
	Vec2u Console<T>::getMaxScreenBufferSize()
	{
		assert(m_handles.out != INVALID_HANDLE_VALUE);

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		[[unlikely]]
		if (!::GetConsoleScreenBufferInfo(m_handles.out, &csbi))
			return { 0, 0 };

		return Vec2u(csbi.dwMaximumWindowSize.X, csbi.dwMaximumWindowSize.Y);
	}

	template <typename T>
	bool Console<T>::getStdHandles()
	{
		auto original_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
		auto input_handle = ::GetStdHandle(STD_INPUT_HANDLE);

		[[unlikely]]
		if (original_handle == INVALID_HANDLE_VALUE ||
			input_handle == INVALID_HANDLE_VALUE)
			return false;

		m_handles.original = original_handle;
		m_handles.in = input_handle;
		return true;
	}

	template <typename T>
	bool Console<T>::createScreenBuffer()
	{
		auto buffer_handle = ::CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CONSOLE_TEXTMODE_BUFFER,
			NULL
		);

		[[unlikely]]
		if (INVALID_HANDLE_VALUE == buffer_handle)
			return false;

		m_handles.out = buffer_handle;
		return true;
	}

	template <typename T>
	bool Console<T>::configureOutput()
	{
		// Enable ANSI escape codes
		/*DWORD dwMode = 0;
		if (!::GetConsoleMode(m_handles.out, &dwMode))
			return false;

		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!::SetConsoleMode(m_handles.out, dwMode))
			return false;*/

		// Enable unicode
		[[unlikely]]
		if (!::SetConsoleOutputCP(CP_UTF8))
			return false;

		return true; // everything is OK
	}

	template <typename T>
	bool Console<T>::assignScreenBuffer()
	{
		/*
		*
		* Desine sperare qui hic intras
		*
		* Thanks to Microsoft, there are no good ways to do the following
		* This code is written according to painful experience, so just folow these rules:
		* 1) Create screen buffer (Console::createScreenBuffer)
		* 2) Set console window size to the minimal one;
		* 3) Expand screen buffer to given size
		* 4) Assign screen buffer to the console
		* 5) Set font settings
		* 6) Get maximum allowed screen size and check if our one in bounds
		* 7) If evrything is still OK the try to expand window to given size
		*/

		// Screen buffer can't be less then actual console window size
		// So, we set minimal window size and then extend screen buffer as far as we wish
		SMALL_RECT windowSize = { 0, 0, 1, 1 };
		[[unlikely]]
		if (!::SetConsoleWindowInfo(m_handles.out, TRUE, &windowSize))
			return false;

		// Extend screen buffer to given size
		COORD bufferSize = { (SHORT)m_resolution.x, (SHORT)m_resolution.y };
		[[unlikely]]
		if (!::SetConsoleScreenBufferSize(m_handles.out, bufferSize))
			return false;

		// Assign screen buffer to the console
		assert(m_handles.out != INVALID_HANDLE_VALUE);
		[[unlikely]]
		if (!::SetConsoleActiveScreenBuffer(m_handles.out))
			return false;

		// Set the font size now that the screen buffer has been assigned to the console
		[[unlikely]]
		if (!setFontSize(m_fontSize))
			return false;

		// Get maximum screen size in columns and rows
		// And check if we are out of the bounds
		m_maxResolution = getMaxScreenBufferSize();

		[[unlikely]]
		if (m_resolution.x > m_maxResolution.x ||
			m_resolution.y > m_maxResolution.y)
			return false;

		// If evrything is still OK the try to expand window to given size
		// Stretch console window back to actual size
		assert(m_resolution.x <= SHRT_MAX + 1 && m_resolution.y <= SHRT_MAX + 1);
		windowSize = {
			0,
			0,
			static_cast<SHORT>(m_resolution.x - 1),
			static_cast<SHORT>(m_resolution.y - 1)
		};

		assert(m_handles.out != INVALID_HANDLE_VALUE);
		[[unlikely]]
		if (!::SetConsoleWindowInfo(m_handles.out, TRUE, &windowSize))
			return false;

		return true; // everything is OK
	}

	template <typename T>
	bool Console<T>::disableCursor()
	{
		CONSOLE_CURSOR_INFO cursor_info;
		cursor_info.dwSize = 42;
		cursor_info.bVisible = FALSE;

		assert(m_handles.out != INVALID_HANDLE_VALUE);
		return ::SetConsoleCursorInfo(m_handles.out, &cursor_info);
	}

	template <typename T>
	bool Console<T>::setFontSize(Vec2u fontSize)
	{
		assert(fontSize.x <= SHRT_MAX && fontSize.y <= SHRT_MAX);

		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = static_cast<SHORT>(fontSize.x);
		cfi.dwFontSize.Y = static_cast<SHORT>(fontSize.y);
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Consolas");

		assert(m_handles.out != INVALID_HANDLE_VALUE);
		return ::SetCurrentConsoleFontEx(m_handles.out, FALSE, &cfi);
	}

} // namespace cg

#endif