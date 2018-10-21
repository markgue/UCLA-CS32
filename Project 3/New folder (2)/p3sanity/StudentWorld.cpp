#include "StudentWorld.h"
#include "GameConstants.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
	nachenblaster_ = nullptr;
}

int StudentWorld::init()
{
	// Initialize the NachenBlaster
	nachenblaster_ = new NachenBlaster(this);

	// Set new values
	alien_destroyedCount_ = 0;
	alien_currentOnScreen_ = 0;
	alien_maxCount_ = 4 + (int)(.5 * getLevel());
	alien_totalToNextLevel_ = 6 + (4 * getLevel());
	// Create stars
	for (int i = 0; i < 30; i++)
	{
		createActor(IID_STAR, randInt(0, VIEW_WIDTH - 1), randInt(0, VIEW_HEIGHT - 1));
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// Check if the NachenBlaster has died
	if (nachenblaster_->getHealth() <= 0)
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	// Check if the player has completed the level
	if (alien_destroyedCount_ == alien_totalToNextLevel_)
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
	// Check for player input
	nachenblaster_->doSomething();

	// Iterate through the list and delete any dead actors, otherwise cause them to move
	list<Actor*>::iterator i = actors_.begin();
	while (i != actors_.end())
	{
		// Delete dead actors
		if (!(*i)->isAlive())
		{
			// decrease the count of aliens on screen if applicable
			int alienCheck = (*i)->getID();
			if (alienCheck == IID_SMALLGON || alienCheck == IID_SMOREGON || alienCheck == IID_SNAGGLEGON)
				alien_currentOnScreen_--;
			// delete the actor
			delete (*i);
			i = actors_.erase(i);
			continue;
		}
		// Cause each remaining actor to move
		(*i)->doSomething();
		i++;
	}

	// Update the game statistics info
	setGameStatText(createGameText());

	// Introduce new objects:
	// Stars
	if (randInt(1, 15) == 1)
	{
		createActor(IID_STAR, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1));
	}
	// New alien ships
	addShip();

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	if (nachenblaster_ != nullptr)
	{
		delete nachenblaster_;
		nachenblaster_ = nullptr;
	}

	list<Actor*>::iterator i = actors_.begin();
	while (i != actors_.end())
	{
		delete (*i);
		i = actors_.erase(i);
	}
}

Actor* StudentWorld::findCollision(Actor* src, int imageIDTarget)
{
	// Iterate through the list and try to find a collision
	double srcX = src->getX();
	double srcY = src->getY();
	list<Actor*>::iterator i = actors_.begin();
	while (i != actors_.end())
	{
		if ((*i)->getID() == imageIDTarget)
		{
			double posX = (*i)->getX();
			double posY = (*i)->getY();
			double dist = sqrt((srcX - posX) * (srcX - posX) + (srcY - posY) * (srcY - posY));
			if (dist < 0.75 * (src->getRadius() + (*i)->getRadius()))
			{
				return (*i);
			}
		}
		i++;
	}
	return nullptr;
}

void StudentWorld::createActor(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth)
{
	switch (imageID)
	{
		case IID_STAR:
		{
			Actor* star = new Star(this, startX, startY);
			actors_.push_back(star);
		}
			break;

		case IID_EXPLOSION:
		{
			Actor* explosion = new Explosion(this, startX, startY);
			actors_.push_back(explosion);
		}
			break;
	
		case IID_CABBAGE:
		{
			Actor* cabbage = new Cabbage(this, startX, startY);
			actors_.push_back(cabbage);
		}
			break;
	
		case IID_TORPEDO:
		{
			// Distinguish the type of torpedo through the value of dir
			bool isPlayerTorpedo = (dir == 0);

			Actor* torpedo = new FlatulenceTorpedo(isPlayerTorpedo, this, startX, startY, dir);
			actors_.push_back(torpedo);
		}
			break;
	
		case IID_TURNIP:
		{
			Actor* turnip = new Turnip(this, startX, startY);
			actors_.push_back(turnip);
		}
			break;
	
		case IID_LIFE_GOODIE:
		{
			Actor* life = new ExtraLifeGoodie(this, startX, startY);
			actors_.push_back(life);
		}
			break;
	
		case IID_REPAIR_GOODIE:
		{
			Actor* rep = new RepairGoodie(this, startX, startY);
			actors_.push_back(rep);
		}
			break;
	
		case IID_TORPEDO_GOODIE:
		{
			Actor* torg = new FlatulenceTorpedoGoodie(this, startX, startY);
			actors_.push_back(torg);
		}
			break;
	
		case IID_SMALLGON:
		{
			Actor* smallgon = new Smallgon(this, startX, startY);
			actors_.push_back(smallgon);
		}
			break;
	
		case IID_SMOREGON:
		{
			Actor* smoregon = new Smoregon(this, startX, startY);
			actors_.push_back(smoregon);
		}
			break;
	
		case IID_SNAGGLEGON:
		{
			Actor* snagglegon = new Snagglegon(this, startX, startY);
			actors_.push_back(snagglegon);
		}
			break;
	}
}

void StudentWorld::addShip()
{
	if (alien_currentOnScreen_ < min(alien_maxCount_, alien_totalToNextLevel_ - alien_destroyedCount_))
	{
		int s1 = 60;
		int s2 = 20 + getLevel() * 5;
		int s3 = 5 + getLevel() * 10;
		int s = s1 + s2 + s3;
		int r = randInt(1, s);
		if (r <= 60)
			createActor(IID_SMALLGON, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1));
		else if (r <= 60 + s2)
			createActor(IID_SMOREGON, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1));
		else
			createActor(IID_SNAGGLEGON, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1));
		alien_currentOnScreen_++;
	}
}

void StudentWorld::shipDestroyed()
{
	alien_destroyedCount_++;
}

NachenBlaster* StudentWorld::collisionWithNachenBlaster(Actor* src)
{
	double srcX = src->getX();
	double srcY = src->getY();
	double posX = nachenblaster_->getX();
	double posY = nachenblaster_->getY();

	double dist = sqrt((srcX - posX) * (srcX - posX) + (srcY - posY) * (srcY - posY));
	if (dist < 0.75 * (src->getRadius() + nachenblaster_->getRadius()))
		return nachenblaster_;

	return nullptr;
}

NachenBlaster* StudentWorld::getNachenBlaster()
{
	return nachenblaster_;
}

std::string StudentWorld::createGameText()
{
	ostringstream text;
	text.setf(ios::fixed);
	text.precision(0);
	text << "Lives: " << getLives() << "  ";
	text << "Health: " << setw(3) << (int)(nachenblaster_->getHealth()/ 0.50) << "%  ";
	text << "Score: " << getScore() << "  ";
	text << "Level: " << getLevel() << "  ";
	text << "Cabbages: " << setw(3) << (nachenblaster_->getCabbageEnergy() / 0.30) << "%  ";
	text << "Torpedoes: " << nachenblaster_->getTorpedoes();         

	return text.str();
}