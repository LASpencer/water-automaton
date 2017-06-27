#pragma once
#include "Application.h"
#include "Renderer2D.h"
#include "WaterAutomaton.h"

class DemoApp : public aie::Application
{
public:
	DemoApp();
	virtual ~DemoApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	aie::Renderer2D*	m_2dRenderer;
	WaterAutomaton		m_automaton;

	float m_timer;
};

