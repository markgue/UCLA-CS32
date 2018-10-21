#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include <cstdlib>
using namespace std;

const bool PART2 = true;

GameWorld* createStudentWorld(string);

void die(string msg)
{
	cout << "***FAILED***\n" << msg << endl;
	cout << "****** FAILED the sanity check! ******" << endl;
	exit(1);
}

void part2Checks(GameWorld* gw, GraphObject* player);

set<GraphObject*>& depth0 = GraphObject::getGraphObjects(0);
set<GraphObject*>& depth1 = GraphObject::getGraphObjects(1);
set<GraphObject*>& depth3 = GraphObject::getGraphObjects(3);

int main()
{
	GameWorld* gw = createStudentWorld("dummyAssets");

	  // **********************
	  // init
	cout << "Calling init for the StudentWorld..." << flush;
	if (gw->init() != GWSTATUS_CONTINUE_GAME)
		die("StudentWorld::init did not return GWSTATUS_CONTINUE_GAME");

	  // The only GraphObjects at depth 3 are supposed to be stars
	  // Check that init produced 30 good stars and save their
	  // inital positions in starMap
	if (depth3.size() != 30)
		die("StudentWorld::init did not create 30 stars at depth 3");
	using StarMap = map<GraphObject*, pair<double,double>>;
	StarMap starMap;
	for (auto go : depth3)
	{
		if (go->getSize() < .05  ||  go->getSize() > .50)
			die("A star has the wrong size");
		if (go->getX() < 0  ||  go->getX() >= VIEW_WIDTH  ||
		    go->getY() < 0  ||  go->getY() >= VIEW_HEIGHT)
			die("A star was created at bad coordinates");
		starMap[go] = make_pair(go->getX(), go->getY());
	}

	  // The only GraphObject at depth 0 must be the NachenBlaster
	if (depth0.size() != 1)
		die("StudentWorld::init did not create exactly one Nachenblaster at depth 0");
	GraphObject* player = *(depth0.begin());
	if (player->getX() != 0  ||  player->getY() != 128)
		die("NachenBlaster created in wrong place");
	if (player->getSize() != 1)
		die("NachenBlaster has the wrong size");

	  // There should be nothing at depth 1
	if (!depth1.empty())
		die("Something was created at depth 1");

	cout << "OK" << endl;

	  // **********************
	  // Move 1.  Act as if a right arrow key was pressed.
	cout << "Calling move for the StudentWorld with simulated right arrow key press..." << flush;
	gw->setKey(KEY_PRESS_RIGHT);
    if (gw->move() != GWSTATUS_CONTINUE_GAME)
		die("StudentWorld::move did not return GWSTATUS_CONTINUE_GAME");

      // 30 stars must have moved left by 1.  Any that moved off screen might
	  // be gone.  New stars might have been created.
	for (auto go : depth3)
	{
		StarMap::iterator p = starMap.find(go);
		if (p == starMap.end())  //  newly-created star
		{
			if (go->getSize() < .05  ||  go->getSize() > .50)
				die("A newly-created star has the wrong size");
			if (go->getX() != VIEW_WIDTH-1  ||
		    	go->getY() < 0  ||  go->getY() >= VIEW_HEIGHT)
				die("A newly-created star was created at wrong coordinates");
		}
		else
		{
			int xold = p->second.first;
			int yold = p->second.second;
			if (go->getX() != xold-1  ||  go->getY() != yold)
				die("A star moved incorrectly");
			starMap.erase(p);
		}
	}
	  // starMap may still have stars that moved off screen
	for (auto& pr : starMap)
	{
		if (pr.second.first > 0)  // x > 0 should not have moved off screen
			die("A star with x coordinate > 0 was destroyed");
	}

	  // The player must have moved up.
	for (auto go : depth0)
	{
		if (go != player)
			die("Something other than the original NachenBlaster was created at depth 0");
		if (player->getX() != 6  ||  player->getY() != 128)
			die("NachenBlaster did not move right six pixels");
	}

	  // Part 2 has many more checks
	if (PART2)
		part2Checks(gw, player);
	else
		cout << "OK" << endl;

	  // **********************
	  // Cleanup
	cout << "About to call cleanup for the StudentWorld" << endl;
    gw->cleanUp();
	for (int depth = 3; depth >= 0; depth--)
	{
		if (!GraphObject::getGraphObjects(depth).empty())
		{
			string d = to_string(depth);
			die("Memory leak:  Not all GraphObjects at depth " + d + " were destroyed");
		}
	}

	  // **********************
	  // Destroy StudentWorld object
	cout << "About to destroy StudentWorld object" << endl;
	delete gw;

	cout << "****** SUCCESS! Passed the sanity check. ******" << endl;
}

void part2Checks(GameWorld* gw, GraphObject* player)
{
	  // An alien must have been added at depth 1.  It might have moved.
	if (depth1.size() != 1)
		die("Exactly one object at depth 1 (an alien) was not created");
	
	GraphObject* alien1 = *(depth1.begin());
	double alien1X = alien1->getX();
	if (alien1X > VIEW_WIDTH-1  ||  alien1X < VIEW_WIDTH-3  ||
		alien1->getY() < 0  ||  alien1->getY() >= VIEW_HEIGHT)
		die("Alien created in wrong place");
	if (alien1->getSize() != 1.5)
		die("Newly-created alien has the wrong size");

	cout << "OK" << endl;

	  // **********************
	  // Move 2.  Act as if a space was pressed.
	cout << "Calling move for the StudentWorld with simulated space key press..." << flush;
	gw->setKey(' ');
    if (gw->move() != GWSTATUS_CONTINUE_GAME)
		die("StudentWorld::move did not return GWSTATUS_CONTINUE_GAME");

	  // A second alien and a cabbage must have been created at depth 1.
	  // Either may have moved.
	GraphObject* alien2 = nullptr;
	GraphObject* cabbage = nullptr;

	if (depth1.size() != 3)
		die("Did not create exactly one cabbage and exactly one new alien");

	bool alien1StillExists = false;
	for (auto go : depth1)
	{
		if (go == alien1)
		{
			alien1StillExists = true;
			continue;
		}
		if (go->getX() < VIEW_WIDTH/2 )  // cabbage in left half
			cabbage = go;
		else
			alien2 = go;
	}
	if (alien2 == nullptr  ||  alien2->getX() > VIEW_WIDTH-1  ||
		alien2->getX() < VIEW_WIDTH-3  ||
		alien2->getY() < 0  ||  alien2->getY() >= VIEW_HEIGHT)
		die("A second alien in the proper place was not created");
	if (cabbage == nullptr)
		die("A cabbage was not created");
	double cabbageX = cabbage->getX();
	if ((cabbageX != 6+12  && cabbageX != 6+12+8)  ||
		cabbage->getY() < 0  ||  cabbage->getY() >= VIEW_HEIGHT)
		die("A cabbage in the proper place was not created");
	double cabbageDir = cabbage->getDirection();
	if (cabbageDir != 0  &&  cabbageDir != 20)
		die("The new cabbage was not created with the proper direction");
	if (!alien1StillExists)
		die("The alien created in the first move call has disappeared");

	if (alien1->getX() < alien1X - 2  ||  alien1->getX() > alien1X - 1) 
		die("The alien created in the first move call did not move to the proper place");
	alien1X = alien1->getX();

	cout << "OK" << endl;

	  // **********************
	  // Move 3.
	cout << "Calling move for the StudentWorld..." << flush;
    if (gw->move() != GWSTATUS_CONTINUE_GAME)
		die("StudentWorld::move did not return GWSTATUS_CONTINUE_GAME");

	if (alien1->getX() < alien1X - 2  ||  alien1->getX() > alien1X - 1) 
		die("The alien created in the first move call did not move to the proper place");

	if (cabbage->getX() != cabbageX+8)
		die("The cabbage did not move to the proper place");
	if (cabbage->getDirection() != cabbageDir+20)
		die("The cabbage did not rotate 20 degrees counterclockwise");

	cout << "OK" << endl;

	  // Teleport alien to be within collision range of cabbage
	  // sqrt(6*6 + 10*10) < .75*(8*.5 + 8*1.5)
	cout << "Teleporting alien to collision range of cabbage" << endl;
	double newY = cabbage->getY()-4;
	if (newY <= 0)
		newY += 4+4;
	alien1->moveTo(cabbage->getX()+10, newY);

	  // **********************
	  // Move 4.  The cabbage should be gone.
	cout << "Calling move for the StudentWorld..." << flush;
    if (gw->move() != GWSTATUS_CONTINUE_GAME)
		die("StudentWorld::move did not return GWSTATUS_CONTINUE_GAME");

	alien1StillExists = false;
	for (auto go : depth1)
	{
		if (go == alien1)
			alien1StillExists = true;
		if (go == cabbage)
			die("The collision did not destroy the cabbage");
	}
	if (!alien1StillExists)
		die("The collision mistakenly destroyed the alien");

	cout << "OK" << endl;

	  // **********************
	  // Call move 800 times with no player action.
	cout << "Calling move for the StudentWorld 800 times with no player action..." << flush;
	for (int k = 0; k < 800  &&  gw->move() == GWSTATUS_CONTINUE_GAME; k++)
		;

	cout << "didn't crash" << endl;
}














