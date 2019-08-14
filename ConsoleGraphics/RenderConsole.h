#ifndef RENDER_CONSOLE_H
#define RENDER_CONSOLE_H

#include <Windows.h>

#include "Console.h"
#include "RenderSurface.h"

namespace cg
{
	template <size_t W, size_t H>
	class RenderConsole :
		public Console<RenderConsole<W, H>>,
		public RenderSurface<W, H>
	{
	public:
		explicit RenderConsole(Vec2u resolution, Vec2u font_size);
		RenderConsole(const RenderConsole&) = delete;
		RenderConsole& operator=(const RenderConsole&) = delete;
		
		[[nodiscard]] bool display() noexcept;
	};

	template<size_t W, size_t H>
	inline RenderConsole<W, H>::RenderConsole(Vec2u resolution, Vec2u font_size) :
		Console<RenderConsole>(resolution, font_size),
		RenderSurface<W, H>(resolution)
	{}

	template<size_t W, size_t H>
	inline bool RenderConsole<W, H>::display() noexcept
	{
		assert(m_resolution.x <= SHRT_MAX - 1 && m_resolution.y <= SHRT_MAX - 1);

		static COORD screenSize = {
			static_cast<SHORT>(this->m_resolution.x),
			static_cast<SHORT>(this->m_resolution.y)
		};

		static SMALL_RECT writeRegion = {
			0,
			0,
			static_cast<SHORT>(this->m_resolution.x - 1),
			static_cast<SHORT>(this->m_resolution.y - 1)
		};

		auto buffer = this->getBuffer();
		assert(buffer);
		assert(m_handles.out != INVALID_HANDLE_VALUE);

		return ::WriteConsoleOutputW(this->m_handles.out, buffer, screenSize, { 0, 0 }, &writeRegion);
	}

} // namespace cg

#endif