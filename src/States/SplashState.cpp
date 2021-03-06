// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SplashState.h"

#include "MainMenuState.h"


const int PAUSE_TIME = 5800;

const int FADE_LENGTH = 800;

float LOGO_SCALE = 1.0f;

enum LogoState
{
	LOGO_NONE,
	LOGO_LAIRWORKS,
	LOGO_NAS2D,
	LOGO_OUTPOSTHD
};

LogoState CURRENT_STATE = LOGO_NONE;


SplashState::SplashState() :	mLogoLairworks("sys/lairworks-logo.png"),
								mLogoNas2d("sys/nas2d_logo.png"),
								mLogoOutpostHd("sys/ophd_large.png"),
								mSplash("music/splash.ogg"),
								mReturnState(this)
{}


SplashState::~SplashState()
{
	EventHandler& e = Utility<EventHandler>::get();
	e.keyDown().disconnect(this, &SplashState::onKeyDown);
	e.mouseButtonDown().disconnect(this, &SplashState::onMouseDown);
}


void SplashState::initialize()
{
	EventHandler& e = Utility<EventHandler>::get();
	e.keyDown().connect(this, &SplashState::onKeyDown);
	e.mouseButtonDown().connect(this, &SplashState::onMouseDown);
	e.windowResized().connect(this, &SplashState::onWindowResized);

	Utility<Mixer>::get().fadeInMusic(mSplash, -1, 1000);
	Utility<Renderer>::get().showSystemPointer(false);

	if (mLogoOutpostHd.width() > Utility<Renderer>::get().width())
	{
		LOGO_SCALE = Utility<Renderer>::get().width() / mLogoOutpostHd.width();
	}
}


void setNextState(LogoState& _ls)
{
	if (_ls == LOGO_NONE)
	{
		_ls = LOGO_LAIRWORKS;
		return;
	}
	if (_ls == LOGO_LAIRWORKS)
	{
		_ls = LOGO_NAS2D;
		return;
	}
	if (_ls == LOGO_NAS2D)
	{
		_ls = LOGO_OUTPOSTHD;
		return;
	}
}


void SplashState::skipSplash()
{
	Utility<Mixer>::get().fadeOutMusic(FADE_LENGTH);
	Utility<Renderer>::get().fadeOut((float)FADE_LENGTH);
	mReturnState = new MainMenuState();
}


State* SplashState::update()
{
	Renderer& r = Utility<Renderer>::get();

	if (r.isFaded() && !r.isFading() && mTimer.accumulator() > 2500)
	{
		if (mReturnState != this)
			return mReturnState;

		setNextState(CURRENT_STATE);
		r.fadeIn((float)FADE_LENGTH);
		mTimer.reset();
	}

	if (CURRENT_STATE == LOGO_OUTPOSTHD) { r.drawBoxFilled(0, 0, r.width(), r.height(), 0, 0, 0); }
	else { r.drawBoxFilled(0, 0, r.width(), r.height(), 255, 255, 255); }

	if (CURRENT_STATE == LOGO_LAIRWORKS)
	{
		r.drawImage(mLogoLairworks, static_cast<int>(r.center_x() - mLogoLairworks.width() / 2), static_cast<int>(r.center_y() - mLogoLairworks.height() / 2));
	}
	if (CURRENT_STATE == LOGO_NAS2D)
	{
		r.drawImage(mLogoNas2d, static_cast<int>(r.center_x() - mLogoNas2d.width() / 2), static_cast<int>(r.center_y() - mLogoNas2d.height() / 2));
	}
	if (CURRENT_STATE == LOGO_OUTPOSTHD)
	{
		float _x = r.center_x() - ((mLogoOutpostHd.width() * LOGO_SCALE) / 2) - (100 * LOGO_SCALE);
		float _y = r.center_y() - ((mLogoOutpostHd.height() * LOGO_SCALE) / 2);
		
		r.drawImage(mLogoOutpostHd, static_cast<int>(_x), static_cast<int>(_y), LOGO_SCALE);
	}

	
	if (r.isFading()) { return this; }

	if (CURRENT_STATE == LOGO_OUTPOSTHD)
	{
		if (mTimer.accumulator() > 11000)
		{
			skipSplash();
			return this;
		}
	}
	else if (mTimer.accumulator() > PAUSE_TIME)
	{
		r.fadeOut((float)FADE_LENGTH);
		mTimer.reset();
	}

	return mReturnState;
}


void SplashState::onKeyDown(EventHandler::KeyCode key, EventHandler::KeyModifier mod, bool repeat)
{
	if (key == EventHandler::KEY_F11)
	{
		Utility<Renderer>::get().fullscreen(!Utility<Renderer>::get().fullscreen());
		return;
	}

	skipSplash();
}


void SplashState::onMouseDown(EventHandler::MouseButton button, int x, int y)
{
	skipSplash();
}


void SplashState::onWindowResized(int width, int height)
{
	if (mLogoOutpostHd.width() > Utility<Renderer>::get().width())
	{
		LOGO_SCALE = Utility<Renderer>::get().width() / mLogoOutpostHd.width();
	}
	else
	{
		LOGO_SCALE = 1.0f;
	}
}
