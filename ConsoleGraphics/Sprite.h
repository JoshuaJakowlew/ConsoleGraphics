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

		inline Vec2i getPos() const noexcept;
		inline Vec2u getSize() const noexcept;
		inline Vec2i getOrigin() const noexcept;

		inline void setPos(Vec2i pos) noexcept;
		inline void setOrigin(Vec2i origin) noexcept;
		inline void move(Vec2i offset) noexcept;
	private:
		Vec2i m_pos = { 0, 0 };
		Vec2u m_size = { 0u, 0u };
		Vec2i m_origin = { 0, 0 };
		const CHAR_INFO* m_data = nullptr;
	};

	inline Sprite::Sprite(const Texture & texture) noexcept :
		m_size(texture.getSize()),
		m_data(texture.getBuffer())
	{}

	inline Vec2i cg::Sprite::getPos() const noexcept
	{
		return m_pos;
	}

	inline Vec2u Sprite::getSize() const noexcept
	{
		return m_size;
	}

	inline Vec2i Sprite::getOrigin() const noexcept
	{
		return m_origin;
	}

	inline void Sprite::setPos(Vec2i pos) noexcept
	{
		m_pos = pos;
	}

	inline void Sprite::setOrigin(Vec2i origin) noexcept
	{
		m_origin = origin;
	}

	inline void Sprite::move(Vec2i offset) noexcept
	{
		m_pos += offset;
	}

} // namespace cg

#endif