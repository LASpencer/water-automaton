#pragma once

class Block {
public:
	Block();

	Block(bool open, float water);

	// Makes the block a rock, preventing water movement
	void makeRock();

	void makeOpen();

	bool isOpen() const;

	float getWaterLevel() const;

	void setWaterLevel(float water);

	void addWater(float amount);

	void flowWater(float amount, Block& other);

	static const float compression_factor;

	// Calculates max water in a block under an open block
	static float maxWater(float waterAbove);

private:
	bool m_open;
	float m_water;
};