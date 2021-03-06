#pragma once

#include "NAS2D/NAS2D.h"

#include "Control.h"

#include <string>

using namespace NAS2D;

class Button: public Control
{
public:
	enum Type
	{
		BUTTON_NORMAL,
		BUTTON_TOGGLE
	};

public:
	typedef NAS2D::Signals::Signal0<void> ClickCallback;

public:
	Button();
	virtual ~Button();

	void type(Type type);

	void toggle(bool toggle);
	bool toggled() const;

	void image(const std::string& path);
	bool hasImage() const;
	
	ClickCallback& click() { return mCallback; }

	virtual void update();

protected:
	virtual void onMouseDown(EventHandler::MouseButton button, int x, int y);
	virtual void onMouseUp(EventHandler::MouseButton button, int x, int y);
	virtual void onMouseMotion(int x, int y, int dX, int dY);

private:
	enum State
	{
		STATE_NORMAL,
		STATE_PRESSED
	};

private:
	virtual void draw();

private:
	State			mState;				/**< Current state of the Button. */
	Type			mType;				/**< Modifies Button behavior. */

	Image			mImage;				/**< Image to draw centered on the Button. */

	ImageList		mSkinNormal;
	ImageList		mSkinHover;
	ImageList		mSkinPressed;

	ClickCallback	mCallback;			/**< Object to notify when the Button is activated. */

	bool			mUsesImage;			/**< Internal flag indicating that the Button uses an image graphic. */
	bool			mMouseHover;		/**< Mouse is within the bounds of the Button. */
};
