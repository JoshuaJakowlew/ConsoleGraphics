#include <iostream>
#include <string>
#include <chrono>

#include "cg.h"

using namespace std::literals;

int main(int argc, char* argv[])
{
	if (argc != 6)
	{
		// Run default configuration if no command-line params provided
		cg::LuaApplication app{ {200, 100}, {4, 4}, L"Test" };
		app.start();
		
		// std::puts("Not enough parameters");
		// return 1;
	}

	const cg::Vec2u resolution{
		cg::Vec2i{
			std::atoi(argv[1]),
			std::atoi(argv[2])
		}
	};

	const cg::Vec2u fontSize{
		cg::Vec2i{
			std::atoi(argv[3]),
			std::atoi(argv[4])
		}
	};

	const std::string title = argv[5];
	const std::wstring wtitle{ std::begin(title), std::end(title) };

	cg::LuaApplication app{ resolution, fontSize, wtitle };
	app.start();
}
