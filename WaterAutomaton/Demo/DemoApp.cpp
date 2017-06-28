#include <algorithm>
#include "DemoApp.h"
#include "Block.h"
#include "Input.h"

const float DemoApp::tick_time = 0.1f;
const float DemoApp::block_size = 25.f;
const size_t DemoApp::blocks_x = 40;
const size_t DemoApp::blocks_y = 20;

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
			if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
				// add water
				m_automaton.addWater(mouseBlockX, mouseBlockY);
			}
			else if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_MIDDLE)) {
				//add rock
				m_automaton.addRock(mouseBlockX, mouseBlockY);
			}
			else if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT)) {
				//remove from block
				m_automaton.clearBlock(mouseBlockX, mouseBlockY);
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
			drawBlock(i, j);
		}
	}
	m_renderer->end();
}

void DemoApp::drawBlock(size_t x, size_t y)
{
	if (x > m_automaton.getCurrentWorld().size() || y > m_automaton.getCurrentWorld()[x].size()) {
		throw std::invalid_argument("Block index out of bounds");
	}
	Block block = m_automaton.getCurrentWorld()[x][y];
	float drawHeight = 1.f;
	if (block.isOpen()) {
		//TODO: Set colour based on total water (not depth)
		float depth = std::min(1.f, block.getWaterLevel());
		//if (depth > 0.01f &&
		//	depth < 1.f &&
		//	y + 1 != m_automaton.getCurrentWorld()[x].size() &&
		//	m_automaton.getCurrentWorld()[x][y + 1].getWaterLevel() > 0.01f) {
		//	// TODO: Set colour based on depth
		//	m_renderer->setRenderColour(0x90c0ffff);
		//}
		//else {
			drawHeight = depth;
			//TODO use interpolation instead of this ugly thing
			if (block.getWaterLevel() < 1.2f) {
				m_renderer->setRenderColour(0x0000ffff);
			}
			else if (block.getWaterLevel() < 1.7f) {
				m_renderer->setRenderColour(0x2000ffff);
			}
			else if (block.getWaterLevel() < 2.2f) {
				m_renderer->setRenderColour(0x4000ffff);
			} else if (block.getWaterLevel() < 2.7f) {
				m_renderer->setRenderColour(0x6000ffff);
			}
			else if (block.getWaterLevel() < 3.2f) {
				m_renderer->setRenderColour(0x8000ffff);
			}
			else if (block.getWaterLevel() < 3.7f) {
				m_renderer->setRenderColour(0xA000ffff);
			}
			else if (block.getWaterLevel() < 4.2f) {
				m_renderer->setRenderColour(0xC000ffff);
			}
			else {
				m_renderer->setRenderColour(0xC000D0ff);
			}
		//}
	}
	else {
		m_renderer->setRenderColour(0x808080ff);
	}
	float xPos = (x + 0.5f) * block_size;
	float yPos = (y + 0.5f*drawHeight) * block_size;
	if (drawHeight > 0.f) {
		m_renderer->drawBox(xPos, yPos, block_size, drawHeight*block_size);
	}
}
