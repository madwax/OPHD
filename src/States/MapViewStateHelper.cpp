// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/**
 * MapViewStateHelper.h / MapViewStateHelper.cpp
 * 
 * These are files that are used exclusively by the MapViewState class. They are here
 * in an effort to reduce the size/complexity of the MapViewState object as most of these
 * functions do not require access to internal parts of the MapViewState class (and if
 * they do, require use of a specific object).
 */

#include "MapViewStateHelper.h"

#include "../AiVoiceNotifier.h"
#include "../Constants.h"

#include "../Things/Structures/RobotCommand.h"
#include "../Things/Structures/Warehouse.h"


using namespace NAS2D::Xml;


extern int ROBOT_ID_COUNTER; /// \fixme Kludge


/**
 * Checks to see if a given tube connection is valid.
 * 
 * \warning		Assumes \c tile is never nullptr.
 */
bool checkTubeConnection(Tile* _tile, Direction _dir, ConnectorDir _source_connector_dir)
{
	if (_tile->mine() || !_tile->bulldozed() || !_tile->excavated() || !_tile->thingIsStructure())
		return false;

	Structure* structure = _tile->structure();

	if (_source_connector_dir == CONNECTOR_INTERSECTION)
	{
		if (_dir == DIR_EAST || _dir == DIR_WEST)
		{
			if (structure->connectorDirection() == CONNECTOR_INTERSECTION || structure->connectorDirection() == CONNECTOR_RIGHT || structure->connectorDirection() == CONNECTOR_VERTICAL)
				return true;
		}
		else // NORTH/SOUTH
		{
			if (structure->connectorDirection() == CONNECTOR_INTERSECTION || structure->connectorDirection() == CONNECTOR_LEFT || structure->connectorDirection() == CONNECTOR_VERTICAL)
				return true;
		}
	}
	else if (_source_connector_dir == CONNECTOR_RIGHT && (_dir == DIR_EAST || _dir == DIR_WEST))
	{
		if (structure->connectorDirection() == CONNECTOR_INTERSECTION || structure->connectorDirection() == CONNECTOR_RIGHT || structure->connectorDirection() == CONNECTOR_VERTICAL)
			return true;
	}
	else if (_source_connector_dir == CONNECTOR_LEFT && (_dir == DIR_NORTH || _dir == DIR_SOUTH))
	{
		if (structure->connectorDirection() == CONNECTOR_INTERSECTION || structure->connectorDirection() == CONNECTOR_LEFT || structure->connectorDirection() == CONNECTOR_VERTICAL)
			return true;
	}

	return false;
}


/**
 * Checks to see if the given tile offers the a proper connection for a Structure.
 * 
 * \warning		Assumes \c tile is never nullptr.
 */
bool checkStructurePlacement(Tile* tile, Direction dir)
{
	if (tile->mine() || !tile->bulldozed() || !tile->excavated() || !tile->thingIsStructure() || !tile->connected())
		return false;

	Structure* _structure = tile->structure();
	if (!_structure->isConnector())
		return false;

	if (dir == DIR_EAST || dir == DIR_WEST)
	{
		if (_structure->connectorDirection() == CONNECTOR_INTERSECTION || _structure->connectorDirection() == CONNECTOR_RIGHT)
			return true;
	}
	else // NORTH/SOUTH
	{
		if (_structure->connectorDirection() == CONNECTOR_INTERSECTION || _structure->connectorDirection() == CONNECTOR_LEFT)
			return true;
	}

	return false;
}


/**
 * Checks to see if a tile is a valid tile to place a tube onto.
 * 
 * \warning		Assumes \c tilemap is never nullptr.
 */
bool validTubeConnection(TileMap* tilemap, int x, int y, ConnectorDir _cd)
{
	return checkTubeConnection(tilemap->getTile(x + 1, y, tilemap->currentDepth()), DIR_EAST, _cd) ||
		checkTubeConnection(tilemap->getTile(x - 1, y, tilemap->currentDepth()), DIR_WEST, _cd) ||
		checkTubeConnection(tilemap->getTile(x, y + 1, tilemap->currentDepth()), DIR_SOUTH, _cd) ||
		checkTubeConnection(tilemap->getTile(x, y - 1, tilemap->currentDepth()), DIR_NORTH, _cd);
}


/**
 * Checks a tile to see if a valid Tube connection is available for Structure placement.
 *
 * \warning		Assumes \c tilemap is never nullptr.
 */
bool validStructurePlacement(TileMap* tilemap, int x, int y)
{
	return	checkStructurePlacement(tilemap->getTile(x, y - 1, tilemap->currentDepth()), DIR_NORTH) ||
		checkStructurePlacement(tilemap->getTile(x + 1, y, tilemap->currentDepth()), DIR_EAST) ||
		checkStructurePlacement(tilemap->getTile(x, y + 1, tilemap->currentDepth()), DIR_SOUTH) ||
		checkStructurePlacement(tilemap->getTile(x - 1, y, tilemap->currentDepth()), DIR_WEST);
}


/**
 * Indicates that the selected landing site is clear of obstructions.
 *
 * \warning		Assumes \c tile is never nullptr.
 */
bool validLanderSite(Tile* t)
{
	if (!t->empty() || (t->index() == TERRAIN_IMPASSABLE))
	{
		Utility<AiVoiceNotifier>::get().notify(AiVoiceNotifier::UNSUITABLE_LANDING_SITE);
		cout << "MapViewState::placeStructure(): Unsuitable landing site -- Impassable Terrain." << endl;
		return false;
	}

	return true;
}


/**
 * Document me!
 */
int totalStorage(StructureManager::StructureList& _sl)
{
	int storage = 0;
	for (size_t i = 0; i < _sl.size(); ++i)
	{
		if (_sl[i]->operational())
		{
			storage += _sl[i]->storage().capacity();
		}
	}

	return constants::BASE_STORAGE_CAPACITY + storage;
}


/**
 * Check landing site for obstructions such as mining beacons, things
 * and impassable terrain.
 * 
 * This is used for the SEED Lander only
 */
bool landingSiteSuitable(TileMap* tilemap, int x, int y)
{
	for (int offY = y - 1; offY <= y + 1; ++offY)
	{
		for (int offX = x - 1; offX <= x + 1; ++offX)
		{
			Tile* tile = tilemap->getTile(offX, offY);
			if (tile->index() == TERRAIN_IMPASSABLE || tile->mine() || tile->thing())
			{
				return false;
			}
		}
	}

	return true;
}


/**
 * Document me!
 */
void deleteRobotsInRCC(Robot* r, RobotCommand* rcc, RobotPool& rp, RobotTileTable& rtt, Tile* tile)
{
	if (rcc->commandedByThis(r))
	{
		std::cout << "Cannot bulldoze Robot Command Center by a Robot under its command." << endl;
		return;
	}

	const RobotList& rl = rcc->robots();

	for (auto robot : rl)
	{
		if (rtt.find(robot) != rtt.end())
		{
			robot->die();
			continue;
		}

		rp.erase(robot);
		delete robot;
	}
}


/**
 * Document me!
 */
void updateRobotControl(RobotPool& _rp, StructureManager& _sm)
{
	auto CommandCenter = _sm.structureList(Structure::CLASS_COMMAND);
	auto RobotCommand = _sm.structureList(Structure::CLASS_ROBOT_COMMAND);

	// 3 for the first command center
	uint32_t _maxRobots = 0;
	if (CommandCenter.size() > 0) { _maxRobots += 3; }
	// the 10 per robot command facility
	for (size_t s = 0; s < RobotCommand.size(); ++s)
	{
		if (RobotCommand[s]->operational()) { _maxRobots += 10; }
	}

	_rp.InitRobotCtrl(_maxRobots);
}


/** 
 * Indicates that a given StructureID is a Lander of sorts.
 */
bool structureIsLander(StructureID id)
{
	return id == SID_SEED_LANDER || id == SID_COLONIST_LANDER || id == SID_CARGO_LANDER;
}


/**
 * Determines if the structure requires a tube connection or not.
 *
 * \note	At the moment this is really just a check for comm towers
 *			as all other structures that are self contained are not
 *			placeable by the user.
 */
bool selfSustained(StructureID id)
{
	return id == SID_COMM_TOWER;
}


/** 
 * Indicates that a specified tile is out of communications range (out of range of a CC or Comm Tower).
 */
bool outOfCommRange(StructureManager& sm, Point_2d& cc_location, TileMap* tile_map, Tile* current_tile)
{
	Tile* tile = tile_map->getVisibleTile();

	if (tile->distanceTo(tile_map->getTile(cc_location.x(), cc_location.y(), 0)) <= constants::ROBOT_COM_RANGE)
		return false;

	Tile* _comm_t = nullptr;
	for (auto _tower : sm.structureList(Structure::CLASS_COMM))
	{
		if (!_tower->operational())
		{
			continue;
		}

		_comm_t = sm.tileFromStructure(_tower);
		if (_comm_t->distanceTo(current_tile) <= constants::COMM_TOWER_BASE_RANGE)
		{
			return false;
		}
	}

	return true;
}


/**
 * Gets a pointer to a Warehouse structure that has the specified
 * amount of storage available.
 * 
 * \param	_sm		Reference to a StructureManager.
 * \param	_pt		Product to store. Use value from ProductType enumerator.
 * \param	_ct		Count of products that need to be stored.
 * 
 * \return	Returns a pointer to a Warehouse structure or \c nullptr if
 *			there are no warehouses available with the required space.
 */
Warehouse* getAvailableWarehouse(StructureManager& _sm, ProductType _pt, size_t _ct)
{
	for (auto _st : _sm.structureList(Structure::CLASS_WAREHOUSE))
	{
		Warehouse* _wh = static_cast<Warehouse*>(_st);
		if (_wh->products().canStore(_pt, _ct))
		{
			return _wh;
		}
	}

	return nullptr;
}


/**
 * Gets a RobotCommand structure that has available command capacity
 * to house additional robots.
 * 
 * \note	Assumes a check for only one robot at any given time.
 * 
 * \param	_sm		Reference to a StructureManager.
 * 
 * \return	Returns a pointer to a Warehouse structure or \c nullptr if
 *			there are no warehouses available with the required space.
 */
RobotCommand* getAvailableRobotCommand(StructureManager& _sm)
{
	for (auto _st : _sm.structureList(Structure::CLASS_ROBOT_COMMAND))
	{
		RobotCommand* _rc = static_cast<RobotCommand*>(_st);
		if (_rc->operational() && _rc->commandCapacityAvailable())
		{
			return _rc;
		}
	}

	return nullptr;
}


/**
 * Attempts to move all products from a Warehouse into any remaining warehouses.
 */
void moveProducts(Warehouse* wh, StructureManager& _sm)
{
	StructureManager::StructureList& structures = _sm.structureList(Structure::CLASS_WAREHOUSE);
	for (auto structure : structures)
	{
		if (structure->operational())
		{
			Warehouse* warehouse = static_cast<Warehouse*>(structure);
			if (warehouse != wh)
			{
				transferProducts(wh, warehouse);
			}
		}
	}
}



// ==============================================================
// = CONVENIENCE FUNCTIONS FOR WRITING OUT GAME STATE INFORMATION
// ==============================================================

/** 
 * Document me!
 */
void checkRobotDeployment(XmlElement* _ti, RobotTileTable& _rm, Robot* _r, RobotType _type)
{
	_ti->attribute("id", _r->id());
	_ti->attribute("type", _type);
	_ti->attribute("age", _r->fuelCellAge());
	_ti->attribute("production", _r->turnsToCompleteTask());

	for (auto it = _rm.begin(); it != _rm.end(); ++it)
	{
		if (it->first == _r)
		{
			_ti->attribute("x", it->second->x());
			_ti->attribute("y", it->second->y());
			_ti->attribute("depth", it->second->depth());
		}
	}

}


/** 
 * Document me!
 * 
 * Convenience function
 */
void writeRobots(XmlElement* _ti, RobotPool& _rp, RobotTileTable& _rm)
{
	XmlElement* robots = new XmlElement("robots");
	robots->attribute("id_counter", ROBOT_ID_COUNTER);

	RobotPool::DiggerList& diggers = _rp.diggers();

	for (auto digger : diggers)
	{
		XmlElement* robot = new XmlElement("robot");
		checkRobotDeployment(robot, _rm, digger, ROBOT_DIGGER);
		robot->attribute("direction", digger->direction());
		robots->linkEndChild(robot);
	}

	RobotPool::DozerList& dozers = _rp.dozers();
	for (auto dozer : dozers)
	{
		XmlElement* robot = new XmlElement("robot");
		checkRobotDeployment(robot, _rm, dozer, ROBOT_DOZER);
		robots->linkEndChild(robot);
	}

	RobotPool::MinerList& miners = _rp.miners();
	for (auto miner : miners)
	{
		XmlElement* robot = new XmlElement("robot");
		checkRobotDeployment(robot, _rm, miner, ROBOT_MINER);
		robots->linkEndChild(robot);
	}

	_ti->linkEndChild(robots);
}


/** 
 * Document me!
 */
void writeResources(XmlElement* _ti, ResourcePool& _rp)
{
	XmlElement* resources = new XmlElement("resources");
	_rp.serialize(resources);
	_ti->linkEndChild(resources);
}


/** 
 * Document me!
 */
void readResources(XmlElement* _ti, ResourcePool& _rp)
{
	if (_ti)
		_rp.deserialize(_ti);
}
