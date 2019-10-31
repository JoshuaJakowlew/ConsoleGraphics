#ifndef APPLICATION_H
#define APPLICATION_H

#include "Console.h"
#include "RenderSurface.h"
#include "EventManager.h"
#include "Clock.h"

#include "ctpl_stl.h"

namespace cg
{
	class Application
	{
	public:
		Application(Vec2u resolution, Vec2u fontSize);

		void start();
	protected:
		EventManager::EventQueue m_events{};

		Console m_console;
		RenderSurface m_surface;
		EventManager m_eventManager;
		Clock m_clock;

		virtual void setup() noexcept = 0;
		virtual void processEvent(const Event& e) noexcept = 0;
		virtual void update(float dt) noexcept = 0;
		virtual void draw() noexcept = 0;

		enum class RenderState
		{
			DrawStart,
			DrawEnd,
			Displayed
		};

		RenderState m_surfaceReady = RenderState::DrawEnd;
	};

} // namespace cg

#endif APPLICATION_H
