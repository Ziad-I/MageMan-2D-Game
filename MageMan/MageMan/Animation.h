#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"

class Animation
{
private:

	sf::Sprite m_sprite;

	size_t m_frameCount = 1;
	size_t m_currentFrame = 0;
	size_t m_aliveCounter = 0; // number of frames the animation has be alive for
	size_t m_speed = 0;

	Vec2 m_size = { 0,0 };

	std::string m_name;

public:
	Animation();
	Animation(const std::string& name, const sf::Texture& tex);
	Animation(const std::string& name, const sf::Texture& tex, size_t frameCount, size_t speed);
	void update();
	bool hasEnded();
	std::string& getName();
	Vec2& getSize();
	sf::Sprite& getSprite();
};
