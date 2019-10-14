#ifndef SPRITE_H
#define SPRITE_H

#include "Texture.h"

namespace cg
{
	class Sprite
	{
	public:
		Sprite(const Texture& texture) noexcept;

		[[nodiscard]] inline auto getPos() const noexcept -> Vec2i;
		[[nodiscard]] inline auto getActualPos() const noexcept -> Vec2i;
		[[nodiscard]] inline auto getSize() const noexcept -> Vec2i;
		[[nodiscard]] inline auto getOrigin() const noexcept -> Vec2i;
		[[nodiscard]] inline auto getData() const noexcept -> const CHAR_INFO*;

		inline auto setPos(Vec2i pos) noexcept -> void;
		inline auto setOrigin(Vec2i origin) noexcept -> void;
		inline auto move(Vec2i offset) noexcept -> void;

		inline auto isTransparent() const noexcept -> bool;
		inline auto setTransparent(bool transparent) noexcept -> void;
		inline auto setTransparent(bool transparent, CHAR_INFO color) noexcept -> void;
		inline auto getTransparentColor() const noexcept -> CHAR_INFO;
	private:
		Vec2i m_pos = { 0, 0 };
		Vec2i m_size;
		Vec2i m_origin;
		bool m_transparent = false;
		CHAR_INFO m_transparentColor;
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

	inline auto Sprite::getActualPos() const noexcept -> Vec2i
	{
		return m_pos - m_origin;
	}

	inline auto Sprite::getSize() const noexcept -> Vec2i
	{
		return m_size;
	}

	inline auto Sprite::getOrigin() const noexcept -> Vec2i
	{
		return m_origin;
	}

	inline auto Sprite::getData() const noexcept -> const CHAR_INFO*
	{
		return m_data;
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

	inline auto Sprite::isTransparent() const noexcept -> bool
	{
		return m_transparent;
	}

	inline auto Sprite::setTransparent(bool transparent) noexcept -> void
	{
		setTransparent(transparent, *m_data);
	}

	inline auto Sprite::setTransparent(bool transparent, CHAR_INFO color) noexcept -> void
	{
		m_transparent = transparent;
		m_transparentColor = color;
	}

	inline auto Sprite::getTransparentColor() const noexcept -> CHAR_INFO
	{
		return m_transparentColor;
	}

} // namespace cg

#endif