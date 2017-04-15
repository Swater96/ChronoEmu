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

void WorldSession::HandleConvertGroupToRaidOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN
	// This is just soooo easy now   
	Group *pGroup = _player->GetGroup();
	if(!pGroup) return;

	if ( pGroup->GetLeader() != _player->m_playerInfo )   //access denied
	{
		SendPartyCommandResult(_player, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
		return;
	}

	pGroup->ExpandToRaid();
	SendPartyCommandResult(_player, 0, "", ERR_PARTY_NO_ERROR);
}

void WorldSession::HandleGroupChangeSubGroup(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN
	std::string name;
	uint8 subGroup;

	recv_data >> name;
	recv_data >> subGroup;

	PlayerInfo * inf = objmgr.GetPlayerInfoByName(name.c_str());
	if(inf == nullptr || inf->m_Group == nullptr || inf->m_Group != _player->m_playerInfo->m_Group)
		return;

	_player->GetGroup()->MovePlayer(inf, subGroup);
}

void WorldSession::HandleGroupAssistantLeader(WorldPacket & recv_data)
{
	uint64 guid;
	uint8 on;

	if(!_player->IsInWorld())
		return;

	if(_player->GetGroup() == nullptr)
		return;

	if ( _player->GetGroup()->GetLeader() != _player->m_playerInfo )   //access denied
	{
		SendPartyCommandResult(_player, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
		return;
	}

	recv_data >> guid >> on;
	if(on == 0)
        _player->GetGroup()->SetAssistantLeader(nullptr);
	else
	{
		PlayerInfo * np = objmgr.GetPlayerInfo((uint32)guid);
		if(np==nullptr)
			_player->GetGroup()->SetAssistantLeader(nullptr);
		else
		{
			if(_player->GetGroup()->HasMember(np))
				_player->GetGroup()->SetAssistantLeader(np);
		}
	}
}

void WorldSession::HandleGroupPromote(WorldPacket& recv_data)
{
	uint8 promotetype, on;
	uint64 guid;

	if(!_player->IsInWorld())
		return;

	if(_player->GetGroup() == nullptr)
		return;

	if ( _player->GetGroup()->GetLeader() != _player->m_playerInfo )   //access denied
	{
		SendPartyCommandResult(_player, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
		return;
	}

	recv_data >> promotetype >> on;
	recv_data >> guid;

	void(Group::*function_to_call)(PlayerInfo*);

	if(promotetype == 0)
		function_to_call = &Group::SetMainTank;
	else if(promotetype==1)
		function_to_call = &Group::SetMainAssist;

	if(on == 0)
		(_player->GetGroup()->*function_to_call)(nullptr);
	else
	{
		PlayerInfo * np = objmgr.GetPlayerInfo((uint32)guid);
		if(np==nullptr)
			(_player->GetGroup()->*function_to_call)(nullptr);
		else
		{
			if(_player->GetGroup()->HasMember(np))
				(_player->GetGroup()->*function_to_call)(np);
		}
	}
}

void WorldSession::HandleRequestRaidInfoOpcode(WorldPacket & recv_data)
{  
	//		  SMSG_RAID_INSTANCE_INFO			 = 716,  //(0x2CC)	
	//sInstanceSavingManager.BuildRaidSavedInstancesForPlayer(_player);
	sInstanceMgr.BuildRaidSavedInstancesForPlayer(_player);
}

void WorldSession::HandleReadyCheckOpcode(WorldPacket& recv_data)
{
	Group * pGroup  = _player->GetGroup();
	WorldPacket data(CMSG_RAID_READYCHECK, 20);
	uint8 ready;

	if(!pGroup || ! _player->IsInWorld())
		return;

	if(recv_data.size() == 0)
	{
		if(pGroup->GetLeader() == _player->m_playerInfo)
		{
			/* send packet to group */
			pGroup->SendPacketToAllButOne(&data, _player);
		}
		else
		{
			SendNotification(NOTIFICATION_MESSAGE_NO_PERMISSION);
		}
	}
	else
	{
		if(_player->m_playerInfo != pGroup->GetLeader())
		{
			recv_data >> ready;
			data << _player->GetGUID();
			data << ready;
			if(pGroup->GetLeader() && pGroup->GetLeader()->m_loggedInPlayer)
				pGroup->GetLeader()->m_loggedInPlayer->GetSession()->SendPacket(&data);
		}
	}
}

