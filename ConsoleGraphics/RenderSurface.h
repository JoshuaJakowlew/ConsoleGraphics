#ifndef RENDER_SURFACE_H
#define RENDER_SURFACE_H

#include <vector>
#include <string_view>
#include <cassert>

#include <Windows.h>

#include "Vec2.h"
#include "Rect.h"
#include "Color.h"
#include "Sprite.h"

namespace cg
{
	class RenderSurface
	{
	public:
		RenderSurface(Vec2u size);
		RenderSurface(unsigned width, unsigned height);

		[[nodiscard]] inline Vec2u getSize() const noexcept;

		[[nodiscard]] inline CHAR_INFO& getCell(Vec2i coords);
		[[nodiscard]] inline CHAR_INFO * getLine(int line);
		[[nodiscard]] inline CHAR_INFO * getBuffer();

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

		void drawSprite(const Sprite& sprite);

		void fill(Color color);
		void fill(CHAR_INFO color);
	private:
		Vec2u m_surfaceSize;
		std::vector<CHAR_INFO> m_surface;

		constexpr void clamp(int& x0, int& y0 , int& x1, int& y1);
		constexpr void clamp(int& x, int& y);

		constexpr bool on_surface(int x, int y);
	};
} // namespace cg

namespace cg
{
	inline Vec2u RenderSurface::getSize() const noexcept
	{
		return m_surfaceSize;
	}

	inline CHAR_INFO& RenderSurface::getCell(Vec2i coords)
	{
		assert(
			coords.x >= 0 &&
			coords.y >= 0 &&
			coords.x < static_cast<long long>(m_surfaceSize.x) &&
			coords.y < static_cast<long long>(m_surfaceSize.y)
		);
		return m_surface[m_surfaceSize.x * coords.y + coords.x];
	}

	CHAR_INFO * RenderSurface::getLine(int line)
	{
		assert(
			line >= 0 &&
			line < static_cast<long long>(m_surfaceSize.y)
		);
		assert(&m_surface[m_surfaceSize.x * line] != nullptr);
		return &m_surface[m_surfaceSize.x * line];
	}

	CHAR_INFO * RenderSurface::getBuffer()
	{
		assert(&m_surface[0] != nullptr);
		return &m_surface[0];
	}

	constexpr void RenderSurface::clamp(int & x0, int & y0, int & x1, int & y1)
	{
		x0 = std::clamp(x0, 0, static_cast<int>(m_surfaceSize.x) - 1);
		y0 = std::clamp(y0, 0, static_cast<int>(m_surfaceSize.y) - 1);
		x1 = std::clamp(x1, 0, static_cast<int>(m_surfaceSize.x) - 1);
		y1 = std::clamp(y1, 0, static_cast<int>(m_surfaceSize.y) - 1);
	}

	constexpr void cg::RenderSurface::clamp(int & x, int & y)
	{
		x = std::clamp(x, 0, static_cast<int>(m_surfaceSize.x) - 1);
		y = std::clamp(y, 0, static_cast<int>(m_surfaceSize.y) - 1);
	}

	constexpr bool cg::RenderSurface::on_surface(int x, int y)
	{
		return 
			x >= 0 &&
			y >= 0 &&
			x < static_cast<int>(m_surfaceSize.x) &&
			y < static_cast<int>(m_surfaceSize.y);
	}

} // namespace cg

#endif
