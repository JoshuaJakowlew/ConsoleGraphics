#include <iostream>
#include <string>
#include <chrono>

#include "RenderConsole.h"
#include "bitmap_image.hpp"

int main()
{
	using namespace std;

	srand(time(nullptr));

	vector<rgb_t> palette{
		{0, 0, 0}, {0, 0, 128}, {0, 128, 0}, {0, 128, 128},
		{128, 0, 0}, {128, 0, 128}, {128, 128, 0}, {192, 192, 192},
		{128, 128, 128}, {0, 0, 255}, {0, 255, 0}, {0, 255, 255},
		{255, 0, 0}, {255, 0, 255}, {255, 255, 0}, {255, 255, 255}
	};

	cg::Texture tex(std::move(palette));
	tex.loadFromBitmap("image.bmp");

	cg::Sprite sprite{ tex };
	sprite.setPos({0, 0 });
	sprite.setOrigin(cg::Vec2i(sprite.getSize().x / 2, sprite.getSize().y / 2));
	
	const cg::Vec2u resolution = { tex.getSize().x, tex.getSize().y };
	const cg::Vec2u fontSize{ 2u, 2u };
	cg::RenderConsole console{ resolution, fontSize };

	if (!console.create())
		MessageBoxA(0, "Error: can't create console", "Error", MB_ICONERROR);

	wstring fps;
	wstring screenInfo = L"Resolution: " + to_wstring(resolution.x) + L'x' + to_wstring(resolution.y) +
		L", Font size: " + to_wstring(fontSize.x) + L'x' + to_wstring(fontSize.y);
	wstring minmaxFps;
	int minfpsf = INT_MAX;
	int maxfpsf = INT_MIN;
	size_t counter = 0;
	while (true)
	{
		auto start = chrono::system_clock::now();

		console.fill(CHAR_INFO{ L' ', 0x00 });

		static bool direction = false;
		if (sprite.getPos().x % resolution.x == 0)
			direction = !direction;

		sprite.move(cg::Vec2i( (direction) ? 1 : -1, 0 ));
		console.drawSprite(sprite);

		//Sleep(10);

		console.drawString(0, resolution.y - 4, screenInfo, cg::Color{ '#', cg::Color::DarkBlue, cg::Color::White });
		console.drawString(0, resolution.y - 3, fps, cg::Color{ '#', cg::Color::DarkBlue, cg::Color::White });
		console.drawString(0, resolution.y - 2, minmaxFps, cg::Color{ '#', cg::Color::DarkBlue, cg::Color::White });
		console.drawString(0, resolution.y - 1, L"Frame #" + to_wstring(counter++), cg::Color{ '#', cg::Color::DarkBlue, cg::Color::White });

		if (!console.display())
		{
			MessageBoxA(0, "Error: can't display console", "Error", MB_ICONERROR);
			std::terminate();
		}

		auto end = chrono::system_clock::now();
		auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start).count();
		
		int fpsf = 1'000'000 / elapsed;

		if (fpsf < minfpsf) minfpsf = fpsf;
		if (fpsf > maxfpsf) maxfpsf = fpsf;

		fps = L"Fps: " + to_wstring(fpsf) +
			L", Frame time: " + to_wstring(elapsed / 1000.f) + L"ms";
		
		minmaxFps = L"Max fps: " + to_wstring(maxfpsf) + L", min fps: " + to_wstring(minfpsf);
	}
}