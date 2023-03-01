#pragma once
#include "PlayScene.h"
#include "MenuScene.h"
#include "Physics.h"
#include <fstream>
#include <iostream>
#include "Profiler.h"

PlayScene::PlayScene(GameEngine* gameEngine, const std::string& lvlPath)
	: Scene(gameEngine), m_lvlPath(lvlPath)
{
	init(lvlPath);
}

void PlayScene::init(const std::string& lvlPath)
{
	PROFILE_FUNCTION();
	registerAction(sf::Keyboard::P, "PAUSE");
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
	registerAction(sf::Keyboard::G, "TOGGLE_GRID");

	registerAction(sf::Keyboard::W, "JUMP");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::Space, "SHOOT");

	m_gridText.setCharacterSize(12);
	m_gridText.setFont(m_game->assets().getFont("OpenSans"));

	loadLevel(lvlPath);
}

Vec2 PlayScene::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	// (realX, realY) represent the bottom left corner of the grid (x, y)
	float realY = height() - (gridY * m_gridSize.y);
	float realX = gridX * m_gridSize.x;

	Vec2 animSize = entity->getComponent<CAnimation>().animation.getSize();
	Vec2 midPixel(realX + animSize.x / 2.0f, realY - animSize.y / 2.0f);

	return midPixel;
}

Vec2 PlayScene::windowToWorld(const Vec2& window)
{
	const auto& view = m_game->window().getView();

	// left side of screen after view moves
	float wx = view.getCenter().x - m_game->window().getSize().x / 2;

	// bottom side of screen after view moves
	float wy = view.getCenter().y - m_game->window().getSize().y / 2;;

	return Vec2(window.x + wx, window.y + wy);
}

bool PlayScene::isInside(Vec2 pos, std::shared_ptr<Entity> e)
{
	auto ePos = e->getComponent<CTransform>().pos;
	auto size = e->getComponent<CAnimation>().animation.getSize();

	// distance in x and y directions between pos and entity's center
	float dx = abs(pos.x - ePos.x);
	float dy = abs(pos.y - ePos.y);

	return (dx <= size.x / 2) && (dy <= size.y / 2);
}

void PlayScene::loadLevel(const std::string& lvlPath)
{
	PROFILE_FUNCTION();
	// reset manager
	m_manager = EntityManager();

	std::fstream fin(lvlPath);
	std::string entityType, animName;
	int gridX, gridY;

	while (fin >> entityType)
	{
		if (entityType == "Tile")
		{
			fin >> animName >> gridX >> gridY;
			auto brick = m_manager.addEntity("tile");
			brick->addComponent<CAnimation>(m_game->assets().getAnimation(animName), true);
			brick->addComponent<CBoundingBox>(brick->getComponent<CAnimation>().animation.getSize());
			Vec2 midPixel = gridToMidPixel(gridX, gridY, brick);
			brick->addComponent<CTransform>(midPixel);
			brick->addComponent<CDraggable>();
		}
		else if (entityType == "Dec")
		{
			// add decorations here
			fin >> animName >> gridX >> gridY;
			auto dec = m_manager.addEntity("dec");
			dec->addComponent<CAnimation>(m_game->assets().getAnimation(animName), true);
			Vec2 midPixel = gridToMidPixel(gridX, gridY, dec);
			dec->addComponent<CTransform>(midPixel);
			dec->addComponent<CDraggable>();
		}
		else if (entityType == "Player")
		{
			fin >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >> m_playerConfig.CY
				>> m_playerConfig.SPD >> m_playerConfig.JMP >> m_playerConfig.MAX_SPD
				>> m_playerConfig.GRAVITY >> m_playerConfig.WEAP;
			SpawnPlayer();
		}
		else if (entityType == "Background")
		{
			fin >> animName;
			m_lvlBackground = animName;
		}
	}

	//NOTE: IF YOU ARE GETTING A COMPONENT USING THE entity->getComponent<T>()
	//		THEN you must use a reference type since it returns a reference instead of a pointer
	//		otherwise it will not work
	//		ex: auto& transform = entity->getComponenet<CTransform>()
	//			    ^
}

void PlayScene::SpawnPlayer()
{
	m_player = m_manager.addEntity("player");
	m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
	Vec2 midPixel = gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player);
	m_player->addComponent<CTransform>(midPixel, Vec2(m_playerConfig.SPD, m_playerConfig.SPD));
	m_player->addComponent<CInput>();
	m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));
	m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
	m_player->addComponent<CState>("Air");
}

void PlayScene::spawnFireball(std::shared_ptr<Entity> entity)
{
	auto& eTransform = entity->getComponent<CTransform>();

	auto fireball = m_manager.addEntity("fireball");
	fireball->addComponent<CAnimation>(m_game->assets().getAnimation(m_playerConfig.WEAP), true);
	fireball->addComponent<CBoundingBox>(Vec2(32, 32));
	fireball->addComponent<CTransform>(eTransform.pos, Vec2(15 * eTransform.scale.x, 0), eTransform.scale, eTransform.angle);
	fireball->addComponent<CLifespan>(m_currentFrame, 60);
}

void PlayScene::update()
{
	PROFILE_FUNCTION();

	m_manager.update();

	//  pause functionality
	if (!m_paused)
	{
		sMovment();
		sLifespan();
		sAnimation();
		sCollision();
		sDragAndDrop();
	}
	sRender();

	m_currentFrame++;
}

void PlayScene::sRender()
{
	PROFILE_FUNCTION();

	// get size of window
	Vec2 windowSize(m_game->window().getSize().x, m_game->window().getSize().y);

	// clear window with solid color
	m_game->window().clear(sf::Color(50, 50, 150));

	auto& pPos = m_player->getComponent<CTransform>().pos;

	// set viewpoint to be centered on player
	sf::View view = m_game->window().getView();

	// check if player has passed half the screen horizontally
	float windowCenterX = std::max(windowSize.x / 2.0f, pPos.x);
	float windowCenterY = windowSize.y - view.getCenter().y;
	// if u  want to cetner the view vertically too then use the line below
	//float windowCenterY = std::min(windowSize.y / 2.0f, pPos.y);

	view.setCenter(windowCenterX, windowCenterY);
	m_game->window().setView(view);

	// draw background
	auto& background = m_game->assets().getAnimation(m_lvlBackground).getSprite();
	background.setScale(windowSize.x / background.getTexture()->getSize().x, windowSize.y / background.getTexture()->getSize().y);
	background.setPosition(windowCenterX, windowCenterY);
	m_game->window().draw(background);

	// if game is paused
	if (m_paused)
	{
		sf::Text pauseText = m_gridText;
		pauseText.setString("PAUSED");
		pauseText.setFillColor(sf::Color::Black);
		pauseText.setOutlineColor(sf::Color::White);
		pauseText.setCharacterSize(48);
		pauseText.setPosition(570, 200);
		m_game->window().draw(pauseText);
	}

	m_mouseShape.setFillColor(sf::Color::Red);
	m_mouseShape.setRadius(4);
	m_mouseShape.setOrigin(2, 2);

	// draw textures
	if (m_drawTextures)
	{
		for (auto& e : m_manager.getEntites())
		{
			auto& transform = e->getComponent<CTransform>();

			if (e->hasComponenet<CAnimation>())
			{
				auto& animation = e->getComponent<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				m_game->window().draw(animation.getSprite());
			}
		}
	}

	// draw collision boxes
	if (m_drawCollision)
	{
		for (auto& e : m_manager.getEntites())
		{
			if (e->hasComponenet<CBoundingBox>())
			{
				auto& box = e->getComponent<CBoundingBox>();
				auto& transform = e->getComponent<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
				rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
				rect.setPosition(transform.pos.x, transform.pos.y);
				rect.setFillColor(sf::Color(0, 0, 0, 0));
				rect.setOutlineColor(sf::Color(255, 255, 255, 255));
				rect.setOutlineThickness(1);
				m_game->window().draw(rect);
			}
		}
	}

	// draw grid cells
	if (m_drawGrid)
	{
		float leftX = m_game->window().getView().getCenter().x - width() / 2;
		float rightX = leftX + width() + m_gridSize.x;

		float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

		for (float x = nextGridX; x < rightX; x += m_gridSize.x)
		{
			drawLine(Vec2(x, 0), Vec2(x, height()));
		}

		for (float y = 0; y < height(); y += m_gridSize.y)
		{
			drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

			for (float x = nextGridX; x < rightX; x += m_gridSize.x)
			{
				std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
				std::string yCell = std::to_string((int)y / (int)m_gridSize.y);

				m_gridText.setString("(" + xCell + ", " + yCell + ")");
				m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);

				m_game->window().draw(m_gridText);
			}
		}
	}

	m_game->window().draw(m_mouseShape);
	m_game->window().display();
}

void PlayScene::sMovment()
{
	PROFILE_FUNCTION();

	auto& pTransform = m_player->getComponent<CTransform>();
	auto& pInput = m_player->getComponent<CInput>();
	auto& pState = m_player->getComponent<CState>();
	Vec2 previousPlayerSpd = pTransform.velocity;
	Vec2 pVelocity(0, m_player->getComponent<CTransform>().velocity.y);

	if (pInput.left)
	{
		pTransform.scale.x = -1;
		pVelocity.x = -m_playerConfig.SPD;
	}

	if (pInput.right)
	{
		pTransform.scale.x = 1;
		pVelocity.x = m_playerConfig.SPD;
	}
	if (pInput.up && pState.state != "Air" && pInput.canJump)
	{
		pVelocity.y = m_playerConfig.JMP;
	}

	pTransform.velocity = pVelocity;

	for (auto& e : m_manager.getEntites())
	{
		auto& transform = e->getComponent<CTransform>();

		if (e->hasComponenet<CGravity>())
		{
			e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
		}

		// if player is faster than max speed in any direction
		// set its speed to max speed in that direction
		if (transform.velocity.y > m_playerConfig.MAX_SPD)
			transform.velocity.y = m_playerConfig.MAX_SPD;
		if (transform.velocity.x > m_playerConfig.MAX_SPD)
			transform.velocity.x = m_playerConfig.MAX_SPD;
		if (transform.velocity.y < -m_playerConfig.MAX_SPD)
			transform.velocity.y = -m_playerConfig.MAX_SPD;
		if (transform.velocity.x < -m_playerConfig.MAX_SPD)
			transform.velocity.x = -m_playerConfig.MAX_SPD;

		transform.prevPos = transform.pos;
		transform.pos += transform.velocity;
	}

	if (pInput.shoot && pInput.canShoot)
	{
		spawnFireball(m_player);
		pInput.canShoot = false;
	}
}

void PlayScene::sLifespan()
{
	PROFILE_FUNCTION();

	for (auto& e : m_manager.getEntites("fireball"))
	{
		if (e->hasComponenet<CLifespan>())
		{
			auto& eLife = e->getComponent<CLifespan>();

			if (m_currentFrame == eLife.frameCreated + eLife.lifespan)
			{
				e->destroy();
			}
		}
	}
}

void PlayScene::sCollision()
{
	PROFILE_FUNCTION();

	// sfml 0, 0 pos is the top left
	// so jumping will have +y and gravity will have -y
	// below has greater y and above has smaller y

	Physics physics = Physics();

	auto& pTransform = m_player->getComponent<CTransform>();
	auto& pBox = m_player->getComponent<CBoundingBox>();
	auto& pState = m_player->getComponent<CState>();

	pState.state = "Air";

	for (auto& e : m_manager.getEntites("tile"))
	{
		if (e->hasComponenet<CBoundingBox>())
		{
			// player-tile collision detection and resolution
			auto& eTransform = e->getComponent<CTransform>();
			Vec2 overlap = physics.GetOverlap(m_player, e);

			// check collision happens
			if (overlap.x > 0 && overlap.y > 0)
			{
				// if collision with end Portal then end the level
				// otherwise resolve the collision
				if (e->getComponent<CAnimation>().animation.getName() == "Portal")
				{
					onEnd();
					return;
				}
				else
				{
					Vec2 prevOverlap = physics.GetPrevOverlap(m_player, e);
					Vec2 pPos = pTransform.pos;
					Vec2 ePos = eTransform.pos;
					Vec2 posDiff = pPos - ePos;
					Vec2 shift;
					if (prevOverlap.x > 0)
					{
						// !  Since prevOverlap.x is more than 0  then collision came from y direction
						//! top or bottom collision
						if (posDiff.y > 0)
							shift.y += overlap.y;
						else if (posDiff.y < 0)
						{
							shift.y -= overlap.y;
							pState.state = "Stand";
							pTransform.pos += eTransform.velocity;
						}

						//! reset player movment in y direction
						pTransform.velocity.y = 0;
					}
					else if (prevOverlap.y > 0)
					{
						// !  Since prevOverlap.y is more than 0  then collision came from x direction
						//! left or right collision
						if (posDiff.x > 0)
							shift.x += overlap.x;
						else if (posDiff.x < 0)
							shift.x -= overlap.x;

						//! reset player movment in x direction
						pTransform.velocity.x = 0;
					}

					// ! resolve any collision that happened
					pTransform.pos += shift;
				}
			}

			// fireball-tile collision detection and resolution
			for (auto& f : m_manager.getEntites("fireball"))
			{
				Vec2 overlap = physics.GetOverlap(f, e);

				if (overlap.x > 0 && overlap.y > 0)
				{
					f->destroy();

					if (e->hasComponenet<CAnimation>())
					{
						if (e->getComponent<CAnimation>().animation.getName() == "WCrate")
						{
							e->addComponent<CAnimation>(m_game->assets().getAnimation("Boom"), false);
							e->removeComponent<CBoundingBox>();
						}
					}
				}
			}
		}
	}

	// check if player has fallen from a hole y < height
	// dont let the player wwalk of the left side of the map
	if (m_player->getComponent<CTransform>().pos.x < 0)
	{
		pTransform.pos.x += m_playerConfig.SPD;
	}
	if (m_player->getComponent<CTransform>().pos.y > height())
		SpawnPlayer();
}

void PlayScene::sAnimation()
{
	PROFILE_FUNCTION();

	auto& pState = m_player->getComponent<CState>();
	auto& pTransform = m_player->getComponent<CTransform>();

	if (pState.state == "Air")
	{
		m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Air"), true);
	}
	else if (pState.state == "Run"
		&& m_player->getComponent<CAnimation>().animation.getName() != "Run")
	{
		m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Run"), true);
	}
	else if (pState.state == "Attack"
		&& m_player->getComponent<CAnimation>().animation.getName() != "Attack")
	{
		m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Attack"));
	}
	else if (pState.state == "Stand")
	{
		auto& pInput = m_player->getComponent<CInput>();

		if ((pInput.left || pInput.right) && !(pInput.left && pInput.right))
		{
			if (m_player->getComponent<CAnimation>().animation.getName() != "Run")
				m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Run"), true);
		}
		else
			m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
	}
	// set animation of player based on CState of player
	// for each entity with animation call e->getComponenet<CAnimation>().animation.update
	//		if animation is not repeating and has ended then destroy entity

	for (auto& e : m_manager.getEntites())
	{
		if (e->hasComponenet<CAnimation>())
		{
			auto& ea = e->getComponent<CAnimation>();

			if (ea.animation.hasEnded() && !ea.repeat)
				e->destroy();
			else
				ea.animation.update();
		}
	}
}

void PlayScene::sDoAction(Action& action)
{
	PROFILE_FUNCTION();

	auto& pState = m_player->getComponent<CState>();
	auto& pInput = m_player->getComponent<CInput>();
	auto& pTransform = m_player->getComponent<CTransform>();

	if (action.type() == "START")
	{
		if (action.name() == "TOGGLE_TEXTURE")
			m_drawTextures = !m_drawTextures;
		else if (action.name() == "TOGGLE_COLLISION")
			m_drawCollision = !m_drawCollision;
		else if (action.name() == "TOGGLE_GRID")
			m_drawGrid = !m_drawGrid;
		else if (action.name() == "PAUSE")
			setPaused(!m_paused);
		else if (action.name() == "QUIT")
			onEnd();
		else if (action.name() == "LEFT")
		{
			pInput.left = true;
		}
		else if (action.name() == "RIGHT")
		{
			pInput.right = true;
		}
		else if (action.name() == "DOWN")
		{
			pInput.down = true;
		}
		else if (action.name() == "SHOOT")
		{
			pInput.shoot = true;
		}
		else if (action.name() == "JUMP")
		{
			if (pState.state != "Air" && pInput.canJump)
				pTransform.velocity.y = m_playerConfig.JMP;

			pInput.canJump = false;
			pInput.up = true;
		}
		else if (action.name() == "LEFT_CLICK")
		{
			Vec2 worldPos = windowToWorld(m_mPos);
			for (auto& e : m_manager.getEntites())
			{
				if (e->hasComponenet<CDraggable>() && isInside(worldPos, e))
				{
					std::cout << e->getComponent<CAnimation>().animation.getName() << "\n";
					e->getComponent<CDraggable>().dragging = !e->getComponent<CDraggable>().dragging;
				}
			}
		}
		//else if (action.name() == "RIGHT_CLICK")
		//{
		//}
		//else if (action.name() == "MIDDLE_CLICK")
		//{
		//}
		else if (action.name() == "MOUSE_MOVE")
		{
			m_mPos = action.pos();
			Vec2 worldMousePos = windowToWorld(m_mPos);
			m_mouseShape.setPosition(worldMousePos.x, worldMousePos.y);
		}
		// add rest of actions
	}
	else if (action.type() == "END")
	{
		if (action.name() == "LEFT")
		{
			pInput.left = false;
		}
		else if (action.name() == "RIGHT")
		{
			pInput.right = false;
		}
		else if (action.name() == "DOWN")
		{
			pInput.down = false;
		}
		else if (action.name() == "SHOOT")
		{
			pInput.shoot = false;
			pInput.canShoot = true;
		}
		else if (action.name() == "JUMP")
		{
			if (pTransform.velocity.y < 0)
				pTransform.velocity.y = 0;

			pInput.canJump = true;
			pInput.up = false;
		}
	}
}

void PlayScene::sDragAndDrop()
{
	PROFILE_FUNCTION();

	for (auto& e : m_manager.getEntites())
	{
		if (e->hasComponenet<CDraggable>() && e->getComponent<CDraggable>().dragging)
		{
			Vec2 worldPos = windowToWorld(m_mPos);
			e->getComponent<CTransform>().pos = worldPos;
		}
	}
}

void PlayScene::onEnd()
{
	// when scene ends change back to menu scene
	// m_game->changeScene(params)

	m_game->changeScene("MENU", std::make_shared<MenuScene>(m_game), true);
}

void PlayScene::setPaused(bool pause)
{
	m_paused = pause;
}