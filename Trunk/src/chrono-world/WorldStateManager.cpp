/*
 * Chrono Emulator
 * Copyright (C) 2010 ChronoEmu Team <http://www.forsakengaming.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"
initialiseSingleton(WorldStateTemplateManager);

/** World State Manager Implementation Class **/

// creates world state structure
void WorldStateManager::CreateWorldState(uint32 uWorldStateId, uint32 uInitialValue, int32 iFactionMask /* = FACTION_MASK_ALL */, int32 iZoneMask /* = ZONE_MASK_ALL */)
{
	WorldState newState;
	WorldStateMap::iterator itr;

	// lockit!
	//m_lock.Acquire();
	
	if( m_states.empty() ) return;

	// search away, for naughty people
	itr = m_states.find(uWorldStateId);
	
	// does it already exist? naughty boy.
	if( itr != m_states.end() )
	{
		itr->second.FactionMask = iFactionMask;
		itr->second.ZoneMask = iZoneMask;
		itr->second.Value = uInitialValue;
	}
	else
	{
		// set the worldstate, add it to the list.
		newState.FactionMask = iFactionMask;
		newState.ZoneMask = iZoneMask;
		newState.Value = uInitialValue;

		// add!
		m_states.insert( make_pair( uWorldStateId, newState) );
	}

	// release the lock
	//m_lock.Release();
}

// updates a world state with a new value
void WorldStateManager::UpdateWorldState(uint32 uWorldStateId, uint32 uValue)
{
	WorldStateMap::iterator itr;
	uint8 msgdata[10];
	StackPacket data(SMSG_UPDATE_WORLD_STATE, msgdata, 10);

	// lockit!
	//m_lock.Acquire();

	if( m_states.empty() ) return;
	
	// we should be existant.
	itr = m_states.find(uWorldStateId);

	// otherwise abort
	if( itr == m_states.end() )
	{
		printf("!!! WARNING: We are setting world state %u to %u on map %u, but uninitialized !!!\n", uWorldStateId, uValue, m_mapMgr.GetMapId());
		CreateWorldState(uWorldStateId, uValue);
		itr = m_states.find(uWorldStateId);
		if( itr == m_states.end() )
		{
			//m_lock.Release();
			return;
		}

		//m_lock.Release();
		//return;
	}

	// set the new value
	itr->second.Value = uValue;

	// build the new packet
	data << uWorldStateId << uValue;

	// send to the appropriate players
	m_mapMgr.SendPacketToPlayers(itr->second.ZoneMask, itr->second.FactionMask, &data);

	// release the lock
	//m_lock.Release();
}

void WorldStateManager::SendWorldStates(Player* pPlayer)
{
    uint16 NumberOfFields = 0;
    uint32 mapid = pPlayer->GetMapId();
    uint32 zoneid = pPlayer->GetZoneId();

	// Ported from Mangos till the Ascent way is re-done
    switch(zoneid)
    {
        case 0:
        case 1:
        case 4:
        case 8:
        case 10:
        case 11:
        case 12:
        case 36:
        case 38:
        case 40:
        case 41:
        case 51:
        case 267:
        case 1519:
        case 1537:
        case 2257:
        case 2918:
            NumberOfFields = 6;
            break;
        case 139:
            NumberOfFields = 39;
            break;
        case 1377:
            NumberOfFields = 13;
            break;
        case 2597:
            NumberOfFields = 81;
            break;
        case 3277:
            NumberOfFields = 14;
            break;
        case 3358:
        case 3820:
            NumberOfFields = 38;
            break;
        case 3483:
            NumberOfFields = 22;
            break;
        case 3519:
            NumberOfFields = 36;
            break;
        case 3521:
            NumberOfFields = 35;
            break;
        case 3698:
        case 3702:
        case 3968:
            NumberOfFields = 9;
            break;
        case 3703:
            NumberOfFields = 9;
            break;
        default:
            NumberOfFields = 10;
            break;
    }

    WorldPacket data(SMSG_INIT_WORLD_STATES, (4+2+(NumberOfFields*8)));
    data << uint32(mapid);                                  // mapid
    data << uint32(zoneid);                                 // zone id
    data << uint16(NumberOfFields);                         // count of uint64 blocks
    data << uint32(0x8d8) << uint32(0x0);                   // 1
    data << uint32(0x8d7) << uint32(0x0);                   // 2
    data << uint32(0x8d6) << uint32(0x0);                   // 3
    data << uint32(0x8d5) << uint32(0x0);                   // 4
    data << uint32(0x8d4) << uint32(0x0);                   // 5
    data << uint32(0x8d3) << uint32(0x0);                   // 6
    switch(zoneid)
    {
        case 1:
        case 11:
        case 12:
        case 38:
        case 40:
        case 51:
        case 1519:
        case 1537:
        case 2257:
            break;
        case 139:                                           // EPL
            {
                data << uint32(0x97a) << uint32(0x0); // 10 2426
                data << uint32(0x917) << uint32(0x0); // 11 2327
                data << uint32(0x918) << uint32(0x0); // 12 2328
                data << uint32(0x97b) << uint32(0x32); // 13 2427
                data << uint32(0x97c) << uint32(0x32); // 14 2428
                data << uint32(0x933) << uint32(0x1); // 15 2355
                data << uint32(0x946) << uint32(0x0); // 16 2374
                data << uint32(0x947) << uint32(0x0); // 17 2375
                data << uint32(0x948) << uint32(0x0); // 18 2376
                data << uint32(0x949) << uint32(0x0); // 19 2377
                data << uint32(0x94a) << uint32(0x0); // 20 2378
                data << uint32(0x94b) << uint32(0x0); // 21 2379
                data << uint32(0x932) << uint32(0x0); // 22 2354
                data << uint32(0x934) << uint32(0x0); // 23 2356
                data << uint32(0x935) << uint32(0x0); // 24 2357
                data << uint32(0x936) << uint32(0x0); // 25 2358
                data << uint32(0x937) << uint32(0x0); // 26 2359
                data << uint32(0x938) << uint32(0x0); // 27 2360
                data << uint32(0x939) << uint32(0x1); // 28 2361
                data << uint32(0x930) << uint32(0x1); // 29 2352
                data << uint32(0x93a) << uint32(0x0); // 30 2362
                data << uint32(0x93b) << uint32(0x0); // 31 2363
                data << uint32(0x93c) << uint32(0x0); // 32 2364
                data << uint32(0x93d) << uint32(0x0); // 33 2365
                data << uint32(0x944) << uint32(0x0); // 34 2372
                data << uint32(0x945) << uint32(0x0); // 35 2373
                data << uint32(0x931) << uint32(0x1); // 36 2353
                data << uint32(0x93e) << uint32(0x0); // 37 2366
                data << uint32(0x931) << uint32(0x1); // 38 2367 ??  grey horde not in dbc! send for consistency's sake, and to match field count
                data << uint32(0x940) << uint32(0x0); // 39 2368
                data << uint32(0x941) << uint32(0x0); // 7 2369
                data << uint32(0x942) << uint32(0x0); // 8 2370
                data << uint32(0x943) << uint32(0x0); // 9 2371
            }
            break;
        case 1377:                                          // Silithus
            {
                data << uint32(0x909) << uint32(0x0); // 7 ally silityst gathered
                data << uint32(0x90A) << uint32(0x0); // 8 horde silityst gathered
                data << uint32(0x90D) << uint32(0x0C8); // 9 max silithyst
                data << uint32(0x912) << uint32(0x0); // 10 sandworm N
                data << uint32(0x913) << uint32(0x0); // 11 sandworm S
                data << uint32(0x914) << uint32(0x0); // 12 sandworm SW
                data << uint32(0x915) << uint32(0x0); // 13 sandworm E
            }
            break;
        case 2597:                                          // AV
            {
                data << uint32(0x7ae) << uint32(0x1);           // 7 snowfall n
                data << uint32(0x532) << uint32(0x1);           // 8 frostwolfhut hc
                data << uint32(0x531) << uint32(0x0);           // 9 frostwolfhut ac
                data << uint32(0x52e) << uint32(0x0);           // 10 stormpike firstaid a_a
                data << uint32(0x571) << uint32(0x0);           // 11 east frostwolf tower horde assaulted -unused
                data << uint32(0x570) << uint32(0x0);           // 12 west frostwolf tower horde assaulted - unused
                data << uint32(0x567) << uint32(0x1);           // 13 frostwolfe c
                data << uint32(0x566) << uint32(0x1);           // 14 frostwolfw c
                data << uint32(0x550) << uint32(0x1);           // 15 irondeep (N) ally
                data << uint32(0x544) << uint32(0x0);           // 16 ice grave a_a
                data << uint32(0x536) << uint32(0x0);           // 17 stormpike grave h_c
                data << uint32(0x535) << uint32(0x1);           // 18 stormpike grave a_c
                data << uint32(0x518) << uint32(0x0);           // 19 stoneheart grave a_a
                data << uint32(0x517) << uint32(0x0);           // 20 stoneheart grave h_a
                data << uint32(0x574) << uint32(0x0);           // 21 1396 unk
                data << uint32(0x573) << uint32(0x0);           // 22 iceblood tower horde assaulted -unused
                data << uint32(0x572) << uint32(0x0);           // 23 towerpoint horde assaulted - unused
                data << uint32(0x56f) << uint32(0x0);           // 24 1391 unk
                data << uint32(0x56e) << uint32(0x0);           // 25 iceblood a
                data << uint32(0x56d) << uint32(0x0);           // 26 towerp a
                data << uint32(0x56c) << uint32(0x0);           // 27 frostwolfe a
                data << uint32(0x56b) << uint32(0x0);           // 28 froswolfw a
                data << uint32(0x56a) << uint32(0x1);           // 29 1386 unk
                data << uint32(0x569) << uint32(0x1);           // 30 iceblood c
                data << uint32(0x568) << uint32(0x1);           // 31 towerp c
                data << uint32(0x565) << uint32(0x0);           // 32 stoneh tower a
                data << uint32(0x564) << uint32(0x0);           // 33 icewing tower a
                data << uint32(0x563) << uint32(0x0);           // 34 dunn a
                data << uint32(0x562) << uint32(0x0);           // 35 duns a
                data << uint32(0x561) << uint32(0x0);           // 36 stoneheart bunker alliance assaulted - unused
                data << uint32(0x560) << uint32(0x0);           // 37 icewing bunker alliance assaulted - unused
                data << uint32(0x55f) << uint32(0x0);           // 38 dunbaldar south alliance assaulted - unused
                data << uint32(0x55e) << uint32(0x0);           // 39 dunbaldar north alliance assaulted - unused
                data << uint32(0x55d) << uint32(0x0);           // 40 stone tower d
                data << uint32(0x3c6) << uint32(0x0);           // 41 966 unk
                data << uint32(0x3c4) << uint32(0x0);           // 42 964 unk
                data << uint32(0x3c2) << uint32(0x0);           // 43 962 unk
                data << uint32(0x516) << uint32(0x1);           // 44 stoneheart grave a_c
                data << uint32(0x515) << uint32(0x0);           // 45 stonheart grave h_c
                data << uint32(0x3b6) << uint32(0x0);           // 46 950 unk
                data << uint32(0x55c) << uint32(0x0);           // 47 icewing tower d
                data << uint32(0x55b) << uint32(0x0);           // 48 dunn d
                data << uint32(0x55a) << uint32(0x0);           // 49 duns d
                data << uint32(0x559) << uint32(0x0);           // 50 1369 unk
                data << uint32(0x558) << uint32(0x0);           // 51 iceblood d
                data << uint32(0x557) << uint32(0x0);           // 52 towerp d
                data << uint32(0x556) << uint32(0x0);           // 53 frostwolfe d
                data << uint32(0x555) << uint32(0x0);           // 54 frostwolfw d
                data << uint32(0x554) << uint32(0x1);           // 55 stoneh tower c
                data << uint32(0x553) << uint32(0x1);           // 56 icewing tower c
                data << uint32(0x552) << uint32(0x1);           // 57 dunn c
                data << uint32(0x551) << uint32(0x1);           // 58 duns c
                data << uint32(0x54f) << uint32(0x0);           // 59 irondeep (N) horde
                data << uint32(0x54e) << uint32(0x0);           // 60 irondeep (N) ally
                data << uint32(0x54d) << uint32(0x1);           // 61 mine (S) neutral
                data << uint32(0x54c) << uint32(0x0);           // 62 mine (S) horde
                data << uint32(0x54b) << uint32(0x0);           // 63 mine (S) ally
                data << uint32(0x545) << uint32(0x0);           // 64 iceblood h_a
                data << uint32(0x543) << uint32(0x1);           // 65 iceblod h_c
                data << uint32(0x542) << uint32(0x0);           // 66 iceblood a_c
                data << uint32(0x540) << uint32(0x0);           // 67 snowfall h_a
                data << uint32(0x53f) << uint32(0x0);           // 68 snowfall a_a
                data << uint32(0x53e) << uint32(0x0);           // 69 snowfall h_c
                data << uint32(0x53d) << uint32(0x0);           // 70 snowfall a_c
                data << uint32(0x53c) << uint32(0x0);           // 71 frostwolf g h_a
                data << uint32(0x53b) << uint32(0x0);           // 72 frostwolf g a_a
                data << uint32(0x53a) << uint32(0x1);           // 73 frostwolf g h_c
                data << uint32(0x539) << uint32(0x0);           // 74 frostwolf g a_c
                data << uint32(0x538) << uint32(0x0);           // 75 stormpike grave h_a
                data << uint32(0x537) << uint32(0x0);           // 76 stormpike grave a_a
                data << uint32(0x534) << uint32(0x0);           // 77 frostwolf hut h_a
                data << uint32(0x533) << uint32(0x0);           // 78 frostwolf hut a_a
                data << uint32(0x530) << uint32(0x0);           // 79 stormpike first aid h_a
                data << uint32(0x52f) << uint32(0x0);           // 80 stormpike first aid h_c
                data << uint32(0x52d) << uint32(0x1);           // 81 stormpike first aid a_c
            }
            break;
        case 3277:                                          // WS
            {
                data << uint32(0x62d) << uint32(0x0);       // 7 1581 alliance flag captures
                data << uint32(0x62e) << uint32(0x0);       // 8 1582 horde flag captures
                data << uint32(0x609) << uint32(0x0);       // 9 1545 unk, set to 1 on alliance flag pickup...
                data << uint32(0x60a) << uint32(0x0);       // 10 1546 unk, set to 1 on horde flag pickup, after drop it's -1
                data << uint32(0x60b) << uint32(0x2);       // 11 1547 unk
                data << uint32(0x641) << uint32(0x3);       // 12 1601 unk (max flag captures?)
                data << uint32(0x922) << uint32(0x1);       // 13 2338 horde (0 - hide, 1 - flag ok, 2 - flag picked up (flashing), 3 - flag picked up (not flashing)
                data << uint32(0x923) << uint32(0x1);       // 14 2339 alliance (0 - hide, 1 - flag ok, 2 - flag picked up (flashing), 3 - flag picked up (not flashing)
            }
            break;
        case 3358:                                          // AB
            {
                data << uint32(0x6e7) << uint32(0x0);       // 7 1767 stables alliance
                data << uint32(0x6e8) << uint32(0x0);       // 8 1768 stables horde
                data << uint32(0x6e9) << uint32(0x0);       // 9 1769 unk, ST?
                data << uint32(0x6ea) << uint32(0x0);       // 10 1770 stables (show/hide)
                data << uint32(0x6ec) << uint32(0x0);       // 11 1772 farm (0 - horde controlled, 1 - alliance controlled)
                data << uint32(0x6ed) << uint32(0x0);       // 12 1773 farm (show/hide)
                data << uint32(0x6ee) << uint32(0x0);       // 13 1774 farm color
                data << uint32(0x6ef) << uint32(0x0);       // 14 1775 gold mine color, may be FM?
                data << uint32(0x6f0) << uint32(0x0);       // 15 1776 alliance resources
                data << uint32(0x6f1) << uint32(0x0);       // 16 1777 horde resources
                data << uint32(0x6f2) << uint32(0x0);       // 17 1778 horde bases
                data << uint32(0x6f3) << uint32(0x0);       // 18 1779 alliance bases
                data << uint32(0x6f4) << uint32(0x7d0);     // 19 1780 max resources (2000)
                data << uint32(0x6f6) << uint32(0x0);       // 20 1782 blacksmith color
                data << uint32(0x6f7) << uint32(0x0);       // 21 1783 blacksmith (show/hide)
                data << uint32(0x6f8) << uint32(0x0);       // 22 1784 unk, bs?
                data << uint32(0x6f9) << uint32(0x0);       // 23 1785 unk, bs?
                data << uint32(0x6fb) << uint32(0x0);       // 24 1787 gold mine (0 - horde contr, 1 - alliance contr)
                data << uint32(0x6fc) << uint32(0x0);       // 25 1788 gold mine (0 - conflict, 1 - horde)
                data << uint32(0x6fd) << uint32(0x0);       // 26 1789 gold mine (1 - show/0 - hide)
                data << uint32(0x6fe) << uint32(0x0);       // 27 1790 gold mine color
                data << uint32(0x700) << uint32(0x0);       // 28 1792 gold mine color, wtf?, may be LM?
                data << uint32(0x701) << uint32(0x0);       // 29 1793 lumber mill color (0 - conflict, 1 - horde contr)
                data << uint32(0x702) << uint32(0x0);       // 30 1794 lumber mill (show/hide)
                data << uint32(0x703) << uint32(0x0);       // 31 1795 lumber mill color color
                data << uint32(0x732) << uint32(0x1);       // 32 1842 stables (1 - uncontrolled)
                data << uint32(0x733) << uint32(0x1);       // 33 1843 gold mine (1 - uncontrolled)
                data << uint32(0x734) << uint32(0x1);       // 34 1844 lumber mill (1 - uncontrolled)
                data << uint32(0x735) << uint32(0x1);       // 35 1845 farm (1 - uncontrolled)
                data << uint32(0x736) << uint32(0x1);       // 36 1846 blacksmith (1 - uncontrolled)
                data << uint32(0x745) << uint32(0x2);       // 37 1861 unk
                data << uint32(0x7a3) << uint32(0x708);     // 38 1955 warning limit (1800)
            }
            break;
        default:
            data << uint32(0x914) << uint32(0x0);           // 7
            data << uint32(0x913) << uint32(0x0);           // 8
            data << uint32(0x912) << uint32(0x0);           // 9
            data << uint32(0x915) << uint32(0x0);           // 10
            break;
    }

	pPlayer->GetSession()->SendPacket(&data);	
}

void WorldStateManager::ClearWorldStates(Player *pPlayer)
{
	// clears the clients view of world states for this map
	uint8 msgdata[10];
	StackPacket data(SMSG_INIT_WORLD_STATES, msgdata, 10);

	// map=0
	// data1=0
	// data2=0
	// valcount=0
	data << uint32(0) << uint16(0) << uint16(0) << uint16(0);

	// send
	pPlayer->GetSession()->SendPacket(&data);
}

const string WorldStateManager::GetPersistantSetting(const char *szKeyName, const char *szDefaultValue)
{
	QueryResult * pResult = CharacterDatabase.Query("SELECT setting_value FROM worldstate_save_data WHERE setting_id = \"%s\"", 
		CharacterDatabase.EscapeString(string(szKeyName)).c_str());

	if( pResult == nullptr )
		return string(szDefaultValue);

	string ret = string(pResult->Fetch()[0].GetString());
	delete pResult;
	return ret;
}

void WorldStateManager::SetPersistantSetting(const char *szKeyName, const char *szValue)
{
	string pkey = string(szKeyName);
	string pval = string(szValue);

	pkey = CharacterDatabase.EscapeString(pkey);
	pval = CharacterDatabase.EscapeString(pval);

	CharacterDatabase.Execute("REPLACE INTO worldstate_save_data VALUES(\"%s\", \"%s\")", pkey.c_str(), pval.c_str());
}


//////////////////////////////////////////////////////////////////////////
// Template Manager
//////////////////////////////////////////////////////////////////////////

void WorldStateTemplateManager::LoadFromDB()
{
	QueryResult * pResult = WorldDatabase.Query("SELECT * FROM worldstate_template");
	if( pResult == nullptr )
		return;

	Field *fields;
	WorldStateTemplate tmpl;
	int32 mapid;
	do 
	{
		fields = pResult->Fetch();
		mapid = fields[0].GetInt32();
		tmpl.iZoneMask = fields[1].GetInt32();
		tmpl.iFactionMask = fields[2].GetInt32();
		tmpl.uField = fields[3].GetUInt32();
		tmpl.uValue = fields[4].GetUInt32();

		if( mapid == -1 )
		{
			m_general.push_back(tmpl);
		}
		else
		{
			if( mapid >= NUM_MAPS )
			{
				Log.LargeErrorMessage(LARGERRORMESSAGE_WARNING, "Worldstate template for field %u on map %u (%s) contains out of range map.", 
					tmpl.uField, mapid, fields[5].GetString());

				continue;
			}

			m_templatesForMaps[mapid].push_back(tmpl);
		}
	} while (pResult->NextRow());
	delete pResult;
}

void WorldStateTemplateManager::ApplyMapTemplate(MapMgr *pmgr)
{
	WorldStateTemplateList::iterator itr = m_templatesForMaps[pmgr->GetMapId()].begin();
	WorldStateTemplateList::iterator itrend = m_templatesForMaps[pmgr->GetMapId()].end();
	for(; itr != itrend; ++itr)
		pmgr->GetStateManager().CreateWorldState(itr->uField, itr->uValue, itr->iFactionMask, itr->iZoneMask);

	itr = m_general.begin();
	itrend = m_general.end();
	for(; itr != itrend; ++itr)
		pmgr->GetStateManager().CreateWorldState(itr->uField, itr->uValue, itr->iFactionMask, itr->iZoneMask);
}
