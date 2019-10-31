#ifndef FRAME_ANIMATION_H
#define FRAME_ANIMATION_H

#include <vector>
#include <tuple>

#include "Sprite.h"

namespace cg
{
	class FrameAnimation
	{
	public:
		void addFrame(float duration, const Texture::Rect& subrect);
		void operator()(Sprite& sprite, float dt) const;
	private:
		struct Frame
		{
			Vec2u left;
			Vec2u right;
			float duration;
		};

		std::vector<Frame> m_anims;
		mutable size_t m_currentFrame = 0;
		mutable float m_elapsedTime;
	};

	void FrameAnimation::addFrame(float duration, const Texture::Rect& subrect)
	{
		m_anims.emplace_back(duration, subrect);
	}

	inline void FrameAnimation::operator()(Sprite& sprite, float dt) const
	{
		m_elapsedTime += dt;
		const auto frame = m_anims[m_currentFrame];
		if (m_elapsedTime >= frame.duration)
		{
			m_elapsedTime -= frame.duration;
			++m_currentFrame;
			sprite.setTextureRect(frame.left, frame.right);
		}
	}

} // namespace cg

#endif
