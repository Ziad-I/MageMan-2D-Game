#pragma once
#include "EntityManager.h"

EntityManager::EntityManager()
{}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto e = std::shared_ptr<Entity>(new Entity(tag, m_totalEntites++));
	m_toAdd.push_back(e);
	return e;
}

void EntityManager::update()
{
	for (auto e : m_toAdd)
	{
		m_entites.push_back(e);
		m_entityMap[e->tag()].push_back(e);
	}

	removeDeadEntites(m_entites);

	for (auto& entry : m_entityMap)
	{
		removeDeadEntites(entry.second);
	}

	m_toAdd.clear();
}

EntityVec& EntityManager::getEntites()
{
	return m_entites;
}

EntityVec& EntityManager::getEntites(const std::string name)
{
	return m_entityMap[name];
}

void EntityManager::removeDeadEntites(EntityVec& vec)
{
	auto it = std::remove_if(vec.begin(), vec.end(), Entity::isDead);
	vec.erase(it, vec.end());
}