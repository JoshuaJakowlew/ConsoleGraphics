#include <iostream>
#include <string>
#include <chrono>

#include "RenderConsole.h"

int main()
{
	using namespace std;

	cg::Palette palette{
			RGB(53, 14, 88), RGB(0, 0, 128), RGB(0, 128, 0), RGB(0, 128, 128),
			RGB(128, 0, 0), RGB(128, 0, 128), RGB(128, 128, 0), RGB(192, 192, 192),
			RGB(128, 128, 128), RGB(0, 0, 255), RGB(0, 255, 0), RGB(0, 255, 255),
			RGB(255, 0, 0), RGB(255, 0, 255), RGB(255, 255, 0), RGB(255, 255, 255)
	};

	cg::Texture tex{ "sprite.bmp", palette };

	cg::Sprite sprite{ tex };
	sprite.setPos({ 25, 25 });
	sprite.setTransparent(true);

	cg::Sprite sprite1{ tex };
	sprite1.setPos({ 25, 25 });
	sprite1.setTransparent(true, *(sprite1.getData() + 5));

	cg::RenderConsole console{ { 200, 100 }, { 4, 4 } };
	if (!console.isOpen())
		MessageBoxA(0, "Error: can't create console", "Error", MB_ICONERROR);

	console.setPalette(palette);

	while (console.isOpen())
	{
		cg::Event e = {};
		while (console.pollEvent(e))
		{
			if (e.type == cg::EventType::KeyPressed || e.type == cg::EventType::MouseClick)
				console.close();
			if (e.type == cg::EventType::MouseMove)
				sprite.setPos({ e.mouseMove.position.X, e.mouseMove.position.Y });
		}

		console.fill(CHAR_INFO{ L' ', 0x00 });

		console.drawSprite(sprite1);
		console.drawSprite(sprite);

		if (!console.display())
			break;
	}
}