#pragma once
#include <map>
#include <vector>
#include <iostream>
#include "Entity.h"

class Entity;

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager
{
private:
	EntityVec m_entites;
	EntityMap m_entityMap;
	EntityVec m_toAdd;
	size_t m_totalEntites = 0;

public:

	EntityManager();
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	void update();
	EntityVec& getEntites();
	EntityVec& getEntites(const std::string name);
	void removeDeadEntites(EntityVec& vec);
};
