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

  Let's assume, that we want to draw a sprite in the screen with custom palette, and move it with mouse. This example will cover a lot of usage principles.
  Let's create basic main function. We will write all the code inside it.

  ```C++
#include "RenderConsole.h"

int main()
{
	// Add your code here
}
  ```

  First of all, we should create custom palette. It's just a simple collection of 16 RGB colors. Our palette is similar to the standart one, except the first color.
  Now our sprite will use colors from this palette, not the system one, or texture's source image pixels.

  ```C++
  cg::Palette palette{
			RGB(53, 14, 88), RGB(0, 0, 128), RGB(0, 128, 0), RGB(0, 128, 128),
			RGB(128, 0, 0), RGB(128, 0, 128), RGB(128, 128, 0), RGB(192, 192, 192),
			RGB(128, 128, 128), RGB(0, 0, 255), RGB(0, 255, 0), RGB(0, 255, 255),
			RGB(255, 0, 0), RGB(255, 0, 255), RGB(255, 255, 0), RGB(255, 255, 255)
	};
  ```

  We want to display an image, so, let's load it! We divide all the graphics in two entities. Textures and sprites. Texture is the entire data. It's stored somewhere in one copy.
  When we want to display this data, we must create a sprite. Sprite is a pointer to texture, coordinates on the screen, and so on.

  ```C++
// Load texture from file and bind our palette
cg::Texture tex{ "image.bmp", palette };

// Create sprite from texture
cg::Sprite sprite{ tex };
// And set the position at given coords
sprite.setPos({ 25, 25 });
  ```

  Now we can create our console. We must specify the boundary and font size.
  ```C++
// Create console with 200x100 screen size and 4x4 cell size
cg::RenderConsole console{ { 200, 100 }, { 4, 4 } };
if (!console.isOpen())
	MessageBoxA(0, "Error: can't create console", "Error", MB_ICONERROR);

// Bind our palette to console
console.setPalette(palette);
  ```

  At this time we created console. Now we need to do our logic. Our scheme looks like this: read events, clear screen, draw our data.
  We create main loop. It will run while console is opened - usually while our app is running.
  Then we must read all the input events and catch mouse movement.
  We set sprite coords to pointer coords and then clear screen from the old frame and drow a new one.
 
 ```C++
 // Run entirely
 while (console.isOpen())
	{
		// Read events
		cg::Event e = {};
		while (console.pollEvent(e))
		{
			// If we hit any key - close application
			if (e.type == cg::EventType::KeyPressed || e.type == cg::EventType::MouseClick)
				console.close();
			// If we move mouse - assign sprite coords to the pointer coords
			if (e.type == cg::EventType::MouseMove)
				sprite.setPos({ e.mouseMove.position.X, e.mouseMove.position.Y });
		}

		// Clear draw buffer from the old frame
		console.fill(CHAR_INFO{ L' ', 0x00 });

		// Draw our sprite
		console.drawSprite(sprite);

		// Now our frame is rendered and we can display it on the screen.
		// If something went wrong, or console is already closed - break the main loop
		if (!console.display())
			break;
	}
 ```

### Installation
VS2017 Solution is provided, do wtf you want with it :)

### Todos
 - Add full documentation
 - ~~Add input handling~~
 - Add more complex input handling
 - ~~Add console pallete change support~~
 - Add different fonts support
 - Add several features in sprite rendering like rotating, stretching, scaling and so on
 - Optimize code and bring it to the one style
