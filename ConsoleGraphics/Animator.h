#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <unordered_map>

namespace cg
{
	template <typename Animated, typename Id = std::string>
	class Animator
	{
	public:
		auto addAnimation(Id&& id, Animated&& animation) -> void;
		auto removeAnimation(Id&& id) -> void;

		auto play() -> void;
		auto stop() -> void;
		auto update(Sprite& sprite, float dt) -> void;

		auto playAnimation(Id&& id) -> void;
		auto isPlayingAnimation() -> void;
		auto getPlayingAnimation() -> Animated&;
	private:
		bool m_isPlaying = false;
		Id m_currentAnimation;
		std::unordered_map<Id, Animated> m_animations;
	};

	template<typename Animated, typename Id>
	inline auto Animator<Animated, Id>::addAnimation(Id&& id, Animated&& animation) -> void
	{
		m_currentAnimation = id;
		m_animations.insert(std::make_pair(std::forward<Id>(id), std::forward<Animated>(animation)));
	}

	template<typename Animated, typename Id>
	inline auto Animator<Animated, Id>::removeAnimation(Id&& id) -> void
	{
		if (id == m_currentAnimation && !m_isPlaying)
			return;
		m_animations.erase(std::forward<Id>(id));
	}

	template<typename Animated, typename Id>
	inline auto Animator<Animated, Id>::play() -> void
	{
		m_isPlaying = true;
	}

	template<typename Animated, typename Id>
	inline auto Animator<Animated, Id>::stop() -> void
	{
		m_isPlaying = false;
	}

	template<typename Animated, typename Id>
	inline auto Animator<Animated, Id>::update(Sprite& sprite, float dt) -> void
	{
		m_animations[m_currentAnimation](sprite, dt);
	}

	template<typename Animated, typename Id>
	inline auto Animator<Animated, Id>::playAnimation(Id&& id) -> void
	{
		m_currentAnimation = id;
	}

	template<typename Animated, typename Id>
	inline auto Animator<Animated, Id>::isPlayingAnimation() -> void
	{
		return m_isPlaying;
	}

	template<typename Animated, typename Id>
	inline auto Animator<Animated, Id>::getPlayingAnimation() -> Animated&
	{
		return m_animations[m_currentAnimation];
	}

} // namespace cg

#endif
