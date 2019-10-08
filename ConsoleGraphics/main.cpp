#include <iostream>
#include <string>
#include <chrono>

#include "RenderConsole.h"
#include "Color.h"
#include "bitmap_image.hpp"

int main()
{
	using namespace std;

	cg::Palette palette{
			RGB(53, 14, 88), RGB(0, 0, 128), RGB(0, 128, 0), RGB(0, 128, 128),
			RGB(128, 0, 0), RGB(128, 0, 128), RGB(128, 128, 0), RGB(192, 192, 192),
			RGB(128, 128, 128), RGB(0, 0, 255), RGB(0, 255, 0), RGB(0, 255, 255),
			RGB(255, 0, 0), RGB(255, 0, 255), RGB(255, 255, 0), RGB(255, 255, 255)
	};

	cg::Texture tex{ "image-half.bmp", palette };

	cg::Sprite sprite{ tex };
	sprite.setPos({ 25, 25 });

	cg::RenderConsole console{ { 200, 100 }, { 4u, 4u } };

	if (!console.create())
		MessageBoxA(0, "Error: can't create console", "Error", MB_ICONERROR);

	console.setPalette(palette);

	int frameCounter = 0;
	auto start = chrono::system_clock::now();

	while (true)
	{
		console.pollEvent();

		console.fill(CHAR_INFO{ L' ', 0x00 });

		static bool direction = false;
		if (sprite.getPos().x % console.getResolution().x == 0)
			direction = !direction;

		sprite.move(cg::Vec2i(direction ? 1 : -1, 0));
		console.drawSprite(sprite);

		if (!console.display())
		{
			MessageBoxA(0, "Error: can't display console", "Error", MB_ICONERROR);
			break;
		}

		auto end = chrono::system_clock::now();
		auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
		++frameCounter;
		//if (elapsed >= 10s)
		//	break;
	}

	return frameCounter;
}

//	HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
//
//	DWORD dwMode = 0;
//	if (!::GetConsoleMode(in, &dwMode))
//		return false;
//	dwMode &= ~ENABLE_QUICK_EDIT_MODE;
//	dwMode |= ENABLE_PROCESSED_INPUT;
//	dwMode |= ENABLE_MOUSE_INPUT;
//	dwMode |= ENABLE_WINDOW_INPUT;
//
//	if (!::SetConsoleMode(in, dwMode))
//		return false;
//
//	while (true)
//	{
//		DWORD nEvents = 0;
//		bool success = GetNumberOfConsoleInputEvents(in, &nEvents);
//
//		std::vector<INPUT_RECORD> events(nEvents);
//
//		DWORD read = 0;
//		success = ReadConsoleInput(in, events.data(), nEvents, &read);
//
//		for (auto e : events)
//		{
//			if (e.EventType == MOUSE_EVENT)
//			{
//				continue;
//			}
//			if (e.EventType == KEY_EVENT)
//			{
//				continue;
//			}
//		}
//	}
//}
