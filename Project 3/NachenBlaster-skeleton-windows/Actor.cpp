#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//////////////////////////////////////////////////
// Actor										//
//////////////////////////////////////////////////

StudentWorld* Actor::getWorld()
{
	return world_;
}

int Actor::getID()
{
	return imgID_;
}

bool Actor::isAlive()
{
	return alive_;
}

void Actor::setAlive(bool a)
{
	alive_ = a;
}

bool Actor::isOnScreen()
{
	return (getX() >= 0 && getX() < VIEW_WIDTH && getY() >= 0 && getY() < VIEW_HEIGHT);
}

bool Actor::isPastLeftBorder()
{
	return(getX() < 0);
}



//////////////////////////////////////////////////
// NachenBlaster								//
//////////////////////////////////////////////////

void NachenBlaster::doSomething()
{
	// Check if it is alive
	if (!isAlive())
		return;
	
	// Check for player input
	int ch;
	if (getWorld()->getKey(ch))
	{
		
		// Check for player inputs
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			if (getX() >= 6)
				moveTo(getX() - 6, getY());
			break;
		case KEY_PRESS_RIGHT:
			if (getX() < VIEW_WIDTH - 6)
				moveTo(getX() + 6, getY());
			break;
		case KEY_PRESS_UP:
			if (getY() < VIEW_HEIGHT - 6)
				moveTo(getX(), getY() + 6);
			break;
		case KEY_PRESS_DOWN:
			if (getY() >= 6)
				moveTo(getX(), getY() - 6);
			break;
		case KEY_PRESS_TAB:
			if (torpedoes_ > 0)
			{
				torpedoes_--;
				getWorld()->createActor(IID_TORPEDO, getX() + 12, getY(), 0);
				getWorld()->playSound(SOUND_TORPEDO);
			}
			break;
		case KEY_PRESS_SPACE:
			if (cabbage_energy_ >= 5)
			{
				cabbage_energy_ = cabbage_energy_ - 5;
				getWorld()->createActor(IID_CABBAGE, getX() + 12, getY());
				getWorld()->playSound(SOUND_PLAYER_SHOOT);
			}
			break;
		}
	}

	// Add cabbage energy if able
	
	if (cabbage_energy_ < 30)
		cabbage_energy_++;
	// Check for collisions
}


int NachenBlaster::getHealth() const
{
	return health_;
}
int NachenBlaster::getCabbageEnergy() const 
{
	return cabbage_energy_;
}
int NachenBlaster::getTorpedoes() const
{
	return torpedoes_;
}

void NachenBlaster::setHealth(int hp)
{
	if (hp > 50)
	{
		health_ = 50;
	}
	else if (hp <= 0)
	{
		setAlive(false);
		health_ = 0;
	}
	else
	{
		if (hp < health_)
			getWorld()->playSound(SOUND_BLAST);
		health_ = hp;
	}

}
void NachenBlaster::addTorpedo(int amount)
{
	torpedoes_+= amount;
}




//////////////////////////////////////////////////
// Star											//
//////////////////////////////////////////////////

void Star::doSomething()
{
	moveTo(getX() - 1, getY());
	if (isPastLeftBorder())
		setAlive(false);
}

//////////////////////////////////////////////////
// Explosion									//
//////////////////////////////////////////////////

void Explosion::doSomething()
{
	if (lifetime_ > 0)
	{
		if (lifetime_ != 4)
			setSize(getSize() * 1.5);
		lifetime_--;
	}
	else
	{
		setAlive(false);
	}
}

//////////////////////////////////////////////////
// Alien										//
//////////////////////////////////////////////////
//////////////////////////////////////////////////

Alien::Alien(double damage, unsigned int score, StudentWorld* world, int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
	: Actor(world, imageID, startX, startY, dir, size, depth)
{
	damage_ = damage * (1 + (world->getLevel() - 1) * 0.1);
	scoreValue_ = score;
	
	flightPlanLength_ = 0;
	flightDirection_ = DIR_DOWN_AND_LEFT;

	// Set base HP, projectile type, and speed
	switch (imageID)
	{
	case IID_SMALLGON:
	case IID_SMOREGON:
		hp_ = 5;
		projectileType_ = IID_TURNIP;
		travelSpeed_ = 2;
		break;
	case IID_SNAGGLEGON:
		hp_ = 10;
		projectileType_ = IID_TORPEDO;
		travelSpeed_ = 1.75;
		break;
	default:
		hp_ = 0;
	}
	// Scale HP based on level
	hp_ = hp_ * (1 + (getWorld()->getLevel() - 1) * 0.1);

}


void Alien::doSomething()
{
	// Check if the alien should be dead
	if (hp_ <= 0 || isPastLeftBorder())
	{
		setAlive(false);
		return;
	}
	// Check for collisions
	checkCollisions();
	if (!isAlive())
		return;
	// Check if path needs to change
	updatePath();
	// Attempt to attack
	if (attack())
		return;
	// Attempt to move
	moveAlien();
	// Check again for collisions
	checkCollisions();

}

void Alien::updatePath()
{
	if (!isOnScreen() || flightPlanLength_ <= 0)
	{
		if (getY() >= VIEW_HEIGHT)
		{
			flightDirection_ = DIR_DOWN_AND_LEFT;
		}
		else if (getY() < 0)
		{
			flightDirection_ = DIR_UP_AND_LEFT;
		}
		else if (flightPlanLength_ <= 0)
		{
			flightDirection_ = randInt(1, 3);
		}

		flightPlanLength_ = randInt(1, 32);
	}
}

void Alien::moveAlien()
{
	switch (flightDirection_)
	{
	case DIR_UP_AND_LEFT:
		moveTo(getX() - travelSpeed_, getY() + travelSpeed_);
		break;
	case DIR_DOWN_AND_LEFT:
		moveTo(getX() - travelSpeed_, getY() - travelSpeed_);
		break;
	case DIR_LEFT:
		moveTo(getX() - travelSpeed_, getY());
		break;
	}
	flightPlanLength_--;
}

void Alien::fireProjectile()
{
	getWorld()->createActor(projectileType_, getX(), getY() - 14);
}


void Alien::explodeAndGivePoints()
{
	attemptToDropGoodie();
	getWorld()->shipDestroyed();
	getWorld()->increaseScore(scoreValue_);
	getWorld()->createActor(IID_EXPLOSION, getX(), getY());
	getWorld()->playSound(SOUND_DEATH);
}

void Alien::setFlightPlan(double speed, int direction, int distance)
{
	travelSpeed_ = speed;
	flightDirection_ = direction;
	flightPlanLength_ = distance;
}

bool Alien::playerInSight()
{
	double nb_x = getWorld()->getNachenBlaster()->getX();
	double nb_y = getWorld()->getNachenBlaster()->getY();
	return (nb_y - getY() <= 4 && nb_y - getY() >= -4 && nb_x < getX());
}

bool Alien::checkCollisions()
{
	// returns true if the Alien dies

	// Check for collisions with the NachenBlaster
	NachenBlaster* collisionCheck = getWorld()->collisionWithNachenBlaster(this);
	if (collisionCheck != nullptr)
	{
		collisionCheck->setHealth(collisionCheck->getHealth() - damage_);
		setAlive(false);
		explodeAndGivePoints();
		return true;
	}
	
	// Check for collisions with cabbages
	Actor* cab = getWorld()->findCollision(this, IID_CABBAGE);
	if (cab != nullptr)
	{
		cab->setAlive(false);
		hp_ -= 2;
		if (hp_ <= 0)
		{
			setAlive(false);
			explodeAndGivePoints();
			return true;
		}
		getWorld()->playSound(SOUND_BLAST);
	}

	// Check for collisions with player torpedoes
	Actor* tor = getWorld()->findCollision(this, IID_TORPEDO);
	if (tor != nullptr && tor->getDirection() == 0)
	{
		tor->setAlive(false);
		hp_ -= 8;
		if (hp_ <= 0)
		{
			setAlive(false);
			explodeAndGivePoints();
			return true;
		}
		getWorld()->playSound(SOUND_BLAST);
	}
	return false;
}

//////////////////////////////////////////////////
// Smallgon										//
//////////////////////////////////////////////////

bool Smallgon::attack()
{
	if (playerInSight())
	{
		if (randInt(1, ((20 / getWorld()->getLevel()) + 5)) == 1)
		{
			getWorld()->createActor(IID_TURNIP, getX() - 14, getY());
			getWorld()->playSound(SOUND_ALIEN_SHOOT);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////
// Smoregon										//
//////////////////////////////////////////////////

bool Smoregon::attack(){
	if (playerInSight())
	{
		if (randInt(1, ((20 / getWorld()->getLevel()) + 5)) == 1)
		{
			getWorld()->createActor(IID_TURNIP, getX() - 14, getY());
			getWorld()->playSound(SOUND_ALIEN_SHOOT);
			return true;
		}
		if (randInt(1, ((20 / getWorld()->getLevel()) + 5)) == 1)
		{
			setFlightPlan(5, DIR_LEFT, VIEW_WIDTH);
			return true;
		}
	}
	return false;
}

void Smoregon::attemptToDropGoodie()
{
	if (randInt(1, 6) == 1)
	{
		if (randInt(1, 2) == 1)
		{
			getWorld()->createActor(IID_REPAIR_GOODIE, getX(), getY());
		}
		else
		{
			getWorld()->createActor(IID_TORPEDO_GOODIE, getX(), getY());
		}
	}
}

//////////////////////////////////////////////////
// Snagglegon									//
//////////////////////////////////////////////////

bool Snagglegon::attack()
{
	if (playerInSight())
	{
		if (randInt(1, ((15 / getWorld()->getLevel()) + 10)) == 1)
		{
			getWorld()->createActor(IID_TORPEDO, getX() - 14, getY(), 180);
			getWorld()->playSound(SOUND_TORPEDO);
			return true;
		}
	}
	return false;
}

void Snagglegon::updatePath()
{
	if (!isOnScreen())
	{
		if (getY() >= VIEW_HEIGHT)
		{
			setFlightPlan(1.75, DIR_DOWN_AND_LEFT, 0);
		}
		else if (getY() < 0)
		{
			setFlightPlan(1.75, DIR_UP_AND_LEFT, 0);
		}

	}
}

void Snagglegon::attemptToDropGoodie()
{
	if (randInt(1, 6) == 1)
	{
		getWorld()->createActor(IID_LIFE_GOODIE, getX(), getY());
	}
}

//////////////////////////////////////////////////
// Projectile									//
//////////////////////////////////////////////////
//////////////////////////////////////////////////


//////////////////////////////////////////////////
// Cabbage										//
//////////////////////////////////////////////////

void Cabbage::doSomething()
{
	if (getX() > VIEW_WIDTH - 1 || !isAlive())
	{
		setAlive(false);
		return;
	}
	moveTo(getX() + 8, getY());
	setDirection(getDirection() + 20);
}


//////////////////////////////////////////////////
// Turnip										//
//////////////////////////////////////////////////

void Turnip::doSomething()
{
	// Check if its alive and on screen
	if (getX() < 0 || !isAlive())
	{
		setAlive(false);
		return;
	}

	// Check for collisions
	NachenBlaster* collisionCheck = getWorld()->collisionWithNachenBlaster(this);
	if (collisionCheck != nullptr)
	{
		collisionCheck->setHealth(collisionCheck->getHealth() - 2);
		setAlive(false);
		return;
	}

	// Move
	moveTo(getX() - 8, getY());
	setDirection(getDirection() + 20);

	// Check for collisions again
	collisionCheck = getWorld()->collisionWithNachenBlaster(this);
	if (collisionCheck != nullptr)
	{
		collisionCheck->setHealth(collisionCheck->getHealth() - 2);
		setAlive(false);
		return;
	}
}


//////////////////////////////////////////////////
// FlatulenceTorpedo							//
//////////////////////////////////////////////////

void FlatulenceTorpedo::doSomething()
{
	// Set of movements if the torpedo belongs to the player
	if (playerTorpedo)
	{
		// Check if its alive and on screen
		if (getX() > VIEW_WIDTH - 1 || !isAlive())
		{
			setAlive(false);
			return;
		}

		// Move
		moveTo(getX() + 8, getY());

	}
	// Set of movements if the torpedo belongs to an alien
	else
	{
		// Check if its alive and on screen
		if (getX() < 0 || !isAlive())
		{
			setAlive(false);
			return;
		}
		
		// Check for collisions
		NachenBlaster* collisionCheck = getWorld()->collisionWithNachenBlaster(this);
		if (collisionCheck != nullptr)
		{
			collisionCheck->setHealth(collisionCheck->getHealth() - 2);
			setAlive(false);
			return;
		}

		// Move
		moveTo(getX() - 8, getY());

		collisionCheck = getWorld()->collisionWithNachenBlaster(this);
		// Check for collisions again
		if (collisionCheck != nullptr)
		{
			collisionCheck->setHealth(collisionCheck->getHealth() - 2);
			setAlive(false);
			return;
		}
	}
}


//////////////////////////////////////////////////
// Goodie										//
//////////////////////////////////////////////////
//////////////////////////////////////////////////

void Goodie::doSomething()
{
	// Check if it is alive and on screen
	if (!isOnScreen() || !isAlive())
	{
		setAlive(false);
		return;
	}

	// Check for collisions
	NachenBlaster* collisionCheck = getWorld()->collisionWithNachenBlaster(this);
	if (collisionCheck != nullptr)
	{
		getWorld()->increaseScore(100);
		setAlive(false);
		getWorld()->playSound(SOUND_GOODIE);
		powerup();
		return;
	}
	
	// Move
	moveTo(getX() - 0.75, getY() - 0.75);


	// Check again for collisions
	collisionCheck = getWorld()->collisionWithNachenBlaster(this);
	if (collisionCheck != nullptr)
	{
		getWorld()->increaseScore(100);
		setAlive(false);
		getWorld()->playSound(SOUND_GOODIE);
		powerup();
		return;
	}
}

//////////////////////////////////////////////////
// RepairGoodie									//
//////////////////////////////////////////////////

void RepairGoodie::powerup()
{
	getWorld()->getNachenBlaster()->setHealth(getWorld()->getNachenBlaster()->getHealth() + 10);
}

//////////////////////////////////////////////////
// ExtraLifeGoodie								//
//////////////////////////////////////////////////

void ExtraLifeGoodie::powerup()
{
	getWorld()->incLives();
}


//////////////////////////////////////////////////
// FlatulenceTorpedoGoodie						//
//////////////////////////////////////////////////

void FlatulenceTorpedoGoodie::powerup()
{
	getWorld()->getNachenBlaster()->addTorpedo(5);
}
