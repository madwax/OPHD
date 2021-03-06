#pragma once

#include "NAS2D/NAS2D.h"

namespace constants
{
	const int BOTTOM_UI_HEIGHT = 162;

	const int MARGIN = 6;
	const int MARGIN_TIGHT = 2;

	const int MAIN_BUTTON_SIZE = 30;
	const int MINI_MAP_BUTTON_SIZE = 20;

	const int RESOURCE_ICON_SIZE = 16;

	const int RESOURCE_BOX_WIDTH = 200;

	const int NO_SELECTION = -1;

	// ROBOT SELECTION UI SHEET ID
	const int ROBODIGGER_SHEET_ID = 1;
	const int ROBODOZER_SHEET_ID = 0;
	const int ROBOMINER_SHEET_ID = 2;

	const NAS2D::Color_4ub MINE_COLOR(255, 0, 0, 255);
	const NAS2D::Color_4ub ACTIVE_MINE_COLOR(255, 255, 0, 255);

	// Mouse pointers
	const std::string MOUSE_POINTER_NORMAL		= "ui/pointers/normal.png";
	const std::string MOUSE_POINTER_PLACE_TILE	= "ui/pointers/place_tile.png";
	const std::string MOUSE_POINTER_INSPECT		= "ui/pointers/inspect.png";
}
