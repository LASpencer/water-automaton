#include <algorithm>
#include "WaterAutomaton.h"
#include "Block.h"

const float WaterAutomaton::flow_rate = 0.2f;
const float WaterAutomaton::min_level = 0.01f;

WaterAutomaton::WaterAutomaton()
{
}

WaterAutomaton::WaterAutomaton(std::vector<std::vector<Block>>& world) : m_currentWorld(world), m_nextWorld(world)
{
}

const std::vector<std::vector<Block>>& WaterAutomaton::getCurrentWorld() const
{
	return m_currentWorld;
}

std::vector<std::vector<Block>>& WaterAutomaton::getNextWorld()
{
	return m_nextWorld;
}

void WaterAutomaton::setWorld(std::vector<std::vector<Block>> world)
{
	m_currentWorld = world;
	m_nextWorld = world;
}

void WaterAutomaton::addWater(size_t x, size_t y, float amount)
{
	if (x < m_nextWorld.size() && y < m_nextWorld[x].size()) {
		m_nextWorld[x][y].addWater(amount);
	}
	else {
		throw std::invalid_argument("Block index out of range");
	}
}

void WaterAutomaton::addRock(size_t x, size_t y)
{
	if (x < m_nextWorld.size() && y < m_nextWorld[x].size()) {
		m_nextWorld[x][y].makeRock();
		m_currentWorld[x][y].makeRock();
	}
	else {
		throw std::invalid_argument("Block index out of range");
	}
}

void WaterAutomaton::clearBlock(size_t x, size_t y)
{
	if (x < m_nextWorld.size() && y < m_nextWorld[x].size()) {
		m_nextWorld[x][y].makeOpen();
		m_currentWorld[x][y].makeOpen();
		m_nextWorld[x][y].setWaterLevel(0);
		m_currentWorld[x][y].setWaterLevel(0);
	}
	else {
		throw std::invalid_argument("Block index out of range");
	}
}

void WaterAutomaton::update()
{
	// Change water in nextworld based on currentworld
	for (size_t x = 0; x < m_currentWorld.size(); ++x) {
		for (size_t y = 0; y < m_currentWorld[x].size(); ++y) {
			float waterLevel = m_currentWorld[x][y].getWaterLevel();
			float maxBelow = Block::maxWater(m_currentWorld[x][y].getWaterLevel());
			// If below not full/blocked, flow down
			if (y != 0 &&
				m_currentWorld[x][y - 1].isOpen() &&
				m_currentWorld[x][y - 1].getWaterLevel() < maxBelow) {
				float flow = std::min(maxBelow - m_currentWorld[x][y - 1].getWaterLevel(), waterLevel);
				waterLevel -= flow;
				m_nextWorld[x][y].flowWater(flow, m_nextWorld[x][y - 1]);
			}
			// Flow to sides if more water
			bool flowLeft = false;
			bool flowRight = false;
			float leftAmount = 0.f;
			float rightAmount = 0.f;
			if (x != 0 && m_currentWorld[x - 1][y].isOpen() && m_currentWorld[x - 1][y].getWaterLevel() < waterLevel) {
				flowLeft = true;
				
			}
			if (x + 1 != m_currentWorld.size() && m_currentWorld[x + 1][y].isOpen() && m_currentWorld[x + 1][y].getWaterLevel() < waterLevel) {
				flowRight = true;
			}

			if (flowLeft) {
				float difference = waterLevel - m_currentWorld[x - 1][y].getWaterLevel();
				if(flowRight){
					leftAmount = difference / 3.f;
				}
				else {
					leftAmount = difference / 2.f;
				}
				if (m_currentWorld[x - 1][y].getWaterLevel() < 1.f) {	//limit flow rate if other not full
					leftAmount = std::min(leftAmount, flow_rate);
				}
				waterLevel -= leftAmount;
				m_nextWorld[x][y].flowWater(leftAmount, m_nextWorld[x - 1][y]);
			}
			if (flowRight) {
				float difference = waterLevel - m_currentWorld[x + 1][y].getWaterLevel();
				if (flowLeft) {
					rightAmount = difference / 3.f;
				}
				else {
					rightAmount = difference / 2.f;
				}
				//limit flow rate if other not full
				if (m_currentWorld[x + 1][y].getWaterLevel()<1.f) {
					rightAmount = std::min(rightAmount, flow_rate);
				}
				waterLevel -= rightAmount;
				m_nextWorld[x][y].flowWater(rightAmount, m_nextWorld[x + 1][y]);
			}
			// If more water than max, flow upwards
			if (y + 1 != m_currentWorld[x].size() && m_currentWorld[x][y + 1].isOpen()) {
				float maxWater = Block::maxWater(m_currentWorld[x][y + 1].getWaterLevel());
				if (waterLevel > maxWater) {
					float flowUp = waterLevel - maxWater;
					waterLevel -= rightAmount;
					m_nextWorld[x][y].flowWater(flowUp, m_nextWorld[x][y + 1]);
				}
			}
			//TODO evaporate if bottom level/on rock and lower than minimum water level
			/*if (m_nextWorld[x][y].getWaterLevel() < min_level) {
				m_nextWorld[x][y].setWaterLevel(0.f);
			}*/
		}
	}

	// Copy into currentWorld
	m_currentWorld = m_nextWorld;
}
