/*
 * ChronoEmu Scripts - Azshara Zone Mobs
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by Seloria
 */

#include "StdAfx.h"
#include "../Setup.h"
#include "../../Common/Base.h"

//Antilos
#define CN_ANTILOS 6648
class AntilosAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AntilosAI, MoonScriptCreatureAI);
	AntilosAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Cleave
		AddSpell(40505, Target_Current, 30, 0, 0);
		// Rend
		AddSpell(13445, Target_Current, 15, 0, 15);
		// Swoop
		AddSpell(5708, Target_Current, 30, 0, 5);
	}
};

//Arkkoran Oracle
#define CN_ARKKORAN_ORACLE 6138
class ArkkoranOracleAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ArkkoranOracleAI, MoonScriptCreatureAI);
	ArkkoranOracleAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Healing Wave
		AddSpell(11986, Target_Self, 30, 3, 0);
		// Lightning Shield
		ApplyAura(12550);
		// Shock
		AddSpell(11824, Target_Current, 20, 0, 6);
	}
};

//Arkkoran Pincer
#define CN_ARKKORAN_PINCER 6137
class ArkkoranPincerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ArkkoranPincerAI, MoonScriptCreatureAI);
	ArkkoranPincerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Rend
		AddSpell(13443, Target_Current, 30, 0, 15);
	}
};

//Avalanchion
//Elite
#define CN_AVALANCHION 14464
class AvalanchionAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AvalanchionAI, MoonScriptCreatureAI);
	AvalanchionAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Boulder
		AddSpell(23392, Target_Current, 30, 2, 0);
		// Ground Tremor
		AddSpell(6524, Target_Current, 20, 0, 5);
		// Trample
		AddSpell(5568, Target_Current, 30, 0, 0);
	}
};

//Blood Elf Reclaimer
//For heal and renew it should also cast on nearby friendly npc's that have low health
#define CN_BLOOD_ELF_RECLAIMER 6199
class BloodElfReclaimerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BloodElfReclaimerAI, MoonScriptCreatureAI);
	BloodElfReclaimerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Fireball
		AddSpell(20823, Target_Current, 40, 3, 0);
		// Heal
		AddSpell(11642, Target_Self, 25, 3.5, 0);
		// Renew
		AddSpell(11640, Target_Self, 30, 0, 15);
	}
};

//Blood Elf Surveyor
#define CN_BLOOD_ELF_SURVEYOR 6198
class BloodElfSurveyorAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BloodElfSurveyorAI, MoonScriptCreatureAI);
	BloodElfSurveyorAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Fire Nova
		AddSpell(11969, Target_Self, 30, 2, 0);
	}
};

//Blue Dragonspawn
//Elite
#define CN_BLUE_DRAGONSPAWN 193
class BlueDragonspawnAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BlueDragonspawnAI, MoonScriptCreatureAI);
	BlueDragonspawnAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Cleave
		AddSpell(15284, Target_Current, 50, 0, 0);
	}
};

//Blue Scalebane
//Elite
#define CN_BLUE_SCALEBANE 6130
class BlueScalebaneAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BlueScalebaneAI, MoonScriptCreatureAI);
	BlueScalebaneAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Knockdown
		AddSpell(16790, Target_Current, 20, 0, 10);
		// Mortal Cleave
		AddSpell(22859, Target_Current, 45, 0, 5);
	}
};

//Caravan Master Tset
#define CN_CARAVAN_MASTER_TSET 8409
class CaravanMasterTsetAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CaravanMasterTsetAI, MoonScriptCreatureAI);
	CaravanMasterTsetAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Frost Armor
		ApplyAura(12544);
		// Frost Nova
		AddSpell(11831, Target_Self, 30, 0, 10);
		// Frostbolt
		AddSpell(9672, Target_Current, 30, 3, 0);
	}
};

//Cliff Walker
//Elite
#define CN_CLIFF_WALKER 6148
class CliffWalkerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CliffWalkerAI, MoonScriptCreatureAI);
	CliffWalkerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// War Stomp
		AddSpell(11876, Target_Current, 30, 0, 0);
	}
};

//Cliff Thunderer
//Elite
#define CN_CLIFF_THUNDERER 6147
class CliffThundererAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CliffThundererAI, MoonScriptCreatureAI);
	CliffThundererAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Thunderclap
		AddSpell(8147, Target_Self, 30, 0, 0);
	}
};

//Draconic Magelord
#define CN_DRACONIC_MAGELORD 6129
class DraconicMagelordAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DraconicMagelordAI, MoonScriptCreatureAI);
	DraconicMagelordAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Frost Shock
		AddSpell(12548, Target_Current, 30, 0, 0);
		// Frostbolt
		AddSpell(9672, Target_Current, 30, 3, 0);
	}
};

// Draconic Mageweaver
#define CN_DRACONIC_MAGEWEAVER 6131
class DraconicMageweaverAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DraconicMageweaverAI, MoonScriptCreatureAI);
	DraconicMageweaverAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Cone of Cold
		AddSpell(12557, Target_Current, 20, 0, 0);
		// Frostbolt
		AddSpell(9672, Target_Current, 30, 3, 0);
	}
};

// Forest Ooze
#define CN_FOREST_OOZE 8766
class ForestOozeAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ForestOozeAI, MoonScriptCreatureAI);
	ForestOozeAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Diseased Slime
		AddSpell(6907, Target_Current, 30, 2, 0);
	}
};

// Gatekeeper Rageroar
#define CN_CAP 6651
class GatekeeperRageroarAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GatekeeperRageroarAI, MoonScriptCreatureAI);
	GatekeeperRageroarAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Entangling Roots
		AddSpell(12747, Target_Current, 20, 1.5, 10);
		// Strike
		AddSpell(13584, Target_Current, 30, 0, 0);
	}
};

// GeneralFangferror
#define CN_GENERAL_FANGFERROR 6650
class GeneralFangferrorAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GeneralFangferrorAI, MoonScriptCreatureAI);
	GeneralFangferrorAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Cleave
		AddSpell(15284, Target_Current, 30, 0, 0);
		// Knockdown
		AddSpell(18812, Target_Current, 20, 0, 5);
		// Strike
		AddSpell(12057, Target_Current, 50, 0, 0);
	}
};

//Great Wavethrasher
#define CN_GREAT_WAVETHRASHER 3391
class GreatWavethrasherAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GreatWavethrasherAI, MoonScriptCreatureAI);
	GreatWavethrasherAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Thrash
		AddSpell(3391, Target_Self, 40, 0, 0);
	}
};

//Haldarr Felsworn
#define CN_HALDARR_FELSWORN 6127
class HaldarrFelswornAI : public MoonScriptCreatureAI
{
SpellDesc *summon;
	MOONSCRIPT_FACTORY_FUNCTION(HaldarrFelswornAI, MoonScriptCreatureAI);
	HaldarrFelswornAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Shadow Bolt
		AddSpell(20825, Target_Current, 50, 3, 0);
		//Summon Imp
		summon = AddSpell(11939, Target_Self, 0, 0, 0);
  		CastSpell(summon);
	}
};

//Haldarr Trickster
#define CN_HALDARR_TRICKSTER 6126
class HaldarrTricksterAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HaldarrTricksterAI, MoonScriptCreatureAI);
	HaldarrTricksterAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Curse of Mending
		AddSpell(7098, Target_Current, 50, 2, 0);
	}
};

//Hetaera
#define CN_HETAERA 6140
class HetaeraAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HetaeraAI, MoonScriptCreatureAI);
	HetaeraAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Infected Bite
		AddSpell(7367, Target_Current, 65, 0, 0);
		//Thrash
		AddSpell(3391, Target_Self, 50, 0, 0);
	}
};

//Highborne Apparition
#define CN_HIGHBORNE_APPARITION 6116
class HighborneApparitionAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HighborneApparitionAI, MoonScriptCreatureAI);
	HighborneApparitionAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Fear
		AddSpell(12542, Target_Current, 40, 1.5, 5);
	}
};

//Highborne Lichling
#define CN_HIGHBORNE_LICHLING 6117
class HighborneLichlingAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HighborneLichlingAI, MoonScriptCreatureAI);
	HighborneLichlingAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Frost Armor
		ApplyAura(12544);
		//Frostbolt
		AddSpell(20822, Target_Current, 50, 3, 0);
	}
};

//Lady Sesspira
//Caster NPC, If it doesn't cast Explosive Shot or Forked Lightning, it is spaming Shoot
#define CN_LADY_SESSPIRA 6649
class LadySesspiraAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LadySesspiraAI, MoonScriptCreatureAI);
	LadySesspiraAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Shoot
		AddSpell(15547, Target_Current, 95, 0, 0);
		//Forked Lightning
		AddSpell(20299, Target_Current, 30, 2, 5);
		//Explosive Shot
		AddSpell(15495, Target_Current, 40, 1, 5);
	}
};

//Legashi Hellcaller
#define CN_LEGASHI_HELLCALLER 6202
class LegashiHellcallerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LegashiHellcallerAI, MoonScriptCreatureAI);
	LegashiHellcallerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		// Fireball
		AddSpell(20823, Target_Current, 95, 3, 0);
		//Rain of Fire
		AddSpell(11990, Target_RandomDestination, 30, 0, 0);
	}
};

//Legashi Rogue
#define CN_LEGASHI_ROGUE 6201
class LegashiRogueAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LegashiRogueAI, MoonScriptCreatureAI);
	LegashiRogueAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Backstab
		AddSpell(7159, Target_Current, 50, 0, 0);
		//Gouge
		AddSpell(12540, Target_Current, 20, 0, 0);
	}
};

//Legashi Satyr
#define CN_LEGASHI_SATYR 6200
class LegashiSatyrAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LegashiSatyrAI, MoonScriptCreatureAI);
	LegashiSatyrAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Mana Burn
		AddSpell(11981, Target_Current, 60, 3, 0);
	}
};

//Lindros
#define CN_LINDROS 8381
class LindrosAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LindrosAI, MoonScriptCreatureAI);
	LindrosAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Arcane Explosion
		AddSpell(11975, Target_Self, 50, 1.5, 0);
		//Fireball
		AddSpell(20823, Target_Current, 95, 3, 0);
	}
};

//Lingering Highborne
#define CN_LINGERING_HIGHBORNE 7864
class LingeringHighborneAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LingeringHighborneAI, MoonScriptCreatureAI);
	LingeringHighborneAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Enfeeble
		AddSpell(11963, Target_Current, 80, 2, 30);
	}
};

//Magister Hawkhelm
#define CN_MAGISTER_HAWKHELM 6647
class MagisterHawkhelmAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MagisterHawkhelmAI, MoonScriptCreatureAI);
	MagisterHawkhelmAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Multi-Shot
		AddSpell(18651, Target_Current, 50, 0, 0);
		//Shoot
		AddSpell(6660, Target_Current, 95, 0, 0);
	}
};

//Magus Rimtori
#define CN_MAGUS_RIMTORI 8575
class MagusRimtoriAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MagusRimtoriAI, MoonScriptCreatureAI);
	MagusRimtoriAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Fireball
		AddSpell(20823, Target_Current, 95, 3, 0);
		//Frost Nova
		AddSpell(11831, Target_Self, 30, 0, 0);
	}
};

//Makrinni Razorclaw
#define CN_MAKRINNI_RAZORCLAW 6350
class MakrinniRazorclawAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MakrinniRazorclawAI, MoonScriptCreatureAI);
	MakrinniRazorclawAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Sunder Armor
		AddSpell(13444, Target_Current, 50, 0, 0);
	}
};

//Makrinni Scrabbler
#define CN_MAKRINNI_SCRABBLER 6370
class MakrinniScrabblerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MakrinniScrabblerAI, MoonScriptCreatureAI);
	MakrinniScrabblerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Frostbolt
		AddSpell(20822, Target_Current, 95, 3, 0);
		//Heal
		AddSpell(11642, Target_Self, 20, 3.5, 0);
	}
};

//Makrinni Snapclaw
#define CN_MAKRINNI_SNAPCLAW 6372
class MakrinniSnapclawAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MakrinniSnapclawAI, MoonScriptCreatureAI);
	MakrinniSnapclawAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Tendon Rip
		AddSpell(3604, Target_Current, 50, 0, 0);
	}
};

//Master Feardred
#define CN_MASTER_FEARDRED 6652
class MasterFeardredAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MasterFeardredAI, MoonScriptCreatureAI);
	MasterFeardredAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Arcane Explosion
		AddSpell(21073, Target_Self, 40, 1.5, 0);
	}
};

//Maws
#define CN_MAWS 15571
class MawsAI : public MoonScriptCreatureAI
{
SpellDesc *enrage;
bool do_enrage;
	MOONSCRIPT_FACTORY_FUNCTION(MawsAI, MoonScriptCreatureAI);
	MawsAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//enrage
		do_enrage = false;
  		enrage = AddSpell(19812, Target_Self, 0, 0, 0);
		//Rampage Intercept
		AddSpell(25744, Target_RandomPlayer, 20, 0, 0);
		//Dark Water
		AddSpell(25743, Target_RandomDestination, 30, 0, 0);
	}
	void AIUpdate() {
		if(!do_enrage && (GetHealthPercent() < 20.0)) {
			do_enrage = true;
			CastSpell(enrage);
		}
		ParentClass::AIUpdate();
	}
};

//Mistwing Ravager
#define CN_MISTWING_RAVAGER 8764
class MistwingRavagerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MistwingRavagerAI, MoonScriptCreatureAI);
	MistwingRavagerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Poisoned Shot
		AddSpell(8806, Target_Current, 30, 2, 0);
	}
};

//Mistwing Rogue
#define CN_MISTWING_ROGUE 8763
class MistwingRogueAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MistwingRogueAI, MoonScriptCreatureAI);
	MistwingRogueAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Lightning Bolt
		AddSpell(9532, Target_Current, 40, 3, 0);
	}
};

//Monnos The Elder
//Rare Elite
#define CN_MONNOS_THE_ELDER 6646
class MonnosTheElderAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MonnosTheElderAI, MoonScriptCreatureAI);
	MonnosTheElderAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Knock Away
		AddSpell(10101, Target_Current, 30, 0, 0);
		//Trample
		AddSpell(5568, Target_Current, 50, 0, 5);
		//War Stomp
		AddSpell(11876, Target_Self, 50, 0, 20);
	}
};

//MosshoofCourser
#define CN_MOSSHOOF_COURSER 8761
class MosshoofCourserAI : public MoonScriptCreatureAI
{
SpellDesc *enrage;
bool do_enrage;
	MOONSCRIPT_FACTORY_FUNCTION(MosshoofCourserAI, MoonScriptCreatureAI);
	MosshoofCourserAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//enrage
		do_enrage = false;
  		enrage = AddSpell(8599, Target_Self, 0, 0, 0);
	}
	void AIUpdate() {
		if(!do_enrage && (GetHealthPercent() < 30.0)) {
			do_enrage = true;
			CastSpell(enrage);
		}
		ParentClass::AIUpdate();
	}
};

//Patrick Mills
#define CN_PATRICK_MILLS 8382
class PatrickMillsAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PatrickMillsAI, MoonScriptCreatureAI);
	PatrickMillsAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Devotion Aura
		ApplyAura(1032);
		//Holy Strike
		AddSpell(17143, Target_Current, 50, 0, 0);
	}
};

//Servant of Arkkoroc
#define CN_SERVANT_OF_ARKKOROC 6143
class ServantofArkkorocAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ServantofArkkorocAI, MoonScriptCreatureAI);
	ServantofArkkorocAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Trample
		AddSpell(5568, Target_Current, 35, 0, 0);
	}
};

//Spitelash Enchantress
#define CN_SPITELASH_ENCHANTRESS 7886
class SpitelashEnchantressAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpitelashEnchantressAI, MoonScriptCreatureAI);
	SpitelashEnchantressAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Enchanted Quickness
		AddSpell(3443, Target_Self, 25, 3, 15);
		//Frost Shock
		AddSpell(12548, Target_Current, 40, 0, 0);
	}
};

//Spitelash Myrmidon
#define CN_SPITELAST_MYRMIDON 6196
class SpitelashMyrmidonAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpitelashMyrmidonAI, MoonScriptCreatureAI);
	SpitelashMyrmidonAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Strike
		AddSpell(11976, Target_Current, 50, 0, 0);
	}
};

//Spitelash Raider
#define CN_SPITELASH_RAIDER 12204
class SpitelashRaiderAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpitelashRaiderAI, MoonScriptCreatureAI);
	SpitelashRaiderAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Frost Shock
		AddSpell(12548, Target_Current, 40, 0, 0);
	}
};

//Spitelash Screamer
#define CN_SPITELASH_SCREAMER 6193
class SpitelashScreamerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpitelashScreamerAI, MoonScriptCreatureAI);
	SpitelashScreamerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Deafening Screech
		AddSpell(3589, Target_Self, 25, 0, 0);
		//Polymorph Backfire
		AddSpell(28406, Target_Self, 95, 0, 10);
	}
};

//Spitelash Serpent Guard
#define CN_SPITELASH_SERPENT_GUARD 6194
class SpitelashSerpentGuardAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpitelashSerpentGuardAI, MoonScriptCreatureAI);
	SpitelashSerpentGuardAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Frost Shock
		AddSpell(12548, Target_Current, 40, 0, 0);
		//Polymorph Backfire
		AddSpell(28406, Target_Self, 95, 0, 10);
	}
};

//Spitelash Warrior
#define CN_SPITELASH_WARRIOR 6190
class SpitelashWarriorAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpitelashWarriorAI, MoonScriptCreatureAI);
	SpitelashWarriorAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Polymorph Backfire
		AddSpell(28406, Target_Self, 95, 0, 10);
		//Disarm
		AddSpell(6713, Target_Current, 50, 0, 0);
	}
};

//Spitelash Siren
#define CN_SPITELASH_SIREN 6195
class SpitelashSirenAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpitelashSirenAI, MoonScriptCreatureAI);
	SpitelashSirenAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{
		//Frost Nova
		AddSpell(11831, Target_Self, 25, 0, 0);
		//Frost Shot
		AddSpell(12551, Target_Current, 30, 0, 0);
		//Renew
		AddSpell(11640, Target_Self, 10, 0, 0);
		//Shoot
		AddSpell(6660, Target_Current, 99, 0, 0);
		//Polymorph Backfire
		AddSpell(28406, Target_Self, 95, 0, 10);
	}
};

void SetupAzsharaNPC(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_ANTILOS, &AntilosAI::Create);
	mgr->register_creature_script(CN_ARKKORAN_ORACLE, &ArkkoranOracleAI::Create);
	mgr->register_creature_script(CN_ARKKORAN_PINCER, &ArkkoranPincerAI::Create);
	mgr->register_creature_script(CN_AVALANCHION, &AvalanchionAI::Create);
	mgr->register_creature_script(CN_BLOOD_ELF_RECLAIMER, &BloodElfReclaimerAI::Create); 
	mgr->register_creature_script(CN_BLOOD_ELF_SURVEYOR, &BloodElfSurveyorAI::Create); 
	mgr->register_creature_script(CN_BLUE_DRAGONSPAWN, &BlueDragonspawnAI::Create);
	mgr->register_creature_script(CN_BLUE_SCALEBANE, &BlueScalebaneAI::Create);
	mgr->register_creature_script(CN_CARAVAN_MASTER_TSET, &CaravanMasterTsetAI::Create);
	mgr->register_creature_script(CN_CLIFF_WALKER, &CliffWalkerAI::Create);
	mgr->register_creature_script(CN_CLIFF_THUNDERER, &CliffThundererAI::Create);
	mgr->register_creature_script(CN_DRACONIC_MAGELORD, &DraconicMagelordAI::Create);
	mgr->register_creature_script(CN_DRACONIC_MAGEWEAVER, &DraconicMageweaverAI::Create);
	mgr->register_creature_script(CN_FOREST_OOZE, &ForestOozeAI::Create);
	mgr->register_creature_script(CN_CAP, &GatekeeperRageroarAI::Create);
	mgr->register_creature_script(CN_GENERAL_FANGFERROR, &GeneralFangferrorAI::Create);
	mgr->register_creature_script(CN_GREAT_WAVETHRASHER, &GreatWavethrasherAI::Create);
	mgr->register_creature_script(CN_HALDARR_FELSWORN, &HaldarrFelswornAI::Create);
	mgr->register_creature_script(CN_HALDARR_TRICKSTER, &HaldarrTricksterAI::Create);
	mgr->register_creature_script(CN_HETAERA, &HetaeraAI::Create);
	mgr->register_creature_script(CN_HIGHBORNE_APPARITION, &HighborneApparitionAI::Create);
	mgr->register_creature_script(CN_HIGHBORNE_LICHLING, &HighborneLichlingAI::Create);
	mgr->register_creature_script(CN_LADY_SESSPIRA, &LadySesspiraAI::Create);
	mgr->register_creature_script(CN_LEGASHI_HELLCALLER, &LegashiHellcallerAI::Create);
	mgr->register_creature_script(CN_LEGASHI_ROGUE, &LegashiRogueAI::Create);
	mgr->register_creature_script(CN_LEGASHI_SATYR, &LegashiSatyrAI::Create);
	mgr->register_creature_script(CN_LINDROS, &LindrosAI::Create);
	mgr->register_creature_script(CN_LINGERING_HIGHBORNE, &LingeringHighborneAI::Create);
	mgr->register_creature_script(CN_MAGISTER_HAWKHELM, &MagisterHawkhelmAI::Create);
	mgr->register_creature_script(CN_MAGUS_RIMTORI, &MagusRimtoriAI::Create);
	mgr->register_creature_script(CN_MAKRINNI_RAZORCLAW, &MakrinniRazorclawAI::Create);
	mgr->register_creature_script(CN_MAKRINNI_SCRABBLER, &MakrinniScrabblerAI::Create);
	mgr->register_creature_script(CN_MAKRINNI_SNAPCLAW, &MakrinniSnapclawAI::Create);
	mgr->register_creature_script(CN_MASTER_FEARDRED, &MasterFeardredAI::Create);
	mgr->register_creature_script(CN_MAWS, &MawsAI::Create);
	mgr->register_creature_script(CN_MISTWING_RAVAGER, &MistwingRavagerAI::Create);
	mgr->register_creature_script(CN_MISTWING_ROGUE, &MistwingRogueAI::Create);
	mgr->register_creature_script(CN_MONNOS_THE_ELDER, &MonnosTheElderAI::Create);
	mgr->register_creature_script(CN_MOSSHOOF_COURSER, &MosshoofCourserAI::Create);
	mgr->register_creature_script(CN_PATRICK_MILLS, &PatrickMillsAI::Create);
	mgr->register_creature_script(CN_SERVANT_OF_ARKKOROC, &ServantofArkkorocAI::Create);
	mgr->register_creature_script(CN_SPITELASH_ENCHANTRESS, &SpitelashEnchantressAI::Create);
	mgr->register_creature_script(CN_SPITELASH_RAIDER, &SpitelashRaiderAI::Create);
	mgr->register_creature_script(CN_SPITELAST_MYRMIDON, &SpitelashMyrmidonAI::Create);
	mgr->register_creature_script(CN_SPITELASH_SCREAMER, &SpitelashScreamerAI::Create);
	mgr->register_creature_script(CN_SPITELASH_SERPENT_GUARD, &SpitelashSerpentGuardAI::Create);
	mgr->register_creature_script(CN_SPITELASH_WARRIOR, &SpitelashWarriorAI::Create);
	mgr->register_creature_script(CN_SPITELASH_SIREN, &SpitelashSirenAI::Create);
}
//End of Azshara NPC Mobs
//Seloria is God

