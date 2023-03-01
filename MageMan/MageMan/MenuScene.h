#pragma once
#include "Scene.h"
#include "GameEngine.h"

class MenuScene : public Scene
{
protected:

	std::string m_title = "Main Menu";
	std::vector<std::string> m_menuString =
	{
	"Level 1",
	"Level 2",
	"Level 3"
	};
	std::vector<std::string> m_lvlPath =
	{
	"Levels/lvl1.txt",
	"Levels/lvl2.txt",
	"Levels/lvl3.txt"
	};

	sf::Text m_menuText;
	size_t m_selectIndex = 0;

	void init();
	void update();
	void onEnd();
	void sDoAction(Action& action);

public:

	MenuScene(GameEngine* gameEngine);
	void sRender();
};