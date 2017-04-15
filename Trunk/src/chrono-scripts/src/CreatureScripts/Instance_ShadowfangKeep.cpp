/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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
/* Instance_ShadowfangKeep.cpp Script									*/	
/* Events scripted by SideWays											*/
/************************************************************************/


class SCRIPT_DECL DSAdamantGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		/*"At last! Someone to free me from this cell! High Executor Hadrec sent us to gather information on the Keep so that a plan could be formulated to overthrow Arugal once and for all. 
		But the old wizard has many tricks up his sleeve and we were detected by a magical ward. I was thrown in this prison. Vincent was not so lucky. I must return to Hadrec to debrief him at once. 
		But first I will pick the lock to the courtyard door for you. Perhaps you can try your luck against the foes that lurk beyond." */
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 500 , Plr);
		
		Menu->AddItem(0, "Please unlock the courtyard door.", 1);
		if(AutoSend)
			Menu->SendTo(Plr);
	}
	
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
			if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;
		switch(IntId)
		{
			case 1:
				Creature* Adamant = Plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-243.712006f, 2113.719971f, 81.262901f, 3849);

				Adamant->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Free from this wretched cell at last! Let me show you the courtyard..");
				Adamant->GetAIInterface()->StopMovement(0);
				Adamant->SetUInt32Value(UNIT_NPC_FLAGS, 0);
				Adamant->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				Adamant->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				Adamant->GetAIInterface()->setWaypointToMove(1);
				Adamant->GetAIInterface()->SetAllowedToEnterCombat(false);
				Adamant->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
				break;
		}
	}
	void Destroy()
	{
		delete this;
	}
};

class SCRIPT_DECL SorcererAshcrombeGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		//needs text
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1 , Plr);
		
		Menu->AddItem(0, "Please unlock the courtyard door.", 1);
		if(AutoSend)
			Menu->SendTo(Plr);
	}
	
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
			case 0:
				GossipHello(pObject, Plr, true);
				break;
			case 1:
				Creature* Sorcerer = Plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-234.673996f, 2134.550049f, 81.179604f, 3850);
				
				Sorcerer->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Follow me and I'll open the courtyard for you.");
				Sorcerer->GetAIInterface()->StopMovement(0);
				Sorcerer->SetUInt32Value(UNIT_NPC_FLAGS, 0);
				Sorcerer->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				Sorcerer->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				Sorcerer->GetAIInterface()->setWaypointToMove(1);
				Sorcerer->GetAIInterface()->SetAllowedToEnterCombat(false);
				Sorcerer->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
				Sorcerer->m_noRespawn = true;

				break;
		}
	}
	void Destroy()
	{
		delete this;
	}
};

#define GO_FLAG_UNCLICKABLE 0x00000010
static Coords HDoorEventWP[]=
{
	{ -255.971085f, 2118.124023f, 81.180519f, 1.357309f },
	{ -253.193481f, 2130.936523f, 81.213432f, 1.357309f },
	{ -249.597412f, 2142.019531f, 86.892159f, 1.284267f },
	{ -247.486389f, 2144.292969f, 87.012962f, 6.046922f },
	{ -239.587479f, 2140.958008f, 87.012985f, 0.205130f },
	{ -234.684799f, 2149.650391f, 90.624077f, 1.202585f },
	{ -240.517242f, 2154.806641f, 90.624077f, 1.931442f },
	{ -209.106735f, 2141.027832f, 90.626022f, 0.238909f },
	{ -204.393784f, 2147.800049f, 88.740463f, 1.028234f }
};

#define PICK_LOCK 1804

class DSAdamantAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DSAdamantAI, MoonScriptCreatureAI);
	DSAdamantAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		m_entry = pCreature->GetEntry();
		
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(8, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(9, 0, RUN));

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);

	}
	void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		if(iWaypointId == 7)
		{
			RegisterAIUpdateEvent(5500);
			_unit->CastSpell(_unit, PICK_LOCK, false);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, "Deathstalker Adamant fumbles with the rusty lock on the courtyard door.");
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You are indeed courageous for wanting to brave the horrors that lie beyond this door.");
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);	
		}
		if(iWaypointId == 9)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			Despawn(2000);
		}
		ParentClass::OnReachWP(iWaypointId, bForwards);
	}
	void AIUpdate()
	{
		GameObject* CourtyardDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-240.956085f, 2157.637939f, 90.624008f, 18895);
		CourtyardDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		CourtyardDoor->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "There we go!\n");
		_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Good luck with Arugal. I must hurry back to Hadrec now.");
		RemoveAIUpdateEvent();	
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
		_unit->GetAIInterface()->setWaypointToMove(7);
	}
	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = HDoorEventWP[id].mX;
		wp->y = HDoorEventWP[id].mY;
		wp->z = HDoorEventWP[id].mZ;
		wp->o = HDoorEventWP[id].mO;
		wp->waittime = waittime;
		wp->flags = flags;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		return wp;
	}
protected:
	uint32 m_entry;
};

static Coords ADoorEventWP[]=
{
	{ -247.321899f, 2134.694580f, 81.179642f, 2.977588f },
	{ -250.142426f, 2128.632813f, 81.179642f, -2.056191f },
	{ -253.193481f, 2130.936523f, 81.213432f, 1.357309f },
	{ -249.597412f, 2142.019531f, 86.892159f, 1.284267f },
	{ -247.486389f, 2144.292969f, 87.012962f, 6.046922f },
	{ -239.587479f, 2140.958008f, 87.012985f, 0.205130f },
	{ -234.684799f, 2149.650391f, 90.624077f, 1.202585f },
	{ -241.551941f, 2157.413818f, 90.624077f, 1.528526f }
};

#define UNLOCK 6421

class SorcererAshcrombeAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SorcererAshcrombeAI, MoonScriptCreatureAI);
	SorcererAshcrombeAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{		
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, WALK));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, RUN));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(8, 0, RUN));

		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
	}
	void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		if(iWaypointId == 8)
		{
			_unit->CastSpell(_unit, UNLOCK, false);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I have just the spell to get this door open. Too bad the cell doors weren't locked so haphazardly.");
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		}
		RegisterAIUpdateEvent(4500);
		ParentClass::OnReachWP(iWaypointId, bForwards);
	}
	void AIUpdate()
	{
			GameObject* CourtyardDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-240.956085f, 2157.637939f, 90.624008f, 18895);
			CourtyardDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
			CourtyardDoor->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "There it is! Wide open. Good luck to you conquering what lies beyond. I must report back to the Kirin Tor at once! And poof, he's gone...");
			Despawn(2000);
			RemoveAIUpdateEvent();
	}

	inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
	{
		WayPoint * wp = _unit->CreateWaypointStruct();
		wp->id = id;
		wp->x = ADoorEventWP[id].mX;
		wp->y = ADoorEventWP[id].mY;
		wp->z = ADoorEventWP[id].mZ;
		wp->o = ADoorEventWP[id].mO;
		wp->waittime = waittime;
		wp->flags = flags;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		return wp;
	}
protected:
	uint32 m_entry;
};
 
class BaronSilverlaine : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BaronSilverlaine, MoonScriptCreatureAI);
	BaronSilverlaine(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		//Veil of Shadow
		AddSpell(7068, Target_RandomPlayer, 10, 1.5f, 0);
	}
};

// Commander Springvale AI
#define CN_SPRINGVALE 4278
class SpringvaleAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SpringvaleAI, MoonScriptCreatureAI);
    SpringvaleAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Holy Light -- heals the player ^.^
		AddSpell(1026, Target_Self, 10, 2.5, 0);
		// Devotion Aura (Not the right Spell ID)
		DevoAura = AddSpell(10290, Target_Self, 0, 0, 0);
		// Divine Protection (Not the right Spell ID)
		DivineProt = AddSpell(498, Target_Self, 0, 0, 0);
		// Hammer of Justice
		AddSpell(5588, Target_Current, 12, 0, 15);
    }
    
    void OnCombatStart(Unit* pTarget)
    {
		if(!GetUnit()->HasAura(DevoAura->mInfo->Id))
			CastSpellNowNoScheduling(DevoAura);

		ParentClass::OnCombatStart(pTarget);
    }

    void AIUpdate()
    {
		if(GetHealthPercent() <= 20 && DivineProt->mEnabled == true)
        {
			CastSpellNowNoScheduling(DivineProt);
			DivineProt->mEnabled = false;
		}
		ParentClass::AIUpdate();
    }
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }

	SpellDesc* DevoAura;
	SpellDesc* DivineProt;
};

// Odo the Blindwatcher AI
#define CN_BLINDWATCHER 4279
class BlindWatcherAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(BlindWatcherAI, MoonScriptBossAI);
    BlindWatcherAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
    {
		// Howling Rage 1
		HowlingRage1 = AddSpell(7481, Target_Self, 0, 5, 0);
		// Howling Rage 2
		HowlingRage2 = AddSpell(7483, Target_Self, 0, 1.5f, 0);
		// Howling Rage 3
		HowlingRage3 = AddSpell(7484, Target_Self, 0, 1.5f, 0);
    }

    void AIUpdate()
    {
        if(GetHealthPercent() <= 75 && GetPhase() == 1)
        {
			SetPhase(2, HowlingRage1);
		}
		else if(GetHealthPercent() <= 45 && GetPhase() == 2)
        {
			if(GetUnit()->HasAura(7481))
				RemoveAura(7481);
			SetPhase(3, HowlingRage2);
		}
		else if(GetHealthPercent() <= 20 && GetPhase() == 3)
        {
			if(GetUnit()->HasAura(7483))
				RemoveAura(7483);
			SetPhase(4, HowlingRage3);
		}
		ParentClass::AIUpdate();
    }
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }

	SpellDesc* HowlingRage1;
	SpellDesc* HowlingRage2;
	SpellDesc* HowlingRage3;
};

// Fenrus the Devourer AI
#define CN_FENRUS 4274
static Coords VWSpawns[] = {
	{}, // Spawn Locations for the 4 voidwalkers
	{-154.274368f, 2177.196533f, 128.448517f, 5.760980f},
	{-142.647537f, 2181.019775f, 128.448410f, 4.178475f},
	{-139.146774f, 2168.201904f, 128.448364f, 2.650803f},
	{-150.860092f, 2165.156250f, 128.448502f, 0.999966f},
};
class FenrusAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FenrusAI, MoonScriptCreatureAI);
	FenrusAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7125, Target_RandomPlayer, 12, 1.5f, 15);
		//AddEmote(Event_OnCombatStart, "text", 6017);
	}

	void OnDied(Unit*  pKiller)
	{
		GetUnit()->SendChatMessageAlternateEntry(4275, CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who dares interfere with the Sons of Arugal?");
		GetUnit()->PlaySoundToSet(5791);

		MoonScriptCreatureAI* voidwalker = nullptr;
		// Spawn 4 x Arugal's Voidwalkers
		for(int x=1; x<5; x++)
		{
			voidwalker = SpawnCreature(4627, VWSpawns[x].mX, VWSpawns[x].mY, VWSpawns[x].mZ, VWSpawns[x].mO);
			if(voidwalker)
			{
				voidwalker->AggroNearestPlayer(100);
				voidwalker = nullptr;
			}
		}

		ParentClass::OnDied(pKiller);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

//Arugals Voidwalkers
#define CN_VOIDWALKER 4627
class VoidWalkerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VoidWalkerAI, MoonScriptCreatureAI);
	VoidWalkerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7154, Target_WoundedFriendly, 5, 0, 7);
	}

	void OnDied(Unit*  pKiller)
	{
		GameObject*  pDoor = GetUnit()->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-129.034f, 2166.16f, 129.187f, 18972);
		if(pDoor)
			pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
	
		ParentClass::OnDied(pKiller);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

// Archmage Arugal AI
#define CN_ARUGAL 4275
class ArugalAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ArugalAI, MoonScriptCreatureAI);
    ArugalAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Void Bolt
		AddSpell(7588, Target_Current, 25, 3, 0);
		// Thunder Shock
		AddSpell(7803, Target_Self, 10, 0, 3);
		// Arugal's Curse
		AddSpell(7621, Target_RandomPlayer, 5, 0, 3, 0, 0, false, "Release your rage!", Text_Yell, 5797);
		// Arugal spawn-in spell (Teleport) --- THIS SPELL NEEDS SERIOUS TESTING, IT WORKS LIKE 50% OF THE TIME ^^
		AddSpell(10418, Target_Self, 5, 2, 5);

		AddEmote(Event_OnCombatStart, "You, too, shall serve!", Text_Yell, 5793);
		AddEmote(Event_OnTargetDied, "Another falls!", Text_Yell, 5795);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

//Wolf Master Nandos AI
#define CN_NANDOS 3927
class NandosAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(NandosAI, MoonScriptCreatureAI);
    NandosAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature){}

    void OnDied(Unit*  pKiller)
    {
		GameObject*  pDoor = GetUnit()->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-118.11f, 2161.86f, 155.678f, 18971);
		if(pDoor)		
			pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);

		ParentClass::OnDied(pKiller);
    }
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

//Rethilgore AI
#define CN_RETHILGORE 3914
class RETHILGOREAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(RETHILGOREAI, MoonScriptCreatureAI);
    RETHILGOREAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		//Life Drain
		AddSpell(7295, Target_Current, 15, 2, 0);
	}
    void OnDied(Unit*  pKiller)
    {
		//workaround if Gossip-scripts fail
		/*GameObject* CourtyardDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-240.956085f, 2157.637939f, 90.624008f, 18895);
		CourtyardDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		CourtyardDoor->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);*/
		_unit->SendChatMessageAlternateEntry(3849, CHAT_MSG_MONSTER_SAY, LANG_ORCISH, "About time someone killed the wretch.");
		_unit->SendChatMessageAlternateEntry(3850, CHAT_MSG_MONSTER_SAY, LANG_COMMON, "For once I agree with you... scum.");  
		ParentClass::OnDied(pKiller);
    }
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

//Trash
#define BLEAK_WORG 3861
class BleakWorg : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(BleakWorg, MoonScriptCreatureAI);
    BleakWorg(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7127, Target_Current, 10, 0, 60);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define BLOOD_SEEKER 3868
class BloodSeeker : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(BloodSeeker, MoonScriptCreatureAI);
    BloodSeeker(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7140, Target_Current, 10, 0, 25);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define FEL_STEED 3864
class FelSteed : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(FelSteed, MoonScriptCreatureAI);
    FelSteed(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7139, Target_Current, 10, 0, 20);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define HAUNTED_SERVITOR 3875
class HauntedServitor : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(HauntedServitor, MoonScriptCreatureAI);
    HauntedServitor(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7057, Target_Current, 10, 2, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define LUPINE_HORROR 3863
class LupineHorror : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(LupineHorror, MoonScriptCreatureAI);
    LupineHorror(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7132, Target_Self, 10, 1, 210);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SHADOWFANG_DARKSOUL 3855
class ShadowfangDarksoul : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShadowfangDarksoul, MoonScriptCreatureAI);
    ShadowfangDarksoul(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(970, Target_Current, 10, 0, 10);
		AddSpell(8140, Target_RandomPlayer, 10, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SHADOWFANG_GLUTTON 3857
class ShadowfangGlutton : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShadowfangGlutton, MoonScriptCreatureAI);
    ShadowfangGlutton(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7122, Target_Current, 10, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SHADOWFANG_MOONWALKER 3853
class ShadowfangMoonwalker : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShadowfangMoonwalker, MoonScriptCreatureAI);
    ShadowfangMoonwalker(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7121, Target_Self, 10, 2, 13);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SHADOWFANG_RAGETOOTH 3859
class ShadowfangRagetooth : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShadowfangRagetooth, MoonScriptCreatureAI);
    ShadowfangRagetooth(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7072, Target_Self, 10, 0, 60);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SHADOWFANG_WOLFGUARD 3854
class ShadowfangWolfguard : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShadowfangWolfguard, MoonScriptCreatureAI);
    ShadowfangWolfguard(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7106, Target_Self, 10, 2, 7);
		AddSpell(7107, Target_Self, 10, 0, 15);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SHADOWFANG_WHITESCALP 3851
class ShadowfangWhitescalp : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShadowfangWhitescalp, MoonScriptCreatureAI);
    ShadowfangWhitescalp(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(12544, Target_Self, 10, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SON_OF_ARUGAL 2529
class SonOfArugal : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SonOfArugal, MoonScriptCreatureAI);
    SonOfArugal(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7124, Target_Current, 10, 2.5f, 30);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define TORMENTED_OFFICER 3873
class TormentedOfficer : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(TormentedOfficer, MoonScriptCreatureAI);
    TormentedOfficer(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7054, Target_Current, 10, 2.5f, 30);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define VILE_BAT 3866
class VileBat : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(VileBat, MoonScriptCreatureAI);
    VileBat(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7145, Target_Current, 10, 0, 10);
		AddSpell(6713, Target_Current, 10, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define WAILING_GUARDSMAN 3877
class WailingGuardsman : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(WailingGuardsman, MoonScriptCreatureAI);
    WailingGuardsman(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(7074, Target_Current, 10, 0, 8);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

void SetupShadowfangKeep(ScriptMgr * mgr)
{
	//creature scripts
	mgr->register_creature_script(3887, &BaronSilverlaine::Create);
	mgr->register_creature_script(3849, &DSAdamantAI::Create);
	GossipScript * dsGossip = (GossipScript*) new DSAdamantGossip();
	mgr->register_gossip_script( 3849, dsGossip );
	mgr->register_creature_script(3850, &SorcererAshcrombeAI::Create);
	GossipScript * saGossip = (GossipScript*) new SorcererAshcrombeGossip();
	mgr->register_gossip_script( 3850, saGossip );
	mgr->register_creature_script(CN_NANDOS, &NandosAI::Create);
	mgr->register_creature_script(CN_VOIDWALKER, &VoidWalkerAI::Create);
	mgr->register_creature_script(CN_RETHILGORE, &RETHILGOREAI::Create);
	mgr->register_creature_script(CN_SPRINGVALE, &SpringvaleAI::Create);
	mgr->register_creature_script(CN_BLINDWATCHER, &BlindWatcherAI::Create);
	mgr->register_creature_script(CN_FENRUS, &FenrusAI::Create);
	mgr->register_creature_script(CN_ARUGAL, &ArugalAI::Create);
	//Trash
	mgr->register_creature_script(WAILING_GUARDSMAN, &WailingGuardsman::Create);
	mgr->register_creature_script(VILE_BAT, &VileBat::Create);
	mgr->register_creature_script(TORMENTED_OFFICER, &TormentedOfficer::Create);
	mgr->register_creature_script(SON_OF_ARUGAL, &SonOfArugal::Create);
	mgr->register_creature_script(SHADOWFANG_WHITESCALP, &ShadowfangWhitescalp::Create);
	mgr->register_creature_script(SHADOWFANG_WOLFGUARD, &ShadowfangWolfguard::Create);
	mgr->register_creature_script(SHADOWFANG_RAGETOOTH, &ShadowfangRagetooth::Create);
	mgr->register_creature_script(SHADOWFANG_MOONWALKER, &ShadowfangMoonwalker::Create);
	mgr->register_creature_script(SHADOWFANG_GLUTTON, &ShadowfangGlutton::Create);
	mgr->register_creature_script(SHADOWFANG_DARKSOUL, &ShadowfangDarksoul::Create);
	mgr->register_creature_script(LUPINE_HORROR, &LupineHorror::Create);
	mgr->register_creature_script(HAUNTED_SERVITOR, &HauntedServitor::Create);
	mgr->register_creature_script(FEL_STEED, &FelSteed::Create);
	mgr->register_creature_script(BLOOD_SEEKER, &BloodSeeker::Create);
	mgr->register_creature_script(BLEAK_WORG, &BleakWorg::Create);
}
