//! @file FileMapBuilder.h
//! @brief Header file for the FileMapBuilder class  
//!
//! The FileMapBuilder, is a concrete builder 
//! that parses the xml data file into a Map class
//! And can be used as playable data
//! 
//! There's no additional game rules at play in these files that haven't already
//! been implements. The Map Class,the character class and the Item class
//! all check to see if the created maps are valid.
//! we allow doors to be anywhere, no known restrictions that disallow doors
//! to be in the middle of the map in the d20 rules.
//!
//! FileMapBuilder and SavedMapBuilder are similiar in that
//! they both require the map to be saved somewhere
//! therefore, it makes sense for both of them to 
//! inherit MapBuilder, since they'll have similiar fields.
//!
//! Libraries used: 
//! CMarkup, chosen due to ease of use to parse xml files
//! And it's extremely light, does not require, it's simply
//! .cpp/.h file that you add to the project(Markup.cpp + Markup.h), no .lib or .dll files.
#pragma once

#include "MapBuilder.h"

class FileMapBuilder : public MapBuilder
{
public:
	FileMapBuilder();
	FileMapBuilder(Character* player);
	~FileMapBuilder();
	virtual bool loadMap(int id);
	void createCampagin(Map* map);
private:
	Character* player;
};