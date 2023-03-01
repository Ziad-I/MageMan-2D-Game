#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Action.h"
#include "Vec2.h"
#include "GameEngine.h"
#include "EntityManager.h"

using ActionMap = std::map<int, std::string>;

class Scene
{
protected:
	GameEngine* m_game = nullptr;
	EntityManager m_manager;
	ActionMap m_actionMap;

	bool m_paused = false;
	bool m_hasEnded = false;

	size_t m_currentFrame = 0;

	virtual void onEnd() = 0;
	void setPaused(bool pause);

public:

	Scene();
	Scene(GameEngine* gameEngine);

	virtual void update() = 0;
	virtual void sDoAction(Action& action) = 0;
	virtual void sRender() = 0;

	void registerAction(int inputKey, const std::string& actionName);

	size_t width();
	size_t height();
	size_t currentFrame();

	bool hasEnded();
	ActionMap& getActionMap();
	void drawLine(const Vec2& p1, const Vec2& p2);
};