#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, Direction dir = 0, double size = 1.0, unsigned int depth = 0)
		: GraphObject(imageID, startX, startY, dir, size, depth) 
	{
		alive_ = true; 
		world_ = world; 
	}
	
	virtual void doSomething() = 0;
	
	StudentWorld* getWorld()
	{ 
		return world_; 
	}

	const bool isAlive()
	{
		return alive_;
	}

	void setAlive(bool a) 
	{
		alive_ = a;
	}
	

private:
	
	StudentWorld* world_;
	bool alive_;

};

// Subclasses of actors

class Alien : public Actor
{
public:

private:

};

class Projectile : public Actor
{
public:

private:

};

class Goodie : public Actor
{
public:

private:

};

// Special game objects
//
//

class  NachenBlaster : public Actor
{
public:
	NachenBlaster(StudentWorld* world) 
		: Actor(world, IID_NACHENBLASTER, 0, 128, 0, 1.0, 0),
		health_(50), cabbage_energy_(30) {}
	
	virtual void doSomething();

private:

	int health_;
	int cabbage_energy_;

};

class  Star : public Actor
{
public:
	
	Star(StudentWorld* world, double startX, double startY) 
		: Actor(world, IID_STAR, startX, startY, 0, (double)(randInt(5, 50))/100, 3) {}
	
	virtual void doSomething()
	{
		moveTo(getX() - 1, getY());
		if (getX() <= -1)
			setAlive(false);
	}

private:

};

class  Explosion : public Actor
{
public:

private:

};

// Subclasses of Alien
//
//

class  Smallgon : public Alien
{
public:

private:

};

class  Smoregon : public Alien
{
public:

private:

};

class  Snagglegon : public Alien
{
public:

private:

};

// Subclasses of Projectile
//
//

class  Cabbage : public Projectile
{
public:

private:

};

class  Turnip : public Projectile
{
public:

private:

};
class  FlatulenceTorpedo : public Projectile
{
public:

private:

};

// Subclasses of Goodie
//
//

class  RepairGoodie : public Goodie
{
public:

private:

};

class  ExtraLifeGoodie : public Goodie
{
public:

private:

};

class  FlatulenceTorpedoGoodie : public Goodie
{
public:

private:

};

#endif // ACTOR_H_
