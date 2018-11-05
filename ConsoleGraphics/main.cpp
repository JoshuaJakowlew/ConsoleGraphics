#include <iostream>

#include "Console.h"

int main()
{
	using namespace std;
	cg::Console console{ {45u, 25u}, {16u, 16u} };
	bool success = console.create();

	auto color = cg::Color{ L' ', cg::Color::Blue, cg::Color::Red };
	console.fill(color);

	console.drawLine({ 2, 2 }, { 15, 15 }, cg::Color{ L'#', cg::Color::Black, cg::Color::Red });
	console.drawRect(10, 10, 10, 10, cg::Color{ L'#', cg::Color::Cyan, cg::Color::White });
	console.drawString(12, 12, L"Fuck Microsoft", cg::Color{ L'#', cg::Color::Black, cg::Color::Red });
	console.drawStringAlpha(15, 17, L"Я, б*ять, вас ненавижу", cg::Color{ L'#', cg::Color::Black, cg::Color::Red });

	console.display();
	Sleep(100000);
}