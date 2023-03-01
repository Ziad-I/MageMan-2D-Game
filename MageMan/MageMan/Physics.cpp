#pragma once
#include "Physics.h"
#include "Componenets.h"

Physics::Physics()
{}

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	// AABB collision
	Vec2 overlap = Vec2();

	if (a->hasComponenet<CTransform>() && a->hasComponenet<CBoundingBox>() &&
		b->hasComponenet<CTransform>() && b->hasComponenet<CBoundingBox>())
	{
		auto& at = a->getComponent<CTransform>();
		auto& bt = b->getComponent<CTransform>();

		auto& bBoxA = a->getComponent<CBoundingBox>();
		auto& bBoxB = b->getComponent<CBoundingBox>();

		// delta(x, y) represents the difference between x and y values of two entities
		// which means distance between center of two bounding boxes of two entites
		// in the x direction and the y direction
		Vec2 delta(abs(at.pos.x - bt.pos.x), abs(at.pos.y - bt.pos.y));

		// overlap.x represents overlap in the x direction (horizontal overlap) and by how much
		// overlap.y represents overlap in the y direction (vertical overlap) and by how much
		overlap.x = (bBoxB.halfSize.x + bBoxA.halfSize.x) - delta.x;
		overlap.y = (bBoxB.halfSize.y + bBoxA.halfSize.y) - delta.y;
	}

	// if overlap is negative then that means there is no overlap :D
	return overlap;
}

Vec2 Physics::GetPrevOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	// same as AABB collision (getOverlap function) but with prev position

	Vec2 overlap = Vec2();

	if (a->hasComponenet<CTransform>() && a->hasComponenet<CBoundingBox>() &&
		b->hasComponenet<CTransform>() && b->hasComponenet<CBoundingBox>())
	{
		auto& at = a->getComponent<CTransform>();
		auto& bt = b->getComponent<CTransform>();

		auto& bBoxA = a->getComponent<CBoundingBox>();
		auto& bBoxB = b->getComponent<CBoundingBox>();

		Vec2 delta(abs(at.prevPos.x - bt.prevPos.x), abs(at.prevPos.y - bt.prevPos.y));

		overlap.x = (bBoxB.halfSize.x + bBoxA.halfSize.x) - delta.x;
		overlap.y = (bBoxB.halfSize.y + bBoxA.halfSize.y) - delta.y;
	}
	return overlap;
}