#include <iostream>
#include <string>
#include <chrono>

#include "RenderConsole.h"

int main()
{
	using namespace std;
	const cg::Vec2u resolution{ 50u, 30u };
	cg::RenderConsole console{ resolution, {16u, 16u} };
	if (!console.create())
		cerr << "Error: can't create console" << '\n';

	wstring fps;
	while (true)
	{
		auto start = chrono::system_clock::now();

		console.drawLine(-1, -1, 99, 99, cg::Color{ L'#', (uint16_t)0xCF });
		console.drawRect(-5, -5, 20, 30, cg::Color{ L'#', (uint16_t)0xAF });
		console.fillRect(49, 29, -5, -5, cg::Color{ L'#', (uint16_t)0xC2 });
		console.drawString(-2, 0, L"Hello, Microsoft", cg::Color{ L'#', (uint16_t)0xFA });
		console.drawStringAlpha(-2, 15, L"Русский текст", cg::Color{ L'#', (uint16_t)0xFA });
		console.drawStringAlpha(37, 6, L"Ὼῷ₥ỜᵺԽ", CHAR_INFO{ L'#', short(0x1234) });

		console.drawString(0, resolution.y - 1, fps, cg::Color{ '#', cg::Color::DarkBlue, cg::Color::White });

		if (!console.display())
			cerr << "Error: can't display console" << '\n';

		auto end = chrono::system_clock::now();
		auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start).count();
		fps = L"Fps: " + to_wstring(1'000'000 / elapsed) + L", Frame time: " + to_wstring(elapsed / 1000.f) + L"ms";
	}
}