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
#include "Instance_DireMaul.h"

/* TODO:	Gordok Brute: pummel melee range casting player
			Gordok Brute: put away a club on enrage
			Gordok Warlock: summon doomguard on spawn
			Phase lasher: implement "phasing"
			Warpwood Guardian: pummel melee range casting player
			Wildspawn Shadowstalker: is stealth and detection handled the right way?
			Warpwood pot

			Lethrendis enlarge on her imp needs testing
			Zevrim's sacrifice need testing (probably rework)

			Guards (moldar etc.) should shout for help.
			Shield charge spell not working.
			Slip'kik Ice block not working.
			King of gordok spell.
			Gossips of trash.

			Tendris - should call big trees to his help when attacked
			Ferra - 100 yards aggro range
			Make sure Immol'thars eyes stick to one target.
			Prince - summon (teleport spell), support for cunterspell

			Isalien = D2 boss
*/

/************************************************************************/
/* Instance_DireMaul.cpp BOSS scripts									*/
/************************************************************************/
//East DM
class OldIronbarkGossip: public GossipScript
{
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 6694, Plr);
		Menu->SendTo(Plr);
	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		GossipHello(pObject, Plr, true);
	}
};

#define IRONBARK_WAYPOINTS		4
static Location IronbarkWaypoints[IRONBARK_WAYPOINTS] = {
	{-49.0, -266.0, -57.6, 0},
	{10.0, -288.0, -52.3, 0},
	{51.0, -270.0, -53.0, 0},
	{120.0, -278.0, -55.8, 0},
};

class OldIronbarkRedeemedAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(OldIronbarkRedeemedAI, MoonScriptCreatureAI);
	OldIronbarkRedeemedAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{ }

	void OnReachWP(uint32 wpid, bool forward) {
		if(wpid == IRONBARK_WAYPOINTS) {
			StopWaypointMovement();
			Emote("My strength wanes, mortal. I have done as promissed, the way is clear. I now return to whence I came.", Text_Say, 0);
			GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(132.033, -278.879, -56.2542, 176907);
			if(door) {
				door->SetState(0);
			}
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			_unit->Die(_unit, 10000000, 5);
			Despawn(5000);
		}
		ParentClass::OnReachWP(wpid, forward);
	}
};

class OldIronbarkRedeemedGossip: public GossipScript
{
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 6695, Plr);
		Menu->AddItem(0, "Thank you, Ironbark. We are ready for you to open the door.", 2);
		Menu->SendTo(Plr);
	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
  		if(IntId != 2) {
			GossipHello(pObject, Plr, true);
		}
		else {
			if(pObject) {
				Creature *pCreature = (Creature*)pObject;
				MoonScriptCreatureAI *ironbark = (MoonScriptCreatureAI*)((Creature*)pObject)->GetScript();
				if(ironbark) {
					ironbark->Emote("As you wish...", Text_Say, 0);
					for(int i=0; i<IRONBARK_WAYPOINTS; i++) {
						ironbark->AddWaypoint(ironbark->CreateWaypoint(i+1, 0, Flag_Run, IronbarkWaypoints[i]));
					}
					pCreature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
					pCreature->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
					pCreature->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
					pCreature->GetAIInterface()->setWaypointToMove(0);	
					pCreature->GetAIInterface()->StopMovement(0);
				}
			}
			Plr->Gossip_Complete();
		}
	}
};

//Hydrospawn & Massive geyser
class HydrospawnAI: public MoonScriptCreatureAI
{
	SpellDesc *summon;
	bool is_summed;

	MOONSCRIPT_FACTORY_FUNCTION(HydrospawnAI, MoonScriptCreatureAI);
	HydrospawnAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_RIPTIDE, Target_Current, 20, 0, 10);
		AddSpell(DM_SUBMERSION, Target_Self, 20, 0, 30);
		AddSpell(DM_MASSIVE_GEYSER, Target_RandomPlayer, 20, 1.5, 10);
		summon = AddSpell(DM_SUMMON_HYDROLING, Target_Current, 0, 0, 0);
	}
	void OnCombatStart(Unit *pTarget) {
		is_summed = false;
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate() {
		if(!is_summed && (GetHealthPercent() < 50.0)) {
			is_summed = true;
			CastSpell(summon);
		}
		ParentClass::AIUpdate();
	}
};

class MassiveGeyserAI: public MoonScriptCreatureAI
{
	SpellDesc *geyser;
	MOONSCRIPT_FACTORY_FUNCTION(MassiveGeyserAI, MoonScriptCreatureAI);
	MassiveGeyserAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		geyser = AddSpell(DM_WATER, Target_Self, 0, 0, 0);
	}

	void OnCombatStart(Unit *pTarget) {
		CastSpellNowNoScheduling(geyser);
		Despawn(10000);
		ParentClass::OnCombatStart(pTarget);
	}
};

//Lethrendis & big imp
class LethtendrisAI: public MoonScriptCreatureAI
{
	Unit *bigimp;

	MOONSCRIPT_FACTORY_FUNCTION(LethtendrisAI, MoonScriptCreatureAI);
	LethtendrisAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		bigimp = NULL;
//		this spell seems to be bugged
//		AddSpell(DM_CURSE_OF_THORNS, Target_RandomPlayer, 50, 2, 60);
		AddSpell(DM_CURSE_OF_TONGUES, Target_RandomPlayerNotCurrent, 30, 0, 16);
		AddSpell(DM_LETH_IMMOLATE, Target_RandomPlayer, 30, 2, 5);
		AddSpell(DM_LETH_SHADOW_BOLT_VOLLEY, Target_RandomPlayer, 20, 0, 15);
		AddSpell(DM_VOID_BOLT, Target_Current, 15, 4, 10);
	}

	void OnCombatStart(Unit *pTarget) {
		if(!bigimp)
			bigimp = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), CN_PIMGIB);
		if(!bigimp)
			Emote("Big imp not found!");
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate() {
		if(IsAlive()) {
			if(bigimp && bigimp->isAlive() && !bigimp->HasAura(DM_ENLARGE) && (RandomUInt(100) < 50)) {
				bigimp->CastSpell(bigimp->GetGUID(), DM_ENLARGE, true);
//				_unit->CastSpell(bigimp->GetGUID(), DM_ENLARGE, true);
			}
		}
		ParentClass::AIUpdate();
	}
};

class BigImpAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BigImpAI, MoonScriptCreatureAI);
	BigImpAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_BLAST_WAVE, Target_Self, 30, 0, 15);
		AddSpell(DM_HELL_FIREBALL, Target_Current, 20, 3, 0);
		AddSpell(DM_FLAME_BUFFET, Target_Current, 30, 0, 5);
//		AddSpell(DM_ENLARGE, Target_Self, 10, 2, 30);
	}
};

class ZevrimThornhoofAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ZevrimThornhoofAI, MoonScriptCreatureAI);
	ZevrimThornhoofAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_INTENSE_PAIN, Target_Self, 30, 1, 10);
//		AddSpell(DM_SACRIFICE, Target_RandomPlayer, 30, 1, 10);
	}
	void OnDied(Unit *pKiller) {
		Creature *Ironbark = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-70.0f, -283.0f, -57.0f, CN_OLD_IRONBARK);
		if(Ironbark) {
			Ironbark->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "At last... Freed from his cursed grasp!");
			Ironbark->Despawn(0, 0);
		}
		SpawnCreature(CN_OLD_IRONBARK_REDEEMED, -70.0f, -283.0f, -57.6f, 4.08f);
		ParentClass::OnDied(pKiller);
	}
};

//pusillin has several places to stop at, way to each place consists of 3 waypoints
#define PUSILLIN_WAYPOINTS		4
static Location PusillinWaypoints[PUSILLIN_WAYPOINTS][3] = {
	{
		{-160.0, -201.0, -4.0, 0},
		{-157.0, -235.0, -4.0, 0},
		{-155.0, -275.0, -4.0, 0}
	},
	{
		{-150.0, -354.0, -4.0, 0},
		{0.0, -354.0, -4.0, 0},
		{113.0, -354.0, -4.0, 0}
	},
	{
		{110.0, -539.0, -11.0, 0},
		{52.0, -559.0, -19.0, 0},
		{49.0, -651.0, -25.0, 0}
	},
	{
		{6.0, -707.0, -25.0, 0},
		{8.0, -668.0, -12.64, 0},
		{13.0, -706.0, -12.64, 0},
	},
};

class PusillinAI: public MoonScriptCreatureAI
{
	SpellDesc *spirit;
	bool imps_spawned;
	int run_phase;

	MOONSCRIPT_FACTORY_FUNCTION(PusillinAI, MoonScriptCreatureAI);
	PusillinAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_PUS_BLAST_WAVE, Target_Self, 30, 0, 20);
		AddSpell(DM_PUS_FIRE_BLAST, Target_Current, 30, 0, 10);
		AddSpell(DM_PUS_FIREBALL, Target_Current, 50, 3, 1);
		spirit = AddSpell(DM_SPIRIT_OF_RUNN_TUM, Target_Self, 0, 0, 0);
		imps_spawned = false;
		run_phase = 1;
	}

	void OnCombatStart(Unit *pTarget) {
		CastSpellNowNoScheduling(spirit);
		//spawn 5 wildspawn imps to assist pusilin
		if(!imps_spawned) {
			imps_spawned = true;
			for(int i=0; i<5; i++)
				SpawnCreature(CN_WILDSPAWN_IMP);
		}
		ParentClass::OnCombatStart(pTarget);
	}

	void OnReachWP(uint32 wpid, bool forwards) {
		if(wpid == 3) {
			//gossip flag
			_unit->SetUInt64Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
		}
		ParentClass::OnReachWP(wpid, forwards);
	}

public:
	int GetRunPhase() {
		return run_phase;
	}
	void SetRunPhase(int phase) {
		run_phase = phase;
	}
};

struct _PusillinChat {
	int textid;
	char itemtext[100];
	char saytext[100];
};

struct _PusillinChat PusillinChat[] = {
	{6877, "Game? Are you crazy?", "If you want the key, you'll have to catch me!"},
	{6878, "Why you little...", "Chase me if you dare! I run without a care!"},
	{6879, "Mark my words, I will catch you imp. And when I do!", "Why would you ever want to harm me!? Come. Friends we can be!"},
	{6880, "DIE!", "DIE?! You make Pusillin cry!"},
	{6881, "Prepare to meet your maker.", "Say hello to my little friends."}
};


class PusillinGossip: public GossipScript
{
	int phase;
	PusillinAI *pusillin;

	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(pObject && ((Creature*)pObject)->GetScript()) {
			pusillin = (PusillinAI*)((Creature*)pObject)->GetScript();
			if(pusillin)
				phase = pusillin->GetRunPhase();
			else
				phase = 1;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), PusillinChat[phase-1].textid, Plr);
			Menu->AddItem(0, PusillinChat[phase-1].itemtext, 1);
			Menu->SendTo(Plr);
		}
	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
  		if(IntId != 1) {
			GossipHello(pObject, Plr, true);
		}
		else {
			if(pusillin) {
				Creature *pCreature = (Creature*)pObject;
				pCreature->GetAIInterface()->deleteWaypoints();
				pusillin->Emote(PusillinChat[phase-1].saytext, Text_Say, 0);
				if(phase == 5) {
					pCreature->SetFaction(90);
					pCreature->GetAIInterface()->SetAIState(STATE_IDLE);
				}
				else {
					for(int i=0; i<3; i++) {
						pusillin->AddWaypoint(pusillin->CreateWaypoint(i+1, 0, Flag_Run, PusillinWaypoints[phase-1][i]));
					}
					pCreature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
					pCreature->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
					pCreature->GetAIInterface()->setWaypointToMove(0);	
					pCreature->GetAIInterface()->StopMovement(0);
					pusillin->SetRunPhase(phase+1);
				}
			}
			Plr->Gossip_Complete();
		}
	}
};

#define ALZZIN_IMPS_WAYPOINTS		3
static Location AlzzinImpWaypoints[ALZZIN_IMPS_WAYPOINTS] = {
	{251.0f, -348.0f, -102.0f, 0},
	{253.0f, -403.0f, -117.0f, 0},
	{0, 0, 0},
};

class AlzzinTheWildshaperAI: public MoonScriptBossAI
{
	SpellDesc *wither, *enervate, *direwolf, *viciousbite, *mangle, *treeform, *disarm, *knockaway, *wildregen;
	int32 timer;
	bool summ_imps;

	MOONSCRIPT_FACTORY_FUNCTION(AlzzinTheWildshaperAI, MoonScriptBossAI);
	AlzzinTheWildshaperAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
	{
		ApplyAura(AW_THORNS);
		wither = AddSpell(AW_WITHER, Target_RandomPlayer, 30, 1.5, 10);
		wither->mEnabled = false;
		enervate = AddSpell(AW_ENERVATE, Target_RandomPlayerNotCurrent, 15, 1.5, 15);
		enervate->mEnabled = false;
		direwolf = AddSpell(AW_DIRE_WORLF, Target_Self, 0, 0, 0);
		viciousbite = AddSpell(AW_VICIOUS_BITE, Target_Current, 30, 0, 5);
		viciousbite->mEnabled = false;
		mangle = AddSpell(AW_MANGLE, Target_Current, 30, 0, 10);
		mangle->mEnabled = false;
		treeform = AddSpell(AW_TREE_FORM, Target_Self, 0, 0, 0);
		disarm = AddSpell(AW_DISARM, Target_Current, 20, 0, 5);
		disarm->mEnabled = false;
		knockaway = AddSpell(AW_KNOCK_AWAY, Target_Current, 20, 0, 15);
		knockaway->mEnabled = false;
		wildregen = AddSpell(AW_WILD_REGEN, Target_Self, 0, 3, 0);
	}
	void OnCombatStart(Unit *pTarget) {
		summ_imps = false;
		wither->mEnabled = true;
		enervate->mEnabled = true;
		SetPhase(1);
		timer = AddTimer(10000);
		ParentClass::OnCombatStart(pTarget);
	}
	void AIUpdate() {
		if(!summ_imps && GetHealthPercent() < 50) {
			summ_imps = true;

			GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(256.6f, -406.0f, -119.9f, 177220);
			if(door)
				door->SetState(0);

			MoonScriptCreatureAI *imp;

			int i;
			int j;

			AlzzinImpWaypoints[ALZZIN_IMPS_WAYPOINTS-1].x = _unit->GetPositionX();
			AlzzinImpWaypoints[ALZZIN_IMPS_WAYPOINTS-1].y = _unit->GetPositionY();
			AlzzinImpWaypoints[ALZZIN_IMPS_WAYPOINTS-1].z = _unit->GetPositionZ();
			for(i=1 ; i<=20; i++) {
				imp = SpawnCreature(CN_WILDSPAWN_IMP, 271.0f+RandomFloat(10)-10, -348.0f+RandomFloat(10)-10, -102);
				if(imp) {
					for(j=0; j<ALZZIN_IMPS_WAYPOINTS; j++) {
						imp->AddWaypoint(CreateWaypoint(j+1, 0, Flag_Run, AlzzinImpWaypoints[j]));
					}
				}
			}
		}
		if(IsTimerFinished(timer)) {
			ResetTimer(timer, 10000);

			int32 phase = GetPhase();
			switch(phase) {
				case 1: 
					wither->mEnabled = false;
					enervate->mEnabled = false;
					break;
				case 2:
					_unit->RemoveAura(AW_DIRE_WORLF);
					viciousbite->mEnabled = false;
					mangle->mEnabled = false;
					break;
				case 3:
					_unit->RemoveAura(AW_TREE_FORM);
					disarm->mEnabled = false;
					knockaway->mEnabled = false;
					break;
			}
			//get random phase different than current one
			phase = ((phase + RandomUInt(1)) % 3) + 1;
			SetPhase(phase);
			switch(phase) {
				case 1: 
					wither->mEnabled = true;
					enervate->mEnabled = true;
					break;
				case 2:
					CastSpellNowNoScheduling(direwolf);
					viciousbite->mEnabled = true;
					mangle->mEnabled = true;
					break;
				case 3:
					CastSpellNowNoScheduling(treeform);
					disarm->mEnabled = true;
					knockaway->mEnabled = true;
					if(GetHealthPercent() < 50.0)
						CastSpell(wildregen);
					break;
			}
		}
		ParentClass::AIUpdate();
	}
	void OnCombatStop(Unit *pTarget) {
		RemoveTimer(timer);
		int32 phase = GetPhase();
		switch(phase) {
			case 1: 
				wither->mEnabled = false;
				enervate->mEnabled = false;
				break;
			case 2:
				_unit->RemoveAura(AW_DIRE_WORLF);
				viciousbite->mEnabled = false;
				mangle->mEnabled = false;
				break;
			case 3:
				_unit->RemoveAura(AW_TREE_FORM);
				disarm->mEnabled = false;
				knockaway->mEnabled = false;
				break;
		}
		SetPhase(1);
	}
};

//DM North
#define TRIBUTE_RUN_TRIGGER			15606
class TributeRunTriggerAI: public MoonScriptCreatureAI
{
	int liveGuards;
	bool cptDead;
	MOONSCRIPT_FACTORY_FUNCTION(TributeRunTriggerAI, MoonScriptCreatureAI);
	TributeRunTriggerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		liveGuards = 0;
		cptDead = false;
	}
	int RemoveGuard() {
		if(liveGuards>0)
			liveGuards = 0;
		return --liveGuards;
	}
	int GetGuards() {
		if(liveGuards>0)
			liveGuards = 0;
		return liveGuards;
	}
	void CptDied() {
		cptDead = true;
	}
	bool CptIsDead() {
		return cptDead;
	}
};

#define MOLDAR_HELLO		6907
#define MOLDAR_HELLO1		6908
class MoldarGossip: public GossipScript
{
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), MOLDAR_HELLO, Plr);
		Menu->AddItem(0, "Call me \"Boss\". What have you got for me!", 1);
		Menu->SendTo(Plr);
	}
	void GossipHello1(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		Plr->CastSpell(Plr, DM_MOLDAR_MOXIE, false);
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), MOLDAR_HELLO1, Plr);
		Menu->SendTo(Plr);
	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
  		if(IntId != 1) {
			GossipHello(pObject, Plr, true);
		}
		else {
			GossipHello1(pObject, Plr, true);
		}
	}
};
#define FENGUS_HELLO		6903
#define FENGUS_HELLO1		6904
class FengusGossip: public GossipScript
{
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FENGUS_HELLO, Plr);
		Menu->AddItem(0, "I am, amI? Well what have you got for the new big dog of Gordok, Fengus?", 1);
		Menu->SendTo(Plr);
	}
	void GossipHello1(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		Plr->CastSpell(Plr, DM_FENGUS_FEROCITY, false);
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FENGUS_HELLO1, Plr);
		Menu->SendTo(Plr);
	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
  		if(IntId != 1) {
			GossipHello(pObject, Plr, true);
		}
		else {
			GossipHello1(pObject, Plr, true);
		}
	}
};
#define SLIPKIK_HELLO		6905
#define SLIPKIK_HELLO1		6906
class SlipkikGossip: public GossipScript
{
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), SLIPKIK_HELLO, Plr);
		Menu->AddItem(0, "Yeah, you're a real brainiac. Just how smart do you think you are, Sli'kik?", 1);
		Menu->SendTo(Plr);
	}
	void GossipHello1(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		Plr->CastSpell(Plr, DM_SLIPKIK_SAVVY, false);
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), SLIPKIK_HELLO1, Plr);
		Menu->SendTo(Plr);
	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
  		if(IntId != 1) {
			GossipHello(pObject, Plr, true);
		}
		else {
			GossipHello1(pObject, Plr, true);
		}
	}
};

#define SPELL_ICE_LOCK		45438//22856
#define DM_BROKEN_TRAP		179485
#define DM_FIXED_TRAP		179512
class NorthGuardsAI: public MoonScriptCreatureAI
{
	SpellDesc *charge;
	SpellDesc *frenzy;
	bool frenzied;

	MOONSCRIPT_FACTORY_FUNCTION(NorthGuardsAI, MoonScriptCreatureAI);
	NorthGuardsAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		frenzy = AddSpell(DM_FRENZY, Target_Self, 0, 0, 0);
		charge = AddSpell(DM_SHIELD_CHARGE, Target_Current, 0, 0, 0);
		AddSpell(DM_SHIELD_BASH, Target_Current, 30, 0, 10);
		AddSpell(DM_KNOCK_AWAY, Target_Current, 30, 0, 15);
		AddSpell(DM_SATYR_STRIKE, Target_Current, 50, 0, 5);
	}

	void OnCombatStart(Unit *pTarget) {
		if(pTarget->HasAura(AURA_KING_GORDOK)) {
			_unit->SetFaction(1080);
		}
		CastSpellNowNoScheduling(charge);
		frenzied = false;
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate() {
		if(_unit->GetFaction() == 1080) {
			SetCanEnterCombat(false);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			_unit->GetAIInterface()->HandleEvent(EVENT_LEAVECOMBAT, _unit, 0);
		}
		if(!frenzied && GetHealthPercent()<40) {
			char emoteString[200];
			frenzied = true;
			sprintf(emoteString, "%s lets out a deep roar, alerting nearby allies and becoming enraged!", _unit->GetCreatureInfo()->Name);
			Emote(emoteString, Text_Emote);
			CastSpellNowNoScheduling(frenzy);
		}
		ParentClass::AIUpdate();
	}

	void OnReachWP(uint32 wpid, bool forward) {
		if(wpid == 2 && (_unit->GetCreatureInfo()->Id == CN_GUARD_SLIPKIK)) {
			GameObject *fixedTrap = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(559.6f, 549.8f, -25.4f, DM_FIXED_TRAP);
			if(fixedTrap) {
				ApplyAura(7405);
				SetMoveType(Move_DontMoveWP);
				SetCanEnterCombat(false);
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
		}
		if(wpid == 16 && (_unit->GetCreatureInfo()->Id == CN_GUARD_FENGUS)) {
			SetWaypointToMove(0);
		}
		ParentClass::OnReachWP(wpid, forward);
	}

	void OnCombatStop(Unit *pTarget) {
		if(IsAlive())
			RemoveAllAuras();
		ParentClass::OnCombatStop(pTarget);
	}

	void OnDied(Unit *pKiller) {
		TributeRunTriggerAI *watcher = (TributeRunTriggerAI *)_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(271.7f, -26.1f, -2.5f, TRIBUTE_RUN_TRIGGER);
		if(watcher) {
			char buffer[100];
			int acolytes = watcher->RemoveGuard();
			sprintf(buffer, "Watcher: %d!", acolytes);
			Emote(buffer);
		}
		else
			Emote("Watcher not found!");
		ParentClass::OnDied(pKiller);
	}
};
class BrokenTrapQuest: public QuestScript {
public:
	void OnQuestComplete(Player *target, QuestLogEntry *entry) {
		if(target) {
			GameObject *brokenTrap = target->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(559.6f, 549.8f, -25.4f, DM_BROKEN_TRAP);
			if(brokenTrap) {
				GameObject *fixedTrap = brokenTrap->GetMapMgr()->CreateGameObject(DM_FIXED_TRAP);
				if(fixedTrap) {
					fixedTrap->GetInfo()->Size = 3;
					fixedTrap->SetInstanceID(target->GetInstanceID());
					fixedTrap->CreateFromProto(DM_FIXED_TRAP, target->GetMapId(), target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0);
					fixedTrap->PushToWorld(target->GetMapMgr());
				}
				brokenTrap->Despawn(0, 0);
			}
		}
	}
};

class StomperKreegAI: public MoonScriptCreatureAI
{
	SpellDesc *frenzy;
	bool frenzied;

	MOONSCRIPT_FACTORY_FUNCTION(StomperKreegAI, MoonScriptCreatureAI);
	StomperKreegAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		frenzy = AddSpell(DM_DRUNKEN_RAGE, Target_Self, 0, 0, 0);
		AddSpell(DM_BOOZE_SPIT, Target_Current, 30, 0, 10);
		AddSpell(DM_WHIRLWIND, Target_Current, 30, 0, 10);
		AddSpell(DM_KREEG_WARSTOMP, Target_Self, 30, 0, 10);
		_unit->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
	}

	void OnCombatStart(Unit *pTarget) {
		if(pTarget->HasAura(AURA_KING_GORDOK)) {
			_unit->SetFaction(1080);
		}
		else {
			CastSpellNowNoScheduling(frenzy);
		}
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate() {
		if(_unit->GetFaction() == 1080) {
			SetCanEnterCombat(false);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			_unit->GetAIInterface()->HandleEvent(EVENT_LEAVECOMBAT, _unit, 0);
		}
		if(!frenzied && GetHealthPercent()<40) {
			frenzied = true;
			Emote("Stomper Kreeg goes into a drunken rage!", Text_Emote);
			CastSpellNowNoScheduling(frenzy);
		}
		ParentClass::AIUpdate();
	}

	void OnCombatStop(Unit *pTarget) {
		if(IsAlive())
			RemoveAllAuras();
		ParentClass::OnCombatStop(pTarget);
	}

	void OnDied(Unit *pKiller) {
		TributeRunTriggerAI *watcher = (TributeRunTriggerAI *)_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(271.7f, -26.1f, -2.5f, TRIBUTE_RUN_TRIGGER);
		if(watcher) {
			char buffer[100];
			int acolytes = watcher->RemoveGuard();
			sprintf(buffer, "Watcher: %d!", acolytes);
			Emote(buffer);
		}
		else
			Emote("Watcher not found!");
		ParentClass::OnDied(pKiller);
	}
};
#define KREEG_HELLO			6894
class KreegGossip: public GossipScript
{
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), KREEG_HELLO, Plr);
		Menu->AddItem(1, "Show me what hooch you've got to sell, Kreeg.", 1);
		Menu->AddItem(0, "What do you mean buy?! I'm the king now... and it's good to be the king!", 2);
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
  		switch(IntId) {
			case 0:
				GossipHello(pObject, Plr, true);
				break;
			case 1:
				Plr->GetSession()->SendInventoryList((Creature*)pObject);
				break;
			case 2:
				((Creature*)pObject)->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
				Plr->Gossip_Complete();
				break;
		}
	}
};

#define CN_KNOT_THIMNLEJACK 14338
#define KNOT_HELLO		6795
#define KNOT_HELLO_1	6883
#define KEY_HELLO		6875
#define SHACKLE_KEY		18250
class KnotThimAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(KnotThimAI, MoonScriptCreatureAI); 
	KnotThimAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
};
class KnotGossip: public GossipScript
{
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), KNOT_HELLO, Plr);
		if(Plr->GetItemInterface()->GetItemCount(SHACKLE_KEY))
			Menu->AddItem(0, "So I found this shackle key...", 2);
		Menu->AddItem(0, "Why should I bother fixing the trap? Why not just eliminate the guard the old fashioned way?", 1);
		Menu->SendTo(Plr);
	}
	void GossipHello1(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), KNOT_HELLO_1, Plr);
		Menu->SendTo(Plr);
	}
	void GossipKeyHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), KEY_HELLO, Plr);
		Menu->AddItem(0, "I guess so!", 3);
		Menu->SendTo(Plr);
	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
  		switch(IntId) {
			case 0:
				GossipHello(pObject, Plr, true);
				break;
			case 1:
				GossipHello1(pObject, Plr, true);
				break;
			case 2:
				GossipKeyHello(pObject, Plr, true);
				break;
			case 3:
				Plr->Gossip_Complete();
				break;
		}
	}
};
#define KNOTS_CACHE		179501
#define KNOTS_BALL		179511
static Location KnotWaypoint = {518.1, 544.1, -25.4, 0};
class GordokKeyQuest: public QuestScript {
public:
	void OnQuestComplete(Player *target, QuestLogEntry *entry) {
		if(target) {
			GameObject *knotsBall = target->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(580.2f, 522.9f, -25.2f, KNOTS_BALL);
			if(knotsBall) {
				knotsBall->Despawn(0, 0);
			}
			Creature *knot = target->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(581.0f, 523.3f, -25.2f, CN_KNOT_THIMNLEJACK);
			if(knot) {
				char buffer[1000];
				sprintf(buffer, "%s, I'll make sure all my friends in Cartel know that you've saved my life today. Here - I'm going to need to be able to travel light, so please help yourself to my cache of tailoring and leatherworking supplies! Thanks again - see ya!",
					target->getPlayerInfo()->name);
				knot->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, buffer);
				GameObject *cache = knot->GetMapMgr()->CreateGameObject(KNOTS_CACHE);
				if(cache) {
					cache->SetInstanceID(knot->GetInstanceID());
					cache->CreateFromProto(KNOTS_CACHE, knot->GetMapId(), knot->GetPositionX(), knot->GetPositionY(), knot->GetPositionZ(), 0);
					cache->PushToWorld(knot->GetMapMgr());
				}
				
				MoonScriptCreatureAI *knotAI = (MoonScriptCreatureAI *)knot->GetScript();
				if(knotAI) {
					knotAI->AddWaypoint(knotAI->CreateWaypoint(1, 0, Flag_Run, KnotWaypoint));
					knot->SetUInt32Value(UNIT_NPC_FLAGS, 0);
					knot->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
					knot->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
					knot->GetAIInterface()->setWaypointToMove(0);	
					knot->GetAIInterface()->StopMovement(0);
				}
				knot->Despawn(5000, 0);
			}
		}
	}
};

#define TANNIN_GUARD_WAYPOINTS		4
static Location TanninGuardWaypoints[TANNIN_GUARD_WAYPOINTS] = {
	{580.6, 535.7, 6.8, 0},
	{585.9, 541.6, 6.8, 0},
	{587.2, 561.6, -4.7, 0},
	{594.1, 600.5, -4.7, 0}
};

#define OGRE_TANNIN_BASKET		179499
class OgreTanninBasketAI : public GameObjectAIScript
{
public:
	OgreTanninBasketAI(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new OgreTanninBasketAI(GO); }

	void OnLootTaken(Player* pPlayer)	
	{
		Creature* guard = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(CN_GORDOK_BUSHWHACKER, 542.1f, 535.5f, 27.5f, 0, true, false, 0, 0, 0);
		if(guard) {
			MoonScriptCreatureAI* guardAI = (MoonScriptCreatureAI*)guard->GetScript();
			if(guardAI) {
				guardAI->Emote("NO!!! It puts the tannin in the basket, or it gets the mallet again!");
				for(int j=0; j<TANNIN_GUARD_WAYPOINTS; j++) {
					guardAI->AddWaypoint(guardAI->CreateWaypoint(j+1, 0, Flag_Run, TanninGuardWaypoints[j]));
				}
				guardAI->GetUnit()->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				guardAI->GetUnit()->GetAIInterface()->setWaypointToMove(0);	
			}
		}
	}
};

class CaptainKromcrushAI: public MoonScriptCreatureAI
{
	SpellDesc *sum_reavers;
	SpellDesc *enrage;
	SpellDesc *retaliation;
	bool do_enrage;
	bool do_retaliation;

	MOONSCRIPT_FACTORY_FUNCTION(CaptainKromcrushAI, MoonScriptCreatureAI);
	CaptainKromcrushAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		enrage = AddSpell(DM_ENRAGE, Target_Self, 0, 0, 0);
		sum_reavers = AddSpell(DM_CALL_REAVERS, Target_Self, 0, 0, 0);
		AddSpell(DM_FREIGHTENING_SHOUT, Target_Self, 10, 0, 20);
		AddSpell(DM_MORTAL_CLEAVE, Target_Current, 30, 0, 10);
		retaliation = AddSpell(DM_RETALIATION, Target_Self, 0, 0, 0);
		_unit->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
	}
	
	void OnCombatStart(Unit *pTarget) {
		if(pTarget->HasAura(AURA_KING_GORDOK)) {
			_unit->SetFaction(1080);
		}
		else {
			do_enrage = false;
			do_retaliation = false;
			Emote("No one get past me and threatens da king! Ungh, take it!", Text_Say);
		}
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate() {
		if(_unit->GetFaction() == 1080) {
			SetCanEnterCombat(false);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			_unit->GetAIInterface()->HandleEvent(EVENT_LEAVECOMBAT, _unit, 0);
		}
		if(!do_enrage && (GetHealthPercent() < 50)) {
			do_enrage = true;
			Emote("Help me crush these punys!");
			CastSpellNowNoScheduling(enrage);
			CastSpell(sum_reavers);
		}
		if(!do_retaliation && (GetHealthPercent() < 20)) {
			do_retaliation = true;
			Emote("Captain Kromcrush begins to retaliate all attacks against him!", Text_Emote);
			CastSpellNowNoScheduling(retaliation);
		}
		ParentClass::AIUpdate();
	}

	void OnCombatStop(Unit *pTarget) {
		if(IsAlive())
			RemoveAllAuras();
		ParentClass::OnCombatStop(pTarget);
	}

	void OnDied(Unit *pKiller) {
		TributeRunTriggerAI *watcher = (TributeRunTriggerAI *)_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(271.7f, -26.1f, -2.5f, TRIBUTE_RUN_TRIGGER);
		if(watcher) {
			char buffer[100];
			int acolytes = watcher->RemoveGuard();
			watcher->CptDied();
			sprintf(buffer, "Watcher: %d!", acolytes);
			Emote(buffer);
		}
		else
			Emote("Watcher not found!");
		ParentClass::OnDied(pKiller);
	}
};
#define KROM_HELLO			6913
#define KROM_HELLO1			6915
#define KROM_KING_HELLO		6914
#define KROM_KING_HELLO1	6920
static Location KromcrushWaypoint = {525.2, 481.5, 29.5, 0};
class KromcrushGossip: public GossipScript
{
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(Plr->HasAura(AURA_KING_GORDOK)) {
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), KROM_KING_HELLO, Plr);
			Menu->AddItem(0, "So, now that I'm the king... what have you got for me?!", 3);
		}
		else {
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), KROM_HELLO, Plr);
			Menu->AddItem(0, "Um, I'm taking some prisoners we found outside before the king for punishment.", 1);
		}
		Menu->SendTo(Plr);
	}
	void GossipHello1(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), KROM_HELLO1, Plr);
		Menu->AddItem(0, "Er... that's how I found them. I wanted to show the king that they were a threat! Say Captain... I overheard Guard Fengus calling you a fat, useless gnoll lover!", 2);
		Menu->SendTo(Plr);
	}
	void GossipHelloKing1(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), KROM_KING_HELLO1, Plr);
		Menu->AddItem(0, "This sounds like a task worthy of the new king!", 4);
		((Creature*)pObject)->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
		Menu->SendTo(Plr);
	}
	void GossipEnrage(Object * pObject, Player* Plr, bool AutoSend)
	{
		Creature* kromcrush = (Creature*)pObject;
		kromcrush->CastSpell(kromcrush, DM_ENRAGE, false);
		kromcrush->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hey, who Fengus callin' a gnoll lover?! Take da prisoners to da king, you smart to bring them with their weapons and show da king that they are a threat. I'll go see if Fengus talk smack when I give him da beatdown! HAR!");
		MoonScriptCreatureAI *kromcrushAI = (MoonScriptCreatureAI *)kromcrush->GetScript();
		if(kromcrushAI) {
			kromcrushAI->SetCanEnterCombat(false);
			kromcrush->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			kromcrushAI->AddWaypoint(kromcrushAI->CreateWaypoint(1, 0, Flag_Run, KromcrushWaypoint));
			kromcrush->SetUInt32Value(UNIT_NPC_FLAGS, 0);
			kromcrush->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			kromcrush->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
			kromcrush->GetAIInterface()->setWaypointToMove(0);	
			kromcrush->GetAIInterface()->StopMovement(0);
			kromcrushAI->Despawn(15000);
		}
		Plr->Gossip_Complete();
	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
  		switch(IntId) {
			case 0:
				GossipHello(pObject, Plr, true);
				break;
			case 1:
				GossipHello1(pObject, Plr, true);
				break;
			case 2:
				GossipEnrage(pObject, Plr, true);
				break;
			case 3:
				GossipHelloKing1(pObject, Plr, true);
				break;
			case 4:
				Plr->Gossip_Complete();
				break;
		}
	}
};

//king & chorush
#define CHORUSH_HELLO		6923
#define CHORUSH_HELLO_KING	6918
class ChorushGossip: public GossipScript
{
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(Plr->HasAura(AURA_KING_GORDOK))
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), CHORUSH_HELLO_KING, Plr);
		else
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), CHORUSH_HELLO, Plr);
		Menu->SendTo(Plr);
	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
  		switch(IntId) {
			case 0:
				GossipHello(pObject, Plr, true);
				break;
		}
	}
};
#define MIZZLE_WAYPOINTS		3
static Location MizzleWaypoints[MIZZLE_WAYPOINTS] = {
	{761.9, 480.6, 27.6, 6.2},
	{781.9, 480.6, 36.2, 6.2},
	{823.9, 477.8, 37.4, 3.11}
};
class ChorushObserverAI: public MoonScriptCreatureAI
{
	bool king_died;
	bool king_finished;
	MOONSCRIPT_FACTORY_FUNCTION(ChorushObserverAI, MoonScriptCreatureAI);
	ChorushObserverAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		king_died = false;
		king_finished = false;
		_unit->SetFaction(14);
		int randomNumber = RandomUInt(2);
		switch(randomNumber) {
			//priest
			case 0:
				_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 11932);
				//Heal
				AddSpell(22883, Target_WoundedFriendly, 15, 3.5, 20);
				//Mind blast
				AddSpell(17194, Target_Current, 20, 1.5, 5);
				//Power word: shield
				AddSpell(17139, Target_Self, 30, 0, 30);
				//Psychinc scream
				AddSpell(22884, Target_Self, 15, 0, 5);
				break;
			//shammy
			case 1:
				_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 20556);
				//Healing wave
				AddSpell(15982, Target_WoundedFriendly, 15, 3, 20);
				//Chain lightning
				AddSpell(15305, Target_Current, 20, 3, 5);
				//Bloodlust
				AddSpell(16170, Target_Self, 30, 0, 30);
				//Totem
				AddSpell(8376, Target_Self, 15, 0.5, 5);
				break;
			//mage
			case 2:
				_unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 2226);
				//Lightning bolt
				AddSpell(15234, Target_Current, 30, 3, 5);
				//Fireball
				AddSpell(17290, Target_Current, 30, 3, 5);
				//Arcane explosion
				AddSpell(13745, Target_Self, 30, 1.5, 5);
				//Frost nova
				AddSpell(15531, Target_Self, 15, 0, 10);
				break;
		}
	}
	void OnCombatStop(Unit *target) {
		if(IsAlive()){
			_unit->GetAIInterface()->MoveTo(_unit->GetSpawnX(), _unit->GetSpawnY(), _unit->GetSpawnZ(), _unit->GetSpawnO());
			_unit->GetAIInterface()->StopMovement(0);
			_unit->SetStandState(STANDSTATE_SIT);
		}

		ParentClass::OnCombatStop(target);
	}
	void AIUpdate() {
		if(king_died && !king_finished) {
			king_finished = true;
			_unit->SetFaction(1080);
			Emote("The king is dead - OH NOES! Summon Mizzle da Crafty! He knows what to do next!");
			SetCanEnterCombat(false);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			_unit->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, _unit, 0);
		}
		ParentClass::AIUpdate();
	}

	void OnDied(Unit *pKiller) {
		TributeRunTriggerAI *watcher = (TributeRunTriggerAI *)_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(271.7f, -26.1f, -2.5f, TRIBUTE_RUN_TRIGGER);
		if(watcher) {
			char buffer[100];
			int acolytes = watcher->RemoveGuard();
			sprintf(buffer, "Watcher: %d!", acolytes);
			Emote(buffer);
		}
		else
			Emote("Watcher not found!");
		ParentClass::OnDied(pKiller);
	}
public:
	void KingDied() {
		king_died = true;
	}
};

class MizzleAI: public MoonScriptCreatureAI
{
	bool treasureChest;
	MOONSCRIPT_FACTORY_FUNCTION(MizzleAI, MoonScriptCreatureAI);
	MizzleAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		treasureChest = false;
	}
	void OnReachWP(uint32 id, bool forward) {
		if(id == 1)
			Emote("OH NOES! Da king is dead! Uh... hail to da new king! Yeah!");
		if(id == MIZZLE_WAYPOINTS) {
			Emote("Yar, he's dead alright. That makes you da new king... well, all of you! Gordok is yours now, boss! You should talk to me so you can learn everything there is about being da king! I was... is his assistant! Yeah, that's why I'm called da crafty one!", Text_Say);
			TributeRunTriggerAI *watcher = (TributeRunTriggerAI *)_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(271.7f, -26.1f, -2.5f, TRIBUTE_RUN_TRIGGER);
			if(watcher) {
				if(!watcher->CptIsDead()) {
					MoonScriptCreatureAI *captain = SpawnCreature(CN_CAPTAIN_KROMCRUSH, 384.2, 257.1, 11.22);
					if(captain) {
						captain->Emote("OK Fengus, where you at?! You come call me a gnoll lover while I give you da hammer upside da head!");
					}
					else
						Emote("Captain not found!");
				}
				else
					Emote("Captain is dead!");
			}
			else
				Emote("Watcher not found!");
		}
		ParentClass::OnReachWP(id, forward);
	}
	bool IsChestSpawned() {
		return treasureChest;
	}
	void SpawnChest() {
		treasureChest = true;
	}
};
#define MIZZLE_HELLO		6876
#define MIZZLE_HELLO1		6882
#define MIZZLE_KING_HELLO	6895
#define MIZZLE_KING_HELLO1	6916

#define DM_TRIBUTE_CHEST		179564
static Location ChestLocation = {829.4, 477.9, 37.4, 0};
static uint32 loot1[] = {
	18480,		//scarab blate helm
	18475,		//oddly magical belt
	18482,		//ogre toothpick shooter
	18477		//shaggy leggings
};
static uint32 loot2[] = {
	18478,		//hyena hide jerkin
	18476,		//mud stained boots
	18479,		//carrion scorpid helm
	18481		//skullcracking mace
};
static uint32 loot3[] = {
	18528,		//cyclone spaulders
	18529,		//elemental plate girdle
	18500		//tarnished elven ring
};
static uint32 loot4[] = {
	18532,		//mindsurge robe
	18499,		//barrier shield
	18530,		//ogre forged hauberk
	18531		//unyielding maul
};
static uint32 loot5[] = {
	18533,		//gordok bracers of power
	18537,		//counteratttack lodestone
	18495,		//redoubt cloak
	18534		//rod of the ogre magi
};
#define TREANTS_BANE		18538
class MizzleGossip: public GossipScript
{
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(Plr->HasAura(AURA_KING_GORDOK)) {
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), MIZZLE_KING_HELLO, Plr);
			Menu->AddItem(0, "Henchmen? Tribute?", 2);
		}
		else {
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), MIZZLE_HELLO, Plr);
			Menu->AddItem(0, "I'm the new king? What are you talking about?", 1);
		}
		Menu->SendTo(Plr);
	}
	void GossipHello1(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		Plr->CastSpell(Plr, 22799, false);
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), MIZZLE_HELLO1, Plr);
		Menu->AddItem(0, "It's good to be the king! Now, let's get back to what you were talking about before...", 4);
		Menu->SendTo(Plr);
	}
	void GossipKingHello1(Object * pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), MIZZLE_KING_HELLO1, Plr);
		Menu->AddItem(0, "Well then... show me the tribute!", 3);
		Menu->SendTo(Plr);
	}
	void SpawnTributeChest(Player* Plr, int liveBosses) {
		GameObject *chest = Plr->GetMapMgr()->CreateGameObject(DM_TRIBUTE_CHEST);
		if(chest) {
			chest->SetInstanceID(Plr->GetInstanceID());
			chest->CreateFromProto(DM_TRIBUTE_CHEST, Plr->GetMapId(), ChestLocation.x, ChestLocation.y, ChestLocation.z, ChestLocation.o);
			chest->loot.items.clear();
			if(!RandomUInt(10000)) {						//0.01%
				lootmgr.AddLoot(&chest->loot, TREANTS_BANE, 1, 1, 0);
			}
			switch(liveBosses) {
				case 0:
					lootmgr.AddLoot(&chest->loot, loot5[RandomUInt(3)], 1, 1, 0);
				case -1:
					lootmgr.AddLoot(&chest->loot, loot4[RandomUInt(3)], 1, 1, 0);
				case -2:
					lootmgr.AddLoot(&chest->loot, loot3[RandomUInt(2)], 1, 1, 0);
					lootmgr.AddLoot(&chest->loot, loot2[RandomUInt(3)], 1, 1, 0);
				case -3:
					lootmgr.AddLoot(&chest->loot, loot1[RandomUInt(3)], 1, 1, 0);
				case -4:
				case -5:
					lootmgr.AddLoot(&chest->loot, 13446, 2, 6, 0);		//major heal
					lootmgr.AddLoot(&chest->loot, 13444, 2, 6, 0);		//major mana
				case -6:
					lootmgr.AddLoot(&chest->loot, 8766, 15, 20, 0);		//morning glore dew
					lootmgr.AddLoot(&chest->loot, 8952, 15, 20, 0);		//roasted quail
					break;
			}
			chest->PushToWorld(Plr->GetMapMgr());
		}
	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		MizzleAI *mizzle;
  		switch(IntId) {
			case 0:
				GossipHello(pObject, Plr, true);
				break;
			case 1:
				GossipHello1(pObject, Plr, true);
				break;
			case 2:
				GossipKingHello1(pObject, Plr, true);
				break;
			case 3:
				mizzle = (MizzleAI *)((Creature*)pObject)->GetScript();
				if(mizzle && !mizzle->IsChestSpawned()) {
					mizzle->SpawnChest();
					TributeRunTriggerAI *watcher = (TributeRunTriggerAI *)Plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(271.7f, -26.1f, -2.5f, TRIBUTE_RUN_TRIGGER);
					if(watcher) {
						int guards = watcher->GetGuards();
						SpawnTributeChest(Plr, guards);
					}
				}
				Plr->Gossip_Complete();
				break;
			case 4:
				Plr->Gossip_Complete();
				break;
		}
	}
};

class KingGordokAI: public MoonScriptCreatureAI
{
	ChorushObserverAI *chorush;

	MOONSCRIPT_FACTORY_FUNCTION(KingGordokAI, MoonScriptCreatureAI);
	KingGordokAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_BERSERKER_CHARGE, Target_RandomPlayer, 50, 0, 30);
		AddSpell(DM_KING_MORTAL_STRIKE, Target_Current, 30, 0, 15);
		AddSpell(DM_KING_SUNDER_ARMOR, Target_Current, 30, 0, 10);
		AddSpell(DM_WARSTOMP, Target_Self, 20, 0, 15);
	}

	void OnCombatStart(Unit *pTarget) {
		Emote("You no challenge me, scrubs! I'm da king now, and I stay king FOREVER!!!", Text_Say);
		chorush = (ChorushObserverAI*)GetNearestCreature(CN_CHORUSH_OBSERVER);
		if(!chorush)
			Emote("Chorush not found!");
		ParentClass::OnCombatStart(pTarget);
	}
	void OnDied(Unit *pKiller) {
		if(chorush) {
			chorush->KingDied();
		}
		MoonScriptCreatureAI *mizzle = SpawnCreature(CN_MIZZLE, 745.6f, 481.5f, 28.1f, 6.2f);
		if(mizzle) {
			for(int i = 0; i<MIZZLE_WAYPOINTS; i++)
				mizzle->AddWaypoint(CreateWaypoint(i+1, 0, Flag_Run, MizzleWaypoints[i]));
			mizzle->GetUnit()->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			mizzle->GetUnit()->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
			mizzle->GetUnit()->GetAIInterface()->setWaypointToMove(0);	
			mizzle->GetUnit()->GetAIInterface()->StopMovement(0);
		}
		else
			Emote("mizzle no found!");
		ParentClass::OnDied(pKiller);
	}
};

//west DM
class TendirsWarpwoodAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TendirsWarpwoodAI, MoonScriptCreatureAI);
	TendirsWarpwoodAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_TENDRIS_ENTANGLE, Target_RandomPlayer, 50, 0, 10);
		AddSpell(DM_GRASPING_WINES, Target_Self, 30, 1, 20);
		AddSpell(DM_TENDRIS_UPPERCUT, Target_Current, 30, 0, 10);
		AddSpell(DM_TRAMPLE, Target_Self, 20, 0, 15);
	}
};
class MagisterKalendrisAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MagisterKalendrisAI, MoonScriptCreatureAI);
	MagisterKalendrisAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(DM_SHADOWFORM);
		AddSpell(DM_KALENDRIS_SWPAIN, Target_RandomPlayer, 50, 0, 18);
		AddSpell(DM_DOMINATE_MIND, Target_RandomPlayerNotCurrent, 50, 2, 30);
		AddSpell(DM_MIND_BLAST, Target_Current, 30, 1.5, 8);
		AddSpell(DM_MIND_FLAY, Target_Current, 30, 0, 2);
	}
};
//Illyana & bear
class IllyannaRavenoakAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllyannaRavenoakAI, MoonScriptCreatureAI);
	IllyannaRavenoakAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_CONCUSSIVE_SHOT, Target_Current, 100, 0, 30);
		AddSpell(DM_IMMOLATION_TRAP, Target_Self, 10, 0, 30);
		AddSpell(DM_MULTISHOT, Target_RandomPlayerNotCurrent, 30, 0, 8);
		AddSpell(DM_VOLLEY, Target_RandomPlayer, 30, 0, 60);
	}
};
class FerraAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FerraAI, MoonScriptCreatureAI);
	FerraAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_FERRA_CHARGE, Target_RandomPlayerNotCurrent, 30, 0, 5);
		AddSpell(DM_MAUL, Target_Current, 30, 0, 5);
	}
};

//Immol'thar
class ImmoltharAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ImmoltharAI, MoonScriptCreatureAI);
	ImmoltharAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_FRENZY, Target_Self, 30, 0, 30);
		AddSpell(DM_SUM_EYE_OF_IMMOLTHAR, Target_Self, 100, 0, 10);
		AddSpell(DM_INFECTED_BITE, Target_Current, 30, 0, 10);
		AddSpell(DM_PORTAL_OF_IMMOLTHAR, Target_RandomPlayer, 20, 0, 30);
		AddSpell(DM_TRAMPLE, Target_Self, 30, 0, 10);
	}

	void OnCombatStop(Unit *pTarget) {
		if(IsAlive())
			RemoveAllAuras();
		ParentClass::OnCombatStop(pTarget);
	}
};
class EyeOfImmoltharAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EyeOfImmoltharAI, MoonScriptCreatureAI);
	EyeOfImmoltharAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_EYE_OF_IMMOLTHAR, Target_Current, 100, 0, 4);
	}
};
class PrinceTortheldrinAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PrinceTortheldrinAI, MoonScriptCreatureAI);
	PrinceTortheldrinAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_PRINCE_ARCANE_BLAST, Target_Current, 30, 0, 15);
		AddSpell(DM_COUNTERSPELL, Target_RandomPlayerNotCurrent, 30, 0, 30);
		AddSpell(DM_SUMMON, Target_Self, 30, 0, 30);
		AddSpell(DM_TRASH, Target_Current, 15, 0, 10);
		AddSpell(DM_PRINCE_WHIRLWIND, Target_Current, 15, 0, 10);
	}
};

//Tsu'zee - rare spawn
class TsuZeeAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TsuZeeAI, MoonScriptCreatureAI);
	TsuZeeAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_TSUZEE_BACKSTAB, Target_Current, 15, 0, 10);
		AddSpell(DM_BLIND, Target_RandomPlayerNotCurrent, 30, 0, 20);
		AddSpell(DM_GOUGE, Target_Current, 30, 0, 20);
		AddSpell(DM_TSUZEE_SINISTER_STRIKE, Target_Current, 30, 0, 2);
	}
};
/*
//T0.5 boss just placeholder for now
class IsalienAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IsalienAI, MoonScriptCreatureAI);
	IsalienAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
};
class EmpyreanAI: public MoonScriptCreatureAI
{
	SpellDesc *enrage;
	bool do_enrage;

	MOONSCRIPT_FACTORY_FUNCTION(EmpyreanAI, MoonScriptCreatureAI);
	EmpyreanAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		enrage = AddSpell(DM_ENRAGE, Target_Self, 0, 0, 0);
		AddSpell(DM_RAKE, Target_Current, 30, 0, 10);
		AddSpell(DM_SUNDERING_CLEAVE, Target_Current, 30, 0, 10);
		AddSpell(DM_TRASH, Target_Current, 10, 0, 20);
	}

	void OnCombatStart(Unit *pTarget) {
		do_enrage = false;
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate() {
		if(!do_enrage && (GetHealthPercent() < 50.0)) {
			do_enrage = true;
			CastSpell(enrage);
		}
		ParentClass::AIUpdate();
	}

	void OnCombatStop(Unit *pTarget) {
		if(IsAlive())
			RemoveAllAuras();
		ParentClass::OnCombatStop(pTarget);
	}
};

*/

#define GORDOK_TRASH_GOSSIP1		6922

//classes for trash
class ArcaneAberrationAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ArcaneAberrationAI, MoonScriptCreatureAI);
	ArcaneAberrationAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_ARCANE_BOLT, Target_Current, 30, 1, 1);
		AddSpell(DM_ABER_MANA_BURN, Target_RandomPlayer, 60, 0, 15);
	}
};

class ArcaneFeedbackAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ArcaneFeedbackAI, MoonScriptCreatureAI);
	ArcaneFeedbackAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(DM_LIGHTNING_SHIELD);
	}
};

class ArcaneTorrentAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ArcaneTorrentAI, MoonScriptCreatureAI);
	ArcaneTorrentAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_FORKED_LIGHTNING, Target_Current, 20, 1.5, 10);
		AddSpell(DM_LIGHTNING_CLOUD, Target_RandomPlayer, 100, 2.5, 60);
	}
};

class CarrionSwarmerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CarrionSwarmerAI, MoonScriptCreatureAI);
	CarrionSwarmerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_VENOM_STING, Target_Current, 30, 0, 5);
	}
};

class DreadGuardAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DreadGuardAI, MoonScriptCreatureAI);
	DreadGuardAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_CLEAVE, Target_Current, 20, 0, 5);
		AddSpell(DM_SHADOW_SHOCK, Target_Current, 20, 0, 15);
	}
};

class EldrethApparitionAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EldrethApparitionAI, MoonScriptCreatureAI);
	EldrethApparitionAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(DM_FROST_ARMOR);
		AddSpell(DM_CONE_OF_COLD, Target_Current, 10, 0, 10);
		AddSpell(DM_FROSTBOLT, Target_Current, 45, 1, 1);
		AddSpell(DM_CHAINS_OF_ICE, Target_RandomPlayer, 20, 1, 30);
	}
};

class EldrethDarterAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EldrethDarterAI, MoonScriptCreatureAI);
	EldrethDarterAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_DART_MANA_BURN, Target_RandomPlayerNotCurrent, 20, 0, 10);
		AddSpell(DM_PWSHIELD, Target_RandomFriendly, 50, 0, 30);
	}
};

class EldrethPhantasmAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EldrethPhantasmAI, MoonScriptCreatureAI);
	EldrethPhantasmAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_CALL_OF_THE_GRAVE, Target_RandomPlayer, 30, 0, 30);
		AddSpell(DM_SHRINK, Target_RandomPlayer, 30, 0, 30);
	}
};

class EldrethSeetherAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EldrethSeetherAI, MoonScriptCreatureAI);
	EldrethSeetherAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_CRIMSON_FURY, Target_Self, 10, 0, 30);
		AddSpell(DM_FRENZY, Target_Self, 30, 0, 30);
	}
};

class EldrethSorcererAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EldrethSorcererAI, MoonScriptCreatureAI);
	EldrethSorcererAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_POLYMORPH, Target_RandomPlayerNotCurrent, 10, 0, 30);
		AddSpell(DM_CHAIN_LIGHTNING, Target_Current, 25, 3, 5);
		AddSpell(DM_STARSHARDS, Target_Current, 30, 0, 2);
	}
};

class EldrethSpectreAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EldrethSpectreAI, MoonScriptCreatureAI);
	EldrethSpectreAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(DM_GREATER_INVISIBILITY);
		AddSpell(DM_DARK_OFFERING, Target_WoundedFriendly, 20, 0, 20);
		AddSpell(DM_DISPEL_MAGIC, Target_RandomPlayer, 10, 0, 10);
	}
};

class EldrethSpiritAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EldrethSpiritAI, MoonScriptCreatureAI);
	EldrethSpiritAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_BANSHEE_SHRIEK, Target_Current, 20, 0, 30);
		AddSpell(DM_RIBBON_OF_SOULS, Target_Current, 20, 0, 10);
	}
};

class FelLashAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FelLashAI, MoonScriptCreatureAI);
	FelLashAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_FEL_ARCANE_BOLT, Target_Current, 20, 1, 5);
		AddSpell(DM_ARCANE_EXPLOSION, Target_Self, 20, 0, 10);
	}
};

class GordokBruteAI: public MoonScriptCreatureAI
{
	SpellDesc *enrage;
	bool do_enrage;

	MOONSCRIPT_FACTORY_FUNCTION(GordokBruteAI, MoonScriptCreatureAI);
	GordokBruteAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		enrage = AddSpell(DM_BRUT_ENRAGE, Target_Self, 0, 0, 0);
		AddSpell(DM_BACKHAND, Target_ClosestPlayerNotCurrent, 50, 0, 20);
		AddSpell(DM_BRUISING_BLOW, Target_Current, 30, 0, 5);
		AddSpell(DM_PUMMEL, Target_RandomPlayer, 0, 0, 0);
		AddSpell(DM_UPEERCAT, Target_Current, 15, 0, 10);
	}

	void OnCombatStart(Unit *pTarget) {
		do_enrage = false;
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate() {
		if(!do_enrage && (GetHealthPercent() < 50.0))  {
			do_enrage = true;
			Emote("Gordok Brute puts his club away and begins swinging wildly!", Text_Emote);
			CastSpell(enrage);
		}
		ParentClass::AIUpdate();
	}

	void OnCombatStop(Unit *pTarget) {
		RemoveAllAuras();
		do_enrage = false;
		ParentClass::OnCombatStop(pTarget);
	}
};

class GordokBushwhackerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GordokBushwhackerAI, MoonScriptCreatureAI);
	GordokBushwhackerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_CLEAVE, Target_Current, 30, 0, 5);
		AddSpell(DM_MORTAL_STRIKE, Target_Current, 20, 0, 10);
		AddSpell(DM_SUNDER_ARMOR, Target_Current, 30, 0, 5);
	}
};

class GordokCaptainAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GordokCaptainAI, MoonScriptCreatureAI);
	GordokCaptainAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_DARK_STRIKE, Target_Current, 30, 0, 5);
		AddSpell(DM_DEMO_SHOUT, Target_Self, 30, 0, 30);
		AddSpell(DM_FEAR, Target_RandomPlayerNotCurrent, 30, 0, 5, 0, 20);
		AddSpell(DM_CAPT_SHADOW_SHOCK, Target_Current, 30, 0, 5);
	}
};

class GordokMageLordAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GordokMageLordAI, MoonScriptCreatureAI);
	GordokMageLordAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_BLOODLUST, Target_RandomFriendly, 30, 0, 60);
		AddSpell(DM_FIRE_BLAST, Target_Current, 30, 0, 20);
		AddSpell(DM_FLAMESTRIKE, Target_RandomPlayerNotCurrent, 20, 3, 60, 0, 20);
		AddSpell(DM_MAGELORD_FROSTBOLT, Target_Current, 30, 3, 1);
		AddSpell(DM_POLYMORPH, Target_RandomPlayer, 10, 1.5, 20);
	}
};

class GordokMastiffAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GordokMastiffAI, MoonScriptCreatureAI);
	GordokMastiffAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_REND, Target_Current, 30, 0, 10);
		AddSpell(DM_TENDON_RIP, Target_Current, 20, 0, 10);
	}
};

class GordokReaverAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GordokReaverAI, MoonScriptCreatureAI);
	GordokReaverAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_CLEAVE, Target_Current, 30, 0, 5);
		AddSpell(DM_MORTAL_STRIKE, Target_Current, 20, 0, 10);
		AddSpell(DM_SUNDER_ARMOR, Target_Current, 30, 0, 5);
	}
};

class GordokWarlockAI: public MoonScriptCreatureAI
{
	SpellDesc *summon;

	MOONSCRIPT_FACTORY_FUNCTION(GordokWarlockAI, MoonScriptCreatureAI);
	GordokWarlockAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(DM_DEMON_ARMOR);
		AddSpell(DM_BANISH, Target_RandomPlayer, 30, 1, 15);
		AddSpell(DM_CURSE_OF_TONGUES, Target_RandomPlayerNotCurrent, 30, 0, 15);
		AddSpell(DM_IMMOLATE, Target_RandomPlayer, 30, 2, 1);
		AddSpell(DM_SHADOW_BOLT, Target_Current, 50, 3, 1);
		summon = AddSpell(DM_SUMMON_DOOMGUARD, Target_Self, 0, 0, 0);
		CastSpellNowNoScheduling(summon);
	}
};

class HighborneSummonerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HighborneSummonerAI, MoonScriptCreatureAI);
	HighborneSummonerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_SUM_FIRE_BLAST, Target_Current, 30, 0, 15);
		AddSpell(DM_FROST_NOVA, Target_Self, 10, 0, 20);
		AddSpell(DM_FIREBALL, Target_Current, 50, 3, 1);
	}
};

class IronbarkProtectorAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IronbarkProtectorAI, MoonScriptCreatureAI);
	IronbarkProtectorAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_ENTANGLING_ROOTS, Target_Current, 20, 0, 15);
		AddSpell(DM_PROTECT_KNOCK_AWAY, Target_Current, 20, 0, 5);
		AddSpell(DM_TRAMPLE, Target_Self, 30, 0, 5);
	}
};

class ManaBurstAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ManaBurstAI, MoonScriptCreatureAI);
	ManaBurstAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_BURST_ARCANE_EXPLOSION, Target_Self, 100, 0, 1);
	}
};

class ManaRemnantAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ManaRemnantAI, MoonScriptCreatureAI);
	ManaRemnantAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_BLINK, Target_RandomPlayerNotCurrent, 10, 0, 30);
		AddSpell(DM_REMN_CHAIN_LIGHTNING, Target_Current, 30, 3, 3);
	}
};

class NetherwalkerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(NetherwalkerAI, MoonScriptCreatureAI);
	NetherwalkerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_SHADOW_BOLT_VOLLEY, Target_Self, 50, 0, 15);
	}

	void OnDied(Unit *pKiller) {
		Despawn(2);
		ParentClass::OnDied(pKiller);
	}
};

class PetrifiedGuardianAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PetrifiedGuardianAI, MoonScriptCreatureAI);
	PetrifiedGuardianAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(DM_THORNS);
		AddSpell(DM_REGROWTH, Target_WoundedFriendly, 10, 0, 10);
	}
};

class PetrifiedTreantAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PetrifiedTreantAI, MoonScriptCreatureAI);
	PetrifiedTreantAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(DM_HARDEN_SKIN);
	}
};

class PhaseLasherAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PhaseLasherAI, MoonScriptCreatureAI);
	PhaseLasherAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_PHASE_SHADOW_BOLT_VOLLEY, Target_Self, 40, 0, 10);
	}
};

class ResidualMonstrosityAI: public MoonScriptCreatureAI
{
	SpellDesc *death_summon;

	MOONSCRIPT_FACTORY_FUNCTION(ResidualMonstrosityAI, MoonScriptCreatureAI);
	ResidualMonstrosityAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_ARCANE_BLAST, Target_Current, 20, 0, 10);
		AddSpell(DM_MONS_ARCANE_BOLT, Target_Current, 40, 1, 1);
		death_summon = AddSpell(DM_SUMMON_MANA_BURST, Target_Self, 0, 0, 0);
	}

	void OnDied(Unit *pKiller) {
		CastSpellNowNoScheduling(death_summon);
		ParentClass::OnDied(pKiller);
	}
};

class SkeletalHighborneAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SkeletalHighborneAI, MoonScriptCreatureAI);
	SkeletalHighborneAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_SKEL_SUNDER_ARMOR, Target_Current, 15, 0, 5);
	}
};

class EyeOfKilroggAI: public MoonScriptCreatureAI
{
	SpellDesc *summon_spell;

	MOONSCRIPT_FACTORY_FUNCTION(EyeOfKilroggAI, MoonScriptCreatureAI);
	EyeOfKilroggAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		summon_spell = AddSpell(DM_SUMMON_NETHERWALKER, Target_Self, 0, 4, 0);
	}

	void OnCombatStart(Unit *pTarget) {
		Emote("Wandering Eye of Kilrogg senses your presence and opens a nether portal!", Text_Emote);
		CastSpellNowNoScheduling(summon_spell);
		Despawn(4500);
		ParentClass::OnCombatStart(pTarget);
	}
};

class WarpwoodCrusherAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WarpwoodCrusherAI, MoonScriptCreatureAI);
	WarpwoodCrusherAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_CRUSH_ARMOR, Target_Current, 30, 0, 10);
		AddSpell(DM_CRUSH_TRAMPLE, Target_Self, 15, 0, 5);
	}
};

class WarpwoodGuardianAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WarpwoodGuardianAI, MoonScriptCreatureAI);
	WarpwoodGuardianAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_HEAD_CRACK, Target_Current, 30, 0, 5);
		AddSpell(DM_PUMMEL, Target_Current, 0, 0, 0);
		AddSpell(DM_GUARD_REGROWTH, Target_WoundedFriendly, 20, 2, 15);
	}
};

class WarpwoodStomperAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WarpwoodStomperAI, MoonScriptCreatureAI);
	WarpwoodStomperAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_WARSTOMP, Target_Self, 15, 0, 15);
	}
};

class WarpwoodTanglerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WarpwoodTanglerAI, MoonScriptCreatureAI);
	WarpwoodTanglerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_TANG_ENTANGLING_ROOTS, Target_RandomPlayer, 30, 1.5, 10);
		AddSpell(DM_LIGHTNING_BOLT, Target_Current, 30, 2, 1);
	}
};

class WarpwoodTreantAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WarpwoodTreantAI, MoonScriptCreatureAI);
	WarpwoodTreantAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_KNOCKDOWN, Target_Current, 15, 0, 10);
		AddSpell(DM_STRIKE, Target_Current, 30, 0, 1);
		AddSpell(DM_VICIOUS_REND, Target_Current, 30, 0, 15);
	}
};

class WhipLasherAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WhipLasherAI, MoonScriptCreatureAI);
	WhipLasherAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_LASH_OF_PAIN, Target_Current, 30, 0, 3);
	}
};

class WildspawnBetrayerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WildspawnBetrayerAI, MoonScriptCreatureAI);
	WildspawnBetrayerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_EXPLODING_SHOT, Target_RandomPlayerNotCurrent, 20, 1, 10);
		AddSpell(DM_KNOCKDOWN, Target_Current, 15, 0, 3);
		AddSpell(DM_SHADOW_SHOT, Target_RandomPlayerNotCurrent, 20, 0, 10);
		AddSpell(DM_SHOOT, Target_Current, 30, 0, 3);
	}
};

class WildspawnFelswornAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WildspawnFelswornAI, MoonScriptCreatureAI);
	WildspawnFelswornAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(DM_CHAOTIC_FOCUS);
		AddSpell(DM_FEL_FEAR, Target_RandomPlayerNotCurrent, 10, 1.5, 30);
		AddSpell(DM_SHADOW_SHIELD, Target_Self, 20, 0, 30);
		AddSpell(DM_SWPAIN, Target_RandomPlayer, 30, 0, 20);
		AddSpell(DM_FEL_SHADOW_BOLT, Target_Current, 35, 3, 1);
	}
};

class WildspawnHellcallerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WildspawnHellcallerAI, MoonScriptCreatureAI);
	WildspawnHellcallerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_HELL_FIREBALL, Target_Current, 30, 3, 1);
		AddSpell(DM_RAIN_OF_FIRE, Target_RandomPlayer, 20, 0, 10);
	}
};

class WildspawnImpAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WildspawnImpAI, MoonScriptCreatureAI);
	WildspawnImpAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_IMP_FIRE_BLAST, Target_Current, 20, 0, 10);
	}
	void OnReachWP(uint32 wpid, bool forward) {
		if(wpid == ALZZIN_IMPS_WAYPOINTS) {
			StopWaypointMovement();
			AggroNearestPlayer(1);
		}
		ParentClass::OnReachWP(wpid, forward);
	}
};

class WildspawnRogueAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WildspawnRogueAI, MoonScriptCreatureAI);
	WildspawnRogueAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_SINISTER_STRIKE, Target_Current, 30, 0, 2);
		AddSpell(DM_RUPTURE, Target_Current, 20, 0, 10);
	}
};

class WildspawnSatyrAI: public MoonScriptCreatureAI
{
	SpellDesc *enrage;
	bool do_enrage;

	MOONSCRIPT_FACTORY_FUNCTION(WildspawnSatyrAI, MoonScriptCreatureAI);
	WildspawnSatyrAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		do_enrage = false;
		enrage = AddSpell(DM_ENRAGE, Target_Self, 0, 0, 0);
		AddSpell(DM_BATTLE_SHOUT, Target_Self, 30, 0, 120);
		AddSpell(DM_HARMSTRING, Target_Current, 30, 0, 10);
		AddSpell(DM_SATYR_STRIKE, Target_Current, 30, 0, 3);
	}

	void OnCombatStart(Unit *pTarget) {
		do_enrage = false;
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate() {
		if(!do_enrage && (GetHealthPercent() < 50.0))  {
			do_enrage = true;
			CastSpell(enrage);
		}
		ParentClass::AIUpdate();
	}

	void OnCombatStop(Unit *pTarget) {
		RemoveAllAuras();
		do_enrage = false;
		ParentClass::OnCombatStop(pTarget);
	}
};

class WildspawnShadowstalkerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WildspawnShadowstalkerAI, MoonScriptCreatureAI);
	WildspawnShadowstalkerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(DM_STEALTH);
		ApplyAura(DM_STEALTH_DETECT);
		AddSpell(DM_BACKSTAB, Target_Current, 15, 0, 10);
	}

	void OnCombatStop(Unit *pTarget) {
		ApplyAura(DM_STEALTH);
		ParentClass::OnCombatStop(pTarget);
	}
};

class WildspawnTricksterAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WildspawnTricksterAI, MoonScriptCreatureAI);
	WildspawnTricksterAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DM_CURSE_OF_IMPOTENCE, Target_RandomPlayerNotCurrent, 15, 0, 120);
		AddSpell(DM_CURSE_OF_WEAKNESS, Target_RandomPlayerNotCurrent, 15, 0, 120);
		AddSpell(DM_PARALYZING_POISON, Target_Current, 20, 0, 30);
	}
};

void SetupDireMaul(ScriptMgr * mgr)
{
	mgr->register_gossip_script(CN_OLD_IRONBARK, (GossipScript*) new OldIronbarkGossip());
	mgr->register_creature_script(CN_OLD_IRONBARK_REDEEMED, &OldIronbarkRedeemedAI::Create);
	mgr->register_gossip_script(CN_OLD_IRONBARK_REDEEMED, (GossipScript*) new OldIronbarkRedeemedGossip());
	mgr->register_creature_script(CN_HYDROSPAWN, &HydrospawnAI::Create);
	mgr->register_creature_script(CN_MASSIVE_GEYSER, &MassiveGeyserAI::Create);
	mgr->register_creature_script(CN_LETHTENDRIS, &LethtendrisAI::Create);
	mgr->register_creature_script(CN_PIMGIB, &BigImpAI::Create);
	mgr->register_creature_script(CN_ZEVRIM_THORNHOOF, &ZevrimThornhoofAI::Create);
	mgr->register_creature_script(CN_PUSILLIN, &PusillinAI::Create);
	mgr->register_gossip_script(CN_PUSILLIN, (GossipScript*) new PusillinGossip());
	mgr->register_creature_script(CN_ALZZIN_WILDSHAPER, &AlzzinTheWildshaperAI::Create);

	mgr->register_creature_script(TRIBUTE_RUN_TRIGGER, &TributeRunTriggerAI::Create);
	mgr->register_creature_script(CN_GUARD_MOLDAR, &NorthGuardsAI::Create);
	mgr->register_gossip_script(CN_GUARD_MOLDAR, (GossipScript*) new MoldarGossip());
	mgr->register_creature_script(CN_STOMPER_KREEG, &StomperKreegAI::Create);
	mgr->register_gossip_script(CN_STOMPER_KREEG, (GossipScript*) new KreegGossip());
	mgr->register_creature_script(CN_GUARD_FENGUS, &NorthGuardsAI::Create);
	mgr->register_gossip_script(CN_GUARD_FENGUS, (GossipScript*) new FengusGossip());
	mgr->register_creature_script(CN_GUARD_SLIPKIK, &NorthGuardsAI::Create);
	mgr->register_gossip_script(CN_GUARD_SLIPKIK, (GossipScript*) new SlipkikGossip());
	mgr->register_quest_script(1193, (QuestScript *) new BrokenTrapQuest());
	mgr->register_creature_script(CN_KNOT_THIMNLEJACK, &KnotThimAI::Create);
	mgr->register_gossip_script(CN_KNOT_THIMNLEJACK, (GossipScript*) new KnotGossip());
	mgr->register_quest_script(5525, (QuestScript *) new GordokKeyQuest());
	mgr->register_quest_script(7429, (QuestScript *) new GordokKeyQuest());
	mgr->register_gameobject_script(OGRE_TANNIN_BASKET, &OgreTanninBasketAI::Create);
	mgr->register_creature_script(CN_CAPTAIN_KROMCRUSH, &CaptainKromcrushAI::Create);
	mgr->register_gossip_script(CN_CAPTAIN_KROMCRUSH, (GossipScript*) new KromcrushGossip());
	mgr->register_creature_script(CN_CHORUSH_OBSERVER, &ChorushObserverAI::Create);
	mgr->register_gossip_script(CN_CHORUSH_OBSERVER, (GossipScript*) new ChorushGossip());
	mgr->register_creature_script(CN_KING_GORDOK, &KingGordokAI::Create);
	mgr->register_creature_script(CN_MIZZLE, &MizzleAI::Create);
	mgr->register_gossip_script(CN_MIZZLE, (GossipScript*) new MizzleGossip());

	mgr->register_creature_script(CN_TENDRIS_WARPWOOD, &TendirsWarpwoodAI::Create);
	mgr->register_creature_script(CN_MAGISTER_KALENDRIS, &MagisterKalendrisAI::Create);
	mgr->register_creature_script(CN_ILLYANA_RAVENOAK, &IllyannaRavenoakAI::Create);
	mgr->register_creature_script(CN_FERRA, &FerraAI::Create);
	mgr->register_creature_script(CN_IMMOLTHAR, &ImmoltharAI::Create);
	mgr->register_creature_script(CN_EYE_OF_IMMOLTHAR, &EyeOfImmoltharAI::Create);
	mgr->register_creature_script(CN_PRINCE_TORTHELDRIN, &PrinceTortheldrinAI::Create);

	mgr->register_creature_script(CN_TSUZEE, &TsuZeeAI::Create);
//	mgr->register_creature_script(CN_EMPYREAN, &EmpyreanAI::Create);

	mgr->register_creature_script(CN_ARCANE_ABERRATION, &ArcaneAberrationAI::Create);
	mgr->register_creature_script(CN_ARCANE_FEEDBACK, &ArcaneFeedbackAI::Create);
	mgr->register_creature_script(CN_ARCANE_TORRENT, &ArcaneTorrentAI::Create);
	mgr->register_creature_script(CN_CARRION_SWARMER, &CarrionSwarmerAI::Create);
	mgr->register_creature_script(CN_DREAD_GUARD, &DreadGuardAI::Create);
	mgr->register_creature_script(CN_ELDRETH_APPARITION, &EldrethApparitionAI::Create);
	mgr->register_creature_script(CN_ELDRETH_DARTER, &EldrethDarterAI::Create);
	mgr->register_creature_script(CN_ELDRETH_PHANTASM, &EldrethPhantasmAI::Create);
	mgr->register_creature_script(CN_ELDRETH_SEETHER, &EldrethSeetherAI::Create);
	mgr->register_creature_script(CN_ELDRETH_SORCERER, &EldrethSorcererAI::Create);
	mgr->register_creature_script(CN_ELDRETH_SPECTRE, &EldrethSpectreAI::Create);
	mgr->register_creature_script(CN_ELDRETH_SPIRIT, &EldrethSpiritAI::Create);
	mgr->register_creature_script(CN_FEL_LASH, &FelLashAI::Create);
	mgr->register_creature_script(CN_GORDOK_BRUTE, &GordokBruteAI::Create);
	mgr->register_creature_script(CN_GORDOK_BUSHWHACKER, &GordokBushwhackerAI::Create);
	mgr->register_creature_script(CN_GORDOK_CAPTAIN, &GordokCaptainAI::Create);
	mgr->register_creature_script(CN_GORDOK_MAGE_LORD, &GordokMageLordAI::Create);
	mgr->register_creature_script(CN_GORDOK_MASTIFF, &GordokMastiffAI::Create);
	mgr->register_creature_script(CN_GORDOK_REAVER, &GordokReaverAI::Create);
	mgr->register_creature_script(CN_GORDOK_WARLOCK, &GordokWarlockAI::Create);
	mgr->register_creature_script(CN_HIGHBORNE_SUMMONER, &HighborneSummonerAI::Create);
	mgr->register_creature_script(CN_IRONBARK_PROTECTOR, &IronbarkProtectorAI::Create);
	mgr->register_creature_script(CN_MANA_BURST, &ManaBurstAI::Create);
	mgr->register_creature_script(CN_MANA_REMNANT, &ManaRemnantAI::Create);
	mgr->register_creature_script(CN_NETHERWALKER, &NetherwalkerAI::Create);
	mgr->register_creature_script(CN_PETRIFIED_GUARDIAN, &PetrifiedGuardianAI::Create);
	mgr->register_creature_script(CN_PETRIFIED_TREANT, &PetrifiedTreantAI::Create);
	mgr->register_creature_script(CN_PHASE_LASHER, &PhaseLasherAI::Create);
	mgr->register_creature_script(CN_RESIDUAL_MONSTROSITY, &ResidualMonstrosityAI::Create);
	mgr->register_creature_script(CN_SKELETAL_HIGHBORNE, &SkeletalHighborneAI::Create);
	mgr->register_creature_script(CN_EYE_OF_KILROGG, &EyeOfKilroggAI::Create);
	mgr->register_creature_script(CN_WARPWOOD_CRUSHER, &WarpwoodCrusherAI::Create);
	mgr->register_creature_script(CN_WARPWOOD_GUARDIAN, &WarpwoodGuardianAI::Create);
	mgr->register_creature_script(CN_WARPWOOD_STOMPER, &WarpwoodStomperAI::Create);
	mgr->register_creature_script(CN_WARPWOOD_TANGLER, &WarpwoodTanglerAI::Create);
	mgr->register_creature_script(CN_WARPWOOD_TREANT, &WarpwoodTreantAI::Create);
	mgr->register_creature_script(CN_WHIP_LASHER, &WhipLasherAI::Create);
	mgr->register_creature_script(CN_WILDSPAWN_BETRAYER, &WildspawnBetrayerAI::Create);
	mgr->register_creature_script(CN_WILDSPAWN_FELSWORN, &WildspawnFelswornAI::Create);
	mgr->register_creature_script(CN_WILDSPAWN_HELLCALLER, &WildspawnHellcallerAI::Create);
	mgr->register_creature_script(CN_WILDSPAWN_IMP, &WildspawnImpAI::Create);
	mgr->register_creature_script(CN_WILDSPAWN_ROGUE, &WildspawnRogueAI::Create);
	mgr->register_creature_script(CN_WILDSPAWN_SATYR, &WildspawnSatyrAI::Create);
	mgr->register_creature_script(CN_WILDSPAWN_SHADOWSTALKER, &WildspawnShadowstalkerAI::Create);
	mgr->register_creature_script(CN_WILDSPAWN_TRICKSTER, &WildspawnTricksterAI::Create);
}