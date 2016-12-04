/*
 * ChronoEmu Scripts - Stormwind
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by Maclof
 */

#include "StdAfx.h"
#include "../Setup.h"
#include "../../Common/Base.h"

class Bartleby : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Bartleby, MoonScriptCreatureAI);
    Bartleby(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(9956, Target_Self, 30, 2, 25);
	}

	void OnDamageTaken(Unit * mAttacker, float fAmount)
	{
		if(_unit->GetUInt32Value(UNIT_FIELD_HEALTH)- fAmount<=_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.2)
		{
			if(mAttacker->GetTypeId() == TYPEID_PLAYER)
			{
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				RegisterAIUpdateEvent(1000);
				QuestLogEntry *qle = (TO_PLAYER(mAttacker))->GetQuestLogForEntry( 1640 );
				if(!qle)
					return;
				qle->SendQuestComplete();
			}
		}
	}

	void AIUpdate()
	{
		if(_unit->GetUInt32Value(UNIT_FIELD_HEALTH) <= _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.2)
		{
			RemoveAllAuras();
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 12);
			_unit->SetHealthPct(100);      
			_unit->GetAIInterface()->WipeTargetList();         
			_unit->GetAIInterface()->WipeHateList();
			_unit->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, _unit, 0);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
			RemoveAIUpdateEvent();
		}
	}
};

void SetupStormwindNPC(ScriptMgr * mgr)
{
	mgr->register_creature_script(6090, &Bartleby::Create);
}