#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// NachenBlaster

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
			// Fire a torpedo
			break;
		case KEY_PRESS_SPACE:
			// Fire a cabbage
			break;
		}

		// Add cabbage energy if able
		if (cabbage_energy_ <= 30)
			cabbage_energy_++;

	}
	
	// Check for collisions

	


}






