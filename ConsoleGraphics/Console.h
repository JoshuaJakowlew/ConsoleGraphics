#ifndef CONSOLE_H
#define CONSOLE_H

#include <Windows.h>

#include "RenderSurface.h"

namespace cg
{
	class Console : public RenderSurface
	{
	public:
		explicit Console(Vec2u resolution, Vec2u font_size);
		Console(const Console&) = delete;
		Console& operator=(const Console&) = delete;
		~Console() noexcept;

		[[nodiscard]] bool create() noexcept;
		[[nodiscard]] bool display() noexcept;

		[[nodiscard]] inline Vec2u getResolution() const noexcept;
		[[nodiscard]] inline Vec2u getMaxResolution() const noexcept;
	private:
		struct Handles
		{
			HANDLE original;
			HANDLE out;
			HANDLE in;
		};

		Handles m_handles;

		Vec2u m_resolution;
		Vec2u m_maxResolution;
		Vec2u m_fontSize;

		[[nodiscard]] Vec2u getMaxScreenBufferSize();
		[[nodiscard]] bool getStdHandles();
		[[nodiscard]] bool createScreenBuffer();
		[[nodiscard]] bool configureOutput();
		[[nodiscard]] bool assignScreenBuffer();
		[[nodiscard]] bool disableCursor();
		[[nodiscard]] bool setFontSize(Vec2u fontSize);
	};
} // namespace cg

namespace cg
{
	inline Vec2u Console::getResolution() const noexcept
	{
		return m_resolution;
	}
	inline Vec2u Console::getMaxResolution() const noexcept
	{
		return m_maxResolution;
	}
} // namespace cg

#endif