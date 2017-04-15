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

void CreateDummySpell(uint32 id)
{
	const char * name = "Dummy Trigger";
	SpellEntry * sp = new SpellEntry;
	memset(sp, 0, sizeof(SpellEntry));
	sp->Id = id;
	sp->Attributes = 384;
	sp->AttributesEx = 268435456;
	sp->Flags3 = 4;
	sp->CastingTimeIndex=1;
	sp->procChance=75;
	sp->rangeIndex=13;
	sp->EquippedItemClass=uint32(-1);
	sp->Effect[0]=3;
	sp->EffectImplicitTargetA[0]=25;
	sp->NameHash=crc32((const unsigned char*)name, (unsigned int)strlen(name));
	sp->dmg_multiplier[0]=1.0f;
	sp->FH=-1;
	dbcSpell.SetRow(id,sp);
	sWorld.dummyspells.push_back(sp);
}

uint32 GetSpellClass(SpellEntry *sp)
{
	switch(sp->skilline)
	{
		case SKILL_ARMS:
		case SKILL_FURY:
		case SKILL_PROTECTION:
			return WARRIOR;
		case SKILL_HOLY2:
		case SKILL_PROTECTION2:
		case SKILL_RETRIBUTION:
			return PALADIN;
		case SKILL_BEAST_MASTERY:
		case SKILL_SURVIVAL:
		case SKILL_MARKSMANSHIP:
			return HUNTER;
		case SKILL_ASSASSINATION:
		case SKILL_COMBAT:
		case SKILL_SUBTLETY:
			return ROGUE;
		case SKILL_DISCIPLINE:
		case SKILL_HOLY:
		case SKILL_SHADOW:
			return PRIEST;
		case SKILL_ENHANCEMENT:
		case SKILL_RESTORATION:
		case SKILL_ELEMENTAL_COMBAT:
			return SHAMAN;
		case SKILL_FROST:
		case SKILL_FIRE:
		case SKILL_ARCANE:
			return MAGE;
		case SKILL_AFFLICTION:
		case SKILL_DEMONOLOGY:
		case SKILL_DESTRUCTION:
			return WARLOCK;
		case SKILL_RESTORATION2:
		case SKILL_BALANCE:
		case SKILL_FERAL_COMBAT:
			return DRUID;
	}

	return -1;
}

// fills array 'arr' with integers in arguments and returns its new size. Last argument must be 0!
uint32 fill( uint32* arr, ... ){
	va_list vl;
	uint32 i;
	va_start( vl, arr );
	for( i = 0; i < 100; ++i ){
		arr[i] = va_arg( vl, uint32 );
		if(arr[i] == 0)
			break;
	}
	va_end( vl );
	return i;
}

// Generates SpellNameHashes.h
void GenerateNameHashesFile()
{
	const uint32 fieldSize = 81;
	const char* prefix = "SPELL_HASH_";
	uint32 prefixLen = strlen(prefix);
	DBCFile dbc;

	if( !dbc.open( "DBC/Spell.dbc" ) )
	{
		Log.Error("World", "Cannot find file ./DBC/Spell.dbc" );
		return;
	}
	uint32 cnt = (uint32)dbc.getRecordCount();
	uint32 namehash = 0;
	FILE * f = fopen("SpellNameHashes.h", "w");
	char spaces[fieldSize], namearray[fieldSize];
	strcpy(namearray, prefix);
	char* name = &namearray[prefixLen];
	for(int i=0;i<fieldSize-1;i++)
		spaces[i] = ' ';
	spaces[fieldSize-1] = 0;
	uint32 nameTextLen = 0, nameLen = 0;
	for(uint32 x=0; x < cnt; x++)
	{
		const char* nametext = dbc.getRecord(x).getString(139);
		nameTextLen = (unsigned int)strlen(nametext);
		strncpy(name, nametext, fieldSize-prefixLen-2);	// Cut it to fit in field size
		name[fieldSize-prefixLen-2] = 0; // in case nametext is too long and strncpy didn't copy the nullptr
		nameLen = (unsigned int)strlen(name);
		for(uint32 i = 0;i<nameLen;i++)
		{
			if(name[i] >= 'a' && name[i] <= 'z')
				name[i] = toupper(name[i]);
			else if(!(name[i] >= '0' && name[i] <= '9') &&
				!(name[i] >= 'A' && name[i] <= 'Z'))
				name[i] = '_';
		}
		namehash = crc32((const unsigned char*)nametext, nameTextLen);
		int32 numSpaces = fieldSize-prefixLen-nameLen-1;
		if(numSpaces < 0)
			fprintf(f, "WTF");
		spaces[numSpaces] = 0;
		fprintf(f, "#define %s%s0x%08X\n", namearray, spaces, namehash);
		spaces[numSpaces] = ' ';
	}
	fclose(f);
}

void ApplySingleSpellFixes(SpellEntry *sp)
{
	ApplySingleDruidSpellFixes(sp);
	ApplySingleHunterSpellFixes(sp);
	ApplySingleMageSpellFixes(sp);
	ApplySinglePaladinSpellFixes(sp);
	ApplySingleRogueSpellFixes(sp);
	ApplySingleShamanSpellFixes(sp);
	ApplySingleWarlockSpellFixes(sp);
	ApplySingleWarriorSpellFixes(sp);
	ApplySinglePriestSpellFixes(sp);
	
	switch(sp->Id)
	{
		//Golemagg's Magma Splash
		
		case 13879:
			{
				sp->procFlags = PROC_TARGET_MELEE_HIT;
				sp->c_is_flags |= SPELL_FLAG_CANNOT_PROC_ON_SELF;
			}
	
		default:
		return;
	}
}

void ApplyNormalFixes()
{
	//Updating spell.dbc
	SpellEntry *sp;

	Log.Notice("World", "Processing %u spells...", dbcSpell.GetNumRows());
	uint32 cnt = (uint32)dbcSpell.GetNumRows();
	uint32 effect;

	uint32 All_Seal_Groups_Combined=0;
	uint32 group_relation_paladin_sanctified_light = 0;
	uint32 group_relation_shaman_enhancing_totems = 0;
	uint32 group_relation_shaman_restorative_totems = 0;
	uint32 group_relation_shaman_totems = 0;
	uint32 group_relation_shaman_lightning = 0;
	uint32 group_relation_shaman_shock = 0;
	uint32 group_relation_shaman_mental_quickness = 0;
	
	map<uint32, uint32> talentSpells;
	map<uint32,uint32>::iterator talentSpellIterator;
	unsigned int i,j;
	for(i = 0; i < dbcTalent.GetNumRows(); ++i)
	{
		TalentEntry * tal = dbcTalent.LookupRow(i);
		for(j = 0; j < 5; ++j)
			if(tal->RankID[j] != 0)
				talentSpells.insert(make_pair(tal->RankID[j], tal->TalentTree));
	}

	for(uint32 x=0; x < cnt; x++)
	{
		sp = dbcSpell.LookupRow(x);
		
		uint32 result = 0;

		uint32 type = 0;
		uint32 namehash = 0;

		sp->forced_creature_target = 0;
		sp->AdditionalAura = 0;
		sp->poison_type = 0;
		sp->self_cast_only = false;
		sp->Unique = false;
		sp->apply_on_shapeshift_change = false;
		sp->always_apply = false;

		
		
		// hash the name
		//!!!!!!! representing all strings on 32 bits is dangerous. There is a chance to get same hash for a lot of strings ;)
		namehash = crc32((const unsigned char*)sp->Name, (unsigned int)strlen(sp->Name));
		sp->NameHash   = namehash; //need these set before we start processing spells

		float radius = 0.0f;
		if(sp->EffectRadiusIndex[0] != 0)
			radius=::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[0]));
		
		if( sp->EffectRadiusIndex[1] != 0 )
			radius = std::max(radius,::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[1])));

		if( sp->EffectRadiusIndex[2] != 0 )
			radius=std::max(::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[2])),radius);

		radius=std::max(GetMaxRange(dbcSpellRange.LookupEntry(sp->rangeIndex)),radius);
		sp->base_range_or_radius = radius;
		sp->base_range_or_radius_sqr = radius*radius;
		sp->cone_width = 0.0f;
		sp->area_aura_update_interval = 2000;

		// correct caster aura state
		if( sp->CasterAuraState != 0 )
		{
			switch( sp->CasterAuraState )
			{
			case 1:
				sp->CasterAuraState = AURASTATE_FLAG_DODGE_BLOCK;
				break;

			case 2:
				sp->CasterAuraState = AURASTATE_FLAG_HEALTH20;
				break;

			case 3:
				sp->CasterAuraState = AURASTATE_FLAG_BERSERK;
				break;

			case 5:
				sp->CasterAuraState = AURASTATE_FLAG_JUDGEMENT;
				break;

			case 7:
				sp->CasterAuraState = AURASTATE_FLAG_PARRY;
				break;

			case 10:
				sp->CasterAuraState = AURASTATE_FLAG_LASTKILLWITHHONOR;
				break;

			case 11:
				sp->CasterAuraState = AURASTATE_FLAG_CRITICAL;
				break;

			case 13:
				sp->CasterAuraState = AURASTATE_FLAG_HEALTH35;
				break;

			case 14:
				sp->CasterAuraState = AURASTATE_FLAG_IMMOLATE;
				break;

			case 15:
				sp->CasterAuraState = AURASTATE_FLAG_REJUVENATE;
				break;

			case 16:
				sp->CasterAuraState = AURASTATE_FLAG_POISON;
				break;

			default:
				Log.Error("AuraState", "Spell %u (%s) has unknown caster aura state %u\n", sp->Id, sp->Name, sp->CasterAuraState);
				break;
			}
		}

		if( sp->TargetAuraState != 0 )
		{
			switch( sp->TargetAuraState )
			{
			case 1:
				sp->TargetAuraState = AURASTATE_FLAG_DODGE_BLOCK;
				break;

			case 2:
				sp->TargetAuraState = AURASTATE_FLAG_HEALTH20;
				break;

			case 3:
				sp->TargetAuraState = AURASTATE_FLAG_BERSERK;
				break;

			case 5:
				sp->TargetAuraState = AURASTATE_FLAG_JUDGEMENT;
				break;

			case 7:
				sp->TargetAuraState = AURASTATE_FLAG_PARRY;
				break;

			case 10:
				sp->TargetAuraState = AURASTATE_FLAG_LASTKILLWITHHONOR;
				break;

			case 11:
				sp->TargetAuraState = AURASTATE_FLAG_CRITICAL;
				break;

			case 13:
				sp->TargetAuraState = AURASTATE_FLAG_HEALTH35;
				break;

			case 14:
				sp->TargetAuraState = AURASTATE_FLAG_IMMOLATE;
				break;

			case 15:
				sp->TargetAuraState = AURASTATE_FLAG_REJUVENATE;
				break;

			case 16:
				sp->TargetAuraState = AURASTATE_FLAG_POISON;
				break;

			default:
				Log.Error("AuraState", "Spell %u (%s) has unknown target aura state %u\n", sp->Id, sp->Name, sp->TargetAuraState);
				break;
			}
		}
		
		// apply on shapeshift change
		if( sp->NameHash == SPELL_HASH_TRACK_HUMANOIDS )
			sp->apply_on_shapeshift_change = true;

		if( sp->NameHash == SPELL_HASH_BLOOD_FURY )
			sp->always_apply = true;

		//there are some spells that change the "damage" value of 1 effect to another : devastate = bonus first then damage
		//this is a total bullshit so remove it when spell system supports effect overwriting
		for( uint32 col1_swap = 0; col1_swap < 2 ; col1_swap++ )
		{
			for( uint32 col2_swap = col1_swap ; col2_swap < 3 ; col2_swap++ )
			{
				if( sp->Effect[col1_swap] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE && sp->Effect[col2_swap] == SPELL_EFFECT_DUMMYMELEE )
				{
					uint32 temp;
					float ftemp;
					temp = sp->Effect[col1_swap];			sp->Effect[col1_swap] = sp->Effect[col2_swap] ;						sp->Effect[col2_swap] = temp;
					temp = sp->EffectDieSides[col1_swap];	sp->EffectDieSides[col1_swap] = sp->EffectDieSides[col2_swap] ;		sp->EffectDieSides[col2_swap] = temp;
					temp = sp->EffectBaseDice[col1_swap];	sp->EffectBaseDice[col1_swap] = sp->EffectBaseDice[col2_swap] ;		sp->EffectBaseDice[col2_swap] = temp;
					ftemp = sp->EffectDicePerLevel[col1_swap];			sp->EffectDicePerLevel[col1_swap] = sp->EffectDicePerLevel[col2_swap] ;				sp->EffectDicePerLevel[col2_swap] = ftemp;
					ftemp = sp->EffectRealPointsPerLevel[col1_swap];	sp->EffectRealPointsPerLevel[col1_swap] = sp->EffectRealPointsPerLevel[col2_swap] ;	sp->EffectRealPointsPerLevel[col2_swap] = ftemp;
					temp = sp->EffectBasePoints[col1_swap];	sp->EffectBasePoints[col1_swap] = sp->EffectBasePoints[col2_swap] ;	sp->EffectBasePoints[col2_swap] = temp;
					temp = sp->EffectMechanic[col1_swap];	sp->EffectMechanic[col1_swap] = sp->EffectMechanic[col2_swap] ;	sp->EffectMechanic[col2_swap] = temp;
					temp = sp->EffectImplicitTargetA[col1_swap];	sp->EffectImplicitTargetA[col1_swap] = sp->EffectImplicitTargetA[col2_swap] ;	sp->EffectImplicitTargetA[col2_swap] = temp;
					temp = sp->EffectImplicitTargetB[col1_swap];	sp->EffectImplicitTargetB[col1_swap] = sp->EffectImplicitTargetB[col2_swap] ;	sp->EffectImplicitTargetB[col2_swap] = temp;
					temp = sp->EffectRadiusIndex[col1_swap];	sp->EffectRadiusIndex[col1_swap] = sp->EffectRadiusIndex[col2_swap] ;	sp->EffectRadiusIndex[col2_swap] = temp;
					temp = sp->EffectApplyAuraName[col1_swap];	sp->EffectApplyAuraName[col1_swap] = sp->EffectApplyAuraName[col2_swap] ;	sp->EffectApplyAuraName[col2_swap] = temp;
					temp = sp->EffectAmplitude[col1_swap];		sp->EffectAmplitude[col1_swap] = sp->EffectAmplitude[col2_swap] ;	sp->EffectAmplitude[col2_swap] = temp;
					ftemp = sp->EffectMultipleValue[col1_swap];		sp->EffectMultipleValue[col1_swap] = sp->EffectMultipleValue[col2_swap] ;	sp->EffectMultipleValue[col2_swap] = ftemp;
					temp = sp->EffectChainTarget[col1_swap];	sp->EffectChainTarget[col1_swap] = sp->EffectChainTarget[col2_swap] ;	sp->EffectChainTarget[col2_swap] = temp;
					temp = sp->EffectMiscValue[col1_swap];		sp->EffectMiscValue[col1_swap] = sp->EffectMiscValue[col2_swap] ;	sp->EffectMiscValue[col2_swap] = temp;
					temp = sp->EffectTriggerSpell[col1_swap];	sp->EffectTriggerSpell[col1_swap] = sp->EffectTriggerSpell[col2_swap] ;	sp->EffectTriggerSpell[col2_swap] = temp;
					ftemp = sp->EffectPointsPerComboPoint[col1_swap];	sp->EffectPointsPerComboPoint[col1_swap] = sp->EffectPointsPerComboPoint[col2_swap] ;	sp->EffectPointsPerComboPoint[col2_swap] = ftemp;
				}
			}
		}
		for(uint32 b=0;b<3;++b)
		{
			if(sp->EffectTriggerSpell[b] != 0 && dbcSpell.LookupEntryForced(sp->EffectTriggerSpell[b]) == nullptr)
			{
				/* proc spell referencing non-existant spell. create a dummy spell for use w/ it. */
				CreateDummySpell(sp->EffectTriggerSpell[b]);
			}
			/** Load teaching spells (used for hunters when learning pets wild abilities) */
			if(sp->Effect[b]==SPELL_EFFECT_LEARN_SPELL && sp->EffectImplicitTargetA[b]==EFF_TARGET_PET)
			{
				map<uint32,uint32>::iterator itr = sWorld.TeachingSpellMap.find(sp->EffectTriggerSpell[b]);
				if(itr == sWorld.TeachingSpellMap.end())
					sWorld.TeachingSpellMap.insert(make_pair(sp->EffectTriggerSpell[b],sp->Id));
			}

			if( sp->Attributes & ATTRIBUTES_ONLY_OUTDOORS && sp->EffectApplyAuraName[b] == SPELL_AURA_MOUNTED )
			{
				sp->Attributes &= ~ATTRIBUTES_ONLY_OUTDOORS;
			}
		}

		/*if(sp->self_cast_only && !(sp->Attributes&64))
			printf("SPELL SELF CAST ONLY: %s %u\n", sp->Name, sp->Id);*/

		if(!strcmp(sp->Name, "Hearthstone") || !strcmp(sp->Name, "Stuck") || !strcmp(sp->Name, "Astral Recall"))
			sp->self_cast_only = true;

		sp->proc_interval = 0;//trigger at each event
		sp->ProcsPerMinute = 0;
		sp->c_is_flags = 0;
		sp->spell_coef_flags = 0;
		sp->Dspell_coef_override = 0;
		sp->OTspell_coef_override = 0;
		sp->casttime_coef = 0;
		sp->AP_coef_override = 0;
		sp->RAP_coef_override = 0;
		sp->auraimmune_flag = 0;

		talentSpellIterator = talentSpells.find(sp->Id);
		if(talentSpellIterator == talentSpells.end())
			sp->talent_tree = 0;
		else
			sp->talent_tree = talentSpellIterator->second;

		skilllinespell *sk = objmgr.GetSpellSkill(sp->Id);
		sp->skilline = sk ? sk->skilline : 0;


		//judgement of command
		if( namehash == SPELL_HASH_JUDGEMENT_OF_COMMAND )		
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;

		else if( namehash == SPELL_HASH_ARCANE_SHOT )		
			sp->c_is_flags |= SPELL_FLAG_IS_NOT_USING_DMG_BONUS;

		//Rogue: Posion time fix for 2.3
		if( strstr( sp->Name, "Crippling Poison") && sp->Effect[0] == 54 ) //I, II
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Mind-numbing Poison") && sp->Effect[0] == 54 ) //I,II,III
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Instant Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V,VI,VII    
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Deadly Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V,VI,VII
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Wound Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Anesthetic Poison") && sp->Effect[0] == 54 ) //I
			sp->EffectBasePoints[0] = 3599;

		if( strstr( sp->Name, "Sharpen Blade") && sp->Effect[0] == 54 ) //All BS stones
		    sp->EffectBasePoints[0] = 3599;

		//these mostly do not mix so we can use else 
		// look for seal, etc in name
        if( (strstr( sp->Name, "Seal")) )
		{
            type |= SPELL_TYPE_SEAL;
			All_Seal_Groups_Combined |= sp->SpellGroupType;
		}
		else if( (strstr( sp->Name, "Blessing")) && sp->Id != 64411)
		    type |= SPELL_TYPE_BLESSING;
		else if( strstr( sp->Name, "Curse"))
		    type |= SPELL_TYPE_CURSE;
		else if( strstr( sp->Name, "Aspect"))
		    type |= SPELL_TYPE_ASPECT;
		else if( strstr( sp->Name, "Sting") || strstr( sp->Name, "sting"))
		    type |= SPELL_TYPE_STING;
		// don't break armor items!
		else if(strcmp(sp->Name, "Armor") && strstr( sp->Name, "Armor") || strstr( sp->Name, "Demon Skin"))
		    type |= SPELL_TYPE_ARMOR;
		else if( strstr( sp->Name, "Aura"))
		    type |= SPELL_TYPE_AURA;
		else if( strstr( sp->Name, "Track")==sp->Name)
		    type |= SPELL_TYPE_TRACK;
		else if( namehash == SPELL_HASH_GIFT_OF_THE_WILD || namehash == SPELL_HASH_MARK_OF_THE_WILD )
		    type |= SPELL_TYPE_MARK_GIFT;
		else if( namehash == SPELL_HASH_IMMOLATION_TRAP || namehash == SPELL_HASH_FREEZING_TRAP || namehash == SPELL_HASH_FROST_TRAP || namehash == SPELL_HASH_EXPLOSIVE_TRAP )
		    type |= SPELL_TYPE_HUNTER_TRAP;
		else if( sp->EffectApplyAuraName[0] == SPELL_AURA_MOD_STAT && sp->EffectMiscValue[0] == 3 )
		    type |= SPELL_TYPE_MAGE_INTEL;
		else if( namehash == SPELL_HASH_AMPLIFY_MAGIC || namehash == SPELL_HASH_DAMPEN_MAGIC )
		    type |= SPELL_TYPE_MAGE_MAGI;
		else if( namehash == SPELL_HASH_FIRE_WARD || namehash == SPELL_HASH_FROST_WARD )
		    type |= SPELL_TYPE_MAGE_WARDS;
		else if( namehash == SPELL_HASH_SHADOW_PROTECTION || namehash == SPELL_HASH_PRAYER_OF_SHADOW_PROTECTION )
		    type |= SPELL_TYPE_PRIEST_SH_PPROT;
		else if( sp->Id == 974 || namehash == SPELL_HASH_LIGHTNING_SHIELD )
		    type |= SPELL_TYPE_SHIELD;
		else if( namehash == SPELL_HASH_POWER_WORD__FORTITUDE || namehash == SPELL_HASH_PRAYER_OF_FORTITUDE )
		    type |= SPELL_TYPE_FORTITUDE;
		else if( namehash == SPELL_HASH_DIVINE_SPIRIT || namehash == SPELL_HASH_PRAYER_OF_SPIRIT )
		    type |= SPELL_TYPE_SPIRIT;
//		else if( strstr( sp->Name, "Curse of Weakness") || strstr( sp->Name, "Curse of Agony") || strstr( sp->Name, "Curse of Recklessness") || strstr( sp->Name, "Curse of Tongues") || strstr( sp->Name, "Curse of the Elements") || strstr( sp->Name, "Curse of Idiocy") || strstr( sp->Name, "Curse of Shadow") || strstr( sp->Name, "Curse of Doom"))
//		else if(namehash==4129426293 || namehash==885131426 || namehash==626036062 || namehash==3551228837 || namehash==2784647472 || namehash==776142553 || namehash==3407058720 || namehash==202747424)
//		else if( strstr( sp->Name, "Curse of "))
//		    type |= SPELL_TYPE_WARLOCK_CURSES;
		else if( strstr( sp->Name, "Immolate") || strstr( sp->Name, "Unstable Affliction" ) )
			type |= SPELL_TYPE_WARLOCK_IMMOLATE;
		else if( strstr( sp->Name, "Amplify Magic") || strstr( sp->Name, "Dampen Magic"))
			type |= SPELL_TYPE_MAGE_AMPL_DUMP;
		else if( strstr( sp->Description, "Battle Elixir"))
		    type |= SPELL_TYPE_ELIXIR_BATTLE;
		else if( strstr( sp->Description, "Guardian Elixir"))
		    type |= SPELL_TYPE_ELIXIR_GUARDIAN;
		else if( strstr( sp->Description, "Battle and Guardian elixir"))
		    type |= SPELL_TYPE_ELIXIR_FLASK;
		else if( namehash == SPELL_HASH_HUNTER_S_MARK )		// hunter's mark
			type |= SPELL_TYPE_HUNTER_MARK;
		else if( namehash == SPELL_HASH_COMMANDING_SHOUT || namehash == SPELL_HASH_BATTLE_SHOUT )
		    type |= SPELL_TYPE_WARRIOR_SHOUT;
		else if( strstr( sp->Description, "Finishing move")==sp->Description)
			sp->c_is_flags |= SPELL_FLAG_IS_FINISHING_MOVE;
		if( IsDamagingSpell( sp ) )
			sp->c_is_flags |= SPELL_FLAG_IS_DAMAGING;
		if( IsHealingSpell( sp ) )
			sp->c_is_flags |= SPELL_FLAG_IS_HEALING;
		if( IsTargetingStealthed( sp ) )
			sp->c_is_flags |= SPELL_FLAG_IS_TARGETINGSTEALTHED;
		
		for(uint32 b = 0; b < 3; ++b)
		{
			if( sp->Effect[b] == SPELL_EFFECT_PERSISTENT_AREA_AURA )
			{
				/*if( sp->EffectImplicitTargetB[b] != 0 )
				printf("%s %s - A: %u, B: %u\n", sp->Name, sp->Rank, sp->EffectImplicitTargetA[b], sp->EffectImplicitTargetB[b]);*/

				sp->EffectImplicitTargetA[b] = EFF_TARGET_SELF;
				sp->EffectImplicitTargetB[b] = 0;
			}

			// 3.0.3 totemzzz
			if( sp->Effect[b] == SPELL_EFFECT_HEALTH_FUNNEL )
			{
				sp->Effect[b] = SPELL_EFFECT_APPLY_AREA_AURA;
			}
		}

		// parse rank text
		if( !sscanf( sp->Rank, "Rank %d", (unsigned int*)&sp->RankNumber) )
		{
			const char* ranktext = sp->Rank;
			uint32 rank = 0;

			//stupid spell ranking problem
			if( strstr( ranktext, "Apprentice"))
				rank = 1;
			else if( strstr( ranktext, "Journeyman"))
				rank = 2;
			else if( strstr( ranktext, "Expert"))
				rank = 3;
			else if( strstr( ranktext, "Artisan"))
				rank = 4;
			else if( strstr( ranktext, "Master"))
				rank = 5;
			else if( strstr( ranktext, "Grandmaster"))
				rank = 6;
			sp->RankNumber = rank;
		}

		if(sp->spellLevel==0)
		{
			uint32 new_level=sp->RankNumber;
			if(new_level!=0)
			{
				uint32 teachspell=0;
				if(sp->Effect[0]==SPELL_EFFECT_LEARN_SPELL)
					teachspell = sp->EffectTriggerSpell[0];
				else if(sp->Effect[1]==SPELL_EFFECT_LEARN_SPELL)
					teachspell = sp->EffectTriggerSpell[1];
				else if(sp->Effect[2]==SPELL_EFFECT_LEARN_SPELL)
					teachspell = sp->EffectTriggerSpell[2];
				if(teachspell)
				{
					SpellEntry *spellInfo;
					spellInfo = dbcSpell.LookupEntryForced(teachspell);
					spellInfo->spellLevel = new_level;
					sp->spellLevel = new_level;
				}
			}
		}

		/*FILE * f = fopen("C:\\spells.txt", "a");
		fprintf(f, "case 0x%08X:		// %s\n", namehash, sp->Name);
		fclose(f);*/

		// find diminishing status
		sp->DiminishStatus = GetDiminishingGroup(namehash);
		sp->buffIndexType=0;
		switch(namehash)
		{
		case SPELL_HASH_HUNTER_S_MARK:		// Hunter's mark
			sp->buffIndexType = SPELL_TYPE_INDEX_MARK;
			break;

		case SPELL_HASH_POLYMORPH:			// Polymorph
		case SPELL_HASH_POLYMORPH__CHICKEN:	// Polymorph: Chicken
		case SPELL_HASH_POLYMORPH__SHEEP:	// Polymorph: Sheep
			sp->buffIndexType = SPELL_TYPE_INDEX_POLYMORPH;
			break;

		case SPELL_HASH_FEAR:				// Fear
			sp->buffIndexType = SPELL_TYPE_INDEX_FEAR;
			break;

		case SPELL_HASH_SAP:				// Sap
			sp->buffIndexType = SPELL_TYPE_INDEX_SAP;
			break;

		case SPELL_HASH_SCARE_BEAST:		// Scare Beast
			sp->buffIndexType = SPELL_TYPE_INDEX_SCARE_BEAST;
			break;

		case SPELL_HASH_HIBERNATE:			// Hibernate
			sp->buffIndexType = SPELL_TYPE_INDEX_HIBERNATE;
			break;

		case SPELL_HASH_CYCLONE:			// Cyclone
			sp->buffIndexType = SPELL_TYPE_INDEX_CYCLONE;
			break;

		case SPELL_HASH_BANISH:				// Banish
			sp->buffIndexType = SPELL_TYPE_INDEX_BANISH;
			break;

		//case SPELL_HASH_JUDGEMENT_OF_VENGEANCE:
		case SPELL_HASH_JUDGEMENT_OF_LIGHT:
		case SPELL_HASH_JUDGEMENT_OF_WISDOM:
		case SPELL_HASH_JUDGEMENT_OF_JUSTICE:
			sp->buffIndexType = SPELL_TYPE_INDEX_JUDGEMENT;
			break;
		}

		// HACK FIX: Break roots/fear on damage.. this needs to be fixed properly!
		uint32 z;
		if(!(sp->AuraInterruptFlags & AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN))
		{
			for(z = 0; z < 3; ++z) {
				if(sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_FEAR ||
					sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_ROOT)
				{
					sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_UNUSED2;
					break;
				}
			}
		}

		for(z = 0;z < 3; ++z)
		{
			if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->Spell_Dmg_Type == SPELL_DMG_TYPE_MELEE ) || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE || sp->Effect[z] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE || sp->Effect[z] == SPELL_EFFECT_DUMMYMELEE )
				sp->is_melee_spell = true;
			if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->Spell_Dmg_Type == SPELL_DMG_TYPE_RANGED ) )
			{
				//sLog.outString( "Ranged Spell: %u [%s]" , sp->Id , sp->Name );
				sp->is_ranged_spell = true;
			}

			switch(sp->EffectImplicitTargetA[z])
			{
				case 6:
				case 22:
				case 24:
				case 25:
					sp->c_is_flags |= SPELL_FLAG_CAN_BE_REFLECTED;
					break;
			}

			if( sp->Effect[z] == SPELL_EFFECT_DISPEL )
				sp->c_is_flags |= SPELL_FLAG_IS_DISPEL_SPELL;

			if( sp->Effect[z] == SPELL_EFFECT_HEAL )
				sp->c_is_flags |= SPELL_FLAG_IS_HEALING_SPELL;

			if( sp->Effect[z] == SPELL_EFFECT_ENERGIZE )
				sp->c_is_flags |= SPELL_FLAG_IS_HEALING_MANA_SPELL;

		}

		// set extra properties
		sp->buffType   = type;

		uint32 pr=sp->procFlags;
		for(uint32 y=0;y < 3; y++)
		{
			// get the effect number from the spell
			effect = sp->Effect[y];

			if(effect==SPELL_EFFECT_APPLY_AURA)
			{
				uint32 aura = sp->EffectApplyAuraName[y];
				if( aura == SPELL_AURA_PROC_TRIGGER_SPELL ||
					aura == SPELL_AURA_PROC_TRIGGER_DAMAGE
					)//search for spellid in description
				{
					const char *p=sp->Description;
					while((p=strstr(p,"$")))
					{
						p++;
						//got $  -> check if spell
						if(*p>='0' && *p <='9')
						{//woot this is spell id
						
							result=atoi(p);
						}					
					}
					pr=0;

					char desc[4096];
					strcpy(desc,sp->Description);
					uint32 len = (uint32)strlen(desc);
					for(i = 0; i < len; ++i)
						desc[i] = tolower(desc[i]);
					//dirty code for procs, if any1 got any better idea-> u are welcome
					//139944 --- some magic number, it will trigger on all hits etc
						//for seems to be smth like custom check
					if( strstr( desc,"your ranged criticals"))
						pr|=PROC_ON_RANGED_CRIT;
					if( strstr( desc,"chance on hit"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"takes damage"))
						pr|=PROC_ON_TAKE_DAMAGE;
					if( strstr( desc,"attackers when hit"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"character strikes an enemy"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"strike you with a melee attack"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"target casts a spell"))
						pr|=PROC_ON_CAST_SPELL;
				    if( strstr( desc,"your harmful spells land"))
						pr|=PROC_ON_CAST_SPELL;
				    if( strstr( desc,"on spell critical hit"))
						pr|=PROC_ON_SPELL_CRIT;
				    if( strstr( desc,"spell critical strikes"))
						pr|=PROC_ON_SPELL_CRIT;
				    if( strstr( desc,"being able to resurrect"))
						pr|=CPROC_ON_DIE;
					if( strstr( desc,"any damage caused"))
						pr|=PROC_ON_TAKE_DAMAGE;
					if( strstr( desc,"the next melee attack against the caster"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"when successfully hit"))
						pr|=PROC_ON_MELEE_HIT ;
					if( strstr( desc,"an enemy on hit"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"when it hits"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"when successfully hit"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"on a successful hit"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"damage to attacker on hit"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"on a hit"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"strikes you with a melee attack"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"when caster takes damage"))
						pr|=PROC_ON_TAKE_DAMAGE;
					if( strstr( desc,"when the caster is using melee attacks"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"when struck in combat") || strstr(desc,"When struck in combat"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"successful melee attack"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"chance per attack"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"chance per hit"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"that strikes a party member"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"when hit by a melee attack"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"landing a melee critical strike"))
						pr|=PROC_ON_MELEE_CRIT;
					if( strstr( desc,"your critical strikes"))
						pr|=PROC_ON_MELEE_CRIT;
					if( strstr( desc,"whenever you deal ranged damage"))
						pr|=PROC_ON_RANGED_HIT;
//					if( strstr( desc,"whenever you deal melee damage"))
					if( strstr( desc,"you deal melee damage"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"your melee attacks"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"damage with your Sword"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"when struck in melee combat"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"any successful spell cast against the priest"))
						pr|=PROC_TARGET_SPELL_HIT;
					if( strstr( desc,"the next melee attack on the caster"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"striking melee or ranged attackers"))
						pr|=PROC_TARGET_MELEE_HIT|PROC_TARGET_RANGED_HIT;
					if( strstr( desc,"when damaging an enemy in melee"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"victim of a critical strike"))
						pr|=PROC_TARGET_MELEE_CRIT;
					if( strstr( desc,"on successful melee or ranged attack"))
						pr|=PROC_ON_MELEE_HIT|PROC_ON_RANGED_HIT;
					if( strstr( desc,"enemy that strikes you in melee"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"after getting a critical strike"))
						pr|=PROC_ON_MELEE_CRIT;
					if( strstr( desc,"whenever damage is dealt to you"))
						pr|=PROC_ON_TAKE_DAMAGE;
					if( strstr( desc,"when ranged or melee damage is dealt"))
						pr|=PROC_ON_MELEE_HIT|PROC_ON_RANGED_HIT;
					if( strstr( desc,"damaging melee attacks"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"on melee or ranged attack"))
						pr|=PROC_ON_MELEE_HIT|PROC_ON_RANGED_HIT;
					if( strstr( desc,"on a melee swing"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"Chance on melee"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"spell criticals against you"))
						pr|=PROC_TARGET_SPELL_CRIT;
					if( strstr( desc,"after being struck by a melee or ranged critical hit"))
						pr|=PROC_TARGET_MELEE_CRIT;
//					if( strstr( desc,"on a critical hit"))
					if( strstr( desc,"critical hit"))
						pr|=PROC_ON_MELEE_CRIT;
					if( strstr( desc,"strikes the caster"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"a spell, melee or ranged attack hits the caster"))
						pr|=PROC_ON_TAKE_DAMAGE;
					if( strstr( desc,"after dealing a critical strike"))
						pr|=PROC_ON_MELEE_CRIT;
					if( strstr( desc,"each melee or ranged damage hit against the priest"))
						pr|=PROC_TARGET_MELEE_HIT|PROC_TARGET_RANGED_HIT;				
					if( strstr( desc, "a chance to deal additional"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc, "chance to get an extra attack"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc, "melee attacks has"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc, "any damage spell hits a target"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc, "giving each melee attack a chance"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc, "damage when hit"))
						pr|=PROC_ON_TAKE_DAMAGE; //myabe melee damage ?
					if( strstr( desc, "gives your"))
					{
						if( strstr( desc, "melee"))
							pr|=PROC_ON_MELEE_HIT;
						else if( strstr( desc,"sinister strike, backstab, gouge and shiv"))
							pr|=PROC_ON_CAST_SPELL;
						else if( strstr( desc,"chance to daze the target"))
							pr|=PROC_ON_CAST_SPELL;
						else if( strstr( desc,"finishing moves"))
							pr|=PROC_ON_CAST_SPELL;
//						else if( strstr( desc,"shadow bolt, shadowburn, soul fire, incinerate, searing pain and conflagrate"))
//							pr|=PROC_ON_CAST_SPELL|CPROC_ON_SELF;
						//we should find that specific spell (or group) on what we will trigger
						else pr|=CPROC_CAST_SPECIFIC;
					}
					if( strstr( desc, "chance to add an additional combo") && strstr(desc, "critical") )
						pr|=PROC_ON_MELEE_CRIT;
					else if( strstr( desc, "chance to add an additional combo"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc, "victim of a melee or ranged critical strike"))
						pr|=PROC_TARGET_MELEE_CRIT;
					if( strstr( desc, "getting a critical effect from"))
						pr|=PROC_TARGET_SPELL_CRIT;
					if( strstr( desc, "damaging attack is taken"))
						pr|=PROC_ON_TAKE_DAMAGE;
					if( strstr( desc, "struck by a Stun or Immobilize"))
						pr|=PROC_TARGET_SPELL_HIT;
					if( strstr( desc, "melee critical strike"))
						pr|=PROC_ON_MELEE_CRIT;
					if( strstr( sp->Name, "Bloodthirst"))
						pr|=PROC_ON_MELEE_HIT | CPROC_ON_SELF;
					if( strstr( desc, "experience or honor"))
						pr|=PROC_ON_KILL;
					if( strstr( desc,"your next offensive ability"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"hit by a melee or ranged attack"))
						pr|=PROC_TARGET_MELEE_HIT | PROC_TARGET_RANGED_HIT;
					if( strstr( desc,"enemy strikes the caster"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"melee and ranged attacks against you"))
						pr|=PROC_TARGET_MELEE_HIT | PROC_TARGET_RANGED_HIT;
					if( strstr( desc,"when a block occurs"))
						pr|=PROC_ON_BLOCK;
					if( strstr( desc,"dealing a critical strike from a weapon swing, spell, or ability"))
						pr|=PROC_ON_MELEE_CRIT|PROC_ON_SPELL_CRIT;
					if( strstr( desc,"dealing a critical strike from a weapon swing, spell, or ability"))
						pr|=PROC_ON_MELEE_CRIT|PROC_ON_SPELL_CRIT;
					if( strstr( desc,"shadow bolt critical strikes increase shadow damage"))
						pr|=PROC_ON_SPELL_CRIT;
					if( strstr( desc,"next offensive ability"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"after being hit with a shadow or fire spell"))
						pr|=PROC_ON_SPELL_HIT;
					if( strstr( desc,"giving each melee attack"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"each strike has"))
						pr|=PROC_ON_MELEE_HIT;		
					if( strstr( desc,"your Fire damage spell hits"))
						pr|=PROC_ON_CAST_SPELL;		//this happens only on hit ;)
					if( strstr( desc,"corruption, curse of agony, siphon life and seed of corruption spells also cause"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"pain, mind flay and vampiric touch spells also cause"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"shadow damage spells have"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"on successful spellcast"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"your spell criticals have"))
						pr|=PROC_ON_SPELL_CRIT | PROC_TARGET_SPELL_CRIT;
					if( strstr( desc,"after dodging their attack"))
					{
						pr|=PROC_ON_DODGE;
						if( strstr( desc,"add a combo point"))
							pr|=CPROC_ON_SELF;
					}
					if( strstr( desc,"fully resisting"))
						pr|=PROC_ON_RESIST_SPELL;
					if( strstr( desc,"Your Shadow Word: Pain, Mind Flay and Vampiric Touch spells also cause the target"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"chance on spell hit"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"your spell casts have"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"chance on spell cast"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"your melee and ranged attacks"))
						pr|=PROC_ON_MELEE_HIT|PROC_ON_RANGED_HIT;
//					if( strstr( desc,"chill effect to your Blizzard"))
//						pr|=PROC_ON_CAST_SPELL;	
					//////////////////////////////////////////////////
					//proc dmg flags
					//////////////////////////////////////////////////
					if( strstr( desc,"each attack blocked"))
						pr|=PROC_ON_BLOCK;
					if( strstr( desc,"into flame, causing an additional"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"victim of a critical melee strike"))
						pr|=PROC_TARGET_MELEE_CRIT;
					if( strstr( desc,"damage to melee attackers"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"target blocks a melee attack"))
						pr|=PROC_ON_BLOCK;
					if( strstr( desc,"ranged and melee attacks to deal"))
						pr|=PROC_TARGET_MELEE_HIT | PROC_TARGET_RANGED_HIT;
					if( strstr( desc,"damage on hit"))
						pr|=PROC_ON_TAKE_DAMAGE;
					if( strstr( desc,"chance on hit"))
						pr|=PROC_ON_MELEE_HIT;
					if( strstr( desc,"after being hit by any damaging attack"))
						pr|=PROC_ON_TAKE_DAMAGE;
					if( strstr( desc,"striking melee or ranged attackers"))
						pr|=PROC_TARGET_MELEE_HIT | PROC_TARGET_RANGED_HIT;
					if( strstr( desc,"damage to attackers when hit"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"striking melee attackers"))
						pr|=PROC_TARGET_MELEE_HIT;
					if( strstr( desc,"whenever the caster takes damage"))
						pr|=PROC_ON_TAKE_DAMAGE;
					if( strstr( desc,"damage on every attack"))
						pr|=PROC_ON_MELEE_HIT | PROC_ON_RANGED_HIT;
					if( strstr( desc,"chance to reflect Fire spells"))
						pr|=PROC_TARGET_SPELL_HIT;
					if( strstr( desc,"hunter takes on the aspects of a hawk"))
						pr|=CPROC_ON_SELF | PROC_ON_RANGED_HIT;
					if( strstr( desc,"successful auto shot attacks"))
						pr|=CPROC_AUTO_SHOT; // Can't this just used Ranged hit?
					if( strstr( desc,"after getting a critical effect from your"))
						pr=PROC_ON_SPELL_CRIT;
					if( strstr( desc, "with a melee weapon"))
						pr|=PROC_ON_MELEE_HIT;
//					if( strstr( desc,"Your critical strikes from Fire damage"))
//						pr|=PROC_ON_SPELL_CRIT;
				}//end "if procspellaura"
				//dirty fix to remove auras that should expire on event and they are not
//				else if(sp->procCharges>0)
//				{
					//there are at least 185 spells that should loose charge uppon some event.Be prepared to add more here !
					// ! watch it cause this might conflict with our custom modified spells like : lighning shield !

					//spells like : Presence of Mind,Nature's Swiftness, Inner Focus,Amplify Curse,Coup de Grace
					//SELECT * FROM dbc_spell where proc_charges!=0 and (effect_aura_1=108 or effect_aura_2=108 and effect_aura_3=108) and description!=""
//					if(aura == SPELL_AURA_ADD_PCT_MODIFIER)
//						sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_CAST_SPELL;
					//most of them probably already have these flags...not sure if we should add to all of them without checking
/*					if( strstr( desc, "melee"))
						sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_START_ATTACK;
					if( strstr( desc, "ranged"))
						sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_START_ATTACK;*/
//				}

				// Aura 109 fix
				if(sp->EffectApplyAuraName[y] == SPELL_AURA_ADD_TARGET_TRIGGER)
				{
					sp->EffectApplyAuraName[y] = SPELL_AURA_PROC_TRIGGER_SPELL;
					pr = PROC_ON_CAST_SPELL;
				}
			}//end "if aura"
		}//end "for each effect"
		sp->procFlags = pr;

		if( strstr( sp->Description, "Must remain seated"))
		{
			sp->RecoveryTime = 1000;
			sp->CategoryRecoveryTime = 1000;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// procintervals
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//omg lighning shield trigger spell id's are all wrong ?
		//if you are bored you could make thiese by hand but i guess we might find other spells with this problem..and this way it's safe
		if( strstr( sp->Name, "Lightning Shield" ) && sp->EffectTriggerSpell[0] )
		{
			//check if we can find in the desription
			char *startofid = strstr(sp->Description, "for $");
			if( startofid )
			{
				startofid += strlen("for $");
				sp->EffectTriggerSpell[0] = atoi( startofid ); //get new lightning shield trigger id

				SpellEntry * triggerSpell = dbcSpell.LookupEntryForced( sp->EffectTriggerSpell[0] );
				if(triggerSpell)
					triggerSpell->Dspell_coef_override = 0.3f;
			}
			sp->proc_interval = 3000; //few seconds
		}
		//mage ignite talent should proc only on some chances
		else if( strstr( sp->Name, "Ignite") && sp->Id>=11119 && sp->Id<=12848 && sp->EffectApplyAuraName[0] == 4 )
		{
			//check if we can find in the desription
			char *startofid=strstr(sp->Description, "an additional ");
			if(startofid)
			{
				startofid += strlen("an additional ");
				sp->EffectBasePoints[0]=atoi(startofid); //get new value. This is actually level*8 ;)
			}
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = 42; //force him to use procspell effect
			sp->EffectTriggerSpell[0] = 12654; //evil , but this is good for us :D
			sp->procFlags = PROC_ON_SPELL_CRIT; //add procflag here since this was not processed with the others !
		}
		// Winter's Chill handled by frost school
		else if( strstr( sp->Name, "Winter's Chill"))
		{
			sp->School = 4;
		}
		// Blackout handled by Shadow school
		else if( strstr( sp->Name, "Blackout"))
		{
			sp->School = 5;
		}
#ifndef NEW_PROCFLAGS
		// Shadow Weaving
		else if( strstr( sp->Name, "Shadow Weaving"))
		{
			sp->School = 5;
			sp->EffectApplyAuraName[0] = 42;
			sp->procChance = sp->EffectBasePoints[0] + 1;
			sp->procFlags = CPROC_CAST_SPECIFIC;
		}
#endif
		//this starts to be an issue for trigger spell id : Deep Wounds
		else if( strstr( sp->Name, "Deep Wounds") && sp->EffectTriggerSpell[0])
		{
			//check if we can find in the desription
			char *startofid=strstr(sp->Description, "over $");
			if(startofid)
			{
				startofid += strlen("over $");
				sp->EffectTriggerSpell[0] = atoi(startofid);
			}
		}
		else if( strstr( sp->Name, "Holy Shock"))
		{
			//check if we can find in the desription
			char *startofid=strstr(sp->Description, "causing $");
			if(startofid)
			{
				startofid += strlen("causing $");
				sp->EffectTriggerSpell[0] = atoi(startofid);
			}
			//check if we can find in the desription
			startofid=strstr(sp->Description, " or $");
			if(startofid)
			{
				startofid += strlen(" or $");
				sp->EffectTriggerSpell[1]=atoi(startofid);
			}
		}
		else if( strstr( sp->Name, "Touch of Weakness"))
		{
			//check if we can find in the desription
			char *startofid=strstr(sp->Description, "cause $");
			if(startofid)
			{
				startofid += strlen("cause $");
				sp->EffectTriggerSpell[0] = atoi(startofid);
				sp->EffectTriggerSpell[1]=sp->EffectTriggerSpell[0]; //later versions of this spell changed to eff[1] the aura
				sp->procFlags = uint32(PROC_TARGET_MELEE_HIT);
			}
		}
		else if( strstr( sp->Name, "Firestone Passive"))
		{
			//Enchants the main hand weapon with fire, granting each attack a chance to deal $17809s1 additional fire damage.
			//check if we can find in the desription
			char * startofid=strstr(sp->Description, "to deal $");
			if(startofid)
			{
				startofid += strlen("to deal $");
				sp->EffectTriggerSpell[0] = atoi(startofid);
				sp->EffectApplyAuraName[0] = 42;
				sp->procFlags = PROC_ON_MELEE_HIT;
				sp->procChance = 50;
			}
		}

		if(sp->Id == 974)
			sp->proc_interval = 3000; //few seconds
		else if( strstr( sp->Name, "Shadowguard"))
			sp->proc_interval = 3000; //few seconds
		else if( strstr( sp->Name, "Poison Shield"))
			sp->proc_interval = 3000; //few seconds
		else if( strstr( sp->Name, "Infused Mushroom"))
			sp->proc_interval = 10000; //10 seconds
		else if( strstr( sp->Name, "Aviana's Purpose"))
			sp->proc_interval = 10000; //10 seconds
 		else if( strstr( sp->Name, "Crippling Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
			sp->poison_type = POISON_TYPE_CRIPPLING;
		}
		else if( strstr( sp->Name, "Mind-Numbing Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
			sp->poison_type = POISON_TYPE_MIND_NUMBING;
		}
		else if( strstr( sp->Name, "Instant Poison"))
		{
			sp->poison_type = POISON_TYPE_INSTANT;
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}
		else if( strstr( sp->Name, "Scorpid Poison") )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
			sp->poison_type = POISON_TYPE_SCORPID;
		}
		else if( strstr( sp->Name, "Deadly Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
			sp->poison_type = POISON_TYPE_DEADLY;
		}
		else if( strstr( sp->Name, "Wound Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
			sp->poison_type = POISON_TYPE_WOUND;
		}

		if( strstr( sp->Description, "cannot be resisted" ) )
			sp->c_is_flags |= SPELL_FLAG_IS_NOT_RESISTABLE;

		if( strstr( sp->Description, "pierces through all absorption effects" ) )
			sp->c_is_flags |= SPELL_FLAG_PIERCES_ABSORBTION_EFF;

		//Set Silencing spells mech.
				// Set default mechanics if we don't already have one
		if( !sp->MechanicsType )
		{
		    //Set Silencing spells mechanic.
		    if( sp->EffectApplyAuraName[0] == 27 ||
				sp->EffectApplyAuraName[1] == 27 ||
				sp->EffectApplyAuraName[2] == 27 )
				sp->MechanicsType = MECHANIC_SILENCED;

		    //Set Stunning spells mechanic.
		    if( sp->EffectApplyAuraName[0] == 12 ||
				sp->EffectApplyAuraName[1] == 12 ||
				sp->EffectApplyAuraName[2] == 12 )
				sp->MechanicsType = MECHANIC_STUNNED;

		    //Set Fearing spells mechanic
		    if( sp->EffectApplyAuraName[0] == 7 ||
				sp->EffectApplyAuraName[1] == 7 ||
				sp->EffectApplyAuraName[2] == 7 )
				sp->MechanicsType = MECHANIC_FLEEING;
		}

		// Sap, Gouge, Blehhhh
		if( sp->NameHash == SPELL_HASH_GOUGE ||
			sp->NameHash == SPELL_HASH_SAP ||
			sp->NameHash == SPELL_HASH_REPENTANCE ||
			sp->NameHash == SPELL_HASH_GOBLIN_ROCKET_HELMET ||
			sp->NameHash == SPELL_HASH_RECKLESS_CHARGE)
			sp->MechanicsType = MECHANIC_INCAPACIPATED;

		if( sp->proc_interval != 0 )
			sp->procFlags |= CPROC_REMOVEONUSE;

		// Seal of Command - Proc Chance
		if( sp->NameHash == SPELL_HASH_SEAL_OF_COMMAND )
		{
			sp->procChance = 100;
			sp->procFlags = PROC_ON_MELEE_HIT;
			sp->School = SCHOOL_HOLY; //the procspells of the original seal of command have physical school instead of holy
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC; //heh, crazy spell uses melee/ranged/magic dmg type for 1 spell. Now which one is correct ?
		}

		//Seal of Jusice - Proc Chance
		if( sp->NameHash == SPELL_HASH_SEAL_OF_JUSTICE )
			sp->procChance = 25;

		/* Decapitate */
		if( sp->NameHash == SPELL_HASH_DECAPITATE )
			sp->procChance = 30;

		if( sp->NameHash == SPELL_HASH_DIVINE_SHIELD || sp->NameHash == SPELL_HASH_DIVINE_PROTECTION || sp->NameHash == SPELL_HASH_BLESSING_OF_PROTECTION )
			sp->MechanicsType = 25;

		if(sp->NameHash == SPELL_HASH_DRINK && sp->EffectBasePoints[0] == -1 &&
			sp->EffectApplyAuraName[1] == 226 && sp->EffectBasePoints[1] > 0)
		{
			sp->EffectBasePoints[0] = sp->EffectBasePoints[1];
			sp->Effect[1] = SPELL_EFFECT_NULL;
		}

		/* hackfix for this - FIX ME LATER - Burlex */
		if( namehash == SPELL_HASH_SEAL_FATE )
			sp->procFlags = 0;

		if(
			((sp->Attributes & ATTRIBUTES_TRIGGER_COOLDOWN) && (sp->AttributesEx & ATTRIBUTESEX_NOT_BREAK_STEALTH)) //rogue cold blood
			|| ((sp->Attributes & ATTRIBUTES_TRIGGER_COOLDOWN) && (!sp->AttributesEx || sp->AttributesEx & ATTRIBUTESEX_REMAIN_OOC))
			)
		{
			sp->c_is_flags |= SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
		}

		/////////////////////////////////////////////////////////////////
		//SPELL COEFFICIENT SETTINGS START
		//////////////////////////////////////////////////////////////////

		//Setting Cast Time Coefficient
		SpellCastTime *sd = dbcSpellCastTime.LookupEntry(sp->CastingTimeIndex);
		float castaff = float(GetCastTime(sd));
		if(castaff < 1500) castaff = 1500;
		else
			if(castaff > 7000) castaff = 7000;

		sp->casttime_coef = castaff / 3500;		 

		SpellEntry * spz;
		bool spcheck = false;

		//Flag for DoT and HoT
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if (sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_DAMAGE ||
				sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_HEAL ||
				sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_LEECH )
			{
				sp->spell_coef_flags |= SPELL_FLAG_IS_DOT_OR_HOT_SPELL;
				break;
			}
		}

		//Flag for DD or DH
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if ( sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_TRIGGER_SPELL && sp->EffectTriggerSpell[i] )
			{
				spz = dbcSpell.LookupEntry( sp->EffectTriggerSpell[i] );
				if( spz &&
					spz->Effect[i] == SPELL_EFFECT_SCHOOL_DAMAGE ||
					spz->Effect[i] == SPELL_EFFECT_HEAL
					) 
					spcheck = true;
			}
			if (sp->Effect[i] == SPELL_EFFECT_SCHOOL_DAMAGE ||
				sp->Effect[i] == SPELL_EFFECT_HEAL ||
				spcheck
				)
			{
				sp->spell_coef_flags |= SPELL_FLAG_IS_DD_OR_DH_SPELL;
				break;
			}
		}

		for(uint8 i = 0 ; i < 3; i++)
		{
			switch (sp->EffectImplicitTargetA[i])
			{
				//AoE
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS:
			case EFF_TARGET_ALL_ENEMY_IN_AREA:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT:
			case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
			case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
			case EFF_TARGET_IN_FRONT_OF_CASTER:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_FRIENDLY_IN_AREA:
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME:
			case EFF_TARGET_ALL_PARTY:
			case EFF_TARGET_LOCATION_INFRONT_CASTER:
			case EFF_TARGET_BEHIND_TARGET_LOCATION:
			case EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE:
				{
					sp->spell_coef_flags |= SPELL_FLAG_AOE_SPELL;
					break;
				}
			}	
		}

		for(uint8 i = 0 ; i < 3 ; i++)
		{
			switch (sp->EffectImplicitTargetB[i])
			{
				//AoE
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS:
			case EFF_TARGET_ALL_ENEMY_IN_AREA:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT:
			case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
			case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
			case EFF_TARGET_IN_FRONT_OF_CASTER:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_FRIENDLY_IN_AREA:
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME:
			case EFF_TARGET_ALL_PARTY:
			case EFF_TARGET_LOCATION_INFRONT_CASTER:
			case EFF_TARGET_BEHIND_TARGET_LOCATION:
			case EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE:
				{
					sp->spell_coef_flags |= SPELL_FLAG_AOE_SPELL;
					break;
				}
			}	
			if(sp->Effect[i] == 0)
				continue;

			//Apply aura fixes
			if( sp->Effect[i] == SPELL_EFFECT_APPLY_AURA )
			{
				switch( sp->EffectApplyAuraName[i] )
				{
					case SPELL_AURA_MOD_CONFUSE: 
						sp->auraimmune_flag |= AURAIMMUNE_CONFUSE;break;
					case SPELL_AURA_MOD_CHARM: 
						sp->auraimmune_flag |= AURAIMMUNE_CHARM;break;
					case SPELL_AURA_MOD_FEAR: 
						sp->auraimmune_flag |= AURAIMMUNE_FEAR;break;
					case SPELL_AURA_MOD_STUN: 
						sp->auraimmune_flag |= AURAIMMUNE_STUN;break;
					case SPELL_AURA_MOD_PACIFY: 
						sp->auraimmune_flag |= AURAIMMUNE_PACIFY;break;
					case SPELL_AURA_MOD_ROOT: 
						sp->auraimmune_flag |= AURAIMMUNE_ROOT;break;
					case SPELL_AURA_MOD_SILENCE: 
						sp->auraimmune_flag |= AURAIMMUNE_SILENCE;break;
					case SPELL_AURA_MOD_INCREASE_SPEED: 
						sp->auraimmune_flag |= AURAIMMUNE_INCSPEED;break;
					case SPELL_AURA_MOD_DECREASE_SPEED: 
						sp->auraimmune_flag |= AURAIMMUNE_DECSPEED;break;
					case SPELL_AURA_TRANSFORM: 
						sp->auraimmune_flag |= AURAIMMUNE_TRANSFORM;break;
					case SPELL_AURA_MOD_TAUNT: 
						sp->auraimmune_flag |= AURAIMMUNE_TAUNT;break;
				}
			}
		}

		//Special Cases
		//Holy Light & Flash of Light
		if(sp->NameHash == SPELL_HASH_HOLY_LIGHT ||
			sp->NameHash == SPELL_HASH_FLASH_OF_LIGHT)
			sp->spell_coef_flags |= SPELL_FLAG_IS_DD_OR_DH_SPELL;

		if(sp->NameHash == SPELL_HASH_FIREBALL)
			sp->spell_coef_flags = SPELL_FLAG_IS_DD_OR_DH_SPELL;

		//Additional Effect (not healing or damaging)
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if(sp->Effect[i] == 0)
				continue;

			switch (sp->Effect[i])
			{
			case SPELL_EFFECT_SCHOOL_DAMAGE:
			case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE:
			case SPELL_EFFECT_HEALTH_LEECH:
			case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
			case SPELL_EFFECT_ADD_EXTRA_ATTACKS:
			case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
			case SPELL_EFFECT_POWER_BURN:
			case SPELL_EFFECT_ATTACK:
			case SPELL_EFFECT_HEAL:
			case SPELL_EFFECT_HEALTH_FUNNEL:
			case SPELL_EFFECT_HEAL_MAX_HEALTH:
			case SPELL_EFFECT_DUMMY:
				continue;
			}

			switch (sp->EffectApplyAuraName[i])
			{
			case SPELL_AURA_PERIODIC_DAMAGE:
			case SPELL_AURA_PROC_TRIGGER_DAMAGE:
			case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
			case SPELL_AURA_POWER_BURN:
			case SPELL_AURA_PERIODIC_HEAL:
			case SPELL_AURA_MOD_INCREASE_HEALTH:
			case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
			case SPELL_AURA_DUMMY:
				continue;
			}

			sp->spell_coef_flags |= SPELL_FLAG_ADITIONAL_EFFECT;
			break;

		}

		///	SPELLS CAN CRIT ///
		sp->spell_can_crit = true; // - except in special cases noted in this section

		//////////////////////////////////////////////////////
		// CLASS-SPECIFIC SPELL FIXES						//
		//////////////////////////////////////////////////////

		/* Note: when applying spell hackfixes, please follow a template */

		//////////////////////////////////////////
		// WARRIOR								//
		//////////////////////////////////////////

		// Insert warrior spell fixes here
		if( sp->NameHash == SPELL_HASH_MORTAL_STRIKE)
			sp->maxstack = 1; // Healing reduction shouldn't stack
			
		
		// Shockwave
		if( sp->NameHash == SPELL_HASH_SHOCKWAVE)
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MELEE;

		//////////////////////////////////////////
		// PALADIN								//
		//////////////////////////////////////////

		// Insert paladin spell fixes here
			
		// Seal of Righteousness - cannot crit
		if( sp->NameHash == SPELL_HASH_SEAL_OF_RIGHTEOUSNESS )
			sp->spell_can_crit = false;

		//Seal of Light
		if( sp->NameHash == SPELL_HASH_SEAL_OF_LIGHT)
			sp->procChance = 45;	/* this will do */
			
		if( sp->NameHash == SPELL_HASH_BLESSING_OF_SANCTUARY || sp->NameHash == SPELL_HASH_GREATER_BLESSING_OF_SANCTUARY )
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
			sp->EffectBasePoints[0] = 3;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE;
			sp->EffectBasePoints[1] = 10;
			sp->EffectMiscValue[1] = 4; // stamina
			sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
		}
					
					
		// Benediction
		sp = dbcSpell.LookupEntryForced( 20101 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = All_Seal_Groups_Combined | 8388608;
		sp = dbcSpell.LookupEntryForced( 20102 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = All_Seal_Groups_Combined | 8388608;
		sp = dbcSpell.LookupEntryForced( 20103 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = All_Seal_Groups_Combined | 8388608;
		sp = dbcSpell.LookupEntryForced( 20104 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = All_Seal_Groups_Combined | 8388608;
		sp = dbcSpell.LookupEntryForced( 20105 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = All_Seal_Groups_Combined | 8388608;
		//////////////////////////////////////////
		// HUNTER								//
		//////////////////////////////////////////

		// Insert hunter spell fixes here

		// come to think of it... anything *castable* requiring a ranged weapon is a ranged spell -.-
		// Note that talents etc also come under this, however it does not matter
		// if they get flagged as ranged spells because is_ranged_spell is only used for
		// differentiating between resistable and physically avoidable spells.
		if( sp->EquippedItemClass == 2 && sp->EquippedItemSubClass & 262156 ) // 4 + 8 + 262144 ( becomes item classes 2, 3 and 18 which correspond to bow, gun and crossbow respectively)
			sp->is_ranged_spell = true;

		if( sp->NameHash == SPELL_HASH_AIMED_SHOT)
			sp->maxstack = 1; // Healing reduction shouldn't stack

		if( sp->NameHash == SPELL_HASH_EXPLOSIVE_SHOT )
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;

		//////////////////////////////////////////
		// ROGUE								//
		//////////////////////////////////////////

		// Insert rogue spell fixes here
		if( sp->NameHash == SPELL_HASH_REMORSELESS_ATTACKS )
			sp->procFlags = PROC_ON_KILL;

		//////////////////////////////////////////
		// PRIEST								//
		//////////////////////////////////////////
						
		//////////////////////////////////////////
		// SHAMAN								//
		//////////////////////////////////////////

		/**********************************************************
		 *	Elemental Focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16164 );
		if( sp != nullptr && sp->Id == 16164 )
			sp->procFlags = PROC_ON_SPELL_HIT | PROC_ON_SPELL_CRIT;

		/**********************************************************
		 *	Bloodlust
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 2825 );
		if( sp != nullptr )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		/**********************************************************
		 *	Elemental mastery
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16166 ); 
		if( sp != nullptr )
		{
			sp->EffectSpellGroupRelation[0] = 0xFFFFFFFF;//nature+fire+frost is all that shaman can do
			sp->EffectSpellGroupRelation_high[0] = 0xFFFFFFFF;//nature+fire+frost is all that shaman can do
			sp->EffectSpellGroupRelation[1] = 0xFFFFFFFF;//nature+fire+frost is all that shaman can do
			sp->EffectSpellGroupRelation_high[1] = 0xFFFFFFFF;//nature+fire+frost is all that shaman can do
			sp->EffectMiscValue[0] = SMT_CRITICAL;
			sp->EffectMiscValue[1] = SMT_COST;
		}

		/**********************************************************
		 *	Focused Casting
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 29063 );
		if( sp != nullptr )
		{
			sp->EffectSpellGroupRelation[0] =  0xFFFFFFFF; // shaman spells. Guess that wraps them all 
			sp->EffectSpellGroupRelation_high[0] =  0xFFFFFFFF; // shaman spells. Guess that wraps them all 
		}

		/**********************************************************
		 *	Healing Focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16181 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16230 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16232 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16233 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16234 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;

		/**********************************************************
		 *	Improved Lightning shield 
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16261 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = 1024;
		sp = dbcSpell.LookupEntryForced( 16290 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = 1024;
		sp = dbcSpell.LookupEntryForced( 16291 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = 1024;
		/**********************************************************
		 *	Tidal focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16179 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16214 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16215 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16216 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16217 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;

		/**********************************************************
		 *	Enhancing Totems
		 **********************************************************/
		group_relation_shaman_enhancing_totems = 0x00010000 | 0x00020000; // <--- GROUPING
		sp = dbcSpell.LookupEntryForced( 16259 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_enhancing_totems;
		sp = dbcSpell.LookupEntryForced( 16295 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_enhancing_totems;

		/**********************************************************
		 *	Elemental Fury - ! Not finished !
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16089 ); 
		if( sp != nullptr )
		{
			sp->EffectSpellGroupRelation[0] = 0xFFFFFFFF ; //damn, what other spells do there remain after that list ? Maybe later :P
			//sp->EffectSpellGroupRelation[0] = 1073741824 | 32 | 1048576 | 1 | ... ; //Searing/Magma/Fire Nova Totem effects and Fire,Frost,Nature spells
			sp->EffectSpellGroupRelation_high[0] = 0xFFFFFFFF ; //damn, what other spells do there remain after that list ? Maybe later :P
		}

		/**********************************************************
		 *	Restorative Totems
		 **********************************************************/
		group_relation_shaman_restorative_totems = 0x00004000 | 0x00002000; // <--- GROUPING
		sp = dbcSpell.LookupEntryForced( 16259 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;
		sp = dbcSpell.LookupEntryForced( 16205 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;
		sp = dbcSpell.LookupEntryForced( 16206 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;
		sp = dbcSpell.LookupEntryForced( 16207 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;
		sp = dbcSpell.LookupEntryForced( 16208 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;

		/**********************************************************
		 *	Ancestral healing proc spell
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16177 );
		if( sp != nullptr )
			sp->rangeIndex = 4;
		sp = dbcSpell.LookupEntryForced( 16236 );
		if( sp != nullptr )
			sp->rangeIndex = 4;
		sp = dbcSpell.LookupEntryForced( 16237 );
		if( sp != nullptr )
			sp->rangeIndex = 4;

		/**********************************************************
		 *	Totems grouping
		 **********************************************************/
		group_relation_shaman_totems |= 0x00000008 | 0x00000010 | 0x00001000 | 0x00080000 | 0x20000000;
		
		/**********************************************************
		 *	Totemic focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16173 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		sp = dbcSpell.LookupEntryForced( 16222 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		sp = dbcSpell.LookupEntryForced( 16223 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		sp = dbcSpell.LookupEntryForced( 16224 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		sp = dbcSpell.LookupEntryForced( 16225 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		
		/**********************************************************
		 *	Lightning - Grouping
		 **********************************************************/
		group_relation_shaman_lightning = 0x00000001 | 0x00000002;
		
		/**********************************************************
		 *	Call of Thunder
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16041 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16117 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16118 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16119 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16120 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		
		/**********************************************************
		 *	Shock Grouping
		 **********************************************************/
		group_relation_shaman_shock = 0x00100000 | 0x10000000 | 0x80000000;

		/**********************************************************
		 *	Convection
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16039 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16109 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16110 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16111 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16112 ); 
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;

		/**********************************************************
		 *	Concussion
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16035 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16105 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16106 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16107 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16108 );
		if( sp != nullptr )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;

		//wrath of air totem targets sorounding creatures instead of us
		sp = dbcSpell.LookupEntryForced( 2895 );
		if( sp != nullptr )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		sp = dbcSpell.LookupEntryForced( 20608 ); //Reincarnation
		if( sp != nullptr )
		{
			for(uint32 i=0;i<8;i++)
			{
				if(sp->Reagent[i])
				{
					sp->Reagent[i] = 0;
					sp->ReagentCount[i] = 0;
				}
			}
		}
		
		// Lightning Shield - cannot crit
		if( sp->NameHash == SPELL_HASH_LIGHTNING_SHIELD ) // not a mistake, the correct proc spell for lightning shield is also called lightning shield
			sp->spell_can_crit = false;

		// Frostbrand Weapon - 10% spd coefficient and 9ppm
		if( sp->NameHash == SPELL_HASH_FROSTBRAND_ATTACK )
			sp->Dspell_coef_override = 0.1f;

		if( sp->NameHash == SPELL_HASH_FROSTBRAND_WEAPON )
			sp->ProcsPerMinute = 9.0f;

		// Fire Nova - 21.4% spd coefficient
		if( sp->NameHash == SPELL_HASH_FIRE_NOVA )
			sp->Dspell_coef_override = 0.2142f;

		// Magma Totem - 10% spd coefficient
		if( sp->NameHash == SPELL_HASH_MAGMA_TOTEM )
			sp->Dspell_coef_override = 0.1f;

		// Searing Totem - 16.67% spd coefficient
		if( sp->NameHash == SPELL_HASH_ATTACK )
			sp->Dspell_coef_override = 0.1667f;

		// Healing Stream Totem - 4.5% healing coefficient
		if( sp->NameHash == SPELL_HASH_HEALING_STREAM_TOTEM )
			sp->Dspell_coef_override = 0.045f;

		// Earth Shield
		if( sp->Id == 947) 
			sp->Dspell_coef_override = 0.2857f;

		if(sp->NameHash == SPELL_HASH_WINDFURY_WEAPON || sp->NameHash == SPELL_HASH_FLAMETONGUE_WEAPON || sp->NameHash == SPELL_HASH_ROCKBITER_WEAPON ||
				sp->NameHash == SPELL_HASH_FROSTBRAND_WEAPON )
			sp->Flags3 |= FLAGS3_ENCHANT_OWN_ONLY;

		// Stoneclaw Totem
		if(sp->NameHash == SPELL_HASH_STONECLAW_TOTEM_PASSIVE)
			sp->procFlags = PROC_ON_TAKE_DAMAGE;

		// Flametongue Totem passive target fix
		if(sp->NameHash == SPELL_HASH_FLAMETONGUE_TOTEM && sp->Attributes & ATTRIBUTES_PASSIVE)
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[1] = 0;
		}

		// Unleashed Rage
		if(sp->NameHash == SPELL_HASH_UNLEASHED_RAGE)
			sp->procFlags = PROC_ON_MELEE_CRIT;

		// Healing Stream
		if(sp->NameHash == SPELL_HASH_HEALING_STREAM_TOTEM && sp->Effect[0] == SPELL_EFFECT_DUMMY)
		{
			sp->EffectRadiusIndex[0] = 10; // 30 yards
			sp->Effect[0] = SPELL_EFFECT_HEAL;
			sp->logsId = 5394;
		}

		// Mana Spring
		if(sp->NameHash == SPELL_HASH_MANA_SPRING_TOTEM && sp->Effect[0] == SPELL_EFFECT_DUMMY)
		{
			sp->Effect[0] = SPELL_EFFECT_ENERGIZE;
			sp->EffectMiscValue[0] = POWER_TYPE_MANA;
			sp->logsId = 5675;
		}
		
		if(sp->NameHash == SPELL_HASH_TETANUS || sp->NameHash == SPELL_HASH_PIERCE_ARMOR) // westfall mob spell, pretty hurtful stacking
			sp->Unique = true;

		//////////////////////////////////////////
		// MAGE								//
		//////////////////////////////////////////

		// Insert mage spell fixes here

		if( sp->NameHash == SPELL_HASH_FROSTBITE )
		{
			sp->procFlags = PROC_NULL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_ROOT;
			sp->EffectTriggerSpell[0] = 12494;
			sp->School = SCHOOL_FROST;
			sp->c_is_flags |= SPELL_FLAG_CANNOT_PROC_ON_SELF;
		}
		
		if( sp->NameHash == SPELL_HASH_CONE_OF_COLD )
			sp->EffectImplicitTargetA[0] = sp->EffectImplicitTargetA[1] = sp->EffectImplicitTargetA[2] = 24;

		//////////////////////////////////////////
		// WARLOCK								//
		//////////////////////////////////////////

		// Insert warlock spell fixes here
		
		if(sp->NameHash == SPELL_HASH_CONFLAGRATE)
			sp->Effect[0] = SPELL_EFFECT_DUMMY;

		//////////////////////////////////////////
		// DRUID								//
		//////////////////////////////////////////

		// Insert druid spell fixes here
		
		// Prowl
		if(sp->NameHash == SPELL_HASH_PROWL)
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_STEALTH;
			sp->EffectTriggerSpell[0] = 0;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DECREASE_SPEED;
			sp->EffectTriggerSpell[1] = 0;
			sp->Effect[2] = 0;
			sp->EffectApplyAuraName[2] = sp->EffectBasePoints[2] = sp->EffectTriggerSpell[2] = 0;
		}

		
 		///////////////////////////////////////////
 		// Pets								     //
 		///////////////////////////////////////////
 
 		sp = dbcSpell.LookupEntryForced( 1515 ); //Standard tame beast
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 13481;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19548 ); //Tame Ice Claw Bear
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 19597;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19674 ); //Tame Large Crag Boar
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 19677;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19687 ); //Tame Snow Leopard
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 19676;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 		
 		sp = dbcSpell.LookupEntryForced( 19688 ); //Tame Adult Plainstrider
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 19678;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 		
 		sp = dbcSpell.LookupEntryForced( 19689 ); //Tame Prairie Stalker
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 19679;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19692 ); //Tame Swoop
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 19680;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19693 ); //Tame Webwood Lurker
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 19684;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19694 ); //Tame Dire Mottled Boar
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 19681;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19696 ); //Tame Surf Crawler
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 19682;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19697 ); //Tame Armored Scorpid
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 19683;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19699 ); //Tame Nightsaber Stalker
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 19685;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19700 ); //Tame Strigid Screecher
 		if(sp != nullptr)
 		{
 			sp->EffectTriggerSpell[1] = 19686;
 			sp->EffectAmplitude[1] = 20000;
			sp->EffectMiscValue[2] = 1;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
		}			
	}	

	//Fully loaded coefficients, we must share channeled coefficient to its triggered spells
	for(uint32 x=0; x < cnt; x++)
	{
		sp = dbcSpell.LookupRow(x);
		ApplySingleSpellFixes(sp);
	}
	for(list<SpellEntry*>::iterator itr = sWorld.dummyspells.begin(); itr != sWorld.dummyspells.end(); ++itr)
		ApplySingleSpellFixes(*itr);

	sp = dbcSpell.LookupEntryForced( 26659 );
	SpellEntry * sp2 = sp;
	sp2->Id = 62388;
	sp2->Name = "Dummy Shit";
	sp2->DurationIndex = 41;
	sp2->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
	dbcSpell.SetRow(62388,sp2);
}
