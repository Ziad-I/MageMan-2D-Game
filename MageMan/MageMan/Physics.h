#pragma once
#include "Entity.h"

class Physics
{
public:

	Physics();

	Vec2 GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	Vec2 GetPrevOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};
