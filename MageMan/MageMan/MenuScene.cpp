#pragma once
#include "MenuScene.h"
#include "PlayScene.h"

MenuScene::MenuScene(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}

void MenuScene::init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Space, "PLAY");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_menuText.setCharacterSize(48);
	m_menuText.setFont(m_game->assets().getFont("OpenSans"));
}

void MenuScene::update()
{
	sRender();
}

void MenuScene::sDoAction(Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "UP")
		{
			if (m_selectIndex > 0)
				m_selectIndex--;
			else
				m_selectIndex = m_menuString.size() - 1;
		}
		else if (action.name() == "DOWN")
		{
			m_selectIndex = (m_selectIndex + 1) % m_menuString.size();
		}
		else if (action.name() == "PLAY")
		{
			m_game->changeScene("PLAY", std::make_shared<PlayScene>(m_game, m_lvlPath[m_selectIndex]), true);
		}
		else if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
}

void MenuScene::sRender()
{
	m_game->window().clear(sf::Color(100, 100, 255));

	// reset the view so menu is displayed correctly
	sf::View defaultView = m_game->window().getDefaultView();
	m_game->window().setView(defaultView);

	m_menuText.setString(m_title);
	m_menuText.setPosition(500, 200);
	m_menuText.setFillColor(sf::Color::Black);
	m_game->window().draw(m_menuText);

	for (size_t i = 0; i < m_menuString.size(); i++)
	{
		m_menuText.setString(m_menuString[i]);

		if (i == m_selectIndex)
			m_menuText.setFillColor(sf::Color::White);
		else
			m_menuText.setFillColor(sf::Color::Black);

		m_menuText.setPosition(550, 250 + 50 * (i + 1));
		m_game->window().draw(m_menuText);
	}

	m_game->window().display();
}

void MenuScene::onEnd()
{
	m_game->quit();
}