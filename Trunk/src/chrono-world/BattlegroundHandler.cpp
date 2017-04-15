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

void WorldSession::HandleBattlefieldPortOpcode(WorldPacket &recv_data)
{
	uint32 mapId;
	uint8 action;
	
	if(!_player->IsInWorld()) return;
	recv_data >> mapId >> action;
	
	uint32 bgtype = BattlegroundManager.GetBattleGroundTypeByMapId(mapId);

	if(action == 0)
	{
		uint32 BGQueueSlot = _player->GetBGQueueSlotByBGType(bgtype);
		_player->RemoveFromBattlegroundQueue(BGQueueSlot);
		//BattlegroundManager.RemovePlayerFromQueues(_player);
	}
	else
	{
		for(uint32 i = 0; i < 3; ++i)
		{
			if( GetPlayer()->m_pendingBattleground[i] && 
				(GetPlayer()->m_pendingBattleground[i]->GetType() == bgtype ))
			{
				if( GetPlayer()->m_bg )
				{
					GetPlayer()->m_bg->RemovePlayer(GetPlayer(), true); // Send Logout = true so we can TP him now.
				}

				GetPlayer()->m_pendingBattleground[i]->PortPlayer(GetPlayer());
				return;
			}
		}
	}
}

void WorldSession::HandleBattlefieldStatusOpcode(WorldPacket &recv_data)
{
	BattlegroundManager.SendBattlegroundQueueStatus(GetPlayer(), 0);
	BattlegroundManager.SendBattlegroundQueueStatus(GetPlayer(), 1);
	BattlegroundManager.SendBattlegroundQueueStatus(GetPlayer(), 2);
}

void WorldSession::HandleBattlefieldListOpcode(WorldPacket &recv_data)
{
	uint32 type;
	recv_data >> type;

	CHECK_INWORLD_RETURN;

	if( GetPlayer()->HasBGQueueSlotOfType(type) == 4)
		return;
	
	BattlegroundManager.HandleBattlegroundListPacket(this, type);
}

// Returns -1 if indeterminable.
int32 GetBattlegroundTypeFromCreature(Creature* pCreature)
{
	if( pCreature->m_factionDBC->ID == 890 ) // Silverwing Sentinels
		return BATTLEGROUND_WARSONG_GULCH;

	if( pCreature->m_factionDBC->ID == 889 ) // Warsong Outriders
		return BATTLEGROUND_WARSONG_GULCH;

	if( pCreature->m_factionDBC->ID == 509 ) // League of Arathor
		return BATTLEGROUND_ARATHI_BASIN;

	if( pCreature->m_factionDBC->ID == 510 ) // The Defilers
		return BATTLEGROUND_ARATHI_BASIN;

	if( pCreature->m_factionDBC->ID == 730 ) // Stormpike Guard
		return BATTLEGROUND_ALTERAC_VALLEY;

	if( pCreature->m_factionDBC->ID == 729 ) // The Frostwolf
		return BATTLEGROUND_ALTERAC_VALLEY;

	if( string(pCreature->GetCreatureInfo()->Name).find("Warsong Gulch") != string::npos )
		return BATTLEGROUND_WARSONG_GULCH;

	if( string(pCreature->GetCreatureInfo()->Name).find("Arathi Basin") != string::npos )
		return BATTLEGROUND_ARATHI_BASIN;

	if( string(pCreature->GetCreatureInfo()->Name).find("Alterac Valley") != string::npos )
		return BATTLEGROUND_ALTERAC_VALLEY;

	if( string(pCreature->GetCreatureInfo()->SubName).find("Warsong Gulch") != string::npos )
		return BATTLEGROUND_WARSONG_GULCH;

	if( string(pCreature->GetCreatureInfo()->SubName).find("Arathi Basin") != string::npos )
		return BATTLEGROUND_ARATHI_BASIN;

	if( string(pCreature->GetCreatureInfo()->SubName).find("Alterac Valley") != string::npos )
		return BATTLEGROUND_ALTERAC_VALLEY;

	return -1;
}

void WorldSession::SendBattlegroundList(Creature* pCreature, uint32 mapid)
{
	if(!pCreature)
		return;

	int32 type = GetBattlegroundTypeFromCreature( pCreature );

	if( type == -1 )
		SystemMessage("Sorry, invalid battlemaster.");
	else
		BattlegroundManager.HandleBattlegroundListPacket(this, type);
}

void WorldSession::HandleBattleMasterHelloOpcode(WorldPacket &recv_data)
{
	uint64 guid;
	recv_data >> guid;

	CHECK_INWORLD_RETURN;
	Creature * pCreature = _player->GetMapMgr()->GetCreature( GET_LOWGUID_PART(guid) );
	if( pCreature == nullptr )
		return;

	SendBattlegroundList( pCreature, 0 );
}

void WorldSession::HandleLeaveBattlefieldOpcode(WorldPacket &recv_data)
{
	if(_player->m_bg && _player->IsInWorld())
		_player->m_bg->RemovePlayer(_player, false);
}

void WorldSession::HandleAreaSpiritHealerQueryOpcode(WorldPacket &recv_data)
{
	if(!_player->IsInWorld() || !_player->m_bg) return;
	uint64 guid;
	recv_data >> guid;

	Creature * psg = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(psg == nullptr)
		return;
	
	uint32 restime = _player->m_bg->GetLastResurrect() + 30;
	if((uint32)UNIXTIME > restime)
		restime = 1000;
	else
		restime = (restime - (uint32)UNIXTIME) * 1000;

	WorldPacket data(SMSG_AREA_SPIRIT_HEALER_TIME, 12);
	data << guid << restime;
	SendPacket(&data);
}

void WorldSession::HandleAreaSpiritHealerQueueOpcode(WorldPacket &recv_data)
{
	if(!_player->IsInWorld() || !_player->m_bg) return;
	uint64 guid;
	recv_data >> guid;
	Creature * psg = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(psg == nullptr)
		return;

	_player->m_bg->QueuePlayerForResurrect(_player, psg);
}

void WorldSession::HandleBattlegroundPlayerPositionsOpcode(WorldPacket &recv_data)
{
	/* This packet doesn't appear to be used anymore... 
	 * - Burlex
	 */
}

void WorldSession::HandleBattleMasterJoinOpcode(WorldPacket &recv_data)
{
	CHECK_INWORLD_RETURN
	if(_player->GetGroup() && _player->GetGroup()->m_isqueued)
	{
		SystemMessage("You are in a group that is already queued for a battleground or inside a battleground. Leave this first.");
		return;
	}

	if(_player->IsInWorld())
		BattlegroundManager.HandleBattlegroundJoin(this, recv_data);
}

void WorldSession::HandleInspectHonorStatsOpcode( WorldPacket &recv_data )
{
	Player *pl;
    uint64 guid;
    recv_data >> guid;

	pl = objmgr.GetPlayer(guid);
	if(pl)
	{
		WorldPacket data( MSG_INSPECT_HONOR_STATS, (8+1+4+4+4+4+4+4+4+4+4+4+1) );
		
		data << guid;                                       // player guid
                                                            // Rank, filling bar, PLAYER_BYTES_3, ??
        data << (uint8)pl->GetUInt32Value(PLAYER_FIELD_BYTES2);
                                                            // Today Honorable and Dishonorable Kills
        data << pl->GetUInt32Value(PLAYER_FIELD_SESSION_KILLS);
                                                            // Yesterday Honorable Kills
        data << pl->GetUInt32Value(PLAYER_FIELD_YESTERDAY_KILLS);
                                                            // Last Week Honorable Kills
        data << pl->GetUInt32Value(PLAYER_FIELD_LAST_WEEK_KILLS);
                                                            // This Week Honorable kills
        data << pl->GetUInt32Value(PLAYER_FIELD_THIS_WEEK_KILLS);
                                                            // Lifetime Honorable Kills
        data << pl->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
                                                            // Lifetime Dishonorable Kills
        data << pl->GetUInt32Value(PLAYER_FIELD_LIFETIME_DISHONORABLE_KILLS);
                                                            // Yesterday Honor
        data << pl->GetUInt32Value(PLAYER_FIELD_YESTERDAY_CONTRIBUTION);
                                                            // Last Week Honor
        data << pl->GetUInt32Value(PLAYER_FIELD_LAST_WEEK_CONTRIBUTION);
                                                            // This Week Honor
        data << pl->GetUInt32Value(PLAYER_FIELD_THIS_WEEK_CONTRIBUTION);
                                                            // Last Week Standing
        data << pl->GetUInt32Value(PLAYER_FIELD_LAST_WEEK_RANK);
		
		if(pl->GetHonorHighestRankInfo().rank > 0)
			data << (uint8)(pl->GetHonorHighestRankInfo().rank + 4);
		else
			data << (uint8)(pl->GetHonorHighestRankInfo().rank); 
        SendPacket(&data);
	}
	else
        sLog.outDebug("Player %u not found!", guid);
}

void WorldSession::HandlePVPLogDataOpcode(WorldPacket &recv_data)
{
	CHECK_INWORLD_RETURN
	if(_player->m_bg)
		_player->m_bg->SendPVPData(_player);
}

