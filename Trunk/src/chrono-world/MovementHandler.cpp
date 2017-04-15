//
// Chrono Emu (C) 2016
//

#include "StdAfx.h"
#define SWIMMING_TOLERANCE_LEVEL -0.08f
#define MOVEMENT_PACKET_TIME_DELAY 500

#ifdef WIN32

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#define DELTA_EPOCH_IN_USEC  11644473600000000ULL

uint32 TimeStamp()
{
	//return timeGetTime();

	FILETIME ft;
	uint64 t;
	GetSystemTimeAsFileTime(&ft);

	t = (uint64)ft.dwHighDateTime << 32;
	t |= ft.dwLowDateTime;
	t /= 10;
	t -= DELTA_EPOCH_IN_USEC;

	return uint32(((t / 1000000L) * 1000) + ((t % 1000000L) / 1000));
}

CHRONO_INLINE uint32 mTimeStamp()
{
	return timeGetTime();
}

#else

uint32 TimeStamp()
{
	struct timeval tp;
	gettimeofday(&tp, nullptr);
	return (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
}

CHRONO_INLINE uint32 mTimeStamp()
{
	struct timeval tp;
	gettimeofday(&tp, nullptr);
	return (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
}

#endif

void WorldSession::HandleMoveWorldportAckOpcode( WorldPacket & recv_data )
{
	GetPlayer()->SetPlayerStatus(NONE);
	if(_player->IsInWorld())
	{
		// get outta here
		return;
	}
	sLog.outDebug( "WORLD: got MSG_MOVE_WORLDPORT_ACK." );
	
	if(_player->m_CurrentTransporter && _player->GetMapId() != _player->m_CurrentTransporter->GetMapId())
	{
		/* wow, our pc must really suck. */
		Transporter * pTrans = _player->m_CurrentTransporter;
		float c_tposx = pTrans->GetPositionX() + _player->m_TransporterX;
		float c_tposy = pTrans->GetPositionY() + _player->m_TransporterY;
		float c_tposz = pTrans->GetPositionZ() + _player->m_TransporterZ;

		WorldPacket dataw(SMSG_NEW_WORLD, 20);
		dataw << pTrans->GetMapId() << c_tposx << c_tposy << c_tposz << _player->GetOrientation();
		SendPacket(&dataw);
		
		_player->SetMapId(_player->m_CurrentTransporter->GetMapId());
		_player->SetPosition(c_tposx, c_tposy, c_tposz, 0, false);
	}
	else
	{
		_player->m_TeleportState = 2;
		_player->AddToWorld();
	}
}

void WorldSession::HandleMoveTeleportAckOpcode( WorldPacket & recv_data )
{
	uint64 guid;
	recv_data >> guid;
	if(guid == _player->GetGUID())
	{
		sLog.outDebug( "WORLD: got MSG_MOVE_TELEPORT_ACK." );
		GetPlayer()->SetPlayerStatus(NONE);
		if( GetPlayer()->m_auracount[SPELL_AURA_MOD_ROOT] <= 0 )
			GetPlayer()->SetMovement(MOVE_UNROOT,5);
		_player->ResetHeartbeatCoords();

		if(GetPlayer()->GetSummon() != nullptr)		// move pet too
			GetPlayer()->GetSummon()->SetPosition((GetPlayer()->GetPositionX() + 2), (GetPlayer()->GetPositionY() + 2), GetPlayer()->GetPositionZ(), float(M_PI));
		m_isFalling = false;	
		if(_player->m_sentTeleportPosition.x != 999999.0f)
		{
			_player->m_position = _player->m_sentTeleportPosition;
			_player->m_sentTeleportPosition.ChangeCoords(999999.0f,999999.0f,999999.0f);
		}
	}

}

void _HandleBreathing(MovementInfo &movement_info, Player * _player, WorldSession * pSession)
{

	// no water breathing is required
	if( !sWorld.BreathingEnabled || _player->FlyCheat || _player->m_bUnlimitedBreath || !_player->isAlive() || _player->GodModeCheat )
	{
		// player is flagged as in water
		if( _player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING  )
		{
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_SWIMMING;
			if( _player->FlyCheat )
			{
				if(_player->m_lastMoveType != 2)
				{
					_player->m_lastMoveType = 2; // flying
					_player->ResetHeartbeatCoords();
				}
			}
			else
			{
				if(_player->m_lastMoveType != 1)
				{
					_player->m_lastMoveType = 1; // swimming
					_player->ResetHeartbeatCoords();
				}
			}
		}

		// player is flagged as under water
		if( _player->m_UnderwaterState & UNDERWATERSTATE_UNDERWATER )
		{
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;
			WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
			data << uint32(1) << _player->m_UnderwaterTime << _player->m_UnderwaterMaxTime << int32(-1) << uint32(0);
			pSession->SendPacket(&data);
		}

		// player is above water level
		if( pSession->m_bIsWLevelSet )
		{
			if( ( movement_info.z + _player->m_noseLevel ) > pSession->m_wLevel )
			{
				// remove druid aquatic form on land
				if( _player->getClass() == DRUID )
					_player->RemoveAura( 1066 );

				// unset swim session water level
				pSession->m_bIsWLevelSet = false;
			}
		}

		return;
	}

	//player is swiming and not flagged as in the water
	if( movement_info.flags & MOVEFLAG_SWIMMING && !( _player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING ) )
	{
		// dismount if mounted
		if( _player->m_MountSpellId )
			_player->RemoveAura( _player->m_MountSpellId );

		// get water level only if it was not set before
		if( !pSession->m_bIsWLevelSet )
		{
			// water level is somewhere below the nose of the character when entering water
			pSession->m_wLevel = movement_info.z + _player->m_noseLevel * 0.95f;
			pSession->m_bIsWLevelSet = true;
		}

		if( _player->FlyCheat )
		{
			if( _player->m_lastMoveType != 2 )
			{
				_player->m_lastMoveType = 2; // flying
				_player->ResetHeartbeatCoords();
			}
		}
		else
		{
			if( _player->m_lastMoveType != 1 )
			{
				_player->m_lastMoveType = 1; // swimming
				_player->ResetHeartbeatCoords();
			}
		}
		_player->m_UnderwaterState |= UNDERWATERSTATE_SWIMMING;
	}

	// player is not swimming and is not stationary and is flagged as in the water
	if( !( movement_info.flags & MOVEFLAG_SWIMMING ) && !( movement_info.flags & MOVEFLAG_MOVE_STOP ) && _player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING )
	{
		// player is above water level
		if( ( movement_info.z + _player->m_noseLevel ) > pSession->m_wLevel )
		{
			// remove druid aquatic form on land
			if( _player->getClass() == DRUID )
				_player->RemoveAura( 1066 );

			// unset swim session water level
			pSession->m_bIsWLevelSet = false;

			if( _player->m_lastMoveType != 0 )
			{
				_player->m_lastMoveType = 0; // walk or run on land
				_player->ResetHeartbeatCoords();
			}
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_SWIMMING;
		}
	}

	// player is flagged as in the water and is not flagged as under the water
	if( _player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING && !( _player->m_UnderwaterState & UNDERWATERSTATE_UNDERWATER ) )
	{
		//the player is in the water and has gone under water, requires breath bar.
		if( ( movement_info.z + _player->m_noseLevel ) < pSession->m_wLevel )
		{
			_player->m_UnderwaterState |= UNDERWATERSTATE_UNDERWATER;
			WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
			data << uint32(1) << _player->m_UnderwaterTime << _player->m_UnderwaterMaxTime << int32(-1) << uint32(0);
			pSession->SendPacket(&data);
		}
	}

	// player is flagged as in the water and is flagged as under the water
	if( _player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING && _player->m_UnderwaterState & UNDERWATERSTATE_UNDERWATER )
	{
		//the player is in the water but their face is above water, no breath bar neeeded.
		if( ( movement_info.z + _player->m_noseLevel ) > pSession->m_wLevel )
		{
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;
			WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
			data << uint32(1) << _player->m_UnderwaterTime << _player->m_UnderwaterMaxTime << int32(10) << uint32(0);
			pSession->SendPacket(&data);
		}
	}

	// player is flagged as not in the water and is flagged as under the water
	if( !( _player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING ) && _player->m_UnderwaterState & UNDERWATERSTATE_UNDERWATER )
	{
		//the player is out of the water, no breath bar neeeded.
		if( ( movement_info.z + _player->m_noseLevel ) > pSession->m_wLevel )
		{
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;
			WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
			data << uint32(1) << _player->m_UnderwaterTime << _player->m_UnderwaterMaxTime << int32(10) << uint32(0);
			pSession->SendPacket(&data);
		}
	}

}

void WorldSession::HandleMovementOpcodes( WorldPacket & recv_data )
{
	if(!_player->IsInWorld() || _player->m_uint32Values[UNIT_FIELD_CHARMEDBY] || _player->GetPlayerStatus() == TRANSFER_PENDING || _player->GetTaxiState())
		return;

	// spell cancel on movement, for now only fishing is added
	Object * t_go = _player->m_SummonedObject;
	uint32 mstime = mTimeStamp();
	if (t_go)
	{
		if (t_go->GetEntry() == GO_FISHING_BOBBER)
			TO_GAMEOBJECT(t_go)->EndFishing(GetPlayer(),true);
	}


	/************************************************************************/
	/* Make sure the packet is the correct size range.                      */
	/************************************************************************/
	if(recv_data.size() > 80) { Disconnect(); return; }

	/************************************************************************/
	/* Read Movement Data Packet                                            */
	/************************************************************************/
	movement_info.init(recv_data);
    
	/************************************************************************/
	/* Update player movement state                                         */
	/************************************************************************/
	
	if (movement_info.flags & MOVEFLAG_MOVE_STOP|| 
	movement_info.flags & MOVEFLAG_MOVE_FORWARD ||
	movement_info.flags & MOVEFLAG_MOVE_BACKWARD ||
	movement_info.flags & MOVEFLAG_STRAFE_LEFT ||
	movement_info.flags & MOVEFLAG_STRAFE_RIGHT ||
	movement_info.flags & MOVEFLAG_TURN_LEFT ||
	movement_info.flags & MOVEFLAG_TURN_RIGHT ||
	movement_info.flags & MOVEFLAG_WALK ||
	movement_info.flags & MOVEFLAG_FALLING )
	_player->m_isMoving = true;
		
	/************************************************************************/
	/* Remove Emote State                                                   */
	/************************************************************************/
	if(_player->m_uint32Values[UNIT_NPC_EMOTESTATE])
		_player->SetUInt32Value(UNIT_NPC_EMOTESTATE,0);
		
	/************************************************************************/
	/* Make sure the co-ordinates are valid.                                */
	/************************************************************************/
	if( !((movement_info.y >= _minY) && (movement_info.y <= _maxY)) || !((movement_info.x >= _minX) && (movement_info.x <= _maxX)) )
	{
		Disconnect();
		return;
	}

	/************************************************************************/
	/* Dump movement flags - Wheee!                                         */
	/************************************************************************/
#if 0
	printf("=========================================================\n");
	printf("Full movement flags: 0x%.8X\n", movement_info.flags);
	uint32 z, b;
	for(z = 1, b = 1; b < 32;)
	{
		if(movement_info.flags & z)
			printf("   Bit %u (0x%.8X or %u) is set!\n", b, z, z);

		z <<= 1;
		b+=1;
	}
	printf("=========================================================\n");
#endif

	/************************************************************************/
	/* Orientation dumping                                                  */
	/************************************************************************/
#if 0
	printf("Packet: 0x%03X (%s)\n", recv_data.GetOpcode(), LookupName( recv_data.GetOpcode(), g_worldOpcodeNames ) );
	printf("Orientation: %.10f\n", movement_info.orientation);
#endif

	/************************************************************************/
	/* Calculate the timestamp of the packet we have to send out            */
	/************************************************************************/
	size_t pos = (size_t)m_MoverWoWGuid.GetNewGuidLen() + 1;
	int32 move_time;
	if(m_clientTimeDelay == 0)
		m_clientTimeDelay = mstime - movement_info.time;

	/************************************************************************/
	/* Copy into the output buffer.                                         */
	/************************************************************************/
	if(_player->m_inRangePlayers.size())
	{
		move_time = (movement_info.time - (mstime - m_clientTimeDelay)) + MOVEMENT_PACKET_TIME_DELAY + mstime;
		memcpy(&movement_packet[pos], recv_data.contents(), recv_data.size());
		movement_packet[pos+4] = 0;

		/************************************************************************/
		/* Distribute to all inrange players.                                   */
		/************************************************************************/
		for(set<Player*>::iterator itr = _player->m_inRangePlayers.begin(); itr != _player->m_inRangePlayers.end(); ++itr)
		{
			if( (*itr)->GetSession() && (*itr)->IsInWorld() )
			{
				*(uint32*)&movement_packet[pos+4] = uint32(move_time + (*itr)->GetSession()->m_moveDelayTime);
#if defined(ENABLE_COMPRESSED_MOVEMENT) && defined(ENABLE_COMPRESSED_MOVEMENT_FOR_PLAYERS)
				if( _player->GetPositionNC().Distance2DSq((*itr)->GetPosition()) >= World::m_movementCompressThreshold )
					(*itr)->AppendMovementData( recv_data.GetOpcode(), uint16(recv_data.size() + pos), movement_packet );
				else
					(*itr)->GetSession()->OutPacket(recv_data.GetOpcode(), uint16(recv_data.size() + pos), movement_packet);
#else
				(*itr)->GetSession()->OutPacket(recv_data.GetOpcode(), uint16(recv_data.size() + pos), movement_packet);			
#endif
			}
		}
	}

	/************************************************************************/
	/* Falling damage checks                                                */
	/************************************************************************/

	if( _player->blinked) 
	{
		_player->blinked = false;
		_player->m_fallDisabledUntil = UNIXTIME + 5;
	}
	else
	{
		if ( movement_info.flags && MOVEFLAG_FEATHER_FALL || movement_info.flags && MOVEFLAG_TAXI)
		{
		
		}
		else if( recv_data.GetOpcode() == MSG_MOVE_FALL_LAND )
		{
			// player has finished falling
			//if _player->z_axisposition contains no data then set to current position
			if( !_player->z_axisposition )
				_player->z_axisposition = movement_info.z;

			// calculate distance fallen
			uint32 falldistance = float2int32( _player->z_axisposition - movement_info.z );

			/*if player is a rogue or druid(in cat form), then apply -17 modifier to fall distance.
			these checks need improving, low level rogue/druid should not receive this benefit*/
			if( ( _player->getClass() == ROGUE ) || ( _player->GetShapeShift() == FORM_CAT ) )
			{
				if( falldistance > 17 ) 
					falldistance -=17;
				else
					falldistance = 1;
			}

			//checks that player has fallen more than 12 units, otherwise no damage will be dealt
			//falltime check is also needed here, otherwise sudden changes in Z axis position, such as using !recall, may result in death			
			if( _player->isAlive() && !_player->GodModeCheat && falldistance > 12 && ( UNIXTIME >= _player->m_fallDisabledUntil ) && movement_info.FallTime > 1000 )
			{
				// 1.7% damage for each unit fallen on Z axis over 13
				uint32 health_loss = float2int32( float( _player->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) * ( ( falldistance - 12 ) * 0.017 ) ) );
													
				if( health_loss >= _player->GetUInt32Value( UNIT_FIELD_HEALTH ) )
					health_loss = _player->GetUInt32Value( UNIT_FIELD_HEALTH );

				_player->SendEnvironmentalDamageLog( _player->GetGUID(), DAMAGE_FALL, health_loss );
				_player->DealDamage( _player, health_loss, 0, 0, 0 );

				_player->RemoveStealth(); // Fall Damage will cause stealthed units to lose stealth. 
			}
			_player->z_axisposition = 0.0f;
		}
		else
			if( !( movement_info.flags & MOVEFLAG_FALLING ) )
				_player->z_axisposition = movement_info.z;
	}
	/************************************************************************/
	/* Transporter Setup                                                    */
	/************************************************************************/
	if(!_player->m_lockTransportVariables)
	{
		if(_player->m_TransporterGUID && !movement_info.transGuid)
		{
			/* we left the transporter we were on */
			if(_player->m_CurrentTransporter)
			{
				_player->m_CurrentTransporter->RemovePlayer(_player);
				_player->m_CurrentTransporter = nullptr;
			}

			_player->m_TransporterGUID = 0;
			_player->ResetHeartbeatCoords();
		}
		else if(movement_info.transGuid)
		{
			if(!_player->m_TransporterGUID)
			{
				/* just walked into a transport */
				if(_player->IsMounted())
					_player->RemoveAura(_player->m_MountSpellId);

				_player->m_CurrentTransporter = objmgr.GetTransporter(Chrono::Util::GUID_LOPART(movement_info.transGuid));
				if(_player->m_CurrentTransporter)
					_player->m_CurrentTransporter->AddPlayer(_player);;


				/* set variables */
				_player->m_TransporterGUID = movement_info.transGuid;
				_player->m_TransporterUnk = movement_info.transTime;
				_player->m_TransporterX = movement_info.transX;
				_player->m_TransporterY = movement_info.transY;
				_player->m_TransporterZ = movement_info.transZ;
			}
			else
			{
				/* no changes */
				_player->m_TransporterUnk = movement_info.transTime;
				_player->m_TransporterX = movement_info.transX;
				_player->m_TransporterY = movement_info.transY;
				_player->m_TransporterZ = movement_info.transZ;
			}
		}
	}

	/************************************************************************/
	/* Breathing System                                                     */
	/************************************************************************/
	_HandleBreathing(movement_info, _player, this);

	/************************************************************************/
	/* Remove Spells                                                        */
	/************************************************************************/
	_player->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_MOVEMENT);

	/************************************************************************/
	/* Update our position in the server.                                   */
	/************************************************************************/
	if( _player->m_CurrentCharm )
		_player->m_CurrentCharm->SetPosition(movement_info.x, movement_info.y, movement_info.z, movement_info.orientation);
	else
	{
		if(!_player->m_CurrentTransporter) 
		{
			if( !_player->SetPosition(movement_info.x, movement_info.y, movement_info.z, movement_info.orientation) )
			{
				_player->SetUInt32Value(UNIT_FIELD_HEALTH, 0);
				_player->KillPlayer();
			}
		}
		else
		{
			_player->SetPosition(movement_info.x, movement_info.y, movement_info.z, 
				movement_info.orientation + movement_info.transO, false);
		}
	}		
	
	if(  !(movement_info.flags & MOVEFLAG_MOVING_MASK) )
	{
		if( _player->m_isMoving )
		{
			_player->m_isMoving = false;
			_player->m_startMoveTime = 0;
		}
	}
	else
	{
		if( !_player->m_isMoving )
		{
			_player->m_isMoving = true;
			_player->m_startMoveTime = movement_info.time;
			_player->m_lastHeartbeatPosition.ChangeCoords(movement_info.x, movement_info.y, movement_info.z);
		}
	}

	// reset the period every 5 seconds, for a little more accuracy
	if( _player->m_isMoving && (_player->m_lastMoveTime - _player->m_startMoveTime) >= 5000 )
	{
		_player->m_lastHeartbeatPosition.ChangeCoords(movement_info.x, movement_info.y, movement_info.z);
		_player->m_startMoveTime = _player->m_lastMoveTime;
	}
}

void WorldSession::HandleMoveTimeSkippedOpcode( WorldPacket & recv_data )
{
	uint64 guid;
	uint32 time_dif;
	uint8 buf[16];
	StackPacket data(0x319, buf, 16);

	recv_data >> guid;
	recv_data >> time_dif;

	// ignore updates for not us
	if( _player == nullptr || guid != _player->GetGUID() )
		return;

	// send to other players
	data << _player->GetNewGUID();
	data << time_dif;
	_player->SendMessageToSet(&data, false);	
}

void WorldSession::HandleMoveNotActiveMoverOpcode( WorldPacket & recv_data )
{

}


void WorldSession::HandleSetActiveMoverOpcode( WorldPacket & recv_data )
{
 	// set current movement object
	uint64 guid;
	recv_data >> guid;

	if(guid != m_MoverWoWGuid.GetOldGuid())
	{
		// make sure the guid is valid and we aren't cheating
		if( !(_player->m_CurrentCharm && _player->m_CurrentCharm->GetGUID() == guid) &&
			!(_player->GetGUID() == guid) )
		{
			// cheater!
			return;
		}

		// generate wowguid
		if(guid != 0)
			m_MoverWoWGuid.Init(guid);
		else
			m_MoverWoWGuid.Init(_player->GetGUID());

		// set up to the movement packet
		movement_packet[0] = m_MoverWoWGuid.GetNewGuidMask();
		memcpy(&movement_packet[1], m_MoverWoWGuid.GetNewGuid(), m_MoverWoWGuid.GetNewGuidLen());
	}
}

void WorldSession::HandleMoveSplineCompleteOpcode(WorldPacket &recvPacket)
{

}

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket &recvdata)
{
	WorldPacket data(SMSG_MOUNTSPECIAL_ANIM,8);
	data << _player->GetGUID();
	_player->SendMessageToSet(&data, true);
}

void WorldSession::HandleWorldportOpcode(WorldPacket & recv_data)
{
	uint32 mapid;
	float x,y,z,o;
	recv_data >> mapid >> x >> y >> z >> o;
	
	if(!_player->IsInWorld())
		return;

	if(!HasGMPermissions())
	{
		SendNotification("You do not have permission to use this function.");
		return;
	}

	LocationVector vec(x,y,z,o);
	_player->SafeTeleport(mapid,0,vec);
}

void WorldSession::HandleTeleportToUnitOpcode(WorldPacket & recv_data)
{
	uint8 unk;
	Unit * target;
	recv_data >> unk;

	if(!_player->IsInWorld())
		return;

	if(!HasGMPermissions())
	{
		SendNotification("You do not have permission to use this function.");
		return;
	}

	if( (target = _player->GetMapMgr()->GetUnit(_player->GetSelection())) == nullptr )
		return;

	_player->SafeTeleport(_player->GetMapId(), _player->GetInstanceID(), target->GetPosition());
}

void WorldSession::HandleTeleportCheatOpcode(WorldPacket & recv_data)
{
	float x,y,z,o;
	LocationVector vec;

	if(!HasGMPermissions())
	{
		SendNotification("You do not have permission to use this function.");
		return;
	}

	recv_data >> x >> y >> z >> o;
	vec.ChangeCoords(x,y,z,o);
	_player->SafeTeleport(_player->GetMapId(),_player->GetInstanceID(),vec);
}

void MovementInfo::init(WorldPacket & data)
{
	unk13 = 0;
	transGuid = 0;
	data >> flags >> time;
	data >> x >> y >> z >> orientation;

	if (flags & MOVEFLAG_TAXI)
	{																 //Time//
		data >> transGuid >> transX >> transY >> transZ >> transO>> transTime;
	}
	if (flags & MOVEFLAG_SWIMMING)
	{
		data >> pitch;
	}
	if (flags & MOVEFLAG_FALLING || flags & MOVEFLAG_REDIRECTED)
	{
		data >> FallTime >> jump_sinAngle >> jump_cosAngle >> jump_xySpeed;
	}
	if (flags & MOVEFLAG_SPLINE_MOVER)
	{
		data >> spline_unk;
	}

	data >> unklast;
	if(data.rpos() != data.wpos())
	{
		if(data.rpos() + 4 == data.wpos())
			data >> unk13;
		else
			sLog.outDebug("Extra bits of movement packet left");
	}
}

void MovementInfo::write(WorldPacket & data)
{
	data << flags << getMSTime();

	data << x << y << z << orientation;

	if (flags & MOVEFLAG_TAXI)
	{
		data << transGuid << transX << transY << transZ << transO << transTime;
	}
	if (flags & MOVEFLAG_SWIMMING)
	{
		data << pitch;
	}
	if (flags & MOVEFLAG_FALLING)
	{
		data << FallTime << jump_sinAngle << jump_cosAngle << jump_xySpeed;
	}
	if (flags & MOVEFLAG_SPLINE_MOVER)
	{
		data << spline_unk;
	}

	if(unk13)
		data << unk13;
}