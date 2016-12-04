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
		name[fieldSize-prefixLen-2] = 0; // in case nametext is too long and strncpy didn't copy the null
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

void ApplyExtraDataFixes()
{
	SpellEntry * sp;
	// Spell 53 (Backstab Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(53);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 72 (Shield Bash Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(72);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 75 (Auto Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(75);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 78 (Heroic Strike Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(78);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 116 (Frostbolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(116);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 118 (Polymorph Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(118);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 133 (Fireball Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(133);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 143 (Fireball Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(143);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 145 (Fireball Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(145);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 172 (Corruption Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(172);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 205 (Frostbolt Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(205);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 228 (Polymorph: Chicken Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(228);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 246 (Slow Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(246);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 284 (Heroic Strike Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(284);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 285 (Heroic Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 348 (Immolate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(348);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 370 (Purge Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(370);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 403 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(403);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 408 (Kidney Shot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(408);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 421 (Chain Lightning Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(421);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 529 (Lightning Bolt Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(529);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 548 (Lightning Bolt Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(548);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 585 (Smite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(585);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 589 (Shadow Word: Pain Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(589);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 591 (Smite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(591);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 594 (Shadow Word: Pain Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(594);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 598 (Smite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(598);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 603 (Curse of Doom Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(603);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 686 (Shadow Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(686);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 689 (Drain Life Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(689);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 694 (Mocking Blow Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(694);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 695 (Shadow Bolt Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(695);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 699 (Drain Life Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(699);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 701 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(701);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 702 (Curse of Weakness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(702);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 703 (Garrote Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(703);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 704 (Curse of Recklessness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(704);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 705 (Shadow Bolt Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(705);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 707 (Immolate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(707);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 709 (Drain Life Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(709);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 710 (Banish Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(710);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 770 (Faerie Fire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(770);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 772 (Rend Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(772);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 778 (Faerie Fire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(778);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 830 (Fearless ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(830);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 837 (Frostbolt Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(837);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 845 (Cleave Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(845);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 851 (Polymorph: Sheep Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(851);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 853 (Hammer of Justice Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(853);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 879 (Exorcism Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(879);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 915 (Lightning Bolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(915);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 921 (Pick Pocket ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(921);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 930 (Chain Lightning Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(930);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 943 (Lightning Bolt Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(943);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 970 (Shadow Word: Pain Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(970);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 980 (Curse of Agony Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(980);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 984 (Smite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(984);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 992 (Shadow Word: Pain Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(992);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 1004 (Smite Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1004);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1010 (Curse of Idiocy Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1010);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 1014 (Curse of Agony Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1014);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 1079 (Rip Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1079);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1082 (Claw Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1082);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1088 (Shadow Bolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1088);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1094 (Immolate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1094);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1098 (Enslave Demon Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1098);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1106 (Shadow Bolt Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1106);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1108 (Curse of Weakness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1108);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 1120 (Drain Soul Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1120);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1130 (Hunter's Mark Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1130);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1329 (Mutilate Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(1329);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 1464 (Slam Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1464);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1490 (Curse of the Elements Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1490);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 1495 (Mongoose Bite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1495);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1510 (Volley Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1510);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1608 (Heroic Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1608);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1671 (Shield Bash Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1671);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1672 (Shield Bash Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1672);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1714 (Curse of Tongues Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1714);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 1715 (Hamstring Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1715);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1752 (Sinister Strike Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1752);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1757 (Sinister Strike Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1757);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1758 (Sinister Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1758);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1759 (Sinister Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1759);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1760 (Sinister Strike Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1760);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1766 (Kick Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1766);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1767 (Kick Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1767);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1768 (Kick Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1768);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1769 (Kick Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1769);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1776 (Gouge Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1776);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1777 (Gouge Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1777);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1804 (Pick Lock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1804);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1822 (Rake Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1822);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1823 (Rake Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1823);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1824 (Rake Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1824);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1940 (Rocket Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1940);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1943 (Rupture Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1943);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1949 (Hellfire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1949);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2070 (Sap Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2070);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2098 (Eviscerate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2098);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2136 (Fire Blast Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2136);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2137 (Fire Blast Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2137);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2138 (Fire Blast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2138);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2589 (Backstab Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(2589);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 2590 (Backstab Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(2590);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 2591 (Backstab Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(2591);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 2596 (Touch of the Black Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2596);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2626 (Lightning Bolt Proc V ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2626);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2637 (Hibernate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2637);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2643 (Multi-Shot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2643);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2652 (Touch of Weakness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2652);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2691 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2691);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2764 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2764);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2767 (Shadow Word: Pain Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2767);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 2812 (Holy Wrath Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2812);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2860 (Chain Lightning Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2860);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2908 (Soothe Animal Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2908);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2912 (Starfire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2912);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2941 (Immolate Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2941);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2943 (Touch of Weakness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2943);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2944 (Devouring Plague Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2944);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2948 (Scorch Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2948);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2973 (Raptor Strike Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2973);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2974 (Wing Clip Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2974);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3009 (Claw Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3009);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3010 (Claw Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3010);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3018 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3018);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3029 (Claw Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3029);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3034 (Viper Sting Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3034);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3044 (Arcane Shot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3044);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3110 (Firebolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3110);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3130 (Ice Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3130);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3140 (Fireball Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3140);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3232 (Gouge Stun Test ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3232);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3242 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3242);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3252 (Shred ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3252);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3263 (Touch of Ravenclaw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3263);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3446 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3446);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3674 (Black Arrow Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3674);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3720 (Throw Syndicate Bomb ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3720);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3823 (Throw Proximity Bomb ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3823);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 4043 (Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(4043);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 4164 (Throw Rock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(4164);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 4165 (Throw Rock II ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(4165);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5010 (Scorch Breath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5010);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5019 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5019);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5138 (Drain Mana Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5138);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5143 (Arcane Missiles Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5143);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5144 (Arcane Missiles Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5144);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5145 (Arcane Missiles Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5145);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5171 (Slice and Dice Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5171);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5176 (Wrath Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5176);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5177 (Wrath Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5177);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5178 (Wrath Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5178);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5179 (Wrath Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5179);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5180 (Wrath Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5180);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5201 (Claw Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5201);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5211 (Bash Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5211);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5221 (Shred Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(5221);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 5256 (Claw Cover ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5256);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5401 (Lizard Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5401);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5422 (Lacerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5422);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5424 (Claw Grasp ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5424);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5480 (Cleave Armor Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5480);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5508 (Cleave Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5508);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5532 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5532);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5570 (Insect Swarm Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5570);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5588 (Hammer of Justice Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5588);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5589 (Hammer of Justice Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5589);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5614 (Exorcism Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5614);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5615 (Exorcism Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5615);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5648 (Stunning Blast Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5648);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5649 (Stunning Blast Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5649);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5676 (Searing Pain Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5676);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5782 (Fear Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5782);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 5938 (Shiv Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5938);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5940 (Shiv Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5940);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6041 (Lightning Bolt Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6041);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6060 (Smite Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6060);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6136 (Chilled Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6136);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6146 (Slow Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6146);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6205 (Curse of Weakness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6205);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 6213 (Fear Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6213);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 6215 (Fear Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6215);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 6217 (Curse of Agony Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6217);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 6222 (Corruption Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6222);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 6223 (Corruption Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6223);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 6226 (Drain Mana Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6226);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6254 (Chained Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6254);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6255 (Lizard Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6255);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6345 (Polymorphic Explosion ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6345);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6346 (Fear Ward ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6346);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6353 (Soul Fire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6353);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6434 (Slice and Dice Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6434);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6546 (Rend Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6546);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6547 (Rend Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6547);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6548 (Rend Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6548);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6552 (Pummel Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6552);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6554 (Pummel Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6554);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6572 (Revenge Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6572);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6574 (Revenge Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6574);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6660 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6660);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6760 (Eviscerate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6760);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6761 (Eviscerate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6761);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6762 (Eviscerate Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6762);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6770 (Sap Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6770);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6774 (Slice and Dice Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6774);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6780 (Wrath Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6780);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6785 (Ravage Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(6785);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 6787 (Ravage Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(6787);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 6789 (Death Coil Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6789);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6798 (Bash Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6798);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6800 (Shred Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(6800);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 6949 (Weak Frostbolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6949);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6950 (Faerie Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6950);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6965 (Polymorphic Ray ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6965);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7101 (Flame Blast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7101);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7160 (Execute ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7160);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7269 (Arcane Missiles Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7269);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7270 (Arcane Missiles Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7270);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7288 (Immolate Cumulative (TEST) Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7288);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7321 (Chilled Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7321);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7322 (Frostbolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7322);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7369 (Cleave Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7369);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7372 (Hamstring Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7372);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7373 (Hamstring Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7373);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7379 (Revenge Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7379);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7384 (Overpower Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7384);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7386 (Sunder Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7386);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7400 (Mocking Blow Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7400);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7402 (Mocking Blow Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7402);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7405 (Sunder Armor Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7405);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7588 (Void Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7588);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7641 (Shadow Bolt Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7641);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7646 (Curse of Weakness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7646);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 7648 (Corruption Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7648);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 7651 (Drain Life Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7651);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7656 (Hex of Ravenclaw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7656);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7657 (Hex of Ravenclaw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7657);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7658 (Curse of Recklessness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7658);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 7659 (Curse of Recklessness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7659);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 7799 (Firebolt Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7799);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7800 (Firebolt Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7800);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7801 (Firebolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7801);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7802 (Firebolt Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7802);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7887 (Overpower Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7887);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7978 (Throw Dynamite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7978);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8012 (Purge Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8012);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8042 (Earth Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8042);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8044 (Earth Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8044);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8045 (Earth Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8045);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8046 (Earth Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8046);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8050 (Flame Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8050);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8052 (Flame Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8052);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8053 (Flame Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8053);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8056 (Frost Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8056);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8058 (Frost Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8058);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8092 (Mind Blast Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8092);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8102 (Mind Blast Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8102);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8103 (Mind Blast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8103);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8104 (Mind Blast Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8104);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8105 (Mind Blast Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8105);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8106 (Mind Blast Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8106);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8129 (Mana Burn Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8129);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8131 (Mana Burn Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8131);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8242 (Shield Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8242);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8246 (Lightning Bolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8246);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8288 (Drain Soul Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8288);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8289 (Drain Soul Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8289);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8292 (Chain Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8292);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8380 (Sunder Armor Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8380);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8391 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8391);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8398 (Frostbolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8398);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8400 (Fireball Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8400);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8401 (Fireball Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8401);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8402 (Fireball Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8402);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8406 (Frostbolt Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8406);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8407 (Frostbolt Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8407);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8408 (Frostbolt Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8408);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8412 (Fire Blast Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8412);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8413 (Fire Blast Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8413);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8416 (Arcane Missiles Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8416);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8417 (Arcane Missiles Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8417);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8418 (Arcane Missiles Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8418);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8419 (Arcane Missiles Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8419);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8444 (Scorch Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8444);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8445 (Scorch Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8445);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8446 (Scorch Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8446);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8552 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8552);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 8598 (Lightning Blast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8598);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8621 (Sinister Strike Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8621);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8623 (Eviscerate Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8623);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8624 (Eviscerate Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8624);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8627 (Backstab (TEST) Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8627);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8629 (Gouge Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8629);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8631 (Garrote Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8631);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8632 (Garrote Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8632);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8633 (Garrote Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8633);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8639 (Rupture Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8639);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8640 (Rupture Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8640);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8643 (Kidney Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8643);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8647 (Expose Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8647);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8649 (Expose Armor Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8649);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8650 (Expose Armor Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8650);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8676 (Ambush Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8676);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8721 (Backstab Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8721);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8724 (Ambush Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8724);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8725 (Ambush Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8725);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8818 (Garrote Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8818);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8820 (Slam Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8820);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8905 (Wrath Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8905);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8921 (Moonfire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8921);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8924 (Moonfire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8924);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8925 (Moonfire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8925);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8926 (Moonfire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8926);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8927 (Moonfire Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8927);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8928 (Moonfire Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8928);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8929 (Moonfire Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8929);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8949 (Starfire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8949);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8950 (Starfire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8950);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8951 (Starfire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8951);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8955 (Soothe Animal Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8955);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8983 (Bash Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8983);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8992 (Shred Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8992);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8994 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8994);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8995 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8995);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8996 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8996);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8997 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8997);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9007 (Pounce Bleed Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9007);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9008 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9008);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9034 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9034);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9035 (Hex of Weakness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9035);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9053 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9053);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9057 (Firebolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9057);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9080 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9080);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9081 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9081);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9275 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9275);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9347 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9347);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9435 (Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9435);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9484 (Shackle Undead Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9484);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9485 (Shackle Undead Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9485);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9487 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9487);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9488 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9488);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9492 (Rip Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9492);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9493 (Rip Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9493);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9532 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9532);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9575 (Self Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9575);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9613 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9613);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9672 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9672);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9739 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9739);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9749 (Faerie Fire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9749);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9752 (Rip Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9752);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9771 (Radiation Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9771);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9824 (Pounce Bleed Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9824);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9826 (Pounce Bleed Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9826);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9829 (Shred Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(9829);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 9830 (Shred Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(9830);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 9833 (Moonfire Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9833);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9834 (Moonfire Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9834);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9835 (Moonfire Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9835);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9849 (Claw Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9849);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9850 (Claw Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9850);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9866 (Ravage Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(9866);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 9867 (Ravage Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(9867);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 9875 (Starfire Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9875);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9876 (Starfire Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9876);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9894 (Rip Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9894);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9896 (Rip Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9896);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9901 (Soothe Animal Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9901);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9904 (Rake Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9904);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9907 (Faerie Fire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9907);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9912 (Wrath Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9912);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10148 (Fireball Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10148);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10149 (Fireball Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10149);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10150 (Fireball Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10150);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10151 (Fireball Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10151);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10179 (Frostbolt Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10179);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10180 (Frostbolt Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10180);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10181 (Frostbolt Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10181);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10197 (Fire Blast Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10197);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10199 (Fire Blast Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10199);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10205 (Scorch Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10205);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10206 (Scorch Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10206);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10207 (Scorch Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10207);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10211 (Arcane Missiles Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10211);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10212 (Arcane Missiles Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10212);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10273 (Arcane Missiles Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10273);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10274 (Arcane Missiles Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10274);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10277 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10277);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10308 (Hammer of Justice Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10308);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10312 (Exorcism Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10312);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10313 (Exorcism Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10313);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10314 (Exorcism Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10314);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10318 (Holy Wrath Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10318);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10371 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10371);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10391 (Lightning Bolt Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10391);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10392 (Lightning Bolt Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10392);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10412 (Earth Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10412);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10413 (Earth Shock Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10413);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10414 (Earth Shock Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10414);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10447 (Flame Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10447);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10448 (Flame Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10448);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10472 (Frost Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10472);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10473 (Frost Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10473);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10578 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10578);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10605 (Chain Lightning Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10605);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10617 (Release Rageclaw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10617);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10797 (Starshards Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10797);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10833 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10833);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10874 (Mana Burn Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10874);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10875 (Mana Burn Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10875);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10876 (Mana Burn Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10876);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10887 (Crowd Pummel Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10887);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10892 (Shadow Word: Pain Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10892);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 10893 (Shadow Word: Pain Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10893);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 10894 (Shadow Word: Pain Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10894);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 10933 (Smite Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10933);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10934 (Smite Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10934);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10945 (Mind Blast Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10945);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10946 (Mind Blast Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10946);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10947 (Mind Blast Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10947);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10955 (Shackle Undead Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10955);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11085 (Chain Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11085);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11113 (Blast Wave Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11113);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11197 (Expose Armor Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11197);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11198 (Expose Armor Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11198);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11264 (Ice Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11264);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11267 (Ambush Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11267);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11268 (Ambush Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11268);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11269 (Ambush Rank 6) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11269);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11273 (Rupture Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11273);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11274 (Rupture Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11274);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11275 (Rupture Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11275);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11279 (Backstab Rank 6) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11279);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11280 (Backstab Rank 7) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11280);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11281 (Backstab Rank 8) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11281);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11285 (Gouge Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11286 (Gouge Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11286);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11289 (Garrote Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11289);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11290 (Garrote Rank 6) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11290);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11293 (Sinister Strike Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11293);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11294 (Sinister Strike Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11294);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11297 (Sap Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11297);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11299 (Eviscerate Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11299);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11300 (Eviscerate Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11300);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11366 (Pyroblast Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11366);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11427 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11427);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11430 (Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11430);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11436 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11436);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11444 (Shackle Undead ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11444);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11538 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11538);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11564 (Heroic Strike Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11564);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11565 (Heroic Strike Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11565);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11566 (Heroic Strike Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11566);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11567 (Heroic Strike Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11567);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11572 (Rend Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11572);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11573 (Rend Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11573);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11574 (Rend Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11574);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11584 (Overpower Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11584);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11585 (Overpower Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11585);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11596 (Sunder Armor Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11596);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11597 (Sunder Armor Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11597);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11600 (Revenge Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11600);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11601 (Revenge Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11601);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11604 (Slam Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11604);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11605 (Slam Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11605);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11608 (Cleave Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11608);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11609 (Cleave Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11609);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11639 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11639);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11659 (Shadow Bolt Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11659);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11660 (Shadow Bolt Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11660);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11661 (Shadow Bolt Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11661);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11665 (Immolate Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11665);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11667 (Immolate Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11667);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11668 (Immolate Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11668);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11671 (Corruption Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11671);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11672 (Corruption Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11672);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11675 (Drain Soul Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11675);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11683 (Hellfire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11683);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11684 (Hellfire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11684);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11699 (Drain Life Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11699);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11700 (Drain Life Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11700);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11703 (Drain Mana Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11703);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11704 (Drain Mana Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11704);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11707 (Curse of Weakness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11707);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11708 (Curse of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11708);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11711 (Curse of Agony Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11711);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11712 (Curse of Agony Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11712);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11713 (Curse of Agony Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11713);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11717 (Curse of Recklessness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11717);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11719 (Curse of Tongues Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11719);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11721 (Curse of the Elements Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11721);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11722 (Curse of the Elements Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11722);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11725 (Enslave Demon Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11725);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11726 (Enslave Demon Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11726);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11762 (Firebolt Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11762);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11763 (Firebolt Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11763);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11815 (Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11815);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11839 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11839);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11891 (Antu'sul Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11891);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11921 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11921);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11962 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11962);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11971 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11971);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11972 (Shield Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11972);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11977 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11977);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11978 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11978);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11980 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11980);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 11981 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11981);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11985 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11985);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11988 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11988);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11989 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11989);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12054 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12054);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12058 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12058);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12096 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12096);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12167 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12167);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12170 (Revenge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12170);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12294 (Mortal Strike Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12294);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12331 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12331);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12355 (Impact Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12355);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12466 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12466);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12471 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12471);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12484 (Chilled Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12484);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12485 (Chilled Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12485);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12486 (Chilled Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12486);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12493 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12493);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 12494 (Frostbite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12494);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12505 (Pyroblast Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12505);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12522 (Pyroblast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12522);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12523 (Pyroblast Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12523);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12524 (Pyroblast Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12524);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12525 (Pyroblast Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12525);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12526 (Pyroblast Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12526);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12538 (Ravenous Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12538);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12540 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12540);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12542 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12542);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12548 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12548);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12555 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12555);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12675 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12675);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12693 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12693);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12733 (Fearless ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12733);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12737 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12737);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12739 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12739);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12741 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12741);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 12742 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12742);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12745 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12745);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12824 (Polymorph Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12824);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 12825 (Polymorph Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12825);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 12826 (Polymorph Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12826);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 12889 (Curse of Tongues ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12889);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 12964 (Unbridled Wrath Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12964);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13005 (Hammer of Justice ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13005);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13009 (Amnennar's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13009);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13018 (Blast Wave Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13018);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13019 (Blast Wave Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13019);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13020 (Blast Wave Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13020);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13021 (Blast Wave Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13021);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13140 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13140);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13281 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13281);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13318 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13318);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13321 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13321);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13322 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13322);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13323 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13323);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13338 (Curse of Tongues ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13338);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 13339 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13339);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13340 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13340);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13341 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13341);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13342 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13342);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13374 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13374);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13375 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13375);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13398 (Throw Wrench ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13398);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13424 (Faerie Fire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13424);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13438 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13438);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13439 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13439);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13440 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13440);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13441 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13441);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13442 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13442);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13443 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13443);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13444 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13444);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13445 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13445);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13480 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13480);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13482 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13482);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13491 (Pummel Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13491);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13527 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13527);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13530 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13530);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13553 (Serpent Sting Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13553);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13554 (Serpent Sting Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13554);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13555 (Serpent Sting Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13555);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13579 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13579);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13728 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13728);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13729 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13729);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13737 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13737);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13738 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13738);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13747 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13747);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13748 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13748);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13752 (Faerie Fire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13752);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13860 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13860);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13878 (Scorch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13878);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13901 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13901);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14032 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14032);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 14033 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14033);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14034 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14034);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14087 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14087);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14103 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14103);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14105 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14105);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14106 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14106);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14109 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14109);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14118 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14118);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14119 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14119);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14122 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14122);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14145 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14145);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14200 (Chained Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14200);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14260 (Raptor Strike Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14260);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14261 (Raptor Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14261);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14262 (Raptor Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14262);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14263 (Raptor Strike Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14263);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14264 (Raptor Strike Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14264);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14265 (Raptor Strike Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14265);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14266 (Raptor Strike Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14266);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14267 (Wing Clip Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14267);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14268 (Wing Clip Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14268);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14269 (Mongoose Bite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14269);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14270 (Mongoose Bite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14270);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14271 (Mongoose Bite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14271);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14279 (Viper Sting Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14279);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14280 (Viper Sting Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14280);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14281 (Arcane Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14281);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14282 (Arcane Shot Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14282);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14283 (Arcane Shot Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14283);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14284 (Arcane Shot Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14284);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14285 (Arcane Shot Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14286 (Arcane Shot Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14286);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14287 (Arcane Shot Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14287);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14288 (Multi-Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14288);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14289 (Multi-Shot Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14289);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14290 (Multi-Shot Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14290);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14294 (Volley Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14294);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14295 (Volley Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14295);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14296 (Black Arrow Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14296);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14323 (Hunter's Mark Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14323);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14324 (Hunter's Mark Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14324);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14325 (Hunter's Mark Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14325);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14443 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14443);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14621 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14621);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 14868 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14868);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 14871 (Shadow Bolt Misfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14871);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14873 (Sinister Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14873);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14874 (Rupture ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14874);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14875 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14875);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 14887 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14887);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14895 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14895);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14903 (Rupture ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14903);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14914 (Holy Fire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14914);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15039 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15039);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15040 (Molten Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15040);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15043 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15043);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15089 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15089);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15091 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15091);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15095 (Molten Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15095);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15096 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15096);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15117 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15117);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15124 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15124);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15207 (Lightning Bolt Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15207);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15208 (Lightning Bolt Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15208);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15228 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15228);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15230 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15230);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15232 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15232);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15234 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15234);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15241 (Scorch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15241);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15242 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15242);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15243 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15243);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15245 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15245);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15254 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15254);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15261 (Holy Fire Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15261);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15262 (Holy Fire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15262);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15263 (Holy Fire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15263);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15264 (Holy Fire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15264);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15265 (Holy Fire Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15265);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15266 (Holy Fire Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15266);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15267 (Holy Fire Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15267);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15280 (Cleave Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15280);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15284 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15284);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15285 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15305 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15305);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15407 (Mind Flay Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15407);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15451 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15451);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15470 (Curse of Tongues ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15470);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 15472 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15472);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15496 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15496);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15497 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15497);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15499 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15499);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15501 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15501);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15502 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15502);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15505 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15505);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15530 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15530);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15534 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15534);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15536 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15536);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15537 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15537);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15547 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15547);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15549 (Chained Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15549);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15570 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15570);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15572 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15572);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15574 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15574);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15579 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15579);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15581 (Sinister Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15581);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15583 (Rupture ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15583);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15584 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15584);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15587 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15587);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15592 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15592);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15598 (Firebolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15598);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15607 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15607);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15608 (Ravenous Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15608);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15610 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15610);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15611 (Lizard Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15611);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15612 (Lizard Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15612);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15613 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15613);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15614 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15614);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15615 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15615);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15616 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15616);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15619 (Throw Wrench ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15619);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15620 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15620);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15622 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15622);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15623 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15623);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15654 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15654);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 15655 (Shield Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15655);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15659 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15659);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15661 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15661);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15662 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15662);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15663 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15663);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15665 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15665);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15667 (Sinister Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15667);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15691 (Eviscerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15691);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15692 (Eviscerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15692);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15708 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15708);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15732 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15732);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15735 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15735);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15736 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15736);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15744 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15744);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15754 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15754);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15785 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15785);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15790 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15790);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15791 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15791);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15795 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15795);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15798 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15798);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15800 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15800);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15801 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15801);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15850 (Chilled ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15850);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15878 (Ice Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15878);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15979 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15979);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15980 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15980);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16000 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16000);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16006 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16006);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16033 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16033);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16044 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16044);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16046 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16046);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16067 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16067);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16075 (Throw Axe ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16075);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16100 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16100);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16101 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16101);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16144 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16144);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16145 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16145);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16231 (Curse of Recklessness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16231);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 16249 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16249);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16250 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16250);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16375 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16375);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16393 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16393);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16402 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16402);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16403 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16403);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16406 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16406);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16407 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16407);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16408 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16408);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16409 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16409);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16410 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16410);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16412 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16412);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16413 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16413);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16414 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16414);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16415 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16415);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16454 (Searing Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16454);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16496 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16496);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16498 (Faerie Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16498);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16509 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16509);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16511 (Hemorrhage Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16511);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16553 (Ravenous Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16553);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16554 (Toxic Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16554);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16559 (Flame Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16559);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16560 (Flame Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16560);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16568 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16568);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16570 (Charged Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16570);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16572 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16572);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16608 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16608);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16767 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16767);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16768 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16768);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16772 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16772);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16775 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16775);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16776 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16776);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16777 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16777);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16778 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16778);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16779 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16779);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16780 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16780);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16782 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16782);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16783 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16783);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16784 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16784);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16788 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16788);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16799 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16799);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16804 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16804);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16827 (Claw Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16827);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16828 (Claw Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16828);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16829 (Claw Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16829);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16830 (Claw Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16830);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16831 (Claw Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16831);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16832 (Claw Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16832);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16856 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16856);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16857 (Faerie Fire (Feral) Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16857);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16921 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16921);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16922 (Starfire Stun ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16922);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16927 (Chilled ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16927);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17008 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17008);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17011 (Freezing Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17011);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17140 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17140);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17141 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17141);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17142 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17142);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17144 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17144);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17145 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17145);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17146 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17146);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 17147 (Exorcism ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17147);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17149 (Exorcism ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17149);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17153 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17153);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17165 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17165);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17173 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17173);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17194 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17194);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17195 (Scorch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17195);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17198 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17198);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17203 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17203);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17227 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17227);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 17228 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17228);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17238 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17238);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17243 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17243);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17253 (Bite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17253);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17255 (Bite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17255);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17256 (Bite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17256);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17257 (Bite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17257);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17258 (Bite Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17258);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17259 (Bite Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17259);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17260 (Bite Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17260);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17261 (Bite Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17261);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17273 (Pyroblast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17273);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17274 (Pyroblast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17274);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17277 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17277);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17287 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17287);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17290 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17290);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17311 (Mind Flay Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17311);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17312 (Mind Flay Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17312);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17313 (Mind Flay Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17313);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17314 (Mind Flay Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17314);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17347 (Hemorrhage Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17347);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17348 (Hemorrhage Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17348);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17353 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17353);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17368 (Egan's Blaster ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17368);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17390 (Faerie Fire (Feral) Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17390);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17391 (Faerie Fire (Feral) Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17391);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17392 (Faerie Fire (Feral) Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17392);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17393 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17393);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17434 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17434);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17435 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17435);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17470 (Ravenous Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17470);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17483 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17483);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17503 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17503);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17504 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17504);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17509 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17509);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17510 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17510);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17547 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17547);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17615 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17615);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17620 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17620);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17630 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17630);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17668 (Ramstein's Lightning Bolts ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17668);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17669 (Ramstein's Lightning Bolts ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17669);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17682 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17682);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17685 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17685);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17748 (Kick Peon ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17748);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17771 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17771);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 17862 (Curse of Shadow Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17862);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 17883 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17883);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17919 (Searing Pain Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17919);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17920 (Searing Pain Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17920);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17921 (Searing Pain Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17921);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17922 (Searing Pain Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17922);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17923 (Searing Pain Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17923);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17924 (Soul Fire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17924);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17925 (Death Coil Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17925);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17926 (Death Coil Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17926);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17937 (Curse of Shadow Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17937);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 17962 (Conflagrate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17962);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18075 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18075);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18078 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18078);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18081 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18081);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18082 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18082);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18083 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18083);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18084 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18084);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18085 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18085);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18086 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18086);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18088 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18088);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18089 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18089);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18091 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18091);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18092 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18092);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18101 (Chilled Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18101);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18104 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18104);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18105 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18105);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18106 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18106);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18108 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18108);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18111 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18111);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18112 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18112);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18137 (Shadowguard Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18137);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18138 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18138);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18164 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18164);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18165 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18165);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18187 (Firebolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18187);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18199 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18199);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18200 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18200);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18202 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18202);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18204 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18204);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18205 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18205);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18211 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18211);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18214 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18214);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18217 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18217);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18265 (Siphon Life Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18265);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18266 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18266);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 18267 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18267);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18276 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18276);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18371 (Drain Soul ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18371);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18376 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18376);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18392 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18392);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18396 (Dismounting Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18396);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18398 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18398);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18425 (Kick - Silenced ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18425);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18498 (Shield Bash - Silenced ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18498);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18557 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18557);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18561 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18561);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18647 (Banish Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18647);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18651 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18651);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18656 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18656);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18657 (Hibernate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18657);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18658 (Hibernate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18658);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18662 (Curse of Doom Effect ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18662);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18671 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18671);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 18796 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18796);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18807 (Mind Flay Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18807);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18809 (Pyroblast Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18809);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18817 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18817);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18833 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18833);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18879 (Siphon Life Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18879);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18880 (Siphon Life Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18880);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18881 (Siphon Life Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18881);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18930 (Conflagrate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18930);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18931 (Conflagrate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18931);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18932 (Conflagrate Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18932);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18972 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18972);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19032 (Throw Human Skull ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19032);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19130 (Revenge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19130);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19133 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19133);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19136 (Stormbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19136);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19137 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19137);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19229 (Improved Wing Clip ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19229);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19249 (Touch of Weakness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19249);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19251 (Touch of Weakness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19251);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19252 (Touch of Weakness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19252);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19253 (Touch of Weakness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19253);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19254 (Touch of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19254);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19260 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19260);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19261 (Touch of Weakness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19261);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19262 (Touch of Weakness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19262);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19264 (Touch of Weakness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19264);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19265 (Touch of Weakness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19265);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19266 (Touch of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19266);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19276 (Devouring Plague Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19276);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19277 (Devouring Plague Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19277);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19278 (Devouring Plague Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19278);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19279 (Devouring Plague Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19279);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19280 (Devouring Plague Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19280);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19281 (Hex of Weakness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19281);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19282 (Hex of Weakness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19282);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19283 (Hex of Weakness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19283);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19284 (Hex of Weakness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19284);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19285 (Hex of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19296 (Starshards Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19296);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19299 (Starshards Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19299);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19302 (Starshards Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19302);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19303 (Starshards Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19303);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19304 (Starshards Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19304);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19305 (Starshards Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19305);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19306 (Counterattack Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19306);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19308 (Shadowguard Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19308);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19309 (Shadowguard Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19309);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19310 (Shadowguard Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19310);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19311 (Shadowguard Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19311);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19312 (Shadowguard Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19312);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19386 (Wyvern Sting Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19386);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19391 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19391);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19397 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19397);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19472 (Sinister Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19472);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19574 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19574);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19597 (Tame Ice Claw Bear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19597);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19632 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19632);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19639 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19639);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19642 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19642);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19643 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19643);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19728 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19728);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19729 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19729);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19776 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19776);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 19785 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19785);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19816 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19816);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19820 (Mangle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19820);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19874 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19874);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19881 (Shoot (TEST) ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19881);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19983 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19983);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20005 (Chilled ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20005);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20184 (Judgement of Justice Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20184);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20185 (Judgement of Light Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20185);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20186 (Judgement of Wisdom Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20186);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20187 (Judgement of Righteousness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20187);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20188 (Judgement of the Crusader Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20188);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20228 (Pyroblast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20228);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20229 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20229);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20243 (Devastate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20243);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20253 (Intercept Stun Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20253);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20267 (Judgement of Light Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20267);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20268 (Judgement of Wisdom Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20268);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20271 (Judgement ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20271);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20280 (Judgement of Righteousness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20280);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20281 (Judgement of Righteousness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20281);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20282 (Judgement of Righteousness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20282);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20283 (Judgement of Righteousness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20283);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20284 (Judgement of Righteousness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20284);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20285 (Judgement of Righteousness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20286 (Judgement of Righteousness Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20286);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20294 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20294);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20295 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20295);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20297 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20297);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20298 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20298);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20300 (Judgement of the Crusader Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20300);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20301 (Judgement of the Crusader Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20301);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20302 (Judgement of the Crusader Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20302);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20303 (Judgement of the Crusader Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20303);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20341 (Judgement of Light Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20341);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20342 (Judgement of Light Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20342);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20343 (Judgement of Light Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20343);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20344 (Judgement of Light Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20344);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20345 (Judgement of Light Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20345);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20346 (Judgement of Light Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20346);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20352 (Judgement of Wisdom Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20352);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20353 (Judgement of Wisdom Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20353);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20354 (Judgement of Wisdom Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20354);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20355 (Judgement of Wisdom Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20355);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20420 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20420);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20425 (Judgement of Command Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20425);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20463 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20463);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20467 (Judgement of Command Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20467);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20473 (Holy Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20473);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20508 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20508);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20559 (Mocking Blow Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20559);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20560 (Mocking Blow Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20560);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20565 (Magma Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20565);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20566 (Wrath of Ragnaros ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20566);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20569 (Cleave Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20569);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20605 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20605);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20614 (Intercept Stun Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20614);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20615 (Intercept Stun Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20615);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20623 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20623);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20647 (Execute ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20647);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20656 (Faerie Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20656);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20666 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20666);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20677 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20677);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20678 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20678);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20679 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20679);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20684 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20684);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20685 (Storm Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20685);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20690 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20690);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20691 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20691);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20692 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20692);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20698 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20698);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20714 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20714);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20720 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20720);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20733 (Black Arrow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20733);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20735 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20735);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20741 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20741);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20743 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20743);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20787 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20787);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20791 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20791);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20792 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20792);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20793 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20793);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20795 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20795);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20797 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20797);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20800 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20800);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20801 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20801);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20802 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20802);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20805 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20805);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20806 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20806);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20807 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20807);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20808 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20808);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20811 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20811);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20815 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20815);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20816 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20816);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20817 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20817);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20819 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20819);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20822 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20822);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20823 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20823);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20824 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20824);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20825 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20825);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20826 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20826);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20829 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20829);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20830 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20830);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20831 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20831);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20832 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20832);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20869 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20869);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20882 (Enslave Demon ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20882);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20883 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20883);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20909 (Counterattack Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20909);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20910 (Counterattack Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20910);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20929 (Holy Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20929);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20930 (Holy Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20930);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20961 (Judgement of Command Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20961);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20962 (Judgement of Command Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20962);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20963 (Judgement of Command Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20963);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20964 (Judgement of Command Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20964);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20965 (Judgement of Command Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20965);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20966 (Judgement of Command Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20966);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20967 (Judgement of Command Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20967);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20968 (Judgement of Command Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20968);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21007 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21007);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21008 (Mocking Blow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21008);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21030 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21030);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21052 (Monty Bashes Rats (DND) ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21052);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21066 (Void Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21066);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21067 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21067);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21068 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21068);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21072 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21072);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21077 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21077);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21081 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21081);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21141 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21141);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21159 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21159);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21162 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21162);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21170 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21170);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21179 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21179);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21183 (Judgement of the Crusader Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21183);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21230 (Fireball, Improved DoT ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21230);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21341 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21341);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21369 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21369);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21390 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21390);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21401 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21401);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21402 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21402);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21549 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21549);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21551 (Mortal Strike Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21551);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21552 (Mortal Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21552);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21553 (Mortal Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21553);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21667 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21667);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21668 (Starfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21668);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21669 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21669);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21670 (Faerie Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21670);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21807 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21807);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21887 (Warrior's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21887);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21889 (Judgement Smite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21889);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21949 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21949);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21971 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21971);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21977 (Warrior's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21977);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21987 (Lash of Pain ) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(21987);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 22088 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22088);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22120 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22120);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22121 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22121);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22189 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22189);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22206 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22206);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22272 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22272);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22273 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22273);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22336 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22336);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22355 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22355);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22356 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22356);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22357 (Icebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22357);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22411 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22411);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22414 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22414);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22423 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22423);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22424 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22424);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22425 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22425);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22438 (Mark of Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22438);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22439 (Mark of Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22439);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22540 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22540);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22568 (Ferocious Bite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22568);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22570 (Maim Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22570);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22582 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22582);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22639 (Eskhandar's Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22639);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22643 (Frostbolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22643);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22665 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22665);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22677 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22677);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22678 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22678);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22689 (Mangle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22689);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22709 (Void Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22709);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22822 (Starshards ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22822);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22823 (Starshards ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22823);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22827 (Ferocious Bite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22827);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22828 (Ferocious Bite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22828);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22829 (Ferocious Bite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22829);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22851 (Ferocious Bite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22851);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22853 (Ferocious Bite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22853);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22861 (Ferocious Bite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22861);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22862 (Ferocious Bite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22862);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22878 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22878);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22885 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22885);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22887 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22887);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22893 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22893);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22907 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22907);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22908 (Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22908);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22911 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22911);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22919 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22919);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22920 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22920);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22936 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22936);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22937 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22937);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22940 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22940);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22947 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22947);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23024 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23024);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23038 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23038);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23039 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23039);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23073 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23073);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23102 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23102);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23106 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23106);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23113 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23113);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23114 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23114);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23115 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23115);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23206 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23206);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23267 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23267);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23268 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23268);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 23269 (Holy Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23269);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23308 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23308);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23309 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23309);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23331 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23331);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23337 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23337);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23380 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23380);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23411 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23411);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23412 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23412);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23512 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23512);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23590 (Judgement ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23590);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23592 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23592);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23642 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23642);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23722 (Arcane Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23722);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23860 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23860);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23880 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23880);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23881 (Bloodthirst Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23881);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23885 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23885);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23886 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23886);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23887 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23887);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23888 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23888);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23889 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23889);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23890 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23890);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23891 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23891);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23892 (Bloodthirst Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23892);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23893 (Bloodthirst Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23893);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23894 (Bloodthirst Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23894);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23922 (Shield Slam Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23922);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23923 (Shield Slam Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23923);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23924 (Shield Slam Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23924);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23925 (Shield Slam Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23925);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23952 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23952);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 23953 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23953);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23959 (Test Stab R50 Rank 8) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(23959);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 23968 (Throw Liquid Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23968);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23969 (Throw Liquid Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23969);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23970 (Throw Liquid Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23970);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23979 (Holy Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23979);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24099 (Poison Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24099);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24131 (Wyvern Sting Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24131);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24132 (Wyvern Sting Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24132);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24133 (Wyvern Sting Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24133);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24134 (Wyvern Sting Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24134);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24135 (Wyvern Sting Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24135);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24187 (Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24187);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24193 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24193);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24212 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24212);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 24213 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24213);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24239 (Hammer of Wrath Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24239);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24274 (Hammer of Wrath Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24274);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24275 (Hammer of Wrath Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24275);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24300 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24300);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24317 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24317);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24331 (Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24331);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24332 (Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24332);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24333 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24333);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24335 (Wyvern Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24335);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24336 (Wyvern Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24336);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24374 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24374);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24395 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24395);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24396 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24396);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24397 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24397);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24407 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24407);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24408 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24408);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24415 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24415);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24435 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24435);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24466 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24466);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24530 (Felfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24530);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24573 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24573);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24585 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24585);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24611 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24611);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24618 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24618);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24668 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24668);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24680 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24680);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24685 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24685);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24698 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24698);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24816 (Test Mortal Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24816);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24817 (Test Overpower Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24817);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24820 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24820);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24821 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24821);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24822 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24822);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24823 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24823);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24824 (Test Sinister Strike Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24824);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24825 (Test Backstab Rank 8) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(24825);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 24834 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24834);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24835 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24835);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24836 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24836);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24837 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24837);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24838 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24838);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24857 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24857);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24910 (Corruption of the Earth ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24910);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24942 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24942);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24957 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24957);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24974 (Insect Swarm Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24974);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24975 (Insect Swarm Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24975);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24976 (Insect Swarm Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24976);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24977 (Insect Swarm Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24977);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24993 (Acid Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24993);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24995 (Pyroblast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24995);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25004 (Throw Nightmare Object ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25004);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25021 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25021);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25025 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25025);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25028 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25028);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25030 (Shoot Rocket ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25030);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25031 (Shoot Missile ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25031);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25049 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25049);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25051 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25051);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25055 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25055);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25099 (Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25099);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25195 (Curse of Tongues ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25195);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 25208 (Rend Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25208);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25212 (Hamstring Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25212);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25225 (Sunder Armor Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25225);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25231 (Cleave Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25231);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25234 (Execute Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25234);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25236 (Execute Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25236);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25241 (Slam Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25241);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25242 (Slam Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25242);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25248 (Mortal Strike Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25248);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25251 (Bloodthirst Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25251);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25252 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25252);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25253 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25253);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25258 (Shield Slam Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25258);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25266 (Mocking Blow Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25266);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25269 (Revenge Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25269);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25273 (Intercept Stun Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25273);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25274 (Intercept Stun Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25274);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25286 (Heroic Strike Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25286);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25288 (Revenge Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25288);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25294 (Multi-Shot Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25294);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25295 (Serpent Sting Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25295);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25298 (Starfire Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25298);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25300 (Backstab Rank 9) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(25300);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 25304 (Frostbolt Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25304);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25306 (Fireball Rank 12) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25306);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25307 (Shadow Bolt Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25307);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25309 (Immolate Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25309);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25311 (Corruption Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25311);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 25345 (Arcane Missiles Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25345);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25346 (Arcane Missiles Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25346);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25363 (Smite Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25363);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25364 (Smite Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25364);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25367 (Shadow Word: Pain Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25367);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 25368 (Shadow Word: Pain Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25368);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 25372 (Mind Blast Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25372);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25375 (Mind Blast Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25375);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25379 (Mana Burn Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25379);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25380 (Mana Burn Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25380);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25384 (Holy Fire Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25384);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25387 (Mind Flay Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25387);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25424 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25424);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25439 (Chain Lightning Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25439);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25442 (Chain Lightning Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25442);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25446 (Starshards Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25446);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25454 (Earth Shock Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25454);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25457 (Flame Shock Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25457);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25460 (Touch of Weakness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25460);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25461 (Touch of Weakness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25461);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25464 (Frost Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25464);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25467 (Devouring Plague Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25467);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25470 (Hex of Weakness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25470);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25477 (Shadowguard Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25477);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25515 (Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25515);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25586 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25586);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25595 (Savage Pummel Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25595);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25671 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25671);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25710 (Heroic Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25710);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25712 (Heroic Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25712);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25755 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25755);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25756 (Purge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25756);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25759 (Video Camera 1 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25759);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25760 (Video Camera 2 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25760);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25761 (Video Camera 3 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25761);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25762 (Video Camera 4 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25762);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25779 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25779);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25805 (Soul Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25805);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25811 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25811);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25816 (Hex of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25816);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25821 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25821);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25902 (Holy Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25902);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25903 (Holy Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25903);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25907 (Spell Blasting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25907);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25911 (Holy Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25911);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25912 (Holy Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25912);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25913 (Holy Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25913);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25914 (Holy Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25914);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25991 (Poison Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25991);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26006 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26006);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26044 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26044);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26046 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26046);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26048 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26048);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26049 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26049);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26052 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26052);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26070 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26070);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26098 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26098);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26102 (Sand Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26102);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26121 (Stormcaller's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26121);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26141 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26141);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26143 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26143);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26180 (Wyvern Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26180);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26194 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26194);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26211 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26211);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26233 (Wyvern Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26233);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26282 (Shoot Tauren Rifleman ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26282);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26350 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26350);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26385 (Shadowburn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26385);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26458 (Shock Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26458);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26580 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26580);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26592 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26592);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26601 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26601);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26639 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26639);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26661 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26661);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26679 (Deadly Throw Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26679);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26693 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26693);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26748 (Wyvern Sting Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26748);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26839 (Garrote Rank 7) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(26839);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 26861 (Sinister Strike Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26861);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26862 (Sinister Strike Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26862);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26863 (Backstab Rank 10) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(26863);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 26864 (Hemorrhage Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26864);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26865 (Eviscerate Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26865);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26866 (Expose Armor Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26866);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26867 (Rupture Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26867);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26884 (Garrote Rank 8) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(26884);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 27001 (Shred Rank 6) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(27001);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 27002 (Shred Rank 7) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(27002);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 27005 (Ravage Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(27005);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 27014 (Raptor Strike Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27014);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27016 (Serpent Sting Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27016);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27018 (Viper Sting Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27018);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27019 (Arcane Shot Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27019);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27021 (Multi-Shot Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27021);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27022 (Volley Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27022);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27067 (Counterattack Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27067);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27068 (Wyvern Sting Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27068);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27069 (Wyvern Sting Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27069);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27138 (Exorcism Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27138);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27174 (Holy Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27174);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27175 (Holy Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27175);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27176 (Holy Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27176);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27180 (Hammer of Wrath Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27180);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27210 (Searing Pain Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27210);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27211 (Soul Fire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27211);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27213 (Hellfire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27213);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27215 (Immolate Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27215);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27216 (Corruption Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27216);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 27217 (Drain Soul Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27217);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27218 (Curse of Agony Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27218);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 27219 (Drain Life Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27219);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27220 (Drain Life Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27220);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27221 (Drain Mana Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27221);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27223 (Death Coil Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27223);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27224 (Curse of Weakness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27224);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 27226 (Curse of Recklessness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27226);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 27228 (Curse of the Elements Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27228);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 27229 (Curse of Shadow Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27229);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 27243 (Seed of Corruption Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27243);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27264 (Siphon Life Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27264);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27285 (Seed of Corruption Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27286 (Shadow Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27286);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27382 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27382);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27383 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27383);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27441 (Ambush Rank 7) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(27441);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 27499 (Crusader's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27499);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27555 (Shred ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27555);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27556 (Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27556);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27557 (Ferocious Bite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27557);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27565 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27565);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27567 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27567);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27580 (Mortal Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27580);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27584 (Hamstring Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27584);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27605 (Shadow Word: Pain Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27605);
	if( sp != NULL )
		sp->in_front_status = 0;

	// Spell 27611 (Eviscerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27611);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27613 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27613);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27615 (Kidney Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27615);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27626 (Purge Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27626);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27633 (Wing Clip Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27633);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27636 (Starshards ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27636);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27638 (Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27638);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27641 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27641);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27646 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27646);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27662 (Throw Cupid's Dart ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27662);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27737 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27737);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27760 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27760);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27794 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27794);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27808 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27808);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27812 (Void Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27812);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27814 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27814);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27820 (Mana Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27820);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27826 (Intercept ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27826);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27831 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27831);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27990 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27990);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27991 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27991);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27992 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27992);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27994 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27994);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28167 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28167);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28271 (Polymorph Rank 1: Turtle) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28271);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28272 (Polymorph Rank 1: Pig) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28272);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28293 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28293);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28301 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28301);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28310 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28310);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28311 (Slime Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28311);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28377 (Shadowguard Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28377);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28378 (Shadowguard Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28378);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28379 (Shadowguard Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28379);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28380 (Shadowguard Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28380);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28381 (Shadowguard Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28381);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28382 (Shadowguard Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28382);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28385 (Shadowguard Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28385);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28407 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28407);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28412 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28412);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28448 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28448);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28456 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28456);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28457 (Dark Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28457);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28478 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28478);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28479 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28479);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28522 (Icebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28522);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28526 (Icebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28526);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28598 (Touch of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28598);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28599 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28599);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28608 (Test Curse of Agony Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28608);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28750 (Blessing of the Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28750);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28796 (Poison Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28796);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28883 (Holy Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28883);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28900 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28900);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29058 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29058);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29124 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29124);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29155 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29155);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29168 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29168);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29183 (Polymorph Immunity ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29183);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29214 (Wrath of the Plaguebringer ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29214);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29228 (Flame Shock Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29228);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29310 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29310);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29317 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29317);
	if( sp != NULL )
		sp->in_front_status = 1;
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
			if(sp->EffectTriggerSpell[b] != 0 && dbcSpell.LookupEntryForced(sp->EffectTriggerSpell[b]) == NULL)
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
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = All_Seal_Groups_Combined | 8388608;
		sp = dbcSpell.LookupEntryForced( 20102 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = All_Seal_Groups_Combined | 8388608;
		sp = dbcSpell.LookupEntryForced( 20103 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = All_Seal_Groups_Combined | 8388608;
		sp = dbcSpell.LookupEntryForced( 20104 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = All_Seal_Groups_Combined | 8388608;
		sp = dbcSpell.LookupEntryForced( 20105 );
		if( sp != NULL )
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
		if( sp != NULL && sp->Id == 16164 )
			sp->procFlags = PROC_ON_SPELL_HIT | PROC_ON_SPELL_CRIT;

		/**********************************************************
		 *	Bloodlust
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 2825 );
		if( sp != NULL )
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
		if( sp != NULL )
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
		if( sp != NULL )
		{
			sp->EffectSpellGroupRelation[0] =  0xFFFFFFFF; // shaman spells. Guess that wraps them all 
			sp->EffectSpellGroupRelation_high[0] =  0xFFFFFFFF; // shaman spells. Guess that wraps them all 
		}

		/**********************************************************
		 *	Healing Focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16181 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16230 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16232 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16233 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16234 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] =  64 | 128 | 256;

		/**********************************************************
		 *	Improved Lightning shield 
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16261 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1024;
		sp = dbcSpell.LookupEntryForced( 16290 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1024;
		sp = dbcSpell.LookupEntryForced( 16291 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 1024;
		/**********************************************************
		 *	Tidal focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16179 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16214 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16215 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16216 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
		sp = dbcSpell.LookupEntryForced( 16217 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;

		/**********************************************************
		 *	Enhancing Totems
		 **********************************************************/
		group_relation_shaman_enhancing_totems = 0x00010000 | 0x00020000; // <--- GROUPING
		sp = dbcSpell.LookupEntryForced( 16259 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_enhancing_totems;
		sp = dbcSpell.LookupEntryForced( 16295 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_enhancing_totems;

		/**********************************************************
		 *	Elemental Fury - ! Not finished !
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16089 ); 
		if( sp != NULL )
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
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;
		sp = dbcSpell.LookupEntryForced( 16205 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;
		sp = dbcSpell.LookupEntryForced( 16206 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;
		sp = dbcSpell.LookupEntryForced( 16207 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;
		sp = dbcSpell.LookupEntryForced( 16208 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_restorative_totems;

		/**********************************************************
		 *	Ancestral healing proc spell
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16177 );
		if( sp != NULL )
			sp->rangeIndex = 4;
		sp = dbcSpell.LookupEntryForced( 16236 );
		if( sp != NULL )
			sp->rangeIndex = 4;
		sp = dbcSpell.LookupEntryForced( 16237 );
		if( sp != NULL )
			sp->rangeIndex = 4;

		/**********************************************************
		 *	Totems grouping
		 **********************************************************/
		group_relation_shaman_totems |= 0x00000008 | 0x00000010 | 0x00001000 | 0x00080000 | 0x20000000;
		
		/**********************************************************
		 *	Totemic focus
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16173 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		sp = dbcSpell.LookupEntryForced( 16222 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		sp = dbcSpell.LookupEntryForced( 16223 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		sp = dbcSpell.LookupEntryForced( 16224 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		sp = dbcSpell.LookupEntryForced( 16225 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_totems;
		
		/**********************************************************
		 *	Lightning - Grouping
		 **********************************************************/
		group_relation_shaman_lightning = 0x00000001 | 0x00000002;
		
		/**********************************************************
		 *	Call of Thunder
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16041 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16117 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16118 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16119 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16120 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_lightning;
		
		/**********************************************************
		 *	Shock Grouping
		 **********************************************************/
		group_relation_shaman_shock = 0x00100000 | 0x10000000 | 0x80000000;

		/**********************************************************
		 *	Convection
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16039 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16109 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16110 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16111 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16112 ); 
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;

		/**********************************************************
		 *	Concussion
		 **********************************************************/
		sp = dbcSpell.LookupEntryForced( 16035 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16105 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16106 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16107 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;
		sp = dbcSpell.LookupEntryForced( 16108 );
		if( sp != NULL )
			sp->EffectSpellGroupRelation[0] = group_relation_shaman_shock | group_relation_shaman_lightning;

		//wrath of air totem targets sorounding creatures instead of us
		sp = dbcSpell.LookupEntryForced( 2895 );
		if( sp != NULL )
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}

		sp = dbcSpell.LookupEntryForced( 20608 ); //Reincarnation
		if( sp != NULL )
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
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 13481;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19548 ); //Tame Ice Claw Bear
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 19597;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19674 ); //Tame Large Crag Boar
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 19677;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19687 ); //Tame Snow Leopard
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 19676;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 		
 		sp = dbcSpell.LookupEntryForced( 19688 ); //Tame Adult Plainstrider
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 19678;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 		
 		sp = dbcSpell.LookupEntryForced( 19689 ); //Tame Prairie Stalker
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 19679;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19692 ); //Tame Swoop
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 19680;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19693 ); //Tame Webwood Lurker
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 19684;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19694 ); //Tame Dire Mottled Boar
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 19681;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19696 ); //Tame Surf Crawler
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 19682;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19697 ); //Tame Armored Scorpid
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 19683;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19699 ); //Tame Nightsaber Stalker
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 19685;
 			sp->EffectAmplitude[1] = 20000;
 			sp->EffectMiscValue[2] = 1;
 			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
 		}
 
 		sp = dbcSpell.LookupEntryForced( 19700 ); //Tame Strigid Screecher
 		if(sp != NULL)
 		{
 			sp->EffectTriggerSpell[1] = 19686;
 			sp->EffectAmplitude[1] = 20000;
			sp->EffectMiscValue[2] = 1;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
		}			

		//////////////////////////////////////////
		// OTHER								//
		//////////////////////////////////////////
	}
	//Settings for special cases
	QueryResult * resultx = WorldDatabase.Query("SELECT * FROM spell_coef_override");
	if( resultx != NULL )
	{
		uint32 spellid;
		do 
		{
			Field * f;
			f = resultx->Fetch();
			spellid = f[0].GetUInt32();
			SpellEntry * sp = dbcSpell.LookupEntryForced( spellid );
			if( sp != NULL )
			{
				sp->Dspell_coef_override = f[1].GetFloat();
				sp->OTspell_coef_override = f[2].GetFloat();
				sp->AP_coef_override = f[3].GetFloat();
				sp->RAP_coef_override = f[4].GetFloat();
			}
			else
			{
				if(Config.MainConfig.GetBoolDefault("Server", "CleanDatabase", false))
				{
					WorldDatabase.Query("DELETE FROM spell_coef_override where id = '%u'", spellid);
				}
				Log.Warning("SpellCoefOverride", "Has nonexistant spell %u.", spellid);
			}
			spellid = 0;
		} while( resultx->NextRow() );
		delete resultx;
	}

	
	/////////////////////////////////////////////////////////////////
	//FORCER CREATURE SPELL TARGETING
	//////////////////////////////////////////////////////////////////
	QueryResult * resultfcst = WorldDatabase.Query("SELECT * FROM spell_forced_targets");
	if( resultfcst != NULL )
	{
		do 
		{
			Field * f;
			f = resultfcst->Fetch();
			sp = dbcSpell.LookupEntryForced( f[0].GetUInt32() );
			if( sp )
				sp->forced_creature_target = f[1].GetUInt32();
			} while( resultfcst->NextRow() );
		delete resultfcst;
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
