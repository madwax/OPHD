#pragma once

#include "UI.h"

class PopulationPanel: public Control
{
public:
	PopulationPanel(): mIcons("ui/icons.png"), mPopulation(nullptr), mMorale(nullptr)
	{
		size(100, 200);
	}
	
	virtual ~PopulationPanel() {}

	void population(Population* pop) { mPopulation = pop; }
	void morale(int* m) { mMorale = m; }

	virtual void update()
	{
		Renderer& r = Utility<Renderer>::get();

		r.drawBoxFilled(rect(), 0, 0, 0);

		r.drawText(font(), string_format("Morale: %i", *mMorale), positionX() + 5, positionY() + 5, 255, 255, 255);

		r.drawSubImage(mIcons, positionX() + 5, positionY() + 20, 0, 96, 32, 32);		// Infant
		r.drawSubImage(mIcons, positionX() + 5, positionY() + 54, 32, 96, 32, 32);		// Student
		r.drawSubImage(mIcons, positionX() + 5, positionY() + 88, 64, 96, 32, 32);		// Worker
		r.drawSubImage(mIcons, positionX() + 5, positionY() + 122, 96, 96, 32, 32);		// Scientist
		r.drawSubImage(mIcons, positionX() + 5, positionY() + 156, 128, 96, 32, 32);	// Retired

		r.drawText(font(), string_format("%i", mPopulation->size(Population::ROLE_CHILD)), positionX() + 42, positionY() + 40, 255, 255, 255);
		r.drawText(font(), string_format("%i", mPopulation->size(Population::ROLE_STUDENT)), positionX() + 42, positionY() + 72, 255, 255, 255);
		r.drawText(font(), string_format("%i", mPopulation->size(Population::ROLE_WORKER)), positionX() + 42, positionY() + 104, 255, 255, 255);
		r.drawText(font(), string_format("%i", mPopulation->size(Population::ROLE_SCIENTIST)), positionX() + 42, positionY() + 135, 255, 255, 255);
		r.drawText(font(), string_format("%i", mPopulation->size(Population::ROLE_RETIRED)), positionX() + 42, positionY() + 168, 255, 255, 255);
	}

protected:

private:
	Image		mIcons;

	Population*	mPopulation;
	int*		mMorale;
};