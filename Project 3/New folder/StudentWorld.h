#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
	~StudentWorld()
	{
		cleanUp();
	}
    virtual int init();
    virtual int move();
    virtual void cleanUp();

	void createActor(int imageID, double startX, double startY, Direction dir = 0, double size = 1.0, unsigned int depth = 0);

private:
	std::list<Actor*> actors_;
	NachenBlaster* nachenblaster_;
};

#endif // STUDENTWORLD_H_
