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

void WorldSession::HandleGuildQuery(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 4);

	uint32 guildId;
	recv_data >> guildId;

	// we can skip some searches here if this is our guild
	if(_player && _player->GetGuildId() == guildId && _player->m_playerInfo->guild) {
		_player->m_playerInfo->guild->SendGuildQuery(this);
		return;
	}
	
	Guild * pGuild = objmgr.GetGuild( guildId );
	if(!pGuild)
		return;

	pGuild->SendGuildQuery(this);
}

void WorldSession::HandleCreateGuild(WorldPacket & recv_data)
{
}

/*void WorldSession::SendGuildCommandResult(uint32 typecmd,const char *  str,uint32 cmdresult)
{
	WorldPacket data;
	data.SetOpcode(SMSG_GUILD_COMMAND_RESULT);
	data << typecmd;
	data << str;
	data << cmdresult;
	SendPacket(&data);
}*/

void WorldSession::HandleInviteToGuild(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 1);

	std::string inviteeName;
	recv_data >> inviteeName;

	Player *plyr = objmgr.GetPlayer( inviteeName.c_str() , false);
	Guild *pGuild = _player->m_playerInfo->guild;
	
	if(!plyr)
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S,inviteeName.c_str(),GUILD_PLAYER_NOT_FOUND);
		return;
	}
	else if(!pGuild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S,"",GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	if( plyr->GetGuildId() )
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S,plyr->GetName(),ALREADY_IN_GUILD);
		return;
	}
	else if( plyr->GetGuildInvitersGuid())
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S,plyr->GetName(),ALREADY_INVITED_TO_GUILD);
		return;
	}
	else if(!_player->m_playerInfo->guildRank->CanPerformCommand(GR_RIGHT_INVITE))
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S,"",GUILD_PERMISSIONS);
		return;
	}
	else if(plyr->GetTeam()!=_player->GetTeam() && _player->GetSession()->GetPermissionCount() == 0)
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S,"",GUILD_NOT_ALLIED);
		return;
	}
	Guild::SendGuildCommandResult(this, GUILD_INVITE_S,inviteeName.c_str(),GUILD_U_HAVE_INVITED);
	//41
  
	WorldPacket data(SMSG_GUILD_INVITE, 100);
	data << _player->GetName();
	data << pGuild->GetGuildName();
	plyr->GetSession()->SendPacket(&data);

	plyr->SetGuildInvitersGuid( _player->GetLowGUID() );	
}

void WorldSession::HandleGuildAccept(WorldPacket & recv_data)
{
	Player *plyr = GetPlayer();

	if(!plyr)
		return;

	Player *inviter = objmgr.GetPlayer( plyr->GetGuildInvitersGuid() );
	plyr->UnSetGuildInvitersGuid();

	if(!inviter)
	{
		return;
	}

	Guild *pGuild = inviter->m_playerInfo->guild;
	if(!pGuild)
	{
		return;
	}

	pGuild->AddGuildMember(plyr->m_playerInfo, NULL);
}

void WorldSession::HandleGuildDecline(WorldPacket & recv_data)
{
	WorldPacket data;

	Player *plyr = GetPlayer();

	if(!plyr)
		return;

	Player *inviter = objmgr.GetPlayer( plyr->GetGuildInvitersGuid() );
	plyr->UnSetGuildInvitersGuid(); 

	if(!inviter)
		return;

	data.Initialize(SMSG_GUILD_DECLINE);
	data << plyr->GetName();
	inviter->GetSession()->SendPacket(&data);
}

void WorldSession::HandleSetGuildInformation(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 1);
	std::string NewGuildInfo;
	recv_data >> NewGuildInfo;

	Guild *pGuild = _player->m_playerInfo->guild;
	if(!pGuild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S,"",GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	pGuild->SetGuildInformation(NewGuildInfo.c_str(), this);
}

void WorldSession::HandleGuildInfo(WorldPacket & recv_data)
{
	if(_player->GetGuild() != NULL)
		_player->GetGuild()->SendGuildInfo(this);
}

void WorldSession::HandleGuildRoster(WorldPacket & recv_data)
{
	if(!_player->m_playerInfo->guild)
		return;

	_player->m_playerInfo->guild->SendGuildRoster(this);
}

void WorldSession::HandleGuildPromote(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 1);

	std::string name;
	recv_data >> name;

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S,"",GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	PlayerInfo * dstplr = objmgr.GetPlayerInfoByName(name.c_str());
	if(dstplr==NULL)
		return;

	_player->m_playerInfo->guild->PromoteGuildMember(dstplr, this);
}

void WorldSession::HandleGuildDemote(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 1);

	std::string name;
	recv_data >> name;

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S,"",GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	PlayerInfo * dstplr = objmgr.GetPlayerInfoByName(name.c_str());
	if(dstplr==NULL)
		return;

	_player->m_playerInfo->guild->DemoteGuildMember(dstplr, this);
}

void WorldSession::HandleGuildLeave(WorldPacket & recv_data)
{
	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S,"",GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	_player->m_playerInfo->guild->RemoveGuildMember(_player->m_playerInfo, this);
}

void WorldSession::HandleGuildRemove(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 1);

	std::string name;
	recv_data >> name;

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S,"",GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	PlayerInfo * dstplr = objmgr.GetPlayerInfoByName(name.c_str());
	if(dstplr==NULL)
		return;

	_player->m_playerInfo->guild->RemoveGuildMember(dstplr, this);
}

void WorldSession::HandleGuildDisband(WorldPacket & recv_data)
{
	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S,"",GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	if(_player->m_playerInfo->guild->GetGuildLeader() != _player->GetLowGUID())
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S, "", GUILD_PERMISSIONS);
		return;
	}

	_player->m_playerInfo->guild->Disband();
}

void WorldSession::HandleGuildLeader(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 1);

	std::string name;
	recv_data >> name;

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S,"",GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	PlayerInfo * dstplr = objmgr.GetPlayerInfoByName(name.c_str());
	if(dstplr==NULL)
		return;

	_player->m_playerInfo->guild->ChangeGuildMaster(dstplr, this);
}

void WorldSession::HandleGuildMotd(WorldPacket & recv_data)
{
	std::string motd;
	if(recv_data.size())
		recv_data >> motd;

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S,"",GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	_player->m_playerInfo->guild->SetMOTD(motd.c_str(), this);
}

void WorldSession::HandleGuildRank(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 9);

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S,"",GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	if(GetPlayer()->GetLowGUID() != _player->m_playerInfo->guild->GetGuildLeader())
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S,"",GUILD_PERMISSIONS);
		return;
	}

	uint32 rankId;
	string newName;
	GuildRank * pRank;

	recv_data >> rankId;
	pRank = _player->m_playerInfo->guild->GetGuildRank(rankId);
	if(pRank == NULL)
		return;

	recv_data >> pRank->iRights;
	recv_data >> newName;

	if(newName.length() < 2)
		newName = string(pRank->szRankName);
	
	if(strcmp(newName.c_str(), pRank->szRankName) != 0)
	{
		// name changed
		char * pTmp = pRank->szRankName;
		pRank->szRankName = strdup(newName.c_str());
		free(pTmp);
	}

	CharacterDatabase.Execute("REPLACE INTO guild_ranks VALUES(%u, %u, \"%s\", %d, %d)",
		_player->m_playerInfo->guild->GetGuildId(), pRank->iId, CharacterDatabase.EscapeString(newName).c_str(),
		pRank->iRights);
}

void WorldSession::HandleGuildAddRank(WorldPacket & recv_data)
{
	string rankName;
	Guild * pGuild = _player->GetGuild();

	if(pGuild == NULL)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	if(pGuild->GetGuildLeader() != _player->GetLowGUID())
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PERMISSIONS);
		return;
	}

	recv_data >> rankName;
	if(rankName.size() < 2)
		return;

	pGuild->CreateGuildRank(rankName.c_str(), GR_RIGHT_DEFAULT, false);

	// there is probably a command result for this. need to find it.
	pGuild->SendGuildQuery(NULL);
	pGuild->SendGuildRoster(this);
}

void WorldSession::HandleGuildDelRank(WorldPacket & recv_data)
{
	Guild * pGuild = _player->GetGuild();

	if(pGuild == NULL)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	if(pGuild->GetGuildLeader() != _player->GetLowGUID())
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PERMISSIONS);
		return;
	}

	pGuild->RemoveGuildRank(this);

	// there is probably a command result for this. need to find it.
	pGuild->SendGuildQuery(NULL);
	pGuild->SendGuildRoster(this);
}

void WorldSession::HandleGuildSetPublicNote(WorldPacket & recv_data)
{
	string target, newnote;
	recv_data >> target >> newnote;

	PlayerInfo * pTarget = objmgr.GetPlayerInfoByName(target.c_str());
	if(pTarget == NULL)
		return;

	if(!pTarget->guild)
		return;

	pTarget->guild->SetPublicNote(pTarget, newnote.c_str(), this);
}

void WorldSession::HandleGuildSetOfficerNote(WorldPacket & recv_data)
{
	string target, newnote;
	recv_data >> target >> newnote;

	PlayerInfo * pTarget = objmgr.GetPlayerInfoByName(target.c_str());
	if(pTarget == NULL)
		return;

	if(!pTarget->guild)
		return;

	pTarget->guild->SetOfficerNote(pTarget, newnote.c_str(), this);
}

void WorldSession::HandleSaveGuildEmblem(WorldPacket & recv_data)
{
	uint64 guid;
	Guild * pGuild = _player->GetGuild();
	int32 cost = MONEY_ONE_GOLD * 10;
	uint32 emblemStyle, emblemColor, borderStyle, borderColor, backgroundColor;
	WorldPacket data(MSG_SAVE_GUILD_EMBLEM, 4);
	recv_data >> guid;
	
	CHECK_PACKET_SIZE(recv_data, 28);
	CHECK_INWORLD_RETURN;
	CHECK_GUID_EXISTS(guid);

	recv_data >> emblemStyle >> emblemColor >> borderStyle >> borderColor >> backgroundColor;
	if(pGuild==NULL)
	{
		data << uint32(ERR_GUILDEMBLEM_NOGUILD);
		SendPacket(&data);
		return;
	}

	if(pGuild->GetGuildLeader() != _player->GetLowGUID())
	{
		data << uint32(ERR_GUILDEMBLEM_NOTGUILDMASTER);
		SendPacket(&data);
		return;
	}

	if(_player->GetUInt32Value(PLAYER_FIELD_COINAGE) < (uint32)cost)
	{
		data << uint32(ERR_GUILDEMBLEM_NOTENOUGHMONEY);
		SendPacket(&data);
		return;
	}

	data <<	uint32(ERR_GUILDEMBLEM_SUCCESS);
	SendPacket(&data);

	// set in memory and database
	pGuild->SetTabardInfo(emblemStyle, emblemColor, borderStyle, borderColor, backgroundColor);

	// update all clients (probably is an event for this, again.)
	pGuild->SendGuildQuery(NULL);
}

// Charter part
void WorldSession::HandleCharterBuy(WorldPacket & recv_data)
{
	/*
	{CLIENT} Packet: (0x01BD) CMSG_PETITION_BUY PacketSize = 85
	|------------------------------------------------|----------------|
	|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
	|------------------------------------------------|----------------|
	|50 91 00 00 6E 13 01 F0 00 00 00 00 00 00 00 00 |P...n...........|
	|00 00 00 00 53 74 6F 72 6D 62 72 69 6E 67 65 72 |....Stormbringer|
	|73 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |s...............|
	|00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |................|
	|00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 |................|
	|00 00 00 00 00								  |.....		   |
	-------------------------------------------------------------------
	*/

	uint64 creature_guid;
    uint64 unk1, unk3, unk4, unk5, unk6, unk7;
    uint32 unk2;
	string name;
    uint8 error;
    uint16 unk8;
    uint8  unk9;
    uint32 unk10;                                           // selected index
    uint32 unk11;
	recv_data >> creature_guid >> unk1 >> unk2 >> name >> unk3 >> unk4 >> unk5;
	recv_data >> unk6>> unk7 >> unk8 >> unk9 >> unk10 >> unk11;

	Creature * crt = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(creature_guid));
	if(!crt)
	{
		Disconnect();
		return;
	}
	Guild * g = objmgr.GetGuildByGuildName(name);
	Charter * c = objmgr.GetCharterByName(name, CHARTER_TYPE_GUILD);
	if(g != 0 || c != 0)
	{
		SendNotification("A guild with that name already exists.");
		return;
	}

	if(_player->m_charters[CHARTER_TYPE_GUILD])
	{
		SendNotification("You already have a guild charter.");
		return;
	}

	ItemPrototype * ip = ItemPrototypeStorage.LookupEntry(ITEM_ENTRY_GUILD_CHARTER);
	assert(ip);
	SlotResult res = _player->GetItemInterface()->FindFreeInventorySlot(ip);
	if(res.Result == 0)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_INVENTORY_FULL);
		return;
	}

	error = _player->GetItemInterface()->CanReceiveItem(ItemPrototypeStorage.LookupEntry(ITEM_ENTRY_GUILD_CHARTER),1);
	if(error)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(NULL,NULL,error);
	}
	else
	{
		// Meh...
		WorldPacket data(SMSG_PLAY_OBJECT_SOUND, 12);
		data << uint32(0x000019C2);
		data << creature_guid;
		SendPacket(&data);

		// Create the item and charter
		Item * i = objmgr.CreateItem(ITEM_ENTRY_GUILD_CHARTER, _player);
		c = objmgr.CreateCharter(_player->GetLowGUID(), CHARTER_TYPE_GUILD);
		c->GuildName = name;
		c->ItemGuid = i->GetGUID();


		i->SetUInt32Value(ITEM_FIELD_STACK_COUNT, 1);
		i->SetUInt32Value(ITEM_FIELD_FLAGS, 1);
		i->SetUInt32Value(ITEM_FIELD_ENCHANTMENT, c->GetID());
		i->SetUInt32Value(ITEM_FIELD_PROPERTY_SEED, 57813883);
		if( !_player->GetItemInterface()->AddItemToFreeSlot(i) )
		{
			c->Destroy();
			delete i;
			return;
		}

		c->SaveToDB();

		/*data.clear();
		data.resize(45);
		BuildItemPushResult(&data, _player->GetGUID(), ITEM_PUSH_TYPE_RECEIVE, 1, ITEM_ENTRY_GUILD_CHARTER, 0);
		SendPacket(&data);*/
		SendItemPushResult(i, false, true, false, true, _player->GetItemInterface()->LastSearchItemBagSlot(), _player->GetItemInterface()->LastSearchItemSlot(), 1);

		_player->m_charters[CHARTER_TYPE_GUILD] = c;
		_player->SaveToDB(false);
	}
}

void SendShowSignatures(Charter * c, uint64 i, Player * p)
{
	WorldPacket data(SMSG_PETITION_SHOW_SIGNATURES, 100);
	data << i;
	data << (uint64)c->GetLeader();
	data << c->GetID();
	data << uint8(c->SignatureCount);
	for(uint32 i = 0; i < c->Slots; ++i)
	{
		if(c->Signatures[i] == 0) continue;
		data << uint64(c->Signatures[i]) << uint32(1);
	}
	data << uint8(0);
	p->GetSession()->SendPacket(&data);
}

void WorldSession::HandleCharterShowSignatures(WorldPacket & recv_data)
{
	Charter * pCharter;
	uint64 item_guid;
	recv_data >> item_guid;
	pCharter = objmgr.GetCharterByItemGuid(item_guid);
	
	if(pCharter)
		SendShowSignatures(pCharter, item_guid, _player);
}

void WorldSession::HandleCharterQuery(WorldPacket & recv_data)
{
	/*
	{SERVER} Packet: (0x01C7) SMSG_PETITION_QUERY_RESPONSE PacketSize = 77
	|------------------------------------------------|----------------|
	|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
	|------------------------------------------------|----------------|
	|20 08 00 00 28 32 01 00 00 00 00 00 53 74 6F 72 | ...(2......Stor|
	|6D 62 72 69 6E 67 65 72 73 00 00 09 00 00 00 09 |mbringers.......|
	|00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |................|
	|00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |................|
	|00 00 00 00 00 00 00 00 00 00 00 00 00		  |.............   |
	-------------------------------------------------------------------
	
	uint32 charter_id
	uint64 leader_guid
	string guild_name
	uint8  0			<-- maybe subname? or some shit.. who knows
	uint32 9
	uint32 9
	uint32[9] signatures
	uint8  0
	uint8  0
	*/
//this is wrong there are 42 bytes after 9 9, 4*9+2=38 not 42,
	//moreover it can't signature, blizz uses always fullguid so it must be uin64
	//moreover this does not show ppl who signed this, for this purpose there is another opcde
	uint32 charter_id;
	uint64 item_guid;
	recv_data >> charter_id;
	recv_data >> item_guid;
	/*Charter * c = objmgr.GetCharter(charter_id,CHARTER_TYPE_GUILD);
	if(c == 0)
		c = objmgr.GetCharter(charter_id, CHARTER_TYPE_ARENA_2V2);
	if(c == 0)
		c = objmgr.GetCharter(charter_id, CHARTER_TYPE_ARENA_3V3);
	if(c == 0)
		c = objmgr.GetCharter(charter_id, CHARTER_TYPE_ARENA_5V5);*/

	Charter * c = objmgr.GetCharterByItemGuid(item_guid);
	if(c == 0)
		return;

	WorldPacket data(SMSG_PETITION_QUERY_RESPONSE, 100);
	data << charter_id;
	data << (uint64)c->LeaderGuid;
	data << c->GuildName << uint8(0);
	if(c->CharterType == CHARTER_TYPE_GUILD)
	{
		data << uint32(9) << uint32(9);
	}
	else
	{
		/*uint32 v = c->CharterType;
		if(c->CharterType == CHARTER_TYPE_ARENA_3V3)
			v=2;
		else if(c->CharterType == CHARTER_TYPE_ARENA_5V5)
			v=4;

		data << v << v;*/
		data << uint32(c->Slots) << uint32(c->Slots);
	}

	data << uint32(0);                                      // 4
    data << uint32(0);                                      // 5
    data << uint32(0);                                      // 6
    data << uint32(0);                                      // 7
    data << uint32(0);                                      // 8
    data << uint16(0);                                      // 9 2 bytes field
    
	if( c->CharterType == CHARTER_TYPE_GUILD )
	{
		data << uint32(1);                                      // 10
	}
	else
	{
		data << uint32(70);                                      // 10
	}

    data << uint32(0);                                      // 11
    data << uint32(0);                                      // 13 count of next strings?
    data << uint32(0);                                      // 14

	if (c->CharterType == CHARTER_TYPE_GUILD)
	{
		data << uint32(0);
	}
	else
	{
		data << uint32(1);
	}

	SendPacket(&data);
}

void WorldSession::HandleCharterOffer( WorldPacket & recv_data )
{
	uint32 shit;
	uint64 item_guid, target_guid;
	Charter * pCharter;
	recv_data >> shit >> item_guid >> target_guid;
	
	CHECK_INWORLD_RETURN
	Player * pTarget = _player->GetMapMgr()->GetPlayer((uint32)target_guid);
	pCharter = objmgr.GetCharterByItemGuid(item_guid);

	if(pTarget == 0 || pTarget->GetTeam() != _player->GetTeam() || pTarget == _player)
	{
		SendNotification("Target is of the wrong faction.");
		return;
	}

	if(!pTarget->CanSignCharter(pCharter, _player))
	{
		SendNotification("Target player cannot sign your charter for one or more reasons.");
		return;
	}

	SendShowSignatures(pCharter, item_guid, pTarget);
}

void WorldSession::HandleCharterSign( WorldPacket & recv_data )
{
	uint64 item_guid;
	recv_data >> item_guid;

	Charter * c = objmgr.GetCharterByItemGuid(item_guid);
	if(c == 0)
		return;

	for(uint32 i = 0; i < 9; ++i)
	{
		if(c->Signatures[i] == _player->GetGUID())
		{
			SendNotification("You have already signed that charter.");
			return;
		}
	}

	if(c->IsFull())
		return;

	c->AddSignature(_player->GetLowGUID());
	c->SaveToDB();
	_player->m_charters[c->CharterType] = c;
	_player->SaveToDB(false);

	Player * l = _player->GetMapMgr()->GetPlayer(c->GetLeader());
	if(l == 0)
		return;

	WorldPacket data(SMSG_PETITION_SIGN_RESULTS, 100);
	data << item_guid << _player->GetGUID() << uint32(0);
	l->GetSession()->SendPacket(&data);
	data.clear();
	data << item_guid << (uint64)c->GetLeader() << uint32(0);
	SendPacket(&data);
}

void WorldSession::HandleCharterTurnInCharter(WorldPacket & recv_data)
{
	uint64 mooguid;
	recv_data >> mooguid;
	Charter * pCharter = objmgr.GetCharterByItemGuid(mooguid);
	if(!pCharter) return;

	if(pCharter->CharterType == CHARTER_TYPE_GUILD)
	{
		Charter * gc = _player->m_charters[CHARTER_TYPE_GUILD];
		if(gc == 0)
			return;
		if(gc->SignatureCount < 9 && Config.MainConfig.GetBoolDefault("Server", "RequireAllSignatures", false))
		{
			SendNotification("You don't have the required amount of signatures to turn in this petition.");
			return;
		}

		// dont know hacky or not but only solution for now
		// If everything is fine create guild

		Guild *pGuild = Guild::Create();
		pGuild->CreateFromCharter(gc, this);

		// Destroy the charter
		_player->m_charters[CHARTER_TYPE_GUILD] = 0;
		gc->Destroy();

		_player->GetItemInterface()->RemoveItemAmt(ITEM_ENTRY_GUILD_CHARTER, 1);
		sHookInterface.OnGuildCreate(_player, pGuild);
	}

	WorldPacket data(4);
	data.SetOpcode(SMSG_TURN_IN_PETITION_RESULTS);
	data << uint32(0);
	SendPacket( &data );
}

void WorldSession::HandleCharterRename(WorldPacket & recv_data)
{
	uint64 guid;
	string name;
	recv_data >> guid >> name;

	Charter * pCharter = objmgr.GetCharterByItemGuid(guid);
	if(pCharter == 0)
		return;

	Guild * g = objmgr.GetGuildByGuildName(name);
	Charter * c = objmgr.GetCharterByName(name, (CharterTypes)pCharter->CharterType);
	if(c || g)
	{
		SendNotification("That name is in use by another guild.");
		return;
	}

	c = pCharter;
	c->GuildName = name;
	c->SaveToDB();
	WorldPacket data(MSG_PETITION_RENAME, 100);
	data << guid << name;
	SendPacket(&data);
}
