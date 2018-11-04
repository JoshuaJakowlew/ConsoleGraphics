#include <iostream>

#include "Console.h"

int main()
{
	/*using namespace std;
	cg::Console console{ {80u, 25u}, {16u, 16u} };
	console.create();

	auto color = cg::Color{ L' ', cg::Color::Blue, cg::Color::Red };
	console.fill(color);

	console.drawLine({ 2, 2 }, { 5, 5 }, cg::Color{ L'#', cg::Color::Black, cg::Color::Red });
	console.drawRect(10, 10, 10, 10, cg::Color{ L'#', cg::Color::Cyan, cg::Color::White });
	console.drawString(15, 15, L"Sample text", cg::Color{ L'#', cg::Color::Black, cg::Color::Red });
	console.drawStringAlpha(15, 17, L"Transparent text", cg::Color{ L'#', cg::Color::Black, cg::Color::Red });

	console.display();
	Sleep(10000);*/
	using namespace std;

	cg::Vec2u m_resolution = { 80u, 25u };
	short m_nScreenWidth = 80;
	short m_nScreenHeight = 25;

	SMALL_RECT m_rectWindow = { 0, 0, 1, 1 };
	HANDLE m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow);

	// Set the size of the screen buffer
	COORD coord = { (short)m_nScreenWidth, (short)m_nScreenHeight };
	if (!SetConsoleScreenBufferSize(m_hConsole, coord));

	// Assign screen buffer to the console
	if (!SetConsoleActiveScreenBuffer(m_hConsole));

	// Set the font size now that the screen buffer has been assigned to the console
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 8;
	cfi.dwFontSize.Y = 8;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	//wcscpy_s(cfi.FaceName, L"Lucida Console");
	//wcscpy_s(cfi.FaceName, L"Liberation Mono");
	wcscpy_s(cfi.FaceName, L"Consolas");
	//wcscpy_s(cfi.FaceName, L"Raster");
	if (!SetCurrentConsoleFontEx(m_hConsole, false, &cfi));

	// Get screen buffer info and check the maximum allowed window size. Return
	// error if exceeded, so user knows their dimensions/fontsize are too large
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(m_hConsole, &csbi));
	if (m_nScreenHeight > csbi.dwMaximumWindowSize.Y);
	if (m_nScreenWidth > csbi.dwMaximumWindowSize.X);

	// Set Physical Console Window Size
	m_rectWindow = { 0, 0, (short)m_nScreenWidth - 1, (short)m_nScreenHeight - 1 };
	if (!SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow));

	Sleep(10000);
}