#ifndef RENDER_CONSOLE_H
#define RENDER_CONSOLE_H

#include <Windows.h>

#include "Console.h"
#include "RenderSurface.h"

namespace cg
{
	class RenderConsole : public Console<RenderConsole>, public RenderSurface
	{
	public:
		explicit RenderConsole(Vec2u resolution, Vec2u font_size);
		RenderConsole(const RenderConsole&) = delete;
		RenderConsole& operator=(const RenderConsole&) = delete;
		
		[[nodiscard]] bool display() noexcept;
	};

} // namespace cg

#endif