#pragma once
#include <vector>

//TODO Compile as library?

class Block;

class WaterAutomaton {
public:
	WaterAutomaton();

	WaterAutomaton(std::vector<std::vector<Block>>& world);

	const std::vector<std::vector<Block>>& getCurrentWorld() const;

	std::vector<std::vector<Block>>& getNextWorld();

	void setWorld(std::vector<std::vector<Block>> world);

	void update();

	static const float flow_rate;

private:
	std::vector<std::vector<Block>> m_currentWorld;
	std::vector<std::vector<Block>> m_nextWorld;
};