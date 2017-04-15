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

//
// MapMgr.h
//

#ifndef __MAPMGR_H
#define __MAPMGR_H

class MapCell;
class Map;
class Object;
class MapScriptInterface;
class WorldSession;
class GameObject;
class Creature;
class Player;
class Pet;
class Transporter;
class Corpse;
class CBattleground;
class Instance;
class InstanceScript;


enum MapMgrTimers
{
	MMUPDATE_OBJECTS = 0,
	MMUPDATE_SESSIONS = 1,
	MMUPDATE_FIELDS = 2,
	MMUPDATE_IDLE_OBJECTS = 3,
	MMUPDATE_ACTIVE_OBJECTS = 4,
	MMUPDATE_COUNT = 5
};

enum ObjectActiveState
{
	OBJECT_STATE_NONE	 = 0,
	OBJECT_STATE_INACTIVE = 1,
	OBJECT_STATE_ACTIVE   = 2,
};

typedef std::set<Object*> ObjectSet;
typedef std::set<Object*> UpdateQueue;
typedef std::set<Player*> PUpdateQueue;
typedef std::set<Player*> PlayerSet;
typedef HM_NAMESPACE::hash_map<uint32, Object*> StorageMap;
typedef set<uint64> CombatProgressMap;
typedef set<Creature*> CreatureSet;
typedef set<GameObject*> GameObjectSet;
typedef HM_NAMESPACE::hash_map<uint32, Creature*> CreatureSqlIdMap;
typedef HM_NAMESPACE::hash_map<uint32, GameObject*> GameObjectSqlIdMap;

#define MAX_TRANSPORTERS_PER_MAP 25

class Transporter;
#define RESERVE_EXPAND_SIZE 1024

#define CALL_INSTANCE_SCRIPT_EVENT( Mgr, Func ) if ( Mgr != nullptr && Mgr->GetScript() != nullptr ) Mgr->GetScript()->Func

class SERVER_DECL MapMgr : public CellHandler <MapCell>, public EventableObject,public ThreadContext
{
	friend class UpdateObjectThread;
	friend class ObjectUpdaterThread;
	friend class MapCell;
	friend class MapScriptInterface;
public:
		
	//This will be done in regular way soon

	Mutex m_objectinsertlock;
	ObjectSet m_objectinsertpool;
	void AddObject(Object *);

////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of GameObjects
/////////////////////////////////////////////
	typedef HM_NAMESPACE::hash_map<uint32, GameObject*> GameObjectMap;
	GameObjectMap m_gameObjectStorage;
	uint32 m_GOHighGuid;
	GameObject * CreateGameObject(uint32 entry);

	CHRONO_INLINE uint32 GenerateGameobjectGuid()
	{
		m_GOHighGuid &= 0x00FFFFFF;
		return ++m_GOHighGuid;
	}

	CHRONO_INLINE GameObject * GetGameObject(uint32 guid)
	{
		GameObjectMap::iterator itr = m_gameObjectStorage.find(guid);
		if( itr == m_gameObjectStorage.end() )
			return nullptr;

		return itr->second;
	}

/////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of Creatures
/////////////////////////////////////////////
	uint32 m_CreatureArraySize;
	uint32 m_CreatureHighGuid;
	HM_NAMESPACE::hash_map<const uint32,Creature*> m_CreatureStorage;
	Creature* CreateCreature(uint32 entry);

	__inline Creature* GetCreature(const uint32 guid)
	{
		return guid <= m_CreatureHighGuid ? m_CreatureStorage[guid] : NULLCREATURE;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of DynamicObjects
////////////////////////////////////////////
	uint32 m_DynamicObjectHighGuid;
	typedef HM_NAMESPACE::hash_map<uint32, DynamicObject*> DynamicObjectStorageMap;
	DynamicObjectStorageMap m_DynamicObjectStorage;
	DynamicObject * CreateDynamicObject();
	
	CHRONO_INLINE DynamicObject * GetDynamicObject(uint32 guid)
	{
		DynamicObjectStorageMap::iterator itr = m_DynamicObjectStorage.find(guid);
		return (itr != m_DynamicObjectStorage.end()) ? itr->second : 0;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of pets
///////////////////////////////////////////
	typedef HM_NAMESPACE::hash_map<uint32, Pet*> PetStorageMap;
	PetStorageMap m_PetStorage;
	__inline Pet * GetPet(uint32 guid)
	{
		PetStorageMap::iterator itr = m_PetStorage.find(guid);
		return (itr != m_PetStorage.end()) ? itr->second : 0;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of players for faster lookup
////////////////////////////////
    
    // double typedef lolz// a compile breaker..
	typedef HM_NAMESPACE::hash_map<uint32, Player*>                     PlayerStorageMap;
	PlayerStorageMap m_PlayerStorage;
	__inline Player * GetPlayer(uint32 guid)
	{
		PlayerStorageMap::iterator itr = m_PlayerStorage.find(guid);
		return (itr != m_PlayerStorage.end()) ? itr->second : 0;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of combats in progress
////////////////////////////////
	CombatProgressMap _combatProgress;
	void AddCombatInProgress(uint64 guid)
	{
		_combatProgress.insert(guid);
	}
	void RemoveCombatInProgress(uint64 guid)
	{
		_combatProgress.erase(guid);
	}

//////////////////////////////////////////////////////////
// Lookup Wrappers
///////////////////////////////////
	Unit * GetUnit(const uint64 & guid);
	Object * _GetObject(const uint64 & guid);

	bool run();
	bool Do();

	MapMgr(Map *map, uint32 mapid, uint32 instanceid);
	~MapMgr();
	void Init();
	void Destructor();
	
	void PushObject(Object *obj);
	void PushStaticObject(Object * obj);
	void RemoveObject(Object *obj, bool free_guid);
	void ChangeObjectLocation(Object *obj); // update inrange lists
	void ChangeFarsightLocation(Player *plr, Creature *farsight);

	//! Mark object as updated
	void ObjectUpdated(Object *obj);
	void UpdateCellActivity(uint32 x, uint32 y, int radius);

	// Terrain Functions
	CHRONO_INLINE float  GetLandHeight(float x, float y) { return _terrain->GetLandHeight(x, y); }
	CHRONO_INLINE bool   IsUnderground(float x, float y, float z) { return GetLandHeight(x, y) > (z + 0.5f); }
	CHRONO_INLINE float  GetLiquidHeight(float x, float y) { return _terrain->GetLiquidHeight(x, y); }
	CHRONO_INLINE uint8  GetLiquidType(float x, float y) { return _terrain->GetLiquidType(x, y); }
	CHRONO_INLINE uint16 GetAreaID(float x, float y);
	
	CHRONO_INLINE uint32 GetMapId() { return _mapId; }

	void AddForcedCell(MapCell * c);
	void RemoveForcedCell(MapCell * c);

	void PushToProcessed(Player* plr);

	CHRONO_INLINE bool HasPlayers() { return (m_PlayerStorage.size() > 0); }
	CHRONO_INLINE bool IsCombatInProgress() { return (_combatProgress.size() > 0); }
	void TeleportPlayers();

	CHRONO_INLINE uint32 GetInstanceID() { return m_instanceID; }
	CHRONO_INLINE MapInfo *GetMapInfo() { return pMapInfo; }

	CHRONO_INLINE void SetCollision(bool enable) { collision = enable; }
	CHRONO_INLINE bool IsCollisionEnabled() { return collision; }

	CHRONO_INLINE MapScriptInterface * GetInterface() { return ScriptInterface; }
	virtual int32 event_GetInstanceID() { return m_instanceID; }

	void LoadAllCells();
	CHRONO_INLINE size_t GetPlayerCount() { return m_PlayerStorage.size(); }

	void _PerformObjectDuties();
	uint32 mLoopCounter;
	uint32 lastGameobjectUpdate;
	uint32 lastUnitUpdate;
	void EventCorpseDespawn(uint64 guid);

	time_t InactiveMoveTime;
    uint32 iInstanceMode;

	void UnloadCell(uint32 x,uint32 y);
	void EventRespawnCreature(Creature * c, MapCell * p);
	void EventRespawnGameObject(GameObject * o, MapCell * c);
	void SendMessageToCellPlayers(Object * obj, WorldPacket * packet, uint32 cell_radius = 2);
	void SendChatMessageToCellPlayers(Object * obj, WorldPacket * packet, uint32 cell_radius, uint32 langpos, int32 lang, WorldSession * originator);

	Instance * pInstance;
	void BeginInstanceExpireCountdown();
	void HookOnAreaTrigger(Player * plr, uint32 id);
	
	// better hope to clear any references to us when calling this :P
	void InstanceShutdown()
	{
		pInstance = nullptr;
		Terminate();
	}

	// kill the worker thread only
	void KillThread()
	{
		pInstance=nullptr;
		thread_kill_only = true;
		Terminate();
		while(thread_running)
		{
			Sleep(100);
		}
	}

protected:

	//! Collect and send updates to clients
	void _UpdateObjects();

private:
	//! Objects that exist on map
 
	uint32 _mapId;
	set<Object*> _mapWideStaticObjects;

	bool _CellActive(uint32 x, uint32 y);
	void UpdateInRangeSet(Object *obj, Player *plObj, MapCell* cell);

public:
	// Distance a Player can "see" other objects and receive updates from them (!! ALREADY dist*dist !!)
	float m_UpdateDistance;

private:
	/* Update System */
	FastMutex m_updateMutex;		// use a user-mode mutex for extra speed
	UpdateQueue _updates;
	PUpdateQueue _processQueue;

	/* Sessions */
	SessionSet MapSessions;

	/* Map Information */
	MapInfo *pMapInfo;
	uint32 m_instanceID;

	MapScriptInterface * ScriptInterface;

	TerrainHolder* _terrain;

	bool collision;

public:
#ifdef WIN32
	DWORD threadid;
#endif

	GameObjectSet activeGameObjects;
	CreatureSet activeCreatures;
	EventableObjectHolder eventHolder;
	CBattleground * m_battleground;
	set<Corpse*> m_corpses;
	CreatureSqlIdMap _sqlids_creatures;
	GameObjectSqlIdMap _sqlids_gameobjects;

	Creature * GetSqlIdCreature(uint32 sqlid);
	GameObject * GetSqlIdGameObject(uint32 sqlid);
	deque<uint32> _reusable_guids_creature;

	bool forced_expire;
	bool thread_kill_only;
	bool thread_running;

	// world state manager stuff
	WorldStateManager m_stateManager;

	// bytebuffer caching
	ByteBuffer m_updateBuffer;
	ByteBuffer m_createBuffer;
	ByteBuffer m_updateBuildBuffer;
	ByteBuffer m_compressionBuffer;

public:

	// get!
	CHRONO_INLINE WorldStateManager& GetStateManager() { return m_stateManager; }

	// send packet functions for state manager
	void SendPacketToPlayers(int32 iZoneMask, int32 iFactionMask, WorldPacket *pData);
	void SendPacketToPlayers(int32 iZoneMask, int32 iFactionMask, StackPacket *pData);
	void SendPvPCaptureMessage(int32 iZoneMask, uint32 ZoneId, const char * Format, ...);

	// auras :< (world pvp)
	void RemoveAuraFromPlayers(int32 iFactionMask, uint32 uAuraId);
	void RemovePositiveAuraFromPlayers(int32 iFactionMask, uint32 uAuraId);
	void CastSpellOnPlayers(int32 iFactionMask, uint32 uSpellId);


public:

	// stored iterators for safe checking
	PetStorageMap::iterator __pet_iterator;
	PlayerStorageMap::iterator __player_iterator;

	CreatureSet::iterator __creature_iterator;
	GameObjectSet::iterator __gameobject_iterator;
	
	SessionSet::iterator __session_iterator_1;
	SessionSet::iterator __session_iterator_2;
	
	// Script related
	InstanceScript* GetScript() { return mInstanceScript; };
	void LoadInstanceScript();
	void CallScriptUpdate();

protected:

	InstanceScript* mInstanceScript;
};


#endif
