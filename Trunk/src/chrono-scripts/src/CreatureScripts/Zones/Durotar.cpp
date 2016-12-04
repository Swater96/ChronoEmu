/*
 * ChronoEmu Scripts - Durotar Mobs
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by Lekritos
 */

#include "StdAfx.h"
#include "../Setup.h"
#include "../../Common/Base.h"


#define CN_ARMOREDSCORPID 3058
class ArmoredScorpid: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ArmoredScorpid, MoonScriptCreatureAI);
    ArmoredScorpid(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(16828, Target_Current, 80, 0, 4);
		AddSpell(24640, Target_Current, 50, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_BLOODTALONSCTYTHEMAW 3058
class BloodtalonScythemaw: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BloodtalonScythemaw, MoonScriptCreatureAI);
    BloodtalonScythemaw(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(16828, Target_Current, 80, 0, 4);
		AddSpell(6268, Target_Self, 75, 0, 5);
		AddSpell(50498, Target_Current, 90, 0, 60);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_BLOODTALONTAILLASHER 3122
class BloodtalonTaillasher: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BloodtalonTaillasher, MoonScriptCreatureAI);
    BloodtalonTaillasher(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(16828, Target_Current, 80, 0, 4);
		AddSpell(6268, Target_Self, 75, 0, 5);
		AddSpell(50498, Target_Current, 90, 0, 60);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_BURNINGBLADEAPPRENTICE 3198
class BurningBladeApprentice: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BurningBladeApprentice, MoonScriptCreatureAI);
    BurningBladeApprentice(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(9613, Target_Current, 60, 3, 4);
		AddSpell(12746, Target_Self, 100, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_BURNINGBLADECULTIST 3199
class BurningBladeCultist: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BurningBladeCultist, MoonScriptCreatureAI);
    BurningBladeCultist(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(11962, Target_Current, 75, 2, 15);
		AddSpell(11939, Target_Self, 100, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_BURNINGBLADEFANATIC 3197
class BurningBladeFanatic: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BurningBladeFanatic, MoonScriptCreatureAI);
    BurningBladeFanatic(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(5262, Target_Current, 75, 1, 15);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_BURNINGBLADENEOPHYTE 3196
class BurningBladeNeophyte: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BurningBladeNeophyte, MoonScriptCreatureAI);
    BurningBladeNeophyte(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(11962, Target_Current, 75, 2, 15);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_CAPTAINFLATTUSK 5824
class CaptainFlatTusk: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CaptainFlatTusk, MoonScriptCreatureAI);
    CaptainFlatTusk(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(7165, Target_Self, 100, 0, 180);
		AddSpell(25710, Target_Current, 75, 0, 8);				
	
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_CLATTERINGSCORPID 3125
class ClatteringScorpid: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ClatteringScorpid, MoonScriptCreatureAI);
    ClatteringScorpid(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(24640, Target_Current, 50, 0, 10);			
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_CORRUPTEDBLOODTALONSCYTHEMAW 3227
class CorruptedBloodtalonScythemaw: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CorruptedBloodtalonScythemaw, MoonScriptCreatureAI);
    CorruptedBloodtalonScythemaw(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(16828, Target_Current, 80, 0, 4);
		AddSpell(6268, Target_Self, 75, 0, 5);
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

#define CN_CORRUPTEDMOTTLEDBOAR 3225
class CorruptedMottledBoar: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CorruptedMottledBoar, MoonScriptCreatureAI);
    CorruptedMottledBoar(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6818, Target_Self, 75, 0, 30);
		AddSpell(3385, Target_Self, 50, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_CORRUPTEDSCORPID 3226
class CorruptedScorpid: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CorruptedScorpid, MoonScriptCreatureAI);
    CorruptedScorpid(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(5413, Target_Current, 25, 0, 30);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_CORRUPTEDSURFCRAWLER 3228
class CorruptedSurfCrawler: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CorruptedSurfCrawler, MoonScriptCreatureAI);
    CorruptedSurfCrawler(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6951, Target_Current, 100, 2, 300);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_DEATHFLAYER 5823
class DeathFlayer: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeathFlayer, MoonScriptCreatureAI);
    DeathFlayer(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(5416, Target_Current, 75, 0, 30);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_DIREMOTTLEDBOAR 5823
class DireMottledBoar: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DireMottledBoar, MoonScriptCreatureAI);
    DireMottledBoar(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(17255, Target_Current, 75, 0, 10);
		AddSpell(3385, Target_Self, 50, 0, 10);
		AddSpell(35290, Target_Current, 50, 0, 10); 
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

 #define CN_DUROTARTIGER 3121
class DurotarTiger: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DurotarTiger, MoonScriptCreatureAI);
    DurotarTiger(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(16828, Target_Current, 80, 0, 4);
		AddSpell(59881, Target_Current, 75, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_DUSTWINDPILLAGER 3116
class DustwindPillager: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DustwindPillager, MoonScriptCreatureAI);
    DustwindPillager(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(3147, Target_Current, 80, 0, 15);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_DUSTWINDSTORMWITCH 3118
class DustwindStormWitch: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DustwindStormWitch, MoonScriptCreatureAI);
    DustwindStormWitch(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(9532, Target_Current, 40, 3, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_ELDERMOTTLEDBOAR 3100
class ElderMottledBoar: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ElderMottledBoar, MoonScriptCreatureAI);
    ElderMottledBoar(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(17255, Target_Current, 75, 0, 10);
		AddSpell(3385, Target_Self, 50, 0, 10);		
		AddSpell(35290, Target_Current, 50, 0, 10); 
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_ENCRUSTEDSURFCRAWLER 3108
class EncrustedSurfCrawler: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EncrustedSurfCrawler, MoonScriptCreatureAI);
    EncrustedSurfCrawler(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(5426, Target_Current, 50, 0, 30);
					
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_FELWEAVERSCORNN 3108
class FelweaverScornn: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FelweaverScornn, MoonScriptCreatureAI);
    FelweaverScornn(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(9613, Target_Current, 60, 3, 4);
		AddSpell(12746, Target_Self, 100, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_FIZZLEDARKSTORM 3203
class FizzleDarkstorm: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FizzleDarkstorm, MoonScriptCreatureAI);
    FizzleDarkstorm(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(9613, Target_Current, 60, 3, 4);
		AddSpell(11939, Target_Self, 100, 0, 0);
		AddSpell(7290, Target_Current, 10, 0, 1);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_GAZZUZ 3204
class Gazzuz: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Gazzuz, MoonScriptCreatureAI);
    Gazzuz(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(9613, Target_Current, 60, 3, 4);
		AddSpell(20798, Target_Self, 100, 0, 3000);
		AddSpell(172, Target_Current, 80, 0, 15);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_GEOLORDMOTTLE 5826
class GeolordMottle: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GeolordMottle, MoonScriptCreatureAI);
    GeolordMottle(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(547, Target_Self, 100, 2.5, 10);
		AddSpell(324, Target_Self, 100, 0, 300);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_HEXEDTROLL 3207
class HexedTroll: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HexedTroll, MoonScriptCreatureAI);
    HexedTroll(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(348, Target_Current, 75, 2, 15);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_KOLKARDRUDGE 3119
class KolkarDrudge: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KolkarDrudge, MoonScriptCreatureAI);
    KolkarDrudge(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(7272, Target_Current, 25, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_KOLKAROUTRUNNER 3120
class KolkarOutrunner: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KolkarOutrunner, MoonScriptCreatureAI);
    KolkarOutrunner(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
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

#define CN_KULTIRASMARINE 3129
class KulTirasMarine: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KulTirasMarine, MoonScriptCreatureAI);
    KulTirasMarine(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(11972, Target_Current, 50, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_KULTIRASSAILOR 3128
class KulTirasSailor: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KulTirasSailor, MoonScriptCreatureAI);
    KulTirasSailor(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6268, Target_Self, 75, 0, 5);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_LIEUTENANTBENEDICT 3192
class LieutenantBenedict: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LieutenantBenedict, MoonScriptCreatureAI);
    LieutenantBenedict(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(72, Target_Current, 75, 0, 12);
		AddSpell(7164, Target_Current, 100, 0, 300);		
		AddSpell(3248, Target_Current, 75, 0, 15);			
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_LIGHTNINGHIDE 3131
class LightningHide: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LightningHide, MoonScriptCreatureAI);
    LightningHide(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(5401, Target_Current, 90, 2, 1);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_RAZORMANEBATTLEGAURD 3114
class RazormaneBattleguard: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RazormaneBattleguard, MoonScriptCreatureAI);
    RazormaneBattleguard(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(3248, Target_Current, 75, 0, 15);	
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_RAZORMANEDUSTRUNNER 3113
class RazormaneDustrunner: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RazormaneDustrunner, MoonScriptCreatureAI);
    RazormaneDustrunner(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6950, Target_Current, 100, 2, 60);
		AddSpell(774, Target_Self, 100, 0, 20);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_RAZORMANEQUILBOAR 3111
class RazormaneQuilboar: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RazormaneQuilboar, MoonScriptCreatureAI);
    RazormaneQuilboar(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(5280, Target_Current, 100, 1, 120);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_RAZORMANESCOUT 3112
class RazormaneScout: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RazormaneScout, MoonScriptCreatureAI);
    RazormaneScout(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
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

#define CN_SANDSHARK 5435
class SandShark: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SandShark, MoonScriptCreatureAI);
    SandShark(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(3391, Target_Current, 50, 0, 60);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_THUNDERLIZARD 3130
class ThunderLizard: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ThunderLizard, MoonScriptCreatureAI);
    ThunderLizard(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(5401, Target_Current, 90, 2, 1);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_VENOMTAILSCORPID 3127
class VenomtailScorpid: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VenomtailScorpid, MoonScriptCreatureAI);
    VenomtailScorpid(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(16828, Target_Current, 80, 0, 4);
		AddSpell(5416, Target_Current, 75, 0, 30);
		AddSpell(24640, Target_Current, 50, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_VILEFAMILIAR 3101
class VileFamiliar: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VileFamiliar, MoonScriptCreatureAI);
    VileFamiliar(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(11921, Target_Current, 80, 3, 4);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_VOODOTROLL 3206
class VoodooTroll: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VoodooTroll, MoonScriptCreatureAI);
    VoodooTroll(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(324, Target_Self, 100, 0, 300);
		AddSpell(331, Target_Self, 100, 1.5, 20);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_WARLORDKOLKANIS 5808
class WarlordKolkanis: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WarlordKolkanis, MoonScriptCreatureAI);
    WarlordKolkanis(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(12555, Target_Current, 75, 0, 20);
		AddSpell(8078, Target_Current, 50, 0, 20);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_WATCHCOMMANDERZALAPHIL 5809
class WatchCommanderZalaphil: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WatchCommanderZalaphil, MoonScriptCreatureAI);
    WatchCommanderZalaphil(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(7164, Target_Current, 100, 0, 300);
		AddSpell(11972, Target_Current, 50, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_YARROGBANESHADOW 3183
class YarrogBaneshadow: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(YarrogBaneshadow, MoonScriptCreatureAI);
    YarrogBaneshadow(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(172, Target_Current, 80, 0, 15);
		AddSpell(348, Target_Current, 75, 2, 15);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_ZALAZANE 3183
class Zalazane: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Zalazane, MoonScriptCreatureAI);
    Zalazane(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(332, Target_Self, 100, 2, 20);
		AddSpell(7289, Target_Current, 75, 3, 120);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

void SetupDurotarNPC(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_ARMOREDSCORPID, &ArmoredScorpid::Create);
	mgr->register_creature_script(CN_BLOODTALONSCTYTHEMAW, &BloodtalonScythemaw::Create);
	mgr->register_creature_script(CN_BLOODTALONTAILLASHER, &BloodtalonTaillasher::Create);
	mgr->register_creature_script(CN_BURNINGBLADEAPPRENTICE, &BurningBladeApprentice::Create);
	mgr->register_creature_script(CN_BURNINGBLADECULTIST, &BurningBladeCultist::Create);
	mgr->register_creature_script(CN_BURNINGBLADEFANATIC, &BurningBladeFanatic::Create);
	mgr->register_creature_script(CN_BURNINGBLADEFANATIC, &BurningBladeFanatic::Create);
	mgr->register_creature_script(CN_BURNINGBLADENEOPHYTE, &BurningBladeNeophyte::Create);
	mgr->register_creature_script(CN_CAPTAINFLATTUSK, &CaptainFlatTusk::Create);
	mgr->register_creature_script(CN_CLATTERINGSCORPID, &ClatteringScorpid::Create);
	mgr->register_creature_script(CN_CORRUPTEDBLOODTALONSCYTHEMAW, &CorruptedBloodtalonScythemaw::Create);
	mgr->register_creature_script(CN_CORRUPTEDDREADMAWCROCOLISK, &CorruptedDreadmawCrocolisk::Create);
	mgr->register_creature_script(CN_CORRUPTEDMOTTLEDBOAR, &CorruptedMottledBoar::Create);
	mgr->register_creature_script(CN_CORRUPTEDSCORPID, &CorruptedScorpid::Create);
	mgr->register_creature_script(CN_CORRUPTEDSURFCRAWLER, &CorruptedSurfCrawler::Create);
	mgr->register_creature_script(CN_DEATHFLAYER, &DeathFlayer::Create);
	mgr->register_creature_script(CN_DIREMOTTLEDBOAR, &DireMottledBoar::Create);
	mgr->register_creature_script(CN_DREADMAWCROCOLISK, &DreadmawCrocolisk::Create);
	mgr->register_creature_script(CN_DUROTARTIGER, &DurotarTiger::Create);
	mgr->register_creature_script(CN_DUSTWINDPILLAGER, &DustwindPillager::Create);
	mgr->register_creature_script(CN_DUSTWINDSTORMWITCH, &DustwindStormWitch::Create);
	mgr->register_creature_script(CN_ELDERMOTTLEDBOAR, &ElderMottledBoar::Create);
	mgr->register_creature_script(CN_ENCRUSTEDSURFCRAWLER, &EncrustedSurfCrawler::Create);
	mgr->register_creature_script(CN_FELWEAVERSCORNN, &FelweaverScornn::Create);
	mgr->register_creature_script(CN_FIZZLEDARKSTORM, &FizzleDarkstorm::Create);
	mgr->register_creature_script(CN_GAZZUZ, &Gazzuz::Create);
	mgr->register_creature_script(CN_GEOLORDMOTTLE, &GeolordMottle::Create);
	mgr->register_creature_script(CN_HEXEDTROLL, &HexedTroll::Create);
	mgr->register_creature_script(CN_KOLKARDRUDGE, &KolkarDrudge::Create);
	mgr->register_creature_script(CN_KOLKAROUTRUNNER, &KolkarOutrunner::Create);
	mgr->register_creature_script(CN_KULTIRASMARINE, &KulTirasMarine::Create);
	mgr->register_creature_script(CN_KULTIRASSAILOR, &KulTirasSailor::Create);
	mgr->register_creature_script(CN_LIEUTENANTBENEDICT, &LieutenantBenedict::Create);
	mgr->register_creature_script(CN_LIGHTNINGHIDE, &LightningHide::Create);
	mgr->register_creature_script(CN_RAZORMANEBATTLEGAURD, &RazormaneBattleguard::Create);
	mgr->register_creature_script(CN_RAZORMANEDUSTRUNNER, &RazormaneDustrunner::Create);
	mgr->register_creature_script(CN_RAZORMANEQUILBOAR, &RazormaneQuilboar::Create);
	mgr->register_creature_script(CN_RAZORMANESCOUT, &RazormaneScout::Create);
	mgr->register_creature_script(CN_SANDSHARK, &SandShark::Create);
	mgr->register_creature_script(CN_THUNDERLIZARD, &ThunderLizard::Create);
	mgr->register_creature_script(CN_VENOMTAILSCORPID, &VenomtailScorpid::Create);
	mgr->register_creature_script(CN_VILEFAMILIAR, &VileFamiliar::Create);
	mgr->register_creature_script(CN_VOODOTROLL, &VoodooTroll::Create);
	mgr->register_creature_script(CN_WARLORDKOLKANIS, &WarlordKolkanis::Create);
	mgr->register_creature_script(CN_WATCHCOMMANDERZALAPHIL, &WatchCommanderZalaphil::Create);
	mgr->register_creature_script(CN_YARROGBANESHADOW, &YarrogBaneshadow::Create);
	mgr->register_creature_script(CN_ZALAZANE, &Zalazane::Create);
}