#include "Scene.h"

Scene::Scene()
{}

Scene::Scene(GameEngine* gameEngine)
{
	m_game = gameEngine;
}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
	this->m_actionMap[inputKey] = actionName;
}

size_t Scene::width()
{
	return m_game->window().getSize().x;
}

size_t Scene::height()
{
	return m_game->window().getSize().y;
}

size_t Scene::currentFrame()
{
	return m_currentFrame;
}

bool Scene::hasEnded()
{
	return m_hasEnded;
}

ActionMap& Scene::getActionMap()
{
	return m_actionMap;
}

void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(p1.x, p1.y)),
		sf::Vertex(sf::Vector2f(p2.x, p2.y)),
	};

	m_game->window().draw(line, 2, sf::Lines);
}

void Scene::setPaused(bool pause)
{
	m_paused = pause;
}