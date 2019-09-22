#include <iostream>
#include <string>
#include <chrono>

#include "RenderConsole.h"
#include "Color.h"
#include "bitmap_image.hpp"

int main()
{
	using namespace std;

	vector<rgb_t> palette{
		{0, 0, 0}, {0, 0, 128}, {0, 128, 0}, {0, 128, 128},
		{128, 0, 0}, {128, 0, 128}, {128, 128, 0}, {192, 192, 192},
		{128, 128, 128}, {0, 0, 255}, {0, 255, 0}, {0, 255, 255},
		{255, 0, 0}, {255, 0, 255}, {255, 255, 0}, {255, 255, 255}
	};

	cg::RenderConsole::Palette defaultPalette{
			RGB(53, 14, 88), RGB(0, 0, 128), RGB(0, 128, 0), RGB(0, 128, 128),
			RGB(128, 0, 0), RGB(128, 0, 128), RGB(128, 128, 0), RGB(192, 192, 192),
			RGB(128, 128, 128), RGB(0, 0, 255), RGB(0, 255, 0), RGB(0, 255, 255),
			RGB(255, 0, 0), RGB(255, 0, 255), RGB(255, 255, 0), RGB(255, 255, 255)
	};

	cg::Texture tex(std::move(palette));
	tex.loadFromBitmap("image-half.bmp");

	cg::Sprite sprite{ tex };
	sprite.setPos({25, 25 });
	sprite.setOrigin(cg::Vec2i(sprite.getSize().x / 2, sprite.getSize().y / 2));
	
	constexpr cg::Vec2u resolution = { 200, 100 };
	const cg::Vec2u fontSize{ 4u, 4u };
	cg::RenderConsole console{ resolution, fontSize };

	if (!console.create())
		MessageBoxA(0, "Error: can't create console", "Error", MB_ICONERROR);

	console.setPalette(defaultPalette);

	int counter = 0;
	auto start = chrono::system_clock::now();

	while (true)
	{
		console.fill(CHAR_INFO{ L' ', 0x00 });

		static bool direction = false;
		if (sprite.getPos().x % resolution.x == 0)
			direction = !direction;

		sprite.move(cg::Vec2i( (direction) ? 1 : -1, 0 ));
		console.drawSprite(sprite);
		//console.drawStringAlpha(395, 10, L"Hello world!", cg::makeCharInfo(L' ', cg::Color::Black, cg::Color::Black));

		if (!console.display())
		{
			MessageBoxA(0, "Error: can't display console", "Error", MB_ICONERROR);
			std::terminate();
		}

		auto end = chrono::system_clock::now();
		auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
		++counter;
		if (elapsed >= 10 * 1000) break;
	}

	return counter;
}