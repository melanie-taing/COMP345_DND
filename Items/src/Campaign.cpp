//! @file campaign.cpp 
//! @brief Implementation file for the Character class  
//!
#include <queue>

#include "Campaign.h"
#include "FileMapBuilder.h"
#include "SavedMapBuilder.h"
#include "StatisticsHelper.h"
#include "Door.h"
#include "MapBuildDirector.h"

//! Default constructor
//! @parama start start map of the campaign
//! @param setup whether or not to setup the campaign.
//! @id id of the campaign
Campaign::Campaign(Map* start, bool setup, int id)
{
	this->begin = start;
	this->id = id;

	if (setup)
	{
		this->setupCampaign();
	}
}

//! setupCampaign function
//! @brief begins linking doors.
void Campaign::setupCampaign()
{
	this->setup(begin);
}

//! setup function
//! recursive sets up the doors on each map.
void Campaign::setup(Map* map)
{
	std::vector<pos> doors = map->getDoors();

	for (std::vector<pos>::iterator doorpos = doors.begin(); doorpos != doors.end(); ++doorpos)
	{
		Door* door = static_cast<Door*>(map->getObject(doorpos->x, doorpos->y));
		if (door->getStart()) continue; // skip start doors

		cout << "Door Detected at (" << doorpos->x << ", " << doorpos->y << ")" << "on map #" << map->getID() << endl;
		cout << "Pick a map to link to this door below " << Statistics::getInstance()->getNumMaps() << ". Enter 0 to make the door a finish line.";
		int mapid;

		do
		{
			cin >> mapid;

		} while (mapid > Statistics::getInstance()->getNumMaps() || mapid < 0);

		if (mapid == 0) continue;
		else
		{
			if (map->getPlayer() != nullptr)
			{
				FileMapBuilder builder(map->getPlayer());
				builder.loadMap(mapid);
				Map* map = builder.getMap();
				door->setDestination(map);
				setup(map);
			}
			else
			{
				SavedMapBuilder builder;
				builder.loadMap(mapid);
				Map* map = builder.getMap();
				door->setDestination(map);
				setup(map);
			}
		}
	}
}

//! creatCampagin function
//! creates a new campaign.
Campaign* Campaign::createCampaign()
{
	cout << "Pick a Starting map ID " << Statistics::getInstance()->getNumMaps() << " or under and above 0; ";
	int start;

	do
	{
		cin >> start;

	} while (start > Statistics::getInstance()->getNumMaps() || start <= 0);

	//Load the map that the campaign will start on.
	SavedMapBuilder builder;
	builder.loadMap(start);

	Map* startMap = builder.getMap();
	
	return new Campaign(startMap);
}

//! loadCampaign function
//! loads campaign by id
//! @param id of the camapaign
//! @player player that will play the campaign
Campaign* Campaign::loadCampaign(int id, Character* player)
{
	CMarkup xml;
	char di[20];
	sprintf_s(di, 20, "campaign/%d.xml", id);
	Map* start;
	
	if (xml.Load(string(di)))
	{
		if (!xml.FindElem("Campaign")) return nullptr; //Not a campaign file.

		xml.IntoElem();

		xml.FindElem("start");
		
		queue<Door*> doors;
		queue<Map*> maps;

		MapBuildDirector director;
		if (player != nullptr)
		{
			director.setMapBuilder(new FileMapBuilder(player));
			//FileMapBuilder builder(player);
		}
		else
		{
			director.setMapBuilder(new SavedMapBuilder());
		}

		int startID = atoi(xml.GetData().c_str());
		director.constructMap(startID);
		start = director.getMap();

		maps.push(start);

		while (xml.FindElem("map"))
		{
			xml.IntoElem(); //into map.
			while (xml.FindElem())
			{
				xml.SavePos("map");
				xml.FindElem("ID");
				int mapid = atoi(xml.GetData().c_str());
				xml.RestorePos("map");

				if (mapid != maps.front()->getID())
				{
					break; // something wrong.
				}

				xml.FindElem("doors");
				xml.IntoElem();
				while (xml.FindElem())
				{
					xml.IntoElem();
					int x, y;

					xml.SavePos("door");
					xml.FindElem("x");
					x = atoi(xml.GetData().c_str());

					xml.RestorePos("door");
					xml.FindElem("y");
					y = atoi(xml.GetData().c_str());

					if (maps.front()->getObject(x, y)->getObjectType() != OBJ_DOOR)
					{
						break; //This tile isn't a door.
					}
					xml.RestorePos("door");
					xml.FindElem("doorid");
					int doorid = atoi(xml.GetData().c_str());

					Door* door = static_cast<Door*>(maps.front()->getObject(x, y));
					director.constructMap(doorid);
					Map* dest = director.getMap();
					door->setDestination(dest);
					xml.OutOfElem(); //out of door.
				}
				xml.OutOfElem(); // out of doors.
				xml.OutOfElem(); // out of map.
			}
			maps.pop();
		}
		return new Campaign(start, false, id);

	}
	else
	{
		cout << xml.GetError() << endl;
		return nullptr;
	}
}

//! saveCampaign function
//! Save current campagin to xml file.
void Campaign::saveCampaign()
{
	CMarkup xml;

	xml.AddElem("Campaign");
	//Map* cur = this->begin;

	xml.IntoElem();

	queue<Map*> maps;
	maps.push(this->begin);

	while (!maps.empty())
	{
		Map* cur = maps.front();
		xml.AddElem("map");
		xml.IntoElem();
		xml.AddElem("ID", cur->getID());
		xml.AddElem("Start", cur->getID());

		xml.SavePos("before");

		if (!cur->getDoors().empty())
		{
			xml.AddElem("doors");
			xml.IntoElem();
		}
		
		for (pos doorpos : cur->getDoors())
		{
			xml.AddElem("door");
			xml.IntoElem();
			Door* door = static_cast<Door*>(cur->getObject(doorpos.x, doorpos.y));
			if (!door->getStart())
			{
				if (door->getDestination() == nullptr)
				{
					xml.AddElem("doorid", 0);
				}
				else
				{
					xml.AddElem("doorid", door->getDestination()->getID());
					maps.push(door->getDestination());
				}
				xml.AddElem("x", doorpos.x);
				xml.AddElem("y", doorpos.y);
			}
			xml.OutOfElem(); // out of door.
		}
		xml.OutOfElem(); // Out Of doors
		maps.pop();
		xml.OutOfElem(); // out of map
	}

	char di[20];
	sprintf_s(di, 20, "campaign/%d.xml", id);

	if (!xml.Load(string(di)))
	{
		int num = Statistics::getInstance()->getNumCampaigns();
		Statistics::getInstance()->setNumCampaigns(num + 1);
	}
	xml.Save(string(di));
}

//! setBeginningMap function
//! @param map beginning map.
void Campaign::setBeginningMap(Map* map)
{
	this->begin = map;

	//Resetting the beginning map changes the entire campaign.
	this->setupCampaign();
}