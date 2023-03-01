#include "Animation.h"

Animation::Animation()
{}

Animation::Animation(const std::string& name, const sf::Texture& tex)
	: Animation(name, tex, 1, 0)
{}

Animation::Animation(const std::string& name, const sf::Texture& tex, size_t frameCount, size_t speed)
	: m_sprite(tex)
{
	m_name = name;
	m_frameCount = frameCount;
	m_speed = speed;
	m_currentFrame = 0;

	m_size = Vec2((float)tex.getSize().x / frameCount, (float)tex.getSize().y);
	m_sprite.setOrigin(m_size.x / 2, m_size.y / 2);
	m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
}

// update the animation to show next frame depending on speed
// animation should loop
void Animation::update()
{

	if (m_speed != 0)
	{
		m_aliveCounter++;
		m_currentFrame = (m_aliveCounter / m_speed) % m_frameCount;
		m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
	}

}

bool Animation::hasEnded()
{
	// detect when animation has ended (last frame was played) and return true
	return m_currentFrame == m_frameCount - 1;
}

std::string& Animation::getName()
{
	return m_name;
}

Vec2& Animation::getSize()
{
	return m_size;
}

sf::Sprite& Animation::getSprite()
{
	return m_sprite;
}