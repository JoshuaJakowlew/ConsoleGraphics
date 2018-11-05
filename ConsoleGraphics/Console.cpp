#include "Console.h"

namespace cg
{
	Console::Console(Vec2u resolution, Vec2u font_size) :
		RenderSurface(resolution),
		m_resolution(resolution),
		m_fontSize(font_size)
	{}

	Console::~Console() noexcept
	{
		::SetConsoleActiveScreenBuffer(m_handles.original);
		::CloseHandle(m_handles.out);
	}
	
	bool Console::create() noexcept
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

	bool Console::display() noexcept
	{
		assert(m_resolution.x <= SHRT_MAX - 1 && m_resolution.y <= SHRT_MAX - 1);

		static COORD screenSize = {
			static_cast<SHORT>(m_resolution.x),
			static_cast<SHORT>(m_resolution.y)
		};

		static SMALL_RECT writeRegion = {
			0,
			0,
			static_cast<SHORT>(m_resolution.x - 1),
			static_cast<SHORT>(m_resolution.y - 1)
		};

		auto buffer = getBuffer();
		assert(buffer);
		assert(m_handles.out != INVALID_HANDLE_VALUE);

		return ::WriteConsoleOutputW(m_handles.out, buffer, screenSize, { 0, 0 }, &writeRegion);
	}

	Vec2u Console::getMaxScreenBufferSize()
	{
		assert(m_handles.out != INVALID_HANDLE_VALUE);

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		[[unlikely]]
		if (!::GetConsoleScreenBufferInfo(m_handles.out, &csbi))
			return { 0, 0 };

		return Vec2u(csbi.dwMaximumWindowSize.X, csbi.dwMaximumWindowSize.Y);
	}

	bool Console::getStdHandles()
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

	bool Console::createScreenBuffer()
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

	bool Console::configureOutput()
	{
		// Enable ANSI escape codes
		/*DWORD dwMode = 0;
		if (!::GetConsoleMode(m_handles.out, &dwMode))
			return false;

		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!::SetConsoleMode(m_handles.out, dwMode))
			return false;*/

		// Enable UTC-16 (not UTF-8, thanks, MS)
		[[unlikely]]
		if (!::SetConsoleOutputCP(CP_UTF8))
			return false;

		return true; // everything is OK
	}

	bool Console::assignScreenBuffer()
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

	bool Console::disableCursor()
	{
		CONSOLE_CURSOR_INFO cursor_info;
		cursor_info.dwSize = 42;
		cursor_info.bVisible = FALSE;

		assert(m_handles.out != INVALID_HANDLE_VALUE);
		return ::SetConsoleCursorInfo(m_handles.out, &cursor_info);
	}

	bool Console::setFontSize(Vec2u fontSize)
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