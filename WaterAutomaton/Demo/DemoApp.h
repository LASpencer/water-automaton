#pragma once
#include "Application.h"
#include "Renderer2D.h"
#include "WaterAutomaton.h"

struct Colour {
	float r, g, b;
};


class DemoApp : public aie::Application
{
public:
	DemoApp();
	virtual ~DemoApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	static const size_t blocks_x, blocks_y;
	static const float block_size, tick_time;

protected:
	aie::Renderer2D*	m_renderer;
	WaterAutomaton		m_automaton;

	float m_timer;

	void drawBlock(size_t x, size_t y);

	Colour calculatePressureColour(float pressure);
};

