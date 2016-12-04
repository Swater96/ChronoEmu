//Ragefire Chasm
//Forsaken Gaming <http://www.forsakengaming.com/>
//Copyright Forsaken Gaming.
//Developed by Swater

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"

// Ragefire Shaman
#define CN_RAGEFIRE_SHAMAN 11319
class RagefireShaman : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RagefireShaman, MoonScriptCreatureAI);
    RagefireShaman(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(547, Target_Self, 50, 4, 20);
		AddSpell(45287, Target_Current, 50, 2, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

// Ragefire Trogg
#define CN_RAGEFIRE_TROGG 11318
class RagefireTrogg : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RagefireTrogg, MoonScriptCreatureAI);
    RagefireTrogg(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(11998, Target_Current, 100, 2, 6);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

// Searing Blade Warlock
//#define CN_VOIDWALKER_SPAWN			500000
#define CN_SEARING_BLADE_WARLOCK 11324
class SearingBladeWarlock : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SearingBladeWarlock, MoonScriptCreatureAI);
    SearingBladeWarlock(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(705, Target_Current, 100, 2, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

// Blade Cultist
#define CN_BLADE_CULTIST 11322
class BladeCultist : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BladeCultist, MoonScriptCreatureAI);
    BladeCultist(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(1014, Target_Current, 100, 2, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};


// Molten Elemental
#define CN_MOLTEN_ELEMENTAL 11321
class MoltenElemental : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MoltenElemental, MoonScriptCreatureAI);
    MoltenElemental(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(134, Target_Self, 100, 2, 20);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

// Earthborer
#define CN_EARTHBORER 11320
class Earthborer : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Earthborer, MoonScriptCreatureAI);
    Earthborer(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(18070, Target_Current, 100, 2, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

// Blade Enforcer
#define CN_BLADE_ENFORCER 11323
class BladeEnforcer : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BladeEnforcer, MoonScriptCreatureAI);
    BladeEnforcer(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(8242, Target_Current, 100, 2, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

//BOSSES

//Jergosh The Invoker - Curse Of Weakness & Immolate
#define JERGOSH_CURSE 8552
#define JERGOSH_IMMOLATE 707

class JergoshAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(JergoshAI);
JergoshAI(Creature *pCreature) : CreatureAIScript(pCreature) {
RegisterAIUpdateEvent(1000);
}

void AIUpdate()
{
float JerCast = (float)RandomFloat(100.0f);
CastSpell(JerCast);
}

void CastSpell(float JerCast)
{
//15% chance to Immolate
if (JerCast <= 15.0f) {
_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), JERGOSH_IMMOLATE, false);
}
//10% chance to Curse Of Weakness
else if (JerCast >= 90.0f) {
_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), JERGOSH_CURSE, false);
}
}

void Destroy()
{
delete (JergoshAI*)this;
};

void OnDied()
{
RemoveAIUpdateEvent();
};
};

//Oggleflint - Cleave

#define CLEAVE 15496

class OggleflintAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(OggleflintAI);
OggleflintAI(Creature *pCreature) : CreatureAIScript(pCreature) {
RegisterAIUpdateEvent(1000);
};

void AIUpdate()
{
float OggCast = (float)RandomFloat(100.0f);
CastSpell(OggCast);
}

void CastSpell(float OggCast)
{
//10% chance to Cleave
if (OggCast <= 10.0f) {
_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), CLEAVE, true);
}

}

void Destroy()
{
delete (OggleflintAI*)this;
};

void OnDied()
{
RemoveAIUpdateEvent();
};
};

//Taragaman the Hungerer - Fire Nova & Uppercut

#define TAR_NOVA 11969
#define TAR_UPPERCUT 10966

class TaragamanAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(TaragamanAI);
TaragamanAI(Creature *pCreature) : CreatureAIScript(pCreature) {
RegisterAIUpdateEvent(1000);
};

void AIUpdate()
{
float TarCast = (float)RandomFloat(100.0f);
CastSpell(TarCast);
}

void CastSpell(float TarCast)
{
//10% chance to Fire Nova
if (TarCast <= 10.0f) {
_unit->CastSpell(_unit, TAR_NOVA, false);
}
//10% chance to Uppercut
else if (TarCast >= 90.0f) {
_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), TAR_UPPERCUT, false);
}
}

void Destroy()
{
delete (TaragamanAI*)this;
};

void OnDied()
{
RemoveAIUpdateEvent();
};
};

//Bazzalan - Poison and Sinister Strike

#define POISON 24583
#define SSTRIKE 1758

class BazzalanAI : public CreatureAIScript
{
public:
ADD_CREATURE_FACTORY_FUNCTION(BazzalanAI);
BazzalanAI(Creature *pCreature) : CreatureAIScript(pCreature) {
RegisterAIUpdateEvent(1000);
};

void AIUpdate()
{
float BazCast = (float)RandomFloat(100.0f);
CastSpell(BazCast);
}

void CastSpell(float BazCast)
{
//5% chance to cast "Scorpid Poison 2"
if (BazCast <= 5.0f) {
_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), POISON, true);
}
//5% chance to cast "Sinister Strike (Rank 3)"
else if (BazCast >= 95.0f) {
_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), SSTRIKE, true);
}
}

void Destroy()
{
delete (BazzalanAI*)this;
};

void OnDied()
{
RemoveAIUpdateEvent();
};

};

void SetupRagefireChasm(ScriptMgr * mgr)
{
mgr->register_creature_script(11519, &BazzalanAI::Create);
mgr->register_creature_script(11518, &JergoshAI::Create);
mgr->register_creature_script(11517, &OggleflintAI::Create);
mgr->register_creature_script(11520, &TaragamanAI::Create);
mgr->register_creature_script(CN_SEARING_BLADE_WARLOCK, &SearingBladeWarlock::Create);
mgr->register_creature_script(CN_EARTHBORER, &Earthborer::Create);
mgr->register_creature_script(CN_MOLTEN_ELEMENTAL, &MoltenElemental::Create);
mgr->register_creature_script(CN_RAGEFIRE_SHAMAN, &RagefireShaman::Create);
mgr->register_creature_script(CN_RAGEFIRE_TROGG, &RagefireTrogg::Create);
mgr->register_creature_script(CN_BLADE_CULTIST, &BladeCultist::Create);
mgr->register_creature_script(CN_BLADE_ENFORCER, &BladeEnforcer::Create);
}