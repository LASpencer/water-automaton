#include <stdexcept>
#include "Block.h"

const float Block::compression_factor = 0.5f;

Block::Block() : m_open(true), m_water(0.f)
{
}

Block::Block(bool open, float water) : m_open(open)
{
	if (m_open) {
		m_water = water;
	} else {
		m_water = 0.f;
	}
}

void Block::makeRock()
{
	m_open = false;
	m_water = 0.f;
}

void Block::makeOpen()
{
	m_open = true;
}

bool Block::isOpen() const
{
	return m_open;
}

float Block::getWaterLevel() const
{
	return m_water;
}

void Block::setWaterLevel(float water)
{
	if (water < 0.f) {
		throw std::invalid_argument("Water level must be positive");
	}
	if (m_open) {
		m_water = water;
	}
}

void Block::addWater(float amount)
{
	if (m_open) {
		m_water += amount;
	}
}

void Block::flowWater(float amount, Block& other)
{
	if (m_open && other.isOpen()) {
		m_water -= amount;
		other.m_water += amount;
	}
}

float Block::maxWater(float waterAbove)
{
	return 1.f + (compression_factor * waterAbove);
}
