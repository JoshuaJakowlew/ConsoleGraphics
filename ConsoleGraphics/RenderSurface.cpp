#include "RenderSurface.h"

namespace cg
{
	RenderSurface::RenderSurface(Vec2u size) :
		RenderSurface(size.x, size.y)
	{}

	RenderSurface::RenderSurface(unsigned width, unsigned height) :
		m_surfaceSize{ width, height },
		m_surface{ width * height, makeCharInfo(' ', Color::Black, Color::Gray) }
	{
		m_surface.shrink_to_fit();
	}

	void RenderSurface::putCell(Vec2i coords, Color color)
	{
		putCell(coords.x, coords.y, color.toCharInfo());
	}

	void RenderSurface::putCell(int x, int y, Color color)
	{
		putCell(x, y, color.toCharInfo());
	}

	void RenderSurface::putCell(Vec2i coords, CHAR_INFO color)
	{
		putCell(coords.x, coords.y, color);
	}

	void RenderSurface::putCell(int x, int y, CHAR_INFO color)
	{
		[[unliekly]]
		if (!on_surface(x, y))
			return;

		size_t pos = y * m_surfaceSize.x + x;
		m_surface[pos] = color;
	}

	void RenderSurface::drawLine(Vec2i startPos, Vec2i endPos, Color color)
	{
		drawLine(startPos.x, startPos.y, endPos.x, endPos.y, color.toCharInfo());
	}

	void RenderSurface::drawLine(int x0, int y0, int x1, int y1, Color color)
	{
		drawLine(x0, y0, x1, y1, color.toCharInfo());
	}

	void RenderSurface::drawLine(Vec2i startPos, Vec2i endPos, CHAR_INFO color)
	{
		drawLine(startPos.x, startPos.y, endPos.x, endPos.y, color);
	}

	void RenderSurface::drawLine(int x0, int y0, int x1, int y1, CHAR_INFO color)
	{
		//clamp(x0, y0, x1, y1);

		bool steep = false;
		if (std::abs(x0 - x1) < std::abs(y0 - y1))
		{
			std::swap(x0, y0);
			std::swap(x1, y1);
			steep = true;
		}
		if (x0 > x1) {
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		int dx = x1 - x0;
		int dy = y1 - y0;

		int derror2 = std::abs(dy) * 2;
		int error2 = 0;

		int y = y0;
		for (int x = x0; x <= x1; x++)
		{
			if (steep) putCell(y, x, color);
			else putCell(x, y, color);

			error2 += derror2;

			if (error2 > dx)
			{
				y += ((y1 > y0) ? 1 : -1);
				error2 -= dx * 2;
			}
		}
	}

	void RenderSurface::drawRect(const IntRect& rect, Color color)
	{
		drawRect(rect.x, rect.y, rect.width, rect.height, color.toCharInfo());
	}

	void RenderSurface::drawRect(Vec2i coords, Vec2i size, Color color)
	{
		drawRect(coords.x, coords.y, size.x, size.y, color.toCharInfo());
	}

	void RenderSurface::drawRect(int x, int y, int width, int height, Color color)
	{
		drawRect(x, y, width, height, color.toCharInfo());
	}

	void RenderSurface::drawRect(const IntRect& rect, CHAR_INFO color)
	{
		drawRect(rect.x, rect.y, rect.width, rect.height, color);
	}

	void RenderSurface::drawRect(Vec2i coords, Vec2i size, CHAR_INFO color)
	{
		drawRect(coords.x, coords.y, size.x, size.y, color);
	}

	void RenderSurface::drawRect(int x, int y, int width, int height, CHAR_INFO color)
	{
		if (width > 0) width = width - 1;
		if (height > 0) height = height - 1;

		if (width < 0) width = width + 1;
		if (height < 0) height = height + 1;

		

		drawLine(x, y, x + width, y, color);
		drawLine(x + width, y, x + width, y + height, color);
		drawLine(x + width, y + height, x, y + height, color);
		drawLine(x, y + height, x, y, color);
	}

	void RenderSurface::fillRect(const IntRect& rect, Color color)
	{
		fillRect(rect.x, rect.y, rect.width, rect.height, color.toCharInfo());
	}

	void RenderSurface::fillRect(Vec2i coords, Vec2i size, Color color)
	{
		fillRect(coords.x, coords.y, size.x, size.y, color.toCharInfo());
	}

	void RenderSurface::fillRect(int x, int y, int width, int height, Color color)
	{
		fillRect(x, y, width, height, color.toCharInfo());
	}

	void RenderSurface::fillRect(const IntRect& rect, CHAR_INFO color)
	{
		fillRect(rect.x, rect.y, rect.width, rect.height, color);
	}

	void RenderSurface::fillRect(Vec2i coords, Vec2i size, CHAR_INFO color)
	{
		fillRect(coords.x, coords.y, size.x, size.y, color);
	}

	void RenderSurface::fillRect(int x, int y, int width, int height, CHAR_INFO color)
	{
		if (width < 0)
		{
			x = x + width + 1;
			width = std::abs(width) + 1;
		}

		if (height < 0)
		{
			y = y + height + 1;
			height = std::abs(height) + 1;
		}

		for (int ypos = y; ypos < height + y; ++ypos)
		{
			for (int xpos = x; xpos < width + x; ++xpos)
			{
				putCell(xpos, ypos, color);
			}
		}
	}

	void RenderSurface::drawString(Vec2i coords, std::wstring_view str, Color color)
	{
		drawString(coords.x, coords.y, str, color.toCharInfo());
	}

	void RenderSurface::drawString(int x, int y, std::wstring_view str, Color color)
	{
		drawString(x, y, str, color.toCharInfo());
	}

	void RenderSurface::drawString(Vec2i coords, std::wstring_view str, CHAR_INFO color)
	{
		drawString(coords.x, coords.y, str, color);
	}

	void RenderSurface::drawString(int x, int y, std::wstring_view str, CHAR_INFO color)
	{
		for (int i = 0; i < str.size(); ++i)
		{
			color.Char.UnicodeChar = str[i];
			putCell(x + i, y, color);
		}
	}

	void RenderSurface::drawStringAlpha(Vec2i coords, std::wstring_view str, Color color)
	{
		drawStringAlpha(coords.x, coords.y, str, color.toCharInfo());
	}

	void RenderSurface::drawStringAlpha(int x, int y, std::wstring_view str, Color color)
	{
		drawStringAlpha(x, y, str, color.toCharInfo());
	}

	void RenderSurface::drawStringAlpha(Vec2i coords, std::wstring_view str, CHAR_INFO color)
	{
		drawStringAlpha(coords.x, coords.y, str, color);
	}

	void RenderSurface::drawStringAlpha(int x, int y, std::wstring_view str, CHAR_INFO color)
	{
		const size_t offset = y * m_surfaceSize.x + x;

		for (int i = 0; i < str.size(); ++i)
		{
			[[unlikely]]
			if (!on_surface(x + i, y))
				continue;

			auto bgColor = getBgColor(m_surface[offset + i].Attributes);
			auto fgColor = getFgColor(color.Attributes);

			m_surface[offset + i] = makeCharInfo(str[i], bgColor, fgColor);
		}
	}

	void RenderSurface::drawSprite(const Sprite & sprite)
	{
		for (unsigned y = 0; y < sprite.getSize().y; ++y)
		{
			for (unsigned x = 0; x < sprite.getSize().x; ++x)
			{
				auto pos = sprite.getPos() + Vec2i{static_cast<int>(x), static_cast<int>(y)};
				auto cellptr = sprite.m_data + (y * sprite.getSize().x + x);
				putCell(pos, *cellptr);
			}
		}
	}

	void RenderSurface::fill(Color color)
	{
		fill(color.toCharInfo());
	}

	void RenderSurface::fill(CHAR_INFO color)
	{
		for (int i = 0; i < m_surface.size(); ++i)
			m_surface[i] = color;
	}

} // namespace cg