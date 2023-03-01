#pragma once
#pragma once
#include <string>
#include <tuple>
#include "EntityManager.h"
#include "Componenets.h"

using ComponentTuple = std::tuple
<CTransform,
	CInput,
	CLifespan,
	CBoundingBox,
	CAnimation,
	CGravity,
	CState,
	CDraggable>;

class Entity
{
private:

	friend class EntityManager;

	bool m_alive = true;
	std::string m_tag = "Default";
	size_t m_id = 0;
	ComponentTuple m_components;

	// private constructor so only entity manager can use it

	Entity(const std::string& tag, const size_t id);

public:

	void destroy();
	size_t id();
	bool isAlive();
	std::string& tag();

	static bool isDead(std::shared_ptr<Entity>& e);

	template <typename T>
	bool hasComponenet()
	{
		return getComponent<T>().has;
	}

	template <typename T>
	T& getComponent()
	{
		return std::get<T>(m_components);
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template <typename T>
	void removeComponent()
	{
		getComponent<T>() = T();
	}
};
