// Full Credits to LUAppArc.

#include "StdAfx.h"
#include "LUAEngine.h"
#include "UnitFunctions.h"


namespace luaUnit
{
	int GossipCreateMenu(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		int text_id = luaL_checkint(L, 1);
		Unit * player = Lunar<Unit>::check(L,2);
 		int autosend = luaL_checkint(L, 3);
		if(player->GetTypeId() != TYPEID_PLAYER) return 0;
		Player * plr = TO_PLAYER(player);
		objmgr.CreateGossipMenuForPlayer(&Menu, ptr->GetGUID(), text_id, plr);
		if(autosend)
			Menu->SendTo(plr);
		return 0;
	}
	int GossipMenuAddItem(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		int icon = luaL_checkint(L, 1);
		const char * menu_text = luaL_checkstring(L, 2);
		int IntId = luaL_checkint(L, 3);
		int extra = luaL_checkint(L, 4);

		Menu->AddItem(icon, menu_text, IntId, (extra > 0)? true : false);
		return 0;
	}
	int GossipSendMenu(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		Menu->SendTo(TO_PLAYER(ptr));
		return 0;
	}
	int GossipComplete(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		TO_PLAYER(ptr)->Gossip_Complete();
		return 0;
	}

	int GossipSendPOI(lua_State * L, Unit * ptr)
	{
		Unit * target = Lunar<Unit>::check(L, 1);
		if(target->GetTypeId() != TYPEID_PLAYER) return 0;
		shared_ptr<Player> plr = TO_PLAYER(target);
		float x = (float)luaL_checknumber(L, 2);
		float y = (float)luaL_checknumber(L, 3);
		int icon = luaL_checkint(L, 4);
		int flags = luaL_checkint(L, 5);
		int data = luaL_checkint(L, 6);
		const char * name = luaL_checkstring(L, 7);

		plr->Gossip_SendPOI(x, y, icon, flags, data, name);
		return 0;
	}
	int IsPlayer(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL ||( ptr->GetTypeId() != TYPEID_UNIT && ptr->GetTypeId() != TYPE_PLAYER)) return 0;
		lua_pushboolean(L,(ptr->IsPlayer())?1:0);
		return 1;
	}
	int IsCreature(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL ||( ptr->GetTypeId() != TYPEID_UNIT && ptr->GetTypeId() != TYPE_PLAYER)) return 0;
		lua_pushboolean(L,(ptr->IsCreature())?1:0);
		return 1;
	}
	int Emote(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 emote_id = luaL_checkint(L, 1);
		uint32 time = luaL_checkint(L, 2);
		if(emote_id==0) return 0;
		if (time)
			ptr->EventAddEmote((EmoteType)emote_id,time);
		else
			ptr->Emote((EmoteType)emote_id);
		return 0;
	}
	int GetManaPct(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		if (ptr->GetPowerType() == POWER_TYPE_MANA)
			lua_pushnumber(L, (int)(ptr->GetUInt32Value(UNIT_FIELD_POWER1) * 100.0f / ptr->GetUInt32Value(UNIT_FIELD_MAXPOWER1)));
		else
			lua_pushnil(L);
		return 1;
	}
	int GetName(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();

		switch(ptr->GetTypeId())
		{
		case TYPEID_UNIT:
			lua_pushstring(L, TO_CREATURE(ptr)->GetCreatureName() ? TO_CREATURE(ptr)->GetCreatureName()->Name : "Unknown");
			break;

		case TYPEID_PLAYER:
			lua_pushstring(L, TO_PLAYER(ptr)->GetName());
			break;

		default:
			lua_pushstring(L, "Unknown");
			break;
		}

		return 1;
	}
	int SendChatMessage(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 typ = luaL_checkint(L, 1);
		uint32 lang = luaL_checkint(L, 2);
		const char * message = luaL_checklstring(L, 3, NULL);
		if(message == NULL)
			return 0;

		ptr->SendChatMessage(typ, lang, message);
		return 0;
	}
	int MoveTo(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		double x = luaL_checknumber(L, 1);
		double y = luaL_checknumber(L, 2);
		double z = luaL_checknumber(L, 3);
		double o = luaL_checknumber(L, 4);

		if(x == 0 || y == 0 || z == 0)
			return 0;

		ptr->GetAIInterface()->MoveTo( (float)x, (float)y, (float)z, (float)o );
		return 0;
	}

	int SetMovementType(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 typ = luaL_checkint(L, 1);
		ptr->GetAIInterface()->setMoveType(typ);
		return 0;
	}

	int GetX(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		lua_pushnumber(L, (double)ptr->GetPositionX());
		return 1;
	}

	int GetY(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		lua_pushnumber(L, (double)ptr->GetPositionY());
		return 1;
	}

	int GetZ(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		lua_pushnumber(L, (double)ptr->GetPositionZ());
		return 1;
	}

	int GetO(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		lua_pushnumber(L, (double)ptr->GetOrientation());
		return 1;
	}

	int CastSpell(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 sp = luaL_checkint(L,1);
		if(ptr->GetTypeId() == TYPEID_UNIT)
			TO_CREATURE(ptr)->CastSpell(TO_UNIT(ptr),sp,true);
		else if( ptr->GetTypeId() == TYPEID_PLAYER)
			TO_PLAYER(ptr)->CastSpell(TO_UNIT(ptr),sp,true);
		return 0;
	}

	int FullCastSpell(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 sp = luaL_checkint(L,1);
		if(ptr->GetTypeId() == TYPEID_UNIT)
			TO_CREATURE(ptr)->CastSpell(TO_UNIT(ptr),sp,false);
		else if(ptr->GetTypeId() == TYPEID_PLAYER)
			TO_PLAYER(ptr)->CastSpell(TO_UNIT(ptr),sp,false);
		return 0;
	}
	int CastSpellOnTarget(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 sp = luaL_checkint(L, 1);
		Unit * target = Lunar<Unit>::check(L, 2);

		if(sp==0 || target==NULL) return 0;
		if(ptr->GetTypeId() == TYPEID_UNIT)
			TO_CREATURE(ptr)->CastSpell(target,sp,true);
		else if(ptr->GetTypeId() == TYPEID_PLAYER)
			TO_PLAYER(ptr)->CastSpell(target,sp,true);
		return 0;
	}
	int FullCastSpellOnTarget(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 sp = luaL_checkint(L, 1);
		Unit * target = Lunar<Unit>::check(L, 2);

		if(sp==0 || target==NULL) return 0;
		if(ptr->GetTypeId() == TYPEID_UNIT)
			TO_CREATURE(ptr)->CastSpell(target,sp,false);
		else if(ptr->GetTypeId() == TYPEID_PLAYER)
			TO_PLAYER(ptr)->CastSpell(target,sp,false);
		return 0;
	}
	int SpawnCreature(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();

		uint32 entry_id = luaL_checkint(L, 1);
		float x = (float)luaL_checknumber(L, 2);
		float y = (float)luaL_checknumber(L, 3);
		float z = (float)luaL_checknumber(L, 4);
		float o = (float)luaL_checknumber(L, 5);
		uint32 faction = luaL_checkint(L, 6);
		uint32 duration = luaL_checkint(L, 7);

		if( !x || !y || !z || !entry_id || !faction) return 0;

		CreatureProto * proto = CreatureProtoStorage.LookupEntry(entry_id);
		CreatureInfo * inf = CreatureNameStorage.LookupEntry(entry_id);

		if(!proto || !inf) return 0;

		Creature * pC = ptr->GetMapMgr()->GetInterface()->SpawnCreature(entry_id,x,y,z,o,false,false,0,0);
		if(pC == NULL) return 0;
		pC->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,faction);
		pC->_setFaction();
		pC->SetInstanceID(ptr->GetInstanceID());
		pC->SetMapId(ptr->GetMapId());
		pC->PushToWorld(ptr->GetMapMgr());
		if(duration)
		{
			sEventMgr.AddEvent(pC,&Creature::SafeDelete,EVENT_CREATURE_UPDATE,duration,1,0);
		}
		Lunar<Unit>::push(L,TO_UNIT(pC));
		return 1;
	}
	int SpawnGameObject(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();

		uint32 entry_id = luaL_checkint(L, 1);
		float x = (float)luaL_checknumber(L, 2);
		float y = (float)luaL_checknumber(L, 3);
		float z = (float)luaL_checknumber(L, 4);
		float o = (float)luaL_checknumber(L, 5);
		uint32 duration = luaL_checkint(L, 6);

		if(!entry_id) return 0;

		GameObjectInfo * goi = GameObjectNameStorage.LookupEntry(entry_id);
		if(!goi) return 0;

		GameObject * pC = ptr->GetMapMgr()->GetInterface()->SpawnGameObject(entry_id,x,y,z,o,false,0,0);
		if(pC == NULL) return 0;
		pC->SetMapId(ptr->GetMapId());
		pC->SetInstanceID(ptr->GetInstanceID());
		pC->Spawn(ptr->GetMapMgr());
		if(duration)
		{
			sEventMgr.AddEvent(pC, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, duration, 1, 0);
		}
		Lunar<GameObject>::push(L,pC);
		return 1;
	}
	int RegisterEvent(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();

		const char * func_to_call = luaL_checkstring(L,1);
		int delay=luaL_checkint(L,2);
		int repeats=luaL_checkint(L,3);

		if(!func_to_call||!delay)
			return 0;

		shared_ptr<Creature> pCreature = TO_CREATURE(ptr);
		string strFunc = string(func_to_call);
		sEventMgr.AddEvent(pCreature, &Creature::TriggerScriptEvent, strFunc, EVENT_CREATURE_UPDATE, (uint32)delay, (uint32)repeats, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		return 0;
	}
	int RemoveEvents(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();

		shared_ptr<Creature> pCreature = TO_CREATURE(ptr);
		sEventMgr.RemoveEvents(pCreature);
		return 0;
	}

	int SetFaction(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL || (ptr->GetTypeId() !=  TYPEID_UNIT && ptr->GetTypeId() != TYPEID_PLAYER) ) return 0;
		int faction = luaL_checkint(L,1);
		if(!faction)
			return 0;

		ptr->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,faction);
		ptr->_setFaction();
		return 0;
	}
	int SetStandState(lua_State * L, Unit * ptr) //states 0..8
	{
		CHECK_PTR();
		int state = luaL_checkint(L,1);
		if(state<=0)
			return 0;
		ptr->SetStandState(state);
		return 0;
	}
	int IsInCombat(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		lua_pushboolean(L,(ptr->CombatStatus.IsInCombat())?1:0);
		return 1;
	}

	int SetScale(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		double faction = luaL_checknumber(L,1);
		if(faction==0.0)
			return 0;

		ptr->SetFloatValue(OBJECT_FIELD_SCALE_X,(float)faction);
		return 0;
	}

	int SetModel(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();

		int modelid = luaL_checkint(L,1);
		if(modelid==0)
			return 0;

		ptr->SetUInt32Value(UNIT_FIELD_DISPLAYID,modelid);
		return 0;
	}
	int SetCombatCapable(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		int enabled = luaL_checkint(L,1);
		ptr->GetAIInterface()->disable_combat = (enabled > 0) ? true : false;
		//ptr->GetAIInterface()->SetAllowedToEnterCombat(false);
		return 0;
	}

	int SetCombatMeleeCapable(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		int enabled = luaL_checkint(L,1);
		ptr->GetAIInterface()->disable_melee = (enabled > 0) ? true : false;
		return 0;
	}

	int SetCombatRangedCapable(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		int enabled = luaL_checkint(L,1);
		ptr->GetAIInterface()->disable_ranged = (enabled > 0) ? true : false;
		return 0;
	}

	int SetCombatSpellCapable(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		int enabled = luaL_checkint(L,1);
		ptr->GetAIInterface()->disable_spell = (enabled > 0) ? true : false;
		return 0;
	}

	int SetCombatTargetingCapable(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		int enabled = luaL_checkint(L,1);
		ptr->GetAIInterface()->disable_targeting = (enabled > 0) ? true : false;
		return 0;
	}

	int DestroyCustomWaypointMap(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		TO_CREATURE(ptr)->DestroyCustomWaypointMap();
		return 0;
	}

	int CreateCustomWaypointMap(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		shared_ptr<Creature> pCreature = TO_CREATURE(ptr);
		if(pCreature->m_custom_waypoint_map)
		{
			for(WayPointMap::iterator itr = pCreature->m_custom_waypoint_map->begin(); itr != pCreature->m_custom_waypoint_map->end(); ++itr)
				delete (*itr);
			delete pCreature->m_custom_waypoint_map;
		}

		pCreature->m_custom_waypoint_map = new WayPointMap;
		pCreature->GetAIInterface()->SetWaypointMap(pCreature->m_custom_waypoint_map);
		return 0;
	}

	int CreateWaypoint(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 wpid = luaL_checkint(L,1);
		float x = luaL_checknumber(L,2);
		float y = luaL_checknumber(L,3);
		float z = luaL_checknumber(L,4);
		float o = luaL_checknumber(L,5);
		int waittime = luaL_checkint(L,6);
		int flags = luaL_checkint(L,7);
		int modelid = luaL_checkint(L,8);
		shared_ptr<Creature> pCreature = TO_CREATURE(ptr);
		if(pCreature == NULL) return 0;

		if(!modelid)
			modelid = pCreature->GetUInt32Value(UNIT_FIELD_DISPLAYID);

		WayPoint * wp = pCreature->CreateWaypointStruct();
		wp->backwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardskinid = 0;
		wp->flags = flags;
		wp->forwardemoteid = 0;
		wp->forwardemoteoneshot = 0;
		wp->forwardskinid = modelid;
		wp->id = wpid;
		wp->o = o;
		wp->waittime = waittime;
		wp->x = x;
		wp->y = y;
		wp->z = z;
		pCreature->GetAIInterface()->addWayPoint(wp);

		return 0;
	}

	int MoveToWaypoint(lua_State * L, Unit * ptr)
	{
		CHECK_PTR(); // players w/ ai enabled can move to wps.
		uint32 id = luaL_checkint(L,1);
		ptr->GetAIInterface()->setWaypointToMove(id);
		return 0;
	}

	int RemoveItem(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		int id = luaL_checkint(L,1);
		int count = luaL_checkint(L,2);

		TO_PLAYER(ptr)->GetItemInterface()->RemoveItemAmt(id,count);
		return 0;
	}

	int AddItem(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		int id = luaL_checkint(L,1);
		int count = luaL_checkint(L,2);

		shared_ptr<Player> plr = TO_PLAYER(ptr);
		ItemPrototype * proto = ItemPrototypeStorage.LookupEntry(id);
		if(proto==NULL)
			return 0;

		shared_ptr<Item> add = plr->GetItemInterface()->FindItemLessMax(id,count,false);
		if(add==NULL)
		{
			add=objmgr.CreateItem(id,plr);
			add->SetUInt32Value(ITEM_FIELD_STACK_COUNT,count);
			if(plr->GetItemInterface()->AddItemToFreeSlot(add))
				plr->GetSession()->SendItemPushResult(add,false,true,false,true,plr->GetItemInterface()->LastSearchItemBagSlot(),plr->GetItemInterface()->LastSearchItemSlot(),count);
		}
		else
		{
			add->ModUnsigned32Value(ITEM_FIELD_STACK_COUNT,count);
			plr->GetSession()->SendItemPushResult(add,false,true,false,false,plr->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()),0xFFFFFFFF,count);
		}

		return 0;
	}
	int GetInstanceID(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		if( ptr->GetMapMgr()->GetMapInfo()->type == INSTANCE_NULL)
			lua_pushnil(L);
		else
			lua_pushinteger(L,ptr->GetInstanceID());
		return 1;
	}

	int GetClosestPlayer(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();

		float dist, d2;
		Player * ret = NULLPLR;

		for(PlayerSet::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
		{
			d2=(*itr)->GetDistanceSq(ptr);
			if(!ret||d2<dist)
			{
				dist=d2;
				ret=(*itr);
			}
		}

		if(ret==NULL)
			lua_pushnil(L);
		else
			Lunar<Unit>::push(L,ret);

		return 1;
	}
	int GetRandomPlayer(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		if(!ptr->HasInRangeObjects())
		{
			lua_pushnil(L);
			return 1;
		}

		int flag = luaL_checkint( L, 1 );

		shared_ptr<Player> ret = NULLPLR;

		switch( flag )
		{
		case RANDOM_ANY:
			{
				std::vector<shared_ptr<Player>>PlayerTable;
				for(PlayerSet::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					PlayerTable.push_back(*itr);
				}
				if( PlayerTable.size() != 0 )
					ret = PlayerTable[RandomUInt( (uint32)PlayerTable.size()-1)];
			}break;
		case RANDOM_IN_SHORTRANGE:
			{
				std::vector<shared_ptr<Player>>PlayerTable;
				for(PlayerSet::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					if(ptr->CalcDistance( *itr) <= 8)
						PlayerTable.push_back(*itr);
				}
				if(PlayerTable.size() != 0)
					ret = PlayerTable[RandomUInt( (uint32)PlayerTable.size()-1)];
			}break;
		case RANDOM_IN_MIDRANGE:
			{
				std::vector<shared_ptr<Player>>PlayerTable;
				for(PlayerSet::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					if( ptr->GetDistanceSq(*itr) > 8 && ptr->GetDistanceSq(*itr) <=20 )
						PlayerTable.push_back(*itr);
				}
				if(PlayerTable.size() != 0)
					ret = PlayerTable[RandomUInt( (uint32)PlayerTable.size()-1)];
			}break;
		case RANDOM_IN_LONGRANGE:
			{
				std::vector<shared_ptr<Player>>PlayerTable;
				for(PlayerSet::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					if( ptr->GetDistanceSq(*itr) < 20)
						PlayerTable.push_back(*itr);
				}
				if( PlayerTable.size() != 0 )
					ret = PlayerTable[RandomUInt( (uint32)PlayerTable.size()-1)];
			}break;
		case RANDOM_WITH_MANA:
			{
				std::vector<shared_ptr<Player>>PlayerTable;
				for(PlayerSet::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					if( (*itr)->GetPowerType() == POWER_TYPE_MANA )
						PlayerTable.push_back( *itr);
				}
				if( PlayerTable.size() != 0 )
					ret = PlayerTable[RandomUInt( (uint32)PlayerTable.size()-1)];
			}break;
		case RANDOM_WITH_ENERGY:
			{
				std::vector<shared_ptr<Player>>PlayerTable;
				for(PlayerSet::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					if ( (*itr)->GetPowerType() == POWER_TYPE_ENERGY)
						PlayerTable.push_back( *itr);
				}
				if (PlayerTable.size() != 0 )
					ret = PlayerTable[RandomUInt( (uint32)PlayerTable.size()-1)];
			}break;
		case RANDOM_WITH_RAGE:
			{
				std::vector<shared_ptr<Player>>PlayerTable;
				for(PlayerSet::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					if( (*itr)->GetPowerType() == POWER_TYPE_RAGE )
						PlayerTable.push_back( *itr);
				}
				if( PlayerTable.size() != 0 )
					ret = PlayerTable[RandomUInt( (uint32)PlayerTable.size()-1)];
			}break;
		case RANDOM_NOT_MAINTANK:
			{
				std::vector<shared_ptr<Player>>PlayerTable;
				for(PlayerSet::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					if( (*itr) != ptr->GetAIInterface()->GetMostHated() )
						PlayerTable.push_back(*itr);
				}
				if( PlayerTable.size() != 0 )
					ret = PlayerTable[RandomUInt( (uint32)PlayerTable.size()-1)];
			}break;
		}
		if(ret==NULL)
			lua_pushnil(L);
		else
			Lunar<Unit>::push(L,ret);

		return 1;
	}
	int GetRandomFriend(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		if(!ptr->HasInRangeObjects())
		{
			lua_pushnil(L);
			return 1;
		}
		std::vector<Unit *>Creatures;
		for(ObjectSet::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); ++itr)
		{
			if( (*itr)->IsUnit() && isFriendly(ptr,(*itr)) )
			{
				Unit * pTemp = TO_UNIT((*itr));
				Creatures.push_back(pTemp);
			}
		}
		if( Creatures.size() != 0 )
			Lunar<Unit>::push(L,Creatures[RandomUInt( (uint32)Creatures.size()-1)]);
		return 1;
	}
	int GetRandomEnemy(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		if(!ptr->HasInRangeObjects())
		{
			lua_pushnil(L);
			return 1;
		}
		std::vector<Unit *>Enemies;
		for(ObjectSet::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); ++itr)
		{
			if( (*itr)->IsUnit() && isHostile(ptr,(*itr)) )
			{
				Unit * pTemp = TO_UNIT((*itr));
				Enemies.push_back( pTemp);
			}
		}
		if( Enemies.size() != 0 )
			Lunar<Unit>::push(L,Enemies[RandomUInt( (uint32)Enemies.size()-1)]);
		return 1;
	}

	int StopMovement(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		int tim = luaL_checkint(L,1);
		ptr->GetAIInterface()->StopMovement(tim);
		return 0;
	}

	int RemoveAura(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		int auraid = luaL_checkint(L,1);
		ptr->RemoveAura(auraid);
		return 0;
	}

	int PlaySoundToSet(lua_State * L, Unit * ptr)
	{
		if(!ptr) return 0;
		int soundid = luaL_checkint(L,1);
		ptr->PlaySoundToSet(soundid);
		return 0;
	}

	int GetUnitBySqlId(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 sqlid = luaL_checkint(L,1);
		shared_ptr<Creature> pC = ptr->GetMapMgr()->GetSqlIdCreature(sqlid);
		CreatureProto * proto = CreatureProtoStorage.LookupEntry(sqlid);
		if(proto == NULL)
			lua_pushnil(L);
		Lunar<Unit>::push(L,pC);
		return 1;
	}

	int Despawn(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		int delay = luaL_checkint(L,1);
		int respawntime = luaL_checkint(L,2);
	
		return 0;
	}

	int GetHealthPct(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL)
			lua_pushinteger(L,0);
		else
			lua_pushinteger(L, ptr->GetHealthPct());

		return 1;
	}
	int SetHealthPct(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		if(ptr->GetUInt32Value(UNIT_FIELD_HEALTH) == 0 && ptr->getDeathState() != ALIVE)
			return 0;
		int val = luaL_checkint(L,1);
		if (val>0)
			ptr->SetHealthPct(val);
		return 1;
	}
	int GetItemCount(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID_RET_INT(TYPEID_PLAYER);
		int itemid = luaL_checkint(L,1);
		lua_pushinteger(L, TO_PLAYER(ptr)->GetItemInterface()->GetItemCount(itemid,false));
		return 1;
	}

	int GetMainTank(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID_RET_INT(TYPEID_UNIT);
		shared_ptr<Unit> ret = ptr->GetAIInterface()->GetMostHated();
		if(ret==NULL)
			lua_pushnil(L);
		else
			Lunar<Unit>::push(L,ret);
		return 1;
	}
	int GetAddTank(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID_RET_INT(TYPEID_UNIT);
		shared_ptr<Unit> ret = ptr->GetAIInterface()->GetSecondHated();
		if(ret==NULL)
			lua_pushnil(L);
		else
			Lunar<Unit>::push(L,ret);
		return 1;
	}
	int ClearThreatList(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID_RET_INT(TYPEID_UNIT);
		ptr->GetAIInterface()->ClearHateList();
		return 1;
	}

	int GetTauntedBy(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT)
		if (!ptr->GetAIInterface()->getTauntedBy())
			lua_pushnil(L);
		else
			Lunar<Unit>::push(L,(ptr->GetAIInterface()->getTauntedBy()),false);
		return 1;
	}
	int SetTauntedBy(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		Unit * target = Lunar<Unit>::check(L,1);
		if (!target || ptr->GetAIInterface()->GetIsTaunted() || target==ptr)
			return 0;
		else
			ptr->GetAIInterface()->taunt(TO_UNIT(target));
		return 1;
	}

	int GetSoulLinkedWith(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		/*if(ptr->GetAIInterface()->getSoullinkedWith() == NULL)
			lua_pushnil(L);
		else
			Lunar<Unit>::push(L,(ptr->GetAIInterface()->getSoullinkedWith());*/
		return 1;
	}
	int SetSoulLinkedWith(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		/*Unit * target = Lunar<Unit>::check(L, 1);
		if (!target || ptr->GetAIInterface()->GetIsSoulLinked() || target==ptr)
			return 0;
		else
			ptr->GetAIInterface()->SetSoulLinkedWith(TO_UNIT(ptr));*/
		return 0;
	}
	int ChangeTarget(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		Unit * target = Lunar<Unit>::check(L,1);
		if (!target || !isHostile(TO_OBJECT(ptr),TO_OBJECT(target)) || ptr==target )
			return 0;
		else
			ptr->GetAIInterface()->SetNextTarget(TO_UNIT(target));
		return 0;
	}

	int HasFinishedQuest(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID_RET(TYPEID_PLAYER);
		int questid = luaL_checkint(L,1);
		if( TO_PLAYER(ptr)->HasFinishedQuest(questid) )
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);

		return 1;
	}

	int UnlearnSpell(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		int spellid = luaL_checkint(L,1);
		TO_PLAYER( ptr)->removeSpell(spellid,false,false,0);
		return 0;
	}

	int LearnSpell(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		int spellid = luaL_checkint(L,1);
		TO_PLAYER( ptr)->addSpell(spellid);
		return 0;
	}

	int MarkQuestObjectiveAsComplete(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		int questid = luaL_checkint(L,1);
		int objective = luaL_checkint(L,2);
		shared_ptr<Player> pl = TO_PLAYER( ptr);
		QuestLogEntry * qle = pl->GetQuestLogForEntry(questid);
		qle->SetMobCount(objective, qle->GetQuest()->required_mobcount[objective]);
		qle->SendUpdateAddKill(objective);
		if(qle->CanBeFinished())
			qle->SendQuestComplete();
		qle->UpdatePlayerFields();

		return 0;
	}

	/*int KnockBack(lua_State * L, Unit * ptr)
	{
		if(!ptr) return 0;
		double dx = luaL_checknumber(L,1);
		double dy = luaL_checknumber(L,2);
		double affect1 = luaL_checknumber(L,3);
		double affect2 = luaL_checknumber(L,4);
		WorldPacket data(SMSG_MOVE_KNOCK_BACK, 30);
		data << ptr->GetNewGUID();
		data << getMSTime();
		data << dx << dy << affect1 << affect2;
		ptr->SendMessageToSet(&data, true);

		return 0;
	}*/
	int SendAreaTriggerMessage(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		const char * msg = luaL_checkstring(L,1);
		if(!msg) return 0;
		TO_PLAYER( ptr)->SendAreaTriggerMessage(msg);
		return 0;
	}

	int SendBroadcastMessage(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		shared_ptr<Player> plr = TO_PLAYER( ptr);
		const char * msg = luaL_checkstring(L,1);
		if(!msg) return 0;
		plr->BroadcastMessage(msg);
		return 0;
	}

	int TeleportUnit(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		uint32 mapId = luaL_checkint(L, 1);
		float posX = (float)luaL_checknumber(L, 2);
		float posY = (float)luaL_checknumber(L, 3);
		float posZ = (float)luaL_checknumber(L, 4);
		if(posX == 0 || posY == 0 || posZ == 0)
			return 0;
		LocationVector vec(posX, posY, posZ);
		TO_PLAYER( ptr)->SafeTeleport(mapId, 0, vec);
		return 0;
	}

	int GetHealth(lua_State * L, Unit * ptr)
	{
		if( ptr == NULL )
			lua_pushnil(L);
		else
			lua_pushinteger( L, ptr->GetUInt32Value( UNIT_FIELD_HEALTH ) );
		return 1;
	}

	int GetMaxHealth(lua_State * L, Unit * ptr)
	{
		if( ptr == NULL )
			lua_pushnil(L);
		else
			lua_pushinteger( L, ptr->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
		return 1;
	}

	int SetHealth(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		if(ptr->GetUInt32Value(UNIT_FIELD_HEALTH) == 0 && ptr->getDeathState() != ALIVE) return 0;
		int val = luaL_checkint( L, 1 );
		if( ptr != NULL && val > 0 )
		{
			if( (uint32)val > ptr->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) )
				ptr->SetUInt32Value( UNIT_FIELD_HEALTH, ptr->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			else
				ptr->SetUInt32Value( UNIT_FIELD_HEALTH, val );
		}
		return 0;
	}

	int SetMaxHealth(lua_State * L, Unit * ptr)
	{
		int val = luaL_checkint( L, 1 );
		if( ptr != NULL && val > 0 )
		{
			if( (uint32)val < ptr->GetUInt32Value( UNIT_FIELD_HEALTH ) )
			{
			ptr->SetUInt32Value( UNIT_FIELD_HEALTH, val );
			ptr->SetUInt32Value( UNIT_FIELD_MAXHEALTH, val );
			}
		}
		return 0;
	}

	int WipeHateList(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID_RET_INT(TYPEID_UNIT);
		ptr->GetAIInterface()->WipeHateList();
		return 0;
	}

	int WipeTargetList(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID_RET_INT(TYPEID_UNIT);
		ptr->GetAIInterface()->WipeTargetList();
		return 0;
	}

	int WipeCurrentTarget(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID_RET_INT(TYPEID_UNIT);
		ptr->GetAIInterface()->WipeCurrentTarget();
		return 0;
	}

	int GetPlayerClass(lua_State * L, Unit * ptr)
	{
	   if(ptr == NULL || ptr->GetTypeId()!=TYPEID_PLAYER) 
	   {
		  lua_pushstring(L, "Unknown");
		  return 1;
	   }
	   int plrclass = TO_PLAYER( ptr )->getClass();
	   switch(plrclass) 
	   {
		  case 1:
			 lua_pushstring(L, "Warrior");
			 break;
		  case 2:
			 lua_pushstring(L, "Paladin");
			 break;
		  case 3:
			 lua_pushstring(L, "Hunter");
			 break;
		  case 4:
			 lua_pushstring(L, "Rogue");
			 break;
		  case 5:
			 lua_pushstring(L, "Priest");
			 break;
		  //case 6:
		  case 7:
			 lua_pushstring(L, "Shaman");
			 break;
		  case 8:
			 lua_pushstring(L, "Mage");
			 break;
		  case 9:
			 lua_pushstring(L, "Warlock");
			 break;
		  //case 10:
		  case 11:
			 lua_pushstring(L, "Druid");
			 break;
		  default:
			 lua_pushstring(L, "Unknown");
			 break;
	   }
	   return 1;
	}
	int ClearHateList(lua_State * L, Unit * ptr)
	{
 		CHECK_TYPEID_RET_INT(TYPEID_UNIT);
 		ptr->GetAIInterface()->ClearHateList();
 		return 1;
	}
	int PvPFlagged(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		lua_pushboolean(L,( TO_PLAYER( ptr )->IsPvPFlagged())?1:0);
		return 1;
	}
	int GetRunSpeed(lua_State *  L, Unit * ptr)
	{
		CHECK_PTR();
		if(ptr->GetTypeId() == TYPEID_PLAYER || ptr->GetTypeId() == TYPEID_UNIT)
			lua_pushnumber(L,(float)ptr->m_runSpeed);
		return 1;
	}
	int GetFlySpeed(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		if(ptr->GetTypeId() == TYPEID_UNIT || ptr->GetTypeId() == TYPEID_PLAYER)
			lua_pushnumber(L,(float)ptr->m_flySpeed);
		return 1;
	}
	int GetModel(lua_State * L, Unit * ptr)
	{
		if( ptr == NULL )
			lua_pushnil(L);
		else
			lua_pushinteger( L, ptr->GetUInt32Value( UNIT_FIELD_DISPLAYID ) );

		return 1;
	}
	int Disconnect(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		TO_PLAYER( ptr )->GetSession()->Disconnect();
		return 0;
	}
	int GetZone(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		lua_pushinteger(L,ptr->GetZoneId());
		return 1;
	}
	int GetMapId(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		lua_pushinteger(L,ptr->GetMapId());
		return 1;
	}
	int SendWideBroadcastMessage(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		const char * msg = luaL_checkstring(L,1);
		if(!msg) return 0;
		sWorld.SendWorldWideScreenText(msg, 0);
		return 0;
	}
	int IsBehind(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		Unit * target = Lunar<Unit>::check(L,1);
		if(target == NULL) return 0;
		lua_pushboolean(L,(ptr->isInBack( TO_UNIT( target)))?1:0);
		return 1;
	}
	int RemoveFlag(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		int newflag = luaL_checkint( L, 1 );
		int oldflag = luaL_checkint( L, 2 );
		ptr->RemoveFlag(newflag, oldflag);
		return 0;
	}
	int IsFlying(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL || ptr->GetTypeId() !=  TYPEID_PLAYER && ptr->GetTypeId() != TYPEID_UNIT) return 0;
		if(ptr->GetTypeId() == TYPEID_PLAYER)
			lua_pushboolean(L,( TO_PLAYER( ptr)->m_setflycheat)?1:0);
		//else if(ptr->GetTypeId() == TYPEID_UNIT)
			//lua_pushboolean(L,( TO_CREATURE( ptr)->GetAIInterface()->IsFlying())?1:0);
			return 1;
	}
	int SetFactionStanding(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		int faction = luaL_checkint( L, 1 );
		int val = luaL_checkint( L, 2 );
		if( val != 0 )
			TO_PLAYER( ptr)->SetStanding(faction,val);
		return 0;
	}
	int GetLevel(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL || ptr->GetTypeId() != TYPEID_UNIT && ptr->GetTypeId() != TYPEID_PLAYER) return 0;
		lua_pushinteger(L,ptr->getLevel());
		return 1;
	}
	int IsAlive(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL || ptr->GetTypeId() != TYPEID_UNIT && ptr->GetTypeId() != TYPEID_PLAYER) return 0;
		lua_pushboolean(L,(ptr->isAlive())?1:0);
		return 1;
	}
	int IsStealthed(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		//lua_pushboolean(L,(TO_PLAYER(ptr)->IsStealth())?1:0);
		return 1;
	}
	int IsGM(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		lua_pushboolean(L,(TO_PLAYER(ptr)->GetSession()->HasGMPermissions())?1:0);
		return 1;
	}
	int IsAlliance(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		lua_pushboolean(L,(TO_PLAYER(ptr)->GetTeam() == 1)?1:0);
		return 1;
	}
	int IsHorde(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		lua_pushboolean(L,(TO_PLAYER(ptr)->GetTeam() == 2)?1:0);
		return 1;
	}
	int HasAmountOfPlayers(lua_State * L, Unit * ptr)
	{
		int Amount = luaL_checkint( L, 1 );
		if (Amount == 0) return 0;
		lua_pushboolean(L, (sWorld.AlliancePlayers+sWorld.HordePlayers == Amount)?1:0);
		return 1;
	}
	int HasGMTagOn(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		lua_pushboolean(L,(TO_PLAYER(ptr)->bGMTagOn)?1:0);
		return 1;
	}
	int CastSpellAoF(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 sp = luaL_checkint(L, 4);
		float x = (float)luaL_checknumber(L,1);
		float y = (float)luaL_checknumber(L,2);
		float z = (float)luaL_checknumber(L,3);
		if(!sp | !x | !y | !z)
			return 0;
		ptr->CastSpellAoF(x,y,z,dbcSpell.LookupEntry(sp), true);
		return 0;
	}
	int RemoveAllAuras(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		if(ptr->GetTypeId() != TYPEID_UNIT || ptr->GetTypeId() != TYPEID_PLAYER )
			return 0;
		ptr->RemoveAllAuras();
		return 0;
	}
	int TogglePvP(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		TO_PLAYER(ptr)->PvPToggle();
		return 0;
	}
	int ApplyRestState(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		Player *  plr = TO_PLAYER(ptr);
		uint32 rs = luaL_checkint(L, 1);
		plr->ApplyPlayerRestState( (rs >0)?true:false);
		return 0;
	}
	int ResurrectPlayer(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		Player *  Plr = TO_PLAYER(ptr);
		Plr->ResurrectPlayer(NULLPLR);
		return 0;
	}
	int SetEmoteState(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		uint32 emote = luaL_checkint(L, 1);
		ptr->setEmoteState(emote);
		return 0;
	}
	int GetPlayerAccountName(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		lua_pushstring(L,TO_PLAYER(ptr)->GetSession()->GetAccountNameS());
		return 1;
	}
	int RemoveStealth(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		TO_PLAYER(ptr)->RemoveStealth();
		return 0;
	}
	int Die(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		ptr->DealDamage(TO_UNIT(ptr),ptr->GetUInt32Value(UNIT_FIELD_HEALTH),0,0,0);
		return 0;
	}
	int SetStealth(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		uint32 id = luaL_checkint(L, 1);
		if(id == 0) return 0;
		TO_PLAYER(ptr)->SetStealth(id);
		return 0;
	}
	int WorldQuery(lua_State * L, Unit * ptr)
	{
		const char * query = luaL_checkstring(L, 1);
		if(query == 0) return 0;
		WorldDatabase.Query(query);
		return 0;
	}
	int AddCreatureLoot(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		int entry = luaL_checkint(L, 1);
		int item = luaL_checkint(L, 2);
		int perc = luaL_checkint(L, 3);
		int hper = luaL_checkint(L, 4);
		int min = luaL_checkint(L, 5);
		int max = luaL_checkint(L, 6);
		int ffa = luaL_checkint(L, 7);
		WorldDatabase.Query("INSERT INTO `creatureloot` VALUES ('','%i','%i','%i','%i','%i','%i','%i')", entry, item, perc, hper, min, max, ffa);
		return 0;
	}

	int SystemMessageToPlr(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		Unit * pTemp = Lunar<Unit>::check(L, 1);
		const char * msg = luaL_checkstring(L, 2);
		int color = luaL_checkint(L, 3);
		shared_ptr<Player> target = TO_PLAYER(pTemp);
		if(color == 0)
			sChatHandler.SystemMessageToPlr(target, msg);
		else if(color == 1)
			sChatHandler.RedSystemMessageToPlr(target, msg);
		else if(color == 2)
			sChatHandler.BlueSystemMessageToPlr(target, msg);
		else if(color == 3)
			sChatHandler.GreenSystemMessageToPlr(target, msg);
		else
			sLog.outColor(TRED, "Error You specified a non existent interger for the function");
		return 0;
	}

	int CharacterQuery(lua_State * L, Unit * ptr)
	{
		const char * query = luaL_checkstring(L, 1);
		if(query == 0) return 0;
		CharacterDatabase.Query(query);
		return 0;
	}

	int IsInvincible(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		if(ptr->GetTypeId() != TYPEID_PLAYER && ptr->GetTypeId() != TYPEID_UNIT ) return 0;
		lua_pushboolean(L,(TO_PLAYER(ptr)->bInvincible)?1:0);
		return 1;
	}
	int SaveToDB(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		if(ptr->GetTypeId() == TYPEID_UNIT)
			TO_CREATURE(ptr)->SaveToDB();
		else if(ptr->GetTypeId() == TYPEID_PLAYER)
			TO_PLAYER(ptr)->SaveToDB(false);
		return 0;
	}

	int IsMoving(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		if(ptr->GetTypeId() == TYPEID_UNIT)
			lua_pushboolean(L,(TO_CREATURE(ptr)->GetAIInterface()->m_creatureState == MOVING)?1:0);
		else if(ptr->GetTypeId() == TYPEID_PLAYER)
			lua_pushboolean(L,( TO_PLAYER(ptr)->m_isMoving)?1:0);
		return 1;
	}
	int IsAttacking(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		if(ptr->GetTypeId() == TYPEID_UNIT)
			lua_pushboolean(L,( TO_PLAYER(ptr)->GetAIInterface()->m_creatureState == ATTACKING)?1:0);
		else if( ptr->GetTypeId() == TYPEID_PLAYER)
			lua_pushboolean(L,( TO_PLAYER(ptr)->IsAttacking())?1:0);
		return 1;
	}
			
	int SetInvincible(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 choice = luaL_checkint(L,1);
		ptr->bInvincible = ( (choice > 0)? true : false );
		return 0;
	}
	bool IsInrangeSQ( Object * t1, Object * t2,float sq_r)
	{
		if( !t1 || !t2 )
			return false;

		float distx=t1->GetPositionX()-t2->GetPositionX();
		float disty=t1->GetPositionY()-t2->GetPositionY();
		float dist = distx*distx + disty*disty; 
		return dist <= sq_r;
	}
	int IsInRange(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		Player *  plr = TO_PLAYER(ptr);
		float range = (float)luaL_checknumber(L,1);
		( (IsInrangeSQ(plr, ptr, range) == true)?lua_pushboolean(L,1) : lua_pushboolean(L,0) );
		return 1;
	}
	int FinishQuest(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		QuestLogEntry * qle;
		uint32 questid = luaL_checkint(L, 1);
		if(questid == 0) return 0;
		qle = TO_PLAYER(ptr)->GetQuestLogForEntry(questid);
		( (qle != NULL) ? qle->Finish() : lua_pushnil(L) );
		return 0;
	}
		
	int IfQuestCanBeFinished(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		QuestLogEntry * qle;
		uint32 questid = luaL_checkint(L, 1);
		if(questid == 0) return 0;
		qle = TO_PLAYER(ptr)->GetQuestLogForEntry(questid);
		if(qle != NULL)
		lua_pushboolean(L, (qle->CanBeFinished())?1:0);
		return 1;
	}
	int SetQuestComplete(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		QuestLogEntry * qle;
		uint32 questid = luaL_checkint(L, 1);
		if(questid == 0) return 0;
		qle = TO_PLAYER(ptr)->GetQuestLogForEntry(questid);
		if(qle == 0) return 0;
		qle->SendQuestComplete();
		return 0;
	}

	int SubtractTime(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		QuestLogEntry * qle;
		uint32 time = luaL_checkint(L, 2);
		uint32 questid = luaL_checkint(L, 1);
		if(questid == 0) return 0;
		qle = TO_PLAYER(ptr)->GetQuestLogForEntry(questid);
		if(qle == 0 ) return 0;
		qle->SubtractTime(time);
		return 0;
	}

	int MoveToPlayer(lua_State * L, Unit * ptr)
	{
		Unit * plr = Lunar<Unit>::check(L,1);
		if( plr->IsPlayer())
			ptr->GetAIInterface()->MoveTo(plr->GetPositionX(),plr->GetPositionY(),plr->GetPositionZ(),ptr->GetOrientation());
		return 0;
	}

	int EventCreatureYell(lua_State * L, Unit * ptr)
	{
		Creature * cre = TO_CREATURE(ptr);
		uint32 time = luaL_checkint(L, 2);
		string msg = luaL_checkstring(L, 1);
		//sEAS.EventCreatureYell(cre, msg, time);
		return 0;
	}

	int EventCreatureDelete(lua_State * L, Unit * ptr)
	{
		Creature * cre = TO_CREATURE(ptr);
		uint32 time = luaL_checkint(L, 1);
		//sEAS.EventCreatureDelete(cre, time);
		return 0;
	}

	int EventCastSpell(lua_State * L, Unit * ptr)
	{
		Creature * cre = TO_CREATURE(ptr);
		uint32 time = luaL_checkint(L, 2);
		uint32 spellid = luaL_checkint(L, 1);
		Unit * target = Lunar<Unit>::check(L,3);
		//sEAS.EventCastSpell(ptr, target, spellid, time);
		return 0;
	}

	int EventCastSpellOnSelf(lua_State * L, Unit * ptr)
	{
		Creature * cre = TO_CREATURE(ptr);
		uint32 time = luaL_checkint(L, 2);
		uint32 spellid = luaL_checkint(L, 1);
		//sEAS.EventCastSpell(ptr, ptr, spellid, time);
		return 0;
	}

	int EventPlaySound(lua_State * L, Unit * ptr)
	{
		Creature * cre = TO_CREATURE(ptr);
		uint32 time = luaL_checkint(L, 2);
		uint32 soundid = luaL_checkint(L, 1);
		//sEAS.EventPlaySound(cre, soundid, time);
		return 0;
	}
	int MonsterMove(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL || (ptr->GetTypeId() != TYPEID_UNIT && ptr->GetTypeId() != TYPEID_PLAYER)) return 0;
		float x = (float)luaL_checkint(L,1);
		float y = (float)luaL_checkint(L,2);
		float z = (float)luaL_checkint(L,3);
		uint32 time = luaL_checkint(L,4);
		uint32 moveflags = luaL_checkint(L,5);
		if(!x || !y || !z || !time) return 0;

		WorldPacket data(SMSG_MONSTER_MOVE,(sizeof(float)*6)+(sizeof(uint32)*4)+sizeof(WoWGuid));
		data << ptr->GetNewGUID();
		data << ptr->GetPosition();
		data << getMSTime();
		if(ptr->GetOrientation())
		{
			data << uint8(4);
			data << ptr->GetOrientation();
		}
		else
			data << uint8(0);
		data << moveflags;
		data << time;
		data << uint32(1);
		data << x << y << z;
		if(ptr->GetTypeId() == TYPEID_UNIT)
			ptr->SendMessageToSet(&data, false);
		else if(ptr->GetTypeId() == TYPEID_PLAYER)
			TO_PLAYER(ptr)->GetSession()->SendPacket(&data);
		return 0;
	}
	int SetPosition(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		float x = (float)luaL_checkint(L,1);
		float y = (float)luaL_checkint(L,2);
		float z = (float)luaL_checkint(L,3);
		if(!x ||!y||!z) return 0;
		LocationVector vec(x,y,z);
		ptr->SetPosition(vec,true);
		return 0;
	}
	int SetCurrentAgent(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		uint32 agent = luaL_checkint(L,1);
		if(agent == NULL) return 0;
		ptr->GetAIInterface()->setCurrentAgent(AI_Agent(agent));
		return 0;
	}
	/*int AddAISpell(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		uint32 spellid = luaL_checkint(L,1);
		uint32 agent	= luaL_checkint(L,2);
		uint32 cd	= luaL_checkint(L,3);
		uint32 spelltype = luaL_checkint(L,4);
		uint32 targetype = luaL_checkint(L,5);
		uint32 procchance = luaL_checkint(L,6);
		uint32 autocast = luaL_checkint(L,7);
		if(!spellid) return 0;
		SpellEntry * spp = dbcSpell.LookupEntry(spellid);
		AI_Spell * sp = new AI_Spell();
		sp->agent = agent;
		sp->autocast_type = autocast;
		sp->cooldown = cd;
		sp->cooldowntime = 0;
		sp->custom_pointer = true;
		sp->entryId = ptr->GetEntry();
		sp->floatMisc1 = 0;
		sp->maxrange = GetMaxRange(dbcSpellRange.LookupEntry(spp->rangeIndex));
		sp->minrange = GetMinRange(dbcSpellRange.LookupEntry(spp->rangeIndex));
		sp->Misc2 = 0;
		sp->procChance = procchance;
		sp->procCount = 0;
		sp->procCounter = 0;
		sp->spell = spp;
		sp->spelltargetType = targetype;
		sp->spellType = spelltype;
		if( agent == AGENT_RANGED)
			ptr->GetAIInterface()->m_canRangedAttack = true;
		else if( agent == AGENT_SPELL)
			ptr->GetAIInterface()->addSpellToList(sp);
		return 0;
	}*/
	int RegisterAIUpdateEvent(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		uint32 time = luaL_checkint(L,1);
		if(!time) return 0;
		sEventMgr.AddEvent( TO_CREATURE(ptr),&Creature::CallScriptUpdate,EVENT_CREATURE_UPDATE,time,0,0);
		return 0;
	}
	int ModifyAIUpdateEvent(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		uint32 newtime = luaL_checkint(L,1);
		if(!newtime) return 0;
		sEventMgr.ModifyEventTime( TO_CREATURE(ptr),EVENT_CREATURE_UPDATE,newtime);
		return 0;
	}
	int RemoveAIUpdateEvent(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		sEventMgr.RemoveEvents( TO_CREATURE(ptr),EVENT_CREATURE_UPDATE);
		return 0;
	}
	int SetDamageDone(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		uint32 school = luaL_checkint(L,1);
		uint32 coeff = luaL_checkint(L,2);

		TO_CREATURE(ptr)->ModDamageDone[school] = coeff;
		return 0;
	}
	int GetDamageDone(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		uint32 school = luaL_checkint(L,1);
		lua_pushnumber(L,TO_CREATURE(ptr)->ModDamageDone[school]);
		return 1;
	}
	int SetAIState(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		uint32 typ = luaL_checkint(L,1);
		TO_CREATURE(ptr)->GetAIInterface()->SetAIState(AI_State(typ));
		return 0;
	}
	int DisableRespawn(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		TO_CREATURE(ptr)->m_noRespawn = true;
		return 0;
	}
	int ModifySpeed(lua_State  * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		uint32 speed = luaL_checkint(L,1);
		if(speed == 0) return 0;
		ptr->m_speedModifier = (float)speed;
		ptr->UpdateSpeed();
		return 0;
	}
	int SetInvisible(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		uint32 choice = luaL_checkint(L,1);
		if( choice > 0 )
		{
			ptr->m_invisFlag = (uint8)choice;
			ptr->m_invisible = true;
		}
		else
		{
			ptr->m_invisFlag = INVIS_FLAG_NORMAL;
			ptr->m_invisible = false;
		}
		ptr->UpdateVisibility();
		return 0;
	}
	int GetInRangePlayers(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		uint32 count = NULL;
		lua_newtable(L);
		for(PlayerSet::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); itr++)
		{
			count++;
			lua_pushinteger(L,count);
			Lunar<Unit>::push(L, TO_UNIT(*itr));
			lua_rawset(L,-3);
		}
		return 1;
	}
	int GetInRangeFriends(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		uint32 count = NULL;
		lua_newtable(L);
		for(ObjectSet::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); itr++)
		{
			if( (*itr)->IsUnit() && isFriendly(TO_OBJECT(ptr),(*itr)) )
			{
				count++;
				lua_pushinteger(L,count);
				Lunar<Unit>::push(L, TO_UNIT(*itr));
				lua_rawset(L,-3);
			}
		}
		return 1;
	}
	int GetInRangeUnits(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 count = 0;
		lua_newtable(L);
		for(ObjectSet::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); itr++)
		{
			if( (*itr)->GetTypeId() == TYPEID_UNIT  )
			{
				count++;
				lua_pushinteger(L,count);
				Lunar<Unit>::push(L,TO_UNIT(*itr));
				lua_rawset(L,-3);
			}
		}
		return 1;
	}
	int RemoveFromWorld(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		if( ptr->GetTypeId() != TYPEID_UNIT && ptr->GetTypeId() != TYPEID_PLAYER ) return 0;
		ptr->RemoveFromWorld(true);
		return 0;
	}
	int GetCreatureNearestCoords(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		float x = (float)luaL_checkinteger(L,1);
		float y = (float)luaL_checkinteger(L,2);
		float z = (float)luaL_checkinteger(L,3);
		uint32 entryid = luaL_checkint(L,4);
		if(x == 0 || y == 0 || z == 0 || entryid == 0 ) return 0;
		shared_ptr<Creature> pC = ptr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(x,y,z,entryid);
		if( pC == NULL) return 0;
		Lunar<Unit>::push(L,TO_UNIT(pC));
		return 1;
	}
	int GetGameObjectNearestCoords(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		float x = (float)luaL_checkinteger(L,1);
		float y = (float)luaL_checkinteger(L,2);
		float z = (float)luaL_checkinteger(L,3);
		uint32 entryid = luaL_checkint(L,4);
		if(x == 0 || y == 0 || z == 0 || entryid == 0 ) return 0;
		shared_ptr<GameObject> Obj = ptr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(x,y,z,entryid);
		if(Obj == NULL) return 0;
		Lunar<GameObject>::push(L,Obj);
		return 1;
	}
	int GetUInt32Value(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL || (ptr->GetTypeId() != TYPEID_UNIT && ptr->GetTypeId() != TYPEID_PLAYER)) return 0;
		uint32 field = luaL_checkint(L,1);
		lua_pushinteger(L,ptr->GetUInt32Value(field));
		return 1;
	}
	int GetUInt64Value(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL || (ptr->GetTypeId() != TYPEID_UNIT && ptr->GetTypeId() != TYPEID_PLAYER)) return 0;
		uint32 field = luaL_checkint(L,1);
		lua_pushnumber(L,ptr->GetUInt64Value(field));
		return 1;
	}
	int GetDistance(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL ) return 0;
		Unit * tar = Lunar<Unit>::check(L,1);
		if( tar == NULL) return 0;
		lua_pushnumber(L,ptr->CalcDistance(tar->GetPositionX(),tar->GetPositionY(),tar->GetPositionZ()));
		return 1;
	}
	int CalcDistance(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		float x = (float)luaL_checkint(L,1);
		float y = (float)luaL_checkint(L,2);
		float z = (float)luaL_checkint(L,3);
		if(x == 0 || y == 0 || z == 0 ) return 0;
		lua_pushnumber(L,ptr->CalcDistance(x,y,z));
		return 1;
	}
	int GetCurrentSpellId(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		if(ptr->GetCurrentSpell() != 0)
			lua_pushinteger(L,ptr->GetCurrentSpell()->m_spellInfo->Id);
		return 1;
	}
	int GetCurrentSpellName(lua_State * L, Unit * ptr)
	{
		if(ptr == NULL) return 0;
		if(ptr->GetCurrentSpell() != 0 )
			lua_pushstring(L,ptr->GetCurrentSpell()->m_spellInfo->Name);
		return 1;
	}
	int ModThreat(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		Unit * target = Lunar<Unit>::check(L,1);
		uint32 threattomod = luaL_checkint(L,2);
		if(target == NULL) return 0;
		ptr->GetAIInterface()->modThreatByPtr(TO_UNIT( target),threattomod);
		return 0;
	}
	int RemoveThreat(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		Unit * target = Lunar<Unit>::check(L,1);
		if(target == NULL) return 0;
		ptr->GetAIInterface()->RemoveThreatByPtr(TO_UNIT( target));
		return 0;
	}
	int GetThreat(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		Unit * target = Lunar<Unit>::check(L,1);
		if(target == NULL) return 0;
		lua_pushinteger(L,(uint32)ptr->GetAIInterface()->getThreatByPtr(TO_UNIT( target)));
		return 1;
	}
	int GetPowerType(lua_State * L, Unit * ptr)
	{
		if( ptr == NULL ||(ptr->GetTypeId() != TYPEID_PLAYER && ptr->GetTypeId() != TYPEID_UNIT)) return 0;
		switch(ptr->GetPowerType())
		{
		case POWER_TYPE_HEALTH:
			lua_pushstring(L,"Health");
			break;
		case POWER_TYPE_MANA:
			lua_pushstring(L,"Mana");
			break;
		case POWER_TYPE_FOCUS:
			lua_pushstring(L,"Focus");
			break;
		case POWER_TYPE_ENERGY:
			lua_pushstring(L,"Energy");
			break;
		case POWER_TYPE_RAGE:
			lua_pushstring(L,"Rage");
			break;
		default:
			lua_pushstring(L,"Unknown");
			break;
		}
		return 1;
	}
	int SetNextTarget(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		Unit * tar = Lunar<Unit>::check(L,1);
		if( tar == NULL) return 0;
		ptr->GetAIInterface()->SetNextTarget(TO_UNIT( tar ));
		return 0;
	}
	int GetNextTarget(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		Lunar<Unit>::push(L,(ptr->GetAIInterface()->GetNextTarget()) );
		return 1;
	}
	int GetSpawnX(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		if(ptr->GetSpawnX() != 0)
			lua_pushnumber(L,(float)ptr->GetSpawnX());
		return 1;
	}
	int GetSpawnY(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		if(ptr->GetSpawnY() != 0)
			lua_pushnumber(L,(float)ptr->GetSpawnY());
		return 1;
	}
	int GetSpawnZ(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		if(ptr->GetSpawnZ() != 0)
			lua_pushnumber(L,(float)ptr->GetSpawnZ());
		return 1;
	}
	int GetSpawnO(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		if(ptr->GetSpawnO() != 0)
			lua_pushnumber(L,(float)ptr->GetSpawnO());
		return 1;
	}
	int GetInstanceMode(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		Player * plr = TO_PLAYER(ptr);
		if(plr->GetMapMgr()->GetMapInfo()->type != INSTANCE_NULL)
		{
			switch(plr->iInstanceType)
			{
			case MODE_EPIC:
				lua_pushstring(L,"Epic");
			case MODE_NORMAL:
				lua_pushstring(L,"Normal");
			case MODE_HEROIC:
				lua_pushstring(L,"Heroic");
			default:
				lua_pushstring(L,"Unknown");
			}
		}
		return 1;
	}
	/*int KnockBack(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		float distance = (float)luaL_checkint(L,1);
		float height = (float)luaL_checkint(L,2);
		Player *  plr = TO_PLAYER(ptr);
		WorldPacket data(SMSG_MOVE_KNOCK_BACK,50);
		data << plr->GetNewGUID();
		data << getMSTime();
		data << cosf(plr->GetOrientation()) << sinf(ptr->GetOrientation());
		data << distance;
		data << (-height);
		plr->blinked = true;
		plr->DelaySpeedHack(5000);
		plr->GetSession()->SendPacket(&data);
		return 0;
	}*/
	int Root(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		ptr->m_canMove = false;
		return 0;
	}
	int Unroot(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		ptr->m_canMove = true;
		return 0;
	}
	int GetSelection(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		Player * plr =  TO_PLAYER(ptr);
		if( plr->GetSelection() != 0)
			Lunar<Unit>::push(L,(plr->GetMapMgr()->GetUnit(plr->GetSelection())) );
		return 1;
	}
	int SetSelection(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		Unit * tar = Lunar<Unit>::check(L,1);
		if(tar == NULL) return 0;
		TO_PLAYER(ptr)->SetSelection(tar->GetGUID());
		return 0;
	}
	int SendMeetingStoneQueue(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		uint32 dungeon_id = luaL_checkint(L,1);
		uint32 status = luaL_checkint(L,2);
		TO_PLAYER(ptr)->SendMeetingStoneQueue(dungeon_id,status);
		return 0;
	}
	int IsInFeralForm(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		Player *  plr = TO_PLAYER(ptr);
		lua_pushboolean(L,(plr->IsInFeralForm() != 0)?1:0);
		return 1;
	}
	int GetStanding(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		uint32 faction = luaL_checkint(L,1);
		Player *  plr = TO_PLAYER(ptr);
		lua_pushinteger(L,( plr->GetStanding(faction) != 0)?plr->GetStanding(faction) : 0);
		return 1;
	}
	int SetStanding(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		uint32 faction = luaL_checkint(L,1);
		int32 value = luaL_checkint(L,2);
		TO_PLAYER(ptr)->SetStanding(faction,value);
		return 0;
	}
	int ModStanding(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		uint32 faction = luaL_checkint(L,1);
		int32 value = luaL_checkint(L,2);
		TO_PLAYER(ptr)->ModStanding(faction,value);
		return 0;
	}
	int SetAtWar(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		uint32 faction = luaL_checkint(L,1);
		uint32 set = luaL_checkint(L,2);
		TO_PLAYER(ptr)->SetAtWar(faction,(set > 0)?true:false);
		return 0;
	}
	/*int IsAtWar(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		uint32 faction = luaL_checkint(L,1);
		Player *  plr = static_cast<Player*>(ptr);
		if( plr->IsAtWar(faction) == true)
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}*/
	int GetStandingRank(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		uint32 faction = luaL_checkint(L,1);
		switch( TO_PLAYER(ptr)->GetStandingRank(faction))
		{
		case STANDING_HATED:
			lua_pushstring(L,"Hated");
		case STANDING_HOSTILE:
			lua_pushstring(L,"Hostile");
		case STANDING_UNFRIENDLY:
			lua_pushstring(L,"Unfriendly");
		case STANDING_NEUTRAL:
			lua_pushstring(L,"Neutral");
		case STANDING_FRIENDLY:
			lua_pushstring(L,"Friendly");
		case STANDING_HONORED:
			lua_pushstring(L,"Honored");
		case STANDING_REVERED:
			lua_pushstring(L,"Revered");
		case STANDING_EXALTED:
			lua_pushstring(L,"Exalted");
		default:
			lua_pushstring(L,"Unknown");
		}
		return 1;
	}
	int UpdateInRangeSet(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		TO_PLAYER(ptr)->UpdateInrangeSetsBasedOnReputation();
		return 0;
	}
	int GetRepRankFromStanding(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		uint32 value = luaL_checkint(L,1);
		switch( TO_PLAYER(ptr)->GetReputationRankFromStanding(value))
		{
		case STANDING_HATED:
			lua_pushstring(L,"Hated");
		case STANDING_HOSTILE:
			lua_pushstring(L,"Hostile");
		case STANDING_UNFRIENDLY:
			lua_pushstring(L,"Unfriendly");
		case STANDING_NEUTRAL:
			lua_pushstring(L,"Neutral");
		case STANDING_FRIENDLY:
			lua_pushstring(L,"Friendly");
		case STANDING_HONORED:
			lua_pushstring(L,"Honored");
		case STANDING_REVERED:
			lua_pushstring(L,"Revered");
		case STANDING_EXALTED:
			lua_pushstring(L,"Exalted");
		default:
			lua_pushstring(L,"Unknown");
		}
		return 1;
	}
	int GetPVPRank(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		lua_pushinteger(L, TO_PLAYER(ptr)->GetPVPRank());
		return 1;
	}
	int SetPVPRank(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		int32 newrank = luaL_checkint(L,1);
		TO_PLAYER(ptr)->SetPVPRank(newrank);
		return 0;
	}
	int IsGroupLeader(lua_State* L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		lua_pushboolean(L,( TO_PLAYER(ptr)->IsGroupLeader() == true)?1:0);
		return 1;
	}
	int IsGroupMember(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		lua_pushboolean(L,( TO_PLAYER(ptr )->IsGroupMember( TO_PLAYER(ptr )) == true)?1:0);
		return 1;
	}
	int SetBanned(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		const char * msg = luaL_checklstring(L,1,NULL);
		if(msg)
			TO_PLAYER(ptr)->SetBanned(string(msg));
		else
			TO_PLAYER(ptr)->SetBanned();
		return 0;
	}
	int UnSetBanned(lua_State  * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		TO_PLAYER(ptr)->UnSetBanned();
		return 0;
	}
	int SetBindPoint(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		float x = (float)luaL_checkint(L,1);
		float y = (float)luaL_checkint(L,2);
		float z = (float)luaL_checkint(L,3);
		uint32 mapid = luaL_checkint(L,4);
		uint32 zoneid = luaL_checkint(L,5);
		TO_PLAYER(ptr)->SetBindPoint(x,y,z,mapid,zoneid);
		return 0;
	}
	int ResetTalents(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		TO_PLAYER(ptr)->Reset_Talents();
		return 0;
	}
	int GetBindPosition(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		lua_newtable(L);
		Player *  plr = TO_PLAYER(ptr);
		lua_pushstring(L,"x");
		lua_pushnumber(L,(float)plr->GetBindPositionX());
		lua_rawset(L,-3);
		lua_pushstring(L,"y");
		lua_pushnumber(L,(float)plr->GetBindPositionY());
		lua_rawset(L,-3);
		lua_pushstring(L,"z");
		lua_pushnumber(L,(float)plr->GetBindPositionZ());
		lua_rawset(L,-3);
		lua_pushstring(L,"mapid");
		lua_pushnumber(L,(float)plr->GetBindMapId());
		lua_rawset(L,-3);
		lua_pushstring(L,"zoneid");
		lua_pushnumber(L,(float)plr->GetBindZoneId());
		lua_rawset(L,-3);
		
		return 1;
	}
	int AllowFallDamage(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		uint32 choice = luaL_checkint(L,1);
		TO_PLAYER(ptr)->m_noFallDamage = (choice > 0)?true:false;
		return 0;
	}
	int GetSoulStone(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		lua_pushinteger(L,TO_PLAYER(ptr)->GetSoulStone());
		return 1;
	}
	int SetSoulStone(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		uint32 id = luaL_checkint(L,1);
		if(id != 0)
			TO_PLAYER(ptr)->SetSoulStone(id);
		return 0;
	}
	int SetReincarnate(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		uint32 choice = luaL_checkint(L,1);
		TO_PLAYER(ptr)->bReincarnation = (choice >0)?true:false;
		return 0;
	}
	int EjectFromInstance(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		TO_PLAYER(ptr)->EjectFromInstance();
		return 0;
	}
	int SetPlayerName(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		const char * name = luaL_checkstring(L,1);
		if(name == 0 ) return 0;
		TO_PLAYER(ptr)->SetName(string(name));
		return 0;
	}
	int OnUnitEvent(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		const char * func_to_call = luaL_checkstring(L,1);
		uint32 eventnum = luaL_checkint(L,2);
		Unit * pMisc = Lunar<Unit>::check(L,3);
		uint32 Misc  = luaL_checkint(L,4);
		g_engine.OnUnitEvent(ptr,func_to_call,eventnum,pMisc,Misc);
		return 0;
	}
	/* Date : 01/05/2009 */
	int GetEntry(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		( (ptr != NULL) ? lua_pushinteger(L, ptr->GetEntry()) : lua_pushnil(L));
		return 1;
	}
	int Energize(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		Unit * target = Lunar<Unit>::check(L,1);
		uint32 spellid = luaL_checkint(L,2);
		uint32 amt = luaL_checkint(L,3);
		uint32 type = luaL_checkint(L,4);
		if(target == NULL || spellid == 0) return 0;
		ptr->Energize( TO_UNIT(target), spellid, amt, type);
		return 0;
	}
	int Heal(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		Unit * target = Lunar<Unit>::check(L,1);
		uint32 spellid = luaL_checkint(L,2);
		uint32 amt = luaL_checkint(L,3);
		if(target == NULL || !spellid) return 0;
		ptr->Heal( TO_UNIT(target), spellid, amt);
		return 0;
	}
	int GetAP(lua_State * L, Unit * ptr)
	{
		bool ranged = ( (luaL_checkint(L,1) != 0) ? true : false );
		if(ptr == NULL) lua_pushnil(L);
		( (ranged == true) ? lua_pushinteger(L, ptr->GetRAP()) : lua_pushinteger(L,ptr->GetAP()) );
		return 1;
	}
	int GetPlayerRace(lua_State * L, Unit * ptr) {
		char * s;
		switch( ptr->getRace()) {
			case RACE_HUMAN: s = "Human"; break;
			case RACE_BLOODELF: s = "BloodElf"; break;
			case RACE_TROLL: s = "Troll"; break;
			case RACE_NIGHTELF: s = "NightElf"; break;
			case RACE_UNDEAD: s = "Undead"; break;
			case RACE_ORC: s = "Orc"; break;
			case RACE_DWARF: s = "Dwarf"; break;
			case RACE_GNOME: s = "Gnome"; break;
			case RACE_DRAENEI: s = "Draenei"; break;
			case RACE_TAUREN: s = "Tauren"; break;
			default: s = "Unknown"; break;
		}
		lua_pushstring(L,s);
		return 1;
	}
	int GetSummon(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		shared_ptr<Pet> pPet = TO_PLAYER(ptr)->GetSummon();
		( (pPet != NULL) ? Lunar<Unit>::push(L,pPet) : lua_pushnil(L) );
		return 1;
	}
	int IsInParty(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		Player * plr = TO_PLAYER(ptr);
		if(plr->GetGroup() == NULL) return 0;
		Group * party = plr->GetGroup();
		if(party->GetGroupType() == GROUP_TYPE_PARTY)
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}
	int IsInRaid(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		Player * plr = TO_PLAYER(ptr);
		if(plr->GetGroup() == NULL) return 0;
		Group * party = plr->GetGroup();
		if(party->GetGroupType() == GROUP_TYPE_RAID)
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
		return 1;
	}
	int IsCasting(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		( (ptr->isCasting() == true) ? lua_pushboolean(L,1) : lua_pushboolean(L, 0) );
		return 1;
	}
	int SetAttackTimer(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		int32 timer = luaL_checkint(L,1);
		bool offhand = ( (luaL_checkint(L,2)) ? true : false );
		ptr->setAttackTimer(timer, offhand);
		return 0;
	}
	int SetDeathState(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 deathstate = luaL_checkint(L,1);
		if(deathstate > 3) return 0;
		ptr->setDeathState( DeathState(deathstate) );
		return 0;
	}
	int GetDeathState(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		lua_pushinteger(L, uint32(ptr->getDeathState()) );
		return 1;
	}
	int ShareHealth(lua_State * L, Unit * ptr)
	{
		Unit * target = Lunar<Unit>::check(L,1);
		if(target == NULL) return 0;
		//ptr->ShareHealthWithUnit( TO_UNIT(target()) );
		return 0;
	}
	int GetStandState(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		lua_pushinteger(L, uint32(ptr->GetStandState()));
		return 1;
	}
	/* End Date : 01/05/2009 */
	/* Date : 01/07/2009 */
	int SetUInt32Value(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 field = luaL_checkint(L,1);
		uint32 valu = luaL_checkint(L,2);
		if(!field || !valu) return 0;
		ptr->SetUInt32Value(field,valu);
		return 0;
	}
	int SetUInt64Value(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 field = luaL_checkint(L,1);
		uint32 valu = luaL_checkint(L,2);
		if(!field || !valu) return 0;
		ptr->SetUInt64Value(field, uint64(valu));
		return 0;
	}
	int GetGUID(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		( (ptr->GetGUID()) ? lua_pushinteger(L, (uint32)ptr->GetGUID()) : lua_pushnil(L) );
		return 1;
	}
	int CalcRadAngle(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 x1 = luaL_checkint(L,1);
		uint32 y1 = luaL_checkint(L,2);
		uint32 x2 = luaL_checkint(L,3);
		uint32 y2 = luaL_checkint(L,4);
		if(!x1 || !y1 || !x2 || !y2) return 0;
		lua_pushnumber(L, ptr->calcRadAngle(x1,y1,x2,y2) );
		return 1;
	}
	int EnableFlight(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		WorldPacket data(835,13);
		if(ptr->IsUnit())
		{
			data << ptr->GetGUID();
			data << uint32(2);
			ptr->SendMessageToSet(&data, false);
			return 0;
		}
		else if(ptr->IsPlayer())
		{
			data << ptr->GetGUID();
			data << uint32(2);
			TO_PLAYER(ptr)->SendMessageToSet(&data,true);
			return 0;
		}
		else return 0;
	}
	int DisableFlight(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		WorldPacket data(836,13);
		if(ptr->IsUnit())
		{
			data << ptr->GetGUID();
			data << uint32(5);
			ptr->SendMessageToSet(&data, false);
		}
		else if(ptr->IsPlayer())
		{
			data << ptr->GetGUID();
			data << uint32(5);
			TO_PLAYER(ptr)->SendMessageToSet(&data,true);
		}
		else 
			return 0;

	return 0;
	}
	int AttackReaction(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		Unit * target = Lunar<Unit>::check(L,1);
		uint32 amt = luaL_checkint(L,2);
		if(target == NULL) return 0;
		TO_CREATURE(ptr)->GetAIInterface()->AttackReaction(TO_UNIT(target),amt);
		return 0;
	}
	int IsInWorld(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		if(ptr->IsInWorld() == true)
				lua_pushboolean(L,1);
		else
				lua_pushboolean(L,0);
		return 1;
	}
	int PlaySpellVisual(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 visualid = luaL_checkint(L,2);
		Unit * target = Lunar<Unit>::check(L,1);
		if(target == NULL || !visualid) return 0;
		ptr->PlaySpellVisual(target->GetGUID(),visualid);
		return 0;
	}
	int ImmuneMechanic(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 mechanic = luaL_checkint(L,1);
		uint32 choice = luaL_checkint(L,2);
		if(!mechanic|| mechanic < 27) return 0;
		( (choice > 0) ? ptr->MechanicsDispels[mechanic]++ : ptr->MechanicsDispels[mechanic] = 0);
		return 0;
	}
	/* End Date : 01/07/2008 */
	/* Date : 01/13/2009 */
	int GetCurrentWaypoint(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		lua_pushinteger(L,ptr->GetAIInterface()->m_currentWaypoint);
		return 1;
	}
	int SetAllowedToEnterCombat(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		uint32 choice = luaL_checkint(L,1);
		( (choice > 0) ? ptr->GetAIInterface()->SetAllowedToEnterCombat(true) : ptr->GetAIInterface()->SetAllowedToEnterCombat(false));
		return 0;
	}
	int GetDistanceSq(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		Unit * tar = Lunar<Unit>::check(L,1);
		if( tar == NULL) return 0;
		lua_pushnumber(L,(float)ptr->GetDistanceSq(TO_OBJECT( tar)));
		return 1;
	}
	int SetOutOfCombatRange(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		uint32 nrange = luaL_checkint(L,1);
		ptr->GetAIInterface()->setOutOfCombatRange(nrange);
		return 0;
	}
	int SetSpawnLocation(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		double x = luaL_checknumber(L,1);
		double y = luaL_checknumber(L,2);
		double z = luaL_checknumber(L,3);
		double o = luaL_checknumber(L,4);
		LocationVector pVect(x,y,z,o);
		//ptr->SetSpawnPosition(pVect);
		return 0;
	}
	int DeleteAllWaypoints(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		ptr->GetAIInterface()->deleteWaypoints();
		return 0;
	}
	int ChangeWaypoint(lua_State * L , Unit * ptr)
	{
		CHECK_PTR();
		uint32 oldwp = luaL_checkint(L,1);
		uint32 newwp = luaL_checkint(L,2);
		ptr->GetAIInterface()->changeWayPointID(oldwp,newwp);
		return 0;
	}
	int DeleteWaypoint(lua_State * L, Unit * ptr)
	{
		CHECK_PTR();
		uint32 wp = luaL_checkint(L,1);
		ptr->GetAIInterface()->deleteWayPoint(wp);
		return 0;
	}
	int SaveInstanceToDB(lua_State * L, Unit * ptr)
	{
		/*CHECK_TYPEID(TYPEID_PLAYER);
		shared_ptr<Player> plr = TO_PLAYER(ptr);
		if(plr->GetMapMgr()->GetMapInfo()->type != INSTANCE_RAID) return 0;
		shared_ptr<MapMgr> m_mgr = sInstanceMgr.GetInstance(plr->GetMapId(),plr->GetInstanceID());
		Instance * in = (Instance*)(m_mgr.get());
		if(in == NULL) return 0;
		in->SaveToDB();*/
		return 0;
	}
	int ResetSavedInstances(lua_State * L, Unit * ptr)
	{
		CHECK_TYPEID(TYPEID_PLAYER);
		shared_ptr<Player> plr = TO_PLAYER(ptr);
		sInstanceMgr.ResetSavedInstances(plr);
		return 0;
	}
}
