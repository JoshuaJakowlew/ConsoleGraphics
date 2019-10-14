#ifndef RENDER_CONSOLE_H
#define RENDER_CONSOLE_H

#include "Console.h"
#include "RenderSurface.h"

namespace cg
{
	class RenderConsole :
		public Console<RenderConsole>,
		public RenderSurface
	{
	public:
		explicit RenderConsole(Vec2u resolution, Vec2u font_size);
		RenderConsole(const RenderConsole&) = delete;
		RenderConsole& operator=(const RenderConsole&) = delete;
		
		[[nodiscard]] bool display() noexcept;
	};

	inline RenderConsole::RenderConsole(Vec2u resolution, Vec2u font_size) :
		Console<RenderConsole>{ resolution, font_size },
		RenderSurface{ resolution }
	{}

} // namespace cg

#endif