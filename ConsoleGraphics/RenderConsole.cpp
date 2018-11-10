#include "RenderConsole.h"

namespace cg
{
	RenderConsole::RenderConsole(Vec2u resolution, Vec2u font_size) :
		Console<RenderConsole>(resolution, font_size),
		RenderSurface(resolution)
	{}

	bool RenderConsole::display() noexcept
	{
		assert(m_resolution.x <= SHRT_MAX - 1 && m_resolution.y <= SHRT_MAX - 1);

		static COORD screenSize = {
			static_cast<SHORT>(m_resolution.x),
			static_cast<SHORT>(m_resolution.y)
		};

		static SMALL_RECT writeRegion = {
			0,
			0,
			static_cast<SHORT>(m_resolution.x - 1),
			static_cast<SHORT>(m_resolution.y - 1)
		};

		auto buffer = getBuffer();
		assert(buffer);
		assert(m_handles.out != INVALID_HANDLE_VALUE);

		return ::WriteConsoleOutputW(m_handles.out, buffer, screenSize, { 0, 0 }, &writeRegion);
	}



} // namespace cg