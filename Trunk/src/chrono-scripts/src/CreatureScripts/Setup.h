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

#include <string>
#ifndef INSTANCE_SCRIPTS_SETUP_H
#define INSTANCE_SCRIPTS_SETUP_H

//Instances
void SetupScholomanceBosses(ScriptMgr * mgr);
void SetupStratholmeBosses(ScriptMgr * mgr);
void SetupUldamanBosses(ScriptMgr * mgr);
void SetupBlackfathomDeeps(ScriptMgr * mgr);
void SetupDeadmines(ScriptMgr * mgr);
void SetupMaraudon(ScriptMgr * mgr);
void SetupRagefireChasm(ScriptMgr * mgr);
void SetupRazorfenDowns(ScriptMgr * mgr);
void SetupScarletMonastery(ScriptMgr * mgr);
void SetupShadowfangKeep(ScriptMgr * mgr);
void SetupStockades(ScriptMgr * mgr);
void SetupWailingCaverns(ScriptMgr * mgr);
void SetupScholomanceTrash(ScriptMgr * mgr);
void SetupStratholmeTrash(ScriptMgr * mgr);

//Raids
void SetupMoltenCoreTrash(ScriptMgr * mgr);
void SetupMoltenCore(ScriptMgr * mgr);
void SetupOnyxia(ScriptMgr * mgr);
void SetupWorldBosses(ScriptMgr * mgr);

//Zones
void SetupAlteracMountainsNPC(ScriptMgr * mgr);
void SetupAzsharaNPC(ScriptMgr * mgr);
void SetupDeadwindPassNPC(ScriptMgr * mgr);
void SetupDurotarNPC(ScriptMgr * mgr);
void SetupElwynForestNPC(ScriptMgr * mgr);
void SetupMulgoreNPC(ScriptMgr *mgr);
void SetupStormwindNPC(ScriptMgr *mgr);

struct ScriptSpell
{
	uint32 normal_spellid;
	uint32 heroic_spellid;
	uint32 timer;
	uint32 time;
	uint32 chance;
	uint32 target;
	uint32 phase;
};

enum SPELL_TARGETS
{
	SPELL_TARGET_SELF,
	SPELL_TARGET_CURRENT_ENEMY,
	SPELL_TARGET_RANDOM_PLAYER,
	SPELL_TARGET_SUMMONER,
	SPELL_TARGET_RANDOM_PLAYER_POSITION,
	SPELL_TARGET_GENERATE, // this will send null as target
	SPELL_TARGET_LOWEST_THREAT,
	SPELL_TARGET_CUSTOM,
};

struct SP_AI_Spell{
	SpellEntry *info;		// spell info
	char targettype;		// 0-self , 1-attaking target, ....
	bool instant;			// does it is instant or not?
	float perctrigger;		// % of the cast of this spell in a total of 100% of the attacks
	int attackstoptimer;	// stop the creature from attacking
	int soundid;			// sound id from DBC
	std::string speech;		// text displaied when spell was casted
	uint32 cooldown;		// spell cooldown
	uint32 casttime;		// "time" left to cast spell
	uint32 reqlvl;			// required level ? needed?
	float hpreqtocast;		// ? needed?
	float mindist2cast;		// min dist from caster to victim to perform cast (dist from caster >= mindist2cast)
	float maxdist2cast;		// max dist from caster to victim to perform cast (dist from caster <= maxdist2cast)
	int minhp2cast;			// min hp amount of victim to perform cast on it (health >= minhp2cast)
	int maxhp2cast;			// max hp amount of victim to perform cast on it (health <= maxhp2cast)
};

enum
{
	TARGET_SELF,
	TARGET_VARIOUS,
	TARGET_ATTACKING,
	TARGET_DESTINATION,
	TARGET_SOURCE,
	TARGET_RANDOM_FRIEND,	// doesn't work yet
	TARGET_RANDOM_SINGLE,
	TARGET_RANDOM_DESTINATION,

	//.....add
};

#endif
