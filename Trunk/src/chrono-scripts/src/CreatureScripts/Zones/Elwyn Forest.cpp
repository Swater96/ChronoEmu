#include "StdAfx.h"
#include "../Setup.h"
#include "../../Common/Base.h"

#define CN_CROWN_LACKEY 37214
class CrownLackeyAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CrownLackeyAI, MoonScriptCreatureAI);
	CrownLackeyAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(70074, Target_Self, 30,0,0);
	}
};

#define CN_CYLINA_DARKHEART 6374
class CylinaDarkheartAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CylinaDarkheartAI, MoonScriptCreatureAI);
	CylinaDarkheartAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(11939, Target_Self, 10,0,30);
	}
};

#define CN_DANE_WINSLOW 6373
class DaneWinslowAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DaneWinslowAI, MoonScriptCreatureAI);
	DaneWinslowAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(11939, Target_Self, 10,0,30);
	}
};

#define CN_DEFIAS_BANDIT 116
class DefiasBanditAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DefiasBanditAI, MoonScriptCreatureAI);
	DefiasBanditAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(8646, Target_Current, 30,0,10);
	}
};

#define CN_DEFIAS_BODYGUARDS 6866
class DefiasBodyguardsAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DefiasBodyguardsAI, MoonScriptCreatureAI);
	DefiasBodyguardsAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(6713, Target_Current, 30,0,10);
		AddSpell(7159, Target_Current, 10,0,20);
	}
};

#define CN_DEFIAS_CUTPURSE 94
class DefiasCutpurseAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DefiasCutpurseAI, MoonScriptCreatureAI);
	DefiasCutpurseAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(53, Target_Self, 10,0,20);
	}
};

#define CN_DEFIAS_DOCKWORKER 6927
class DefiasDockworkerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DefiasDockworkerAI, MoonScriptCreatureAI);
	DefiasDockworkerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(8646, Target_Self, 30,0,10);
	}
};

#define CN_DEFIAS_ROGUE_WIZARD 474
class DefiasRogueWizardAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DefiasRogueWizardAI, MoonScriptCreatureAI);
	DefiasRogueWizardAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(13322, Target_Current, 30,3,1);
		ApplyAura(12544);
	}
};

#define CN_DISEASED_YOUNG_WOLF 299
class DiseasedYoungWolfAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DiseasedYoungWolfAI, MoonScriptCreatureAI);
	DiseasedYoungWolfAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		ApplyAura(24604);
	}
};

#define CN_FEDFENNEL 472
class FedfennelAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FedfennelAI, MoonScriptCreatureAI);
	FedfennelAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(13730, Target_Self, 25,0,30);
		ApplyAura(3238);
	}
};

#define CN_FOREST_SPIDER 30
class ForestSpiderAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ForestSpiderAI, MoonScriptCreatureAI);
	ForestSpiderAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(4167, Target_Current, 15,0,10);
		AddSpell(17255, Target_Current, 30,0,8);
		AddSpell(14916, Target_Current, 10,0,5);
	}
};

#define CN_FORLORN_SPIRIT 2044
class ForlornSpiritAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ForlornSpiritAI, MoonScriptCreatureAI);
	ForlornSpiritAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(118, Target_RandomPlayerNotCurrent, 15,1.5,10);
		AddSpell(3105,Target_Current, 15,0,45);
	}
};

#define CN_GARRICK_PADFOOT 103
class GarrickPadfootAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GarrickPadfootAI, MoonScriptCreatureAI);
	GarrickPadfootAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		ApplyAura(7164);
	}
};

#define CN_HOGGER 448
class HoggerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HoggerAI, MoonScriptCreatureAI);
	HoggerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(6016, Target_Current, 30,0,45);
		AddSpell(6730, Target_Current, 20,0,10);
		ApplyAura(6268);
	}
};

#define CN_CHICKEN 620
class ChickenAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ChickenAI, MoonScriptCreatureAI);
	ChickenAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		//AddSpell(13563, Target_Self, 10,0,0);
	}
};

#define CN_KOBOLD_GEOMANCER 476
class KoboldGeomancerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KoboldGeomancerAI, MoonScriptCreatureAI);
	KoboldGeomancerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(20793, Target_Current, 30,3,1);
		ApplyAura(12544);
	}
};

#define CN_KOBOLD_MINER 40
class KoboldMinerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KoboldMinerAI, MoonScriptCreatureAI);
	KoboldMinerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(6016, Target_Current, 20,0,45);
	}
};

#define CN_LONGSNOUT 119
class LongsnoutAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LongsnoutAI, MoonScriptCreatureAI);
	LongsnoutAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(17255, Target_Current, 25,0,1);
		AddSpell(14916, Target_Current, 10,0,5);
		ApplyAura(6268);
	}
};

#define CN_MANGY_WOLF 525
class MangyWolfAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MangyWolfAI, MoonScriptCreatureAI);
	MangyWolfAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(17255, Target_Current, 30,0,10);
		ApplyAura(24604);
		AddSpell(14916, Target_Current, 15,0,5);
	}
};

#define CN_MORGAIN_THE_SLY 99
class MorgainTheSlyAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MorgainTheSlyAI, MoonScriptCreatureAI);
	MorgainTheSlyAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(53, Target_Current, 20,0,1);
		AddSpell(1776, Target_Current, 30,0,10);
	}
};

#define CN_MORGAN_THE_COLLECTOR 473
class MorganTheCollectorAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MorganTheCollectorAI, MoonScriptCreatureAI);
	MorganTheCollectorAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(53, Target_Current, 20,0,1);
		AddSpell(1776, Target_Current, 30,0,10);
	}
};

#define CN_MOTHER_FANG 471
class MotherFangAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MotherFangAI, MoonScriptCreatureAI);
	MotherFangAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(12023, Target_Current, 20,0,8);
	}
};

#define CN_MURLOC_FORAGER 46
class MurlocForagerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MurlocForagerAI, MoonScriptCreatureAI);
	MurlocForagerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(3368, Target_Self, 20,3,1);
	}
};

#define CN_MURLOC_LURKER 732
class MurlocLurkerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MurlocLurkerAI, MoonScriptCreatureAI);
	MurlocLurkerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(7159, Target_Current, 20,0,1);
	}
};

#define CN_NARG_THE_TASKMASTER 79
class NargTheTaskmasterAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(NargTheTaskmasterAI, MoonScriptCreatureAI);
	NargTheTaskmasterAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		ApplyAura(9128);
	}
};

#define CN_PORCINE_ENTOURAGE 390
class PorcineEntourageAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PorcineEntourageAI, MoonScriptCreatureAI);
	PorcineEntourageAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		ApplyAura(6268);
	}
};

#define CN_PRINCESS 330
class PrincessAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PrincessAI, MoonScriptCreatureAI);
	PrincessAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		ApplyAura(6268);
	}
};

#define CN_PROWLER 118
class ProwlerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ProwlerAI, MoonScriptCreatureAI);
	ProwlerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(17255, Target_Current, 25,0,1);
		ApplyAura(24604);
		AddSpell(14916,Target_Current, 10,0,1);
	}
};

#define CN_RIVERPAW_OUTRUNNER 478
class RiverpawOutrunnerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RiverpawOutrunnerAI, MoonScriptCreatureAI);
	RiverpawOutrunnerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(3391, Target_Current, 20,0,1);
	}
};

#define CN_ROCKHIDE_BOAR 524
class RockhideBoarAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RockhideBoarAI, MoonScriptCreatureAI);
	RockhideBoarAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		ApplyAura(6268);
	}
};

#define CN_RUKLAR_THE_TRAPPER 60
class RuklarTheTrapperAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RuklarTheTrapperAI, MoonScriptCreatureAI);
	RuklarTheTrapperAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(6016, Target_Current, 30,0,45);
		AddSpell(12024, Target_Current, 20,0,5);
	}
};

#define CN_STONETUSK_BOAR 113
class StonetuskBoarAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(StonetuskBoarAI, MoonScriptCreatureAI);
	StonetuskBoarAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		ApplyAura(6268);
	}
};

#define CN_SURENA_CALEDON 881
class SurenaCaledonAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SurenaCaledonAI, MoonScriptCreatureAI);
	SurenaCaledonAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(20793, Target_Current, 30,3,1);
		ApplyAura(12544);
	}
};

#define CN_THUROS_LIGHTFINGERS 61
class ThurosLightfingersAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ThurosLightfingersAI, MoonScriptCreatureAI);
	ThurosLightfingersAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(7159, Target_Current, 20,0,1);
	}
};

#define CN_YOUNG_FOREST_BEAR 822
class YoungForestBearAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(YoungForestBearAI, MoonScriptCreatureAI);
	YoungForestBearAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(16828, Target_Current, 30,0,1);
		AddSpell(50256, Target_Current, 20,0,5);
		AddSpell(14916, Target_Current, 10,0,10);
	}
};

void SetupElwynForestNPC(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_CROWN_LACKEY, &CrownLackeyAI::Create);
	mgr->register_creature_script(CN_CYLINA_DARKHEART, &CylinaDarkheartAI::Create);
	mgr->register_creature_script(CN_DANE_WINSLOW, &DaneWinslowAI::Create);
	mgr->register_creature_script(CN_DEFIAS_BANDIT, &DefiasBanditAI::Create);
	mgr->register_creature_script(CN_DEFIAS_BODYGUARDS, &DefiasBodyguardsAI::Create);
	mgr->register_creature_script(CN_DEFIAS_CUTPURSE, &DefiasCutpurseAI::Create);
	mgr->register_creature_script(CN_DEFIAS_DOCKWORKER, &DefiasDockworkerAI::Create);
	mgr->register_creature_script(CN_DEFIAS_ROGUE_WIZARD, &DefiasRogueWizardAI::Create);
	mgr->register_creature_script(CN_DISEASED_YOUNG_WOLF, &DiseasedYoungWolfAI::Create);
	mgr->register_creature_script(CN_FEDFENNEL, &FedfennelAI::Create);
	mgr->register_creature_script(CN_FOREST_SPIDER, &ForestSpiderAI::Create);
	mgr->register_creature_script(CN_FORLORN_SPIRIT, &ForlornSpiritAI::Create);
	mgr->register_creature_script(CN_GARRICK_PADFOOT, &GarrickPadfootAI::Create);
	mgr->register_creature_script(CN_HOGGER, &HoggerAI::Create);
	mgr->register_creature_script(CN_CHICKEN, &ChickenAI::Create);
	mgr->register_creature_script(CN_KOBOLD_GEOMANCER, &KoboldGeomancerAI::Create);
	mgr->register_creature_script(CN_KOBOLD_MINER, &KoboldMinerAI::Create);
	mgr->register_creature_script(CN_LONGSNOUT, &LongsnoutAI::Create);
	mgr->register_creature_script(CN_MANGY_WOLF, &MangyWolfAI::Create);
	mgr->register_creature_script(CN_MORGAIN_THE_SLY, &MorgainTheSlyAI::Create);
	mgr->register_creature_script(CN_MORGAN_THE_COLLECTOR, &MorganTheCollectorAI::Create);
	mgr->register_creature_script(CN_MOTHER_FANG, &MotherFangAI::Create);
	mgr->register_creature_script(CN_MURLOC_FORAGER, &MurlocForagerAI::Create);
	mgr->register_creature_script(CN_MURLOC_LURKER, &MurlocLurkerAI::Create);
	mgr->register_creature_script(CN_NARG_THE_TASKMASTER, &NargTheTaskmasterAI::Create);
	mgr->register_creature_script(CN_PORCINE_ENTOURAGE, &PorcineEntourageAI::Create);
	mgr->register_creature_script(CN_PRINCESS, &PrincessAI::Create);
	mgr->register_creature_script(CN_PROWLER, &ProwlerAI::Create);
	mgr->register_creature_script(CN_RIVERPAW_OUTRUNNER, &RiverpawOutrunnerAI::Create);
	mgr->register_creature_script(CN_ROCKHIDE_BOAR, &RockhideBoarAI::Create);
	mgr->register_creature_script(CN_RUKLAR_THE_TRAPPER, &RuklarTheTrapperAI::Create);
	mgr->register_creature_script(CN_STONETUSK_BOAR, &StonetuskBoarAI::Create);
	mgr->register_creature_script(CN_SURENA_CALEDON, &SurenaCaledonAI::Create);
	mgr->register_creature_script(CN_THUROS_LIGHTFINGERS, &ThurosLightfingersAI::Create);
	mgr->register_creature_script(CN_YOUNG_FOREST_BEAR, &YoungForestBearAI::Create);
}