#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Animation.h"

using TextureMap = std::map<std::string, sf::Texture>;
using AnimationMap = std::map<std::string, Animation>;
using FontMap = std::map<std::string, sf::Font>;
//using SoundMap = std::map<std::string, Sound>;
//using BufferMap = std::map<std::string, sf::SoundBuffer>;

class Assets
{
private:

	TextureMap m_textureMap;
	AnimationMap m_animationMap;
	FontMap m_fontMap;
	//SoundMap m_soundMap;
	//BufferMap m_bufferMap;

public:

	void loadFromFile(const std::string path);

	void addTexture(const std::string name, const std::string path);
	void addAnimation(const std::string name, Animation& animation);
	void addFont(const std::string name, const std::string path);
	sf::Texture& getTexture(const std::string name);
	Animation& getAnimation(const std::string name);

	sf::Font& getFont(const std::string name);

	//void addSoundBuffer(const std::string name, const std::string path);
	//void addSound(const std::string name, Sound& sound);
	//sf::SoundBuffer& getSoundBuffer(const std::string name);
	//Sound& getSound(const std::string name);
};