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


initialiseSingleton(CBattlegroundManager);
typedef CBattleground*(*CreateBattlegroundFunc)( MapMgr* mgr,uint32 iid,uint32 group, uint32 type);

const static uint32 BGMapIds[BATTLEGROUND_NUM_TYPES] = {
	0,		// 0
	30,		// AV
	489,	// WSG
	529,	// AB
};

const static CreateBattlegroundFunc BGCFuncs[BATTLEGROUND_NUM_TYPES] = {
	nullptr,							// 0
	&AlteracValley::Create,			// AV
	&WarsongGulch::Create,			// WSG
	&ArathiBasin::Create,			// AB
};

const static uint32 BGMinimumPlayers[BATTLEGROUND_NUM_TYPES] = {
	0,							// 0
	20,							// AV
	5,							// WSG
	7,							// AB
};

const static uint32 BGPvPDataFieldCount[BATTLEGROUND_NUM_TYPES] = {
	0,							// NONE
	5,							// AV
	2,							// WSG
	2,							// AB
};

CBattlegroundManager::CBattlegroundManager() : EventableObject()
{
	int i, j;

	m_maxBattlegroundId = 0;

	for (i=0; i<BATTLEGROUND_NUM_TYPES; i++)
	{
		m_instances[i].clear();
		m_queuedGroups[i].clear();

		for (j=0; j<MAX_LEVEL_GROUP; j++)
		{
			m_queuedPlayers[i][j].clear();
		}
	}

	memset(m_averageQueueTimes, 0, sizeof(uint32)*BATTLEGROUND_NUM_TYPES*10);
}

void CBattlegroundManager::Init()
{
	sEventMgr.AddEvent(this, &CBattlegroundManager::EventQueueUpdate, false, EVENT_BATTLEGROUNDMGR_QUEUE_UPDATE, 5000, 0,0);
}

CBattlegroundManager::~CBattlegroundManager()
{
	// Moved from Master.cpp
	Log.Notice("BattlegroundMgr", "~BattlegroundMgr()");
}

// Returns the average queue time for a bg type, using last 10 players queued
uint32 CBattlegroundManager::GetAverageQueueTime(uint32 BgType)
{
	uint32 avg;
	int i;

	assert(BgType < BATTLEGROUND_NUM_TYPES);

	avg = m_averageQueueTimes[BgType][0];
	for (i=1; i<10; i++)
	{
		if (m_averageQueueTimes[BgType][i] == 0) break;

		avg = (avg + m_averageQueueTimes[BgType][i])/2;
	}

	return avg;
}

void CBattlegroundManager::AddAverageQueueTime(uint32 BgType, uint32 queueTime)
{
	int i;

	assert(BgType < BATTLEGROUND_NUM_TYPES);

	// move the queue times one place in the array
	for (i=0; i<9; i++)
	{
		m_averageQueueTimes[BgType][i+1] = m_averageQueueTimes[BgType][i];
	}

	// add the new queueTime in the first slot
	m_averageQueueTimes[BgType][0] = queueTime;
}

void CBattlegroundManager::HandleBattlegroundListPacket(WorldSession * m_session, uint32 BattlegroundType, bool battlemaster)
{
	if( BattlegroundType >= BATTLEGROUND_NUM_TYPES )
		return;

	uint32 LevelGroup = GetLevelGrouping(m_session->GetPlayer()->getLevel());
	uint32 Count = 0;
	uint32 mapid = 0;
	WorldPacket data(SMSG_BATTLEFIELD_LIST, 200);
	data << m_session->GetPlayer()->GetGUID();
	data << uint32(GetBattleGroundMapIdByType(BattlegroundType));	data << uint8(0);	// unk
	size_t CountPos = data.wpos();
	data << uint32(0);		// Count, will be replaced later

	/* Append the battlegrounds */
	m_instanceLock.Acquire();
	for(map<uint32, CBattleground* >::iterator itr = m_instances[BattlegroundType].begin(); itr != m_instances[BattlegroundType].end(); ++itr)
	{
		if( itr->second->GetLevelGroup() == LevelGroup  && !itr->second->HasEnded() )
		{
			data << itr->first;
			++Count;
		}
	}
	m_instanceLock.Release();
	*(uint32*)&data.contents()[13] = Count;
	m_session->SendPacket(&data);
}

void CBattlegroundManager::HandleBattlegroundJoin(WorldSession * m_session, WorldPacket & pck)
{
	uint64 guid;
	uint32 bgtype;
	uint32 instance;
	uint32 mapId;
	uint8 joinasgroup; //0x01 = Group and 0x00 = Player

	Player* plr = m_session->GetPlayer();
	uint32 pguid = plr->GetLowGUID();
	uint32 lgroup = GetLevelGrouping(plr->getLevel());

	pck >> guid >> mapId >> instance >> joinasgroup;
	
	bgtype = (GetBattleGroundTypeByMapId(mapId));
	
	if(bgtype >= BATTLEGROUND_NUM_TYPES || BGMapIds[bgtype] == 0)
	{
		m_session->Disconnect();
		return;		// cheater!
	}

	if( joinasgroup && m_session->GetPlayer()->GetGroup() == nullptr ) 
	{
		//Correct? is there any message here at blizz?
		sChatHandler.RedSystemMessage( m_session, "You are not in a Group." );
		return;
	}

	/* Check the instance id */
	if(instance)
	{
		/* We haven't picked the first instance. This means we've specified an instance to join. */
		m_instanceLock.Acquire();
		map<uint32, CBattleground* >::iterator itr = m_instances[bgtype].find(instance);

		if(itr == m_instances[bgtype].end())
		{
			sChatHandler.SystemMessage(m_session, "You have tried to join an invalid instance id.");
			m_instanceLock.Release();
			return;
		}

		m_instanceLock.Release();
	}

	/* Queue him! */
	m_queueLock.Acquire();
	if( !plr->HasBattlegroundQueueSlot() )
	{
		plr->BroadcastMessage("You are in too many queues already.");
		m_queueLock.Release();
		return;
	}

	if( joinasgroup && m_session->GetPlayer()->GetGroup())
	{
		m_session->GetPlayer()->GetGroup()->m_isqueued = true;
		Log.Success("BattlegroundManager", "Group %u is now in battleground queue for instance %u", m_session->GetPlayer()->GetGroupID(), instance );
	}
	else
		Log.Success("BattlegroundManager", "Player %u is now in battleground queue for instance %u", m_session->GetPlayer()->GetLowGUID(), instance );


	/* send the battleground status packet */
	uint32 queueSlot = plr->GetBGQueueSlot();
	if(queueSlot == 0xFFFFFFFF)
	{
		sLog.outDebug("WARNING: queueSlot is 0xFFFFFFFF in %s\n", __FUNCTION__);
		m_queueLock.Release();
		return;
	}

	if( plr->HasBGQueueSlotOfType( bgtype ) != 4)
		queueSlot = plr->HasBGQueueSlotOfType( bgtype );

	if( queueSlot >= 3 )
	{
		m_queueLock.Release();
		return;
	}

	if( plr->m_pendingBattleground[ queueSlot ] )
		plr->m_pendingBattleground[ queueSlot ]->RemovePendingPlayer(plr);

	if( plr->m_bg && plr->m_bg->GetType() == bgtype )
	{
		sLog.outDebug("Player has queued from already in a BG for itself.");
		m_queueLock.Release();
		return;
	}

	//Log.Notice("BattlegroundManager", "Chose queue slot %u for HandleBattlegroundJoin", queueSlot);
	plr->m_bgIsQueued[queueSlot] = true;
	plr->m_bgQueueInstanceId[queueSlot] = instance;
	plr->m_bgQueueType[queueSlot] = bgtype;
	plr->m_bgQueueTime[queueSlot] = (uint32)UNIXTIME;

	/* Set battleground entry point */
	plr->m_bgEntryPointX = plr->GetPositionX();
	plr->m_bgEntryPointY = plr->GetPositionY();
	plr->m_bgEntryPointZ = plr->GetPositionZ();
	plr->m_bgEntryPointMap = plr->GetMapId();
	plr->m_bgEntryPointInstance = plr->GetInstanceID();

	m_queuedPlayers[bgtype][lgroup].push_back(pguid);

	m_queueLock.Release();

	SendBattlegroundQueueStatus( plr, queueSlot);

	/* We will get updated next few seconds =) */
}

void ErasePlayerFromList(uint32 guid, list<uint32>* l)
{
	for(list<uint32>::iterator itr = l->begin(); itr != l->end(); ++itr)
	{
		if((*itr) == guid)
		{
			l->erase(itr);
			return;
		}
	}
}

void CBattlegroundManager::AddPlayerToBg(CBattleground* bg, deque<Player*  > *playerVec, uint32 i, uint32 j)
{
	Player* plr = *playerVec->begin();
	playerVec->pop_front();
	if(bg->CanPlayerJoin(plr))
	{
		bg->AddPlayer(plr, plr->GetTeam());
		ErasePlayerFromList(plr->GetLowGUID(), &m_queuedPlayers[i][j]);
	}
	else 
	{
		// Put again the player in the queue
		playerVec->push_back(plr);
	}
}

void CBattlegroundManager::AddPlayerToBgTeam(CBattleground* bg, deque<Player*  > *playerVec, uint32 i, uint32 j, int Team)
{
	if (bg->HasFreeSlots(Team))
	{
		Player* plr = *playerVec->begin();
		playerVec->pop_front();
		plr->m_bgTeam=Team;
		bg->AddPlayer(plr, Team);
		ErasePlayerFromList(plr->GetLowGUID(), &m_queuedPlayers[i][j]);
	}
}

void CBattlegroundManager::EventQueueUpdate(bool forceStart)
{
	deque<Player*  > tempPlayerVec[2];
	uint32 i,j,k;
	Player* plr;
	CBattleground* bg;
	list<uint32>::iterator it3, it4;
	//vector<Player*  >::iterator it6;
	map<uint32, CBattleground* >::iterator iitr;
	m_queueLock.Acquire();
	m_instanceLock.Acquire();

	for(i = 0; i < BATTLEGROUND_NUM_TYPES; ++i)
	{
		for(j = 0; j < MAX_LEVEL_GROUP; ++j)
		{
			if(!m_queuedPlayers[i][j].size())
				continue;

			tempPlayerVec[0].clear();
			tempPlayerVec[1].clear();

			for(it3 = m_queuedPlayers[i][j].begin(); it3 != m_queuedPlayers[i][j].end();)
			{
				it4 = it3++;
                plr = objmgr.GetPlayer(*it4);
				
				if(!plr || GetLevelGrouping(plr->getLevel()) != j)
				{
                    m_queuedPlayers[i][j].erase(it4);
					continue;
				}

				uint32 queueSlot = plr->GetBGQueueSlotByBGType(i);
				if(queueSlot >= 3) continue;

				if( !plr->m_bgIsQueued[queueSlot] )
				{
					// We've since cancelled our queue
					m_queuedPlayers[i][j].erase(it4);
					continue;
				}

				// queued to a specific instance id?
				if(plr->m_bgQueueInstanceId[queueSlot] != 0 && plr->m_bgIsQueued[queueSlot])
				{
					if( m_instances[i].empty() )
						continue;
					
					iitr = m_instances[i].find( plr->m_bgQueueInstanceId[queueSlot] );
					if(iitr == m_instances[i].end())
					{
						// queue no longer valid
						//plr->GetSession()->SystemMessage("Your queue on battleground instance id %u is no longer valid. Reason: Instance Deleted.", plr->m_bgQueueInstanceId[queueSlot]);
						plr->RemoveFromBattlegroundQueue(queueSlot);
						//m_queuedPlayers[i][j].erase(it4);
						SendBattlegroundQueueStatus(plr, queueSlot);
						continue;
					}

					// can we join?
					bg = (*iitr).second;
					if(bg->CanPlayerJoin(plr))
					{
						bg->AddPlayer(plr, plr->GetTeam());
						m_queuedPlayers[i][j].erase(it4);
					}
				}
				else
				{
					tempPlayerVec[plr->GetTeam()].push_back(plr);
				}
			}

			// try to join existing instances
			for(iitr = m_instances[i].begin(); iitr != m_instances[i].end(); ++iitr)
			{
				if( iitr->second->HasEnded() || iitr->second->GetLevelGroup() != j)
					continue;

				bg = iitr->second;
				if(bg)
				{
					int size = (int)min(tempPlayerVec[0].size(),tempPlayerVec[1].size());

					for(int counter = 0; (counter < size) && (bg->IsFull() == false); counter++)
					{
						AddPlayerToBgTeam(bg, &tempPlayerVec[0], i, j, 0);
						AddPlayerToBgTeam(bg, &tempPlayerVec[1], i, j, 1);
					}

					while (tempPlayerVec[0].size() > 0 && bg->HasFreeSlots(0))
					{
						AddPlayerToBgTeam(bg, &tempPlayerVec[0], i, j, 0);
					}
					
					while (tempPlayerVec[1].size() > 0 && bg->HasFreeSlots(1))
					{
						AddPlayerToBgTeam(bg, &tempPlayerVec[1], i, j, 1);
					}
				}
			}
			if((forceStart == true &&
			    (tempPlayerVec[0].size() >= 1 ||
				tempPlayerVec[1].size() >= 1)) ||
			    tempPlayerVec[0].size() >= BGMinimumPlayers[i] &&
				tempPlayerVec[1].size() >= BGMinimumPlayers[i])
			{
				if(CanCreateInstance(i,j))
				{
					bg = CreateInstance(i,j);
					if( bg == nullptr )
					{
						// creation failed
						for(k = 0; k < 2; ++k)
						{
							while(tempPlayerVec[k].size())
							{
								plr = *tempPlayerVec[k].begin();
								tempPlayerVec[k].pop_front();
								ErasePlayerFromList(plr->GetLowGUID(), &m_queuedPlayers[i][j]);
							}
						}
					}
					else
					{						
						// push as many as possible in
						if (forceStart)
						{
							for(k = 0; k < 2; ++k)
							{
								while(tempPlayerVec[k].size() && bg->HasFreeSlots(k))
								{
									AddPlayerToBgTeam(bg, &tempPlayerVec[k], i, j, k);
								}
							}
						}
						else
						{
							int size = (int)min(tempPlayerVec[0].size(),tempPlayerVec[1].size());

							for(int counter = 0; (counter < size) && (bg->IsFull() == false); counter++)
							{
								AddPlayerToBgTeam(bg, &tempPlayerVec[0], i, j, 0);
								AddPlayerToBgTeam(bg, &tempPlayerVec[1], i, j, 1);
							}
						}
					}
				}
			}
		}
	}

	m_queueLock.Release();
	m_instanceLock.Release();
}

void CBattlegroundManager::RemovePlayerFromQueues(Player* plr)
{
	m_queueLock.Acquire();

	uint32 lgroup = GetLevelGrouping(plr->getLevel());
	
	uint32 i;
	for(i = 0; i < 3; ++i)
	{
		list<uint32>::iterator itr2;
		list<uint32>::iterator itr = m_queuedPlayers[plr->m_bgQueueType[i]][lgroup].begin();
		while(itr != m_queuedPlayers[plr->m_bgQueueType[i]][lgroup].end())
		{
			itr2 = itr;
			itr2++;
			if((*itr) == plr->GetLowGUID())
			{
				DEBUG_LOG("BattlegroundManager", "Removing player %u from queue instance %u type %u", plr->GetLowGUID(), plr->m_bgQueueInstanceId[i], plr->m_bgQueueType[i]);
				m_queuedPlayers[plr->m_bgQueueType[i]][lgroup].erase(itr);
			}

			itr = itr2;
		}
		plr->m_bgIsQueued[i] = false;
		plr->m_bgTeam=plr->GetTeam();
		plr->m_pendingBattleground[i]=NULLBATTLEGROUND;
		plr->m_bgQueueType[i] = 0;
		plr->m_bgQueueInstanceId[i] = 0;
		plr->m_bgQueueTime[i] = 0;
	}
    m_queueLock.Release();
}

void CBattlegroundManager::RemoveGroupFromQueues(Group * grp)
{
	m_queueLock.Acquire();
	for(GroupMembersSet::iterator itr = grp->GetSubGroup(0)->GetGroupMembersBegin(); itr != grp->GetSubGroup(0)->GetGroupMembersEnd(); ++itr)
		if((*itr)->m_loggedInPlayer)
		{
			for(uint32 i = 0; i < 2; ++i)
			{
				(*itr)->m_loggedInPlayer->m_bgIsQueued[i] = false;
				SendBattlegroundQueueStatus((*itr)->m_loggedInPlayer, 0);
			}
		}

	grp->m_isqueued = false;
	m_queueLock.Release();
}


bool CBattlegroundManager::CanCreateInstance(uint32 Type, uint32 LevelGroup)
{
	/*uint32 lc = 0;
	for(map<uint32, CBattleground* >::iterator itr = m_instances[Type].begin(); itr != m_instances[Type].end(); ++itr)
	{
		if(itr->second->GetLevelGroup() == LevelGroup)
		{
			lc++;
			if(lc >= MAXIMUM_BATTLEGROUNDS_PER_LEVEL_GROUP)
				return false;
		}
	}*/

	return true;
}

CBattleground::CBattleground( MapMgr* mgr, uint32 id, uint32 levelgroup, uint32 type) : m_mapMgr(mgr), m_id(id), m_type(type), m_levelGroup(levelgroup)
{
	m_nextPvPUpdateTime = 0;
	m_countdownStage = 0;
	m_ended = false;
	m_started = false;
	m_losingteam = 0xff;
	m_startTime = (uint32)UNIXTIME;
	m_lastResurrect = (uint32)UNIXTIME;

	/* create raid groups */
	for(uint32 i = 0; i < 2; ++i)
	{
		m_groups[i] = new Group(true);
		m_groups[i]->SetFlag(GROUP_FLAG_DONT_DISBAND_WITH_NO_MEMBERS | GROUP_FLAG_BATTLEGROUND_GROUP);
		m_groups[i]->ExpandToRaid();
	}
}

void CBattleground::Init()
{
	sEventMgr.AddEvent(this, &CBattleground::EventResurrectPlayers, EVENT_BATTLEGROUNDMGR_QUEUE_UPDATE, 30000, 0,0);
}

CBattleground::~CBattleground()
{
	sEventMgr.RemoveEvents(this);
	for(uint32 i = 0; i < 2; ++i)
	{
		PlayerInfo *inf;
		for(uint32 j = 0; j < m_groups[i]->GetSubGroupCount(); ++j) {
			for(GroupMembersSet::iterator itr = m_groups[i]->GetSubGroup(j)->GetGroupMembersBegin(); itr != m_groups[i]->GetSubGroup(j)->GetGroupMembersEnd();) {
				inf = (*itr);
				++itr;
				m_groups[i]->RemovePlayer(inf);
			}
		}
		delete m_groups[i];
	}
}

void CBattleground::UpdatePvPData()
{
	if(UNIXTIME >= m_nextPvPUpdateTime)
	{
		WorldPacket data(10*(m_players[0].size()+m_players[1].size())+50);
		BuildPvPUpdateDataPacket(&data);
		DistributePacketToAll(&data);

		m_nextPvPUpdateTime = UNIXTIME + 2;
	}
}

void CBattleground::BuildPvPUpdateDataPacket(WorldPacket * data)
{
	data->Initialize(MSG_PVP_LOG_DATA);
	data->reserve(10*(m_players[0].size()+m_players[1].size())+50);

	BGScore * bs;

	if(m_ended)
	{
		*data << uint8(1);
		*data << uint8(m_losingteam);
	}
	else
		*data << uint8(0);		// If the game has ended - this will be 1

	size_t pos = data->wpos();
	*data << uint32(0); //will be set to correct number later //uint32(m_players[0].size() + m_players[1].size());

	uint32 count = 0;
	uint32 fcount = BGPvPDataFieldCount[GetType()];
	for(uint32 i = 0; i < 2; ++i)
	{
		for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
		{
			if( (*itr)->m_isGmInvisible ) continue;
			*data << (*itr)->GetGUID();
			bs = &(*itr)->m_bgScore;

			*data << uint32(0); // Player Rank
			*data << bs->KillingBlows;
			*data << bs->HonorableKills;
			*data << bs->Deaths;
			*data << bs->BonusHonor;
			//*data << uint32(0x2);
			//*data << bs->Misc1;
			//*data << bs->Misc2;
			*data << fcount;	// count of values after this
			for(uint32 j = 0; j < fcount; ++j)
				*data << bs->MiscData[j];

			count++;
		}
	}
	// Have to set correct number of players sent in log since we skip invisible GMs
	*(uint32*)&data->contents()[pos] = count;
}

void CBattleground::AddPlayer(Player* plr, uint32 team)
{
	m_mainLock.Acquire();

	/* This is called when the player is added, not when they port. So, they're essentially still queued, but not inside the bg yet */
	m_pendPlayers[team].insert(plr->GetLowGUID());

	uint32 queueSlot = plr->GetBGQueueSlotByBGType(GetType());
	if(queueSlot >= 3)
	{
		m_mainLock.Release();
		return;
	}

	/* Add an event to remove them in 2 minutes time. */
	sEventMgr.AddEvent(plr, &Player::RemoveFromBattlegroundQueue, queueSlot, true, EVENT_BATTLEGROUND_QUEUE_UPDATE_SLOT_1 + queueSlot, 120000, 1,0);

	plr->m_pendingBattleground[queueSlot] = this;
	plr->m_bgIsQueued[queueSlot] = false;

	/* Send a packet telling them that they can enter */
	BattlegroundManager.SendBattlegroundQueueStatus(plr, queueSlot);

	/* Add the queue time */
	BattlegroundManager.AddAverageQueueTime(GetType(), (uint32)(UNIXTIME-plr->m_bgQueueTime[queueSlot]));

	m_mainLock.Release();
}

void CBattleground::RemovePendingPlayer(Player* plr)
{
	m_mainLock.Acquire();
	m_pendPlayers[0].erase(plr->GetLowGUID());
	m_pendPlayers[1].erase(plr->GetLowGUID());

	/* send a nullptr bg update (so they don't join) */
	for(uint32 i = 0; i < 3; ++i)
	{
		if( plr->m_pendingBattleground[i] && 
			plr->m_pendingBattleground[i] == this )
		{
			plr->m_pendingBattleground[i] = NULLBATTLEGROUND;
			BattlegroundManager.SendBattlegroundQueueStatus(plr, i);
			break;
		}
	}
	plr->m_bgTeam=plr->GetTeam();

	m_mainLock.Release();
}

void CBattleground::OnPlayerPushed(Player* plr)
{
	if( plr->GetGroup() )
		plr->GetGroup()->RemovePlayer(plr->m_playerInfo);

	plr->ProcessPendingUpdates();
	
	if( plr->GetGroup() == nullptr && !plr->m_isGmInvisible )
		m_groups[plr->m_bgTeam]->AddMember( plr->m_playerInfo );
}

void CBattleground::PortPlayer(Player* plr, bool skip_teleport /* = false*/)
{
	m_mainLock.Acquire();
	if(m_ended)
	{
		for(uint32 i = 0; i < 3; ++i)
		{
			if( plr->m_pendingBattleground[i] == this )
			{
				plr->m_pendingBattleground[i] = NULLBATTLEGROUND;
				plr->m_bgIsQueued[i] = false;
			}
		}
		sChatHandler.SystemMessage(plr->GetSession(), "You cannot join this battleground as it has already ended.");
		BattlegroundManager.SendBattlegroundQueueStatus(plr, 0);
		BattlegroundManager.SendBattlegroundQueueStatus(plr, 1);
		BattlegroundManager.SendBattlegroundQueueStatus(plr, 2);
		m_mainLock.Release();
		return;
	}

	m_pendPlayers[0].erase(plr->GetLowGUID());
	m_pendPlayers[1].erase(plr->GetLowGUID());
	if(m_players[plr->m_bgTeam].find(plr) != m_players[plr->m_bgTeam].end())
	{
		m_mainLock.Release();
		return;
	}

	if( plr->m_bg != nullptr )
	{
		plr->m_bg->RemovePlayer(plr, true);		// don't bother porting them out, we're about to port them anyway
		plr->m_bg = NULLBATTLEGROUND;
	}

	plr->FullHPMP();
	plr->SetTeam(plr->m_bgTeam);
	if( !plr->m_isGmInvisible )
	{
		WorldPacket data(SMSG_BATTLEGROUND_PLAYER_JOINED, 8);
		data << plr->GetGUID();
		for(set<Player*  >::iterator itr = m_players[plr->m_bgTeam].begin(); itr != m_players[plr->m_bgTeam].end(); ++itr)
			(*itr)->GetSession()->SendPacket(&data);	}

	m_players[plr->m_bgTeam].insert(plr);

	if( !skip_teleport )
	{
		if( plr->IsInWorld() )
			plr->RemoveFromWorld();
	}

	for(uint32 i = 0; i < 3; ++i)
	{
		if( plr->m_pendingBattleground[i] == this )
		{
			plr->m_pendingBattleground[i] = NULLBATTLEGROUND;
			plr->m_bgSlot = i;
			BattlegroundManager.SendBattlegroundQueueStatus(plr, i);
		}
	}

	/* remove from any auto queue remove events */
	sEventMgr.RemoveEvents(plr, EVENT_BATTLEGROUND_QUEUE_UPDATE_SLOT_1 + plr->m_bgSlot);
	
	if(!plr->IsPvPFlagged())
		plr->SetPvPFlag();

	/* Reset the score */
	memset(&plr->m_bgScore, 0, sizeof(BGScore));

	/* update pvp data */
	UpdatePvPData();
	
	/* send him the world states */
	SendWorldStates(plr);

	/* add the player to the group */
	if(plr->GetGroup())
	{
		// remove them from their group
		plr->GetGroup()->RemovePlayer( plr->m_playerInfo );
	}

	if(!m_countdownStage)
	{
		m_countdownStage = 1;
		sEventMgr.AddEvent(this, &CBattleground::EventCountdown, EVENT_BATTLEGROUND_COUNTDOWN, 30000, 0,0);
		sEventMgr.ModifyEventTimeLeft(this, EVENT_BATTLEGROUND_COUNTDOWN, 10000);
	}

	plr->m_bg = this;

	sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);
	if(!skip_teleport)
	{
		/* This is where we actually teleport the player to the battleground. */	
		plr->SafeTeleport(m_mapMgr,GetStartingCoords(plr->m_bgTeam));
	}


	m_mainLock.Release();
}

CBattleground* CBattlegroundManager::CreateInstance(uint32 Type, uint32 LevelGroup)
{
	CreateBattlegroundFunc cfunc = BGCFuncs[Type];
	MapMgr* mgr = NULLMAPMGR;
	CBattleground* bg = nullptr;
	bool isWeekend = false;
	struct tm tm;
	time_t t;
	uint32 iid;
	int n;

	if(cfunc == nullptr)
	{
		Log.Error("BattlegroundManager", "Could not find CreateBattlegroundFunc pointer for type %u level group %u", Type, LevelGroup);
		return NULLBATTLEGROUND;
	}

	/* Create Map Manager */
	mgr = sInstanceMgr.CreateBattlegroundInstance(BGMapIds[Type]);
	if(mgr == nullptr)
	{
		Log.Error("BattlegroundManager", "CreateInstance() call failed for map %u, type %u, level group %u", BGMapIds[Type], Type, LevelGroup);
		return NULLBATTLEGROUND;		// Shouldn't happen
	}

	t = time(nullptr);
#ifdef WIN32
	tm = *localtime(&t);
#else
	localtime_r(&t, &tm);
#endif

	switch (Type)
	{
		case BATTLEGROUND_WARSONG_GULCH: n = 0; break;
		case BATTLEGROUND_ARATHI_BASIN: n = 1; break;
		case BATTLEGROUND_ALTERAC_VALLEY: n = 2; break;
		default: n = 0; break;
	}
	if (((tm.tm_yday / 7) % 4) == n)
	{
		/* Set weekend from thursday night at midnight until tuesday morning */
		isWeekend = tm.tm_wday >= 5 || tm.tm_wday < 2;
	}

	/* Call the create function */
	iid = ++m_maxBattlegroundId;
	bg = cfunc(mgr, iid, LevelGroup, Type);	
	bg->Init();
	bg->SetIsWeekend(isWeekend);
	mgr->m_battleground = bg;
	sEventMgr.AddEvent(bg, &CBattleground::EventCreate, EVENT_BATTLEGROUNDMGR_QUEUE_UPDATE, 1, 1,0);
	Log.Success("BattlegroundManager", "Created battleground type %u for level group %u.", Type, LevelGroup);

	m_instanceLock.Acquire();
	m_instances[Type].insert( make_pair(iid, bg) );
	m_instanceLock.Release();

	return bg;
}

void CBattlegroundManager::DeleteBattleground(CBattleground* bg)
{
	uint32 i = bg->GetType();
	uint32 j = bg->GetLevelGroup();
	Player* plr;

	m_instanceLock.Acquire();
	m_queueLock.Acquire();
	m_instances[i].erase(bg->GetId());
	
	/* erase any queued players */
	list<uint32>::iterator itr = m_queuedPlayers[i][j].begin();
	list<uint32>::iterator it2;
	for(; itr != m_queuedPlayers[i][j].end();)
	{
		it2 = itr++;
		plr = objmgr.GetPlayer(*it2);
		if(!plr)
		{
			m_queuedPlayers[i][j].erase(it2);
			continue;
		}

		for(uint32 z= 0; z < 3; ++z)
		{
			if(plr && plr->m_bgQueueInstanceId[z] == bg->GetId())
			{
				sChatHandler.SystemMessageToPlr(plr, "Your queue on battleground instance %u is no longer valid, the instance no longer exists.", bg->GetId());
				SendBattlegroundQueueStatus(plr, z);
				plr->m_bgIsQueued[z] = false;
				m_queuedPlayers[i][j].erase(it2);
			}
		}
	}

	m_queueLock.Release();
	m_instanceLock.Release();

}

GameObject* CBattleground::SpawnGameObject(uint32 entry,float x, float y, float z, float o, uint32 flags, uint32 faction, float scale)
{
	GameObject *go = m_mapMgr->CreateGameObject(entry);
	if(go == nullptr || !go->CreateFromProto(entry, m_mapMgr->GetMapId(), x, y, z, o))
		return NULLGOB;

	go->SetUInt32Value(GAMEOBJECT_FACTION,faction);
	go->SetFloatValue(OBJECT_FIELD_SCALE_X,scale);	
	go->SetUInt32Value(GAMEOBJECT_FLAGS, flags);
	go->SetFloatValue(GAMEOBJECT_POS_X, x);
	go->SetFloatValue(GAMEOBJECT_POS_Y, y);
	go->SetFloatValue(GAMEOBJECT_POS_Z, z);
	go->SetFloatValue(GAMEOBJECT_FACING, o);
	go->SetInstanceID(m_mapMgr->GetInstanceID());
	go->m_battleground = this;

	return go;
}

Creature* CBattleground::SpawnCreature(uint32 entry,float x, float y, float z, float o)
{
	CreatureProto *cp = CreatureProtoStorage.LookupEntry(entry);
	CreatureInfo *ci = CreatureNameStorage.LookupEntry(entry);
	Creature* c = NULLCREATURE;
	if (cp && ci)
	{
		c = m_mapMgr->CreateCreature(entry);
		if (c != NULLCREATURE)
		{
			c->Load(cp,x, y, z);
			c->PushToWorld(m_mapMgr);
		}
	}
	return c;
}

void CBattleground::SendChatMessage(uint32 Type, uint64 Guid, const char * Format, ...)
{
	char msg[500];
	va_list ap;
	va_start(ap, Format);
	vsnprintf(msg, 500, Format, ap);
	va_end(ap);
	WorldPacket * data = sChatHandler.FillMessageData(Type, 0, msg, Guid, 0);
	DistributePacketToAll(data);
	delete data;
}

void CBattleground::DistributePacketToAll(WorldPacket * packet)
{
	m_mainLock.Acquire();
	for(int i = 0; i < 2; ++i)
	{
		if( !m_players[i].size() )
			continue;

		for(set<Player*  >::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			(*itr)->GetSession()->SendPacket(packet);
	}
	m_mainLock.Release();
}

void CBattleground::DistributePacketToTeam(WorldPacket * packet, uint32 Team)
{
	m_mainLock.Acquire();
	if( !m_players[Team].size() )
	{
		m_mainLock.Release();
		return;
	}
	for(set<Player*  >::iterator itr = m_players[Team].begin(); itr != m_players[Team].end(); ++itr)
		(*itr)->GetSession()->SendPacket(packet);
	m_mainLock.Release();
}

void CBattleground::PlaySoundToAll(uint32 Sound)
{
	WorldPacket data(SMSG_PLAY_SOUND, 4);
	data << Sound;
	DistributePacketToAll(&data);
}

void CBattleground::PlaySoundToTeam(uint32 Team, uint32 Sound)
{
	WorldPacket data(SMSG_PLAY_SOUND, 4);
	data << Sound;
	DistributePacketToTeam(&data, Team);
}

void CBattlegroundManager::SendBattlegroundQueueStatus(Player* plr, uint32 queueSlot)
{
	if( queueSlot > 2 ) return;
	
	uint32 Type = plr->m_bgQueueType[ queueSlot ];
	uint32 MapId = GetBattleGroundMapIdByType(Type);
	//Log.Notice("BattlegroundManager", "Sending updated Battleground queues for %u.", queueSlot);
	WorldPacket data(SMSG_BATTLEFIELD_STATUS, 30);
	if( plr->m_bg && plr->m_bgSlot == queueSlot)
	{
		// Perform a manual update: this BG
		data << uint32(queueSlot);
		data << MapId;
		data << plr->m_bg->GetMapMgr()->GetInstanceID();
		data << uint8(plr->m_bgTeam);
		data << uint32(3); 	// Status
		data << uint32(0);
		data << uint32(60);
		plr->GetSession()->SendPacket(&data);
		return;
	}
	// We're no longer queued!
	if( !plr->m_bgIsQueued[queueSlot] && !plr->m_pendingBattleground[queueSlot])
	{
		//Log.Notice("BattlegroundManager", "No queue slot active for %u.", queueSlot);
		data << uint32(queueSlot) << uint64(0);
		plr->GetSession()->SendPacket(&data);
		return;
	}

	data << uint32(queueSlot);
	data << MapId;
	data << plr->m_bgQueueInstanceId[queueSlot];
	data << uint8(plr->m_bgTeam);
	
	// Im in a BG
	if( plr->m_bg )
	{
		// Should've been handled already :P
		return;
	}

	// We're clear to join!
	if( plr->m_pendingBattleground[queueSlot] )
	{
		data << uint32(2);
		data << uint32(0); // Time
		plr->GetSession()->SendPacket(&data);
		return;
	}

	data << uint32(1);
	// And we're waiting...
	data << uint32(GetAverageQueueTime(Type)*1000);		// average time in msec
	data << uint32(0);
	plr->GetSession()->SendPacket(&data);
	
}

void CBattleground::RemovePlayer(Player* plr, bool logout)
{
	if(!plr->IsPlayer())
		return;

	WorldPacket data(SMSG_BATTLEGROUND_PLAYER_LEFT, 30);
	data << plr->GetGUID();

	m_mainLock.Acquire();
	m_players[0].erase(plr);
	m_players[1].erase(plr);
	if ( plr->m_isGmInvisible == false )
	{
		//Dont show invisble gm's leaving the game.
		for(int i = 0; i < 2; ++i)
		{
			if( !m_players[i].size() )
				continue;
			for(set<Player*  >::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
				(*itr)->GetSession()->SendPacket(&data);
		}
	}

	memset(&plr->m_bgScore, 0, sizeof(BGScore));
	OnRemovePlayer(plr);

	/* are we in the group? */
	if(plr->GetGroup() == m_groups[plr->m_bgTeam])
		plr->GetGroup()->RemovePlayer( plr->m_playerInfo );

	plr->m_bg = NULLBATTLEGROUND;
	plr->FullHPMP();


	// reset team
	plr->ResetTeam();

	/* revive the player if he is dead */
	if(!logout && !plr->isAlive())
	{
		plr->SetUInt32Value(UNIT_FIELD_HEALTH, plr->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		plr->ResurrectPlayer();
	}
	
	/* teleport out */
	if(!logout)
	{
		BattlegroundManager.SendBattlegroundQueueStatus(plr, 0);
		BattlegroundManager.SendBattlegroundQueueStatus(plr, 1);
		BattlegroundManager.SendBattlegroundQueueStatus(plr, 2);

		GetMapMgr()->GetStateManager().ClearWorldStates(plr);

		if(!IS_INSTANCE(plr->m_bgEntryPointMap))
		{
			LocationVector vec(plr->m_bgEntryPointX, plr->m_bgEntryPointY, plr->m_bgEntryPointZ, plr->m_bgEntryPointO);
			plr->SafeTeleport(plr->m_bgEntryPointMap, plr->m_bgEntryPointInstance, vec);
		}
		else
		{
			LocationVector vec(plr->GetBindPositionX(), plr->GetBindPositionY(), plr->GetBindPositionZ());
			plr->SafeTeleport(plr->GetBindMapId(), 0, vec);
		}
	}

	if(!m_ended && m_players[0].size() == 0 && m_players[1].size() == 0)
	{
		/* create an inactive event */
		sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);						// 10mins
		sEventMgr.AddEvent(this, &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 600000, 1,0);
	}

	plr->m_bgTeam=plr->GetTeam();
	m_mainLock.Release();
}

void CBattleground::SendPVPData(Player* plr)
{              
	m_mainLock.Acquire();
	if( m_players[0].size() == 0 && m_players[1].size() == 0 )
	{
		m_mainLock.Release();
		return;
	}

	WorldPacket data(10*(m_players[0].size()+m_players[1].size())+50);
	BuildPvPUpdateDataPacket(&data);
	plr->GetSession()->SendPacket(&data);
	
	m_mainLock.Release();
}

void CBattleground::EventCreate()
{
	OnCreate();
}

int32 CBattleground::event_GetInstanceID()
{
	return m_mapMgr->GetInstanceID();
}

void CBattleground::EventCountdown()
{
	if(m_countdownStage == 1)
	{
		m_countdownStage = 2;
		SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "One minute until the battle for %s begins!", GetName() );
	}
	else if(m_countdownStage == 2)
	{
		m_countdownStage = 3;
		SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "Thirty seconds until the battle for %s begins!", GetName() );
		sEventMgr.ModifyEventTimeAndTimeLeft(this, EVENT_BATTLEGROUND_COUNTDOWN, 15000);
	}
	else if(m_countdownStage == 3)
	{
		m_countdownStage = 4;
		SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "Fifteen seconds until the battle for %s begins!", GetName() );
	}
	else
	{
		SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The battle for %s has begun!", GetName() );
		sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_COUNTDOWN);
		Start();
	}
}

void CBattleground::Start()
{
	OnStart();
	m_startTime = (uint32)UNIXTIME;
}

void CBattleground::Close()
{
	/* remove all players from the battleground */
	m_mainLock.Acquire();
	m_ended = true;
	for(uint32 i = 0; i < 2; ++i)
	{
		set<Player*  >::iterator itr;
		set<uint32>::iterator it2;
		uint32 guid;
		Player* plr;
		for(itr = m_players[i].begin(); itr != m_players[i].end();)
		{
			plr = *itr;
			++itr;
			RemovePlayer(plr, false);
		}

		for(it2 = m_pendPlayers[i].begin(); it2 != m_pendPlayers[i].end();)
		{
			guid = *it2;
			++it2;
			plr = objmgr.GetPlayer(guid);

			if(plr!=nullptr)
				RemovePendingPlayer(plr);
			else
				m_pendPlayers[i].erase(guid);
		}
	}

	/* call the virtual onclose for cleanup etc */
	OnClose();

	/* shut down the map thread. this will delete the battleground from the corrent context. */
	m_mapMgr->Terminate();

	m_mainLock.Release();
}

Creature* CBattleground::SpawnSpiritGuide(float x, float y, float z, float o, uint32 horde)
{
	if(horde > 1)
		horde = 1;

	CreatureInfo * pInfo = CreatureNameStorage.LookupEntry(13116 + horde);
	if(pInfo == 0)
		return NULLCREATURE;

	CreatureProto *pProto = CreatureProtoStorage.LookupEntry(13116 + horde);
	if( pProto == nullptr )
		return NULLCREATURE;

	Creature* pCreature = NULLCREATURE;
	pCreature = m_mapMgr->CreateCreature(pInfo->Id);
	if (pCreature == NULLCREATURE)
		return NULLCREATURE;

	pCreature->Create(pInfo->Name, m_mapMgr->GetMapId(), x, y, z, o);
	pCreature->proto = pProto;
	pCreature->creature_info = pInfo;

	pCreature->SetInstanceID(m_mapMgr->GetInstanceID());
	pCreature->SetUInt32Value(OBJECT_FIELD_ENTRY, 13116 + horde);
	pCreature->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);

	pCreature->SetUInt32Value(UNIT_FIELD_HEALTH, 100000);
	pCreature->SetUInt32Value(UNIT_FIELD_POWER1, 4868);
	pCreature->SetUInt32Value(UNIT_FIELD_POWER3, 200);
	pCreature->SetUInt32Value(UNIT_FIELD_POWER5, 2000000);

	pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH, 10000);
	pCreature->SetUInt32Value(UNIT_FIELD_MAXPOWER1, 4868);
	pCreature->SetUInt32Value(UNIT_FIELD_MAXPOWER3, 200);
	pCreature->SetUInt32Value(UNIT_FIELD_MAXPOWER5, 2000000);

	pCreature->SetUInt32Value(UNIT_FIELD_LEVEL, 60);
	pCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 84 - horde);
	pCreature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0 | (2 << 8) | (1 << 16));

	pCreature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 22802);
	pCreature->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 2 | (0xA << 8) | (2 << 16) | (0x11 << 24));
	pCreature->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 2);
	
	pCreature->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLUS_MOB | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_PASSIVE); // 4928

	pCreature->SetUInt32Value(UNIT_FIELD_AURA, 22011);
	pCreature->SetUInt32Value(UNIT_FIELD_AURAFLAGS, 9);
	pCreature->SetUInt32Value(UNIT_FIELD_AURALEVELS, 0x3C);
	pCreature->SetUInt32Value(UNIT_FIELD_AURAAPPLICATIONS, 0xFF);
	
	pCreature->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, 2000);
	pCreature->SetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME, 2000);
	pCreature->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.208f);
	pCreature->SetFloatValue(UNIT_FIELD_COMBATREACH, 1.5f);

	pCreature->SetUInt32Value(UNIT_FIELD_DISPLAYID, 13337 + horde);
	pCreature->SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, 13337 + horde);

	pCreature->SetUInt32Value(UNIT_CHANNEL_SPELL, 22011);
	pCreature->SetUInt32Value(UNIT_MOD_CAST_SPEED, 1065353216);

	pCreature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITGUIDE);
	pCreature->SetUInt32Value(UNIT_FIELD_BYTES_2, 1 | (0x10 << 8));

	pCreature->SetPvPFlag();

	pCreature->DisableAI();
	pCreature->_setFaction();

	pCreature->PushToWorld(m_mapMgr);
	return pCreature;
}

void CBattleground::QueuePlayerForResurrect(Player* plr, Creature* spirit_healer)
{
	m_mainLock.Acquire();
	map<Creature*,set<uint32> >::iterator itr = m_resurrectMap.find(spirit_healer);
	if(itr != m_resurrectMap.end())
	{
		itr->second.insert(plr->GetLowGUID());
		plr->m_areaSpiritHealer_guid=spirit_healer->GetGUID();
		plr->CastSpell(plr,2584,true);
	}
	m_mainLock.Release();
}

void CBattleground::RemovePlayerFromResurrect(Player* plr, Creature* spirit_healer)
{
	m_mainLock.Acquire();
	map<Creature*,set<uint32> >::iterator itr = m_resurrectMap.find(spirit_healer);
	if(itr != m_resurrectMap.end())
		itr->second.erase(plr->GetLowGUID());
	plr->m_areaSpiritHealer_guid=0;
	m_mainLock.Release();
}

void CBattleground::AddSpiritGuide(Creature* pCreature)
{
	if (pCreature == nullptr)
		return;
	m_mainLock.Acquire();
	map<Creature*,set<uint32> >::iterator itr = m_resurrectMap.find(pCreature);
	if(itr == m_resurrectMap.end())
	{
		set<uint32> ti;
		m_resurrectMap.insert(make_pair(pCreature,ti));
	}
	m_mainLock.Release();
}

void CBattleground::RemoveSpiritGuide(Creature* pCreature)
{
	m_mainLock.Acquire();
	m_resurrectMap.erase(pCreature);
	m_mainLock.Release();
}

void CBattleground::EventResurrectPlayers()
{
	m_mainLock.Acquire();
	Player* plr;
	set<uint32>::iterator itr;
	map<Creature*,set<uint32> >::iterator i;
	WorldPacket data(50);
	for(i = m_resurrectMap.begin(); i != m_resurrectMap.end(); ++i)
	{
		for(itr = i->second.begin(); itr != i->second.end(); ++itr)
		{
			plr = m_mapMgr->GetPlayer(*itr);
			if(plr && plr->isDead())
			{
                data.Initialize(SMSG_SPELL_START);
				data << plr->GetNewGUID() << plr->GetNewGUID() << uint32(RESURRECT_SPELL) << uint8(0) << uint16(0) << uint32(0) << uint16(2) << plr->GetGUID();
				plr->SendMessageToSet(&data, true);

				data.Initialize(SMSG_SPELL_GO);
				data << plr->GetNewGUID() << plr->GetNewGUID() << uint32(RESURRECT_SPELL) << uint8(0) << uint8(1) << uint8(1) << plr->GetGUID() << uint8(0) << uint16(2)
					<< plr->GetGUID();
				plr->SendMessageToSet(&data, true);

				plr->ResurrectPlayer();
				plr->SpawnCorpseBones();
				plr->SetUInt32Value(UNIT_FIELD_HEALTH, plr->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				plr->SetUInt32Value(UNIT_FIELD_POWER1, plr->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
				plr->SetUInt32Value(UNIT_FIELD_POWER4, plr->GetUInt32Value(UNIT_FIELD_MAXPOWER4));
			}
		}
		i->second.clear();
	}
	m_lastResurrect = (uint32)UNIXTIME;
	m_mainLock.Release();
}

bool CBattleground::CanPlayerJoin(Player* plr)
{
	return ( plr->bGMTagOn || HasFreeSlots(plr->m_bgTeam)&&(GetLevelGrouping(plr->getLevel())==GetLevelGroup()));
}

void CBattleground::QueueAtNearestSpiritGuide(Player* plr, Creature* old)
{
	float dd;
	float dist = 999999.0f;
	Creature* cl = NULLCREATURE;
	set<uint32> *closest = nullptr;
	m_lock.Acquire();
	map<Creature*, set<uint32> >::iterator itr = m_resurrectMap.begin();
	for(; itr != m_resurrectMap.end(); ++itr)
	{
		if( itr->first == old )
			continue;

		dd = plr->GetDistance2dSq(itr->first) < dist;
		if( dd < dist )
		{
			cl = itr->first;
			closest = &itr->second;
			dist = dd;
		}
	}

	if( closest != nullptr )
	{
		closest->insert(plr->GetLowGUID());
		plr->m_areaSpiritHealer_guid=cl->GetGUID();
		plr->CastSpell(plr,2584,true);
	}

	m_lock.Release();
}

void CBattleground::GiveHonorToTeam(uint32 team, uint32 amt)
{
	for(set<Player*  >::iterator itx = m_players[team].begin(); itx != m_players[team].end(); ++itx)
	{
		Player* plr = (*itx);
		if(!plr) continue;

		plr->m_bgScore.BonusHonor += amt;
		plr->AddHonorCP(amt, HONORABLE, 0, 0);
	}
}


bool CBattleground::HookSlowLockOpen(GameObject* pGo, Player* pPlayer, Spell* pSpell)
{

	if( pPlayer->m_stealth )
		pPlayer->RemoveAura( pPlayer->m_stealth );

	return false;
}

// World States Management

void CBattleground::SendWorldStates(Player * plr)
{
	if(!m_worldStates.size())
		return;

	uint32 bflag = 0;
	uint32 bflag2 = 0;

	switch(m_mapMgr->GetMapId())
	{
	case  489: bflag = 0x0CCD; bflag2 = 0x0CF9; break;
	case  529: bflag = 0x0D1E; break;
	case   30: bflag = 0x0A25; break;
	}

	WorldPacket data(SMSG_INIT_WORLD_STATES, 10 + (m_worldStates.size() * 8));
	data << m_mapMgr->GetMapId();
	data << bflag;
	data << bflag2;
	data << uint16(m_worldStates.size());

	for(map<uint32, uint32>::iterator itr = m_worldStates.begin(); itr != m_worldStates.end(); ++itr)
		data << itr->first << itr->second;
	plr->GetSession()->SendPacket(&data);
}

void CBattleground::SetWorldState(uint32 Index, uint32 Value)
{
	map<uint32, uint32>::iterator itr = m_worldStates.find(Index);
	if(itr == m_worldStates.end())
		m_worldStates.insert( make_pair( Index, Value ) );
	else
		itr->second = Value;

	WorldPacket data(SMSG_UPDATE_WORLD_STATE, 8);
	data << Index << Value;
	DistributePacketToAll(&data);
}