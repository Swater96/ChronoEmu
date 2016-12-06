//Full credits go to LuaAppArc, this form makes it much neater to look for something.
#include "StdAfx.h"
#include "LUAEngine.h"
#include "GameObjectFunctions.h"

///////////////////////////////////////////////////////
////////////////////GAMEOBJECT - COMMANDS//////////////
///////////////////////////////////////////////////////
namespace luaGameObject
{
	int Teleport(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		Unit *  target = Lunar<Unit>::check(L, 1);
		int mapId = luaL_checkint(L, 2);
		double posX = luaL_checknumber(L, 3);
		double posY = luaL_checknumber(L, 4);
		double posZ = luaL_checknumber(L, 5);
		if(target == NULL || posX == 0 ||posY == 0 ||posZ == 0)
			return 0;
		if(target->GetTypeId() != TYPEID_PLAYER) return 0;
		LocationVector vec((float)posX, (float)posY, (float)posZ);
		static_pointer_cast<Player>(target)->SafeTeleport((uint32)mapId, 0, vec);
		return 0;
	}
	int GetName(lua_State * L, GameObject * ptr)
	{
		if(!ptr||ptr->GetTypeId()!=TYPEID_GAMEOBJECT||!ptr->GetInfo())
		{
			lua_pushstring(L,"Unknown");
			return 1;
		}

		lua_pushstring(L,ptr->GetInfo()->Name);
		return 1;
	}
	int GetCreatureNearestCoords(lua_State * L, GameObject * ptr)
	{
		if(ptr == NULL) return 0;
		float x = (float)luaL_checkinteger(L,1);
		float y = (float)luaL_checkinteger(L,2);
		float z = (float)luaL_checkinteger(L,3);
		uint32 entryid = luaL_checkint(L,4);
		if(x == 0 || y == 0 || z == 0 || entryid == 0 ) return 0;
		shared_ptr<Creature> pC = ptr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(x,y,z,entryid);
		if( pC == NULL) return 0;
		Lunar<Unit>::push(L,(Unit *)pC);
		return 1;
	}
	int GetGameObjectNearestCoords(lua_State * L, GameObject * ptr)
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
	int GossipCreateMenu(lua_State * L, GameObject * ptr)
	{
		int text_id = luaL_checkint(L, 1);
		Unit *  target = Lunar<Unit>::check(L, 2);
		int autosend = luaL_checkint(L, 3);   
		if(target->GetTypeId() != TYPEID_PLAYER) return 0;
		shared_ptr<Player> plr = TO_PLAYER(target->shared_from_this());
		objmgr.CreateGossipMenuForPlayer(&Menu, ptr->GetGUID(), text_id, plr);
		if(autosend)
			Menu->SendTo(plr);
		return 1;
	}
	int GossipMenuAddItem(lua_State * L, GameObject * ptr)
	{
		int icon = luaL_checkint(L, 1);
		const char * menu_text = luaL_checkstring(L, 2);
		int IntId = luaL_checkint(L, 3);
		int extra = luaL_checkint(L, 4);
		Menu->AddItem(icon, menu_text, IntId, (extra > 0)? true : false);
		return 1;
	}
	int GossipSendMenu(lua_State * L, GameObject * ptr)
	{
		Unit *  target = Lunar<Unit>::check(L, 1);
		if(target->GetTypeId() != TYPEID_PLAYER) return 0;
		shared_ptr<Player> plr = TO_PLAYER(target->shared_from_this());
		Menu->SendTo(plr);
		return 1;
		}
	int GossipComplete(lua_State * L, GameObject * ptr)
	{
		Unit * target = Lunar<Unit>::check(L, 1);
		if(target->GetTypeId() != TYPEID_PLAYER) return 0;

		shared_ptr<Player> plr = TO_PLAYER(target->shared_from_this());
		plr->Gossip_Complete();
		return 0;
	}
	int GossipSendPOI(lua_State * L, GameObject * ptr)
	{
		Unit * target = Lunar<Unit>::check(L, 1);
		if(target->GetTypeId() != TYPEID_PLAYER) return 0;
		shared_ptr<Player> plr = TO_PLAYER(target->shared_from_this());
		float x = (float)luaL_checknumber(L, 2);
		float y = (float)luaL_checknumber(L, 3);
		int icon = luaL_checkint(L, 4);
		int flags = luaL_checkint(L, 5);
		int data = luaL_checkint(L, 6);
		const char * name = luaL_checkstring(L, 7);

		plr->Gossip_SendPOI(x, y, icon, flags, data, name);
		return 0;
	}
	int AddItem(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		int id = luaL_checkint(L,1);
		int count = luaL_checkint(L,2);
		Unit *  nplr = Lunar<Unit>::check(L,3);
		if(nplr->GetTypeId() != TYPEID_PLAYER) return 0;

		shared_ptr<Player> plr = TO_PLAYER(nplr->shared_from_this());
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
			add->ModSignedInt32Value(ITEM_FIELD_STACK_COUNT,count);
			plr->GetSession()->SendItemPushResult(add,false,true,false,false,plr->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()),0xFFFFFFFF,count);
		}

		return 0;
	}
	int GetClosestPlayer(lua_State * L, GameObject * ptr)
	{
		if(!ptr)
			return 0;

		float dist, d2;
		Player *  ret = NULLPLR;

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
			Lunar<Unit>::push(L,ret,false);

		return 1;
	}
	int GetDistance(lua_State * L, GameObject * ptr)
	{
		if(!ptr)
			return 0;
		Unit *  target = Lunar<Unit>::check(L, 1);
		lua_pushnumber(L,ptr->GetDistance2dSq(target));
		return 1;
	}
	int IsInWorld(lua_State * L, GameObject * ptr)
	{
		if (ptr)
			if(ptr->IsInWorld())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
		return 1;
	}
	int GetZoneId(lua_State *L, GameObject * ptr)
	{
		if(!ptr)
			return 0;
		lua_pushinteger(L,(ptr->GetZoneId()));
		return 1;
	}
	int PlaySoundToSet(lua_State * L, GameObject * ptr)
	{
		if(!ptr) return 0;
		int soundid = luaL_checkint(L,1);
		ptr->PlaySoundToSet(soundid);
		return 0;
	}
	int SpawnCreature(lua_State * L, GameObject * ptr)
	{
		if(ptr == NULL) return 0;
		uint32 entry_id = luaL_checkint(L, 1);
		float x = (float)luaL_checkint(L, 2);
		float y = (float)luaL_checkint(L, 3);
		float z = (float)luaL_checkint(L, 4);
		float o = (float)luaL_checkint(L, 5);
		uint32 faction = luaL_checkint(L, 6);
		uint32 duration = luaL_checkint(L, 7);

		if( !x || !y || !z || !entry_id || !faction /*|| !duration*/) //Shady: is it really required?
		{
			lua_pushnil(L);
			return 1;
		}
		CreatureProto* p = CreatureProtoStorage.LookupEntry(entry_id);
	    
		if(p == NULL)
		  return 0;
		shared_ptr<Creature> pC = ptr->GetMapMgr()->GetInterface()->SpawnCreature(entry_id,x,y,z,o,false,true,faction,0);
		pC->SetMapId(ptr->GetMapId());
		pC->SetInstanceID(ptr->GetInstanceID());
		pC->PushToWorld(ptr->GetMapMgr());
		if(duration)
		{
			sEventMgr.AddEvent(pC,&Creature::SafeDelete,EVENT_CREATURE_RESPAWN,duration,1,0);
		}
		Lunar<Unit>::push(L,pC);
		return 1;
	}
	int GetItemCount(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID_RET_INT(TYPEID_PLAYER);
		int itemid = luaL_checkint(L,1);
		//lua_pushinteger(L,ptr->GetItemInterface()->GetItemCount(itemid,false));
		return 1;
	}
	int SpawnGameObject(lua_State * L, GameObject * ptr)
	{
		if(ptr == NULL) return 0;
		uint32 entry_id = luaL_checkint(L, 1);
		float x = (float)luaL_checkint(L, 2);
		float y = (float)luaL_checkint(L, 3);
		float z = (float)luaL_checkint(L, 4);
		float o = (float)luaL_checkint(L, 5);
		uint32 duration = luaL_checkint(L, 6);

		if(!entry_id || !duration)
		{
			lua_pushnil(L);
			return 1;
		}

		GameObjectInfo *goi = GameObjectNameStorage.LookupEntry(entry_id);
		if (goi == NULL) return 0;
		shared_ptr<GameObject> pC = ptr->GetMapMgr()->GetInterface()->SpawnGameObject(entry_id,x,y,z,o,false,0,0);
		pC->SetMapId(ptr->GetMapId());
		pC->SetInstanceID(ptr->GetInstanceID());
		pC->Spawn(ptr->GetMapMgr());
		if(duration)
		{
			sEventMgr.AddEvent(pC, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_UPDATE, duration, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
		Lunar<GameObject>::push(L,pC);
		return 1;
	}
	int CalcDistance(lua_State * L, GameObject * ptr)
	{
		Unit *  desttar = Lunar<Unit>::check(L,1);
		if(desttar)
			lua_pushnumber(L,ptr->CalcDistance(desttar));
		return 1;
	}
	int GetSpawnX(lua_State * L, GameObject * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetSpawnX());
		return 1;
	}
	int GetSpawnY(lua_State * L, GameObject * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetSpawnY());
		return 1;
	}
	int GetSpawnZ(lua_State * L, GameObject * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetSpawnZ());
		return 1;
	}
	int GetSpawnO(lua_State * L, GameObject * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetSpawnO());
		return 1;
	}
	int GetX(lua_State * L, GameObject * ptr)
	{
		if(ptr != NULL)
			lua_pushnumber(L,ptr->GetPositionX());
		return 1;
	}
	int GetY(lua_State * L, GameObject * ptr)
	{
		if(ptr != NULL)
			lua_pushnumber(L,ptr->GetPositionY());
		return 1;
	}
	int GetZ(lua_State * L, GameObject * ptr)
	{
		if(ptr != NULL)
			lua_pushnumber(L,ptr->GetPositionZ());
		return 1;
	}
	int GetO(lua_State * L, GameObject * ptr)
	{
		if(ptr != NULL)
			lua_pushnumber(L,ptr->GetOrientation());
		return 1;
	}

	int GetInRangePlayersCount(lua_State * L, GameObject * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetInRangePlayersCount());
		return 1;
	}
	int GetEntry(lua_State * L, GameObject * ptr)
	{
		if(ptr)
			lua_pushnumber(L,ptr->GetEntry());
		return 1;
	}
	int SetOrientation(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		float newo = (float)luaL_checkint(L, 1);
		float x = ptr->GetPosition().x;
		float y = ptr->GetPosition().y;
		float z = ptr->GetPosition().z;
		shared_ptr<MapMgr> Mgr = ptr->GetMapMgr();
		ptr->RemoveFromWorld(false);
		ptr->SetPosition(x,y,z,newo,true);
		ptr->PushToWorld(Mgr);
		return 0;
	}
	int CalcRadAngle(lua_State * L, GameObject * ptr)
	{
		float x = (float)luaL_checkint(L,1 );
		float y = (float)luaL_checkint(L, 2);
		float x2 = (float)luaL_checkint(L, 3);
		float y2 = (float)luaL_checkint(L, 4);
		if(!x||!y||!x2||!y2||!ptr)
			return 0;
		lua_pushnumber(L,ptr->calcRadAngle(x,y,x2,y2));
		return 1;
	}
	int GetInstanceID(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_UNIT);
		if(ptr->GetMapMgr()->GetMapInfo()->type == INSTANCE_NULL)
			lua_pushnil(L);
		else
		lua_pushinteger(L,ptr->GetInstanceID());
		return 1;
	}
	int GetInRangePlayers(lua_State * L, GameObject * ptr)
	{
		uint32 count = 0;
		lua_newtable(L);
		for(PlayerSet::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); itr++)
		{
			if( (*itr) ->IsUnit())
			{
				count++,
				lua_pushinteger(L,count);
				Lunar<Unit>::push(L,(*itr));
				lua_rawset(L,-3);
			}
		}
		return 1;
	}

	int GetInRangeGameObjects(lua_State * L, GameObject * ptr)
	{
		uint32 count = 0;
		lua_newtable(L);
		for (ObjectSet::iterator itr = ptr->GetInRangeSetBegin();itr!= ptr->GetInRangeSetEnd();itr++)
		{
			if( (*itr)->GetTypeId() == TYPEID_GAMEOBJECT)
			{
				count++,
				lua_pushinteger(L,count);
				Lunar<GameObject>::push(L,TO_GAMEOBJECT((*itr)));
				lua_rawset(L,-3);
			}
		}
		return 1;
	}
	int IsInFront(lua_State * L, GameObject * ptr)
	{
		Unit *  target = Lunar<Unit>::check(L, 1);
		if(ptr != NULL && target != NULL)
		{
			if(ptr->isInFront((ptr)))
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
		}
		return 1;
	}
	int IsInBack(lua_State * L, GameObject * ptr)
	{
		Unit * target = Lunar<Unit>::check(L, 1);
		if(ptr != NULL && target != NULL)
		{
			if(ptr->isInBack((ptr)))
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
		}
		return 1;
	}
	int GetUInt32Value(lua_State * L, GameObject * ptr)
	{
		int field = luaL_checkint(L,1);
		if (ptr)
			lua_pushinteger(L,ptr->GetUInt32Value(field));
		return 1;
	}
	int GetUInt64Value(lua_State * L, GameObject * ptr)
	{
		int field = luaL_checkint(L,1);
		if (ptr && field)
		{
			lua_pushinteger(L,(int)ptr->GetUInt64Value(field));
		}
		return 1;
	}
	int SetUInt32Value(lua_State * L, GameObject * ptr)
	{
		int field = luaL_checkint(L,1);
		int value = luaL_checkint(L,2);
		if(ptr && field && value)
		{
			ptr->SetUInt32Value(field,value);
		}
		return 0;
	}
	int SetUInt64Value(lua_State * L, GameObject * ptr)
	{
		int field = luaL_checkint(L,1);
		int value = luaL_checkint(L,2);
		if(ptr && field && value)
		{
			ptr->SetUInt64Value(field,value);
		}
		return 0;
	}
	int SetFloatValue(lua_State * L, GameObject * ptr)
	{
		int field = luaL_checkint(L,1);
		float value = (float)luaL_checkint(L,2);
		if( ptr && value)
		{
			ptr->SetFloatValue(field,value);
		}
		return 0;
	}
	int GetFloatValue(lua_State * L, GameObject * ptr)
	{
		int field = luaL_checkint(L,1);
		if( ptr && field)
		{
			lua_pushnumber(L,ptr->GetFloatValue(field));
		}
		return 1;
	}
	int ModUInt32Value(lua_State * L, GameObject * ptr)
	{
		int field = luaL_checkint(L,1);
		int value = luaL_checkint(L,2);
		if( ptr && field && value)
		{
			ptr->ModSignedInt32Value(field,value);
		}
		return 0;
	}
	int CastSpell(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		uint32 sp = luaL_checkint(L,1);
		if( !ptr || !sp|| sp == 0) return 0;
		//ptr->CastSpell(ptr,sp,true);
		return 0;
	}
	int FullCastSpell(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		uint32 sp = luaL_checkint(L,1);
		if( !ptr || !sp|| sp == 0) return 0;
		//ptr->CastSpell(ptr,sp,false);
		return 0;
	}
	int CastSpellOnTarget(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		uint32 sp = luaL_checkint(L,1);
		Unit *  target = Lunar<Unit>::check(L,2);
		if( !ptr || !sp || sp == 0) return 0;
		//ptr->CastSpell(target,sp,true);
		return 0;
	}
	int FullCastSpellOnTarget(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		uint32 sp = luaL_checkint(L,1);
		Unit *  target = Lunar<Unit>::check(L,2);
		//ptr->CastSpell(target,sp,false);
		return 0;
	}
	int GetGUID(lua_State * L, GameObject *  ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		lua_pushnumber(L,ptr->GetGUID());
		return 1;
	}
	int Despawn(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		uint32 delay = luaL_checkint(L,1);
		uint32 respawnt = luaL_checkint(L,2);
		if(delay)
		{
			sEventMgr.AddEvent( TO_GAMEOBJECT(ptr->shared_from_this()),&GameObject::Despawn,respawnt,EVENT_GAMEOBJECT_UPDATE, delay, 1, 0);
		}
		return 0;
	}
	int RegisterAIUpdateEvent(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		uint32 time = luaL_checkint(L,1);
		if(!time) return 0;
		sEventMgr.AddEvent(TO_GAMEOBJECT(ptr->shared_from_this()),&GameObject::CallScriptUpdate,EVENT_GAMEOBJECT_UPDATE,time,0,0);
		return 0;
	}
	int ModifyAIUpdateEvent(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		uint32 ntime = luaL_checkint(L,1);
		if(!ntime) return 0;
		sEventMgr.ModifyEventTime(TO_GAMEOBJECT(ptr->shared_from_this()),EVENT_GAMEOBJECT_UPDATE,ntime);
		return 0;
	}
	int RemoveAIUpdateEvent(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		sEventMgr.RemoveEvents(TO_GAMEOBJECT(ptr->shared_from_this()),EVENT_GAMEOBJECT_UPDATE);
		return 0;
	}
	int RemoveFromWorld(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		ptr->RemoveFromWorld(true);
		return 0;
	}
	int OnGameObjectEvent(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		const char * function = luaL_checkstring(L,1);
		uint32 eventnum = luaL_checkint(L,2);
		Unit *  pMisc = Lunar<Unit>::check(L,3);
		uint32 Misc = luaL_checkint(L,4);
		if(Misc == 0) Misc = 0;
		g_engine.OnGameObjectEvent(ptr,function,eventnum,pMisc,Misc,0);
		return 0;
	}
	/* Date : 01/09/2009 */
	int SetSpawnPosition(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		float x = luaL_checklong(L,1);
		float y = luaL_checklong(L,2);
		float z = luaL_checklong(L,3);
		float o = luaL_checklong(L,4);
		if(!x || !y || ! z || !o) return 0;
		LocationVector pvect(x,y,z,o);
		//static_pointer_cast<Object>(ptr)->SetSpawnPosition(pvect);
		return 0;
	}
	int IsInArc(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		Unit *  target = Lunar<Unit>::check(L,1);
		float degrees = luaL_checklong(L,2);
		if(target == NULL || !degrees) return 0;
		( (ptr->isInArc(TO_OBJECT(target->shared_from_this()),degrees) == true) ? lua_pushboolean(L,1) : lua_pushboolean(L,0));
		return 1;
	}
	int SaveToDB(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		ptr->SaveToDB();
		return 0;
	}
	int DeleteFromDB(lua_State * L, GameObject * ptr)
	{
		CHECK_TYPEID(TYPEID_GAMEOBJECT);
		ptr->DeleteFromDB();
		return 0;
	}
	int WorldQuery(lua_State * L, GameObject * ptr)
	{
		const char * qry = luaL_checkstring(L,1);
		if(!qry) return 0;
		WorldDatabase.Query(qry);
		return 0;
	}
	/* End Date: 01/09/2009 */
}
