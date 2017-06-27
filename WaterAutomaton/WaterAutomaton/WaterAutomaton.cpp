#include <algorithm>
#include "WaterAutomaton.h"
#include "Block.h"

const float WaterAutomaton::flow_rate = 0.2f;

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

void WaterAutomaton::update()
{
	// Change water in nextworld based on currentworld
	for (size_t x = 0; x < m_currentWorld.size(); ++x) {
		for (size_t y = 0; y < m_currentWorld.size(); ++y) {
			float waterLevel = m_currentWorld[x][y].getWaterLevel();
			float maxBelow = Block::maxWater(m_currentWorld[x][y].getWaterLevel());
			// If below not full/blocked, flow down
			if (y != 0 &&
				m_currentWorld[x][y - 1].isOpen() &&
				m_currentWorld[x][y - 1].getWaterLevel() < maxBelow) {
				float flow = std::max(maxBelow - m_currentWorld[x][y - 1].getWaterLevel(), waterLevel);
				waterLevel -= flow;
				m_nextWorld[x][y].flowWater(-flow);
				m_nextWorld[x][y - 1].flowWater(flow);
			}
			// Flow to sides if more water
			bool flowLeft, flowRight = false;
			if (x != 0 && m_currentWorld[x - 1][y].isOpen() && m_currentWorld[x - 1][y].getWaterLevel() < waterLevel) {
				flowLeft = true;
			}
			if (x+1 != m_currentWorld.size() && m_currentWorld[x + 1][y].isOpen() && m_currentWorld[x + 1][y].getWaterLevel() < waterLevel) {
				flowRight = true;
			}

			// If more water than max, flow upwards
		}
	}

	// Copy into currentWorld
	m_currentWorld = m_nextWorld;
}
