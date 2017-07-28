#pragma once
#include "Button.h"
#include "Control.h"
#include "../../Common.h"
/*!
 * \file	Slider.h
 * \brief	Header for sliding controls.
 * \author Goof
 */

using namespace std;


class Slider : public Control
{
	/*!
	 * \class	Slider
	 * \brief	Base class for all sliding controls.
	 * 
	 * Handle horizontal and vertical slide barre.
	 * The handle could go backward or forward.
	 * the handle width/height depend on the max value.
	 * the minimum width/height is the height/width of the slide bar
	 */
public:


	typedef NAS2D::Signals::Signal1<double> ValueChangedCallback; /*!< type for Callback on value changed. */

	Slider();
	~Slider();

	void position(double value);		/*!< Set the current position. */
	double position();					/*!< Get the current position. */
	void changePosition(double change);	/*!< Adds the change amount to the current position. */

	bool displayPosition() { return mDisplayPosition;}			/*!< Get the position display flag. */
	void displayPosition(bool _d) { mDisplayPosition = _d; }	/*!< Set the position display flag. */

	double length(); 			/*!< Get the max value for the slide area. */
	void length(double _lengh);	/*!< Set the max value for the slide area. */
	
	bool backward() { return mBackward; }	 	/*!< Get the backward flag. */
	void backward(bool _b) { mBackward = _b; } 	/*!< Set the backward flag. */

	void update(); 							/*!< Called to display the slider. */
	virtual void size(float w, float h); 	/*!< Set the slider size. */

	ValueChangedCallback& change() { return mCallback; } 	/*!< Give the callback to enable another control or a window to dis/connect to this event call. */

protected:
	virtual void onMouseDown(EventHandler::MouseButton button, int x, int y); 	/*!< Event raised on mouse button down. */
	virtual void onMouseUp(EventHandler::MouseButton button, int x, int y); 	/*!< Event raised on mouse button up. */
	virtual void onMouseMotion(int x, int y, int dX, int dY); 	/*!< Event raised on mouse move. */

private:
	/*!
	 * \enum 	SliderType
	 * \brief	List the types of slider that could be used
	 */
	enum SliderType
	{
		SLIDER_VERTICAL,	/*!< Vertical slider. */
		SLIDER_HORIZONTAL	/*!< Horizontal slider. */
	};
	
	double positionInternal() { return mPosition; } /*!< Get internal slider position. */
	
	/*!
	 *  \brief set internal slider position
     */
	void positionInternal(double _pos) { 
		if (_pos < 0.0)
			_pos = 0.0;
		else if (_pos > mLenght)
			_pos = mLenght;
		mPosition = _pos;
	}
	void setSkins();	/*!< Helper function that load the gui skin on the first update call. */
	void draw();		/*!< Draw the widget on screen. */
	void logic();		/*!< Compute some values before drawing the control. */
	
	ValueChangedCallback	mCallback; /*!< Callback executed when the value is changed. */

	SliderType	mSliderType;		/*!< Type of the Slider. */
	
	// mouse event related vars
	int			mMouseX;			/*!< Mouse X coordinate. Used to prevent odd behavior in the thumb. */
	int			mMouseY;			/*!< Mouse X coordinate. Used to prevent odd behavior in the thumb. */
	bool		mMouseHover;		/*!< Mouse is within the bounds of the Button. */
	bool		mMouseHoverSlide;	/*!< Mouse is within the bounds of the Button. */
	bool		mThumbPressed;		/*!< Flag to indicate if this control is pressed. */

    // Slider values
	double		mPosition;			/*!< Current value that represent the position of the slider. */
	double		mLenght;			/*!< Maximum value for the position of the slider. */
	bool		mBackward;			/*!< Does the value returned in backward mode . */

	// drawing vars
	ImageList	mSkinButton1;		/*!< Skin for button 1 (Up or Left). */
	ImageList	mSkinButton2;		/*!< Skin for button 2 (Down or Right). */
	ImageList	mSkinMiddle;		/*!< Skin for the slide area. */
	
	ImageList	mSkinSlider;		/*!< Skin for the slider. */
	bool		mDisplayPosition;	/*!< Indicate if the slider display the value on mouse over. */

	
	Rectangle_2df	mButton1;		/*!< Area on screen where the second button is displayed. (Down/Left) */
	Rectangle_2df	mButton2;		/*!< Area on screen where the first button is displayed. (Up/Right)*/
	Rectangle_2df	mSlideBar;		/*!< Area on screen where the slide area is displayed. */
	Rectangle_2df	mSlider;		/*!< Area on screen where the slider is displayed. */
};