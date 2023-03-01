#pragma once
#include "GameEngine.h"
#include "Assets.h"
#include "PlayScene.h"
#include "MenuScene.h"
#include <iostream>
#include "Profiler.h"

GameEngine::GameEngine(const std::string& path)
{
	init(path);
}

void GameEngine::init(const std::string& path)
{
	PROFILE_FUNCTION();

	m_window.create(sf::VideoMode(1280, 768), "Mage-Man");
	m_window.setFramerateLimit(60);
	m_assets.loadFromFile(path);

	changeScene("MENU", std::make_shared<MenuScene>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_sceneMap[m_currentScene];
}

void GameEngine::changeScene(const std::string& name, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	if (scene)
		m_sceneMap[name] = scene;
	else
	{
		if (m_sceneMap.find(name) == m_sceneMap.end())
		{
			std::cout << "Scene doesn't exist\n";
			return;
		}
	}

	if (endCurrentScene)
		m_sceneMap.erase(m_sceneMap.find(m_currentScene));
	m_currentScene = name;
}

Assets& GameEngine::assets()
{
	return m_assets;
}

bool GameEngine::isRunning()
{
	return m_running & m_window.isOpen();
}

sf::RenderWindow& GameEngine::window()
{
	return m_window;
}

void GameEngine::run()
{
	while (isRunning())
	{
		update();
	}
}

void GameEngine::update()
{
	PROFILE_FUNCTION();
	if (m_sceneMap.empty())
		return;
	sUserInput();
	currentScene()->update();
}

void GameEngine::quit()
{
	m_running = false;
}

void GameEngine::sUserInput()
{
	PROFILE_FUNCTION();

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.key.code == sf::Keyboard::X && event.type == sf::Event::KeyPressed)
		{
			std::cout << "screenshot saved\n";
			sf::Texture texture;
			texture.create(m_window.getSize().x, m_window.getSize().y);
			texture.update(m_window);
			texture.copyToImage().saveToFile("screenshot.png");
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			// if scene has no action associated with the key then skip
			auto sceneActionMap = currentScene()->getActionMap();
			if (sceneActionMap.find(event.key.code) == sceneActionMap.end())
				continue;

			std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

			Action action(sceneActionMap.at(event.key.code), actionType);
			currentScene()->sDoAction(action);
		}

		if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
		{
			auto mousePos = sf::Mouse::getPosition(m_window);
			Vec2 mpos(mousePos.x, mousePos.y);
			std::string actionType = (event.type == sf::Event::MouseButtonPressed) ? "START" : "END";

			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
			{
				Action action("LEFT_CLICK", actionType, mpos);
				currentScene()->sDoAction(action);
				break;
			}
			case sf::Mouse::Right:
			{
				Action action("RIGHT_CLICK", actionType, mpos);
				currentScene()->sDoAction(action);
				break;
			}
			case sf::Mouse::Middle:
			{
				Action action("MIDDLE_CLICK", actionType, mpos);
				currentScene()->sDoAction(action);
				break;
			}
			default:
				break;
			}
		}

		if (event.type == sf::Event::MouseMoved)
		{
			Action action("MOUSE_MOVE", "START", Vec2(event.mouseMove.x, event.mouseMove.y));
			currentScene()->sDoAction(action);
		}
	}
}