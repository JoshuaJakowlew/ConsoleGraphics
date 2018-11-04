#include "Console.h"

namespace cg
{
	Console::Console(Vec2u resolution, Vec2u font_size) :
		RenderSurface(resolution),
		m_resolution(resolution),
		m_fontSize(font_size)
	{}

	Console::~Console()
	{
		::SetConsoleActiveScreenBuffer(m_handles.original);
		::CloseHandle(m_handles.out);
	}
	bool Console::create()
	{
		if (!getStdHandles())
			return false;

		if (!createScreenBuffer())
			return false;

		if (!assignScreenBuffer())
			return false;

		if (!configureOutput())
			return false;

		return true; // Everything is OK
	}

	void Console::display()
	{
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

		::WriteConsoleOutput(m_handles.out, getBuffer(), screenSize, { 0, 0 }, &writeRegion);
	}

	Vec2u Console::getMaxScreenBufferSize()
	{

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!::GetConsoleScreenBufferInfo(m_handles.out, &csbi))
			return { 0, 0 };

		return Vec2u(csbi.dwMaximumWindowSize.X, csbi.dwMaximumWindowSize.Y);
	}

	bool Console::getStdHandles()
	{
		auto original_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
		auto input_handle = ::GetStdHandle(STD_INPUT_HANDLE);

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
		if (!::SetConsoleOutputCP(CP_UTF8))
			return false;
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
		if (!::SetConsoleWindowInfo(m_handles.out, TRUE, &windowSize))
			return false;

		// Extend screen buffer to given size
		COORD bufferSize = { (SHORT)m_resolution.x, (SHORT)m_resolution.y };
		if (!::SetConsoleScreenBufferSize(m_handles.out, bufferSize))
			return false;

		// Assign screen buffer to the console
		if (!::SetConsoleActiveScreenBuffer(m_handles.out))
			return false;

		// Set the font size now that the screen buffer has been assigned to the console
		if (!setFontSize(m_fontSize))
			return false;

		// Get maximum screen size in columns and rows
		// And check if we are out of the bounds
		m_maxResolution = getMaxScreenBufferSize();
		if (m_resolution.x > m_maxResolution.x ||
			m_resolution.y > m_maxResolution.y)
			return false;

		// If evrything is still OK the try to expand window to given size
		// Stretch console window back to actual size
		windowSize = { 0, 0, (SHORT)m_resolution.x - 1, (SHORT)m_resolution.y - 1 };
		if (!::SetConsoleWindowInfo(m_handles.out, TRUE, &windowSize))
			return false;
	}

	bool Console::setFontSize(Vec2u fontSize)
	{
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 8;//static_cast<SHORT>(fontSize.x);
		cfi.dwFontSize.Y = 8;//static_cast<SHORT>(fontSize.y);
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Consolas");

		if (!::SetCurrentConsoleFontEx(m_handles.out, FALSE, &cfi))
			return false;

		return true;
	}



} // namespace cg