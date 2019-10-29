#include <iostream>
#include <string>
#include <chrono>

#include "cg.h"
#include "Vec2.h"

int main()
{
	using namespace std;

	cg::Palette palette{
			RGB(53, 14, 88), RGB(0, 0, 128), RGB(0, 128, 0), RGB(0, 128, 128),
			RGB(128, 0, 0), RGB(128, 0, 128), RGB(128, 128, 0), RGB(192, 192, 192),
			RGB(128, 128, 128), RGB(0, 0, 255), RGB(0, 255, 0), RGB(0, 255, 255),
			RGB(255, 0, 0), RGB(255, 0, 255), RGB(255, 255, 0), RGB(255, 255, 255)
	};

	cg::Texture bgTex{ "assets/bg.bmp", palette };
	cg::Texture marioTex{ "assets/sprite.bmp", palette };

	cg::Sprite bg{ bgTex };
	bg.setOrigin({ 0, 0 });

	cg::Sprite sprite{ marioTex };
	sprite.setPos({ 25, 25 });
	sprite.setTransparent(true);

	cg::Sprite sprite1{ marioTex, { 0, 0 }, { 15, 7 } };
	sprite1.setPos({ 25, 25 });
	sprite1.setTransparent(true);
	auto speed = 25.f;

	cg::Console console{ { 200, 100 }, { 4, 4 } };
	if (!console.isOpen())
		MessageBoxA(0, "Error: can't create console", "Error", MB_ICONERROR);
	console.setPalette(palette);

	cg::RenderSurface render{ 200, 100 };

	cg::Clock clock;

	while (console.isOpen())
	{
		cg::Event e = {};
		while (console.pollEvent(e))
		{
			if (e.type == cg::EventType::KeyPressed || e.type == cg::EventType::MouseClick)
				speed = -speed;
			if (e.type == cg::EventType::MouseMove)
				sprite.setPos({ e.mouseMove.position.X, e.mouseMove.position.Y });
		}

		auto elapsed = clock.restart();
		sprite1.move(cg::Vec2f{ speed * elapsed, 0.f });

		render.fill(CHAR_INFO{ L' ', 0x00 });
		render.drawSprite(bg);
		render.drawSprite(sprite1);
		render.drawSprite(sprite);

		if (!console.display(render.getBuffer()))
			break;
	}
}