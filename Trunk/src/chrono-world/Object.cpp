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
#include "Unit.h"
#include "formulas.h"
using namespace std;

//#define DEG2RAD (M_PI/180.0)
#define M_PI		3.14159265358979323846
#define M_H_PI		1.57079632679489661923
#define M_Q_PI		0.785398163397448309615

Object::Object() : m_position(0,0,0,0), m_spawnLocation(0,0,0,0)
{
	m_mapId = 0;
	m_zoneId = 0;

	m_uint32Values = 0;
	m_objectUpdated = false;


	m_valuesCount = 0;

	//official Values
	m_walkSpeed = 2.5f;
	m_runSpeed = 7.0f;
	m_base_runSpeed = m_runSpeed;
	m_base_walkSpeed = m_walkSpeed;

	m_flySpeed = 7.0f;
	m_backFlySpeed = 4.5f;

	m_backWalkSpeed = 4.5f;	// this should really be named m_backRunSpeed
	m_swimSpeed = 4.722222f;
	m_backSwimSpeed = 2.5f;
	m_turnRate = 3.141593f;

	m_mapMgr = 0;
	m_mapCell = 0;

	mSemaphoreTeleport = false;


	m_faction = nullptr;
	m_factionDBC = nullptr;

	m_instanceId = 0;
	Active = false;
	m_inQueue = false;
	m_extensions = nullptr;
	m_loadedFromDB = false;

	m_inRangePlayers.clear();
}

Object::~Object( )
{
	POINTERLOGDELETE(this);
	
	if(m_objectTypeId != TYPEID_ITEM)
		ASSERT(!m_inQueue);

	if (this->IsInWorld() && m_objectTypeId != TYPEID_ITEM && m_objectTypeId != TYPEID_CONTAINER)
	{
		this->RemoveFromWorld(false);
	}

	// for linux
	m_instanceId = -1;
	m_objectTypeId=TYPEID_UNUSED;
	
	if( m_extensions != nullptr )
		delete m_extensions;
}

void Object::Init()
{
}

void Object::Destructor()
{
	delete this;
}

void Object::_Create( uint32 mapid, float x, float y, float z, float ang )
{
	m_mapId = mapid;
	m_position.ChangeCoords(x, y, z, ang);
	m_spawnLocation.ChangeCoords(x, y, z, ang);
	m_lastMapUpdatePosition.ChangeCoords(x,y,z,ang);
}

uint32 Object::BuildCreateUpdateBlockForPlayer(ByteBuffer *data, Player *target)
{
	uint8 flags = 0;
	uint32 flags2 = 0;
	
	uint8 updatetype = UPDATETYPE_CREATE_OBJECT;
	if(m_objectTypeId == TYPEID_CORPSE)
	{
		if(m_uint32Values[CORPSE_FIELD_DISPLAY_ID] == 0)
			return 0;
	}
	
	// any other case
	switch(m_objectTypeId)
	{
		// items + containers: 0x8
	case TYPEID_ITEM:
	case TYPEID_CONTAINER:
        flags  = 0x10;
		break;
		
		// player/unit: 0x68 (except self)
	case TYPEID_UNIT:
        flags  = 0x70;
		flags2 = 0x800000;
		break;

	case TYPEID_PLAYER:
		flags  = 0x70;
		break;

		// gameobject
	case TYPEID_GAMEOBJECT:
	case TYPEID_DYNAMICOBJECT:
	case TYPEID_CORPSE:
		flags = 0x50;
		break;

		// anyone else can get fucked and die!
	}

	if(target == this)
	{
		// player creating self
        flags  |= 0x71;
		flags2 |= 0x2000;
		updatetype = UPDATETYPE_CREATE_YOURSELF;
	}

	// gameobject stuff
	if(m_objectTypeId == TYPEID_GAMEOBJECT)
	{
		switch(m_uint32Values[GAMEOBJECT_TYPE_ID])
		{
			case GAMEOBJECT_TYPE_MO_TRANSPORT:  
				{
					if(GetTypeFromGUID() != HIGHGUID_TYPE_TRANSPORTER)
						return 0;   // bad transporter
					else
						flags |= 0x02;
				}break;

			case GAMEOBJECT_TYPE_TRANSPORT:
				{
					/* deeprun tram, etc */
					flags |= 0x02;
				}
			break;
		}
	}

	// build our actual update
	*data << updatetype;

	// we shouldn't be here, under any cercumstances, unless we have a wowguid..
	ASSERT(m_wowGuid.GetNewGuidLen());
	*data << m_wowGuid;
	
	*data << m_objectTypeId;

	_BuildMovementUpdate(data, flags, flags2, target);

	// we have dirty data, or are creating for ourself.
	UpdateMask updateMask;
	updateMask.SetCount( m_valuesCount );
	_SetCreateBits( &updateMask, target );

	// this will cache automatically if needed
	_BuildValuesUpdate( data, &updateMask, target );

	// update count: 1 ;)
	return 1;
}


//That is dirty fix it actually creates update of 1 field with
//the given value ignoring existing changes in fields and so on
//usefull if we want update this field for certain players
//NOTE: it does not change fields. This is also very fast method
WorldPacket *Object::BuildFieldUpdatePacket( uint32 index,uint32 value)
{
   // uint64 guidfields = GetGUID();
   // uint8 guidmask = 0;
	WorldPacket * packet=new WorldPacket(1500);
	packet->SetOpcode( SMSG_UPDATE_OBJECT );
	
	*packet << (uint32)1;//number of update/create blocks

	*packet << (uint8) UPDATETYPE_VALUES;		// update type == update
	*packet << GetNewGUID();

	uint32 mBlocks = index/32+1;
	*packet << (uint8)mBlocks;
	
	for(uint32 dword_n=mBlocks-1;dword_n;dword_n--)
	*packet <<(uint32)0;

	*packet <<(((uint32)(1))<<(index%32));
	*packet << value;
	
	return packet;
}

void Object::BuildFieldUpdatePacket(Player* Target, uint32 Index, uint32 Value)
{
	ByteBuffer buf(500);

	BuildFieldUpdatePacket(&buf, Index, Value);
	Target->PushUpdateData(&buf, 1);
}

void Object::BuildFieldUpdatePacket(ByteBuffer * buf, uint32 Index, uint32 Value)
{
	*buf << uint8(UPDATETYPE_VALUES);
	*buf << GetNewGUID();

	uint32 mBlocks = Index/32+1;
	*buf << (uint8)mBlocks;

	for(uint32 dword_n=mBlocks-1;dword_n;dword_n--)
		*buf <<(uint32)0;

	*buf <<(((uint32)(1))<<(Index%32));
	*buf << Value;
}

uint32 Object::BuildValuesUpdateBlockForPlayer(ByteBuffer *data, Player *target)
{
	UpdateMask updateMask;
	updateMask.SetCount( m_valuesCount );
	_SetUpdateBits( &updateMask, target );
	for(uint32 x = 0; x < m_valuesCount; ++x)
	{
		if(updateMask.GetBit(x))
		{
			*data << (uint8) UPDATETYPE_VALUES;		// update type == update
			ASSERT(m_wowGuid.GetNewGuidLen());
			*data << m_wowGuid;

			_BuildValuesUpdate( data, &updateMask, target );
			return 1;
		}
	}

	return 0;
}

uint32 Object::BuildValuesUpdateBlockForPlayer(ByteBuffer * buf, UpdateMask * mask )
{
	// returns: update count
	*buf << (uint8) UPDATETYPE_VALUES;		// update type == update

	ASSERT(m_wowGuid.GetNewGuidLen());
	*buf << m_wowGuid;

	_BuildValuesUpdate( buf, mask, NULLPLR );

	// 1 update.
	return 1;
}

void Object::DestroyForPlayer(Player *target) const
{
	if(target->GetSession() == 0) return;

	ASSERT(target);

	WorldPacket data(SMSG_DESTROY_OBJECT, 8);
	data << GetGUID();

	target->GetSession()->SendPacket( &data );
}


///////////////////////////////////////////////////////////////
/// Build the Movement Data portion of the update packet
/// Fills the data with this object's movement/speed info
/// TODO: rewrite this stuff, document unknown fields and flags
uint32 TimeStamp();

void Object::_BuildMovementUpdate(ByteBuffer * data, uint8 flags, uint32 flags2, Player* target )
{
  *data << (uint8)flags;
  if(m_objectTypeId == TYPEID_PLAYER)
  {
    if(((Player*)this)->m_CurrentTransporter)
    {
      flags2 |= 0x2000000;
    }
    *data << (uint32)flags2;

    *data << (uint32)getMSTime();

    if (!((Player *)this)->m_CurrentTransporter)
    {
      *data << ((Player *)this)->GetPositionX();
      *data << ((Player *)this)->GetPositionY();
      *data << ((Player *)this)->GetPositionZ();
      *data << ((Player *)this)->GetOrientation();
    }
    else
    {
      *data << ((Player *)this)->m_CurrentTransporter->GetPositionX();
      *data << ((Player *)this)->m_CurrentTransporter->GetPositionY();
      *data << ((Player *)this)->m_CurrentTransporter->GetPositionZ();
      *data << ((Player *)this)->m_CurrentTransporter->GetOrientation();

      // Fix me ?
      *data << (uint64)(((Player *)this)->m_CurrentTransporter->GetGUID());
      *data << ((Player *)this)->m_CurrentTransporter->GetPositionX();
      *data << ((Player *)this)->m_CurrentTransporter->GetPositionY();
      *data << ((Player *)this)->m_CurrentTransporter->GetPositionZ();
      *data << ((Player *)this)->m_CurrentTransporter->GetOrientation();
    }

    *data << (float)0;

    if(flags2 & 0x2000)                                 //update self
    {
      *data << (float)0;
      *data << (float)1.0;
      *data << (float)0;
      *data << (float)0;
    }

    *data << ((Player*)this)->m_walkSpeed;
    *data << ((Player*)this)->m_runSpeed;
    *data << ((Player*)this)->m_backSwimSpeed;
    *data << ((Player*)this)->m_swimSpeed;
    *data << ((Player*)this)->m_backWalkSpeed;
    *data << ((Player*)this)->m_turnRate;
  }

  if(m_objectTypeId == TYPEID_UNIT)
  {
    *data << (uint32)flags2;
    *data << (uint32)0xB5771D7F;
    *data << ((Unit *)this)->GetPositionX();
    *data << ((Unit *)this)->GetPositionY();
    *data << ((Unit *)this)->GetPositionZ();
    *data << ((Unit *)this)->GetOrientation();
    *data << (float)0;
    *data << ((Creature*)this)->m_walkSpeed;
    *data << ((Creature*)this)->m_runSpeed;
    *data << ((Creature*)this)->m_backSwimSpeed;
    *data << ((Creature*)this)->m_swimSpeed;
    *data << ((Creature*)this)->m_backWalkSpeed;
    *data << ((Creature*)this)->m_turnRate;

    uint8 PosCount=0;
    if(flags2 & 0x400000)
    {
      *data << (uint32)0x0;
      *data << (uint32)0x659;
      *data << (uint32)0xB7B;
      *data << (uint32)0xFDA0B4;
      *data << (uint32)PosCount;
      for(int i=0;i<PosCount+1;i++)
      {
        *data << (float)0;                          //x
        *data << (float)0;                          //y
        *data << (float)0;                          //z
      }
    }
  }

  if((m_objectTypeId == TYPEID_CORPSE) || (m_objectTypeId == TYPEID_GAMEOBJECT) || (m_objectTypeId == TYPEID_DYNAMICOBJECT))
  {
    if(Chrono::Util::GUID_HIPART(GetGUID()) != 0x80000000)
    {
      *data << ((Object *)this)->GetPositionX();
      *data << ((Object *)this)->GetPositionY();
      *data << ((Object *)this)->GetPositionZ();
    }
    else
    {
      *data << (uint32)0;
      *data << (uint32)0;
      *data << (uint32)0;
    }
    *data << ((Object *)this)->GetOrientation();
  }

  if (flags & 16)                             // 0x10
  {
       *data << (uint32)0x1;
  }
 
  if (flags & 2)                       // 0x02, including corpse
  {
       *data << (uint32)getMSTime();
  }
  
  //*data << (uint32)0x1;

  if ((Chrono::Util::GUID_HIPART(GetGUID()) == 0x80000000))
  {
    uint32 updT = (uint32)getMSTime();
    *data << (uint32)updT;
  }

  if(Chrono::Util::GUID_HIPART(GetGUID()) == 0x80000001)
    *data << (uint32)0xBD38BA14;                        //fix me
}

//=======================================================================================
//  Creates an update block with the values of this object as
//  determined by the updateMask.
//=======================================================================================
void Object::_BuildValuesUpdate(ByteBuffer * data, UpdateMask *updateMask, Player* target)
{
	bool activate_quest_object = false;
	bool reset = false;
	uint32 oldflags = 0;

	if(updateMask->GetBit(OBJECT_FIELD_GUID) && target)	   // We're creating.
	{
		Creature * pThis = static_cast<Creature*>(this);
		if(GetTypeId() == TYPEID_UNIT && pThis->Tagged && (pThis->loot.gold || pThis->loot.items.size()))
		{
			// Let's see if we're the tagger or not.
			oldflags = m_uint32Values[UNIT_DYNAMIC_FLAGS];
			uint32 Flags = m_uint32Values[UNIT_DYNAMIC_FLAGS];
			uint32 oldFlags = 0;

			if(pThis->TaggerGuid == target->GetGUID())

			{
				// Our target is our tagger.
				oldFlags = U_DYN_FLAG_TAGGED_BY_OTHER;

				if(Flags & U_DYN_FLAG_TAGGED_BY_OTHER)
					Flags &= ~oldFlags;

				if(!(Flags & U_DYN_FLAG_LOOTABLE))
					Flags |= U_DYN_FLAG_LOOTABLE;
			}
			else
			{
				// Target is not the tagger.
				oldFlags = U_DYN_FLAG_LOOTABLE;

				if(!(Flags & U_DYN_FLAG_TAGGED_BY_OTHER))
					Flags |= U_DYN_FLAG_TAGGED_BY_OTHER;

				if(Flags & U_DYN_FLAG_LOOTABLE)
					Flags &= ~oldFlags;
			}

			m_uint32Values[UNIT_DYNAMIC_FLAGS] = Flags;

			updateMask->SetBit(UNIT_DYNAMIC_FLAGS);
			
			reset = true;
		}

		if(target && GetTypeId() == TYPEID_GAMEOBJECT)
		{
			GameObject *go = ((GameObject*)this);
			QuestLogEntry *qle;
			GameObjectInfo *info;
			if( go->HasQuests() )
			{
				activate_quest_object = true;
			}
			else
			{
				info = go->GetInfo();
				if( info && ( info->goMap.size() || info->itemMap.size() ) )
				{
					for( GameObjectGOMap::iterator itr = go->GetInfo()->goMap.begin(); itr != go->GetInfo()->goMap.end(); ++itr )
					{
						qle = target->GetQuestLogForEntry( itr->first->id );
						if( qle != nullptr )
						{
							if( qle->GetQuest()->count_required_mob == 0 )
								continue;
							for( uint32 i = 0; i < 4; ++i )
							{
								if( qle->GetQuest()->required_mob[i] == go->GetEntry() && qle->GetMobCount(i) < qle->GetQuest()->required_mobcount[i])
								{
									activate_quest_object = true;
									break;
								}
							}
							if(activate_quest_object)
								break;
						}
					}

					if(!activate_quest_object)
					{
						for(GameObjectItemMap::iterator itr = go->GetInfo()->itemMap.begin();
							itr != go->GetInfo()->itemMap.end();
							++itr)
						{
							for(std::map<uint32, uint32>::iterator it2 = itr->second.begin();
								it2 != itr->second.end(); 
								++it2)
							{
								if((qle = target->GetQuestLogForEntry(itr->first->id)))
								{
									if(target->GetItemInterface()->GetItemCount(it2->first) < it2->second)
									{
										activate_quest_object = true;
										break;
									}
								}
							}
							if(activate_quest_object)
								break;
						}
					}
				}
			}
		}
	}


	if(activate_quest_object)
	{
		oldflags = m_uint32Values[GAMEOBJECT_DYN_FLAGS];
		if(!updateMask->GetBit(GAMEOBJECT_DYN_FLAGS))
			updateMask->SetBit(GAMEOBJECT_DYN_FLAGS);
		m_uint32Values[GAMEOBJECT_DYN_FLAGS] = 1;
		reset = true;
	}

	WPAssert( updateMask && updateMask->GetCount() == m_valuesCount );
	uint32 bc;
	uint32 values_count;
	if( m_valuesCount > ( 2 * 0x20 ) )//if number of blocks > 2->  unit and player+item container
	{
		bc = updateMask->GetUpdateBlockCount();
		values_count = (uint32)min( bc * 32, m_valuesCount );

	}
	else
	{
		bc=updateMask->GetBlockCount();
		values_count=m_valuesCount;
	}

	*data << (uint8)bc;
	data->append( updateMask->GetMask(), bc*4 );
	  
	for( uint32 index = 0; index < values_count; index ++ )
	{
		if( updateMask->GetBit( index ) )
		{
			switch(index)
			{
			case UNIT_FIELD_MAXHEALTH:
				{
					if(m_valuesCount < UNIT_END)
						*data << m_uint32Values[index];
					else
					{
						switch(m_objectTypeId)
						{
						case TYPEID_PLAYER:
							*data << m_uint32Values[index];
						break;

						case TYPEID_UNIT:
							{
								if(IsPet())
								{
									*data << m_uint32Values[index];
									break;
								}
								else
								{
									*data << (uint32)100;	
								}
							}
						}
					}
				}
				break;
			case UNIT_FIELD_HEALTH:
				{
					if(m_valuesCount < UNIT_END)
						*data << m_uint32Values[index];
					else
					{
						switch(m_objectTypeId)
						{
						case TYPEID_PLAYER:
							*data << m_uint32Values[index];
						break;

						case TYPEID_UNIT:
							{
								if(IsPet())
								{
									*data << m_uint32Values[index];
									break;
								}
								else
								{
									uint32 pct = uint32(float( float(m_uint32Values[index]) / float(m_uint32Values[UNIT_FIELD_MAXHEALTH]) * 100.0f));

									/* fix case where health value got rounded down and the client sees health as dead */
									if(!pct && m_uint32Values[UNIT_FIELD_HEALTH] != 0)
										++pct;
									*data << pct;	
								}
							}
						}
					}
				}
				break;

			default:
				*data << m_uint32Values[ index ];
				break;
			}
		}
	}

	if(reset)
	{
		switch(GetTypeId())
		{
		case TYPEID_UNIT:
			m_uint32Values[UNIT_DYNAMIC_FLAGS] = oldflags;
			break;
		case TYPEID_GAMEOBJECT:
			m_uint32Values[GAMEOBJECT_DYN_FLAGS] = oldflags;
			break;
		}
	}

}

void Object::BuildHeartBeatMsg(WorldPacket *data) const
{
	data->Initialize(MSG_MOVE_HEARTBEAT);

	*data << GetGUID();

	*data << uint32(0); // flags
	*data << uint32(0); // mysterious value #1

	*data << m_position;
	*data << m_position.o;
}

WorldPacket * Object::BuildTeleportAckMsg(const LocationVector & v)
{
	TO_PLAYER( this )->SetPlayerStatus( TRANSFER_PENDING );

	WorldPacket * data = new WorldPacket(MSG_MOVE_TELEPORT_ACK, 41);
	*data << GetNewGUID();
	*data << (uint32) 0;
	*data << (uint32) 5;
	*data << (uint32) getMSTime();
	*data << v;
	*data << (float) v.o;
	*data << (uint32) 0;

	return data;
}

bool Object::SetPosition(const LocationVector & v, bool allowPorting /* = false */)
{
	bool updateMap = false, result = true;

	if (m_position.x != v.x || m_position.y != v.y)
		updateMap = true;

	m_position = const_cast<LocationVector&>(v);

	if (!allowPorting && v.z < -500)
	{
		m_position.z = 500;
		sLog.outError( "setPosition: fell through map; height ported" );

		result = false;
	}

	if (IsInWorld() && updateMap)
	{
		m_mapMgr->ChangeObjectLocation(this);
	}

	return result;
}

bool Object::SetPosition( float newX, float newY, float newZ, float newOrientation, bool allowPorting )
{
	bool updateMap = false, result = true;

	//if (m_position.x != newX || m_position.y != newY)
		//updateMap = true;
	if(m_lastMapUpdatePosition.Distance2DSq(newX, newY) > 4.0f)		/* 2.0f */
		updateMap = true;

	m_position.ChangeCoords(newX, newY, newZ, newOrientation);

	if (!allowPorting && newZ < -500)
	{
		m_position.z = 500;
		sLog.outError( "setPosition: fell through map; height ported" );

		result = false;
	}

	if (IsInWorld() && updateMap)
	{
		m_lastMapUpdatePosition.ChangeCoords(newX,newY,newZ,newOrientation);
		m_mapMgr->ChangeObjectLocation(this);

		if( m_objectTypeId == TYPEID_PLAYER && TO_PLAYER( this )->GetGroup() && TO_PLAYER( this )->m_last_group_position.Distance2DSq(m_position) > 25.0f ) // distance of 5.0
		{
            TO_PLAYER( this )->GetGroup()->HandlePartialChange( PARTY_UPDATE_FLAG_POSITION, TO_PLAYER( this ) );
		}	
	}

	return result;
}

void Object::SetRotation( uint64 guid )
{
	WorldPacket data(SMSG_AI_REACTION, 12);
	data << guid;
	data << uint32(2);
	SendMessageToSet(&data, false);
}

void Object::OutPacketToSet(uint16 Opcode, uint16 Len, const void * Data, bool self)
{
	if(self && m_objectTypeId == TYPEID_PLAYER)
		TO_PLAYER( this )->GetSession()->OutPacket(Opcode, Len, Data);

	if(!IsInWorld())
		return;

	std::set<Player*>::iterator itr = m_inRangePlayers.begin();
	std::set<Player*>::iterator it_end = m_inRangePlayers.end();
	int gm = ( m_objectTypeId == TYPEID_PLAYER ? TO_PLAYER( this )->m_isGmInvisible : 0 );
	for(; itr != it_end; ++itr)
	{
		ASSERT((*itr)->GetSession());
		if( gm )
		{
			if( (*itr)->GetSession()->GetPermissionCount() > 0 )
				(*itr)->GetSession()->OutPacket(Opcode, Len, Data);
		}
		else
		{
			(*itr)->GetSession()->OutPacket(Opcode, Len, Data);
		}
	}
}

void Object::SendMessageToSet(WorldPacket *data, bool bToSelf,bool myteam_only)
{
	if(bToSelf && m_objectTypeId == TYPEID_PLAYER)
	{
		TO_PLAYER( this )->GetSession()->SendPacket(data);		
	}

	if(!IsInWorld())
		return;

	std::set<Player*>::iterator itr = m_inRangePlayers.begin();
	std::set<Player*>::iterator it_end = m_inRangePlayers.end();
	bool gminvis = (m_objectTypeId == TYPEID_PLAYER ? TO_PLAYER( this )->m_isGmInvisible : false);
	//Zehamster: Splitting into if/else allows us to avoid testing "gminvis==true" at each loop...
	//		   saving cpu cycles. Chat messages will be sent to everybody even if player is invisible.
	if(myteam_only)
	{
		uint32 myteam=TO_PLAYER( this )->GetTeam();
		if(gminvis && data->GetOpcode()!=SMSG_MESSAGECHAT)
		{
			for(; itr != it_end; ++itr)
			{
				ASSERT((*itr)->GetSession());
				if((*itr)->GetSession()->GetPermissionCount() > 0 && (*itr)->GetTeam()==myteam)
					(*itr)->GetSession()->SendPacket(data);
			}
		}
		else
		{
			for(; itr != it_end; ++itr)
			{
				ASSERT((*itr)->GetSession());
				if((*itr)->GetTeam()==myteam)
					(*itr)->GetSession()->SendPacket(data);
			}
		}
	}
	else
	{
		if(gminvis && data->GetOpcode()!=SMSG_MESSAGECHAT)
		{
			for(; itr != it_end; ++itr)
			{
				ASSERT((*itr)->GetSession());
				if((*itr)->GetSession()->GetPermissionCount() > 0)
					(*itr)->GetSession()->SendPacket(data);
			}
		}
		else
		{
			for(; itr != it_end; ++itr)
			{
				ASSERT((*itr)->GetSession());
				(*itr)->GetSession()->SendPacket(data);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
/// Fill the object's Update Values from a space deliminated list of values.
void Object::LoadValues(const char* data)
{
	// thread-safe ;) strtok is not.
	std::string ndata = data;
	std::string::size_type last_pos = 0, pos = 0;
	uint32 index = 0;
	uint32 val;
	do 
	{
		// prevent overflow
		if(index >= m_valuesCount)
		{
			break;
		}
		pos = ndata.find(" ", last_pos);
		val = atol(ndata.substr(last_pos, (pos-last_pos)).c_str());
		if(m_uint32Values[index] == 0)
			m_uint32Values[index] = val;
		last_pos = pos+1;
		++index;
	} while(pos != std::string::npos);
}

void Object::_SetUpdateBits(UpdateMask *updateMask, Player *target) const
{
	*updateMask = m_updateMask;
}


void Object::_SetCreateBits(UpdateMask *updateMask, Player *target) const
{
	/*for( uint16 index = 0; index < m_valuesCount; index++ )
	{
		if(GetUInt32Value(index) != 0)
			updateMask->SetBit(index);
	}*/
	for(uint32 i = 0; i < m_valuesCount; ++i)
		if(m_uint32Values[i] != 0)
			updateMask->SetBit(i);
}

void Object::AddToWorld()
{
	MapMgr *mapMgr = sInstanceMgr.GetInstance(this);
	if(!mapMgr)
		return; //instance add failed

	m_mapMgr = mapMgr;
	m_inQueue = true;

	mapMgr->AddObject(this);

	// correct incorrect instance id's
	m_instanceId = m_mapMgr->GetInstanceID();

	mSemaphoreTeleport = false;
}

void Object::AddToWorld(MapMgr * pMapMgr)
{
	if(!pMapMgr)
		return; //instance add failed

	m_mapMgr = pMapMgr;
	m_inQueue = true;

	pMapMgr->AddObject(this);

	// correct incorrect instance id's
	m_instanceId = pMapMgr->GetInstanceID();

	mSemaphoreTeleport = false;
}

//Unlike addtoworld it pushes it directly ignoring add pool
//this can only be called from the thread of mapmgr!!!
void Object::PushToWorld(MapMgr*mgr)
{
	if(!mgr/* || (m_mapMgr != nullptr && m_mapCell != nullptr) */)
		return; //instance add failed

	m_mapId=mgr->GetMapId();
	m_instanceId = mgr->GetInstanceID();

	m_mapMgr = mgr;
	OnPrePushToWorld();

	mgr->PushObject(this);

	// correct incorrect instance id's
	mSemaphoreTeleport = false;
	m_inQueue = false;
   
	event_Relocate();
	
	// call virtual function to handle stuff.. :P
	OnPushToWorld();
}

void Object::RemoveFromWorld(bool free_guid)
{
	ASSERT(m_mapMgr);
	MapMgr * m = m_mapMgr;
	m_mapMgr = 0;

	mSemaphoreTeleport = true;

	m->RemoveObject(this, free_guid);
	
	// update our event holder
	event_Relocate();
}

//! Set uint32 property
void Object::SetUInt32Value( const uint32 index, const uint32 value )
{
	ASSERT( index < m_valuesCount );
	// save updating when val isn't changing.
	if(m_uint32Values[index] == value)
		return;

	m_uint32Values[ index ] = value;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}

	// Group update handling
	if(m_objectTypeId == TYPEID_PLAYER)
	{
		if(IsInWorld())
		{
			Group* pGroup = TO_PLAYER( this )->GetGroup();
			if( pGroup != nullptr )
				pGroup->HandleUpdateFieldChange( index, TO_PLAYER( this ) );
		}

#ifdef OPTIMIZED_PLAYER_SAVING
		switch(index)
		{
		case UNIT_FIELD_LEVEL:
		case PLAYER_XP:
			TO_PLAYER( this )->save_LevelXP();
			break;

		case PLAYER_FIELD_COINAGE:
			TO_PLAYER( this )->save_Gold();
			break;
		}
#endif
	}
}
/*
//must be in %
void Object::ModPUInt32Value(const uint32 index, const int32 value, bool apply )
{
	ASSERT( index < m_valuesCount );
	int32 basevalue = (int32)m_uint32Values[ index ];
	if(apply)
		m_uint32Values[ index ] += ((basevalue*value)/100);
	else
		m_uint32Values[ index ] = (basevalue*100)/(100+value);

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated )
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}
*/
uint32 Object::GetModPUInt32Value(const uint32 index, const int32 value)
{
	ASSERT( index < m_valuesCount );
	int32 basevalue = (int32)m_uint32Values[ index ];
	return ((basevalue*value)/100);
}

void Object::ModUnsigned32Value(uint32 index, int32 mod)
{
	ASSERT( index < m_valuesCount );
	if(mod == 0)
		return;

	m_uint32Values[ index ] += mod;
	if( (int32)m_uint32Values[index] < 0 )
		m_uint32Values[index] = 0;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}

	if(m_objectTypeId == TYPEID_PLAYER)
	{
#ifdef OPTIMIZED_PLAYER_SAVING
		switch(index)
		{
		case UNIT_FIELD_LEVEL:
		case PLAYER_XP:
			TO_PLAYER( this )->save_LevelXP();
			break;

		case PLAYER_FIELD_COINAGE:
			TO_PLAYER( this )->save_Gold();
			break;
		}
#endif
	}
}

void Object::ModSignedInt32Value(uint32 index, int32 value )
{
	ASSERT( index < m_valuesCount );
	if(value == 0)
		return;

	m_uint32Values[ index ] += value;
	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}

	if(m_objectTypeId == TYPEID_PLAYER)
	{
#ifdef OPTIMIZED_PLAYER_SAVING
		switch(index)
		{
		case UNIT_FIELD_LEVEL:
		case PLAYER_XP:
			TO_PLAYER( this )->save_LevelXP();
			break;

		case PLAYER_FIELD_COINAGE:
			TO_PLAYER( this )->save_Gold();
			break;
		}
#endif
	}
}

void Object::ModFloatValue(const uint32 index, const float value )
{
	ASSERT( index < m_valuesCount );
	m_floatValues[ index ] += value;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}
//! Set uint64 property
void Object::SetUInt64Value( const uint32 index, const uint64 value )
{
	assert( index + 1 < m_valuesCount );
#ifndef USING_BIG_ENDIAN
	if(m_uint32Values[index] == Chrono::Util::GUID_LOPART(value) && m_uint32Values[index+1] == Chrono::Util::GUID_HIPART(value))
		return;

	m_uint32Values[ index ] = *((uint32*)&value);
	m_uint32Values[ index + 1 ] = *(((uint32*)&value) + 1);
#else
	m_uint32Values[index] = value & 0xffffffff;
	m_uint32Values[index+1] = (value >> 32) & 0xffffffff;
#endif

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );
		m_updateMask.SetBit( index + 1 );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}

//! Set float property
void Object::SetFloatValue( const uint32 index, const float value )
{
	ASSERT( index < m_valuesCount );
	if(m_floatValues[index] == value)
		return;

	m_floatValues[ index ] = value;
	
	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}


void Object::SetFlag( const uint32 index, uint32 newFlag )
{
	ASSERT( index < m_valuesCount );

	//no change -> no update
	if((m_uint32Values[ index ] & newFlag)==newFlag)
		return;

	m_uint32Values[ index ] |= newFlag;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}


void Object::RemoveFlag( const uint32 index, uint32 oldFlag )
{
	ASSERT( index < m_valuesCount );

	//no change -> no update
	if((m_uint32Values[ index ] & oldFlag)==0)
		return;

	m_uint32Values[ index ] &= ~oldFlag;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}

////////////////////////////////////////////////////////////

float Object::CalcDistance(Object *Ob)
{
	return CalcDistance(this->GetPositionX(), this->GetPositionY(), this->GetPositionZ(), Ob->GetPositionX(), Ob->GetPositionY(), Ob->GetPositionZ());
}
float Object::CalcDistance(float ObX, float ObY, float ObZ)
{
	return CalcDistance(this->GetPositionX(), this->GetPositionY(), this->GetPositionZ(), ObX, ObY, ObZ);
}
float Object::CalcDistance(Object *Oa, Object *Ob)
{
	return CalcDistance(Oa->GetPositionX(), Oa->GetPositionY(), Oa->GetPositionZ(), Ob->GetPositionX(), Ob->GetPositionY(), Ob->GetPositionZ());
}
float Object::CalcDistance(Object *Oa, float ObX, float ObY, float ObZ)
{
	return CalcDistance(Oa->GetPositionX(), Oa->GetPositionY(), Oa->GetPositionZ(), ObX, ObY, ObZ);
}

float Object::CalcDistance(float OaX, float OaY, float OaZ, float ObX, float ObY, float ObZ)
{
	float xdest = OaX - ObX;
	float ydest = OaY - ObY;
	float zdest = OaZ - ObZ;
	return sqrtf(zdest*zdest + ydest*ydest + xdest*xdest);
}

float Object::calcAngle( float Position1X, float Position1Y, float Position2X, float Position2Y )
{
	float dx = Position2X-Position1X;
	float dy = Position2Y-Position1Y;
	double angle=0.0f;

	// Calculate angle
	if (dx == 0.0)
	{
		if (dy == 0.0)
			angle = 0.0;
		else if (dy > 0.0)
			angle = M_PI * 0.5 /* / 2 */;
		else
			angle = M_PI * 3.0 * 0.5/* / 2 */;
	}
	else if (dy == 0.0)
	{
		if (dx > 0.0)
			angle = 0.0;
		else
			angle = M_PI;
	}
	else
	{
		if (dx < 0.0)
			angle = atanf(dy/dx) + M_PI;
		else if (dy < 0.0)
			angle = atanf(dy/dx) + (2*M_PI);
		else
			angle = atanf(dy/dx);
	}

	// Convert to degrees
	angle = angle * float(180 / M_PI);

	// Return
	return float(angle);
}

float Object::calcRadAngle( float Position1X, float Position1Y, float Position2X, float Position2Y )
{
	double dx = double(Position2X-Position1X);
	double dy = double(Position2Y-Position1Y);
	double angle=0.0;

	// Calculate angle
	if (dx == 0.0)
	{
		if (dy == 0.0)
			angle = 0.0;
		else if (dy > 0.0)
			angle = M_PI * 0.5/*/ 2.0*/;
		else
			angle = M_PI * 3.0 * 0.5/*/ 2.0*/;
	}
	else if (dy == 0.0)
	{
		if (dx > 0.0)
			angle = 0.0;
		else
			angle = M_PI;
	}
	else
	{
		if (dx < 0.0)
			angle = atan(dy/dx) + M_PI;
		else if (dy < 0.0)
			angle = atan(dy/dx) + (2*M_PI);
		else
			angle = atan(dy/dx);
	}

	// Return
	return float(angle);
}

float Object::getEasyAngle( float angle )
{
	while ( angle < 0 ) {
		angle = angle + 360;
	}
	while ( angle >= 360 ) {
		angle = angle - 360;
	}
	return angle;
}

bool Object::inArc(float Position1X, float Position1Y, float FOV, float Orientation, float Position2X, float Position2Y )
{
	float angle = calcAngle( Position1X, Position1Y, Position2X, Position2Y );
	float lborder = getEasyAngle( ( Orientation - (FOV*0.5f/*/2*/) ) );
	float rborder = getEasyAngle( ( Orientation + (FOV*0.5f/*/2*/) ) );
	//sLog.outDebug("Orientation: %f Angle: %f LeftBorder: %f RightBorder %f",Orientation,angle,lborder,rborder);
	if(((angle >= lborder) && (angle <= rborder)) || ((lborder > rborder) && ((angle < rborder) || (angle > lborder))))
	{
		return true;
	}
	else
	{
		return false;
	}
} 

bool Object::isInFront(Object* target)
{
	// check if we facing something ( is the object within a 180 degree slice of our positive y axis )

    double x = target->GetPositionX() - m_position.x;
    double y = target->GetPositionY() - m_position.y;

    double angle = atan2( y, x );
    angle = ( angle >= 0.0 ) ? angle : 2.0 * M_PI + angle;
	angle -= m_position.o;

    while( angle > M_PI)
        angle -= 2.0 * M_PI;

    while(angle < -M_PI)
        angle += 2.0 * M_PI;

	// replace M_PI in the two lines below to reduce or increase angle

    double left = -1.0 * ( M_PI / 2.0 );
    double right = ( M_PI / 2.0 );

    return( ( angle >= left ) && ( angle <= right ) );
}

bool Object::isInBack(Object* target)
{
	// check if we are behind something ( is the object within a 180 degree slice of our negative y axis )

    double x = m_position.x - target->GetPositionX();
    double y = m_position.y - target->GetPositionY();

    double angle = atan2( y, x );
    angle = ( angle >= 0.0 ) ? angle : 2.0 * M_PI + angle;

	// if we are a unit and have a UNIT_FIELD_TARGET then we are always facing them
	if( m_objectTypeId == TYPEID_UNIT && m_uint32Values[UNIT_FIELD_TARGET] != 0 && TO_UNIT( this )->GetAIInterface()->GetNextTarget() )
	{
		Unit* pTarget = TO_UNIT( this )->GetAIInterface()->GetNextTarget();
		angle -= double( Object::calcRadAngle( target->m_position.x, target->m_position.y, pTarget->m_position.x, pTarget->m_position.y ) );
	}
	else
		angle -= target->GetOrientation();

    while( angle > M_PI)
        angle -= 2.0 * M_PI;

    while(angle < -M_PI)
        angle += 2.0 * M_PI;

	// replace M_H_PI in the two lines below to reduce or increase angle

    double left = -1.0 * ( M_H_PI / 2.0 );
    double right = ( M_H_PI / 2.0 );

    return( ( angle <= left ) && ( angle >= right ) );
}
bool Object::isInArc(Object* target , float angle) // angle in degrees
{
    return inArc( GetPositionX() , GetPositionY() , angle , GetOrientation() , target->GetPositionX() , target->GetPositionY() );
}

bool Object::isInRange(Object* target, float range)
{
	float dist = CalcDistance( target );
	return( dist <= range );
}

bool Object::IsPet()
{
	if( this->GetTypeId() != TYPEID_UNIT )
		return false;

	if( TO_UNIT( this )->m_isPet && m_uint32Values[UNIT_FIELD_CREATEDBY] != 0 && m_uint32Values[UNIT_FIELD_SUMMONEDBY] != 0 )
		return true;

	return false;
}

void Object::_setFaction()
{
	FactionTemplateDBC* factT = nullptr;

	if(GetTypeId() == TYPEID_UNIT || GetTypeId() == TYPEID_PLAYER)
	{
		factT = dbcFactionTemplate.LookupEntry(GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	}
	else
	if(GetTypeId() == TYPEID_GAMEOBJECT)
	{
		factT = dbcFactionTemplate.LookupEntry(GetUInt32Value(GAMEOBJECT_FACTION));
	}

	if(!factT)
	{
		return;
	}
	m_faction = factT;
	m_factionDBC = dbcFaction.LookupEntry(factT->Faction);
}

void Object::UpdateOppFactionSet()
{
	m_oppFactsInRange.clear();
	for(Object::InRangeSet::iterator i = GetInRangeSetBegin(); i != GetInRangeSetEnd(); ++i)
	{
		if (((*i)->GetTypeId() == TYPEID_UNIT) || ((*i)->GetTypeId() == TYPEID_PLAYER) || ((*i)->GetTypeId() == TYPEID_GAMEOBJECT))
		{
			if (isHostile(this, (*i)))
			{
				if(!(*i)->IsInRangeOppFactSet(this))
					(*i)->m_oppFactsInRange.insert(this);
				if (!IsInRangeOppFactSet((*i)))
					m_oppFactsInRange.insert((*i));
				
			}
			else
			{
				if((*i)->IsInRangeOppFactSet(this))
					(*i)->m_oppFactsInRange.erase(this);
				if (IsInRangeOppFactSet((*i)))
					m_oppFactsInRange.erase((*i));
			}
		}
	}
}

void Object::EventSetUInt32Value(uint32 index, uint32 value)
{
	SetUInt32Value(index,value);
}

void Object::DealDamage(Unit *pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras)
{
	Player* plr = 0;

	if( !pVictim || !pVictim->isAlive() || !pVictim->IsInWorld() || !IsInWorld() )
		return;
	if( pVictim->GetTypeId() == TYPEID_PLAYER && TO_PLAYER( pVictim )->GodModeCheat == true )
		return;
	if( pVictim->IsSpiritHealer() )
		return;

	if( this->IsUnit() && pVictim->IsUnit() && pVictim != this )
	{
		// Set our attack target to the victim.
		TO_UNIT( this )->CombatStatus.OnDamageDealt( pVictim );
		
		for (std::map<void*, CallbackBase*>::iterator itr=pVictim->OnTakeDamageCallback.begin(); itr!=pVictim->OnTakeDamageCallback.end(); ++itr)
		{
			if (itr->second == nullptr)
				continue;

			itr->second->execute();
		}
	}
	
	if( pVictim->GetStandState() )//not standing-> standup
	{
		pVictim->SetStandState( STANDSTATE_STAND );//probably mobs also must standup
	}

	// This one is easy. If we're attacking a hostile target, and we're not flagged, flag us.
	// Also, you WONT get flagged if you are dueling that person - FiShBaIt
	if( pVictim->IsPlayer() && IsPlayer() )
	{
		if( isHostile( this, pVictim ) && TO_PLAYER( pVictim )->DuelingWith != TO_PLAYER( this ) )
			TO_PLAYER( this )->SetPvPFlag();
	}
	//If our pet attacks  - flag us.
	if( pVictim->IsPlayer() && IsPet() )
	{
		Player* owner = TO_PLAYER( static_cast< Pet* >( this )->GetPetOwner() );
		if( owner != nullptr )
			if( owner->isAlive() && TO_PLAYER( pVictim )->DuelingWith != owner )
				owner->SetPvPFlag();		
	}

	if(!no_remove_auras)
	{
		//zack 2007 04 24 : root should not remove self (and also other unknown spells)
		if(spellId)
		{
			pVictim->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN,spellId);
			if(Rand(35.0f))
				pVictim->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_UNUSED2,spellId);
		}
		else
		{
			pVictim->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN);
			if(Rand(35.0f))
				pVictim->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_UNUSED2);
		}
	}

	if(this->IsUnit())
	{
/*		if(!pVictim->isInCombat() && pVictim->IsPlayer())
			sHookInterface.OnEnterCombat( TO_PLAYER( pVictim ), TO_UNIT( this ) );

		if(IsPlayer() && ! TO_PLAYER( this )->isInCombat())
			sHookInterface.OnEnterCombat( TO_PLAYER( this ), TO_PLAYER( this ) );*/

		//the black sheep , no actually it is paladin : Ardent Defender
		if(static_cast<Unit*>(this)->DamageTakenPctModOnHP35 && HasFlag(UNIT_FIELD_AURASTATE , AURASTATE_FLAG_HEALTH35) )
			damage = damage - float2int32(damage * static_cast<Unit*>(this)->DamageTakenPctModOnHP35) / 100 ;

		plr = 0;
		if(IsPet())
			plr = static_cast<Pet*>(this)->GetPetOwner();
		else if(IsPlayer())
			plr = TO_PLAYER( this );

		if(pVictim->GetTypeId()==TYPEID_UNIT && plr && plr->GetTypeId() == TYPEID_PLAYER) // Units can't tag..
		{
			// Tagging
			Creature *victim = static_cast<Creature*>(pVictim);
			bool taggable;
			if(victim->GetCreatureName() && victim->GetCreatureName()->Type == CRITTER || victim->IsPet())
				taggable = false;
			else taggable = true;

			if(!victim->Tagged && taggable)
			{
				victim->Tagged = true;
				victim->TaggerGuid = plr->GetGUID();

				/* set loot method */
				if( plr->GetGroup() != nullptr )
					victim->m_lootMethod = plr->GetGroup()->GetMethod();

				// For new players who get a create object
				uint32 Flags = pVictim->m_uint32Values[UNIT_DYNAMIC_FLAGS];
				Flags |= U_DYN_FLAG_TAPPED_BY_PLAYER;

				pVictim->m_uint32Values[UNIT_DYNAMIC_FLAGS] |= U_DYN_FLAG_TAGGED_BY_OTHER;

				// Update existing players.
				ByteBuffer buf(500);
				ByteBuffer buf1(500);

				pVictim->BuildFieldUpdatePacket(&buf1, UNIT_DYNAMIC_FLAGS, Flags);
				pVictim->BuildFieldUpdatePacket(&buf, UNIT_DYNAMIC_FLAGS, pVictim->m_uint32Values[UNIT_DYNAMIC_FLAGS]);

				// Loop inrange set, append to their update data.
				for(std::set<Player*>::iterator itr = m_inRangePlayers.begin(); itr != m_inRangePlayers.end(); ++itr)
				{
					if (TO_PLAYER(plr)->InGroup())
					{
						if (TO_PLAYER(*itr)->GetGroup() && TO_PLAYER(plr)->GetGroup()->GetID() == TO_PLAYER(*itr)->GetGroup()->GetID())
						{
							(*itr)->PushUpdateData(&buf1, 1);
						} 
						else
						{
							(*itr)->PushUpdateData(&buf, 1);
						}
						
					} 
					else
					{
						(*itr)->PushUpdateData(&buf, 1);
					}
				}

				// Update ourselves
				plr->PushUpdateData(&buf1, 1);

			}
		}		
	}

        ///Rage
	if( pVictim->GetPowerType() == POWER_TYPE_RAGE && pVictim != TO_UNIT(this)
	&& pVictim->IsPlayer() && pVictim->CombatStatus.IsInCombat())
	{
		
		float level = (float)TO_UNIT(this)->getLevel();
		float conv = ( 2.5f * 10 ) / (0.0091107836f * level * level + 3.225598133f * level + 4.2652911f);
		float val = damage * conv;
		
		if( pVictim->IsPlayer() )
			val *= ( 1 + ( TO_PLAYER( pVictim )->rageFromDamageTaken / 100.0f ) );
		uint32 rage = pVictim->GetUInt32Value( UNIT_FIELD_POWER2 );
		if( rage + float2int32( val ) > 1000 )
			val = 1000.0f - (float)pVictim->GetUInt32Value( UNIT_FIELD_POWER2 );
		pVictim->ModUnsigned32Value( UNIT_FIELD_POWER2, (int32)val) ;
	}
 

	if( pVictim->IsPlayer() )
	{
		Player *pThis = TO_PLAYER(pVictim);
		if(pThis->cannibalize)
		{
			sEventMgr.RemoveEvents(pVictim, EVENT_CANNIBALIZE);
			pThis->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			pThis->cannibalize = false;
		}
	}

	//* BATTLEGROUND DAMAGE COUNTER *//
	if( pVictim != this )
	{
		if( IsPlayer() )
		{
			plr = TO_PLAYER( this );
		}
		else if( IsPet() )
		{
			plr = TO_PET( this )->GetPetOwner();
			if (plr)
				if( plr != nullptr && plr->GetMapMgr() == GetMapMgr() )
					plr = nullptr;
		}
	}
   
	uint32 health = pVictim->GetUInt32Value(UNIT_FIELD_HEALTH );

	/*------------------------------------ DUEL HANDLERS --------------------------*/
	if((pVictim->IsPlayer()) && (this->IsPlayer()) && TO_PLAYER(pVictim)->DuelingWith == TO_PLAYER( this ) ) //Both Players
	{
		if((health <= damage) && TO_PLAYER( this )->DuelingWith != nullptr)
		{
			//Player in Duel and Player Victim has lost
			uint32 NewHP = pVictim->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/100;

			if(NewHP < 5) 
				NewHP = 5;

			//Set there health to 1% or 5 if 1% is lower then 5
			pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, NewHP);
			//End Duel
			TO_PLAYER( this )->EndDuel(DUEL_WINNER_KNOCKOUT);

			// surrender emote
			pVictim->Emote(EMOTE_ONESHOT_BEG);			// Animation

			return;		  
		}
	}

	if((pVictim->IsPlayer()) && (IsPet())) 
	{
		if((health <= damage) && TO_PLAYER(pVictim)->DuelingWith == static_cast<Pet*>(this)->GetPetOwner())
		{
			Player *petOwner = static_cast<Pet*>(this)->GetPetOwner();
			if(petOwner)
			{
				//Player in Duel and Player Victim has lost
				uint32 NewHP = pVictim->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/100;
				if(NewHP < 5) NewHP = 5;
				
				//Set there health to 1% or 5 if 1% is lower then 5
				pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, NewHP);
				//End Duel
				petOwner->EndDuel(DUEL_WINNER_KNOCKOUT);
				return;
			}
		}
	}
	/*------------------------------------ DUEL HANDLERS END--------------------------*/

	bool isCritter = false;
	if(pVictim->GetTypeId() == TYPEID_UNIT && ((Creature*)pVictim)->GetCreatureName())
	{
			if(((Creature*)pVictim)->GetCreatureName()->Type == CRITTER)
				isCritter = true;
	}
	/* -------------------------- HIT THAT CAUSES VICTIM TO DIE ---------------------------*/
	if ((isCritter || health <= damage) )
	{
		//warlock - seed of corruption
		if( IsUnit() )
		{
			SpellEntry *killerspell;
			if( spellId )
				killerspell = dbcSpell.LookupEntry( spellId );
			else killerspell = nullptr;
			pVictim->HandleProc( CPROC_ON_DIE, TO_UNIT( this ), killerspell );
			pVictim->m_procCounter = 0;
			TO_UNIT( this )->HandleProc( CPROC_TARGET_DIE, pVictim, killerspell );
			TO_UNIT( this )->m_procCounter = 0;
		}
		// check if pets owner is combat participant
		bool owner_participe = false;
		if( IsPet() )
		{
			Player* owner = static_cast<Pet*>( this )->GetPetOwner();
			if( owner != nullptr && pVictim->GetAIInterface()->getThreatByPtr( owner ) > 0 )
				owner_participe = true;
		}
		/* victim died! */
		if( pVictim->IsPlayer() )
			TO_PLAYER( pVictim )->KillPlayer();
		else
		{
			pVictim->setDeathState( JUST_DIED );
			pVictim->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, TO_UNIT( this ), 0);
		}

		if( pVictim->IsPlayer() && (!IsPlayer() || pVictim == this ) )
		{
			TO_PLAYER( pVictim )->DeathDurabilityLoss(0.10);
		}

		/* Zone Under Attack */
        MapInfo * pMapInfo = WorldMapInfoStorage.LookupEntry(GetMapId());
        if( pMapInfo && pMapInfo->type == INSTANCE_NULL && !pVictim->IsPlayer() && !pVictim->IsPet() && ( IsPlayer() || IsPet() ) )
		{
			// Only NPCs that bear the PvP flag can be truly representing their faction.
			if( ((Creature*)pVictim)->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_PVP ) )
			{
				Player * pAttacker = nullptr;
				if( IsPet() )
					pAttacker = static_cast< Pet* >( this )->GetPetOwner();
				else if(IsPlayer())
					pAttacker = TO_PLAYER( this );

				if( pAttacker != nullptr)
                {
				    uint8 teamId = (uint8)pAttacker->GetTeam();
				    if(teamId == 0) // Swap it.
					    teamId = 1;
				    else
					    teamId = 0;
				    uint32 AreaID = pVictim->GetMapMgr()->GetAreaID(pVictim->GetPositionX(), pVictim->GetPositionY());
				    if(!AreaID)
					    AreaID = pAttacker->GetZoneId(); // Failsafe for a shitty TerrainMgr

				    if(AreaID)
				    {
					    WorldPacket data(SMSG_ZONE_UNDER_ATTACK, 4);
					    data << AreaID;
					    sWorld.SendFactionMessage(&data, teamId);
				    }
                }
			}
		}
		
		if(pVictim->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT) > 0)
		{
			if(pVictim->GetCurrentSpell())
			{
				Spell *spl = pVictim->GetCurrentSpell();
				for(int i = 0; i < 3; i++)
				{
					if(spl->m_spellInfo->Effect[i] == SPELL_EFFECT_PERSISTENT_AREA_AURA)
					{
						DynamicObject *dObj = GetMapMgr()->GetDynamicObject(pVictim->GetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT));
						if(!dObj)
							return;
						WorldPacket data(SMSG_GAMEOBJECT_DESPAWN_ANIM, 8);
						data << dObj->GetGUID();
						dObj->SendMessageToSet(&data, false);
						dObj->RemoveFromWorld(true);
						delete dObj;
					}
				}
				if(spl->m_spellInfo->ChannelInterruptFlags == 48140) spl->cancel();
			}
		}
		
		/* Remove all Auras */
		pVictim->DropAurasOnDeath();

		/* Stop players from casting */
		std::set<Player*>::iterator itr;
		for( itr = pVictim->GetInRangePlayerSetBegin() ; itr != pVictim->GetInRangePlayerSetEnd() ; itr ++ )
		{
			//if player has selection on us
			if( (*itr)->GetSelection()==pVictim->GetGUID())							
			{
				if( (*itr)->isCasting() )
					(*itr)->CancelSpell( nullptr ); //cancel current casting spell
			}
		}
		/* Stop victim from attacking */
		if( this->IsUnit() )
			pVictim->smsg_AttackStop( TO_UNIT( this ) );

		if( pVictim->GetTypeId() == TYPEID_PLAYER )
			TO_PLAYER( pVictim )->EventAttackStop();

		/* Set victim health to 0 */
		pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, 0);
		if(pVictim->IsPlayer())
		{
			uint32 self_res_spell = TO_PLAYER( pVictim )->SoulStone;
			TO_PLAYER( pVictim )->SoulStone = TO_PLAYER( pVictim )->SoulStoneReceiver = 0;

			if( !self_res_spell && TO_PLAYER( pVictim )->bReincarnation )
			{
				SpellEntry* m_reincarnSpellInfo = dbcSpell.LookupEntry( 20608 );
				if( TO_PLAYER( pVictim )->Cooldown_CanCast( m_reincarnSpellInfo ) )
				{
					uint32 ankh_count = TO_PLAYER( pVictim )->GetItemInterface()->GetItemCount( 17030 );
					if( ankh_count )
						self_res_spell = 21169;
				}
			}
			pVictim->SetUInt32Value( PLAYER_SELF_RES_SPELL, self_res_spell );
			pVictim->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , 0 );
			//pVictim->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
		}

		/* -------------------------------- HONOR + BATTLEGROUND CHECKS ------------------------ */
		plr = nullptr;
		if( IsPlayer() )
			plr = TO_PLAYER( this );
		else if(IsPet())
			plr = static_cast< Pet* >( this )->GetPetOwner();

		if( plr != nullptr)
		{
			if( plr->m_bg != 0 )
				plr->m_bg->HookOnPlayerKill( plr, pVictim );

			if( pVictim->IsPlayer() )
			{
				sHookInterface.OnKillPlayer( plr, TO_PLAYER( pVictim ) );
				if(plr->getLevel() > pVictim->getLevel())
				{
					unsigned int diff = plr->getLevel() - pVictim->getLevel();
					if( diff <= 8 )
					{
						TO_PLAYER(pVictim)->CombatStatus.RewardHonorForAttackers();
						SetFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR );
					}
					else
					{
						RemoveFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR );
					}
				}
				else
				{
					TO_PLAYER(pVictim)->CombatStatus.RewardHonorForAttackers();
					SetFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR );
				}
			}
			else
			{
				if (!isCritter) // REPUTATION
				{
					plr->Reputation_OnKilledUnit( pVictim, false );
					RemoveFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR );
				}
			}
		}
		/* -------------------------------- HONOR + BATTLEGROUND CHECKS END------------------------ */

		// Wipe our attacker set on death
		pVictim->CombatStatus.Vanished();

		//		 sent to set. don't send it to the party, becuase if they're out of
		//		 range they won't know this guid exists -> possible 132.

		/*if (this->IsPlayer())
			if( TO_PLAYER( this )->InGroup() )
				TO_PLAYER( this )->GetGroup()->SendPartyKillLog( this, pVictim );*/

		/* Stop Unit from attacking */
		if( this->IsPlayer() )
			TO_PLAYER( this )->EventAttackStop();
	   
		if( this->IsUnit() )
		{
			CALL_SCRIPT_EVENT( this, OnTargetDied )( pVictim );
			TO_UNIT( this )->smsg_AttackStop( pVictim );
		
			/* Tell Unit that it's target has Died */
			TO_UNIT( this )->addStateFlag( UF_TARGET_DIED );

			// We will no longer be attacking this target, as it's dead.
			//static_cast<Unit*>(this)->setAttackTarget(nullptr);
		}
		//so now we are completely dead
		//lets see if we have spirit of redemption
		if( pVictim->IsPlayer() )
		{
			if( TO_PLAYER( pVictim)->HasSpell( 20711 ) ) //check for spirit of Redemption
			{
				SpellEntry* sorInfo = dbcSpell.LookupEntry(27827);
				if( sorInfo != nullptr )
				{
					Spell *sor = new Spell( pVictim, sorInfo, true, nullptr );
					SpellCastTargets targets;
					targets.m_unitTarget = pVictim->GetGUID();
					sor->prepare(&targets);
				}
			}
		}
		uint64 victimGuid = pVictim->GetGUID();

		if(pVictim->GetTypeId() == TYPEID_UNIT)
		{
			pVictim->GetAIInterface()->OnDeath(this);
			if(GetTypeId() == TYPEID_PLAYER)
			{
				WorldPacket data(SMSG_PARTYKILLLOG, 16);
				data << GetGUID() << pVictim->GetGUID();
				SendMessageToSet(&data, true);
			}			

			// it Seems that pets some how dont get a name and cause a crash here
			//bool isCritter = (pVictim->GetCreatureName() != nullptr)? pVictim->GetCreatureName()->Type : 0;

			//-----------------------------------LOOOT--------------------------------------------
			if ((!pVictim->IsPet())&& ( !isCritter ))
			{
				Creature * victim = static_cast<Creature*>(pVictim);
				// fill loot vector.
				victim->generateLoot();

				Player *owner = 0;
				if(victim->TaggerGuid)
					owner = GetMapMgr()->GetPlayer( (uint32)victim->TaggerGuid );

				if(owner == 0)  // no owner
				{
					// donno why this would happen, but anyway.. anyone can loot ;p
					// no owner no loot
					//victim->SetFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_LOOTABLE);
				}
				else
				{
					// Build the actual update.
					ByteBuffer buf( 500 );

					uint32 Flags = victim->m_uint32Values[ UNIT_DYNAMIC_FLAGS ];
					Flags |= U_DYN_FLAG_LOOTABLE;
					Flags |= U_DYN_FLAG_TAPPED_BY_PLAYER;

					victim->BuildFieldUpdatePacket( &buf, UNIT_DYNAMIC_FLAGS, Flags );

					// Check for owner's group.
					Group * pGroup = owner->GetGroup();
					if( pGroup != nullptr )
					{
						// Owner was in a party.
						// Check loot method.
						switch( pGroup->GetMethod() )
						{
						case PARTY_LOOT_RR:
/*						//this commented code is not used because it was never tested and finished !
						{
								//get new tagger for creature
								Player *tp = pGroup->GetnextRRlooter();
								if(tp)
								{
									//we force on creature a new tagger
									victim->TaggerGuid = tp->GetGUID();
									victim->Tagged = true;
									if(tp->IsVisible(victim))  // Save updates for non-existant creatures
										tp->PushUpdateData(&buf, 1);
								}
							}break;*/
						case PARTY_LOOT_FFA:
						case PARTY_LOOT_GROUP:
						case PARTY_LOOT_NBG:
							{
								// Loop party players and push update data.
								GroupMembersSet::iterator itr;
								SubGroup * sGrp;
								pGroup->Lock();
								for( uint32 Index = 0; Index < pGroup->GetSubGroupCount(); ++Index )
								{
									sGrp = pGroup->GetSubGroup( Index );
									itr = sGrp->GetGroupMembersBegin();
									for( ; itr != sGrp->GetGroupMembersEnd(); ++itr )
									{
										if( (*itr)->m_loggedInPlayer && (*itr)->m_loggedInPlayer->IsVisible( victim ) )	   // Save updates for non-existant creatures
											(*itr)->m_loggedInPlayer->PushUpdateData( &buf, 1 );
									}
								}
								pGroup->Unlock();
							}break;
						case PARTY_LOOT_MASTER:
							{
								// Master loot: only the loot master gets the update.
								Player * pLooter = pGroup->GetLooter() ? pGroup->GetLooter()->m_loggedInPlayer : nullptr;
								if( pLooter == nullptr )
									pLooter = pGroup->GetLeader()->m_loggedInPlayer;

								if( pLooter->IsVisible( victim ) )  // Save updates for non-existant creatures
									pLooter->PushUpdateData( &buf, 1 );

							}break;
						}
					}
					else
					{
						// Owner killed the mob solo.
						if( owner->IsVisible( victim ) )
							owner->PushUpdateData( &buf, 1 );
					}
				}
			}
			//---------------------------------looot-----------------------------------------  
			
			if( GetTypeId() == TYPEID_PLAYER && 
				pVictim->GetUInt64Value( UNIT_FIELD_CREATEDBY ) == 0 && 
				pVictim->GetUInt64Value( OBJECT_FIELD_CREATED_BY ) == 0 &&
				!pVictim->IsPet() )
			{
				// TODO: lots of casts are bad make a temp member pointer to use for batches like this
				// that way no local loadhitstore and its just one assignment 

				// Is this player part of a group
				if( TO_PLAYER( this)->InGroup() )
				{
					//Calc Group XP
					TO_PLAYER( this )->GiveGroupXP( pVictim, TO_PLAYER( this ) );
					//TODO: pet xp if player in group
				}
				else
				{
					uint32 xp = CalculateXpToGive( pVictim, TO_UNIT( this ) );
					if( xp > 0 )
					{
						TO_PLAYER( this )->GiveXP( xp, victimGuid, true );
						if( TO_PLAYER( this )->GetSummon() && TO_PLAYER( this )->GetSummon()->GetUInt32Value( UNIT_CREATED_BY_SPELL ) == 0 )
						{
							xp = CalculateXpToGive( pVictim, TO_PLAYER( this )->GetSummon() );
							if( xp > 0 )
								TO_PLAYER( this )->GetSummon()->GiveXP( xp );
						}
					}
				}

				if( pVictim->GetTypeId() != TYPEID_PLAYER )
					sQuestMgr.OnPlayerKill( TO_PLAYER( this ), TO_CREATURE( pVictim ) );
			}
			else /* is Creature or GameObject*/
			{
				/* ----------------------------- PET XP HANDLING -------------- */
				if( owner_participe && IsPet() && !pVictim->IsPet() )
				{
					Player* petOwner = static_cast< Pet* >( this )->GetPetOwner();
					if( petOwner != nullptr && petOwner->GetTypeId() == TYPEID_PLAYER )
					{
						if( petOwner->InGroup() )
						{
							//Calc Group XP
							TO_UNIT( this )->GiveGroupXP( pVictim, petOwner );
							//TODO: pet xp if player in group
						}
						else
						{
							uint32 xp = CalculateXpToGive( pVictim, petOwner );
							if( xp > 0 )
							{
								petOwner->GiveXP( xp, victimGuid, true );
								if( !static_cast< Pet* >( this )->IsSummon() )
								{
									xp = CalculateXpToGive( pVictim, static_cast< Pet* >( this ) );
									if( xp > 0 )
										static_cast< Pet* >( this )->GiveXP( xp );
								}
							}
						}
					}
					if( petOwner != nullptr && pVictim->GetTypeId() != TYPEID_PLAYER && 
						pVictim->GetTypeId() == TYPEID_UNIT )
						sQuestMgr.OnPlayerKill( petOwner, TO_CREATURE( pVictim ) );
				}
				/* ----------------------------- PET XP HANDLING END-------------- */

				/* ----------------------------- PET DEATH HANDLING -------------- */
				if( pVictim->IsPet() )
				{
					// dying pet looses 1 happiness level
					if( !static_cast< Pet* >( pVictim )->IsSummon() )
					{
						uint32 hap = static_cast< Pet* >( pVictim )->GetUInt32Value( UNIT_FIELD_POWER5 );
						hap = hap - PET_HAPPINESS_UPDATE_VALUE > 0 ? hap - PET_HAPPINESS_UPDATE_VALUE : 0;
						static_cast< Pet* >( pVictim )->SetUInt32Value( UNIT_FIELD_POWER5, hap );
					}
					
					static_cast< Pet* >( pVictim )->DelayedRemove( false, true );
					
					//remove owner warlock soul link from caster
					Player* owner = static_cast<Pet*>( pVictim )->GetPetOwner();
					if( owner != nullptr )
						owner->EventDismissPet();
				}
				/* ----------------------------- PET DEATH HANDLING END -------------- */
				else if( pVictim->GetUInt64Value( UNIT_FIELD_CHARMEDBY ) )
				{
					//remove owner warlock soul link from caster
					Unit *owner=pVictim->GetMapMgr()->GetUnit( pVictim->GetUInt64Value( UNIT_FIELD_CHARMEDBY ) );
					if( owner != nullptr && owner->IsPlayer())
						TO_PLAYER( owner )->EventDismissPet();
				}
			}
		}
		else if( pVictim->GetTypeId() == TYPEID_PLAYER )
		{
			
			/* -------------------- RESET BREATH STATE ON DEATH -------------- */
			TO_PLAYER( pVictim )->m_UnderwaterTime = 0;
			TO_PLAYER( pVictim )->m_UnderwaterState = 0;
			TO_PLAYER( pVictim )->m_BreathDamageTimer = 0;
			TO_PLAYER( pVictim )->m_SwimmingTime = 0;

			/* -------------------- KILL PET WHEN PLAYER DIES ---------------*/
			if( TO_PLAYER( pVictim )->GetSummon() != nullptr )
			{
				if( pVictim->GetUInt32Value( UNIT_CREATED_BY_SPELL ) > 0 )
					TO_PLAYER( pVictim )->GetSummon()->Dismiss( true );
				else
					TO_PLAYER( pVictim )->GetSummon()->Remove( true, true, true );
			}
			/* -------------------- KILL PET WHEN PLAYER DIES END---------------*/
		}
		else sLog.outError("DealDamage for Unknown Object.");
	}
	else /* ---------- NOT DEAD YET --------- */
	{
		if(pVictim != this /* && updateskill */)
		{
			// Send AI Reaction UNIT vs UNIT
			if( GetTypeId() ==TYPEID_UNIT )
			{
				TO_UNIT( this )->GetAIInterface()->AttackReaction( pVictim, damage, spellId );
			}
			
			// Send AI Victim Reaction
			if( this->IsPlayer() || this->GetTypeId() == TYPEID_UNIT )
			{
				if( pVictim->GetTypeId() != TYPEID_PLAYER )
				{
					TO_CREATURE( pVictim )->GetAIInterface()->AttackReaction( TO_UNIT( this ), damage, spellId );
				}
			}
			if( pVictim->GetTypeId() == TYPEID_PLAYER )
			{
				// Defensive pet
				Pet* pPet = TO_PLAYER( pVictim )->GetSummon();
				if( pPet != nullptr && pPet->GetPetState() != PET_STATE_PASSIVE )
				{
					pPet->GetAIInterface()->AttackReaction( TO_UNIT(this), 1 );
				}
			}
		}
		
		// TODO: Mark victim as a HK
		/*if( TO_PLAYER( pVictim )->GetCurrentBattleground() != nullptr && TO_PLAYER( this )->GetCurrentBattleground() != nullptr)
		{
			
		}*/	

		pVictim->SetUInt32Value( UNIT_FIELD_HEALTH, health - damage );
	}
}

void Object::SpellNonMeleeDamageLog(Unit *pVictim, uint32 spellID, uint32 damage, bool allowProc, bool static_damage, bool no_remove_auras)
{
//==========================================================================================
//==============================Unacceptable Cases Processing===============================
//==========================================================================================
	if(!pVictim || !pVictim->isAlive())
		return;

	SpellEntry *spellInfo = dbcSpell.LookupEntry( spellID );
	if(!spellInfo)
        return;

	if (this->IsPlayer() && !TO_PLAYER( this )->canCast(spellInfo))
		return;
//==========================================================================================
//==============================Variables Initialization====================================
//========================================================================================== 
	uint32 school = spellInfo->School;
	float res = float(damage);
	uint32 aproc = CPROC_ON_HOSTILE_ACTION | PROC_ON_SPELL_HIT;
	uint32 vproc = CPROC_ON_HOSTILE_ACTION | PROC_ON_TAKE_DAMAGE | PROC_TARGET_SPELL_HIT;
	bool critical = false;
//==========================================================================================
//==============================+Spell Damage Bonus Calculations============================
//==========================================================================================
//------------------------------by stats----------------------------------------------------
	if( this->IsUnit() && !static_damage )
	{
		Unit* caster = TO_UNIT( this );
		caster->RemoveAurasByInterruptFlag( AURA_INTERRUPT_ON_START_ATTACK );
		res += caster->GetSpellDmgBonus( pVictim, spellInfo, ( int )res, false );
//==========================================================================================
//==============================Post +SpellDamage Bonus Modifications=======================
//==========================================================================================
		if( res < 0 )
			res = 0;
		else if( spellInfo->spell_can_crit == true )
		{
//------------------------------critical strike chance--------------------------------------	
			// lol ranged spells were using spell crit chance
			float CritChance;
			if( spellInfo->is_ranged_spell )
			{

				if( IsPlayer() )
				{
					CritChance = GetFloatValue( PLAYER_RANGED_CRIT_PERCENTAGE );
					if( pVictim->IsPlayer() )
						CritChance += TO_PLAYER(pVictim)->res_R_crit_get();

					CritChance += (float)(pVictim->AttackerCritChanceMod[spellInfo->School]);
				}
				else
				{
					CritChance = 5.0f; // static value for mobs.. not blizzlike, but an unfinished formula is not fatal :)
				}
			}
			else if( spellInfo->is_melee_spell )
			{
				// Same shit with the melee spells, such as Judgement/Seal of Command
				if( IsPlayer() )
				{
					CritChance = GetFloatValue(PLAYER_CRIT_PERCENTAGE);
				}
				if( pVictim->IsPlayer() )
				{
					CritChance += TO_PLAYER(pVictim)->res_R_crit_get(); //this could be ability but in that case we overwrite the value
				}
				// Victim's (!) crit chance mod for physical attacks?
				CritChance += (float)(pVictim->AttackerCritChanceMod[0]);

			}
			else
			{
				CritChance = caster->spellcritperc + caster->SpellCritChanceSchool[school] + pVictim->AttackerCritChanceMod[school];
				if( caster->IsPlayer() && ( pVictim->m_auracount[SPELL_AURA_MOD_ROOT] - pVictim->m_auracount[SPELL_AURA_MOD_STUN] ) )	
					CritChance += TO_PLAYER( caster )->m_RootedCritChanceBonus;

				if( spellInfo->SpellGroupType )
				{
					SM_FFValue(caster->SM_CriticalChance, &CritChance, spellInfo->SpellGroupType);
				}
			}
			if( CritChance < 0 ) CritChance = 0;
			if( CritChance > 95 ) CritChance = 95;
			critical = Rand(CritChance);
			//sLog.outString( "SpellNonMeleeDamageLog: Crit Chance %f%%, WasCrit = %s" , CritChance , critical ? "Yes" : "No" );
//==========================================================================================
//==============================Spell Critical Hit==========================================
//==========================================================================================
			if (critical)
			{		
				int32 critical_bonus = 100;
				if( spellInfo->SpellGroupType )
					SM_FIValue( caster->SM_PCriticalDamage, &critical_bonus, spellInfo->SpellGroupType );

				if( critical_bonus > 0 )
				{
					// the bonuses are halved by 50% (funky blizzard math :S)
					float b;
					if( spellInfo->School == 0 || spellInfo->is_melee_spell || spellInfo->is_ranged_spell )		// physical || hackfix SoCommand/JoCommand
						b = ( ( float(critical_bonus) ) / 100.0f ) + 1.0f;
					else
						b = ( ( float(critical_bonus) / 2.0f ) / 100.0f ) + 1.0f;
			
					res *= b;
				}

				pVictim->Emote( EMOTE_ONESHOT_WOUNDCRITICAL );
				aproc |= PROC_ON_SPELL_CRIT;
				vproc |= PROC_TARGET_SPELL_CRIT;
			}
		}
	}
//==========================================================================================
//==============================Post Roll Calculations======================================
//==========================================================================================
//------------------------------absorption--------------------------------------------------	
	uint32 ress=(uint32)res;
	uint32 abs_dmg = pVictim->AbsorbDamage(school, &ress);
	uint32 ms_abs_dmg= pVictim->ManaShieldAbsorb(ress);
	if (ms_abs_dmg)
	{
		if(ms_abs_dmg > ress)
			ress = 0;
		else
			ress-=ms_abs_dmg;

		abs_dmg += ms_abs_dmg;
	}

	if(ress < 0) ress = 0;

	res=(float)ress;
	dealdamage dmg;
	dmg.school_type = school;
	dmg.full_damage = ress;
	dmg.resisted_damage = 0;
	
	if(res <= 0) 
		dmg.resisted_damage = dmg.full_damage;

	//------------------------------resistance reducing-----------------------------------------	
	if(res > 0 && this->IsUnit())
	{
		static_cast<Unit*>(this)->CalculateResistanceReduction(pVictim,&dmg,spellInfo);
		if((int32)dmg.resisted_damage > dmg.full_damage)
			res = 0;
		else
			res = float(dmg.full_damage - dmg.resisted_damage);
	}
	//------------------------------special states----------------------------------------------
	if(pVictim->GetTypeId() == TYPEID_PLAYER && TO_PLAYER(pVictim)->GodModeCheat == true)
	{
		res = float(dmg.full_damage);
		dmg.resisted_damage = dmg.full_damage;
	}
	//DK:FIXME->SplitDamage
	// Completed (Supa)
	// Paladin: Blessing of Sacrifice, and Warlock: Soul Link
	// Paladin: Blessing of Sacrifice, and Warlock: Soul Link
	if (pVictim->m_damageSplitTargets.active)
	{
		res = (float)pVictim->DoDamageSplitTarget((uint32)res, school, false);
	}
//==========================================================================================
//==============================Data Sending ProcHandling===================================
//==========================================================================================
	SendSpellNonMeleeDamageLog(this, pVictim, spellID, float2int32(res), school, abs_dmg, dmg.resisted_damage, false, 0, critical, IsPlayer());
	DealDamage( pVictim, float2int32( res ), 2, 0, spellID );
	
	if( this->IsUnit() && allowProc && spellInfo->Id != 25501 )
	{
		pVictim->HandleProc( vproc, TO_UNIT( this ), spellInfo, float2int32( res ) );
		pVictim->m_procCounter = 0;
		TO_UNIT( this )->HandleProc( aproc, pVictim, spellInfo, float2int32( res ) );
		TO_UNIT( this )->m_procCounter = 0;
	}
	if( this->IsPlayer() )
	{
			TO_PLAYER( this )->m_casted_amount[school] = ( uint32 )res;
	}

	

	if( !(dmg.full_damage == 0 && abs_dmg) )
	{
		//Only pushback the victim current spell if it's not fully absorbed
		if( pVictim->GetCurrentSpell() )
			pVictim->GetCurrentSpell()->AddTime( school );
	}

//==========================================================================================
//==============================Post Damage Processing======================================
//==========================================================================================
	if( (int32)dmg.resisted_damage == dmg.full_damage && !abs_dmg )
	{
		//Magic Absorption
		if( pVictim->IsPlayer() )
		{
			if( TO_PLAYER( pVictim )->m_RegenManaOnSpellResist )
			{
				Player* pl = TO_PLAYER( pVictim );
				uint32 maxmana = pl->GetUInt32Value( UNIT_FIELD_MAXPOWER1 );

				//TODO: wtf is this ugly mess of casting bullshit
				uint32 amount = uint32(float( float(maxmana)*pl->m_RegenManaOnSpellResist));

				pVictim->Energize( pVictim, 29442, amount, POWER_TYPE_MANA );
			}
			// we still stay in combat dude
			TO_PLAYER(pVictim)->CombatStatusHandler_ResetPvPTimeout();
		}
		if( IsPlayer() )
			TO_PLAYER(this)->CombatStatusHandler_ResetPvPTimeout();
	}
	if( school == SHADOW_DAMAGE )
	{
		//VampiricEmbrace
		if( pVictim->VampEmbCaster.find( this->GetGUID() ) != pVictim->VampEmbCaster.end() && IsUnit() )
		{
			if( static_cast< Unit* >( this )->isAlive() )
				static_cast< Unit* >( this )->VampiricEmbrace( float2int32( res ), pVictim );
		}
	}
}

//*****************************************************************************************
//* SpellLog packets just to keep the code cleaner and better to read
//*****************************************************************************************

void Object::SendSpellLog(Object *Caster, Object *Target,uint32 Ability, uint8 SpellLogType)
{
	if ((!Caster || !Target) && Ability)
		return;

	WorldPacket data(SMSG_SPELLLOGMISS,28);
	data << Ability;										// spellid
	data << Caster->GetGUID();							  // caster / player
	data << (uint8)1;									   // unknown but I think they are const
	data << (uint32)1;									  // unknown but I think they are const
	data << Target->GetGUID();							  // target
	data << SpellLogType;								   // spelllogtype
	Caster->SendMessageToSet(&data, true);
}


void Object::SendSpellNonMeleeDamageLog( Object* Caster, Object* Target, uint32 SpellID, uint32 Damage, uint8 School, uint32 AbsorbedDamage, uint32 ResistedDamage, bool PhysicalDamage, uint32 BlockedDamage, bool CriticalHit, bool bToset )
{
	if ((!Caster || !Target) && SpellID)
		return;

	WorldPacket data(SMSG_SPELLNONMELEEDAMAGELOG,40);
	data << Target->GetNewGUID();
	data << Caster->GetNewGUID();
	data << SpellID;                    // SpellID / AbilityID
	data << Damage;                     // All Damage
	data << uint8(g_spellSchoolConversionTable[School]);                     // School
	data << AbsorbedDamage;             // Absorbed Damage
	data << ResistedDamage;             // Resisted Damage
	data << uint8(PhysicalDamage);      // Physical Damage (true/false)
	data << uint8(0);                   // unknown or it binds with Physical Damage
	data << BlockedDamage;		     // Physical Damage (true/false)
	data << uint8(CriticalHit ? 7 : 5);                   // unknown const
	data << uint32(0);

	Caster->SendMessageToSet( &data, bToset );
}

int32 Object::event_GetInstanceID()
{
	// return -1 for non-inworld.. so we get our shit moved to the right thread
	if(!IsInWorld())
		return -1;
	else
		return m_instanceId;
}

void Object::EventSpellDamage(uint64 Victim, uint32 SpellID, uint32 Damage)
{
	if(!IsInWorld())
		return;

	Unit * pUnit = GetMapMgr()->GetUnit(Victim);
	if(pUnit == 0) return;

	SpellNonMeleeDamageLog(pUnit, SpellID, Damage, true);
}

bool Object::CanActivate()
{
	switch(m_objectTypeId)
	{
	case TYPEID_UNIT:
		{
			if(!IsPet())
				return true;
		}break;

	case TYPEID_GAMEOBJECT:
		{
			if(static_cast<GameObject*>(this)->HasAI() && GetUInt32Value(GAMEOBJECT_TYPE_ID) != GAMEOBJECT_TYPE_TRAP)
				return true;
		}break;
	}

	return false;
}

void Object::Activate(MapMgr * mgr)
{
	switch(m_objectTypeId)
	{
	case TYPEID_UNIT:
		mgr->activeCreatures.insert((Creature*)this);
		break;

	case TYPEID_GAMEOBJECT:
		mgr->activeGameObjects.insert((GameObject*)this);
		break;
	}

	Active = true;
}

void Object::Deactivate(MapMgr * mgr)
{
	switch(m_objectTypeId)
	{
	case TYPEID_UNIT:
		mgr->activeCreatures.erase((Creature*)this);
		break;

	case TYPEID_GAMEOBJECT:
		mgr->activeGameObjects.erase((GameObject*)this);
		break;
	}
	Active = false;
}

void Object::SetByte(uint32 index, uint32 index1,uint8 value)
{
	ASSERT( index < m_valuesCount );
	// save updating when val isn't changing.
	#ifndef USING_BIG_ENDIAN
	uint8 * v =&((uint8*)m_uint32Values)[index*4+index1];
	#else
	uint8 * v = &((uint8*)m_uint32Values)[index*4+(3-index1)];
	#endif
	if(*v == value)
		return;

	*v = value;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}

}

void Object::SetZoneId(uint32 newZone)
{
	m_zoneId = newZone;
	if( m_objectTypeId == TYPEID_PLAYER && TO_PLAYER( this )->GetGroup() )
		TO_PLAYER( this )->GetGroup()->HandlePartialChange( PARTY_UPDATE_FLAG_ZONEID, TO_PLAYER( this ) );
}

void Object::PlaySoundToSet(uint32 sound_entry)
{
	WorldPacket data(SMSG_PLAY_SOUND, 4);
	data << sound_entry;
	SendMessageToSet(&data, true);
}

void Object::_SetExtension(const string& name, void* ptr)
{
	if( m_extensions == nullptr )
		m_extensions = new ExtensionSet;

	m_extensions->insert( make_pair( name, ptr ) );
}

void Object::SetByteFlag(const uint32 index, const uint32 flag, uint8 newFlag)
{
	if( HasByteFlag(index,flag,newFlag))
		return;

	SetByte(index, flag, GetByte(index,flag)|newFlag);

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}

void Object::RemoveByteFlag(const uint32 index, const uint32 flag, uint8 checkFlag)
{
	if( !HasByteFlag(index,flag,checkFlag))
		return;

	SetByte(index,flag, GetByte(index,flag) & ~checkFlag );

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}

bool Object::HasByteFlag(const uint32 index, const uint32 flag, uint8 checkFlag)
{
	if( GetByte(index,flag) & checkFlag )
		return true;
	else
		return false;
}

bool Object::IsInLineOfSight(Object* pObj)
{
	return (CollideInterface.CheckLOS( GetMapId(), GetPositionX(), GetPositionY(), GetPositionZ() + 2.0f, pObj->GetPositionX(), pObj->GetPositionY(), pObj->GetPositionZ() + 2.0f) );
}
