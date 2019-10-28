#ifndef SPRITE_H
#define SPRITE_H

#include "Texture.h"

namespace cg
{
	class Sprite
	{
	public:
		Sprite(const Texture& texture) noexcept;
		Sprite(const Texture& texture, Vec2u left, Vec2u right) noexcept;

		[[nodiscard]] inline auto getPos() const noexcept -> Vec2i;
		[[nodiscard]] inline auto getActualPos() const noexcept -> Vec2i;
		[[nodiscard]] inline auto getSize() const noexcept -> Vec2u;
		[[nodiscard]] inline auto getOrigin() const noexcept -> Vec2i;

		[[nodiscard]] inline auto operator[](Vec2u pos) const noexcept -> CHAR_INFO;
		[[nodiscard]] inline auto at(Vec2u pos) const noexcept -> CHAR_INFO;
		[[nodiscard]] inline auto at(unsigned x, unsigned y) const noexcept -> CHAR_INFO;

		inline auto setTexture(const Texture* texture, Vec2u left, Vec2u right) noexcept -> void;
		inline auto setTexture(const Texture* texture) noexcept -> void;
		inline auto setTextureRect(Vec2u left, Vec2u right) noexcept -> void;

		inline auto setPos(Vec2i pos) noexcept -> void;
		inline auto setOrigin(Vec2i origin) noexcept -> void;
		inline auto move(Vec2f offset) noexcept -> void;
		inline auto move(Vec2i offset) noexcept -> void;

		inline auto isTransparent() const noexcept -> bool;
		inline auto setTransparent(bool transparent) noexcept -> void;
		inline auto setTransparent(bool transparent, CHAR_INFO color) noexcept -> void;
		inline auto getTransparentColor() const noexcept -> CHAR_INFO;
	private:
		Texture::Rect m_textureRect;
		const Texture* m_texture;
		Vec2i m_pos = { 0, 0 };
		Vec2i m_origin = { 0, 0 };
		CHAR_INFO m_transparentColor;
		bool m_transparent = false;
	};

	inline Sprite::Sprite(const Texture & texture) noexcept :
		Sprite(texture, { 0, 0 }, texture.getSize())
	{}

	inline Sprite::Sprite(const Texture& texture, Vec2u left, Vec2u right) noexcept
	{
		setTexture(&texture, std::move(left), std::move(right));
	}

	inline auto cg::Sprite::getPos() const noexcept -> Vec2i
	{
		return m_pos;
	}

	inline auto Sprite::getActualPos() const noexcept -> Vec2i
	{
		return m_pos - m_origin;
	}

	inline auto Sprite::getSize() const noexcept -> Vec2u
	{
		return { m_textureRect.x, m_textureRect.y };
	}

	inline auto Sprite::getOrigin() const noexcept -> Vec2i
	{
		return m_origin;
	}

	inline auto Sprite::operator[](Vec2u pos) const noexcept -> CHAR_INFO
	{
		return at(pos.x, pos.y);
	}

	inline auto Sprite::at(Vec2u pos) const noexcept -> CHAR_INFO
	{
		return at(pos.x, pos.y);
	}

	inline auto Sprite::at(unsigned x, unsigned y) const noexcept -> CHAR_INFO
	{
		return m_textureRect.at(x, y);
	}

	inline auto Sprite::setTexture(const Texture* texture, Vec2u left, Vec2u right) noexcept -> void
	{
		setTexture(texture);
		setTextureRect(std::move(left), std::move(right));
	}

	inline auto Sprite::setTexture(const Texture* texture) noexcept -> void
	{
		m_texture = texture;
	}

	inline auto Sprite::setTextureRect(Vec2u left, Vec2u right) noexcept -> void
	{
		m_textureRect = m_texture->getRect(std::move(left), std::move(right));
		const Vec2i size(m_textureRect.x, m_textureRect.y);
		m_origin = size / 2;
	}

	inline auto Sprite::setPos(Vec2i pos) noexcept -> void
	{
		m_pos = pos;
	}

	inline auto Sprite::setOrigin(Vec2i origin) noexcept -> void
	{
		m_origin = origin;
	}

	inline auto Sprite::move(Vec2f offset) noexcept -> void
	{
		static Vec2f movement = { 0.f, 0.f };
		movement += offset;

		move(static_cast<Vec2i>(movement));

		movement -= static_cast<Vec2f>(
			static_cast<Vec2i>(movement)
		);
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
		setTransparent(transparent, *m_textureRect.data);
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