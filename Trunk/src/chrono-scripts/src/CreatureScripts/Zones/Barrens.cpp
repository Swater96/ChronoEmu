/*
 * ChronoEmu Scripts - Barrens Mobs
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by Lekritos
 */

#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"


#define CN_AMBASSADORBLOODRAGE 7895
class AmbassadorBloodrage: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AmbassadorBloodrage, MoonScriptCreatureAI);
    AmbassadorBloodrage(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(9613, Target_Current, 60, 3, 4);
		AddSpell(5137, Target_Current, 100, 2, 120);
		AddSpell(11443, Target_Current, 75, 3, 25);
	
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_AMBASSADORMALCIN 12865
class AmbassadorMalcin: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AmbassadorMalcin, MoonScriptCreatureAI);
    AmbassadorMalcin(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(9613, Target_Current, 60, 3, 4);
		AddSpell(8282, Target_Current, 100, 2, 120);
		
	
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_AZZERETTHESKYBLADE 5834
class AzzeretheSkyblade: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AzzeretheSkyblade, MoonScriptCreatureAI);
    AzzeretheSkyblade(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(13375, Target_Current, 60, 3, 4);
		AddSpell(6725, Target_Current, 75, 3, 30);
		
	
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BAELDUNEXCAVATOR 3374
class BaeldunExcavator: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BaeldunExcavator, MoonScriptCreatureAI);
    BaeldunExcavator(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(7386, Target_Current, 60, 0, 6);
		AddSpell(7164, Target_Current, 100, 0, 180);
		
	
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BAELDUNFOREMAN 3375
class BaeldunForeman: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BaeldunForeman, MoonScriptCreatureAI);
    BaeldunForeman(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6257, Target_Current, 100, 3, 45);
		
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BAELDUNOFFICER 3378
class BaeldunOfficer: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BaeldunOfficer, MoonScriptCreatureAI);
    BaeldunOfficer(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(3391, Target_Current, 50, 0, 60);
		AddSpell(6264, Target_Current, 75, 0, 45);
		
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BAELDUNRIFLEMAN 3378
class BaeldunRifleman: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BaeldunRifleman, MoonScriptCreatureAI);
    BaeldunRifleman(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6660, Target_Current, 100, 0, 1);
				
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BARAKKODOBANE 3394
class BarakKodobane: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BarakKodobane, MoonScriptCreatureAI);
    BarakKodobane(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6660, Target_Current, 100, 0, 1);
		AddSpell(6533, Target_Current, 100, 0, 30);
				
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BARRENSKODO 3236
class BarrensKodo: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BarrensKodo, MoonScriptCreatureAI);
    BarrensKodo(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6266, Target_Current, 50, 0, 30);
		AddSpell(6533, Target_Current, 100, 0, 30);

		
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BOSSCOPPERPLUG 9336
class BossCopperplug: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BossCopperplug, MoonScriptCreatureAI);
    BossCopperplug(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(9143, Target_Current, 75, 2, 30);
		
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BRISTLEBACKGEOMANCER 3263
class BristlebackGeomancer: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BristlebackGeomancer, MoonScriptCreatureAI);
    BristlebackGeomancer(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(20793, Target_Current, 60, 3, 4);
		AddSpell(20794, Target_Current, 90, 3, 30);
		AddSpell(4979, Target_Current, 75, 1.5, 120);
		
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BRISTLEBACKHUNTER 3258
class BristlebackHunter: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BristlebackHunter, MoonScriptCreatureAI);
    BristlebackHunter(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6660, Target_Current, 100, 0, 1);
		AddSpell(8806, Target_Current, 90, 2, 60);
			
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BRISTLEBACKTHORNWEAVER 3261
class Bristlebackthornweaver: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Bristlebackthornweaver, MoonScriptCreatureAI);
    Bristlebackthornweaver(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(782, Target_Self, 100, 0, 600);
		AddSpell(8806, Target_Current, 75, 1.5, 60);
			
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BRISTLEBACKWATERSEEKER 3260
class BristlebackWaterSeeker: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BristlebackWaterSeeker, MoonScriptCreatureAI);
    BristlebackWaterSeeker(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(12748, Target_Current, 75, 0, 30);
					
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BROKESPEAR 5838
class Brokespear: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Brokespear, MoonScriptCreatureAI);
    Brokespear(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(10277, Target_Current, 100, 0.5, 1);
		AddSpell(7992, Target_Current, 50, 1, 10);
					
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BRONTUS 5827
class Brontus: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Brontus, MoonScriptCreatureAI);
    Brontus(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6268, Target_Self, 75, 0, 5); 
		AddSpell(6016, Target_Current, 100, 0, 45);
					
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BURNINGBLADEACOLYTE 3380
class BurningBladeAcolyte: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BurningBladeAcolyte, MoonScriptCreatureAI);
    BurningBladeAcolyte(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(980, Target_Current, 100, 0, 30); 
		AddSpell(707, Target_Current, 100, 2, 20);
		AddSpell(689, Target_Current, 50, 0, 10);
					
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_BURNINGBLADETOXICOLOGIST 12319
class BurningBladeToxicologist: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BurningBladeToxicologist, MoonScriptCreatureAI);
    BurningBladeToxicologist(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(980, Target_Current, 100, 0, 30); 
		AddSpell(707, Target_Current, 100, 2, 20);
		AddSpell(689, Target_Current, 50, 0, 10);
					
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_CAPTAINFAIRMOUNT 3393
class CaptainFairmount: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CaptainFairmount, MoonScriptCreatureAI);
    CaptainFairmount(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(9128, Target_Current, 100, 0, 120); 
		AddSpell(19134, Target_Current, 75, 0, 60);
		AddSpell(12555, Target_Current, 75, 0, 20);
					
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_CAPTAINGEROGGHAMMERTOE 5851
class CaptainGeroggHammertoe: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CaptainGeroggHammertoe, MoonScriptCreatureAI);
    CaptainGeroggHammertoe(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(7164, Target_Current, 100, 0, 300);
		AddSpell(871, Target_Current, 75, 0, 300);
		AddSpell(1672, Target_Current, 50, 0, 12);
					
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_CORRUPTEDDREADMAWCROCOLISK 3231
class CorruptedDreadmawCrocolisk: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CorruptedDreadmawCrocolisk, MoonScriptCreatureAI);
    CorruptedDreadmawCrocolisk(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(7901, Target_Current, 100, 2, 300);
			
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_DEATHSHEADCULTIST 7872
class DeathsHeadCultist: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeathsHeadCultist, MoonScriptCreatureAI);
    DeathsHeadCultist(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(11433, Target_Current, 100, 2, 30);
		AddSpell(9613, Target_Current, 60, 3, 4);
			
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_DEVIATECOILER 3630
class DeviateCoiler: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeviateCoiler, MoonScriptCreatureAI);
    DeviateCoiler(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(25008, Target_Current, 80, 0, 10);
					
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_DEVIATECREEPER 3632
class DeviateCreeper: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeviateCreeper, MoonScriptCreatureAI);
    DeviateCreeper(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(3427, Target_Current, 100, 0, 60);
					
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_DEVIATESTALKER 3634
class DeviateStalker: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeviateStalker, MoonScriptCreatureAI);
    DeviateStalker(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(16829, Target_Current, 80, 0, 4);
		AddSpell(53578, Target_Current, 75, 0, 60);
					
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_DISHU 5865
class Dishu: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Dishu, MoonScriptCreatureAI);
    Dishu(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(8210, Target_Current, 100, 0, 300);
		
					
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_DREADMAWCROCOLISK 3110
class DreadmawCrocolisk: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DreadmawCrocolisk, MoonScriptCreatureAI);
    DreadmawCrocolisk(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(17255, Target_Current, 75, 0, 10);
		AddSpell(12166, Target_Current, 75, 0, 10);
		
			
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_ECHEYAKEE 3475
class Echeyakee: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Echeyakee, MoonScriptCreatureAI);
    Echeyakee(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(16829, Target_Current, 80, 0, 4);
		AddSpell(59882, Target_Current, 75, 0, 10);
		
			
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_ELDERMYSTICRAZORSNOUT 3270
class ElderMysticRazorsnout: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ElderMysticRazorsnout, MoonScriptCreatureAI);
    ElderMysticRazorsnout(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(2484, Target_Current, 100, 0, 45);
		AddSpell(3599, Target_Current, 100, 0, 30);
		AddSpell(547, Target_Self, 100, 2.5, 10);
		AddSpell(5605, Target_Current, 100, 2, 30);
		
			
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_ENGINEERWHIRLEYGIG 5836
class EngineerWhirleygig: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EngineerWhirleygig, MoonScriptCreatureAI);
    EngineerWhirleygig(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(7979, Target_Self, 100, 2, 120);
		AddSpell(8209, Target_Self, 100, 2, 60);
			
			
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_FOREMANGRILLS 5835
class ForemanGrills: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ForemanGrills, MoonScriptCreatureAI);
    ForemanGrills(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(9080, Target_Current, 75, 0, 15);
					
			
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_GEOPRIESTGUKKROK 5863
class GeopriestGukkrok: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GeopriestGukkrok, MoonScriptCreatureAI);
    GeopriestGukkrok(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(2054, Target_Self, 100, 3, 25);
		AddSpell(600, Target_Self, 75, 0, 20);
		AddSpell(6075, Target_Self, 75, 0, 20);

	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_GEOPRIESTGUKKROK 5863
class GeopriestGukkrok: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GeopriestGukkrok, MoonScriptCreatureAI);
    GeopriestGukkrok(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(2054, Target_Self, 100, 3, 25);
		AddSpell(600, Target_Self, 75, 0, 20);
		AddSpell(6075, Target_Self, 75, 0, 20);

	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define CN_GESHARAHAN 3398
class Gesharahan: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Gesharahan, MoonScriptCreatureAI);
    Gesharahan(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(3583, Target_Current, 100, 0, 20);
				
		
		}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

void SetupBarrens(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_AMBASSADORBLOODRAGE, &AmbassadorBloodrage::Create);
	mgr->register_creature_script(CN_AMBASSADORMALCIN, &AmbassadorMalcin::Create);
	mgr->register_creature_script(CN_AZZERETTHESKYBLADE, &AzzeretheSkyblade::Create);
	mgr->register_creature_script(CN_BAELDUNEXCAVATOR, &BaeldunExcavator::Create);
	mgr->register_creature_script(CN_BAELDUNFOREMAN, &BaeldunForeman::Create);
	mgr->register_creature_script(CN_BAELDUNOFFICER, &BaeldunOfficer::Create);
	mgr->register_creature_script(CN_BAELDUNRIFLEMAN, &BaeldunRifleman::Create);
	mgr->register_creature_script(CN_BARAKKODOBANE, &BarakKodobane::Create);
	mgr->register_creature_script(CN_BARRENSKODO, &BarrensKodo::Create);
	mgr->register_creature_script(CN_BOSSCOPPERPLUG, &BossCopperplug::Create);
	mgr->register_creature_script(CN_BRISTLEBACKGEOMANCER, &BristlebackGeomancer::Create);
	mgr->register_creature_script(CN_BRISTLEBACKHUNTER, &BristlebackHunter::Create);
	mgr->register_creature_script(CN_BRISTLEBACKTHORNWEAVER, &Bristlebackthornweaver::Create);
	mgr->register_creature_script(CN_BRISTLEBACKWATERSEEKER, &BristlebackWaterSeeker::Create);
	mgr->register_creature_script(CN_BROKESPEAR, &Brokespear::Create);
	mgr->register_creature_script(CN_BRONTUS, &Brontus::Create);
	mgr->register_creature_script(CN_BURNINGBLADEACOLYTE, &BurningBladeAcolyte::Create);
	mgr->register_creature_script(CN_BURNINGBLADETOXICOLOGIST, &BurningBladeToxicologist::Create);
	mgr->register_creature_script(CN_CAPTAINFAIRMOUNT, &CaptainFairmount::Create);
	mgr->register_creature_script(CN_CAPTAINGEROGGHAMMERTOE, &CaptainGeroggHammertoe::Create);
	mgr->register_creature_script(CN_CORRUPTEDDREADMAWCROCOLISK, &CorruptedDreadmawCrocolisk::Create);
	mgr->register_creature_script(CN_DEATHSHEADCULTIST, &DeathsHeadCultist::Create);
	mgr->register_creature_script(CN_DEVIATECOILER, &DeviateCoiler::Create);
	mgr->register_creature_script(CN_DEVIATECREEPER, &DeviateCreeper::Create);
	mgr->register_creature_script(CN_DEVIATESTALKER, &DeviateStalker::Create);
	mgr->register_creature_script(CN_DISHU, &Dishu::Create);
	mgr->register_creature_script(CN_DREADMAWCROCOLISK, &DreadmawCrocolisk::Create);
	mgr->register_creature_script(CN_ECHEYAKEE, &Echeyakee::Create);
	mgr->register_creature_script(CN_ELDERMYSTICRAZORSNOUT, &ElderMysticRazorsnout::Create);
	mgr->register_creature_script(CN_ENGINEERWHIRLEYGIG, &EngineerWhirleygig::Create);
	mgr->register_creature_script(CN_FOREMANGRILLS, &ForemanGrills::Create);
	mgr->register_creature_script(CN_GEOPRIESTGUKKROK, &GeopriestGukkrok::Create);
	mgr->register_creature_script(CN_GEOPRIESTGUKKROK, &GeopriestGukkrok::Create);

};
