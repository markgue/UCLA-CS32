#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

//////////////////////////////////////////////////
// Actor										//
//////////////////////////////////////////////////

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, Direction dir = 0, double size = 1.0, unsigned int depth = 0)
		: GraphObject(imageID, startX, startY, dir, size, depth) 
	{
		alive_ = true; 
		world_ = world; 
		imgID_ = imageID;
	}
	virtual ~Actor() {}

	virtual void doSomething() = 0;
	
	StudentWorld* getWorld();
	int getID();
	void setAlive(bool a);
	
	bool isAlive();
	bool isOnScreen();
	bool isPastLeftBorder();

private:
	int imgID_;
	StudentWorld* world_;
	bool alive_;

};

//////////////////////////////////////////////////
// Special Game Objects							//
//////////////////////////////////////////////////

//
// Nachenblaster
//

class  NachenBlaster : public Actor
{
public:
	NachenBlaster(StudentWorld* world) 
		: Actor(world, IID_NACHENBLASTER, 0, 128, 0, 1.0, 0),
		health_(50), cabbage_energy_(30), torpedoes_(0) {}
	
	virtual void doSomething();

	int getHealth() const;
	int getCabbageEnergy() const;
	int getTorpedoes() const;

	void setHealth(int hp);
	void addTorpedo(int amount);

private:

	int health_;
	int cabbage_energy_;
	int torpedoes_;

};

//
// Star
//

class  Star : public Actor
{
public:
	
	Star(StudentWorld* world, double startX, double startY) 
		: Actor(world, IID_STAR, startX, startY, 0, (double)(randInt(5, 50))/100, 3) {}
	
	virtual void doSomething();

};

//
// Explosion
//

class  Explosion : public Actor
{
public:
	Explosion(StudentWorld* world, double startX, double startY)
		: Actor(world, IID_EXPLOSION, startX, startY, 0, 1, 0), lifetime_(4) {}

	virtual void doSomething();

private:
	int lifetime_;
	
};

//////////////////////////////////////////////////
// Alien Subclasses								//
//////////////////////////////////////////////////


const int DIR_UP_AND_LEFT = 1;
const int DIR_DOWN_AND_LEFT = 2;
const int DIR_LEFT = 3;

class Alien : public Actor
{
public:
	Alien(double damage, unsigned int score, StudentWorld* world, int imageID, 
		  double startX, double startY, Direction dir, double size, unsigned int depth);
	virtual ~Alien() {}

	virtual void doSomething();
	virtual bool attack() = 0;
	virtual void attemptToDropGoodie() = 0;
	virtual void updatePath();
	
	void moveAlien();
	void fireProjectile();
	void explodeAndGivePoints();
	void setFlightPlan(double speed, int direction, int length);

	bool playerInSight();
	bool checkCollisions();

private:
	double hp_;
	int projectileType_;
	unsigned int scoreValue_;
	double damage_;

	int flightPlanLength_;
	int flightDirection_;
	double travelSpeed_;
};


class  Smallgon : public Alien
{
public:
	Smallgon(StudentWorld* world, double startX, double startY)
		: Alien(5, 250, world, IID_SMALLGON, startX, startY, 0, 1.5, 1) {}
	virtual bool attack();
	virtual void attemptToDropGoodie() {}
};


class  Smoregon : public Alien
{
public:
	Smoregon(StudentWorld* world, double startX, double startY)
		: Alien(5, 250, world, IID_SMOREGON, startX, startY, 0, 1.5, 1) {}
	virtual bool attack();
	virtual void attemptToDropGoodie();
};


class  Snagglegon : public Alien
{
public:
	Snagglegon(StudentWorld* world, double startX, double startY)
		: Alien(10, 1000, world, IID_SNAGGLEGON, startX, startY, 0, 1.5, 1) {}
	virtual bool attack();
	virtual void updatePath();
	virtual void attemptToDropGoodie();
};

//////////////////////////////////////////////////
// Projectile Subclasses						//
//////////////////////////////////////////////////


class Projectile : public Actor
{
public:
	Projectile(StudentWorld* world, int imageID, double startX, double startY, Direction dir)
		: Actor(world, imageID, startX, startY, dir, 0.5, 1) {}
	virtual ~Projectile() {}

	virtual void doSomething() = 0;

};


class  Cabbage : public Projectile
{
public:
	Cabbage(StudentWorld* world, double startX, double startY)
		: Projectile(world, IID_CABBAGE, startX, startY, 0) {}

	virtual void doSomething();

};


class  Turnip : public Projectile
{
public:
	Turnip(StudentWorld* world, double startX, double startY)
		: Projectile(world, IID_TURNIP, startX, startY, 0) {}

	virtual void doSomething();

};

class  FlatulenceTorpedo : public Projectile
{
public:
	FlatulenceTorpedo(bool isPlayerTorpedo, StudentWorld* world, double startX, double startY, Direction dir)
		: Projectile(world, IID_TORPEDO, startX, startY, dir), playerTorpedo(isPlayerTorpedo) {}

	virtual void doSomething();

private:
	bool playerTorpedo;
};


//////////////////////////////////////////////////
// Goodie Subclasses							//
//////////////////////////////////////////////////


class Goodie : public Actor
{
public:
	Goodie(StudentWorld* world, int imageID, double startX, double startY)
		: Actor(world, imageID, startX, startY, 0, 0.5, 1) {}
	virtual ~Goodie() {}

	virtual void doSomething();
	virtual void powerup() = 0;
};

class  RepairGoodie : public Goodie
{
public:
	RepairGoodie(StudentWorld* world, double startX, double startY)
		: Goodie(world, IID_REPAIR_GOODIE, startX, startY) {}
	virtual void powerup();
};

class  ExtraLifeGoodie : public Goodie
{
public:
	ExtraLifeGoodie(StudentWorld* world, double startX, double startY)
		: Goodie(world, IID_REPAIR_GOODIE, startX, startY) {}
	virtual void powerup();
};

class  FlatulenceTorpedoGoodie : public Goodie
{
public:
	FlatulenceTorpedoGoodie(StudentWorld* world, double startX, double startY)
		: Goodie(world, IID_TORPEDO_GOODIE, startX, startY) {}
	virtual void powerup();
};

#endif // ACTOR_H_
