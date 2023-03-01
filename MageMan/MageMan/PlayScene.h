#pragma once
#include "Scene.h"

struct PlayerConfig { float X, Y, CX, CY, SPD, MAX_SPD, JMP, GRAVITY; std::string WEAP; };

class PlayScene : public Scene
{
protected:

	std::shared_ptr<Entity> m_player;
	std::string m_lvlPath;
	std::string m_lvlBackground;
	PlayerConfig m_playerConfig;

	bool m_drawTextures = true;
	bool m_drawCollision = false;
	bool m_drawGrid = false;

	Vec2 m_gridSize = { 64,64 };
	sf::Text m_gridText;

	size_t m_jumpFrame = 0;

	Vec2 m_mPos;
	sf::CircleShape m_mouseShape;

	void init(const std::string& lvlPath);
	void loadLevel(const std::string& lvlPath);

	// utility functions to help durring calculations
	Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);
	Vec2 windowToWorld(const Vec2& window);
	bool isInside(Vec2 pos, std::shared_ptr<Entity> e);

	void SpawnPlayer();
	void spawnFireball(std::shared_ptr<Entity> entity);

	void sRender();
	void sMovment();
	void sLifespan();
	void sCollision();
	void sAnimation();
	void sDoAction(Action& action);
	void sDragAndDrop();

	void onEnd();
	void setPaused(bool pause);

public:

	PlayScene(GameEngine* gameEngine, const std::string& lvlPath);
	void update();
};
