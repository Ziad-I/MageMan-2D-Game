#pragma once
#include "Assets.h"
#include <fstream>
#include <iostream>
#include "Profiler.h"

void Assets::loadFromFile(const std::string path)
{
	PROFILE_FUNCTION();

	// read from file and add appropriate asset to map
	std::fstream fin(path);
	std::string assetType, assetName, assetPath, animTex;
	int	frameCount = 1, animSpeed = 0;

	while (fin >> assetType)
	{
		if (assetType == "Texture")
		{
			fin >> assetName >> assetPath;
			addTexture(assetName, assetPath);
		}
		else if (assetType == "Animation")
		{
			fin >> assetName >> animTex >> frameCount >> animSpeed;
			Animation temp(assetName, getTexture(animTex), frameCount, animSpeed);
			addAnimation(assetName, temp);
		}
		else if (assetType == "Sound")
		{
			//fin >> assetName >> assetPath;
			//addSoundBuffer(assetName, assetPath);
		}
		else if (assetType == "Font")
		{
			fin >> assetName >> assetPath;
			addFont(assetName, assetPath);
		}
		else
			std::cout << "Unkown asset type: " << assetType << " \n";
	}
}

void Assets::addTexture(const std::string name, const std::string path)
{
	sf::Texture tex;
	if (!tex.loadFromFile(path))
		std::cout << "Error loading: " << path << " as texture";
	else
	{
		tex.setSmooth(true);
		m_textureMap[name] = tex;
	}
}

void Assets::addAnimation(const std::string name, Animation& animation)
{
	m_animationMap[name] = animation;
}

void Assets::addFont(const std::string name, const std::string path)
{
	sf::Font font;
	if (!font.loadFromFile(path))
		std::cout << "Error loading: " << path << " as font";
	else
		m_fontMap[name] = font;
}

sf::Texture& Assets::getTexture(const std::string name)
{
	return m_textureMap[name];
}

Animation& Assets::getAnimation(const std::string name)
{
	return m_animationMap[name];
}

sf::Font& Assets::getFont(const std::string name)
{
	return m_fontMap[name];
}