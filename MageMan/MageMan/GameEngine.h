#pragma once
#include "Assets.h"
#include <map>

class Scene;

using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

class GameEngine
{
protected:
	sf::RenderWindow m_window;
	Assets m_assets;
	std::string m_currentScene;
	SceneMap m_sceneMap;
	size_t simulateSpeed = 1;
	bool m_running = true;

	void init(const std::string& path);
	void update();
	void sUserInput();
	std::shared_ptr<Scene> currentScene();

public:

	GameEngine(const std::string& path);

	void changeScene(const std::string& name, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

	void run();
	void quit();

	Assets& assets();
	sf::RenderWindow& window();

	bool isRunning();
};
