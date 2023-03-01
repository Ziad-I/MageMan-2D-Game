#pragma once
#include "Vec2.h"
#include "Animation.h"

class Component
{
public:
	bool has = false;
};

class CTransform : public Component
{
public:

	Vec2 pos = { 0,0 };
	Vec2 prevPos = { 0,0 };
	Vec2 velocity = { 0,0 };
	Vec2 scale = { 1,1 };
	float angle = 0;

	CTransform() {}

	CTransform(Vec2 p)
	{
		pos = p;
	}

	CTransform(Vec2 p, Vec2 spd)
	{
		pos = p;
		velocity = spd;
	}

	CTransform(Vec2 p, Vec2 spd, Vec2 sc, float ang)
	{
		pos = p;
		velocity = spd;
		scale = sc;
		angle = ang;
	}
};

class CLifespan : public Component
{
public:
	int lifespan = 0;
	int frameCreated = 0;

	CLifespan() {}

	CLifespan(int duration, int frame)
	{
		lifespan = duration;
		frameCreated = frame;
	}
};

class CInput : public Component
{
public:

	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool jump = false;
	bool shoot = false;
	bool canJump = false;
	bool canShoot = true;

	CInput() {}
};

class CBoundingBox : public Component
{
public:
	Vec2 size = { 0,0 };
	Vec2 halfSize = { 0,0 };

	CBoundingBox() {}

	CBoundingBox(Vec2 sz)
	{
		size = sz;
		halfSize = Vec2(size.x / 2, size.y / 2);
	}
};

class CAnimation : public Component
{
public:
	Animation animation;
	bool repeat = false;

	CAnimation() {}

	CAnimation(Animation& animation, bool r = true)
		: animation(animation), repeat(r)
	{}
};

class CGravity : public Component
{
public:
	float gravity = 0;

	CGravity() {}

	CGravity(float g)
	{
		gravity = g;
	}
};

class CState : public Component
{
public:

	std::string state = "standing";

	CState() {}

	CState(const std::string& s)
	{
		state = s;
	}
};

class CDraggable : public Component
{
public:
	bool dragging = false;
	CDraggable() {}
};