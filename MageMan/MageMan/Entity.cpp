#include "Entity.h"

Entity::Entity(const std::string& tag, size_t id)
{
	this->m_id = id;
	this->m_tag = tag;
}

std::string& Entity::tag()
{
	return m_tag;
}

void Entity::destroy()
{
	this->m_alive = false;
}

bool Entity::isAlive()
{
	return m_alive;
}

size_t Entity::id()
{
	return m_id;
}

bool Entity::isDead(std::shared_ptr<Entity>& e)
{
	return !(e->isAlive());
}