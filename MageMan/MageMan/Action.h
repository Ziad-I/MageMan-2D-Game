#pragma once
#include <string>
#include "Vec2.h"
#include <sstream>

class Action
{
private:

	std::string m_name = "NONE";
	std::string m_type = "NONE";
	Vec2 m_pos = Vec2(0, 0);
public:
	Action();
	Action(const std::string& name, const std::string& type);
	Action(const std::string& name, const std::string& type, Vec2 pos);
	std::string& name();
	std::string& type();
	Vec2& pos();

	std::string toString();
};