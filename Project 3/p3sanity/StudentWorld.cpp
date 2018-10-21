#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
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

	// Check if the player has completed the level

	// Check for player input
	nachenblaster_->doSomething();

	// Iterate through the list and delete any dead actors, otherwise cause them to move
	list<Actor*>::iterator i = actors_.begin();
	while (i != actors_.end())
	{
		// Delete dead actors
		if (!(*i)->isAlive())
		{
			delete (*i);
			i = actors_.erase(i);
			continue;
		}
		// Cause each remaining actor to move
		(*i)->doSomething();
		i++;
	}

	// Update the game statistics info

	// Introduce new objects:
		// Stars
	if (randInt(1, 15) == 1)
	{
		createActor(IID_STAR, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1));
	}

		// New alien ships


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

	}
		break;
	
	case IID_CABBAGE:
	{

	}
		break;
	
	case IID_TORPEDO:
	{

	}
		break;
	
	case IID_TURNIP:
	{

	}
		break;
	
	case IID_LIFE_GOODIE:
	{

	}
		break;
	
	case IID_REPAIR_GOODIE:
	{

	}
		break;
	
	case IID_TORPEDO_GOODIE:
	{

	}
		break;
	
	case IID_SMALLGON:
	{

	}
		break;
	
	case IID_SMOREGON:
	{

	}
		break;
	
	case IID_SNAGGLEGON:
	{

	}
		break;
	}
}