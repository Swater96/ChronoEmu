/*
 * ChronoEmu Scripts -Onyxia's Lair
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by Sphere
 */
 
#include "StdAfx.h"
#include "Setup.h"
#include "../common/Base.h"

#define BOSS_ONYXIA				10184

//Phase 1 & 3
#define ONYXIA_FLAMEBREATH		18435
#define ONYXIA_KNOCKAWAY		19633
#define ONYXIA_WINGBUFFET		18500
#define ONYXIA_CLEAVE			19983
#define ONYXIA_TAILSWEEP		15847

// Phase 3 Only
#define ONYXIA_FEAR				18431

// Phase 2 Only
#define ONYXIA_FIREBALL			18392
#define ONYXIA_ENTAGLINGFLAMES	20019
#define ONYXIA_DEEPBREATH		17086

class OnyxiaAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(OnyxiaAI, MoonScriptBossAI);
	OnyxiaAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
	{
		// Phase 1
		AddPhaseSpell(1, AddSpell(ONYXIA_FLAMEBREATH, Target_Self, 15, 0, 0));
		AddPhaseSpell(1, AddSpell(ONYXIA_KNOCKAWAY, Target_Self, 15, 0, 0));
		AddPhaseSpell(1, AddSpell(ONYXIA_WINGBUFFET, Target_Self, 15, 0, 0));
		AddPhaseSpell(1, AddSpell(ONYXIA_CLEAVE, Target_Self, 15, 0, 0));
		AddPhaseSpell(1, AddSpell(ONYXIA_TAILSWEEP, Target_Self, 15, 0, 0));
		
		// Phase 2
		AddPhaseSpell(2, AddSpell(ONYXIA_FIREBALL, Target_Self, 15, 0, 0));
		AddPhaseSpell(2, AddSpell(ONYXIA_ENTAGLINGFLAMES, Target_Self, 15, 0, 0));
		AddPhaseSpell(2, AddSpell(ONYXIA_DEEPBREATH, Target_Self, 15, 0, 0));
		
		// Phase 3
		AddPhaseSpell(3, AddSpell(ONYXIA_FLAMEBREATH, Target_Self, 15, 0, 0));
		AddPhaseSpell(3, AddSpell(ONYXIA_KNOCKAWAY, Target_Self, 15, 0, 0));
		AddPhaseSpell(3, AddSpell(ONYXIA_WINGBUFFET, Target_Self, 15, 0, 0));
		AddPhaseSpell(3, AddSpell(ONYXIA_CLEAVE, Target_Self, 15, 0, 0));
		AddPhaseSpell(3, AddSpell(ONYXIA_TAILSWEEP, Target_Self, 15, 0, 0));
		AddPhaseSpell(3, AddSpell(ONYXIA_FEAR, Target_Self, 15, 0, 0));
	}
	
	void OnCombatStart(Unit *pTarget)
	{
		Emote("How fortuitous, usually I must leave my lair to feed!", Text_Yell, 0);
		SetPhase(1);
		ParentClass::OnCombatStart(pTarget);
	}
	
	void AIUpdate()
	{
		if(GetHealthPercent() <= 65 && GetPhase() == 1)
		{
			_unit->Emote(EMOTE_ONESHOT_LAND);

			WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
			data << _unit->GetNewGUID();
			data << uint32(0);
			_unit->SendMessageToSet(&data, false);		
			SetPhase(2);
		}
	}
};


void SetupOnyxia(ScriptMgr * mgr)
{
    mgr->register_creature_script(BOSS_ONYXIA, 				&OnyxiaAI::Create);
};