// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Robot.h"

using namespace std;

Robot::Robot(const string& name, const string& sprite_path) :	Thing(name, sprite_path)
{}


Robot::~Robot()
{}


void Robot::startTask(int turns)
{
	if (turns < 1)
	{
		mTurnsToCompleteTask = 1;
		return;
	}	
		
	mTurnsToCompleteTask = turns;
}


void Robot::updateTask()
{
	if (mSelfDestruct)
	{
		mSelfDestructCallback();
		die();
	}

	mTurnsToCompleteTask--;

	if (mTurnsToCompleteTask == 0)
		mTaskCompleteCallback(this);

	mFuelCellAge++;

	if (mFuelCellAge == 200)
	{
		die();
	}
}
