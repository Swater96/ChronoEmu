/*
 * ChronoEmu Scripts - Maraudon
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by Sphere
 */
 

#include "StdAfx.h" 
#include "Setup.h"
#include "../Common/Base.h"
#include "Instance_Maraudon.h"

/* TODO:		Noxxion - now banishes, should dissapear
				Razorlash - should have more abilities
							- puncture? (sid 15976)
				Rotgrip - same stacking bleed as razorlash
				Princess - dust field spell might not work exaclty as intended
				Celebras the Redeemed - animation
 */

#define PORTAL_ACTIVATE			178386
#define PORTAL_MARA				21127
#define SCEPTER_OF_CEREBRAS		17191

class MaraudonPortalAI : public GameObjectAIScript
{
public:
	MaraudonPortalAI(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new MaraudonPortalAI(GO); }

	void OnActivate(Player* pPlayer)	
	{
		if(pPlayer->GetItemInterface()->GetItemCount(SCEPTER_OF_CEREBRAS)) {
			pPlayer->CastSpell(pPlayer, PORTAL_MARA, false);
		}
		else
		{
			pPlayer->BroadcastMessage("You need Scepter of Cerebras to use this portal!");
		}
	}
};

class NoxxionAI:public MoonScriptBossAI
{
	SpellDesc *summon;
	int phase;
	int32 timer;

    MOONSCRIPT_FACTORY_FUNCTION(NoxxionAI, MoonScriptBossAI);
	NoxxionAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
	{
		phase = 0;
		int32 timer = -1;
		summon = AddSpell(MA_SUMMON_SPAWNS, Target_Self, 0, 0, 0);
		AddSpell(MA_TOXIC_VOLLEY, Target_Self, 15, 0, 15);
		AddSpell(MA_UPPERCUT, Target_Current, 20, 0, 10);
	}

	void AIUpdate() {
		if((timer != -1) && IsTimerFinished(timer)) {
			timer = -1;
			RemoveAllAuras();
			_unit->Unroot();
		}
		if((phase == 0) && (GetHealthPercent() < 66)) {
			phase = 1;
			CastSpellNowNoScheduling(summon);
			ApplyAura(MA_BANISH);
			_unit->Root();
			timer = AddTimer(10000);
		}
		if((phase == 1) && (GetHealthPercent() < 33)) {
			phase = 2;
			CastSpellNowNoScheduling(summon);
			ApplyAura(MA_BANISH);
			_unit->Root();
			timer = AddTimer(10000);
		}
		ParentClass::AIUpdate();
	}
};


class RazorlashAI:public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(RazorlashAI, MoonScriptBossAI);
	RazorlashAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(MA_RAZ_CLEAVE, Target_Current, 20, 0, 10);
		AddSpell(15976, Target_Current, 30, 0, 10);
	}
};

class VyletongueAI:public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(VyletongueAI, MoonScriptBossAI);
	VyletongueAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(VYLE_BLINK, Target_Self, 15, 0, 15);
		AddSpell(VYLE_MULTISHOT, Target_Current, 20, 0, 10);
		AddSpell(VYLE_SHOOT, Target_Current, 75, 0, 1);
		AddSpell(VYLE_SMOKE_BOMB, Target_Self, 20, 0, 25);
	}
};

class MeshlokAI:public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(MeshlokAI, MoonScriptBossAI);
	MeshlokAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(MA_STRIKE, Target_Current, 30, 0, 5);
		AddSpell(MESH_WARSTOMP, Target_Self, 15, 0, 15);
	}
};

class RotgripAI:public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(RotgripAI, MoonScriptBossAI);
	RotgripAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(ROT_FATAL_BITE, Target_Current, 20, 0, 10);
		AddSpell(15976, Target_Current, 30, 0, 10);
	}
};

class GizlockAI:public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(GizlockAI, MoonScriptBossAI);
	GizlockAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(GIZLOCK_BOMB, Target_RandomDestination, 15, 2, 15);
		AddSpell(GIZLOCK_FLASH_BOMB, Target_RandomDestination, 15, 0, 15);
		AddSpell(GIZLOCK_DRAGON_GUN, Target_Current, 30, 0, 10);
		AddSpell(VYLE_SHOOT, Target_Current, 75, 0, 1);
	}
};

#define BOSS_THERADRAS				12201
#define BOSS_ZAELTAR				12238
#define THERADRAS_DUSTFIELD			21909
#define THERADRAS_BOULDER			21832
#define THERADRAS_THRASH			3391
#define THERADRAS_REPULSIVEGAZE		21869

class TheradrasAI : public MoonScriptBossAI
{
	SpellDesc* mDustField;
	uint32 DustTimer;
	bool dusting;

	MOONSCRIPT_FACTORY_FUNCTION(TheradrasAI, MoonScriptBossAI);
	TheradrasAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
	{
		dusting = false;
		mDustField = AddSpell(THERADRAS_DUSTFIELD, Target_Self, 0, 0, 0);
		AddSpell(THERADRAS_BOULDER, Target_RandomPlayer, 30, 0, 2);
		AddSpell(THERADRAS_THRASH, Target_Self, 15, 0, 18);
		AddSpell(THERADRAS_REPULSIVEGAZE, Target_RandomPlayer, 20, 0, 21);
	}

	void OnCombatStart(Unit* mTarget)
    {	
		DustTimer = AddTimer(20000);
		ParentClass::OnCombatStart(mTarget);
	}
	
    void AIUpdate()
	{
		if (IsTimerFinished(DustTimer))
		{
			if(!dusting) {
				_unit->Root();
				CastSpellNowNoScheduling(mDustField);
				ResetTimer(DustTimer, 5000);
				dusting = true;
			}
			else {
				_unit->Unroot();
				ResetTimer(DustTimer, 20000);
				dusting = false;
			}
		}
		ParentClass::AIUpdate();
	}

	void OnDied(Unit *pKiller) {
		SpawnCreature(BOSS_ZAELTAR, 28.64f, 63.57f, -123.55f, 4.63f);
		ParentClass::OnDied(pKiller);
	}
};

#define BOSS_CELEBRAS				12225
#define BOSS_CELEBRAS_REDEEMED		13716
#define CELEBRAS_WRATH				21807
#define CELBBRAS_ENTANGLING_ROOTS	12747
#define CELEBRAS_CORRUPT_FORCES		21968
#define CELEBRAS_TWISTED_TRANQUIL	12225

class CelebrasAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(CelebrasAI, MoonScriptBossAI);
	CelebrasAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(CELEBRAS_WRATH, Target_Self, 75, 2, 8);
		AddSpell(CELBBRAS_ENTANGLING_ROOTS, Target_RandomPlayer, 20, 1.5, 15);
		AddSpell(CELEBRAS_TWISTED_TRANQUIL, Target_Self, 10, 0, 30);
		mCorrupt = AddSpell(CELEBRAS_CORRUPT_FORCES, Target_Self, 0, 0, 0);
	}
	
    void OnCombatStart(Unit* mTarget)
    {	
		CorruptTimer = AddTimer(20000);
		ParentClass::OnCombatStart(mTarget);
	}
	
    void AIUpdate()
	{
		if (IsTimerFinished(CorruptTimer))
		{
			CastSpellNowNoScheduling(mCorrupt);
			ResetTimer(CorruptTimer, 20000);
		}
		ParentClass::AIUpdate();
	}

	void OnDied(Unit* pKiller) {
		SpawnCreature(BOSS_CELEBRAS_REDEEMED);
		ParentClass::OnDied(pKiller);
	}
			
	SpellDesc* mCorrupt;
	uint32 CorruptTimer;
};


/* ScriptData
ISName: Landslide
IS%Complete: 90%
ISComment: Adds need to be removed on reset / death
ISCategory: Maraudon
EndScriptData */

#define BOSS_LANDSLIDE				12203
#define LANDSLIDE_KNOCKAWAY         18670
#define LANDSLIDE_TRAMPLE           5568
#define LANDSLIDE_LANDSLIDE         21808

class LandslideAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(LandslideAI, MoonScriptBossAI);
	LandslideAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
	{
		mTrample = AddSpell(LANDSLIDE_TRAMPLE, Target_Current, 0, 0, 0);
		mLandSlide = AddSpell(LANDSLIDE_LANDSLIDE, Target_Current, 0, 0, 0);
		mKnockAway = AddSpell(LANDSLIDE_KNOCKAWAY, Target_Self, 0, 0, 0);
	}
	
    void OnCombatStart(Unit* mTarget)
    {	
		mKATimer = AddTimer(8000);
		mTrampTimer = AddTimer(2000);
		ParentClass::OnCombatStart(mTarget);
		SetPhase(1);
	}
	
	
    void AIUpdate()
	{	
		if (IsTimerFinished(mKATimer))
		{
			CastSpellNowNoScheduling(mKnockAway);
			ResetTimer(mKATimer, 15000);
		}
		
		if (IsTimerFinished(mLSTimer))
		{
			CastSpellNowNoScheduling(mLandSlide);
			ResetTimer(mLSTimer, 60000);
		}
		
		if (IsTimerFinished(mTrampTimer))
		{
			CastSpellNowNoScheduling(mTrample);
			ResetTimer(mTrampTimer, 8000);
		}
		
		if (GetHealthPercent() < 60 && GetPhase() == 1)
		{
			CastSpellNowNoScheduling(mLandSlide);	
			mLSTimer = AddTimer(60000);
			SetPhase(2);
		}
		ParentClass::AIUpdate();
	}
	
	SpellDesc* 		mLandSlide;
	SpellDesc* 		mKnockAway;
	SpellDesc* 		mTrample;
	uint32			mTrampTimer;
	uint32			mKATimer;
	uint32			mLSTimer;
};

//trash
class VileLarvaAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VileLarvaAI, MoonScriptCreatureAI);
	VileLarvaAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_LARVA_GOO, Target_Self, 15, 0, 15);
	}
};

class ThessalaHydraAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ThessalaHydraAI, MoonScriptCreatureAI);
	ThessalaHydraAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_AQUA_JET, Target_Self, 15, 0, 15);
		AddSpell(MA_THRASH, Target_Self, 15, 0, 15);
	}
};

class TheradrimShardlingAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TheradrimShardlingAI, MoonScriptCreatureAI);
	TheradrimShardlingAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_SHARDLING_STRIKE, Target_Current, 30, 0, 5);
	}
};

class TheradrimGuardianAI: public MoonScriptCreatureAI
{
	SpellDesc *summon;

	MOONSCRIPT_FACTORY_FUNCTION(TheradrimGuardianAI, MoonScriptCreatureAI);
	TheradrimGuardianAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		summon = AddSpell(MA_SUMMON_SHARDLING, Target_Self, 0, 0, 0);
		AddSpell(MA_SHAMBLER_KNOCKDOWN, Target_Current, 20, 0, 10);
	}

	void OnDied(Unit *pKiller) {
		CastSpellNowNoScheduling(summon);
		ParentClass::OnDied(pKiller);
	}
};

class DiemetradonAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DiemetradonAI, MoonScriptCreatureAI);
	DiemetradonAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_SONIC_BURST, Target_Self, 15, 0, 15);
	}
};

class SpiritOfVengAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpiritOfVengAI, MoonScriptCreatureAI);
	SpiritOfVengAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_HOOKED_NET, Target_Current, 30, 0, 20);
		AddSpell(MA_MULTISHOT, Target_Current, 20, 0, 10, 5, 30);
		AddSpell(MA_SHOOT, Target_Current, 50, 0, 1, 5, 30);
	}
};

class SpiritOfMaraudosAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpiritOfMaraudosAI, MoonScriptCreatureAI);
	SpiritOfMaraudosAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_NET, Target_Current, 30, 0, 20);
		AddSpell(MA_SINISTER_STRIKE, Target_Current, 20, 0, 10);
		AddSpell(MA_SISTER_THROW, Target_Current, 20, 0, 5, 5, 30);
	}
};

class SpewedLarvaAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpewedLarvaAI, MoonScriptCreatureAI);
	SpewedLarvaAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_NOXIOUS_CATALYST, Target_Current, 30, 0, 5);
	}
};

class SisterOfCelebrasAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SisterOfCelebrasAI, MoonScriptCreatureAI);
	SisterOfCelebrasAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_SISTER_STRIKE, Target_Current, 30, 0, 5);
		AddSpell(MA_SISTER_THROW, Target_Current, 30, 0, 2, 5, 30);
	}
};

class PutridusTricksterAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PutridusTricksterAI, MoonScriptCreatureAI);
	PutridusTricksterAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_BACKSTAB, Target_Current, 15, 0, 20);
		AddSpell(MA_THRASH, Target_Self, 20, 0, 15);
	}
};

class PutridusShadowstalkerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PutridusShadowstalkerAI, MoonScriptCreatureAI);
	PutridusShadowstalkerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_EVASION, Target_Self, 15, 0, 60);
		AddSpell(MA_HARMSTRING, Target_Current, 30, 0, 10);
	}
};

class PutridusSatyrAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PutridusSatyrAI, MoonScriptCreatureAI);
	PutridusSatyrAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_GOUGE, Target_Current, 50, 0, 20);
		AddSpell(MA_SINISTER_STRIKE, Target_Current, 30, 0, 3);
	}
};

class PrimordialBehemothAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PrimordialBehemothAI, MoonScriptCreatureAI);
	PrimordialBehemothAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_BOULDER, Target_Current, 50, 2, 10);
		AddSpell(MA_TRAMPLE, Target_Self, 50, 0, 15);
	}
};

class PoisonSpriteAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PoisonSpriteAI, MoonScriptCreatureAI);
	PoisonSpriteAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_POISON_BOLT, Target_Current, 50, 2.5, 1);
	}
};

class NoxiousSlimeAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(NoxiousSlimeAI, MoonScriptCreatureAI);
	NoxiousSlimeAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_NOXIOUS_CLOUD, Target_Self, 30, 0, 10);
	}
};

class DeeprotTanglerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeeprotTanglerAI, MoonScriptCreatureAI);
	DeeprotTanglerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(MA_THORNS);
		AddSpell(MA_TANGLER_ENTANGLING_ROOTS, Target_Current, 30, 1.5, 10, 0, 30);
	}
};

class DeeprotStomperAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeeprotStomperAI, MoonScriptCreatureAI);
	DeeprotStomperAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_STRIKE, Target_Current, 30, 0, 6);
		AddSpell(MA_WARSTOMP, Target_Self, 15, 0, 15);
	}
};

class CorruptorAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CorruptorAI, MoonScriptCreatureAI);
	CorruptorAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_CORRUPTION, Target_RandomPlayer, 30, 2, 6, 0, 30);
		AddSpell(MA_NOXIOUS_CATALYST, Target_Current, 30, 0, 4);
	}
};

class ConstrictorVineAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ConstrictorVineAI, MoonScriptCreatureAI);
	ConstrictorVineAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_ENTANGLING_ROOTS, Target_Current, 15, 1.5, 20);//, 0, 30);
	}
};

class CelebrianDryadAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CelebrianDryadAI, MoonScriptCreatureAI);
	CelebrianDryadAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_DISPEL_MAGIC, Target_RandomFriendly, 30, 0, 5);
		AddSpell(MA_THROW, Target_Current, 95, 0, 10, 6, 30);
	}
};

class CavernShamblerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CavernShamblerAI, MoonScriptCreatureAI);
	CavernShamblerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_SHAMBLER_KNOCKDOWN, Target_Current, 20, 0, 10);
		AddSpell(MA_WILD_REGENERATION, Target_Self, 20, 3, 30);
	}
};

class CavernLurkerAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CavernLurkerAI, MoonScriptCreatureAI);
	CavernLurkerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_KNOCKDOWN, Target_Current, 20, 0, 10);
	}
};

class BarbedLasherAI: public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BarbedLasherAI, MoonScriptCreatureAI);
	BarbedLasherAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(MA_THORN_VOLLEY, Target_Self, 20, 0, 10);
	}
};


void SetupMaraudon(ScriptMgr * mgr)
{
	mgr->register_creature_script(BOSS_THERADRAS, 			&TheradrasAI::Create);
	mgr->register_creature_script(BOSS_CELEBRAS, 			&CelebrasAI::Create);
	mgr->register_creature_script(BOSS_LANDSLIDE, 			&LandslideAI::Create);
	mgr->register_creature_script(BOSS_NOXXION, 			&NoxxionAI::Create);
	mgr->register_creature_script(BOSS_RAZORLASH, 			&RazorlashAI::Create);
	mgr->register_creature_script(BOSS_VYLETONGUE, 			&VyletongueAI::Create);
	mgr->register_creature_script(BOSS_ROTGRIP, 			&RotgripAI::Create);
	mgr->register_creature_script(BOSS_TINKERER_GIZLOCK, 	&GizlockAI::Create);

	mgr->register_creature_script(RARE_MESHLOK, 			&MeshlokAI::Create);

	mgr->register_gameobject_script(PORTAL_ACTIVATE,		&MaraudonPortalAI::Create);

	mgr->register_creature_script(MA_BARBED_LASHER, 		&BarbedLasherAI::Create);
	mgr->register_creature_script(MA_CAVERN_LURKER, 		&CavernLurkerAI::Create);
	mgr->register_creature_script(MA_CAVERN_SHAMBLER, 		&CavernShamblerAI::Create);
	mgr->register_creature_script(MA_CELEBRIAN_DRYAD, 		&CelebrianDryadAI::Create);
	mgr->register_creature_script(MA_CONSTRICTOR_VINE, 		&ConstrictorVineAI::Create);
	mgr->register_creature_script(MA_CORRUPTOR,				&CorruptorAI::Create);
	mgr->register_creature_script(MA_DEEPROT_STOMPER,		&DeeprotStomperAI::Create);
	mgr->register_creature_script(MA_DEEPROT_TANGLER,		&DeeprotTanglerAI::Create);
	mgr->register_creature_script(MA_NOXIOUS_SLIME,			&NoxiousSlimeAI::Create);
	mgr->register_creature_script(MA_NOXXIOUS_SCION,		&CorruptorAI::Create);
	mgr->register_creature_script(MA_POISON_SPRITE,			&PoisonSpriteAI::Create);
	mgr->register_creature_script(MA_PRIMORDIAL_BEHEMOTH,	&PrimordialBehemothAI::Create);
	mgr->register_creature_script(MA_PUTRIDUS_SATYR,		&PutridusSatyrAI::Create);
	mgr->register_creature_script(MA_PUTRIDUS_SHADOWSTALKER,&PutridusShadowstalkerAI::Create);
	mgr->register_creature_script(MA_PUTRIDUS_TRICKSTER,	&PutridusTricksterAI::Create);
	mgr->register_creature_script(MA_SISTER_OF_CELEBRAS,	&SisterOfCelebrasAI::Create);
	mgr->register_creature_script(MA_SPEWED_LARVA,			&SpewedLarvaAI::Create);
	mgr->register_creature_script(MA_SPIRIT_OF_MARAUDOS,	&SpiritOfMaraudosAI::Create);
	mgr->register_creature_script(MA_SPIRIT_OF_VENG,		&SpiritOfVengAI::Create);
	mgr->register_creature_script(MA_SUBTERRANEAN_DIEMETRADON, &DiemetradonAI::Create);
	mgr->register_creature_script(MA_THERADRIM_GUARDIAN,	&TheradrimGuardianAI::Create);
	mgr->register_creature_script(MA_THERADRIM_SHARDLING,	&TheradrimShardlingAI::Create);
	mgr->register_creature_script(MA_THESSALA_HYDRA,		&ThessalaHydraAI::Create);
	mgr->register_creature_script(MA_VILE_LARVA,			&VileLarvaAI::Create);
}