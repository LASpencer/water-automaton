#include <algorithm>
#include "DemoApp.h"
#include "Block.h"
#include "Input.h"

const float DemoApp::tick_time = 0.1f;
const float DemoApp::block_size = 50.f;
const size_t DemoApp::blocks_x = 20;
const size_t DemoApp::blocks_y = 10;

DemoApp::DemoApp()
{
}


DemoApp::~DemoApp()
{
}

bool DemoApp::startup()
{
	m_timer = 0.f;
	m_renderer = new aie::Renderer2D();
	m_automaton.setWorld(std::vector<std::vector<Block>>(blocks_x, std::vector<Block>(blocks_y)));
	m_automaton.getNextWorld()[10][9].setWaterLevel(1);
	return true;
}

void DemoApp::shutdown()
{
	delete m_renderer;
}

void DemoApp::update(float deltaTime)
{
	m_timer += deltaTime;
	while (m_timer > tick_time) {
		m_timer -= tick_time;
		m_automaton.update();
	}
	//TODO input to place blocks, place water, delete
	aie::Input* input = aie::Input::getInstance();
	int mouseX, mouseY;
	input->getMouseXY(&mouseX, &mouseY);
	if (mouseX >= 0 && mouseY >= 0) {
		size_t mouseBlockX = mouseX / block_size;
		size_t mouseBlockY = mouseY / block_size;
		if (mouseBlockX < blocks_x && mouseBlockY < blocks_y) {
			Block* mouseBlock = &m_automaton.getNextWorld()[mouseBlockX][mouseBlockY];
			if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
				// add water
				if (mouseBlock->isOpen()) {
					mouseBlock->addWater(1);
				}
			}
			else if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_MIDDLE)) {
				//add rock
				mouseBlock->makeRock();
			}
			else if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT)) {
				//remove from block
				mouseBlock->makeOpen();
				mouseBlock->setWaterLevel(0);
			}
		}
	}
	if (input->wasKeyPressed(aie::INPUT_KEY_ESCAPE)) {
		quit();
	}
}

void DemoApp::draw()
{
	clearScreen();
	m_renderer->begin();
	for (size_t i = 0; i < blocks_x; ++i) {
		for (size_t j = 0; j < blocks_y; ++j) {
			Block b = m_automaton.getCurrentWorld()[i][j];
			float xPos = (i + 0.5f) * block_size;
			float depth = 1.f;
			if (b.isOpen()) {
				depth = std::min(1.f, b.getWaterLevel());
				m_renderer->setRenderColour(0x0000ffff);
			}
			else {
				m_renderer->setRenderColour(0x808080ff);
			}
			float yPos = (j + 0.5f*depth) * block_size;
			if (depth > 0.f) {
				m_renderer->drawBox(xPos, yPos, block_size, depth*block_size);
			}
		}
	}
	m_renderer->end();
}
