/*
 * ChronoEmu Scripts - Molten Core
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by Sphere
 */
 
#include "StdAfx.h"
#include "../Setup.h"
#include "../../common/Base.h"
#include "Raid_MoltenCore.h"

bool LucifronIsDead;
bool MagmadarIsDead;
bool GehennasIsDead;
bool GarrIsDead;
bool GeddonIsDead;
bool ShazzrahIsDead;
bool GolemaggIsDead;
bool MajordomoIsDead;

/* Creature Data
Creature Name:
Creature Complete Percentage: 0
Creature Comment:
End Creature Data */


/************************************************************************/
/* Lucifron and Adds													*/
/************************************************************************/

/* Creature Data
Creature Name: Lucifron
Creature Complete Percentage: 100%
Creature Comment: 
End Creature Data */

class LucifronAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(LucifronAI, MoonScriptBossAI);
	LucifronAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(LUCIFRON_IMPENDING_DOOM, Target_Self, 15, 0, 10);
		AddSpell(LUCIFRON_LUCIFRONS_CURSE, Target_Self, 100, 0, 15);
		AddSpell(LUCIFRON_SHADOW_SHOCK, Target_Self, 8, 0, 6);
	}
	
	void OnDied(Unit* pKiller)
	{
		Creature* Magmadar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1144.63f, -1020.48f, -185.663f, 11982);
		Creature* Gehennas = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(898.074f, -545.87f, -203.405f, 12259);
		Creature* Garr     = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(690.306f, -498.753f, -214.27f, 12057);
		Creature* Shazzrah = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(587.303f, -801.975f, -205.158f, 12264);
		Creature* Geddon   = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(680.061f, -849.253f, -207.93f, 12056);
		Creature* Golemagg = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(793.248f, -998.265f, -206.762f, 11988);
		Creature* Sulfurus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(601.087f, -1179.11f, -195.882f, 12098);
		
		if((!Magmadar || !Magmadar->isAlive()) && (!Gehennas || !Gehennas->isAlive()) &&
		(!Garr || !Garr->isAlive()) && (!Shazzrah || !Shazzrah->isAlive()) && (!Geddon || !Geddon->isAlive()) &&
		(!Sulfurus || !Sulfurus->isAlive()) && (!Golemagg || !Golemagg->isAlive()))
		{
			Creature* Elite1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 726.755005f, -1205.589966f, -119.176003f, 1.449039f, true, false, 0, 0);
			Creature* Elite2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 759.440979f, -1194.760010f, -118.794998f, 2.073430f, true, false, 0, 0);
			Creature* Elite3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 764.520996f, -1164.670044f, -119.707001f, 3.008055f, true, false, 0, 0);
			Creature* Elite4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 724.450012f, -1150.910034f, -120.114998f, 3.577468f, true, false, 0, 0);
			Creature* Healer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 733.827026f, -1206.750000f, -118.829002f, 1.449039f, true, false, 0, 0);
			Creature* Healer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 762.200989f, -1190.630005f, -118.984001f, 2.073430f, true, false, 0, 0);
			Creature* Healer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 761.750977f, -1159.400024f, -119.906998f, 3.008055f, true, false, 0, 0);
			Creature* Healer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 729.835999f, -1149.260010f, -120.081001f, 3.577468f, true, false, 0, 0);
			Creature* MajorDomo = _unit->GetMapMgr()->GetInterface()->SpawnCreature(12018, 736.831970f, -1175.880005f, -119.065002f, 3.261960f, true, false, 0, 0);
		}
	}
};

/* Creature Data
Creature Name: Flamewaker Protector
Creature Complete Percentage: 75%
Creature Comment: Mind Control Casts, but doesn't charm them and take control. Gives Buff instead of debuff.
End Creature Data */

class FlamewakerProtectorAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(FlamewakerProtectorAI, MoonScriptCreatureAI);
	FlamewakerProtectorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		Lucifron = nullptr;
	
		AddSpell(FLAMEWAKER_PROT_CLEAVE, Target_Current, 8, 0, 0, 0, 15);
		DominateMind = AddSpell(FLAMEWAKER_PROT_DM, Target_RandomPlayer, 8, 0, 0, 0, 20);
	}
	
	void OnCombatStart(Unit *pTarget)
	{
		Lucifron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1053.15f, -990.755f, -182.662f, 12118);
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate()
	{
		/*
			They say, if you pull them far enough away from Lucifron, There shalt be no mind control.
		*/
		if(Lucifron && GetRangeToUnit((Unit*)Lucifron) < 500)
			DominateMind->mEnabled = true;
		else
			DominateMind->mEnabled = false;

		ParentClass::AIUpdate();
	}

	Creature * Lucifron;
	SpellDesc* DominateMind;
};

/************************************************************************/
/* Magmadar and Adds													*/
/************************************************************************/


/* Creature Data
Creature Name: Ancient Core Hound
Creature Complete Percentage: 100%
Creature Comment:
End Creature Data */	

bool IsMagmadarDead[10000000];
class AncientCoreHoundAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(AncientCoreHoundAI, MoonScriptCreatureAI);
    AncientCoreHoundAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		if(IsMagmadarDead[_unit->GetInstanceID()] == true)
		{
			Despawn(0, 0);
			return;
		}
		AddSpell(ACHOUND_LAVA_BREATH, Target_Self, 20, 0, 3);
		AddSpell(ACHOUND_VICIOUS_BITE, Target_Self, 20, 0, 0);

		switch( RandomUInt(5) )
		{
			case 0: AddSpell(ACHOUND_GROUND_STOMP, Target_Self, 20, 0, 15); break;
			case 1: AddSpell(ACHOUND_ANCIENT_DREAD, Target_Self, 20, 0, 15); break;
			case 2: AddSpell(ACHOUND_ANCIENT_DESPAIR, Target_Self, 20, 0, 15); break;
			case 3: AddSpell(ACHOUND_CAUTERIZING_FLAMES, Target_Self, 20, 0, 15); break;
			case 4: AddSpell(ACHOUND_WITHERING_HEAT, Target_Self, 20, 0, 15); break;
			case 5: AddSpell(ACHOUND_ANCIENT_HYSTERIA, Target_Self, 20, 0, 15); break;
		}
    }
};

/* Creature Data
Creature Name: Core Hound
Creature Complete Percentage: 100%
Creature Comment:
End Creature Data */	

class CoreHoundAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(CoreHoundAI, MoonScriptCreatureAI);
    CoreHoundAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		if(IsMagmadarDead[_unit->GetInstanceID()] == true)
		{
			Despawn(0, 0);
			return;
		}
		AddSpell(COREHOUND_SERRATEDBITE, Target_RandomPlayer, 10, 0, 0, 0, 10);
    }
};

/* Creature Data
Creature Name: Magmadar
Creature Complete Percentage: 80%
Creature Comment: Magma Spit needs to be checked. Otherwise everything else seems to work.
End Creature Data */		
			
class MagmadarAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(MagmadarAI, MoonScriptBossAI);
	MagmadarAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		// Magmadar Should use 19411 for Lava bomb. 
		// However, because the spell is setup weird,
		// We'll use the debuff it gives instead.
		AddSpell(MAGMADAR_MAGMA_SPIT, Target_Current, 100, 0, 15);
		AddSpell(MAGMADAR_LAVA_BREATH, Target_Current, 100, 0, 10);
		AddSpell(MAGMADAR_ENRAGE, Target_Self, 100, 0, 30);
		AddSpell(MAGMADAR_PANIC, Target_Current, 100, 0, 20);
		AddSpell(MAGMADAR_LAVA_BOMB_DEBUFF, Target_RandomPlayer, 100, 0, 12);
	}
	
	void OnDied(Unit* pKiller)
	{
		Creature* Lucifron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1053.15f, -990.755f, -182.662f, 12118);
		Creature* Gehennas = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(898.074f, -545.87f, -203.405f, 12259);
		Creature* Garr     = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(690.306f, -498.753f, -214.27f, 12057);
		Creature* Shazzrah = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(587.303f, -801.975f, -205.158f, 12264);
		Creature* Geddon   = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(680.061f, -849.253f, -207.93f, 12056);
		Creature* Golemagg = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(793.248f, -998.265f, -206.762f, 11988);
		Creature* Sulfurus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(601.087f, -1179.11f, -195.882f, 12098);
		
		if((!Lucifron || !Lucifron->isAlive()) && (!Gehennas || !Gehennas->isAlive()) &&
		(!Garr || !Garr->isAlive()) && (!Shazzrah || !Shazzrah->isAlive()) && (!Geddon || !Geddon->isAlive()) &&
		(!Sulfurus || !Sulfurus->isAlive()) && (!Golemagg || !Golemagg->isAlive()))
		{
			Creature* Elite1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 726.755005f, -1205.589966f, -119.176003f, 1.449039f, true, false, 0, 0);
			Creature* Elite2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 759.440979f, -1194.760010f, -118.794998f, 2.073430f, true, false, 0, 0);
			Creature* Elite3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 764.520996f, -1164.670044f, -119.707001f, 3.008055f, true, false, 0, 0);
			Creature* Elite4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 724.450012f, -1150.910034f, -120.114998f, 3.577468f, true, false, 0, 0);
			Creature* Healer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 733.827026f, -1206.750000f, -118.829002f, 1.449039f, true, false, 0, 0);
			Creature* Healer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 762.200989f, -1190.630005f, -118.984001f, 2.073430f, true, false, 0, 0);
			Creature* Healer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 761.750977f, -1159.400024f, -119.906998f, 3.008055f, true, false, 0, 0);
			Creature* Healer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 729.835999f, -1149.260010f, -120.081001f, 3.577468f, true, false, 0, 0);
			Creature* MajorDomo = _unit->GetMapMgr()->GetInterface()->SpawnCreature(12018, 736.831970f, -1175.880005f, -119.065002f, 3.261960f, true, false, 0, 0);
		}
	}
};

/************************************************************************/
/* Gehennas and Adds													*/
/************************************************************************/

/* Creature Data
Creature Name: Flamewaker
Creature Complete Percentage: 100%
Creature Comment:
End Creature Data */

class FlamewakerAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(FlamewakerAI, MoonScriptCreatureAI);
	FlamewakerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(FLAMEWAKER_SUNDER_ARMOR, Target_Current, 8, 0, 0);
		AddSpell(FLAMEWAKER_FISTOFRAG, Target_Self, 8, 0, 0);
		AddSpell(FLAMEWAKER_STRIKE, Target_Current, 14, 0, 0);
	}
};

/* Creature Data
Creature Name: Gehennas
Creature Complete Percentage: 100%
Creature Comment:
End Creature Data */

class GehennasAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(GehennasAI, MoonScriptBossAI);
	GehennasAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(GEHENNAS_SHADOW_BOLT, Target_RandomPlayer, 10, 0, 0, 0, 40);
		AddSpell(GEHENNAS_RAIN_OF_FIRE, Target_RandomPlayer, 10, 0, 10); 
		AddSpell(GEHENNAS_CURSE, Target_Self, 8, 6, 0, 0, 40);
	}
	
	void OnDied(Unit* pKiller)
	{
		Creature* Magmadar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1144.63f, -1020.48f, -185.663f, 11982);
		Creature* Lucifron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1053.15f, -990.755f, -182.662f, 12118);
		Creature* Garr     = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(690.306f, -498.753f, -214.27f, 12057);
		Creature* Shazzrah = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(587.303f, -801.975f, -205.158f, 12264);
		Creature* Geddon   = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(680.061f, -849.253f, -207.93f, 12056);
		Creature* Golemagg = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(793.248f, -998.265f, -206.762f, 11988);
		Creature* Sulfurus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(601.087f, -1179.11f, -195.882f, 12098);
		
		if((!Magmadar || !Magmadar->isAlive()) && (!Lucifron || !Lucifron->isAlive()) &&
		(!Garr || !Garr->isAlive()) && (!Shazzrah || !Shazzrah->isAlive()) && (!Geddon || !Geddon->isAlive()) &&
		(!Sulfurus || !Sulfurus->isAlive()) && (!Golemagg || !Golemagg->isAlive()))
		{
			Creature* Elite1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 726.755005f, -1205.589966f, -119.176003f, 1.449039f, true, false, 0, 0);
			Creature* Elite2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 759.440979f, -1194.760010f, -118.794998f, 2.073430f, true, false, 0, 0);
			Creature* Elite3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 764.520996f, -1164.670044f, -119.707001f, 3.008055f, true, false, 0, 0);
			Creature* Elite4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 724.450012f, -1150.910034f, -120.114998f, 3.577468f, true, false, 0, 0);
			Creature* Healer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 733.827026f, -1206.750000f, -118.829002f, 1.449039f, true, false, 0, 0);
			Creature* Healer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 762.200989f, -1190.630005f, -118.984001f, 2.073430f, true, false, 0, 0);
			Creature* Healer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 761.750977f, -1159.400024f, -119.906998f, 3.008055f, true, false, 0, 0);
			Creature* Healer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 729.835999f, -1149.260010f, -120.081001f, 3.577468f, true, false, 0, 0);
			Creature* MajorDomo = _unit->GetMapMgr()->GetInterface()->SpawnCreature(12018, 736.831970f, -1175.880005f, -119.065002f, 3.261960f, true, false, 0, 0);
		}
	}
	
};

/************************************************************************/
/* Garr and Adds														*/
/************************************************************************/

/* Creature Data
Creature Name: Firesworn
Creature Complete Percentage: 100%
Creature Comment:
End Creature Data */

class FireswornAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(FireswornAI, MoonScriptCreatureAI);
	FireswornAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		mGarr = nullptr;

		//Spells
		AddSpell(FIRESWORN_IMMOLATE, Target_Current, 10, 0, 0, 0, 40);
		mEruption = AddSpell(FIRESWORN_ERUPTION, Target_Self, 0, 0, 0);
		mSeparationAnxiety = AddSpell(FIRESWORN_SEPANXIETY, Target_Self, 0, 5, 5);
	}

	void OnCombatStart(Unit* pTarget)
	{
		mGarr = (MoonScriptBossAI*)GetNearestCreature(12057);
		ParentClass::OnCombatStart(pTarget);
	}

	void OnDied(Unit* pKiller)
	{
		CastSpellNowNoScheduling(mEruption);
		ParentClass::OnDied(pKiller);
	}

	void AIUpdate()
	{
		if( mGarr && mGarr->IsAlive() && GetRange(mGarr) > 100 )
		{
			CastSpell(mSeparationAnxiety);
		}
		ParentClass::AIUpdate();
	}

	SpellDesc*			mEruption;
	SpellDesc*			mSeparationAnxiety;
	MoonScriptBossAI*	mGarr;
};

/* Creature Data
Creature Name: Garr
Creature Complete Percentage: 100%
Creature Comment:
End Creature Data */

class GarrAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(GarrAI, MoonScriptBossAI);
	GarrAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(GARR_ANTIMAGIC_PULSE, Target_Self, 15, 0, 0);
		AddSpell(GARR_MAGMA_SHACKLES, Target_Self, 15, 0, 0);
	}
	
	void OnDied(Unit* pKiller)
	{
		Creature* Magmadar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1144.63f, -1020.48f, -185.663f, 11982);
		Creature* Lucifron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1053.15f, -990.755f, -182.662f, 12118);
		Creature* Gehennas = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(898.074f, -545.87f, -203.405f, 12259);
		Creature* Shazzrah = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(587.303f, -801.975f, -205.158f, 12264);
		Creature* Geddon   = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(680.061f, -849.253f, -207.93f, 12056);
		Creature* Golemagg = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(793.248f, -998.265f, -206.762f, 11988);
		Creature* Sulfurus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(601.087f, -1179.11f, -195.882f, 12098);
		
		if((!Magmadar || !Magmadar->isAlive()) && (!Lucifron || !Lucifron->isAlive()) && (!Gehennas || !Gehennas->isAlive()) &&
		(!Shazzrah || !Shazzrah->isAlive()) && (!Geddon || !Geddon->isAlive()) &&
		(!Sulfurus || !Sulfurus->isAlive()) && (!Golemagg || !Golemagg->isAlive()))
		{
			Creature* Elite1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 726.755005f, -1205.589966f, -119.176003f, 1.449039f, true, false, 0, 0);
			Creature* Elite2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 759.440979f, -1194.760010f, -118.794998f, 2.073430f, true, false, 0, 0);
			Creature* Elite3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 764.520996f, -1164.670044f, -119.707001f, 3.008055f, true, false, 0, 0);
			Creature* Elite4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 724.450012f, -1150.910034f, -120.114998f, 3.577468f, true, false, 0, 0);
			Creature* Healer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 733.827026f, -1206.750000f, -118.829002f, 1.449039f, true, false, 0, 0);
			Creature* Healer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 762.200989f, -1190.630005f, -118.984001f, 2.073430f, true, false, 0, 0);
			Creature* Healer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 761.750977f, -1159.400024f, -119.906998f, 3.008055f, true, false, 0, 0);
			Creature* Healer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 729.835999f, -1149.260010f, -120.081001f, 3.577468f, true, false, 0, 0);
			Creature* MajorDomo = _unit->GetMapMgr()->GetInterface()->SpawnCreature(12018, 736.831970f, -1175.880005f, -119.065002f, 3.261960f, true, false, 0, 0);
		}
	}
	
};

/************************************************************************/
/* Barron Geddon and Adds												*/
/************************************************************************/

/* Creature Data
Creature Name: Barron Geddon
Creature Complete Percentage: 100%
Creature Comment:
End Creature Data */

class BaronGeddonAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(BaronGeddonAI, MoonScriptBossAI);
	BaronGeddonAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(GEDDON_INFERNO, Target_Self, 10, 0, 0);
		AddSpell(GEDDON_IGNITE_MANA, Target_Self, 10, 0, 0);
		AddSpell(GEDDON_LIVING_BOMB, Target_RandomPlayer, 100, 0, 0, 0, 45);
		Explode = AddSpell(GEDDON_ARMAGEDDON, Target_Self, 0, 0, 0);
		Explode->mEnabled = false;
	}
	
	void AIUpdate()
	{
		if(GetHealthPercent() <= 2) // 5 SECONDS HE GO BOOM.
		{
			Emote("Baron Geddon performs one last service for Ragnaros.", Text_Emote, 0);
			ExplodeTimer = AddTimer(5000);
			Explode->mEnabled = true;
		}
		if(Explode->mEnabled == true && IsTimerFinished(ExplodeTimer))
			CastSpellNowNoScheduling(Explode);
		ParentClass::AIUpdate();
	}
	
	void OnDied(Unit* pKiller)
	{
		Creature* Magmadar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1144.63f, -1020.48f, -185.663f, 11982);
		Creature* Lucifron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1053.15f, -990.755f, -182.662f, 12118);
		Creature* Gehennas = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(898.074f, -545.87f, -203.405f, 12259);
		Creature* Garr     = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(690.306f, -498.753f, -214.27f, 12057);
		Creature* Shazzrah = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(587.303f, -801.975f, -205.158f, 12264);
		Creature* Golemagg = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(793.248f, -998.265f, -206.762f, 11988);
		Creature* Sulfurus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(601.087f, -1179.11f, -195.882f, 12098);
		
		if((!Magmadar || !Magmadar->isAlive()) && (!Lucifron || !Lucifron->isAlive()) && (!Gehennas || !Gehennas->isAlive()) &&
		(!Garr || !Garr->isAlive()) && (!Shazzrah || !Shazzrah->isAlive()) &&
		(!Sulfurus || !Sulfurus->isAlive()) && (!Golemagg || !Golemagg->isAlive()))
		{
			Creature* Elite1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 726.755005f, -1205.589966f, -119.176003f, 1.449039f, true, false, 0, 0);
			Creature* Elite2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 759.440979f, -1194.760010f, -118.794998f, 2.073430f, true, false, 0, 0);
			Creature* Elite3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 764.520996f, -1164.670044f, -119.707001f, 3.008055f, true, false, 0, 0);
			Creature* Elite4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 724.450012f, -1150.910034f, -120.114998f, 3.577468f, true, false, 0, 0);
			Creature* Healer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 733.827026f, -1206.750000f, -118.829002f, 1.449039f, true, false, 0, 0);
			Creature* Healer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 762.200989f, -1190.630005f, -118.984001f, 2.073430f, true, false, 0, 0);
			Creature* Healer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 761.750977f, -1159.400024f, -119.906998f, 3.008055f, true, false, 0, 0);
			Creature* Healer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 729.835999f, -1149.260010f, -120.081001f, 3.577468f, true, false, 0, 0);
			Creature* MajorDomo = _unit->GetMapMgr()->GetInterface()->SpawnCreature(12018, 736.831970f, -1175.880005f, -119.065002f, 3.261960f, true, false, 0, 0);
		}
	}

	uint32 ExplodeTimer;
	SpellDesc* Explode;
};

/************************************************************************/
/* Shazzrah and Adds													*/
/************************************************************************/

/* Creature Data
Creature Name: Shazzrah
Creature Complete Percentage: 100%
Creature Comment:
End Creature Data */

class ShazzrahAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ShazzrahAI, MoonScriptBossAI);
	ShazzrahAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{	
		AddSpell(SHAZZRAH_CURSE, Target_Self, 8, 0, 0);
		AddSpell(SHAZZRAH_MAGIC_GROUNDING, Target_Self, 6, 0, 0);
		AddSpell(SHAZZRAH_COUNTERSPELL, Target_Self, 6, 0, 0);

		mBlink = AddSpell(SHAZZRAH_BLINK, Target_RandomPlayer, 0, 0, 0);
		mArcaneExplosion = AddSpell(SHAZZRAH_ARCANE_EXPLOSION, Target_Self, 0, 0, 0);
		AddSpellFunc(&SpellFunc_ShazzrahBlink, Target_RandomPlayer, 100, -1, 45);
	}
	
	void OnDied(Unit* pKiller)
	{
		Creature* Magmadar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1144.63f, -1020.48f, -185.663f, 11982);
		Creature* Lucifron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1053.15f, -990.755f, -182.662f, 12118);
		Creature* Gehennas = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(898.074f, -545.87f, -203.405f, 12259);
		Creature* Garr     = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(690.306f, -498.753f, -214.27f, 12057);
		Creature* Geddon   = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(680.061f, -849.253f, -207.93f, 12056);
		Creature* Golemagg = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(793.248f, -998.265f, -206.762f, 11988);
		Creature* Sulfurus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(601.087f, -1179.11f, -195.882f, 12098);
		
		if((!Magmadar || !Magmadar->isAlive()) && (!Lucifron || !Lucifron->isAlive()) && (!Gehennas || !Gehennas->isAlive()) &&
		(!Garr || !Garr->isAlive()) && (!Geddon || !Geddon->isAlive()) &&
		(!Sulfurus || !Sulfurus->isAlive()) && (!Golemagg || !Golemagg->isAlive()))
		{
			Creature* Elite1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 726.755005f, -1205.589966f, -119.176003f, 1.449039f, true, false, 0, 0);
			Creature* Elite2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 759.440979f, -1194.760010f, -118.794998f, 2.073430f, true, false, 0, 0);
			Creature* Elite3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 764.520996f, -1164.670044f, -119.707001f, 3.008055f, true, false, 0, 0);
			Creature* Elite4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 724.450012f, -1150.910034f, -120.114998f, 3.577468f, true, false, 0, 0);
			Creature* Healer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 733.827026f, -1206.750000f, -118.829002f, 1.449039f, true, false, 0, 0);
			Creature* Healer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 762.200989f, -1190.630005f, -118.984001f, 2.073430f, true, false, 0, 0);
			Creature* Healer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 761.750977f, -1159.400024f, -119.906998f, 3.008055f, true, false, 0, 0);
			Creature* Healer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 729.835999f, -1149.260010f, -120.081001f, 3.577468f, true, false, 0, 0);
			Creature* MajorDomo = _unit->GetMapMgr()->GetInterface()->SpawnCreature(12018, 736.831970f, -1175.880005f, -119.065002f, 3.261960f, true, false, 0, 0);
		}
	}

	SpellDesc* mBlink;
	SpellDesc* mArcaneExplosion;
};

void SpellFunc_ShazzrahBlink(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	ShazzrahAI* Shazzrah = ( pCreatureAI ) ? (ShazzrahAI*)pCreatureAI : nullptr;
	if( Shazzrah )
	{
			// Sloppy code is sloppy, but it works with no issues.
			Shazzrah->CastSpell(Shazzrah->mBlink);
			float PosX = pTarget->GetPositionX();
			float PosY = pTarget->GetPositionY();
			float PosZ = pTarget->GetPositionZ();
			Shazzrah->GetUnit()->SetPosition(PosX, PosY, PosZ, 0, false);
			Shazzrah->CastSpell(Shazzrah->mArcaneExplosion);
			Shazzrah->ClearHateList();
			Shazzrah->AggroNearestPlayer();
	}
};

/************************************************************************/
/* Sulfuron and Adds													*/
/************************************************************************/

/* Creature Data
Creature Name: Flamewaker Priest
Creature Complete Percentage: 100%
Creature Comment:
End Creature Data */

class FlamewakerPriestAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FlamewakerPriestAI, MoonScriptCreatureAI);
    FlamewakerPriestAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(FLAMEWAKERPRIEST_DMENDING, Target_WoundedFriendly, 8, 2, 0);
		AddSpell(FLAMEWAKERPRIEST_DSTRIKE, Target_Self, 12, 0, 0);
		AddSpell(FLAMEWAKERPRIEST_IMMOLATE, Target_RandomPlayer, 15, 0, 20);
		AddSpell(FLAMEWAKERPRIEST_SWPAIN, Target_RandomPlayer, 15, 0, 15);
	}
};

/* Creature Data
Creature Name: Sulfuron Harbinger
Creature Complete Percentage: 100%
Creature Comment:
End Creature Data */

class SulfuronAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SulfuronAI, MoonScriptBossAI);
	SulfuronAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(SULFURON_DEMORALIZING_SHOUT, Target_Self, 15, 0, 30);
		AddSpell(SULFURON_INSPIRE, Target_Self, 8, 0, 10);
		AddSpell(SULFURON_HAND_OF_RAGNAROS, Target_Self, 10, 0, 0);
		AddSpell(SULFURON_FLAME_SPEAR, Target_Current, 10, 0, 0);
	}
	
	void OnDied(Unit* pKiller)
	{
		Creature* Magmadar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1144.63f, -1020.48f, -185.663f, 11982);
		Creature* Lucifron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1053.15f, -990.755f, -182.662f, 12118);
		Creature* Gehennas = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(898.074f, -545.87f, -203.405f, 12259);
		Creature* Garr     = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(690.306f, -498.753f, -214.27f, 12057);
		Creature* Shazzrah = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(587.303f, -801.975f, -205.158f, 12264);
		Creature* Geddon   = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(680.061f, -849.253f, -207.93f, 12056);
		Creature* Golemagg = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(793.248f, -998.265f, -206.762f, 11988);
		
		if((!Magmadar || !Magmadar->isAlive()) && (!Lucifron || !Lucifron->isAlive()) && (!Gehennas || !Gehennas->isAlive()) &&
		(!Garr || !Garr->isAlive()) && (!Shazzrah || !Shazzrah->isAlive()) && (!Geddon || !Geddon->isAlive()) &&
		(!Golemagg || !Golemagg->isAlive()))
		{
			Creature* Elite1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 726.755005f, -1205.589966f, -119.176003f, 1.449039f, true, false, 0, 0);
			Creature* Elite2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 759.440979f, -1194.760010f, -118.794998f, 2.073430f, true, false, 0, 0);
			Creature* Elite3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 764.520996f, -1164.670044f, -119.707001f, 3.008055f, true, false, 0, 0);
			Creature* Elite4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 724.450012f, -1150.910034f, -120.114998f, 3.577468f, true, false, 0, 0);
			Creature* Healer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 733.827026f, -1206.750000f, -118.829002f, 1.449039f, true, false, 0, 0);
			Creature* Healer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 762.200989f, -1190.630005f, -118.984001f, 2.073430f, true, false, 0, 0);
			Creature* Healer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 761.750977f, -1159.400024f, -119.906998f, 3.008055f, true, false, 0, 0);
			Creature* Healer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 729.835999f, -1149.260010f, -120.081001f, 3.577468f, true, false, 0, 0);
			Creature* MajorDomo = _unit->GetMapMgr()->GetInterface()->SpawnCreature(12018, 736.831970f, -1175.880005f, -119.065002f, 3.261960f, true, false, 0, 0);
		}
	}
};

/************************************************************************/
/* Golemagg and Adds													*/
/************************************************************************/

class CoreRagerAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(CoreRagerAI, MoonScriptCreatureAI);
    CoreRagerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		Golemagg = nullptr;

		AddSpell(CORERAGER_MANGLE, Target_Current, 20, 0, 20);
    }

	void OnCombatStart(Unit *pTarget)
	{
		Golemagg = GetUnit()->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(793.248f, -998.265f, -206.762f, 11988);
		ParentClass::OnCombatStart(pTarget);
	}
	
	void AIUpdate()
	{
		// If Health is less than 50% and Golemagg is still alive then they regain full health
		if(GetHealthPercent() <= 50 && Golemagg && Golemagg->isAlive())
		{
			Regenerate();
			Emote("Core Rager refuses to die while it's master is in trouble.", Text_Emote);
		}
		ParentClass::AIUpdate();
	}

	Creature* Golemagg;
};

/* Creature Data
Creature Name: Golemagg the Incinerator
Creature Complete Percentage: 50%
Creature Comment: Magma Splash needs scripting
End Creature Data */

class GolemaggAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(GolemaggAI, MoonScriptBossAI);
	GolemaggAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		CoreRager1 = CoreRager2 = nullptr;
		GolemaggTrust = AddSpell(GOLEMAGG_TRUST, Target_Self, 8, 0, 3);
		AddSpell(GOLEMAGG_MAGMA_SPLASH, Target_Self, 15, 0, 0);
		AddSpell(GOLEMAGG_PYROBLAST, Target_RandomPlayer, 10, 0, 8, 0, 40); 
		AddPhaseSpell(2, AddSpell(GOLEMAGG_EARTHQUAKE, Target_Self, 10, 0, 0));
		
	}
	
	void OnDied(Unit* pKiller)
	{
		Creature* Magmadar = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1144.63f, -1020.48f, -185.663f, 11982);
		Creature* Lucifron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1053.15f, -990.755f, -182.662f, 12118);
		Creature* Gehennas = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(898.074f, -545.87f, -203.405f, 12259);
		Creature* Garr     = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(690.306f, -498.753f, -214.27f, 12057);
		Creature* Shazzrah = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(587.303f, -801.975f, -205.158f, 12264);
		Creature* Geddon   = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(680.061f, -849.253f, -207.93f, 12056);
		Creature* Sulfurus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(601.087f, -1179.11f, -195.882f, 12098);
		
		if((!Magmadar || !Magmadar->isAlive()) && (!Lucifron || !Lucifron->isAlive()) && (!Gehennas || !Gehennas->isAlive()) &&
		(!Garr || !Garr->isAlive()) && (!Shazzrah || !Shazzrah->isAlive()) && (!Geddon || !Geddon->isAlive()) &&
		(!Sulfurus || !Sulfurus->isAlive()))
		{
			Creature* Elite1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 726.755005f, -1205.589966f, -119.176003f, 1.449039f, true, false, 0, 0);
			Creature* Elite2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 759.440979f, -1194.760010f, -118.794998f, 2.073430f, true, false, 0, 0);
			Creature* Elite3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 764.520996f, -1164.670044f, -119.707001f, 3.008055f, true, false, 0, 0);
			Creature* Elite4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11664, 724.450012f, -1150.910034f, -120.114998f, 3.577468f, true, false, 0, 0);
			Creature* Healer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 733.827026f, -1206.750000f, -118.829002f, 1.449039f, true, false, 0, 0);
			Creature* Healer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 762.200989f, -1190.630005f, -118.984001f, 2.073430f, true, false, 0, 0);
			Creature* Healer3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 761.750977f, -1159.400024f, -119.906998f, 3.008055f, true, false, 0, 0);
			Creature* Healer4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11663, 729.835999f, -1149.260010f, -120.081001f, 3.577468f, true, false, 0, 0);
			Creature* MajorDomo = _unit->GetMapMgr()->GetInterface()->SpawnCreature(12018, 736.831970f, -1175.880005f, -119.065002f, 3.261960f, true, false, 0, 0);
		}
	}
	
	void OnCombatStart(Unit *pTarget)
	{
		CoreRager1 = GetUnit()->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(783.448f, -1017.07f, -205.389f, 11672);
		CoreRager2 = GetUnit()->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(785.579f, -974.27f, -207.111f, 11672);
		ParentClass::OnCombatStart(pTarget);
	}
	
	void AIUpdate()
	{
		// 40 yrds
		if((CoreRager1 && CoreRager1->isAlive() && GetRangeToUnit(CoreRager1) < 40) || (CoreRager2 && CoreRager2->isAlive() && GetRangeToUnit(CoreRager2) < 40))
			GolemaggTrust->mEnabled = true;
		else
			GolemaggTrust->mEnabled = false;

		if(GetPhase() == 1 && GetHealthPercent() <= 10)
			SetPhase(2);

		ParentClass::AIUpdate();
	}
	
	Unit* CoreRager1;
	Unit* CoreRager2;
	SpellDesc* GolemaggTrust;
};

/************************************************************************/
/* Majordomo and Adds													*/
/************************************************************************/

// Position for Domo after defeat
//X = 838.719971, Y = -830.810974, Z = -232.095001, O = 2.870590

/* Creature Data
Creature Name: Majordomo Executus
Creature Complete Percentage: 50%
Creature Comment: Teleport may need checking.
End Creature Data */

class MajordomoExecutusAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(MajordomoExecutusAI, MoonScriptBossAI);
	MajordomoExecutusAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(MAJORDOMO_AEGIS_OF_RAGNAROS, Target_Self, 100, 0, 30);
		AddSpell(MAJORDOMO_MAGIC_REFLECTION, Target_RandomFriendly, 100, 0, 15);
		AddSpell(MAJORDOMO_DAMAGE_SHIELD, Target_RandomFriendly, 100, 0, 25);
		AddSpell(MAJORDOMO_TELEPORT, Target_RandomPlayer, 100, 0, 20);
		AddSpell(MAJORDOMO_BLAST_WAVE, Target_Self, 100, 0, 10);
		
		Elite1 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(726.755005f, -1205.589966f, -119.176003f, 11664);
		Elite2 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(759.440979f, -1194.760010f, -118.794998f, 11664);
		Elite3 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(764.520996f, -1164.670044f, -119.707001f, 11664);
		Elite4 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(724.450012f, -1150.910034f, -120.114998f, 11664);
		Healer1 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(733.827026f, -1206.750000f, -118.829002f, 11663);
		Healer2 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(762.200989f, -1190.630005f, -118.984001f, 11663);
		Healer3 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(761.750977f, -1159.400024f, -119.906998f, 11663);
		Healer4 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(729.835999f, -1149.260010f, -120.081001f, 11663);
	}
	
	void OnSpawn()
	{
		Emote("The Runes of Warding have been destroyed! Hunt down the infidels, my brethren!", Text_Yell, 8039);
	}
	
	void OnCombatStart(Unit *pTarget)
	{
		Emote("Reckless mortals! None may challenge the Sons of the Living Flame!", Text_Yell, 8035);
		SetPhase(1);
		ParentClass::OnCombatStart(pTarget);
	}
	
	void AIUpdate()
	{
		if(!Elite1->isAlive() && !Elite2->isAlive() && !Elite3->isAlive() && !Elite4->isAlive() &&
		!Healer1->isAlive() && !Healer2->isAlive() && !Healer3->isAlive() && !Healer4->isAlive() && GetPhase() == 1)
		{
			SetCanEnterCombat(false);
			SetAllowMelee(false);
			SetAllowRanged(false);
			SetAllowSpell(false);
			SetAllowTargeting(false);
			SetCanMove(false);
			RemoveAllAuras();
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);		
			Emote("Impossible! Stay your attack, mortal... I submit! I submit!", Text_Yell, 8045);
			SetPhase(2);
			DomoSayTimer1 = AddTimer(7000);
			DomoSayTimer2 = AddTimer(16000);
			DomoTeleportTimer3 = AddTimer(34000);
		}
		
		if(GetPhase() == 2 && IsTimerFinished(DomoSayTimer1))
		{
			Emote("Brashly, you have come to rest the secrets of the Living Flame! You will soon regret the recklessness of your quest", Text_Yell, 0);
			RemoveTimer(DomoSayTimer1);
		}
		
		if(GetPhase() == 2 && IsTimerFinished(DomoSayTimer2))
		{
			Emote("I go now to summon the lord whose house this is. Should you seek an audience with him, your paltry lives will surely be forfeit! Nevertheless, seek out his lair, if you dare!", Text_Yell, 0);
			RemoveTimer(DomoSayTimer2);
		}
		
		if(GetPhase() == 2 && IsTimerFinished(DomoTeleportTimer3))
		{
			_unit->SetPosition(838.719971f, -830.810974f, -232.095001f, 2.870590f);
			RemoveTimer(DomoTeleportTimer3);
		}
		
			
		ParentClass::AIUpdate();
	}
	
	Creature* Elite1;
	Creature* Elite2;
	Creature* Elite3;
	Creature* Elite4;
	Creature* Healer1;
	Creature* Healer2;
	Creature* Healer3;
	Creature* Healer4;
	
	int32 DomoSayTimer1;
	int32 DomoSayTimer2;
	int32 DomoTeleportTimer3;
};

/* Creature Data
Creature Name: Ragnaros
Creature Complete Percentage: 50%
Creature Comment: Magma Blast and Melt Weapon will need some work.
End Creature Data */

class RagnarosAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(RagnarosAI, MoonScriptBossAI);
	RagnarosAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(RAGNAROS_WRATH_OF_RAGNAROS, Target_Self, 100, 0, 25);
		AddSpell(RAGNAROS_HAND_OF_RAGNAROS,  Target_Self, 100, 0, 25);
		AddSpell(RAGNAROS_ERUPTION, Target_Self, 100, 0, 15); 
		AddSpell(RAGNAROS_ELEMENTAL_FIRE, Target_Current, 100, 0, 3); 
		AddSpell(RAGNAROS_MAGMA_BLAST, Target_RandomPlayer, 0, 0, 2); 
		AddSpell(RAGNAROS_MELT_WEAPON, Target_Self, 100, 0, 30); 
	}
};

void SetupMoltenCore(ScriptMgr* pScriptMgr)
{
    pScriptMgr->register_creature_script(BOSS_LUCIFRON, 				&LucifronAI::Create);
    pScriptMgr->register_creature_script(CREATURE_FLAMEWAKER_PROT, 		&FlamewakerProtectorAI::Create);
	
    pScriptMgr->register_creature_script(BOSS_MAGMADAR, 				&MagmadarAI::Create);
	pScriptMgr->register_creature_script(CREATURE_ACHOUND, 				&AncientCoreHoundAI::Create);	
	pScriptMgr->register_creature_script(CREATURE_COREHOUND, 			&CoreHoundAI::Create);
	
    pScriptMgr->register_creature_script(BOSS_GEHENNAS, 				&GehennasAI::Create);
	pScriptMgr->register_creature_script(CREATURE_FLAMEWAKER, 			&FlamewakerAI::Create);
	
    pScriptMgr->register_creature_script(BOSS_GARR, 					&GarrAI::Create);
    pScriptMgr->register_creature_script(CREATURE_FIRESWORN, 			&FireswornAI::Create);
	
    pScriptMgr->register_creature_script(BOSS_GEDDON, 					&BaronGeddonAI::Create);
	
    pScriptMgr->register_creature_script(BOSS_SHAZZRAH, 				&ShazzrahAI::Create);
	
    pScriptMgr->register_creature_script(BOSS_GOLEMAGG, 				&GolemaggAI::Create);
    pScriptMgr->register_creature_script(CREATURE_CORERAGER, 			&CoreRagerAI::Create);
	
    pScriptMgr->register_creature_script(BOSS_SULFURON, 				&SulfuronAI::Create);
	pScriptMgr->register_creature_script(CREATURE_FLAMEWALKERPRIEST,	&FlamewakerPriestAI::Create);
	
	pScriptMgr->register_creature_script(BOSS_MAJORDOMO,				&MajordomoExecutusAI::Create);
    pScriptMgr->register_creature_script(BOSS_RAGNAROS, 				&RagnarosAI::Create);
};
