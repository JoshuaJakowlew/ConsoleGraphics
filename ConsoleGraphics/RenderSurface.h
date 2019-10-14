#pragma once

#include "Color.h"
#include "Sprite.h"

namespace cg
{
	class RenderSurface
	{
	public:
		inline RenderSurface(Vec2u size);
		inline RenderSurface(size_t width, size_t height);

		[[nodiscard]] inline auto getSize() const noexcept -> Vec2u;

		[[nodiscard]] inline auto getCell(Vec2i coords) -> CHAR_INFO &;
		[[nodiscard]] inline auto getLine(int line) -> CHAR_INFO *;
		[[nodiscard]] inline auto getBuffer() -> CHAR_INFO*;

		auto inline putCellUnsafe(int x, int y, CHAR_INFO color) -> void;
		auto inline putCell(int x, int y, CHAR_INFO color) -> void;

		auto drawLine(int x0, int y0, int x1, int y1, CHAR_INFO color) -> void;
		auto drawRect(int x, int y, int width, int height, CHAR_INFO color) -> void;

		inline auto fill(CHAR_INFO color) -> void;
		auto fillRect(int x, int y, int width, int height, CHAR_INFO color) -> void;

		auto drawString(int x, int y, std::wstring_view str, CHAR_INFO color) -> void;
		auto drawStringAlpha(int x, int y, std::wstring_view str, CHAR_INFO color) -> void;

		auto drawSprite(const Sprite& sprite) -> void;
	private:
		Vec2u m_surfaceSize;
		std::vector<CHAR_INFO> m_surface;

		constexpr auto clamp(int& x0, int& y0 , int& x1, int& y1) -> void;
		constexpr auto clamp(int& x, int& y) -> void;

		constexpr auto on_surface(int x, int y) -> bool;
	};

	inline RenderSurface::RenderSurface(Vec2u size) :
		RenderSurface(size.x, size.y)
	{}

	inline RenderSurface::RenderSurface(size_t width, size_t height) :
		m_surfaceSize{ width, height },
		m_surface{ width * height, makeCharInfo(' ', Color::Black, Color::Gray) }
	{}

	inline auto RenderSurface::getSize() const noexcept -> Vec2u
	{
		return m_surfaceSize;
	} 

	inline auto RenderSurface::getCell(Vec2i coords) -> CHAR_INFO &
	{
		assert(
			coords.x >= 0 &&
			coords.y >= 0 &&
			coords.x < static_cast<long long>(m_surfaceSize.x) &&
			coords.y < static_cast<long long>(m_surfaceSize.y)
		);
		return m_surface[m_surfaceSize.x * coords.y + coords.x];
	}

	inline auto RenderSurface::getLine(int line) -> CHAR_INFO *
	{
		assert(
			line >= 0 &&
			line < static_cast<long long>(m_surfaceSize.y)
		);
		assert(&m_surface[m_surfaceSize.x * line] != nullptr);
		return &m_surface[m_surfaceSize.x * line];
	}

	inline auto RenderSurface::getBuffer() -> CHAR_INFO *
	{
		assert(&m_surface[0] != nullptr);
		return &m_surface[0];
	}

	inline auto RenderSurface::putCellUnsafe(int x, int y, CHAR_INFO color) -> void
	{
		const auto pos = y * m_surfaceSize.x + x;
		m_surface[pos] = color;
	}

	inline auto RenderSurface::putCell(int x, int y, CHAR_INFO color) -> void
	{
		[[unlikely]]
		if (!on_surface(x, y))
			return;

		putCellUnsafe(x, y, color);
	}

	inline auto RenderSurface::fill(CHAR_INFO color) -> void
	{
		std::fill(m_surface.begin(), m_surface.end(), color);
	}

	constexpr auto RenderSurface::clamp(int & x0, int & y0, int & x1, int & y1) -> void
	{
		x0 = std::clamp(x0, 0, static_cast<int>(m_surfaceSize.x) - 1);
		y0 = std::clamp(y0, 0, static_cast<int>(m_surfaceSize.y) - 1);
		x1 = std::clamp(x1, 0, static_cast<int>(m_surfaceSize.x) - 1);
		y1 = std::clamp(y1, 0, static_cast<int>(m_surfaceSize.y) - 1);
	}

	constexpr auto RenderSurface::clamp(int & x, int & y) -> void
	{
		x = std::clamp(x, 0, static_cast<int>(m_surfaceSize.x) - 1);
		y = std::clamp(y, 0, static_cast<int>(m_surfaceSize.y) - 1);
	}

	constexpr auto RenderSurface::on_surface(int x, int y) -> bool
	{
		return 
			x >= 0 &&
			y >= 0 &&
			x < static_cast<int>(m_surfaceSize.x) &&
			y < static_cast<int>(m_surfaceSize.y);
	}

} // namespace cg
