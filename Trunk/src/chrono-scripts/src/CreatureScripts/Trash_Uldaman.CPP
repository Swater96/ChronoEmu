/*
 * ChronoEmu Scripts - Instance - Uldaman.
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by Light.
 */

// TRASH.

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"

/* TODO:	CN_EARTHEN_CUSTODIAN
 */


//StoneVault Ambusher. 7175
#define CN_STONEVAULT_AMBUSHER 7175
class StonevaultAmbusherAI : public MoonScriptCreatureAI
{
	SpellDesc *Enrage;
	bool do_Enrage;

	MOONSCRIPT_FACTORY_FUNCTION(StonevaultAmbusherAI, MoonScriptCreatureAI);
	StonevaultAmbusherAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Backstab ''rank 5''
		AddSpell(8721, Target_Current, 25, 0, 0);
		
		do_Enrage = false;
		Enrage = AddSpell(8599, Target_Self, 0, 0, 0);
	}

	void AIUpdate() {
		if(!do_Enrage && (GetHealthPercent() < 30.0)) {
			do_Enrage = true;
			CastSpell(Enrage);
		}
		ParentClass::AIUpdate();
	}
};
 

//StoneVault oracle. 4852
#define CN_STONEVAULT_ORACLE 4852
class StonevaultOracleAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(StonevaultOracleAI, MoonScriptCreatureAI);
	StonevaultOracleAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Healing Ward
		AddSpell(5605, Target_Self, 25, 2, 5);
		// Healing Wave
		AddSpell(8005, Target_Self, 35, 3, 10);
		// Lava SPout Totem.
		AddSpell(8264, Target_Self, 25, 2, 5);
	    // Lightning Shield.
		AddSpell(945, Target_Self, 100, 0, 40);
	}

};

//StoneVault Pillager. 24830 
#define CN_STONEVAULT_PILLAGER 24830
class StonevaultPillagerAI : public MoonScriptCreatureAI
{	
	SpellDesc *Enrage;
	bool do_Enrage;

	MOONSCRIPT_FACTORY_FUNCTION(StonevaultPillagerAI, MoonScriptCreatureAI);
	StonevaultPillagerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		do_Enrage = false;
		Enrage = AddSpell(8599, Target_Self, 0, 0, 0);
	}
	
	void AIUpdate() {
		if(!do_Enrage && (GetHealthPercent() < 20.0)) {
			do_Enrage = true;
			CastSpell(Enrage);
		}
		ParentClass::AIUpdate();
	}
};


// Shrike Bat 4861
#define CN_SHRIKE_BAT 4861
class ShrikeBatAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(ShrikeBatAI, MoonScriptCreatureAI);
	ShrikeBatAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Sonic Burst , AOE 5sec Silence everyone.
		AddSpell(8281, Target_Self, 25, 0, 5);
	}
};


// StoneVault Cave Lurker. 4850
#define CN_STONEVAULT_CAVE_LURKER 4850
class StonevaultCaveLurkerAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(StonevaultCaveLurkerAI, MoonScriptCreatureAI);
	StonevaultCaveLurkerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Backstab ''rank5''?
		AddSpell(8721, Target_Current, 50, 0, 0);
	}
};


// Jadespine Basilisk 4863
#define CN_JADESPINE_BASILISK 4863
class JadespineBasiliskAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(JadespineBasiliskAI, MoonScriptCreatureAI);
	JadespineBasiliskAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Crystalline Slumber
		AddSpell(3636, Target_RandomPlayer, 25, 2, 10);
        // Reflection.
		AddSpell(9906, Target_Self, 50, 0, 5);
	}
};




//Venomlash Scorpid 7022
#define CN_VENOMLASH_SCORPID 7022
class VenomlashScorpidAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(VenomlashScorpidAI, MoonScriptCreatureAI);
	VenomlashScorpidAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Venom Sting
		AddSpell(8257, Target_Current, 50, 0, 1);
	}
};


// StoneVault Brawler 4855
#define CN_STONEVAULT_BRAWLER 4855
class StonevaultBrawlerAI : public MoonScriptCreatureAI
{
	SpellDesc *Enrage;
	bool do_Enrage;

	MOONSCRIPT_FACTORY_FUNCTION(StonevaultBrawlerAI, MoonScriptCreatureAI);
	StonevaultBrawlerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Trash
		AddSpell(3391, Target_Self, 25, 0, 0);
		
		do_Enrage = false;
		Enrage = AddSpell(8599, Target_Self, 0, 0, 0);
	}

	void AIUpdate() {
		if(!do_Enrage && (GetHealthPercent() < 30.0)) {
			do_Enrage = true;
			CastSpell(Enrage);
		}
		ParentClass::AIUpdate();
	}
};


//Earthen Custodian <-- i think this one is with the Last boss script , Ill lets you deal with it.
#define CN_EARTHEN_CUSTODIAN 4855
class EarthenCustodianAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EarthenCustodianAI, MoonScriptCreatureAI);
	EarthenCustodianAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// find it XD
//		AddSpell(ID, Target_Self, 25, 0, 0);
	}
};

//Earthen Guardian 7076 , Same thing i think this is with another boss.
#define CN_EARTHEN_GUARDIAN 7076
class EarthenGuardianAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EarthenGuardianAI, MoonScriptCreatureAI);
	EarthenGuardianAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
    }
};


//Earthen Hallshaper 7077 , Another one!

#define CN_EARTHEN_HALLSHAPER 7077
class EarthenHallshaperAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EarthenHallshaperAI, MoonScriptCreatureAI);
	EarthenHallshaperAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
	}
};


// Earthen Rocksmasher 7011

#define CN_EARTHEN_ROCKSMASHER 7011
class EarthenRocksmasherAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(EarthenRocksmasherAI, MoonScriptCreatureAI);
	EarthenRocksmasherAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Strike
		AddSpell(12057, Target_Current, 50, 0, 0);
		// Battle stance
		ApplyAura(7165);
	}
};


// Earth Sculptor 7012
#define CN_EARTHEN_SCULPTOR 7012
class EarthenSculptorAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(EarthenSculptorAI, MoonScriptCreatureAI);
	EarthenSculptorAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Fire Shield
		AddSpell(2602, Target_Self, 30, 1, 0 );
	    // Flame Buffet
		AddSpell(12057, Target_Current, 50, 2.2f, 0);
	}
};

// Earthen StoneBreaker 7396
#define CN_EARTHEN_STONEBREAKER 7396
class EarthenStonebreakerAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(EarthenStonebreakerAI, MoonScriptCreatureAI);
	EarthenStonebreakerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Strike
		AddSpell(12057, Target_Current, 50, 0, 0);
	    // Battle stance
		ApplyAura(7165);
	}
};


//Earthen Stonecarver  7397
#define CN_EARTHEN_STONECARVER 7397
class EarthenStonecarverAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(EarthenStonecarverAI, MoonScriptCreatureAI);
	EarthenStonecarverAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Fire Shield
		AddSpell(2602, Target_Self, 30, 1,0 );
	    // Flame Buffet
		AddSpell(12057, Target_Current, 50, 2.2f, 0);
	}
};

//Shadowforge Ambusher 7091
#define CN_SHADOWFORGE_AMBUSHER 7091
class ShadowforgeAmbusherAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(ShadowforgeAmbusherAI, MoonScriptCreatureAI);
	ShadowforgeAmbusherAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Disarm
		AddSpell(6713, Target_Current, 25, 0, 5);
	    // Shield Bash
		AddSpell(12057, Target_RandomPlayer, 25, 0, 0);
	    // Defensive stance
		ApplyAura(7164);
	}
};

// Shadowforge Archaeologist 4849
#define CN_SHADOWFORGE_ARCHEOLOGIST 4849
class ShadowforgeArcheologistAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(ShadowforgeArcheologistAI, MoonScriptCreatureAI);
	ShadowforgeArcheologistAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Disarm
		AddSpell(6713, Target_Current, 25, 0, 5);
	    // Shield Bash
		AddSpell(12057, Target_RandomPlayer, 25, 0, 0);
	    // Defensive stance
		ApplyAura(7164);
	}
};

// Shadowforge Darkcaster 4848
#define CN_SHADOWFORGE_DARKCASTER 4848
class ShadowforgeDarkcasterAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(ShadowforgeDarkcasterAI, MoonScriptCreatureAI);
	ShadowforgeDarkcasterAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Mana burn
		AddSpell(15800, Target_Current, 25, 3, 0);
	    // Shadow Bolt volley
		AddSpell(12057, Target_RandomPlayer, 50, 3, 0);
	}
};


//Shadowforge Geologist 7030
#define CN_SHADOWFORGE_GEOLOGIST 7030
class ShadowforgeGeologistAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(ShadowforgeGeologistAI, MoonScriptCreatureAI);
	 ShadowforgeGeologistAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Flame Lash
		AddSpell(3356, Target_Current, 30, 1, 0);
	    // Flame Spike
		AddSpell(8814, Target_RandomPlayer, 50, 3, 30);
	}
};

//shadowforge Relic Hunter 4847
#define CN_SHADOWFORGE_RELIC_HUNTER 4847
class ShadowForgeRelicHunterAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(ShadowForgeRelicHunterAI, MoonScriptCreatureAI);
	 ShadowForgeRelicHunterAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Heal
		AddSpell(6064, Target_Self, 35, 3, 5);
	    // Shadow Word Pain
		AddSpell(2767, Target_RandomPlayer, 50, 0, 0);
	    // Silence
		AddSpell(6726, Target_RandomPlayer, 25, 0, 0);
    
	}
};

// Shadowforge Sharpshooter 7290
#define CN_SHADOWFORGE_SHARPSHOOTER 7290
class ShadowForgeSharpshooterAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(ShadowForgeSharpshooterAI, MoonScriptCreatureAI);
	 ShadowForgeSharpshooterAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Piercing Shot
		AddSpell(6685, Target_Current, 35, 0, 30);
	    // Shoot
		AddSpell(2767, Target_Current, 70, 0, 0);
	}
};


//Stone Steward 4860
#define CN_STONE_STEWARD 4860
class StoneStewardAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(StoneStewardAI, MoonScriptCreatureAI);
	 StoneStewardAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Ground Tremor
		AddSpell(6524, Target_Current, 60, 0, 5);
	}
};

//stonevaultFlameweaver 7321
#define CN_STONEVAULT_FLAMEWEAVER 7321
class StonevaultFlameweaverAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(StonevaultFlameweaverAI, MoonScriptCreatureAI);
	StonevaultFlameweaverAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Fire Shield
		AddSpell(2602, Target_Self, 30, 1,0 );
	    // Immolate
		AddSpell(2941, Target_Current, 25, 2, 0);
	    // Inferno Shell
		AddSpell(77396, Target_Self, 25, 3, 0);
	}
};

//StonevaultGeomancer 4853
#define CN_STONEVAULT_GEOMANCER 4853
class StonevaultGeomancerAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(StonevaultGeomancerAI, MoonScriptCreatureAI);
	 StonevaultGeomancerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		//  Fireball
		AddSpell(69053, Target_Current, 70, 3, 0);
	    // Shoot
		AddSpell(10452, Target_Current, 30, 2.2f, 0);
	}
};

// StonevaultMauler 7320
#define CN_STONEVAULT_MAULER 7320
class StonevaultMaulerAI : public MoonScriptCreatureAI
{	
	SpellDesc *Enrage;
	bool do_Enrage;

	MOONSCRIPT_FACTORY_FUNCTION(StonevaultMaulerAI, MoonScriptCreatureAI);
	 StonevaultMaulerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		//  Trash
		AddSpell(3391, Target_Self, 30, 0, 0);

		do_Enrage = false;
		Enrage = AddSpell(8269, Target_Self, 0, 0, 0);
	}

	void AIUpdate() {
		if(!do_Enrage && (GetHealthPercent() < 30.0)) {
			do_Enrage = true;
			CastSpell(Enrage);
		}
		ParentClass::AIUpdate();
	}
};

void SetupUldamanTrash(ScriptMgr * mgr)
{
	//TRASH 
	mgr->register_creature_script(CN_STONEVAULT_AMBUSHER, &StonevaultAmbusherAI::Create);
	mgr->register_creature_script(CN_STONEVAULT_ORACLE, &StonevaultOracleAI::Create);
	mgr->register_creature_script(CN_STONEVAULT_PILLAGER, &StonevaultPillagerAI::Create);
	mgr->register_creature_script(CN_SHRIKE_BAT, &ShrikeBatAI::Create);
	mgr->register_creature_script(CN_STONEVAULT_CAVE_LURKER, &StonevaultCaveLurkerAI::Create);
	mgr->register_creature_script(CN_JADESPINE_BASILISK, &JadespineBasiliskAI::Create);
	mgr->register_creature_script(CN_VENOMLASH_SCORPID, &VenomlashScorpidAI::Create);
	mgr->register_creature_script(CN_STONEVAULT_BRAWLER, &StonevaultBrawlerAI::Create);
	mgr->register_creature_script(CN_EARTHEN_ROCKSMASHER, &EarthenRocksmasherAI::Create);
	mgr->register_creature_script(CN_EARTHEN_SCULPTOR, &EarthenSculptorAI::Create);
	mgr->register_creature_script(CN_EARTHEN_STONEBREAKER, &EarthenStonebreakerAI::Create);
	mgr->register_creature_script(CN_EARTHEN_STONECARVER, &EarthenStonecarverAI::Create);
	mgr->register_creature_script(CN_SHADOWFORGE_AMBUSHER, &ShadowforgeAmbusherAI::Create);
	mgr->register_creature_script(CN_SHADOWFORGE_ARCHEOLOGIST, &ShadowforgeArcheologistAI::Create);
	mgr->register_creature_script(CN_SHADOWFORGE_DARKCASTER, &ShadowforgeDarkcasterAI::Create);
	mgr->register_creature_script(CN_SHADOWFORGE_GEOLOGIST, &ShadowforgeGeologistAI::Create);
	mgr->register_creature_script(CN_SHADOWFORGE_RELIC_HUNTER, &ShadowForgeRelicHunterAI::Create);
	mgr->register_creature_script(CN_SHADOWFORGE_SHARPSHOOTER, &ShadowForgeSharpshooterAI::Create);
	mgr->register_creature_script(CN_STONE_STEWARD, &StoneStewardAI::Create);
	mgr->register_creature_script(CN_STONEVAULT_FLAMEWEAVER, &StonevaultFlameweaverAI::Create);
	mgr->register_creature_script(CN_STONEVAULT_GEOMANCER, &StonevaultGeomancerAI::Create);
	mgr->register_creature_script(CN_STONEVAULT_MAULER, &StonevaultMaulerAI::Create);
}