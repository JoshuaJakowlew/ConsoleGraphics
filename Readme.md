# Console graphics

Console Graphics is a simple C++ library that allows you to use Windows console in a simple way.

### Features
  - Use console screen as a surface - simply output characters at needed coordinates
  - Use colors!
  - Сhange console resoulution and even font size
  - Draw sprites, lines, strings and other stuff in a handy way

### Tutorial
  First of all you need to create and configure the console surface - its resolution and font size.
  
  To start drawing a sprite, we must load a texture. Texture is our data and sprite is a rectangle with corrdinates that points to texture.
  Let's see how to display a sprite

  You can also:
  - Draw lines
  - Draw rectangles
  - Draw strings
  - Draw single pixels

  ```C++
#include "RenderConsole.h"

int main()
{
	using namespace std;

	// Create custom palette
    cg::Palette palette{
			RGB(53, 14, 88), RGB(0, 0, 128), RGB(0, 128, 0), RGB(0, 128, 128),
			RGB(128, 0, 0), RGB(128, 0, 128), RGB(128, 128, 0), RGB(192, 192, 192),
			RGB(128, 128, 128), RGB(0, 0, 255), RGB(0, 255, 0), RGB(0, 255, 255),
			RGB(255, 0, 0), RGB(255, 0, 255), RGB(255, 255, 0), RGB(255, 255, 255)
	};

	// Create texture with our palette and load it from file
	cg::Texture tex{ "image-half.bmp", palette };

	// Create sprite with our texture
	cg::Sprite sprite{ tex };
	// Set position to (25, 25)
	sprite.setPos({ 25, 25 });
	
	// Create console with (200, 200) canvas size and (4, 4) font size
	cg::RenderConsole console{ { 200, 100 }, { 4u, 4u } };

	// Show error if console cunstruction failed
	if (!console.create())
		MessageBoxA(0, "Error: can't create console", "Error", MB_ICONERROR);

	// Apply our palette to console
	console.setPalette(palette);

	// Start main cycle
	while (true)
	{
		// Clear screen with black color
		console.fill(CHAR_INFO{ L' ', 0x00 });

		// Rules for move direction
		static bool direction = false;
		if (sprite.getPos().x % console.getResolution().x == 0)
			direction = !direction;

		// Move our sprite
		sprite.move(cg::Vec2i( direction ? 1 : -1, 0 ));
		// Draw sprite on the canvas
		console.drawSprite(sprite);

		// Break if cannot display canvas
		if (!console.display())
		{
			MessageBoxA(0, "Error: can't display console", "Error", MB_ICONERROR);
			break;
		}

		// Break after 10s of demo running
		if (elapsed >= 10s)
			break;
	}
}
  ```

### Installation
VS2017 Solution is provided, do wtf you want with it :)

### Todos
 - Add full documentation
 - Add input handling
 - ~~Add console pallete change support~~
 - Add different fonts support
 - Add several features in sprite rendering like rotating, stretching, scaling and so on
 - Optimize code and bring it to the one style
