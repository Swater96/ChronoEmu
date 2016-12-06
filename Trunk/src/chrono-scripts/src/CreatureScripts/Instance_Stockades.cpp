/*
 * ChronoEmu Scripts - The Stockades
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by Swater
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"

//BOSSES

#define CN_BRAZIL_THREDD 1716
class BrazilThredd : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(BrazilThredd, MoonScriptCreatureAI);
    BrazilThredd(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(13728, Target_Self, 50, 0, 120);
		AddSpell(13729, Target_RandomPlayer, 50, 2, 20);
	}

	void OnCombatStart(Unit* pTarget)
	{
		Emote("Tell the Warden this prison is ours now!", Text_Say);
		ParentClass::OnCombatStart(pTarget);
	}
};

#define CN_DEXTREN_WARD 1663
class DextrenWard : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DextrenWard, MoonScriptCreatureAI);
    DextrenWard(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(7165, Target_Self, 33, 0, 120);
		AddSpell(19134, Target_Current, 33, 0, 15);
		AddSpell(11976, Target_Current, 33, 0, 10);
	}
};

#define CN_TARGORR_THE_DREAD 1696
class TargorrTheDread : public MoonScriptCreatureAI
{
	bool enraged;
	SpellDesc *enrage;
public:
	MOONSCRIPT_FACTORY_FUNCTION(TargorrTheDread, MoonScriptCreatureAI);
    TargorrTheDread(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		enrage = AddSpell(8599, Target_Self, 0, 0, 0);
		AddSpell(3391, Target_Current, 50, 0, 8);
		enraged = false;
	}

	void AIUpdate() {
		if(!enraged && GetHealthPercent() < 50) {
			enraged = true;
			CastSpellNowNoScheduling(enrage);
		}
		ParentClass::AIUpdate();
	}
};

#define CN_KAM_DEEPFURY 1666
class KamDeepfury : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(KamDeepfury, MoonScriptCreatureAI);
    KamDeepfury(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(8242, Target_Current, 50, 0, 8);
		AddSpell(871, Target_Self, 8, 0, 30);
		AddSpell(7164, Target_Self, 10, 0, 60);
	}
};

#define CN_HAMHOCK 1717
class Hamhock : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Hamhock, MoonScriptCreatureAI);
    Hamhock(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(6742, Target_WoundedFriendly, 10, 0, 0);
		AddSpell(421, Target_Current, 50, 2, 7);
	}
	void OnCombatStart(Unit* pTarget)
	{
		Emote("Me smash! You die!", Text_Say);
		ParentClass::OnCombatStart(pTarget);
	}
};

#define CN_BRUEGAL_IRONBUCKLE 1720
class BruegalIronbuckleAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(BruegalIronbuckleAI, MoonScriptCreatureAI);
    BruegalIronbuckleAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
	}
	void OnCombatStart(Unit* pTarget)
	{
		Emote("Fresh meat!", Text_Say);
		ParentClass::OnCombatStart(pTarget);
	}
};

//TRASH

#define CN_DEFIAS_CONVICT 1711
class DefiasConvict : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DefiasConvict, MoonScriptCreatureAI);
    DefiasConvict(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(6253, Target_Current, 40, 0, 8);
	}
};

#define CN_DEFIAS_INMATE 1708
class DefiasInmate : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DefiasInmate, MoonScriptCreatureAI);
    DefiasInmate(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(7165, Target_Self, 50, 0, 60);
		AddSpell(6547, Target_Current, 40, 0, 8);
	}
};

#define CN_DEFIAS_PRISONER 1706
class DefiasPrisoner : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DefiasPrisoner, MoonScriptCreatureAI);
    DefiasPrisoner(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(6713, Target_Current, 20, 0, 15);
		AddSpell(1766, Target_Current, 40, 0, 10);
	}
};

#define CN_DEFIAS_CAPTIVE 1707
class DefiasCaptive : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DefiasCaptive, MoonScriptCreatureAI);
    DefiasCaptive(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(7159, Target_Current, 30, 0, 10);
		AddSpell(3427, Target_RandomPlayer, 40, 0, 15);
	}
};

#define CN_DEFIAS_INSURGENT 1715
class DefiasInsurgent : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DefiasInsurgent, MoonScriptCreatureAI);
    DefiasInsurgent(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(9128, Target_Self, 20, 0, 60);
		AddSpell(13730, Target_Self, 20, 0, 60);
	}
};

void SetupStockades(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_BRAZIL_THREDD, &BrazilThredd::Create);
	mgr->register_creature_script(CN_DEXTREN_WARD, &DextrenWard::Create);
	mgr->register_creature_script(CN_TARGORR_THE_DREAD, &TargorrTheDread::Create);
	mgr->register_creature_script(CN_KAM_DEEPFURY, &KamDeepfury::Create);
	mgr->register_creature_script(CN_HAMHOCK, &Hamhock::Create);
	mgr->register_creature_script(CN_BRUEGAL_IRONBUCKLE, &BruegalIronbuckleAI::Create);
	mgr->register_creature_script(CN_DEFIAS_CONVICT, &DefiasConvict::Create);
	mgr->register_creature_script(CN_DEFIAS_INMATE, &DefiasInmate::Create);
	mgr->register_creature_script(CN_DEFIAS_PRISONER, &DefiasPrisoner::Create);
	mgr->register_creature_script(CN_DEFIAS_CAPTIVE, &DefiasCaptive::Create);
	mgr->register_creature_script(CN_DEFIAS_INSURGENT, &DefiasInsurgent::Create);
}