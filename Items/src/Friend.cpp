//! @file Enemy.cpp
//! @brief Implementation file for the Enemy class  
//! 
#include "Friend.h"

//! Default Constructor for Enemy class
//! sets all stats to 1.
Friend::Friend() : Friend(1, 1, 1, 1, 1, 1, 1)
{

}

//!  Constructor for enemy class.
//! @param lvl level of enemy
//! @param str Strength of enemy
//! @param dex dexterity of enemy
//! @param con constitution of enemy
//! @param intel intellgigence of enemy
//! @param wis wisdom of enemy
//! @param cha charisma of enemy
Friend::Friend(int lvl, int str, int dex, int con, int intel, int wis, int cha) :
Character(lvl, str, dex, con, intel, wis, cha), GameObject(OBJ_FRIEND)
{
}

//! randomize function
//! created a random enemy based of the player, 
//! all stats are within +/-5 the stats of the player
//! @param player player of map or campaign
Friend* Friend::randomize(Character* player)
{
	srand(time(NULL));
	//For each stat, the enemy will within a range of +/- 1  of that.
	//The lvl will be the same.
	int str = (rand() % 3 - 1) + player->getSTR();
	int dex = (rand() % 3 - 1) + player->getDEX();
	int con = (rand() % 3 - 1) + player->getCON();
	int intel = (rand() % 3 - 1) + player->getINTEL();
	int wis = (rand() % 3 - 1) + player->getWIS();
	int cha = (rand() % 3 - 1) + player->getCHA();

	if (str < 3)
	{
		str = 3;
	}
	else if (str > 18)
	{
		str = 18;
	}
	else if (dex < 3)
	{
		dex = 3;
	}
	else if (dex > 18)
	{
		dex = 18;
	}
	else if (con < 3)
	{
		con = 3;
	}
	else if (con > 18)
	{
		con = 18;
	}
	else if (intel < 3)
	{
		intel = 3;
	}
	else if (intel > 18)
	{
		intel = 18;
	}
	else if (wis < 3)
	{
		wis = 3;
	}
	else if (wis > 18)
	{
		wis = 18;
	}
	else if (cha < 3)
	{
		cha = 3;
	}
	else if (cha > 18)
	{
		cha = 18;
	}

	return new Friend(player->getLevel(), str, dex, con, intel, wis, cha);
}