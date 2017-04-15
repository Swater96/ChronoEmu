//
// Chrono Emu (C) 2016
//

#include "StdAfx.h"
#include "AuthCodes.h"
#include "svn_revision.h"

bool VerifyName(const char * name, size_t nlen)
{
	const char * p;
	size_t i;

	static const char * bannedCharacters = "\t\v\b\f\a\n\r\\\"\'\? <>[](){}_=+-|/!@#$%^&*~`.,0123456789\0";
	static const char * allowedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if(sWorld.m_limitedNames)
	{
		for(i = 0; i < nlen; ++i)
		{
			p = allowedCharacters;
			for(; *p != 0; ++p)
			{
				if(name[i] == *p)
					goto cont;
			}

			return false;
cont:
			continue;
		}
	}
	else
	{
		for(i = 0; i < nlen; ++i)
		{
			p = bannedCharacters;
			while(*p != 0 && name[i] != *p && name[i] != 0)
				++p;

			if(*p != 0)
				return false;
		}
	}
	
	return true;
}

bool ChatHandler::HandleRenameAllCharacter(const char * args, WorldSession * m_session)
{
	uint32 uCount = 0;
	uint32 ts = getMSTime();
	QueryResult * result = CharacterDatabase.Query("SELECT guid, name FROM characters");
	if( result )
	{
		do 
		{
			uint32 uGuid = result->Fetch()[0].GetUInt32();
			const char * pName = result->Fetch()[1].GetString();
			size_t szLen = strlen(pName);

			if( !VerifyName(pName, szLen) )
			{
				printf("renaming character %s, %u\n", pName,uGuid);
                Player * pPlayer = objmgr.GetPlayer(uGuid);
				if( pPlayer != nullptr )
				{
					pPlayer->rename_pending = true;
					pPlayer->GetSession()->SystemMessage("Your character has had a force rename set, you will be prompted to rename your character at next login in conformance with server rules.");
				}

				CharacterDatabase.WaitExecute("UPDATE characters SET forced_rename_pending = 1 WHERE guid = %u", uGuid);
				++uCount;
			}

		} while (result->NextRow());
		delete result;
	}

	SystemMessage(m_session, "Procedure completed in %u ms. %u character(s) forced to rename.", getMSTime() - ts, uCount);
	return true;
}

void CapitalizeString(string& arg)
{
	if(arg.length() == 0) return;
	arg[0] = toupper(arg[0]);
	for(uint32 x = 1; x < arg.size(); ++x)
		arg[x] = tolower(arg[x]);
}

void WorldSession::CharacterEnumProc(QueryResult * result)
{
}

void WorldSession::HandleCharEnumOpcode( WorldPacket & recv_data )
{	
	QueryResult* result = CharacterDatabase.Query("SELECT guid, level, race, class, gender, bytes, bytes2, name, positionX, positionY, positionZ, mapId, zoneId, banned, restState, deathstate, forced_rename_pending, player_flags FROM characters WHERE acct=%u ORDER BY guid LIMIT 10", GetAccountId());

	// should be more than enough.. 200 bytes per char..
	WorldPacket data(SMSG_CHAR_ENUM, (result ? result->GetRowCount() * 200 : 1));	
    uint8 num = 0;
	data << num;
    if( result )
    {
	     Player *plr;
         uint32 guid;
         Field *fields;
         do
         {
              fields = result->Fetch();
              guid = fields[0].GetUInt32();
			
              plr = objmgr.GetPlayer(guid);
              if(plr)
              {
                  // we already have that player in world... for some strange reason...
                   continue;
              }

              plr = new Player(HIGHGUID_TYPE_PLAYER);
              ASSERT(plr);
              plr->SetSession(this);
			
              //plr->LoadFromDB( guid );
			  plr->LoadFromDB_Light( fields, guid );
			  plr->BuildEnumData(result, &data);

            num++;			
		}	
        while( result->NextRow() );
        delete result;
    }
	
	data.put<uint8>(0, num);

	SendPacket( &data );
}

void WorldSession::LoadAccountDataProc(QueryResult * result)
{
	size_t len;
	const char * data;
	char * d;

	if(!result)
	{
		CharacterDatabase.Execute("INSERT INTO account_data VALUES(%u, '', '', '', '', '', '', '', '', '')", _accountId);
		return;
	}

	for(uint32 i = 0; i < 7; ++i)
	{
		data = result->Fetch()[1+i].GetString();
		len = data ? strlen(data) : 0;
		if(len > 1)
		{
			d = new char[len+1];
			memcpy(d, data, len+1);
			SetAccountData(i, d, true, (uint32)len);
		}
	}
}

void WorldSession::HandleCharCreateOpcode( WorldPacket & recv_data )
{
	std::string name;
	uint8 race, class_;

	recv_data >> name >> race >> class_;
	recv_data.rpos(0);
	
    WorldPacket data(SMSG_CHAR_CREATE, 1);
	
	if(!VerifyName(name.c_str(), name.length()))
	{
		data << (uint8)CHAR_CREATE_IN_USE;
        SendPacket( &data );
		return;
	}

	if(g_characterNameFilter->Parse(name, false))
	{
        data << (uint8)CHAR_CREATE_IN_USE;
        SendPacket( &data );
		return;
	}

	if(objmgr.GetPlayerInfoByName(name.c_str()) != 0)
	{
        data << (uint8)CHAR_CREATE_IN_USE;
        SendPacket( &data );
        return;
	}

	if(!sHookInterface.OnNewCharacter(race, class_, this, name.c_str()))
	{
        data << (uint8)0x0;
        SendPacket( &data );
        return;
	}

	QueryResult * result = CharacterDatabase.Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(name).c_str());
	if(result)
	{
		if(result->Fetch()[0].GetUInt32() > 0)
		{
			// That name is banned!
			OutPacket(SMSG_CHAR_CREATE, 1, "\x51"); // You cannot use that name
			delete result;
			return;
		}
		delete result;
	}
	// loading characters
	
	//checking number of chars is useless since client will not allow to create more than 10 chars
	//as the 'create' button will not appear (unless we want to decrease maximum number of characters)

	Player * pNewChar = objmgr.CreatePlayer();
	pNewChar->SetSession(this);
	if(!pNewChar->Create( recv_data ))
	{
		// failed.
		pNewChar->ok_to_remove = true;
		delete pNewChar;
		return;
	}

	//Same Faction limitation only applies to PVP and RPPVP realms :)
	uint32 realmType = sLogonCommHandler.GetRealmType();
	if( !HasGMPermissions() && realmType == REALM_PVP && _side >= 0 )
	{
		if( ((pNewChar->GetTeam()== 0) && (_side == 1)) || ((pNewChar->GetTeam()== 1) && (_side == 0)) )
		{
			pNewChar->ok_to_remove = true;
			delete pNewChar;
			WorldPacket data(1);
			data.SetOpcode(SMSG_CHAR_CREATE);
			data << (uint8)ALL_CHARS_ON_PVP_REALM_MUST_AT_SAME_SIDE+1;
			SendPacket( &data );
			return;
		}
	}
	pNewChar->UnSetBanned();
	pNewChar->addSpell(22027);	  // Remove Insignia

	if(pNewChar->getClass() == WARLOCK)
	{
		pNewChar->AddSummonSpell(416, 3110);		// imp fireball
		pNewChar->AddSummonSpell(417, 19505);
		pNewChar->AddSummonSpell(1860, 3716);
		pNewChar->AddSummonSpell(1863, 7814);
	}

	pNewChar->SaveToDB(true);	

	PlayerInfo *pn=new PlayerInfo ;
	pn->guid = pNewChar->GetLowGUID();
	pn->name = strdup(pNewChar->GetName());
	pn->cl = pNewChar->getClass();
	pn->race = pNewChar->getRace();
	pn->gender = pNewChar->getGender();
	pn->publicNote = nullptr;
	pn->officerNote = nullptr;
	pn->m_Group=0;
	pn->subGroup=0;
	pn->m_loggedInPlayer=nullptr;
	pn->team = pNewChar->GetTeam ();
	objmgr.AddPlayerInfo(pn);

	pNewChar->ok_to_remove = true;
	delete  pNewChar;

    data << (uint8)CHAR_CREATE_SUCCESS;
    SendPacket( &data );

	sLogonCommHandler.UpdateAccountCount(GetAccountId(), 1);
}

/* FOR 1.10.1
SMSG_CHAR_CREATE Error Codes:
0x00 Success
0x01 Failure
0x02 Canceled
0x03 Disconnect from server
0x04 Failed to connect
0x05 Connected
0x06 Wrong client version
0x07 Connecting to server
0x08 Negotiating security
0x09 Negotiating security complete
0x0A Negotiating security failed
0x0B Authenticating
0x0C Authentication successful
0x0D Authentication failed
0x0E Login unavailable - Please contact Tech Support
0x0F Server is not valid
0x10 System unavailable 
0x11 System error
0x12 Billing system error
0x13 Account billing has expired
0x14 Wrong client version
0x15 Unknown account
0x16 Incorrect password
0x17 Session expired
0x18 Server Shutting Down
0x19 Already logged in
0x1A Invalid login server
0x1B Position in Queue: 0
0x1C This account has been banned
0x1D This character is still logged on
0x1E Your WoW subscription has expired
0x1F This session has timed out
0x20 This account has been temporarily suspended
0x21 Access to this account blocked by parental controls 
0x22 Retrieving realmlist
0x23 Realmlist retrieved
0x24 Unable to connect to realmlist server
0x25 Invalid realmlist
0x26 The game server is currently down
0x27 Creating account
0x28 Account created
0x29 Account creation failed
0x2A Retrieving character list
0x2B Character list retrieved
0x2C Error retrieving character list
0x2D Creating character
0x2E Character created
0x2F Error creating character
0x30 Character creation failed
0x31 That name is unavailable
0x32 Creation of that race/class is disabled
0x33 You cannot have both horde and alliance character at pvp realm
0x33 All characters on a PVP realm must be on the same team
0x34 You already have maximum number of characters
0x35 You already have maximum number of characters
0x36 The server is currently queued
0x37 Only players who have characters on this realm..
0x38 Creation of that race requires an account that has been upgraded to the approciate expansion
0x39 Deleting character
0x3A Character deleted
0x3B Char deletion failed
0x3c Entering world of warcraft
0x3D Login successful
0x3E World server is down
0x3F A character with that name already exists
0x40 No instance server available
0x41 Login failed
0x42 Login for that race/class is disabled
0x43 Character not found
0x44 Enter a name for your character
0x45 Names must be atleast 2 characters long
0x46 Names must be no more then 12 characters
0x47 Names can only contain letters
0x48 Names must contain only one language
0x49 That name contains profanity
0x4A That name is unavailable
0x4B You cannot use an apostrophe
0x4C You can only have one apostrophe
0x4D You cannot use the same letter three times consecutively
0x4E You cannit use space as the first or last character of your name
0x4F <Blank>
0x50 Invalid character name
0x51 <Blank>
All further codes give the number in dec.
*/

void WorldSession::HandleCharDeleteOpcode( WorldPacket & recv_data )
{
	uint64 guid;
	recv_data >> guid;

	if(objmgr.GetPlayer((uint32)guid) != nullptr)
	{
		// "Char deletion failed"
		//fail = 0x3B;
	} else {

		PlayerInfo * inf = objmgr.GetPlayerInfo((uint32)guid);
		if( inf != nullptr && inf->m_loggedInPlayer == nullptr )
		{
			QueryResult * result = CharacterDatabase.Query("SELECT guildid, name FROM characters WHERE guid = %u", (uint32)guid, _accountId);
			if(!result)
				return;

			uint32 guildid = result->Fetch()[0].GetUInt32();
			string name = result->Fetch()[1].GetString();
			delete result;

			for(int i = 0; i < NUM_CHARTER_TYPES; ++i)
			{
				Charter * c = objmgr.GetCharterByGuid(guid, (CharterTypes)i);
				if(c != nullptr)
					c->RemoveSignature((uint32)guid);
			}

			
			/*if( _socket != nullptr )
				sPlrLog.write("Account: %s | IP: %s >> Deleted player %s", GetAccountName().c_str(), GetSocket()->GetRemoteIP().c_str(), name.c_str());*/
			
			sPlrLog.writefromsession(this, "deleted character %s (GUID: %u)", name.c_str(), (uint32)guid);

			CharacterDatabase.WaitExecute("DELETE FROM characters WHERE guid = %u", (uint32)guid);

			Corpse * c=objmgr.GetCorpseByOwner((uint32)guid);
			if(c)
				CharacterDatabase.Execute("DELETE FROM corpses WHERE guid = %u", c->GetLowGUID());

			CharacterDatabase.Execute("DELETE FROM playeritems WHERE ownerguid=%u",(uint32)guid);
			CharacterDatabase.Execute("DELETE FROM gm_tickets WHERE guid = %u", (uint32)guid);
			CharacterDatabase.Execute("DELETE FROM playerpets WHERE ownerguid = %u", (uint32)guid);
			CharacterDatabase.Execute("DELETE FROM playerpetspells WHERE ownerguid = %u", (uint32)guid);
			CharacterDatabase.Execute("DELETE FROM tutorials WHERE playerId = %u", (uint32)guid);
			CharacterDatabase.Execute("DELETE FROM questlog WHERE player_guid = %u", (uint32)guid);
			CharacterDatabase.Execute("DELETE FROM playercooldowns WHERE player_guid = %u", (uint32)guid);
			CharacterDatabase.Execute("DELETE FROM mailbox WHERE player_guid = %u", (uint32)guid);
			CharacterDatabase.Execute("DELETE FROM social_friends WHERE character_guid = %u OR friend_guid = %u", (uint32)guid, (uint32)guid);
			CharacterDatabase.Execute("DELETE FROM social_ignores WHERE character_guid = %u OR ignore_guid = %u", (uint32)guid, (uint32)guid);

			/* remove player info */
			objmgr.DeletePlayerInfo((uint32)guid);
		}
	}

    WorldPacket data(SMSG_CHAR_DELETE, 1);
    data << (uint8)CHAR_DELETE_SUCCESS;
	SendPacket( &data );
}

void WorldSession::HandleCharRenameOpcode(WorldPacket & recv_data)
{
	WorldPacket data(SMSG_CHAR_RENAME, recv_data.size() + 1);

	uint64 guid;
	string name;
	recv_data >> guid >> name;

	PlayerInfo * pi = objmgr.GetPlayerInfo((uint32)guid);
	if(pi == 0) return;

	QueryResult * result = CharacterDatabase.Query("SELECT forced_rename_pending FROM characters WHERE guid = %u AND acct = %u", 
		(uint32)guid, _accountId);
	if(result == 0)
	{
		delete result;
		return;
	}
	delete result;

	// Check name for rule violation.
	const char * szName=name.c_str();
	for(uint32 x=0;x<strlen(szName);x++)
	{
		if((int)szName[x]<65||((int)szName[x]>90&&(int)szName[x]<97)||(int)szName[x]>122)
		{
			data << uint8(0x32);
			data << guid << name;
			SendPacket(&data);
			return;
		}
	}

	QueryResult * result2 = CharacterDatabase.Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(name).c_str());
	if(result2)
	{
		if(result2->Fetch()[0].GetUInt32() > 0)
		{
			// That name is banned!
			data << uint8(0x31);
			data << guid << name;
			SendPacket(&data);
		}
		delete result2;
	}

	// Check if name is in use.
	if(objmgr.GetPlayerInfoByName(name.c_str()) != 0)
	{
		data << uint8(0x31);
		data << guid << name;
		SendPacket(&data);
		return;
	}

	// correct capitalization
	CapitalizeString(name);
	objmgr.RenamePlayerInfo(pi, pi->name, name.c_str());

	sPlrLog.writefromsession(this, "a rename was pending. renamed character %s (GUID: %u) to %s.", pi->name, pi->guid, name.c_str());

	// If we're here, the name is okay.
	free(pi->name);
	pi->name = strdup(name.c_str());
	CharacterDatabase.WaitExecute("UPDATE characters SET name = '%s' WHERE guid = %u", name.c_str(), (uint32)guid);
	CharacterDatabase.WaitExecute("UPDATE characters SET forced_rename_pending = 0 WHERE guid = %u", (uint32)guid);
	
	data << uint8(0) << guid << name;
	SendPacket(&data);
}


void WorldSession::HandlePlayerLoginOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 playerGuid = 0;

	sLog.outDebug( "WORLD: Recvd Player Logon Message" );

	recv_data >> playerGuid; // this is the GUID selected by the player
	if(objmgr.GetPlayer((uint32)playerGuid) != nullptr || m_loggingInPlayer || _player)
	{
		// A character with that name already exists 0x3E
		uint8 respons = CHAR_LOGIN_DUPLICATE_CHARACTER;
		OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &respons);
		return;
	}

	Player* plr = new Player((uint32)playerGuid);
	ASSERT(plr);
	plr->SetSession(this);
	m_bIsWLevelSet = false;
	
	Log.Debug("WorldSession", "Async loading player %u", (uint32)playerGuid);
	m_loggingInPlayer = plr;
	plr->LoadFromDB((uint32)playerGuid);
}

void WorldSession::FullLogin(Player * plr)
{
	Log.Debug("WorldSession", "Fully loading player %u", plr->GetLowGUID());
	SetPlayer(plr); 
	m_MoverWoWGuid.Init(plr->GetGUID());

	// copy to movement array
	movement_packet[0] = m_MoverWoWGuid.GetNewGuidMask();
	memcpy(&movement_packet[1], m_MoverWoWGuid.GetNewGuid(), m_MoverWoWGuid.GetNewGuidLen());

	/* world preload */
	packetSMSG_LOGIN_VERIFY_WORLD vwpck;
	vwpck.MapId = plr->GetMapId();
	vwpck.O = plr->GetOrientation();
	vwpck.X = plr->GetPositionX();
	vwpck.Y = plr->GetPositionY();
	vwpck.Z = plr->GetPositionZ();
	OutPacket( SMSG_LOGIN_VERIFY_WORLD, sizeof(packetSMSG_LOGIN_VERIFY_WORLD), &vwpck );


	plr->UpdateAttackSpeed();
	/*if(plr->getLevel()>70)
		plr->SetUInt32Value(UNIT_FIELD_LEVEL,70);*/

	// enable trigger cheat by default
	plr->triggerpass_cheat = HasGMPermissions();

	// Make sure our name exists (for premade system)
	PlayerInfo * info = objmgr.GetPlayerInfo(plr->GetLowGUID());
	if(info == 0)
	{
		info = new PlayerInfo;
		info->cl = plr->getClass();
		info->gender = plr->getGender();
		info->guid = plr->GetLowGUID();
		info->name = strdup(plr->GetName());
		info->lastLevel = plr->getLevel();
		info->lastOnline = UNIXTIME;
		info->lastZone = plr->GetZoneId();
		info->officerNote = nullptr;
		info->publicNote = nullptr;
		info->race = plr->getRace();
		info->team = plr->GetTeam();
		info->m_Group=0;
		info->subGroup=0;
		objmgr.AddPlayerInfo(info);
	}
	plr->m_playerInfo = info;

	info->m_loggedInPlayer = plr;

	WorldPacket data(SMSG_ACCOUNT_DATA_MD5, 128);

	MD5Hash md5hash;

	for (int i = 0; i < 8; i++)
	{
		AccountDataEntry* acct_data = GetAccountData(i);

		if (!acct_data->data)
		{
			data << uint64(0) << uint64(0);				// Nothing.
			continue;
		}
		md5hash.Initialize();
		md5hash.UpdateData((const uint8*)acct_data->data, acct_data->sz);
		md5hash.Finalize();

		data.append(md5hash.GetDigest(), MD5_DIGEST_LENGTH);

	}
	SendPacket(&data);

	// Set TIME OF LOGIN
	CharacterDatabase.Execute (
		"UPDATE characters SET online = 1 WHERE guid = %u" , plr->GetLowGUID());

	bool enter_world = true;
#ifndef CLUSTERING
	// Find our transporter and add us if we're on one.
	if(plr->m_TransporterGUID != 0)
	{
		Transporter * pTrans = objmgr.GetTransporter(Chrono::Util::GUID_LOPART(plr->m_TransporterGUID));
		if(pTrans)
		{
			if(plr->isDead())
			{
				plr->ResurrectPlayer();
				plr->SetUInt32Value(UNIT_FIELD_HEALTH, plr->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				plr->SetUInt32Value(UNIT_FIELD_POWER1, plr->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
			}

			float c_tposx = pTrans->GetPositionX() + plr->m_TransporterX;
			float c_tposy = pTrans->GetPositionY() + plr->m_TransporterY;
			float c_tposz = pTrans->GetPositionZ() + plr->m_TransporterZ;
			if(plr->GetMapId() != pTrans->GetMapId())	   // loaded wrong map
			{
				plr->SetMapId(pTrans->GetMapId());

				WorldPacket dataw(SMSG_NEW_WORLD, 20);
				dataw << pTrans->GetMapId() << c_tposx << c_tposy << c_tposz << plr->GetOrientation();
				SendPacket(&dataw);

				// shit is sent in worldport ack.
				enter_world = false;
			}

			plr->SetPosition(c_tposx, c_tposy, c_tposz, plr->GetOrientation(), false);
			plr->m_CurrentTransporter = pTrans;
			pTrans->AddPlayer(plr);
		}
	}
#endif

	Log.Debug("Login", "Player %s logged in.", plr->GetName());

	if(plr->GetTeam() == 1)
		sWorld.HordePlayers++;
	else
		sWorld.AlliancePlayers++;

	if(plr->m_FirstLogin && !HasGMPermissions())
	{
		plr->_InitialReputation();
		uint32 racecinematic = plr->myRace->cinematic_id;
#ifdef USING_BIG_ENDIAN
		swap32(&racecinematic);
#endif
		OutPacket(SMSG_TRIGGER_CINEMATIC, 4, &racecinematic);
	}

	sLog.outDetail( "WORLD: Created new player for existing players (%s)", plr->GetName() );

	// Login time, will be used for played time calc
	plr->m_playedtime[2] = (uint32)UNIXTIME;

	//Issue a message telling all guild members that this player has signed on
	if(plr->IsInGuild())
	{
		Guild *pGuild = objmgr.GetGuild(plr->GetGuildId());
		if(pGuild)
		{
			WorldPacket data(50);
			data.Initialize(SMSG_GUILD_EVENT);
			data << uint8(GUILD_EVENT_MOTD);
			data << uint8(0x01);
			data << pGuild->GetGuildMotd();
			SendPacket(&data);

			data.Initialize(SMSG_GUILD_EVENT);
			data << uint8(GUILD_EVENT_HASCOMEONLINE);
			data << uint8(0x01);
			data << plr->GetName();
			data << plr->GetGUID();
			pGuild->SendMessageToGuild(0, &data, G_MSGTYPE_ALL);
		}
	}

	// Send online status to people having this char in friendlist
	_player->Social_TellFriendsOnline();
	// send friend list (for ignores)
	_player->Social_SendFriendList(7);

	// Send MOTD
	_player->BroadcastMessage(sWorld.GetMotd());

	// Send revision (if enabled)
#ifdef WIN32
	_player->BroadcastMessage("Server: %sChronoEmu r%u/%s-Win-%s %s", MSG_COLOR_WHITE,
		BUILD_REVISION, CONFIG, ARCH, MSG_COLOR_LIGHTBLUE);		
#else
	_player->BroadcastMessage("Server: %sChronoEmu r%u/%s-%s %s", MSG_COLOR_WHITE,
		BUILD_REVISION, PLATFORM_TEXT, ARCH, MSG_COLOR_LIGHTBLUE);
#endif

	if(sWorld.SendStatsOnJoin)
	{
		_player->BroadcastMessage("Online Players: %s%u |rPeak: %s%u|r Accepted Connections: %s%u",
			MSG_COLOR_WHITE, sWorld.GetSessionCount(), MSG_COLOR_WHITE, sWorld.PeakSessionCount, MSG_COLOR_WHITE, sWorld.mAcceptedConnections);
	}

	//Set current RestState
	if( plr->m_isResting) 		// We are resting at an inn , turn on Zzz
		plr->ApplyPlayerRestState(true);

	//Calculate rest bonus if there is time between lastlogoff and now
	if( plr->m_timeLogoff > 0 && plr->GetUInt32Value(UNIT_FIELD_LEVEL) < 60)	// if timelogoff = 0 then it's the first login 
	{
		uint32 currenttime = (uint32)UNIXTIME;
		uint32 timediff = currenttime - plr->m_timeLogoff;

		//Calculate rest bonus
		if( timediff > 0 ) 
			plr->AddCalculatedRestXP(timediff);
	}

#ifdef CLUSTERING
	plr->SetInstanceID(forced_instance_id);
	plr->SetMapId(forced_map_id);
#else
	sHookInterface.OnEnterWorld2(_player);
#endif

	if(info->m_Group)
		info->m_Group->Update();

	if(enter_world && !_player->GetMapMgr())
	{
		plr->AddToWorld();
	}

	objmgr.AddPlayer(_player);
}

bool ChatHandler::HandleRenameCommand(const char * args, WorldSession * m_session)
{
	// prevent buffer overflow
	if(strlen(args) > 100)
		return false;

	char name1[100];
	char name2[100];

	if(sscanf(args, "%s %s", name1, name2) != 2)
		return false;

	if(VerifyName(name2, strlen(name2)) == false)
	{
		RedSystemMessage(m_session, "That name is invalid or contains invalid characters.");
		return true;
	}

	string new_name = name2;
	PlayerInfo * pi = objmgr.GetPlayerInfoByName(name1);
	if(pi == 0)
	{
		RedSystemMessage(m_session, "Player not found with this name.");
		return true;
	}

	if( objmgr.GetPlayerInfoByName(new_name.c_str()) != nullptr )
	{
		RedSystemMessage(m_session, "Player found with this name in use already.");
		return true;
	}

	objmgr.RenamePlayerInfo(pi, pi->name, new_name.c_str());

	free(pi->name);
	pi->name = strdup(new_name.c_str());

	// look in world for him
	Player * plr = objmgr.GetPlayer(pi->guid);
	if(plr != 0)
	{
		plr->SetName(new_name);
		BlueSystemMessageToPlr(plr, "%s changed your name to '%s'.", m_session->GetPlayer()->GetName(), new_name.c_str());
		plr->SaveToDB(false);
	}
	else
	{
		CharacterDatabase.WaitExecute("UPDATE characters SET name = '%s' WHERE guid = %u", CharacterDatabase.EscapeString(new_name).c_str(), (uint32)pi->guid);
	}

	GreenSystemMessage(m_session, "Changed name of '%s' to '%s'.", name1, name2);
	sGMLog.writefromsession(m_session, "renamed character %s (GUID: %u) to %s", name1, pi->guid, name2);
	sPlrLog.writefromsession(m_session, "GM renamed character %s (GUID: %u) to %s", name1, pi->guid, name2);
	return true;
}
