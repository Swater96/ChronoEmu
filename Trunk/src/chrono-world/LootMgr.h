//
// CHRONO EMU (C) 2016
//

#ifndef _LOOTMGR_H
#define _LOOTMGR_H

struct ItemPrototype;
class MapMgr;
class Player;
class LootRoll : public EventableObject
{
public:
	LootRoll(uint32 timer, uint32 groupcount, uint64 guid, uint32 slotid, uint32 itemid, uint32 itemunk1, uint32 itemunk2, MapMgr * mgr);
	~LootRoll();
	void PlayerRolled(Player *player, uint8 choice);
	void Finalize();

	int32 event_GetInstanceID();

private:
	std::map<uint32, uint32> m_NeedRolls;
	std::map<uint32, uint32> m_GreedRolls;
	set<uint32> m_passRolls;
	uint32 _groupcount;
	uint32 _slotid;
	uint32 _itemid;
	uint32 _itemunk1;
	uint32 _itemunk2;
	uint32 _remaining;
	uint64 _guid;
	MapMgr * _mgr;
};

typedef vector<pair<RandomProps*, float> > RandomPropertyVector;

typedef struct
{
	ItemPrototype * itemproto;
	uint32 displayid;
}_LootItem;

typedef std::set<uint32> LooterSet;

typedef struct
{
	_LootItem item;
	uint32 iItemsCount;
	RandomProps * iRandomProperty;
	LootRoll *roll;
	bool passed;
	LooterSet has_looted;
	uint32 ffa_loot;
}__LootItem;


typedef struct
{
	_LootItem item;
	float chance;
	float chance2;
	uint32 mincount;
	uint32 maxcount;
	uint32 ffa_loot;
}StoreLootItem;


typedef struct 
{
	uint32 count;
	StoreLootItem*items;
}StoreLootList;

struct Loot
{
	std::vector<__LootItem> items;
	uint32 gold;
	LooterSet looters;
	bool HasItems();
	bool HasLoot() { return (gold > 0) || HasItems(); }
};

struct tempy
{
	uint32 itemid;
	float chance;
	float chance_2;
	uint32 mincount;
	uint32 maxcount;
	uint32 ffa_loot;
};


//////////////////////////////////////////////////////////////////////////////////////////


typedef HM_NAMESPACE::hash_map<uint32, StoreLootList > LootStore;  

#define PARTY_LOOT_FFA	  0
#define PARTY_LOOT_MASTER   2
#define PARTY_LOOT_RR	   1
#define PARTY_LOOT_NBG	  4
#define PARTY_LOOT_GROUP	3



class SERVER_DECL LootMgr : public Singleton < LootMgr >
{
public:
	LootMgr();
	~LootMgr();

	void AddLoot(Loot * loot, uint32 itemid, uint32 mincount, uint32 maxcount, uint32 ffa_loot);
	void FillCreatureLoot(Loot * loot,uint32 loot_id, bool heroic);
	void FillGOLoot(Loot * loot,uint32 loot_id, bool heroic);
	void FillItemLoot(Loot *loot, uint32 loot_id);
	void FillFishingLoot(Loot * loot,uint32 loot_id);
	void FillSkinningLoot(Loot * loot,uint32 loot_id);
	void FillPickpocketingLoot(Loot *loot, uint32 loot_id);
	void FillDisenchantingLoot(Loot *loot, uint32 loot_id);

	bool CanGODrop(uint32 LootId,uint32 itemid);
	bool IsPickpocketable(uint32 creatureId);
	bool IsSkinnable(uint32 creatureId);
	bool IsFishable(uint32 zoneid);

	void LoadLoot();
	void LoadDelayedLoot();
	void LoadLootProp();
	
	LootStore	CreatureLoot;
	LootStore	FishingLoot;
	LootStore	SkinningLoot;
	LootStore	GOLoot;
	LootStore	ItemLoot;
	LootStore	DisenchantingLoot;
	LootStore	PickpocketingLoot;
	std::map<uint32, std::set<uint32> > quest_loot_go;

	RandomProps * GetRandomProperties(ItemPrototype * proto);

	bool is_loading;
 
	void FillObjectLootMap(map<uint32, vector<uint32> > *dest);

private:
	void LoadLootTables(const char * szTableName,LootStore * LootTable);
	void PushLoot(StoreLootList *list,Loot * loot, bool heroic, bool disenchant);
	
	map<uint32, RandomPropertyVector> _randomprops;
};

#define lootmgr LootMgr::getSingleton()

#endif
