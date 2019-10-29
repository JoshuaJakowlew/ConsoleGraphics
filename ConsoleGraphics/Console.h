#ifndef CONSOLE_H
#define CONSOLE_H

#include <array>
#include <vector>
#include <queue>
#include <type_traits>
#include <cassert>

#include "Vec2.h"
#include "Color.h"

namespace cg
{
	class Console
	{
	public:
		explicit Console(Vec2u resolution, Vec2u font_size) noexcept;

		Console(const Console&) = delete;
		Console& operator=(const Console&) = delete;

		[[nodiscard]] bool display(const CHAR_INFO* buffer) noexcept;
		[[nodiscard]] bool close() noexcept;

		[[nodiscard]] bool isOpen() const noexcept;

		[[nodiscard]] inline const HANDLE getInputHandle() const noexcept;
		[[nodiscard]] inline const HANDLE getOutputHandle() const noexcept;
		[[nodiscard]] inline Vec2u getResolution() const noexcept;
		[[nodiscard]] inline Vec2u getMaxResolution() const noexcept;
		
		[[nodiscard]] bool setPalette(const cg::Palette& palette) noexcept;

	protected:
		struct Handles
		{
			HANDLE original;
			HANDLE out;
			HANDLE in;
		};

		Handles m_handles;
		bool m_isOpen = false;

		Vec2u m_resolution;
		Vec2u m_maxResolution;
		Vec2u m_fontSize;

		Palette m_palette;

		[[nodiscard]] bool create() noexcept;
		[[nodiscard]] Vec2u getMaxScreenBufferSize() noexcept;
		[[nodiscard]] bool getStdHandles() noexcept;
		[[nodiscard]] bool createScreenBuffer() noexcept;
		[[nodiscard]] bool configureInput() noexcept;
		[[nodiscard]] bool configureOutput() noexcept;
		[[nodiscard]] bool assignScreenBuffer() noexcept;
		[[nodiscard]] bool disableCursor() noexcept;
		[[nodiscard]] bool setFontSize(Vec2u fontSize) noexcept;
	};

} // namespace cg

namespace cg
{
	Console::Console(Vec2u resolution, Vec2u font_size) noexcept :
		m_resolution{ std::move(resolution) },
		m_fontSize{ std::move(font_size) }
	{
		m_isOpen = create();
	}

	bool Console::display(const CHAR_INFO* buffer) noexcept
	{
		assert(buffer);
		assert(m_resolution.x <= SHRT_MAX - 1 && m_resolution.y <= SHRT_MAX - 1);
		assert(m_handles.out != INVALID_HANDLE_VALUE);

		if (!buffer)
			return false;

		const COORD screenSize = {
			static_cast<SHORT>(m_resolution.x),
			static_cast<SHORT>(m_resolution.y)
		};

		const SMALL_RECT writeRegion = {
			0,
			0,
			static_cast<SHORT>(m_resolution.x - 1),
			static_cast<SHORT>(m_resolution.y - 1)
		};

		if (!m_isOpen)
			return false;

		auto writtenRegion = writeRegion;

		const auto writeResult = ::WriteConsoleOutputW(m_handles.out, buffer, screenSize, { 0, 0 }, &writtenRegion);

		return
			(writtenRegion.Left == writeRegion.Left) &
			(writtenRegion.Right == writeRegion.Right) &
			(writtenRegion.Bottom == writeRegion.Bottom) &
			(writtenRegion.Top == writeRegion.Top) &
			writeResult;
	}

	bool Console::close() noexcept
	{
		if(!::SetConsoleActiveScreenBuffer(m_handles.original))
			return false;
		
		if(!::CloseHandle(m_handles.out))
			return false;

		m_isOpen = false;

		return true;
	}

	inline bool Console::isOpen() const noexcept
	{
		return m_isOpen;
	}

	inline const HANDLE Console::getInputHandle() const noexcept
	{
		return m_handles.in;
	}

	inline const HANDLE Console::getOutputHandle() const noexcept
	{
		return m_handles.out;
	}

	inline Vec2u Console::getResolution() const noexcept
	{
		return m_resolution;
	}

	inline Vec2u Console::getMaxResolution() const noexcept
	{
		return m_maxResolution;
	}

	bool Console::setPalette(const cg::Palette& palette) noexcept
	{
		CONSOLE_SCREEN_BUFFER_INFOEX csbi;
		csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

		[[unlikely]]
		if (!::GetConsoleScreenBufferInfoEx(m_handles.out, &csbi))
			return false;

		std::copy(std::begin(palette), std::end(palette), std::begin(csbi.ColorTable));

		// Fix WinApi bug
		// When SetConsoleSccreenBufferInfoEx is invoked, window bouns are decremented
		// and awful scrollbars appear (usually both, sometimes only horizontal)
		// To fix this problem, increment windows boundary before function call

		// TODO: Create separateSetConsoleScreenBufferInfoEx wrapper with bugfix?
		++csbi.srWindow.Bottom;
		++csbi.srWindow.Right;
		
		return ::SetConsoleScreenBufferInfoEx(m_handles.out, &csbi);
	}

	bool Console::create() noexcept
	{
		[[unlikely]]
		if (!getStdHandles())
			return false;

		[[unlikely]]
		if (!configureInput())
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

		[[unlikely]]
		if (!setPalette(cg::palette::defaultPalette))
			return false;

		m_isOpen = true;

		return true; // Everything is OK
	}

	Vec2u Console::getMaxScreenBufferSize() noexcept
	{
		assert(m_handles.out != INVALID_HANDLE_VALUE);

		CONSOLE_SCREEN_BUFFER_INFO csbi;

		[[unlikely]]
		if (!::GetConsoleScreenBufferInfo(m_handles.out, &csbi))
			return { 0, 0 };

		return {
			static_cast<unsigned>(csbi.dwMaximumWindowSize.X),
			static_cast<unsigned>(csbi.dwMaximumWindowSize.Y)
		};
	}

	bool Console::getStdHandles() noexcept
	{
		const auto original_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
		const auto input_handle = ::GetStdHandle(STD_INPUT_HANDLE);

		[[unlikely]]
		if (INVALID_HANDLE_VALUE == original_handle ||
			INVALID_HANDLE_VALUE == input_handle)
			return false;

		m_handles.original = original_handle;
		m_handles.in = input_handle;
		return true;
	}

	bool Console::createScreenBuffer() noexcept
	{
		const auto buffer_handle = ::CreateConsoleScreenBuffer(
			/* dwDesiredAccess      */ GENERIC_READ | GENERIC_WRITE,
			/* dwShareMode          */ FILE_SHARE_READ | FILE_SHARE_WRITE,
			/* lpSecurityAttributes */ NULL,
			/* dwFlags              */ CONSOLE_TEXTMODE_BUFFER,
			/* lpScreenBufferData   */ NULL
		);

		[[unlikely]]
		if (INVALID_HANDLE_VALUE == buffer_handle)
			return false;

		m_handles.out = buffer_handle;
		return true;
	}

	bool Console::configureInput() noexcept
	{
		DWORD dwMode = 0;
		if (!::GetConsoleMode(m_handles.in, &dwMode))
			return false;

		dwMode &= ~ENABLE_QUICK_EDIT_MODE;
		dwMode |= ENABLE_MOUSE_INPUT;

		if (!::SetConsoleMode(m_handles.in, dwMode))
			return false;

		return true; // everything is OK
	}

	bool Console::configureOutput() noexcept
	{
		/*
		// Enable ANSI escape codes
		DWORD dwMode = 0;
		if (!::GetConsoleMode(m_handles.out, &dwMode))
			return false;
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!::SetConsoleMode(m_handles.out, dwMode))
			return false;
		*/

		// Enable unicode
		[[unlikely]]
		if (!::SetConsoleOutputCP(CP_UTF8))
			return false;

		return true; // everything is OK
	}

	bool Console::assignScreenBuffer() noexcept
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
		const SMALL_RECT windowSize = { 0, 0, 1, 1 };

		[[unlikely]]
		if (!::SetConsoleWindowInfo(m_handles.out, TRUE, &windowSize))
			return false;

		// Extend screen buffer to given size
		const COORD bufferSize = {
			static_cast<SHORT>(m_resolution.x),
			static_cast<SHORT>(m_resolution.y)
		};

		[[unlikely]]
		if (!::SetConsoleScreenBufferSize(m_handles.out, bufferSize))
			return false;

		// Assign screen buffer to the console
		assert(m_handles.out != INVALID_HANDLE_VALUE);

		[[unlikely]]
		if (!::SetConsoleActiveScreenBuffer(m_handles.out))
			return false;

		// Set the font size when the screen buffer has been already assigned to the console
		[[unlikely]]
		if (!setFontSize(m_fontSize))
			return false;

		// Get maximum screen size in columns and rows
		// And check if we are out of the bounds
		const auto maxResolution = getMaxScreenBufferSize();

		[[unlikely]]
		if (m_resolution.x > maxResolution.x ||
			m_resolution.y > maxResolution.y)
			return false;

		m_maxResolution = maxResolution;

		// If evrything is still OK, try to expand window to given size
		// Stretch console window back to actual size
		assert(m_resolution.x <= SHRT_MAX + 1 && m_resolution.y <= SHRT_MAX + 1);

		const SMALL_RECT newWindowSize = {
			0,
			0,
			static_cast<SHORT>(m_resolution.x - 1),
			static_cast<SHORT>(m_resolution.y - 1)
		};

		assert(m_handles.out != INVALID_HANDLE_VALUE);

		[[unlikely]]
		if (!::SetConsoleWindowInfo(m_handles.out, TRUE, &newWindowSize))
			return false;

		return true; // everything is OK
	}

	bool Console::disableCursor() noexcept
	{
		assert(m_handles.out != INVALID_HANDLE_VALUE);

		const CONSOLE_CURSOR_INFO cursor_info{
			/* dwSize   */ 42,
			/* bVisible */ FALSE
		};

		return ::SetConsoleCursorInfo(m_handles.out, &cursor_info);
	}

	bool Console::setFontSize(Vec2u fontSize) noexcept
	{
		assert(fontSize.x <= SHRT_MAX && fontSize.y <= SHRT_MAX);
		assert(m_handles.out != INVALID_HANDLE_VALUE);

		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = static_cast<SHORT>(fontSize.x);
		cfi.dwFontSize.Y = static_cast<SHORT>(fontSize.y);
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Consolas");

		return ::SetCurrentConsoleFontEx(m_handles.out, FALSE, &cfi);
	}

} // namespace cg

#endif