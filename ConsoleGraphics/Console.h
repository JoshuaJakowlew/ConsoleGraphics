#ifndef CONSOLE_H
#define CONSOLE_H

#include <Windows.h>

#include "RenderSurface.h"

namespace cg
{
	class Console : public RenderSurface
	{
	public:
		Console(Vec2u resolution, Vec2u font_size);
		Console(const Console&) = delete;
		Console& operator=(const Console&) = delete;
		~Console();

		bool create();
		void display();

		inline Vec2u getResolution() const noexcept;
		inline Vec2u getMaxResolution() const noexcept;
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

		Vec2u getMaxScreenBufferSize();
		bool getStdHandles();
		bool createScreenBuffer();
		bool configureOutput();
		bool assignScreenBuffer();
		bool setFontSize(Vec2u fontSize);
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