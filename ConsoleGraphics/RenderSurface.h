#ifndef RENDER_SURFACE_H
#define RENDER_SURFACE_H

#include <vector>
#include <string_view>
#include <cassert>

#include <Windows.h>

#include "Vec2.h"
#include "Rect.h"
#include "Color.h"

namespace cg
{
	class RenderSurface
	{
	public:
		RenderSurface(Vec2u size);
		RenderSurface(unsigned width, unsigned height);

		inline Vec2u getSize() const noexcept;

		inline CHAR_INFO& getCell(Vec2i coords);
		inline CHAR_INFO * getLine(int line);
		inline CHAR_INFO * getBuffer();
		void putCell(Vec2i coords, Color color);
		void putCell(int x, int y, Color color);
		void putCell(Vec2i coords, CHAR_INFO color);
		void putCell(int x, int y, CHAR_INFO color);

		void drawLine(Vec2i startPos, Vec2i endPos, Color color);
		void drawLine(int x0, int y0, int x1, int y1, Color color);
		void drawLine(Vec2i startPos, Vec2i endPos, CHAR_INFO color);
		void drawLine(int x0, int y0, int x1, int y1, CHAR_INFO color);

		void drawRect(const IntRect& rect, Color color);
		void drawRect(Vec2i coords, Vec2i size, Color color);
		void drawRect(int x, int y, int width, int height, Color color);
		void drawRect(const IntRect& rect, CHAR_INFO color);
		void drawRect(Vec2i coords, Vec2i size, CHAR_INFO color);
		void drawRect(int x, int y, int width, int height, CHAR_INFO color);

		void fillRect(const IntRect& rect, Color color);
		void fillRect(Vec2i coords, Vec2i size, Color color);
		void fillRect(int x, int y, int width, int height, Color color);
		void fillRect(const IntRect& rect, CHAR_INFO color);
		void fillRect(Vec2i coords, Vec2i size, CHAR_INFO color);
		void fillRect(int x, int y, int width, int height, CHAR_INFO color);

		void drawString(Vec2i coords, std::wstring_view str, Color color);
		void drawString(int x, int y, std::wstring_view str, Color color);
		void drawString(Vec2i coords, std::wstring_view str, CHAR_INFO color);
		void drawString(int x, int y, std::wstring_view str, CHAR_INFO color);

		void drawStringAlpha(Vec2i coords, std::wstring_view str, Color color);
		void drawStringAlpha(int x, int y, std::wstring_view str, Color color);
		void drawStringAlpha(Vec2i coords, std::wstring_view str, CHAR_INFO color);
		void drawStringAlpha(int x, int y, std::wstring_view str, CHAR_INFO color);

		void fill(Color color);
		void fill(CHAR_INFO color);
	private:
		unsigned m_width;
		unsigned m_height;
		std::vector<CHAR_INFO> m_surface;
	};
} // namespace cg

namespace cg
{
	inline Vec2u RenderSurface::getSize() const noexcept
	{
		return { m_width, m_height };
	}

	inline CHAR_INFO& RenderSurface::getCell(Vec2i coords)
	{
		assert(coords.x >= 0 && coords.y >= 0 && coords.x < m_width && coords.y < m_height);
		return m_surface[m_width * coords.y + coords.x];
	}

	CHAR_INFO * RenderSurface::getLine(int line)
	{
		assert(line >= 0 && line < m_height);
		assert(&m_surface[m_width * line] != nullptr);
		return &m_surface[m_width * line];
	}

	CHAR_INFO * RenderSurface::getBuffer()
	{
		assert(&m_surface[0] != nullptr);
		return &m_surface[0];
	}
} // namespace cg

#endif
