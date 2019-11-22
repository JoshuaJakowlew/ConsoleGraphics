#ifndef APPLICATION_H
#define APPLICATION_H

#include "Console.h"
#include "rendering/RenderSurface.h"
#include "EventManager.h"
#include "utils/Clock.h"

namespace cg
{
	class Application
	{
	public:
		Application(
			Vec2u resolution,
			Vec2u fontSize,
			std::wstring_view title = L"Application");

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
	private:
		enum class RenderState
		{
			DrawStart,
			DrawEnd,
			Displayed
		};

		RenderState m_surfaceReady = RenderState::DrawEnd;

		void startDrawThread();
		void runMainLoop();
	};

} // namespace cg

#endif APPLICATION_H
