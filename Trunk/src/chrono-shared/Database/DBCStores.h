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

#ifndef __SPELLSTORE_H
#define __SPELLSTORE_H

#include "Common.h"
#include "DataStore.h"
#include "Timer.h"

#pragma pack(push,1)

struct AreaTriggerEntry
{
	uint32 AreaTriggerID;	// 1
	uint32 Mapid;			// 2
	float x;				// 3
	float y;				// 4
	float z;				// 5
	float o;				// 6
	float edge_x;			// 7
	float edge_y;			// 8
	float edge_z;			// 9
	float edge_o;			// 10
};

struct BankSlotPrice
{
	uint32 Id;
	uint32 Price;
};

struct ItemSetEntry
{
    uint32 id;						//1
    char* name;						//2
    uint32 flag;					//18 constant
    uint32 itemid[10];				//19 - 28
    uint32 SpellID[8];				//36 - 43
    uint32 itemscount[8];			//44 - 51
    uint32 RequiredSkillID;			//52
    uint32 RequiredSkillAmt;		//53
};

struct Lock
{
    uint32 Id;
    uint32 locktype[5]; // 0 - no lock, 1 - item needed for lock, 2 - min lockping skill needed
    uint32 lockmisc[5]; // if type is 1 here is a item to unlock, else is unknow for now
    uint32 minlockskill[5]; // min skill in lockpiking to unlock.
};

struct emoteentry
{
    uint32 Id;
    uint32 textid;
    uint32 textid2;
    uint32 textid3;
    uint32 textid4;
    uint32 textid5;
    uint32 textid6;
};

struct skilllinespell //SkillLineAbility.dbc
{
    uint32 Id;
    uint32 skilline;
    uint32 spell;
    uint32 next;
    uint32 minrank;
    uint32 grey;
    uint32 green;
    uint32 reqTP;
};

struct EnchantEntry
{
    uint32 Id;			//1
    uint32 type[3];		//3-5
    int32 min[3];		//6-8 //for compat, in practice min==max
    int32 max[3];		//9-11
    uint32 spell[3];	//12-14
    char* Name;			//15
    uint32 visual;		//33
    uint32 EnchantGroups; //34
};

struct skilllineentry //SkillLine.dbc
{
    uint32 id;
    uint32 type;
    uint32 skillCostID;
    char* Name;
    //int32 NameAlt1;
    //uint32 NameAlt2;
    //uint32 NameAlt3;
    //uint32 NameAlt4;
    //uint32 NameAlt5;
    //uint32 NameAlt6;
    //uint32 NameAlt7;
    //uint32 NameAlt8;
    //uint32 NameAlt9;
    //uint32 NameAlt10;
    //uint32 NameAlt11;
    //uint32 NameAlt12;
    //uint32 NameAlt13;
    //uint32 NameAlt14;
    //uint32 NameAlt15;
    //uint32 NameFlags;
    //uint32 Description;
    //uint32 DescriptionAlt1;
    //uint32 DescriptionAlt2;
    //uint32 DescriptionAlt3;
    //uint32 DescriptionAlt4;
    //uint32 DescriptionAlt5;
    //uint32 DescriptionAlt6;
    //uint32 DescriptionAlt7;
    //uint32 DescriptionAlt8;
    //uint32 DescriptionAlt9;
    //uint32 DescriptionAlt10;
    //uint32 DescriptionAlt11;
    //uint32 DescriptionAlt12;
    //uint32 DescriptionAlt13;
    //uint32 DescriptionAlt14;
    //uint32 DescriptionAlt15;
    //uint32 DescriptionFlags;
    //uint32 unk2;

};

// Struct for the entry in Spell.dbc
struct SpellEntry
{
	uint32 Id;                              //1
	uint32 School;							//2
	uint32 Category;                        //2
	uint32 field4;                          //3 something like spelltype 0: general spells 1: Pet spells 2: Disguise / transormation spells 3: enchantment spells
	uint32 DispelType;                      //4
	uint32 MechanicsType;                   //5
	uint32 Attributes;                      //6
	uint32 AttributesEx;                    //7
	uint32 Flags3;                          //8
	uint32 Flags4;                          //9 // Flags to
	uint32 Flags5;                          //10 // Flags....
	uint32 RequiredShapeShift;              //13 // Flags BitMask for shapeshift spells
	uint32 UNK14;                           //14-> this is wrong // Flags BitMask for which shapeshift forms this spell can NOT be used in.
	uint32 Targets;                         //15 - N / M
	uint32 TargetCreatureType;              //16
	uint32 RequiresSpellFocus;              //17
	uint32 CasterAuraState;                 //19
	uint32 TargetAuraState;                 //20
	uint32 CastingTimeIndex;                //23
	uint32 RecoveryTime;                    //24
	uint32 CategoryRecoveryTime;            //25 recoverytime
	uint32 InterruptFlags;                  //26
	uint32 AuraInterruptFlags;              //27
	uint32 ChannelInterruptFlags;           //28
	uint32 procFlags;                       //29
	uint32 procChance;                      //30
	int32 procCharges;                      //31
	uint32 maxLevel;                        //32
	uint32 baseLevel;                       //33
	uint32 spellLevel;                      //34
	uint32 DurationIndex;                   //35
	uint32 powerType;                       //36
	uint32 manaCost;                        //37
	uint32 manaCostPerlevel;                //38
	uint32 manaPerSecond;                   //39
	uint32 manaPerSecondPerLevel;           //40
	uint32 rangeIndex;                      //41
	float  speed;                           //42
	uint32 modalNextSpell;                  //43
	uint32 maxstack;                        //44
	uint32 Totem[2];                        //45 - 46
	uint32 Reagent[8];                      //47 - 54
	uint32 ReagentCount[8];                 //55 - 62
	int32  EquippedItemClass;               //63  
	int32  EquippedItemSubClass;			//64  
	uint32 RequiredItemFlags;               //65
	uint32 Effect[3];                       //66 - 68
	uint32 EffectDieSides[3];               //69 - 71
	uint32 EffectBaseDice[3];               //72 - 74
	float  EffectDicePerLevel[3];           //75 - 77
	float  EffectRealPointsPerLevel[3];     //78 - 80
	int32  EffectBasePoints[3];             //81 - 83
	int32  EffectMechanic[3];               //84 - 86       Related to SpellMechanic.dbc
	uint32 EffectImplicitTargetA[3];        //87 - 89
	uint32 EffectImplicitTargetB[3];        //90 - 92
	uint32 EffectRadiusIndex[3];            //93 - 95
	uint32 EffectApplyAuraName[3];          //96 - 98
	uint32 EffectAmplitude[3];              //99 - 101
	float  EffectMultipleValue[3];                //102 - 104     This value is the $ value from description
	uint32 EffectChainTarget[3];            //105 - 107
	uint32 EffectSpellGroupRelation[3];     //108 - 110     Not sure maybe we should rename it. its the relation to field: SpellGroupType
	uint32 EffectMiscValue[3];              //111 - 113
	uint32 EffectTriggerSpell[3];           //114 - 116
	float  EffectPointsPerComboPoint[3];    //117 - 117
	uint32 SpellVisual;                     //120
	uint32 field114;                        //121
	uint32 dummy;                           //122
	uint32 CoSpell;                         //123   activeIconID;
	uint32 spellPriority;                   //124
	char* Name;                             //125
	//uint32 NameAlt[16];                   //126
	char * Rank;                            //142
	//uint32 RankAlt[16];                   //143
	char * Description;                     //159
	//uint32 DescriptionAlt[16];            //160
	char * BuffDescription;                 //176
	//uint32 BuffDescriptionAlt[16];        //177
	uint32 ManaCostPercentage;              //193
	uint32 unkflags;                        //194 
	uint32 StartRecoveryTime;               //195
	uint32 StartRecoveryCategory;           //196
	uint32 SpellFamilyName;                 //197
	uint64 SpellGroupType;					//198+199
	uint32 MaxTargets;                      //200 
	uint32 Spell_Dmg_Type;                  //201   dmg_class Integer      0=None, 1=Magic, 2=Melee, 3=Ranged
	uint32 FG;                              //202   0,1,2 related to Spell_Dmg_Type I think
	int32 FH;                               //203   related to paladin aura's 
	float dmg_multiplier[3];                //204 - 206   if the name is correct I dono
	uint32 FL;                              //207   only one spellid:6994 has this value = 369 UNUSED
	uint32 FM;                              //208   only one spellid:6994 has this value = 4 UNUSED
	uint32 FN;                              //209  3 spells 1 or 2   

	/** THIS ALL NEEDS TO BE ORGANIZED AND REARRANGED **/
	
	
    /// CUSTOM: these fields are used for the modifications made in the world.cpp
    uint32 DiminishStatus;                  //
    uint32 proc_interval;                   //!!! CUSTOM, <Fill description for variable>
	float ProcsPerMinute;                   //!!! CUSTOM, number of procs per minute    
	uint32 buffIndexType;					//!!! CUSTOM, <Fill description for variable>
    uint32 c_is_flags;						//!!! CUSTOM, store spell checks in a static way : isdamageind,ishealing
    uint32 buffType;                        //!!! CUSTOM, these are related to creating a item through a spell
    uint32 RankNumber;                      //!!! CUSTOM, this protects players from having >1 rank of a spell
    uint32 NameHash;                        //!!! CUSTOM, related to custom spells, summon spell quest related spells
	float base_range_or_radius;				//!!! CUSTOM, needed for aoe spells most of the time
    float base_range_or_radius_sqr;         //!!! CUSTOM, needed for aoe spells most of the time
	uint32 talent_tree;						//!!! CUSTOM,
	bool is_melee_spell;					//!!! CUSTOM,
	bool is_ranged_spell;					//!!! CUSTOM,
	bool spell_can_crit;					//!!! CUSTOM,
	uint32 ThreatForSpell;
	uint32 in_front_status;					//!!! CUSTOM,

	
    uint32 EffectSpellGroupRelation_high[3];     //!!! this is not contained in client dbc but server must have it
	uint32 ProcOnNameHash[3];
	
	// love me or hate me, all "In a cone in front of the caster" spells don't necessarily mean "in front"
	float cone_width;
	
	//Spell Coefficient
	float casttime_coef;                                    //!!! CUSTOM, faster spell bonus calculation
	uint32 spell_coef_flags;                                //!!! CUSTOM, store flags for spell coefficient calculations
	float fixed_dddhcoef;                                   //!!! CUSTOM, fixed DD-DH coefficient for some spells
	float fixed_hotdotcoef;                                 //!!! CUSTOM, fixed HOT-DOT coefficient for some spells
	float Dspell_coef_override;                             //!!! CUSTOM, overrides any spell coefficient calculation and use this value in DD&DH
	float OTspell_coef_override;							//!!! CUSTOM, overrides any spell coefficient calculation and use this value in HOT&DOT
	float AP_coef_override;
	float RAP_coef_override;
	
	bool self_cast_only;
	bool apply_on_shapeshift_change;
	bool always_apply;
	uint32 auraimmune_flag;									//!!! CUSTOM, this var keeps aura effects in binary format.
	
	bool Unique;											//!!! CUSTOM, Is this a unique effect? ex: Mortal Strike -50% healing.

	uint32 area_aura_update_interval;
	uint32 skilline;
	uint32 logsId;	// SpellId used to send log to client for this spell
	uint32 AdditionalAura;

	//poisons type...
	uint32 poison_type;
	
	// Infinite Customs
	uint32 forced_creature_target; // Used for Forcing targets via Database
};

struct TalentEntry
{
    uint32  TalentID;
    uint32  TalentTree;
    uint32  Row;
    uint32  Col;
    uint32  RankID[5];
    //uint32  unk[4];
    uint32  DependsOn;
    //uint32  unk1[2];
    uint32  DependsOnRank;
    //uint32  unk2[4];
	uint32  DependsOnSpell;	//21
};

struct TalentTabEntry
{
    uint32	TalentTabID;	//1
    //char*	Name;
	//unit32  unk3
	//unit32  unk4
	//unit32  unk5
	//unit32  unk6
	//unit32  unk7
	//unit32  unk8
	//unit32  unk9
	//unit32  unk10
	//unit32  unk11
	//unit32  unk12
	//unit32  unk13
	//unit32  unk14
	//unit32  unk15
	//unit32  unk16
	//unit32  unk17
	//unit32  unk18
	//unit32  unk19
	//unit32  unk20
    uint32	ClassMask;	//21
    uint32	TabPage;	//22
    //char*	InternalName;	//23
};

struct Trainerspell
{
    uint32 Id;
    uint32 skilline1;
    uint32 skilline2;
    uint32 skilline3;
    uint32 maxlvl;
    uint32 charclass;
};

struct SpellCastTime
{
    uint32 ID;
    uint32 CastTime;
    //uint32 unk1;
    //uint32 unk2;
};

struct SpellRadius
{
    uint32 ID;
    float Radius;
    //float unk1;
    float Radius2;
};

struct SpellRange
{
    uint32 ID;
    float minRange;
    float maxRange;
	uint32 type;
	char *longname;
	char *name;
};

struct SpellDuration
{
    uint32 ID;
    uint32 Duration1;
    uint32 Duration2;
    uint32 Duration3;
};

struct RandomProps
{
    uint32 ID;
    char *rpname;
    uint32 spells[3];
};

struct WorldMapOverlayEntry
{
	uint32 AreaReference;
	uint32 AreaTableID;
};

struct WMOAreaTableEntry
{
	uint32 Id;                                              // 0        m_ID index
	int32 rootId;                                           // 1        m_WMOID used in root WMO
	int32 adtId;                                            // 2        m_NameSetID used in adt file
	int32 groupId;                                          // 3        m_WMOGroupID used in group WMO
															// uint32 field4;                                       // 4        m_SoundProviderPref
															// uint32 field5;                                       // 5        m_SoundProviderPrefUnderwater
															// uint32 field6;                                       // 6        m_AmbienceID
															// uint32 field7;                                       // 7        m_ZoneMusic
															// uint32 field8;                                       // 8        m_IntroSound
	uint32 Flags;                                           // 9        m_flags (used for indoor/outdoor determination)
	uint32 areaId;                                          // 10       m_AreaTableID (AreaTable.dbc)
															// char *Name[8];                                       //          m_AreaName_lang
															// uint32 nameFlags;
};

struct AreaTable
{
    uint32 AreaId;
    uint32 mapId;
    uint32 ZoneId;
    uint32 explorationFlag;
    uint32 AreaFlags;
    uint32 EXP;//not XP
    uint32 level;
    char* name;
    uint32 category;
};

struct FactionTemplateDBC
{
    uint32 ID;
    uint32 Faction;
	uint32 FactionFlags;
    uint32 Mask;
    uint32 FriendlyMask;
    uint32 HostileMask;
    uint32 EnemyFactions[4];
    uint32 FriendlyFactions[4];
};

struct AuctionHouseDBC
{
    uint32 id;
    uint32 unk;
    uint32 fee;
    uint32 tax;
};

struct FactionDBC
{
    uint32 ID;
    int32 RepListId;
    uint32 baseRepMask[4];
    int32 baseRepValue[4];
    uint32 parentFaction;
    char* Name;
};

struct DBCTaxiNode //1.12.1 updated
{
    uint32 id;
    uint32 mapid;
    float x;
    float y;
    float z;
	//char* name[8];
	//unknown valvue
	uint32    MountCreatureID[2]; 
};

struct DBCTaxiPath
{
    uint32 id;
    uint32 from;
    uint32 to;
    uint32 price;
};

struct DBCTaxiPathNode
{
    uint32 Id;
	uint32 path;
	uint32 seq;
	uint32 mapid;
    float x;
    float y;
    float z;
    uint32 actionFlag;
    uint32 waittime;
};

struct CreatureSpellDataEntry
{
    uint32 id;
    uint32 Spells[3];
    uint32 PHSpell;
    uint32 Cooldowns[3];
    uint32 PH;
};

struct CharRaceEntry
{
	uint32 race_id;
	uint32 team_id;
	uint32 cinematic_id;
	char* name1;
};

struct CharClassEntry
{
    uint32 class_id;
    uint32 power_type;
    char* name;
};

struct CreatureFamilyEntry
{
    uint32 ID;
    float minsize;
    uint32 minlevel;
    float maxsize;
    uint32 maxlevel;
    uint32 skilline;
    uint32 tameable;		//second skill line - 270 Generic
    uint32 petdietflags;
    char* name;
};

struct MapEntry
{
    uint32 id;
    char* name_internal;
    uint32 map_type;
	uint32 is_pvp_zone;
    char* real_name;
    uint32 linked_zone;		// common zone for instance and continent map
	char* hordeIntro;		// text for PvP Zones
	char* allianceIntro;	// text for PvP Zones
    uint32 multimap_id;
};

struct ChatChannelDBC
{
	uint32 id;
	uint32 flags;
	const char* pattern;
};

struct DurabilityQualityEntry
{
    uint32 id;
    float quality_modifier;
};

struct DurabilityCostsEntry
{
    uint32 itemlevel;
    uint32 modifier[29];
};

struct WorldSafeLocs
{
    uint32    ID;                                           // 0
    uint32    map_id;                                       // 1
    float     x;                                            // 2
    float     y;                                            // 3
    float     z;                                            // 4
};

#pragma pack(pop)

CHRONO_INLINE float GetRadius(SpellRadius *radius)
{
    return radius->Radius;
}
CHRONO_INLINE uint32 GetCastTime(SpellCastTime *time)
{
    return time->CastTime;
}
CHRONO_INLINE float GetMaxRange(SpellRange *range)
{
    return range->maxRange;
}
CHRONO_INLINE float GetMinRange(SpellRange *range)
{
    return range->minRange;
}
CHRONO_INLINE uint32 GetDuration(SpellDuration *dur)
{
    return dur->Duration1;
}

#define SAFE_DBC_CODE_RETURNS			/* undefine this to make out of range/nulls return nullptr. */

template<class T>
class SERVER_DECL DBCStorage
{
	T * m_heapBlock;
	T * m_firstEntry;

	T ** m_entries;
	uint32 m_max;
	uint32 m_numrows;
	uint32 m_stringlength;
	char * m_stringData;

public:

class iterator
        {
            private:
                T* p;
            public:
                iterator(T* ip = 0) : p(ip) { }
                iterator & operator++() { ++p; return *this; }
                iterator & operator--() { --p; return *this; }
                bool operator!=(const iterator & i) { return (p != i.p); }
                T* operator*() { return p; }
        };

        iterator begin()
        {
            return iterator(&m_heapBlock[0]);
        }
        iterator end()
        {
            return iterator(&m_heapBlock[m_numrows]);
        }

	DBCStorage()
	{
		m_heapBlock = nullptr;
		m_entries = nullptr;
		m_firstEntry = nullptr;
		m_max = 0;
		m_numrows = 0;
		m_stringlength=0;
		m_stringData = nullptr;
	}

	~DBCStorage()
	{
		Cleanup();
	}

	void Cleanup()
	{
		if(m_heapBlock)
		{
			free(m_heapBlock);
			m_heapBlock = nullptr;
		}
		if(m_entries)
		{
			free(m_entries);
			m_entries = nullptr;
		}
		if( m_stringData != nullptr )
		{
			free(m_stringData);
			m_stringData = nullptr;
		}
	}

	bool Load(const char * filename, const char * format, bool load_indexed, bool load_strings)
	{
		uint32 rows;
		uint32 cols;
		uint32 useless_shit;
		uint32 string_length;
		uint32 header;
		uint32 i;
		long pos;

		FILE * f = fopen(filename, "rb");
		if(f == nullptr)
			return false;

		/* read the number of rows, and allocate our block on the heap */
		fread(&header,4,1,f);
		fread(&rows, 4, 1, f);
		fread(&cols, 4, 1, f);
		fread(&useless_shit, 4, 1, f);
		fread(&string_length, 4, 1, f);
		pos = ftell(f);

#ifdef USING_BIG_ENDIAN
		swap32(&rows); swap32(&cols); swap32(&useless_shit); swap32(&string_length);
#endif

		if( load_strings )
		{
			fseek( f, 20 + ( rows * cols * 4 ), SEEK_SET );
			m_stringData = (char*)malloc(string_length);
			m_stringlength = string_length;
			fread( m_stringData, string_length, 1, f );
		}

		fseek(f, pos, SEEK_SET);

		m_heapBlock = (T*)malloc(rows * sizeof(T));
		ASSERT(m_heapBlock);

		/* read the data for each row */
		for(i = 0; i < rows; ++i)
		{
			memset(&m_heapBlock[i], 0, sizeof(T));
			ReadEntry(f, &m_heapBlock[i], format, cols, filename);

			if(load_indexed)
			{
				/* all the time the first field in the dbc is our unique entry */
				if(*(uint32*)&m_heapBlock[i] > m_max)
					m_max = *(uint32*)&m_heapBlock[i];
			}
		}

		if(load_indexed)
		{
			m_entries = (T**)malloc(sizeof(T*) * (m_max+1));
			ASSERT(m_entries);

			memset(m_entries, 0, (sizeof(T*) * (m_max+1)));
			for(i = 0; i < rows; ++i)
			{
				if(m_firstEntry == nullptr)
					m_firstEntry = &m_heapBlock[i];

				m_entries[*(uint32*)&m_heapBlock[i]] = &m_heapBlock[i];
			}
		}

		m_numrows = rows;

		fclose(f);
		return true;
	}

	void ReadEntry(FILE * f, T * dest, const char * format, uint32 cols, const char * filename)
	{
		const char * t = format;
		uint32 * dest_ptr = (uint32*)dest;
		uint32 c = 0;
		uint32 val;
		size_t len = strlen(format);
		if(len!= cols)
			printf("!!! possible invalid format in file %s (us: %u, them: %u)\n", filename, (uint32)len, cols);

		while(*t != 0)
		{
			if((++c) > cols)
			{
				++t;
				printf("!!! Read buffer overflow in DBC reading of file %s\n", filename);
				continue;
			}

			fread(&val, 4, 1, f);
			if(*t == 'x')
			{
				++t;
				continue;		// skip!
			}
#ifdef USING_BIG_ENDIAN
			swap32(&val);
#endif
			if(*t == 's')
			{
				char ** new_ptr = (char**)dest_ptr;
				static const char * null_str = "";
				char * ptr;
				if( val < m_stringlength )
					ptr = m_stringData + val;
				else
					ptr = (char*)null_str;

				*new_ptr = ptr;
				new_ptr++;
				dest_ptr = (uint32*)new_ptr;
			}
			else
			{
				*dest_ptr = val;
				dest_ptr++;
			}

			++t;
		}
	}

	CHRONO_INLINE uint32 GetNumRows()
	{
		return m_numrows;
	}

	T * LookupEntryForced(uint32 i)
	{
#if 0
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == nullptr)
			{
				printf("LookupEntryForced failed for entry %u\n", i);
				return nullptr;
			}
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return nullptr;
			else
				return &m_heapBlock[i];
		}
#else
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == nullptr)
				return nullptr;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return nullptr;
			else
				return &m_heapBlock[i];
		}
#endif
	}

	void SetRow(uint32 i, T * t)
	{
		if(i < m_max && m_entries)
			m_entries[i] = t;
	}

#ifdef SAFE_DBC_CODE_RETURNS
	T * LookupEntry(uint32 i)
	{
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == nullptr)
				return m_firstEntry;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return &m_heapBlock[0];
			else
				return &m_heapBlock[i];
		}
	}

	T * LookupRow(uint32 i)
	{
		if(i >= m_numrows)
			return &m_heapBlock[0];
		else
			return &m_heapBlock[i];
	}

#else

	T * LookupEntry(uint32 i)
	{
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == nullptr)
				return nullptr;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return nullptr;
			else
				return m_heapBlock[i];
		}
	}

	T * LookupRow(uint32 i)
	{
		if(i >= m_numrows)
			return nullptr;
		else
			return m_heapBlock[i];
	}

#endif
};

extern SERVER_DECL DBCStorage<AreaTriggerEntry> dbcAreaTrigger;
extern SERVER_DECL DBCStorage<WMOAreaTableEntry> dbcWMOArea;
extern SERVER_DECL DBCStorage<WorldSafeLocs> dbcworldsafelocs;
extern SERVER_DECL DBCStorage<ItemSetEntry> dbcItemSet;
extern SERVER_DECL DBCStorage<Lock> dbcLock;
extern SERVER_DECL DBCStorage<SpellEntry> dbcSpell;
extern SERVER_DECL DBCStorage<SpellDuration> dbcSpellDuration;
extern SERVER_DECL DBCStorage<SpellRange> dbcSpellRange;
extern SERVER_DECL DBCStorage<emoteentry> dbcEmoteEntry;
extern SERVER_DECL DBCStorage<SpellRadius> dbcSpellRadius;
extern SERVER_DECL DBCStorage<SpellCastTime> dbcSpellCastTime;
extern SERVER_DECL DBCStorage<AreaTable> dbcArea;
extern SERVER_DECL DBCStorage<FactionTemplateDBC> dbcFactionTemplate;
extern SERVER_DECL DBCStorage<FactionDBC> dbcFaction;
extern SERVER_DECL DBCStorage<EnchantEntry> dbcEnchant;
extern SERVER_DECL DBCStorage<RandomProps> dbcRandomProps;
extern SERVER_DECL DBCStorage<skilllinespell> dbcSkillLineSpell;
extern SERVER_DECL DBCStorage<skilllineentry> dbcSkillLine;
extern SERVER_DECL DBCStorage<DBCTaxiNode> dbcTaxiNode;
extern SERVER_DECL DBCStorage<DBCTaxiPath> dbcTaxiPath;
extern SERVER_DECL DBCStorage<DBCTaxiPathNode> dbcTaxiPathNode;
extern SERVER_DECL DBCStorage<AuctionHouseDBC> dbcAuctionHouse;
extern SERVER_DECL DBCStorage<TalentEntry> dbcTalent;
extern SERVER_DECL DBCStorage<TalentTabEntry> dbcTalentTab;
extern SERVER_DECL DBCStorage<CreatureSpellDataEntry> dbcCreatureSpellData;
extern SERVER_DECL DBCStorage<CreatureFamilyEntry> dbcCreatureFamily;
extern SERVER_DECL DBCStorage<CharClassEntry> dbcCharClass;
extern SERVER_DECL DBCStorage<CharRaceEntry> dbcCharRace;
extern SERVER_DECL DBCStorage<MapEntry> dbcMap;
extern SERVER_DECL DBCStorage<ChatChannelDBC> dbcChatChannels;
extern SERVER_DECL DBCStorage<DurabilityCostsEntry> dbcDurabilityCosts;
extern SERVER_DECL DBCStorage<DurabilityQualityEntry> dbcDurabilityQuality;
extern SERVER_DECL DBCStorage<BankSlotPrice> dbcBankSlotPrices;
extern SERVER_DECL DBCStorage<BankSlotPrice> dbcStableSlotPrices; //uses same structure as Bank
extern SERVER_DECL DBCStorage<WorldMapOverlayEntry> dbcWorldMapOverlay;

bool LoadDBCs();

#endif
