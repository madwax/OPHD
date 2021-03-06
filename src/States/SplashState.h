#pragma once

#include "NAS2D/NAS2D.h"


#include "../Constants.h"


using namespace NAS2D;


class SplashState : public State
{
public:

	SplashState();
	~SplashState();

protected:

	void initialize();
	State* update();

private:
	void onKeyDown(EventHandler::KeyCode key, EventHandler::KeyModifier mod, bool repeat);
	void onMouseDown(EventHandler::MouseButton button, int x, int y);

	void onWindowResized(int width, int height);

	void skipSplash();

private:
	Image				mLogoLairworks;
	Image				mLogoNas2d;
	Image				mLogoOutpostHd;

	Point_2d			mMousePosition;

	Music				mSplash;

	Timer				mTimer;

	State*				mReturnState;
};
