# Console graphics

Console Graphics is a simple C++ library that allows you to use Windows console in a simple way.

### Features
  - Use console screen as a surface - simply output characters at needed coordinates
  - Use colors!
  - Сhange console resoulution and even font size
  - Draw sprites, lines, strings and other stuff in a handy way

### Tutorial
  First of all you need to create and configure the console surface - its resolution and font size.
  
  ```C++
#include "RenderConsole.h"

int main()
{
    // Create console window with 80x25 resolution and 16x12 font size
    cg::RenderConsole console{ {80, 25}, {16, 12} };
    // If everything is not OK - handle errors
    if (!console.create())
        return -1;
		
	// Create a color of the cell we will draw
	// Color consists of a character, background and foreground color
	cg::Color color{ L'#', cg::Color::Red, cg::Color::Blue };
	
	// Start infinite loop to display our stuff
	while(true)
	{
	    // Put cell at position with x = 1 and y = 1 using our color
	    console.putCell({ 1, 1 }, color);
	    // If something went wrong - handle it!
	    if(!console.display())
	        return -2;
	}
}
  ```


You can also:
  - Draw lines
  - Draw rectangles
  - Draw strings
  - Draw sprites

To start drawing a sprite, we must load a texture. Texture is our data and sprite is a rectangle with corrdinates that points to texture.
Let's see how to display a sprite

```C++
// Define a palette of 16 colors
// Image will be converted to this palette
vector<rgb_t> palette{
	{0, 0, 0}, {0, 0, 128}, {0, 128, 0}, {0, 128, 128},
	{128, 0, 0}, {128, 0, 128}, {128, 128, 0}, {192, 192, 192},
	{128, 128, 128}, {0, 0, 255}, {0, 255, 0}, {0, 255, 255},
	{255, 0, 0}, {255, 0, 255}, {255, 255, 0}, {255, 255, 255}
};

// Create and load a texture
cg::Texture tex{std::move(palette));
tex.loadFromBitmap("image.bmp");

// Create our sprite
cg::Sprite sprite{ tex };
sprite.setPos({0, 0 });

// Initialize console
// ...
// Create main loop

while(true)
	{
	    // Finally draw our sprite
        console.drawSprite(sprite);
	    if(!console.display())
	        return -2;
	}
```

### Installation
VS2017 Solution id provided, do wtf you want with it :)

### Todos
 - Add full documentation
 - Add input handling
 - Add different console pallete change support
 - Add different fonts support
 - Add several features in sprite rendering like rotating, stretching, scaling and so on
 - Optimize code and bring it to the one style
