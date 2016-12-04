/*
 * Ascent MMORPG Server
 * LUA Interface
 * Copyright (C) 2007 Burlex <burlex@gmail.com>
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

#ifndef __LUAENGINE_H
#define __LUAENGINE_H

extern "C" {		// we're C++, and LUA is C, so the compiler needs to know to use C function names.
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
};

class LuaEngine;
class LuaEngineMgr;
#ifdef ENABLE_LUA_HOOKS
class pHookInterface;
#endif ENABLE_LUA_HOOKS

extern LuaEngineMgr g_luaMgr;
extern LuaEngine g_engine;

GossipMenu * Menu;

/** Macros for calling lua-based events
 */
#define LUA_ON_UNIT_EVENT(unit,eventtype,miscunit,misc) if(unit->GetTypeId()==TYPEID_UNIT && unit->IsInWorld()) { unit->GetMapMgr()->GetScriptEngine()->OnUnitEvent(unit,eventtype,miscunit,misc); }
#define LUA_ON_QUEST_EVENT(plr,quest,eventtype,miscobject) if(plr->IsPlayer() && plr->IsInWorld() && miscobject->IsInWorld() && !miscobject->IsPlayer()) { plr->GetMapMgr()->GetScriptEngine()->OnQuestEvent(plr,quest,eventtype,miscobject); } 
#define LUA_ON_GO_EVENT(unit,evtype,miscunit) if(unit->GetTypeId()==TYPEID_GAMEOBJECT && unit->IsInWorld()) { unit->GetMapMgr()->GetScriptEngine()->OnGameObjectEvent(unit,evtype,miscunit); }
#define LUA_ON_GOSSIP_EVENT(object, evtype, player, id, intid, code) if(object->IsInWorld()) { object->GetMapMgr()->GetScriptEngine()->OnGossipEvent(object, evtype, player, id, intid, code); }
#define LUA_CALL_FUNC(unit,funcname) if(unit->GetTypeId()==TYPEID_UNIT && unit->IsInWorld()) { unit->GetMapMgr()->GetScriptEngine()->CallFunction(unit,funcname); }

/** Quest Events
 */
enum QuestEvents
{
	QUEST_EVENT_ON_ACCEPT = 1,
	QUEST_EVENT_ON_COMPLETE = 2,
	QUEST_EVENT_ON_CANCEL = 3,
	QUEST_EVENT_GAMEOBJECT_ACTIVATE = 4,
	QUEST_EVENT_ON_CREATURE_KILL  = 5,
	QUEST_EVENT_ON_EXPLORE_AREA = 6,
	QUEST_EVENT_ON_PLAYER_ITEMPICKUP = 7,
	QUEST_EVENT_COUNT,
};

/** Creature Events
 */
enum CreatureEvents
{
	CREATURE_EVENT_ON_ENTER_COMBAT		= 1,
	CREATURE_EVENT_ON_LEAVE_COMBAT		= 2,
	CREATURE_EVENT_ON_TARGET_DIED		= 3,
	CREATURE_EVENT_ON_DIED		= 4,
	CREATURE_EVENT_ON_TARGET_PARRIED		= 5,
	CREATURE_EVENT_ON_TARGET_DODGED		= 6,
	CREATURE_EVENT_ON_TARGET_BLOCKED		= 7,
	CREATURE_EVENT_ON_TARGET_CRIT_HIT		= 8,
	CREATURE_EVENT_ON_PARRY		= 9,
	CREATURE_EVENT_ON_DODGED		= 10,
	CREATURE_EVENT_ON_BLOCKED		= 11,
	CREATURE_EVENT_ON_CRIT_HIT		= 12,
	CREATURE_EVENT_ON_HIT		= 13,
	CREATURE_EVENT_ON_ASSIST_TARGET_DIED		= 14,
	CREATURE_EVENT_ON_FEAR		= 15,
	CREATURE_EVENT_ON_FLEE		= 16,
	CREATURE_EVENT_ON_CALL_FOR_HELP		= 17,
	CREATURE_EVENT_ON_LOAD		= 18,
	CREATURE_EVENT_ON_REACH_WP		= 19,
	CREATURE_EVENT_ON_LOOT_TAKEN		= 20,
	CREATURE_EVENT_ON_AIUPDATE		= 21,
	CREATURE_EVENT_ON_EMOTE		= 22,
	CREATURE_EVENT_USER_DEFINED = 23,
	CREATURE_EVENT_ON_DAMAGE_TAKEN = 24,
	CREATURE_EVENT_ON_CREATEAISCRIPT = 25,
	CREATURE_EVENT_COUNT,
};

/** GameObject Events
 */
enum GameObjectEvents
{
	GAMEOBJECT_EVENT_ON_CREATE			= 1,
	GAMEOBJECT_EVENT_ON_SPAWN			= 2,
	GAMEOBJECT_EVENT_ON_LOOT_TAKEN		= 3,
	GAMEOBJECT_EVENT_ON_USE				= 4,
	GAMEOBJECT_EVENT_AIUPDATE			= 5,
	GAMEOBJECT_EVENT_ON_DESPAWN			= 6,
	GAMEOBJECT_EVENT_USER_DEFINED = 7,
	GAMEOBJECT_EVENT_COUNT,
};

/** Gossip Events
*/
enum GossipEvents
{
	GOSSIP_EVENT_ON_TALK			= 1,
	GOSSIP_EVENT_ON_SELECT_OPTION	= 2,
	GOSSIP_EVENT_ON_END             = 3,
	GOSSIP_EVENT_COUNT,
};

enum RandomFlags
{
	RANDOM_ANY           = 0,
	RANDOM_IN_SHORTRANGE = 1,
	RANDOM_IN_MIDRANGE   = 2,
	RANDOM_IN_LONGRANGE  = 3,
	RANDOM_WITH_MANA     = 4,
	RANDOM_WITH_RAGE     = 5,
	RANDOM_WITH_ENERGY   = 6,
	RANDOM_NOT_MAINTANK  = 7
};
class LuaEngine
{
private:
	lua_State * L;
	Mutex m_Lock;

public:
	LuaEngine();
	~LuaEngine();

	void LoadScripts();
	void Shutdown();
	void Restart();
	void RegisterCoreFunctions();
	HEARTHSTONE_INLINE Mutex& GetLock() { return m_Lock; }

	void OnUnitEvent(Unit *  pUnit, const char * FunctionName, uint32 EventType, Unit *  pMiscUnit, uint32 Misc, int32 Misc2, float Misc3, bool Yes, ItemPrototype * Misc4);
	void OnUnitEvent(Unit *  pUnit, const char * FunctionName, uint32 EventType, Unit *  pMiscUnit, uint32 Misc) { OnUnitEvent(pUnit,FunctionName,EventType,pMiscUnit,Misc,0,0,false,NULL); }
	void OnUnitEvent(Unit *  pUnit, const char * FunctionName ,uint32 EventType, Unit *  pMiscUnit, int32 Misc2){ OnUnitEvent(pUnit,FunctionName,EventType,pMiscUnit,0,Misc2,0,false,NULL); }
	void OnUnitEvent(Unit *  pUnit, const char * FunctionName, uint32 EventType, Unit *  pMiscUnit, float Misc3) { OnUnitEvent(pUnit,FunctionName,EventType,pMiscUnit,0,0,Misc3,false,NULL); }
	void OnUnitEvent(Unit *  pUnit, const char * FunctionName, uint32 EventType, Unit *  pMiscUnit, uint32 Misc, bool Yes) { OnUnitEvent(pUnit,FunctionName,EventType,pMiscUnit,Misc,0,0,Yes,NULL); }
	void OnUnitEvent(Unit *  pUnit, const char * FunctionName, uint32 EventType, Unit *  pMiscUnit, ItemPrototype * Proto){ OnUnitEvent(pUnit,FunctionName, EventType,pMiscUnit,0,0,0,false,Proto); }
	void OnQuestEvent(Player *  QuestOwner, const char * FunctionName, uint32 QuestID, uint32 EventType, Player *  QuestStarter,uint32 Misc, uint32 Misc2);
	void OnGameObjectEvent(GameObject *  pGameObject, const char * FunctionName, uint32 EventType, Unit *  pMiscUnit, uint32 Misc, ItemPrototype* Misc2);
    void OnGossipEvent(Object *  pObject, const char * FunctionName, uint32 EventType, Player *  mPlayer, uint32 Id, uint32 IntId, const char * Code);
	void CallFunction(Unit *  pUnit, const char * FuncName);
#ifdef ENABLE_LUA_HOOKS
	void OnServerHook(Player * , uint32,Object * , const char *, uint32, uint32);
	void OnServerHook(Player * , uint32, Object * , const char *, Quest *, Guild *, SpellEntry *);
	void OnServerHook(Player * , uint32, Object * , const char *, uint32, uint32, const char *, const char*);
	void OnServerHook(Player * , uint32, Object * , const char *, ArenaTeam *, bool, bool);
#endif
};

typedef list<const char * > LuaUnitBinding;
struct LuaQuestBinding { const char * Functions[QUEST_EVENT_COUNT]; };
struct LuaGameObjectBinding { const char * Functions[GAMEOBJECT_EVENT_COUNT]; };
struct LuaUnitGossipBinding { const char * Functions[GOSSIP_EVENT_COUNT]; };
struct LuaItemGossipBinding { const char * Functions[GOSSIP_EVENT_COUNT]; };
struct LuaGOGossipBinding { const char * Functions[GOSSIP_EVENT_COUNT]; };

#ifdef ENABLE_LUA_HOOKS
typedef list<const char*> LuaSHBinding;
typedef unordered_map<const char*, bool> pHoldBoolsByChar;
typedef unordered_map<uint32,pHoldBoolsByChar> CompleteMap;
#endif

class LuaEngineMgr
{
private:
	typedef unordered_map<uint32, LuaUnitBinding*> UnitBindingMap;
	typedef unordered_map<uint32, LuaQuestBinding> QuestBindingMap;
	typedef unordered_map<uint32, LuaGameObjectBinding> GameObjectBindingMap;
	typedef unordered_map<uint32, LuaUnitGossipBinding> GossipUnitScriptsBindingMap;
	typedef unordered_map<uint32, LuaItemGossipBinding> GossipItemScriptsBindingMap;
	typedef unordered_map<uint32, LuaGOGossipBinding> GossipGOScriptsBindingMap;
	UnitBindingMap m_unitBinding;
	QuestBindingMap m_questBinding;
	GameObjectBindingMap m_gameobjectBinding;
	GossipUnitScriptsBindingMap m_unit_gossipBinding;
	GossipItemScriptsBindingMap m_item_gossipBinding;
	GossipGOScriptsBindingMap m_go_gossipBinding;
#ifdef ENABLE_LUA_HOOKS
	LuaSHBinding * m_shBinding;
#endif

public:
#ifdef ENABLE_LUA_HOOKS
	pHookInterface * m_HookInterface;
#endif
	LuaEngine m_engine;
	LuaEngineMgr();
	void Startup();
	void ReloadClasses();
	void Unload();
	// Unit Event Functions //
	void RegisterUnitEvent(uint32 Id, uint32 Event, const char * FunctionName);
	void ModifyUnitEvent(uint32 id, uint32 evt, const char * oldfunc, const char * newfunc);
	void RemoveUnitEvent(uint32 id, uint32 evt, const char * FunctionName);
	// Quest Event Functions //
	void RegisterQuestEvent(uint32 Id, uint32 Event, const char * FunctionName);
	// GO Event Functions //
	void RegisterGameObjectEvent(uint32 Id, uint32 Event, const char * FunctionName);
	// Unit Gossip Functions //
    void RegisterUnitGossipEvent(uint32 Id, uint32 Event, const char * FunctionName);
	// Item Gossip Functions //
    void RegisterItemGossipEvent(uint32 Id, uint32 Event, const char * FunctionName);
	// GO Gossip Functions //
    void RegisterGOGossipEvent(uint32 Id, uint32 Event, const char * FunctionName);

	QueryResult * WorldQuery(const char *);
#ifdef ENABLE_LUA_HOOKS
	// ServerHook evts //
	void RegisterServerHookEvent(uint32,const char *);
	void RemoveServerHookEvent(uint32 id, const char * funcName);
	void ModifyServerHookEvent(uint32 evt, const char * func1, const char * func2);
	bool CheckBoolResult(uint32,const char*);
#endif


	LuaUnitBinding * GetUnitBinding(uint32 Id)
	{
		UnitBindingMap::iterator itr = m_unitBinding.find(Id);
		return (itr == m_unitBinding.end()) ? NULL : itr->second;
	}

	LuaQuestBinding * GetQuestBinding(uint32 Id)
	{
		QuestBindingMap::iterator itr = m_questBinding.find(Id);
		return (itr == m_questBinding.end()) ? NULL : &itr->second;
	}

	LuaGameObjectBinding * GetGameObjectBinding(uint32 Id)
	{
		GameObjectBindingMap::iterator itr =m_gameobjectBinding.find(Id);
		return (itr == m_gameobjectBinding.end()) ? NULL : &itr->second;
	}
    // Gossip Stuff
    LuaUnitGossipBinding * GetLuaUnitGossipBinding(uint32 Id)
	{
		GossipUnitScriptsBindingMap::iterator itr = m_unit_gossipBinding.find(Id);
		return (itr == m_unit_gossipBinding.end()) ? NULL : &itr->second;
	}

    LuaItemGossipBinding * GetLuaItemGossipBinding(uint32 Id)
	{
		GossipItemScriptsBindingMap::iterator itr = m_item_gossipBinding.find(Id);
		return (itr == m_item_gossipBinding.end()) ? NULL : &itr->second;
	}

    LuaGOGossipBinding * GetLuaGOGossipBinding(uint32 Id)
	{
		GossipGOScriptsBindingMap::iterator itr = m_go_gossipBinding.find(Id);
		return (itr == m_go_gossipBinding.end()) ? NULL : &itr->second;
	}
#ifdef ENABLE_LUA_HOOKS
	LuaSHBinding *GetLuaHookBinding() { return m_shBinding; }
#endif
};

#endif







