#ifndef FRAME_ANIMATION_H
#define FRAME_ANIMATION_H

#include <vector>
#include <tuple>

#include "rendering/Sprite.h"

namespace cg
{
	class FrameAnimation
	{
	public:
		struct Frame
		{
			Frame(Vec2u left, Vec2u right, float duration) :
				left{ std::move(left) },
				right{ std::move(right) },
				duration{ duration }
			{}

			Vec2u left;
			Vec2u right;
			float duration;
		};

		void addFrame(float duration, Vec2u left, Vec2u right);
		void operator()(Sprite& sprite, float dt) const;
	private:
		std::vector<Frame> m_anims;
		mutable size_t m_currentFrame = 0;
		mutable float m_elapsedTime = 0.f;
	};

	inline void FrameAnimation::addFrame(float duration, Vec2u left, Vec2u right)
	{
		m_anims.emplace_back(left, right, duration);
	}

	inline void FrameAnimation::operator()(Sprite& sprite, float dt) const
	{
		m_elapsedTime += dt;
		auto frame = m_anims[m_currentFrame];

		while (m_elapsedTime >= frame.duration)
		{
			m_elapsedTime -= frame.duration;
			++m_currentFrame;
			m_currentFrame %= m_anims.size();
			frame = m_anims[m_currentFrame];
		}
		sprite.setTextureRect(frame.left, frame.right);
	}

} // namespace cg

#endif
