#pragma once

#include <vector>
#include <array>
#include <string_view>
#include <cassert>

#include <Windows.h>

#include "Vec2.h"
#include "Color.h"
#include "Sprite.h"

namespace cg
{
	template <size_t W, size_t H>
	class RenderSurface
	{
	public:
		constexpr RenderSurface(Vec2u size);
		constexpr RenderSurface(size_t width, size_t height);

		[[nodiscard]] constexpr auto getSize() const noexcept -> Vec2u;

		[[nodiscard]] constexpr auto getCell(Vec2i coords) -> CHAR_INFO &;
		[[nodiscard]] constexpr auto getLine(int line) -> CHAR_INFO *;
		[[nodiscard]] constexpr auto getBuffer() -> CHAR_INFO*;

		constexpr auto putCellUnsafe(int x, int y, CHAR_INFO color) -> void;
		constexpr auto putCell(int x, int y, CHAR_INFO color) -> void;

		constexpr auto drawLine(int x0, int y0, int x1, int y1, CHAR_INFO color) -> void;
		constexpr auto drawRect(int x, int y, int width, int height, CHAR_INFO color) -> void;

		constexpr auto fill(CHAR_INFO color) -> void;
		constexpr auto fillRect(int x, int y, int width, int height, CHAR_INFO color) -> void;

		constexpr auto drawString(int x, int y, std::wstring_view str, CHAR_INFO color) -> void;
		constexpr auto drawStringAlpha(int x, int y, std::wstring_view str, CHAR_INFO color) -> void;

		constexpr auto drawSprite(const Sprite& sprite) -> void;
	private:
		Vec2u m_surfaceSize;
		std::array<CHAR_INFO, W * H> m_surface;

		constexpr auto clamp(int& x0, int& y0 , int& x1, int& y1) -> void;
		constexpr auto clamp(int& x, int& y) -> void;

		constexpr auto on_surface(int x, int y) -> bool;
	};

	template <size_t W, size_t H>
	constexpr RenderSurface<W, H>::RenderSurface(Vec2u size) :
		RenderSurface(size.x, size.y)
	{}

	template <size_t W, size_t H>
	constexpr RenderSurface<W, H>::RenderSurface(size_t width, size_t height) :
		m_surfaceSize{ width, height },
		m_surface{ makeCharInfo(' ', Color::Black, Color::Gray) }
	{}

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::drawLine(int x0, int y0, int x1, int y1, CHAR_INFO color) -> void
	{
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

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::drawRect(int x, int y, int width, int height, CHAR_INFO color) -> void
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

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::fillRect(int x, int y, int width, int height, CHAR_INFO color) -> void
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

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::drawString(int x, int y, std::wstring_view str, CHAR_INFO color) -> void
	{
		int endX = x + str.length();
		clamp(x, y, endX, y);

		for (int i = x; i <= endX; ++i)
		{
			color.Char.UnicodeChar = str[i - x];
			putCellUnsafe(i, y, color);
		}
	}

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::drawStringAlpha(int x, int y, std::wstring_view str, CHAR_INFO color) -> void
	{
		const size_t offset = y * m_surfaceSize.x + x;

		int endX = x + str.length();
		clamp(x, y, endX, y);

		for (int i = 0; i <= endX - x; ++i)
		{
			auto bgColor = getBgColor(m_surface[offset + i].Attributes);
			auto fgColor = getFgColor(color.Attributes);

			m_surface[offset + i] = makeCharInfo(str[i], bgColor, fgColor);
		}
	}

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::drawSprite(const Sprite& sprite) -> void
	{
		/*auto left = sprite.getPos();
		auto right = sprite.getPos() + sprite.getSize();

		clamp(left.x, right.y);
		clamp(right.x, right.y);

		for (auto y = left.y; y <= right.y; ++y)
		{
			const std::uintptr_t offset = y * sprite.getSize().x;
			const auto srcLine = sprite.getData() + offset;
			const auto dstLine = &m_surface[0] + offset + left.x;
			const auto size = (right.x - left.x + 1) * sizeof(CHAR_INFO);

			std::memcpy(dstLine, srcLine, size);
		}*/
		for (unsigned y = 0; y < sprite.getSize().y; ++y)
		{
			const auto line_offset = sprite.getData() + y * sprite.getSize().x;
			for (unsigned x = 0; x < sprite.getSize().x; ++x)
			{
				//const auto pos = sprite.getPos() + Vec2i{static_cast<int>(x), static_cast<int>(y)};
				const auto xPos = sprite.getPos().x + x;
				const auto yPos = sprite.getPos().y + y;
				const auto cellptr = line_offset + x;
				putCell(xPos, yPos, *cellptr);
			}
		}
	}

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::fill(CHAR_INFO color) -> void
	{
		std::fill(m_surface.begin(), m_surface.end(), color);
	}

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::getSize() const noexcept -> Vec2u
	{
		return m_surfaceSize;
	} 

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::getCell(Vec2i coords) -> CHAR_INFO &
	{
		assert(
			coords.x >= 0 &&
			coords.y >= 0 &&
			coords.x < static_cast<long long>(m_surfaceSize.x) &&
			coords.y < static_cast<long long>(m_surfaceSize.y)
		);
		return m_surface[m_surfaceSize.x * coords.y + coords.x];
	}

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::getLine(int line) -> CHAR_INFO *
	{
		assert(
			line >= 0 &&
			line < static_cast<long long>(m_surfaceSize.y)
		);
		assert(&m_surface[m_surfaceSize.x * line] != nullptr);
		return &m_surface[m_surfaceSize.x * line];
	}

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::getBuffer() -> CHAR_INFO *
	{
		assert(&m_surface[0] != nullptr);
		return &m_surface[0];
	}

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::putCellUnsafe(int x, int y, CHAR_INFO color) -> void
	{
		const auto pos = y * m_surfaceSize.x + x;
		m_surface[pos] = color;
	}

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::putCell(int x, int y, CHAR_INFO color) -> void
	{
		[[unlikely]]
		if (!on_surface(x, y))
			return;

		putCellUnsafe(x, y, color);
	}

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::clamp(int & x0, int & y0, int & x1, int & y1) -> void
	{
		x0 = std::clamp(x0, 0, static_cast<int>(m_surfaceSize.x) - 1);
		y0 = std::clamp(y0, 0, static_cast<int>(m_surfaceSize.y) - 1);
		x1 = std::clamp(x1, 0, static_cast<int>(m_surfaceSize.x) - 1);
		y1 = std::clamp(y1, 0, static_cast<int>(m_surfaceSize.y) - 1);
	}

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::clamp(int & x, int & y) -> void
	{
		x = std::clamp(x, 0, static_cast<int>(m_surfaceSize.x) - 1);
		y = std::clamp(y, 0, static_cast<int>(m_surfaceSize.y) - 1);
	}

	template <size_t W, size_t H>
	constexpr auto RenderSurface<W, H>::on_surface(int x, int y) -> bool
	{
		return 
			x >= 0 &&
			y >= 0 &&
			x < static_cast<int>(m_surfaceSize.x) &&
			y < static_cast<int>(m_surfaceSize.y);
	}

} // namespace cg
