#include "Action.h"

Action::Action()
{}

Action::Action(const std::string& name, const std::string& type)
	:m_name(name), m_type(type)
{}

Action::Action(const std::string& name, const std::string& type, Vec2 pos)
	: m_name(name), m_type(type), m_pos(pos)
{}

std::string& Action::name()
{
	return m_name;
}

std::string& Action::type()
{
	return m_type;
}

Vec2& Action::pos()
{
	return m_pos;
}

std::string Action::toString()
{
	std::stringstream ss;
	ss << name() << " " << type();
	return std::string();
}