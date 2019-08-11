#ifndef SPRITE_H
#define SPRITE_H

#include "Vec2.h"
#include "Texture.h"

namespace cg
{
	class Sprite
	{
	public:
		friend class RenderSurface;

		Sprite(const Texture& texture) noexcept;

		inline auto getPos() const noexcept -> Vec2i;
		inline auto getSize() const noexcept -> Vec2i;
		inline auto getOrigin() const noexcept -> Vec2i;

		inline auto setPos(Vec2i pos) noexcept -> void;
		inline auto setOrigin(Vec2i origin) noexcept -> void;
		inline auto move(Vec2i offset) noexcept -> void;
	private:
		Vec2i m_pos = { 0, 0 };
		Vec2i m_size;
		Vec2i m_origin;
		const CHAR_INFO* m_data = nullptr;
	};

	inline Sprite::Sprite(const Texture & texture) noexcept :
		m_size{ texture.getSize() },
		m_data{ texture.getBuffer() },
		m_origin{ m_size / 2 }
	{}

	inline auto cg::Sprite::getPos() const noexcept -> Vec2i
	{
		return m_pos;
	}

	inline auto Sprite::getSize() const noexcept -> Vec2i
	{
		return m_size;
	}

	inline auto Sprite::getOrigin() const noexcept -> Vec2i
	{
		return m_origin;
	}

	inline auto Sprite::setPos(Vec2i pos) noexcept -> void
	{
		m_pos = pos;
	}

	inline auto Sprite::setOrigin(Vec2i origin) noexcept -> void
	{
		m_origin = origin;
	}

	inline auto Sprite::move(Vec2i offset) noexcept -> void
	{
		m_pos += offset;
	}

} // namespace cg

#endif