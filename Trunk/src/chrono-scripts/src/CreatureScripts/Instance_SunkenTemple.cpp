/*
 * Sun++ Scripts for Aspire MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"

/************************************************************************/
/* Instance_SunkenTemple.cpp Script		                               			     */
/************************************************************************/

/* TODO:
	Hakkari Bloodkeeper summon demon - hakkar event
	Shade of hakkar - hakkar event

	Nightmare Suppressor - has some server scripts, not quite sure what it should do
*/

//trash defines
#define CN_ATALAI_DEATHWALKER			5271
#define CN_ATALAI_HIGH_PRIEST			5273
#define CN_ATALAI_WARRIOR				5256
#define CN_ATALAI_WITCH_DOCTOR			5259
#define CN_DEEP_LURKER					8384
#define CN_HAKKARI_BLOODKEEPER			8438
#define CN_HAKKARI_FROSTWING			5291
#define CN_HAKKARI_SAPPER				8336
#define CN_HUKKU_IMP					8658
#define CN_HUKKU_SUCCUBUS				8657
#define CN_NIGHTMARE_SCALEBANE			5277
#define CN_NIGHTMARE_SUPPRESSOR			8497
#define CN_NIGHTMARE_WANDERER			5283
#define CN_SATURATED_OOZE				5228
#define CN_SHADE_OF_HAKKAR				8440
#define CN_UNLIVING_ATALAI				5267

//sunken spells defines
#define SS_FEAR							12096
#define SS_SHADOW_WORD_PAIN				11639
#define SS_ATALAI_DEATHWALKER_SPIRIT	12095
#define SS_HEAL							12039
#define SS_RENEW						8362
#define SS_SHADOW_BOLT					9613
#define SS_SHADOW_SHIELD				12040
#define SS_SUMMON_ATALAI_SKELETON		12151
#define SS_REND							13445
#define SS_STRIKE						13446
#define SS_CHAIN_LIGHTNING				12058
#define SS_HEALING_WAVE					11986
#define SS_HEX							11641
#define SS_LIGHTNING_BOLT				9532
#define SS_TRAMPLE						5568
#define SS_CORRUPTION5					11671
#define SS_FROST_NOVA					11831
#define SS_FROSTBOLT_VOLLEY				8398
#define SS_SWOOP						5708
#define SS_MANA_BURN					11981
#define SS_WING_FLAP					11019
#define SS_FIRE_SHIELD4					11770
#define SS_FIREBOLT6					11762
#define SS_LASH_OF_PAIN5				11779
#define SS_SEDUCTION					6358
#define SS_IMPROVED_BLOCKING			3248
#define SS_PIERCE_ARMOR					12097
#define SS_SUMMON_OOZELING				12250
#define SS_SUMMON_OOZELING_DEATH		12018
#define SS_FRENZY						8269
#define SS_THRASH						3391

//classes
class AtalaiDeathwalkerAI: public MoonScriptCreatureAI
{
	SpellDesc *sum_spirit;

	MOONSCRIPT_FACTORY_FUNCTION(AtalaiDeathwalkerAI, MoonScriptCreatureAI);
	AtalaiDeathwalkerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_FEAR, Target_RandomPlayerNotCurrent, 30, 1.5, 20);
		AddSpell(SS_SHADOW_WORD_PAIN, Target_RandomPlayer, 100, 0, 18);
		sum_spirit = AddSpell(SS_ATALAI_DEATHWALKER_SPIRIT, Target_Self, 0, 0, 0);
	}

	void OnDied(Unit *pKiller)
	{
		CastSpell(sum_spirit);
		ParentClass::OnDied(pKiller);
	}
};

class AtalaiHighPriestAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AtalaiHighPriestAI, MoonScriptCreatureAI);
	AtalaiHighPriestAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_HEAL, Target_Self, 20, 3, 10);
		AddSpell(SS_RENEW, Target_RandomFriendly, 20, 0, 10);
		AddSpell(SS_SHADOW_BOLT, Target_Current, 20, 3, 0);
		AddSpell(SS_SHADOW_SHIELD, Target_Self, 10, 3, 100);
		AddSpell(SS_SUMMON_ATALAI_SKELETON, Target_Self, 40, 1, 100);
	}
};

class AtalaiWarriorAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AtalaiWarriorAI, MoonScriptCreatureAI);
	AtalaiWarriorAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_REND, Target_Current, 50, 0, 15);
		AddSpell(SS_STRIKE, Target_Current, 50, 0, 10);
	}
};

class AtalaiWitchDoctorAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AtalaiWitchDoctorAI, MoonScriptCreatureAI);
	AtalaiWitchDoctorAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_CHAIN_LIGHTNING, Target_Current, 10, 3, 20);
		AddSpell(SS_HEALING_WAVE, Target_Self, 30, 3, 10);
		AddSpell(SS_HEX, Target_RandomPlayerNotCurrent, 30, 0, 20);
		AddSpell(SS_LIGHTNING_BOLT, Target_Current, 30, 3, 0);
	}
};

class DeepLurkerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeepLurkerAI, MoonScriptCreatureAI);
	DeepLurkerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_TRAMPLE, Target_Self, 50, 0, 10);
	}
};

class HakkariBloodkeeperAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HakkariBloodkeeperAI, MoonScriptCreatureAI);
	HakkariBloodkeeperAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_CORRUPTION5, Target_RandomPlayer, 50, 0, 18);
		AddSpell(SS_SHADOW_BOLT, Target_RandomPlayer, 20, 3, 9);
	}
};

class HakkariFrostwingAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HakkariFrostwingAI, MoonScriptCreatureAI);
	HakkariFrostwingAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_FROST_NOVA, Target_Current, 20, 0, 20);
		AddSpell(SS_FROSTBOLT_VOLLEY, Target_Current, 20, 3, 9);
		AddSpell(SS_SWOOP, Target_Current, 20, 0, 10);
	}
};

class HakkariSapperAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HakkariSapperAI, MoonScriptCreatureAI);
	HakkariSapperAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_MANA_BURN, Target_RandomPlayerNotCurrent, 50, 3, 20);
		AddSpell(SS_WING_FLAP, Target_Current, 20, 0, 20);
	}
};

class HukkuImpAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HukkuImpAI, MoonScriptCreatureAI);
	HukkuImpAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_FIRE_SHIELD4, Target_RandomFriendly, 100, 0, 120);
		AddSpell(SS_FIREBOLT6, Target_Current, 100, 2.5, 0);
	}
};

class HukkuSuccubusAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HukkuSuccubusAI, MoonScriptCreatureAI);
	HukkuSuccubusAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_LASH_OF_PAIN5, Target_Current, 100, 0, 12);
		AddSpell(SS_SEDUCTION, Target_RandomPlayerNotCurrent, 20, 1.5, 30);
	}
};

class NightmareScalebaneAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(NightmareScalebaneAI, MoonScriptCreatureAI);
	NightmareScalebaneAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_IMPROVED_BLOCKING, Target_Self, 30, 0, 20);
	}
};

class NightmareSuppressorAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(NightmareSuppressorAI, MoonScriptCreatureAI);
	NightmareSuppressorAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
};

class NightmareWandererAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(NightmareWandererAI, MoonScriptCreatureAI);
	NightmareWandererAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_STRIKE, Target_Current, 30, 0, 15);
		AddSpell(SS_PIERCE_ARMOR, Target_Current, 30, 0, 20);
	}
};

class SaturatedOozeAI: public MoonScriptCreatureAI
{
	SpellDesc *sum_ooze_death;

	MOONSCRIPT_FACTORY_FUNCTION(SaturatedOozeAI, MoonScriptCreatureAI);
	SaturatedOozeAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_SUMMON_OOZELING, Target_Self, 10, 0, 15);
		sum_ooze_death = AddSpell(SS_SUMMON_OOZELING_DEATH, Target_Self, 0, 0, 0);
	}

	void OnDied(Unit *pKiller)
	{
		CastSpell(sum_ooze_death);
		ParentClass::OnDied(pKiller);
	}
};

class ShadeOfHakkarAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadeOfHakkarAI, MoonScriptCreatureAI);
	ShadeOfHakkarAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
};

class UnlivingAtalaiAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(UnlivingAtalaiAI, MoonScriptCreatureAI);
	UnlivingAtalaiAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SS_FRENZY, Target_Self, 100, 0, 120);
		AddSpell(SS_THRASH, Target_Current, 15, 0, 10);
	}
};


void SetupSunkenTemple(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_ATALAI_DEATHWALKER, &AtalaiDeathwalkerAI::Create);
	mgr->register_creature_script(CN_ATALAI_HIGH_PRIEST, &AtalaiHighPriestAI::Create);
	mgr->register_creature_script(CN_ATALAI_WARRIOR, &AtalaiWarriorAI::Create);
	mgr->register_creature_script(CN_ATALAI_WITCH_DOCTOR, &AtalaiWitchDoctorAI::Create);
	mgr->register_creature_script(CN_DEEP_LURKER, &DeepLurkerAI::Create);
	mgr->register_creature_script(CN_HAKKARI_BLOODKEEPER, &HakkariBloodkeeperAI::Create);
	mgr->register_creature_script(CN_HAKKARI_FROSTWING, &HakkariFrostwingAI::Create);
	mgr->register_creature_script(CN_HUKKU_IMP, &HukkuImpAI::Create);
	mgr->register_creature_script(CN_HUKKU_SUCCUBUS, &HukkuSuccubusAI::Create);
	mgr->register_creature_script(CN_NIGHTMARE_SCALEBANE, &NightmareScalebaneAI::Create);
	mgr->register_creature_script(CN_NIGHTMARE_SUPPRESSOR, &NightmareSuppressorAI::Create);
	mgr->register_creature_script(CN_NIGHTMARE_WANDERER, &NightmareWandererAI::Create);
	mgr->register_creature_script(CN_SATURATED_OOZE, &SaturatedOozeAI::Create);
	mgr->register_creature_script(CN_SHADE_OF_HAKKAR, &ShadeOfHakkarAI::Create);
}
