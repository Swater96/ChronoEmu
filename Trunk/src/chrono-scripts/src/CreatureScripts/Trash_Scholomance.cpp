/*
 * ChronoEmu Scripts - Scholomance Trash
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by SideWays
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"
#include "Scholomance.h"

#define CN_ASPECTOFBANALITY 14518
class AspectOfBanality : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AspectOfBanality, MoonScriptCreatureAI);
    AspectOfBanality(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(8140, Target_RandomPlayer, 30, 0, 3);
		AddSpell(16247, Target_RandomPlayer, 20, 2, 15);
		AddSpell(16244, Target_Current, 24, 0, 5);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_ASPECTOFCORRUPTION 18376
class AspectOfCorruption : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AspectOfCorruption, MoonScriptCreatureAI);
    AspectOfCorruption(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(18376, Target_RandomPlayer, 21, 2, 20);
		AddSpell(23268, Target_RandomPlayer, 29, 0, 5);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_ASPECTOFMALICE 14520
class AspectOfMalice : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AspectOfMalice, MoonScriptCreatureAI);
    AspectOfMalice(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(16098, Target_RandomPlayer, 30, 2, 15);
		AddSpell(13738, Target_Current, 24, 0, 4);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_ASPECTOFSHADOW 14521
class AspectOfShadow : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AspectOfShadow, MoonScriptCreatureAI);
    AspectOfShadow(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(15472, Target_RandomPlayer, 24, 3, 1);
		AddSpell(17228, Target_Current, 20, 0, 4);
		AddSpell(22417, Target_Self, 16, 0, 15);
		AddSpell(17289, Target_Current, 23, 0, 3);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BANALSPIRIT 14514
class BanalSpirit : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BanalSpirit, MoonScriptCreatureAI);
    BanalSpirit(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(8140, Target_RandomPlayer, 10, 0, 0);
		AddSpell(16244, Target_Current, 15, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_BLOODSTEWARD 14861
class BloodSteward : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BloodSteward, MoonScriptCreatureAI);
    BloodSteward(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(22371, Target_RandomPlayer, 17, 0, 15);
		AddSpell(12493, Target_Current, 16, 0, 15);
		AddSpell(3609, Target_Current, 24, 0, 11);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_BONEMINION 16119
class BoneMinion : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BoneMinion, MoonScriptCreatureAI);
    BoneMinion(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_CORRUPTEDSPIRIT 14512
class CorruptedSpirit : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CorruptedSpirit, MoonScriptCreatureAI);
    CorruptedSpirit(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(18376, Target_RandomPlayer, 34, 2, 5);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_DISEASEDGHOUL 10495
class DiseasedGhoul : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DiseasedGhoul, MoonScriptCreatureAI);
    DiseasedGhoul(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		ApplyAura(12627);
	}
	void OnDied(Unit* pTarget)
	{
		_unit->CastSpell(_unit, 17742, false);
		ParentClass::OnDied(pTarget);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_MALICIOUSSPIRIT 14513
class MaliciousSpirit : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MaliciousSpirit, MoonScriptCreatureAI);
    MaliciousSpirit(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(13738, Target_Current, 45, 0, 4);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_NECROFIEND 11551
class NecroFiend : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(NecroFiend, MoonScriptCreatureAI);
    NecroFiend(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(15474, Target_Current, 34, 0, 15);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_PLAGUEDHATCHLING 10678
class PlaguedHatchling : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PlaguedHatchling, MoonScriptCreatureAI);
    PlaguedHatchling(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(17745, Target_Current, 20, 3, 0.5);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_RISENBONEWARDER 10491
class RisenBonewarder : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RisenBonewarder, MoonScriptCreatureAI);
    RisenBonewarder(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		BoneArmor = AddSpell(16431, Target_Self, 15, 0, 15);
		AddSpell(17715, Target_Self, 100, 0, 20);
		AddSpell(17620, Target_Current, 24, 5, 1);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
SpellDesc* BoneArmor;
};

#define CN_RISENABERRATION 10485
class RisenAberration : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RisenAberration, MoonScriptCreatureAI);
    RisenAberration(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(12021, Target_SecondMostHated, 19, 0, 5);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_RISENCONSTRUCT 10488
class RisenConstruct : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RisenConstruct, MoonScriptCreatureAI);
    RisenConstruct(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(16169, Target_Current, 24, 0, 5);
		Frenzy = AddSpell(8269, Target_Self, 0, 0, 0);
		AddSpell(3391, Target_Self, 29, 0, 4);
	}
	void OnCombatStart(Unit* pTarget)
	{
		frenzyb = true;
		ParentClass::OnCombatStart(pTarget);
	}
	void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 60 && frenzyb == true)
		{
			CastSpellNowNoScheduling(Frenzy);
			frenzyb = false;
		}
		ParentClass::AIUpdate();
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }

SpellDesc* Frenzy;
protected:
	bool frenzyb;
};

#define CN_RISENGUARD 10489
class RisenGuard : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RisenGuard, MoonScriptCreatureAI);
    RisenGuard(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(15655, Target_Current, 19, 0, 6);
		AddSpell(15572, Target_Current, 24, 0, 4);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_RISENGUARDIAN 11598
class RisenGuardian : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RisenGuardian, MoonScriptCreatureAI);
    RisenGuardian(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(11443, Target_Current, 20, 0, 15);
		AddSpell(16583, Target_Current, 24, 0, 4);
		AddSpell(7068, Target_Current, 17, 0, 15);
		AddSpell(7289, Target_Current, 18, 0, 60);
		AddSpell(4974, Target_Current, 21, 0, 30);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_RISENPROTECTOR 10487
class RisenProtector : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RisenProtector, MoonScriptCreatureAI);
    RisenProtector(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(17439, Target_Current, 23, 0, 4);
		AddSpell(15654, Target_Current, 19, 0, 10);
		AddSpell(6726, Target_RandomPlayer, 25, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_RISENWARRIOR 10486
class RisenWarrior : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RisenWarrior, MoonScriptCreatureAI);
    RisenWarrior(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(11428, Target_Current, 13, 0, 8);
		AddSpell(16509, Target_Current, 15, 0, 5);
		AddSpell(14516, Target_Current, 21, 0, 4);
		AddSpell(15588, Target_Current, 25, 0, 6);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_SCHOLOACOLYTE 10471
class ScholoAcolyte : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ScholoAcolyte, MoonScriptCreatureAI);
    ScholoAcolyte(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		ApplyAura(16592);
		AddSpell(17165, Target_Current, 30, 3, 1);
		AddSpell(17615, Target_RandomPlayer, 24, 0, 5);
		DarkMending = AddSpell(17613, Target_Self, 0, 3.5, 8);
		AddSpell(11443, Target_Current, 19, 3, 10);
	}
	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
	}
	void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 60)
		{
			CastSpellNowNoScheduling(DarkMending);
		}
		ParentClass::AIUpdate();
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
//SpellDesc* ShadowForm;
SpellDesc* DarkMending;
};

#define CN_SCHOLOADEPT 10469
class ScholoAdept : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ScholoAdept, MoonScriptCreatureAI);
    ScholoAdept(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(15244, Target_Current, 32, 0, 4);
		AddSpell(15499, Target_Current, 21, 0, 5);
		AddSpell(15043, Target_Current, 25, 3, 0.5);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_SCHOLODARK_SUMMONER 11582
class ScholoDarkSummoner : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ScholoDarkSummoner, MoonScriptCreatureAI);
    ScholoDarkSummoner(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(12279, Target_Current, 24, 0, 15);
		AddSpell(17620, Target_Current, 29, 5, 1);
		Summon = AddSpell(17618, Target_Self, 0, 2, 0);
	}
	void OnCombatStart(Unit *pTarget)
	{
		CastTimeCD = 3;
		SummonCD = 5;
		ParentClass::OnCombatStart(pTarget);
	}
	void AIUpdate()
	{
		SummonCD--;
		if(SummonCD <= 0)
		{
			CastSpell(Summon);
			SummonCD = 10;
			summons = true;
		}
		if(summons == true)
		{
			if(CastTimeCD <= 0)
			{
				Creature* Skeleton = _unit->GetMapMgr()->GetInterface()->SpawnCreature(10482, _unit->GetPositionX()+2, _unit->GetPositionY()+1, _unit->GetPositionZ(), 3.899507f, true, false, 0, 0);
				Skeleton->Despawn(60000, 0);
				Skeleton->GetAIInterface()->GetNextTarget();
				summons = false;
			}
			CastTimeCD--;
		}

		ParentClass::AIUpdate();
	}

	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
protected:
	bool summons;
	int CastTimeCD;
	int SummonCD;
	SpellDesc* Summon;
};

#define CN_SCHOLOHANDLER 11257
class ScholoHandler : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ScholoHandler, MoonScriptCreatureAI);
    ScholoHandler(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(10833, Target_Current, 18, 0, 0);
		AddSpell(17145, Target_Current, 10, 0, 0);
		AddSpell(15244, Target_Current, 17, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_SCHOLONECROLYTE 10476
class ScholoNecrolyte : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ScholoNecrolyte, MoonScriptCreatureAI);
    ScholoNecrolyte(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		ShadowBarrier = AddSpell(17151, Target_Self, 0, 0, 0);
		AddSpell(12739, Target_Current, 15, 3, 0.5);
		AddSpell(17234, Target_Current, 19, 0, 4);
	}
	void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(ShadowBarrier);
		ParentClass::OnCombatStart(pTarget);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
SpellDesc* ShadowBarrier;
};

#define CN_SCHOLONECROMANCER 10477
class ScholoNecromancer : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ScholoNecromancer, MoonScriptCreatureAI);
    ScholoNecromancer(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(12020, Target_RandomPlayer, 24, 0, 61);
		//AddSpell(17616, Target_Self, 10, 1.5f, 0);
		AddSpell(14887, Target_RandomPlayer, 27, 3, 4);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_SCHOLONEOPHYTE 10470
class ScholoNeophyte : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ScholoNeophyte, MoonScriptCreatureAI);
    ScholoNeophyte(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(12542, Target_RandomPlayer, 25, 1.5, 5);
		AddSpell(17165, Target_Current, 37, 3, 5);
		AddSpell(12739, Target_Current, 19, 3, 0.5);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_SCHOLOOCCULTIST 10472
class ScholoOccultist : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ScholoOccultist, MoonScriptCreatureAI);
    ScholoOccultist(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		BoneArmor = AddSpell(16431, Target_Self, 0, 0, 0);
		AddSpell(15122, Target_RandomPlayer, 25, 0, 5);
		AddSpell(17228, Target_Current, 19, 0, 3);
	}
	void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(BoneArmor);
		RegisterAIUpdateEvent(1000);
		ParentClass::OnCombatStart(pTarget);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
SpellDesc* BoneArmor;
};

#define CN_SHADOWEDSPIRIT 14511
class ShadowedSpirit : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowedSpirit, MoonScriptCreatureAI);
    ShadowedSpirit(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(12739, Target_Current, 19, 3, 0.5);
		AddSpell(15245, Target_Current, 23, 3, 4);
		AddSpell(22417, Target_Self, 5, 0, 15);
		AddSpell(22575, Target_Current, 24, 0, 5);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_SPECTRALRESEARCHER 10499
class SpectralResearcher : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpectralResearcher, MoonScriptCreatureAI);
    SpectralResearcher(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(17630, Target_RandomPlayer, 14, 0, 4);
		AddSpell(17631, Target_Current, 19, 0, 5);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_SPECTRALTEACHER 10500
class SpectralTeacher : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpectralTeacher, MoonScriptCreatureAI);
    SpectralTeacher(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(17633, Target_Self, 5, 0, 15);
		AddSpell(8994, Target_RandomPlayerNotCurrent, 24, 1, 10);
		AddSpell(17630, Target_RandomPlayerNotCurrent, 19, 0, 3);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_SPECTRALTUTOR 17651
class SpectralTutor : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpectralTutor, MoonScriptCreatureAI);
    SpectralTutor(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(17651, Target_Self, 6, 0, 15);
		AddSpell(17630, Target_RandomPlayerNotCurrent, 23, 0, 3);
		AddSpell(12528, Target_RandomPlayer, 25, 1.5, 8);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_UNSTABLECORPSE 10480
class UnstableCorpse : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(UnstableCorpse, MoonScriptCreatureAI);
    UnstableCorpse(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//AddSpell(17689, Target_Current, 10, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_BONEMAGE 16120
class BoneMage : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BoneMage, MoonScriptCreatureAI);
    BoneMage(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(16102, Target_Destination, 17, 3, 0);
		AddSpell(15063, Target_Current, 17, 0, 0);
		AddSpell(15043, Target_Current, 10, 3, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_SCHOLOSTUDENT 10475
class ScholoStudent : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ScholoStudent, MoonScriptCreatureAI);
    ScholoStudent(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

void SetupScholomanceTrash(ScriptMgr * mgr)
{
	//TRASH
	mgr->register_creature_script(CN_ASPECTOFBANALITY, &AspectOfBanality::Create);
	mgr->register_creature_script(CN_ASPECTOFCORRUPTION, &AspectOfCorruption::Create);
	mgr->register_creature_script(CN_ASPECTOFMALICE, &AspectOfMalice::Create);
	mgr->register_creature_script(CN_ASPECTOFSHADOW, &AspectOfShadow::Create);
	mgr->register_creature_script(CN_BANALSPIRIT, &BanalSpirit::Create);
	mgr->register_creature_script(CN_BLOODSTEWARD, &BloodSteward::Create);
	mgr->register_creature_script(CN_BONEMAGE, &BoneMage::Create);
	mgr->register_creature_script(CN_BONEMINION, &BoneMinion::Create);
	mgr->register_creature_script(CN_CORRUPTEDSPIRIT, &CorruptedSpirit::Create);
	mgr->register_creature_script(CN_DISEASEDGHOUL, &DiseasedGhoul::Create);
	mgr->register_creature_script(CN_MALICIOUSSPIRIT, &MaliciousSpirit::Create);
	mgr->register_creature_script(CN_NECROFIEND, &NecroFiend::Create);
	mgr->register_creature_script(CN_PLAGUEDHATCHLING, &PlaguedHatchling::Create);
	mgr->register_creature_script(CN_RISENABERRATION, &RisenAberration::Create);
	mgr->register_creature_script(CN_RISENBONEWARDER, &RisenBonewarder::Create);
	mgr->register_creature_script(CN_RISENCONSTRUCT, &RisenConstruct::Create);
	mgr->register_creature_script(CN_RISENGUARD, &RisenGuard::Create);
	mgr->register_creature_script(CN_RISENGUARDIAN, &RisenGuardian::Create);
	mgr->register_creature_script(CN_RISENPROTECTOR, &RisenProtector::Create);
	mgr->register_creature_script(CN_RISENWARRIOR, &RisenWarrior::Create);
	mgr->register_creature_script(CN_SCHOLOACOLYTE, &ScholoAcolyte::Create);
	mgr->register_creature_script(CN_SCHOLOADEPT, &ScholoAdept::Create);
	mgr->register_creature_script(CN_SCHOLODARK_SUMMONER, &ScholoDarkSummoner::Create);
	mgr->register_creature_script(CN_SCHOLOHANDLER, &ScholoHandler::Create);
	mgr->register_creature_script(CN_SCHOLONECROLYTE, &ScholoNecrolyte::Create);
	mgr->register_creature_script(CN_SCHOLONECROMANCER, &ScholoNecromancer::Create);
	mgr->register_creature_script(CN_SCHOLONEOPHYTE, &ScholoNeophyte::Create);
	mgr->register_creature_script(CN_SCHOLOOCCULTIST, &ScholoOccultist::Create);
	mgr->register_creature_script(CN_SHADOWEDSPIRIT, &ShadowedSpirit::Create);
	mgr->register_creature_script(CN_SPECTRALRESEARCHER, &SpectralResearcher::Create);
	mgr->register_creature_script(CN_SPECTRALTEACHER, &SpectralTeacher::Create);
	mgr->register_creature_script(CN_SPECTRALTUTOR, &SpectralTutor::Create);
	mgr->register_creature_script(CN_UNSTABLECORPSE, &UnstableCorpse::Create);
	mgr->register_creature_script(CN_SCHOLOSTUDENT, &ScholoStudent::Create);
}