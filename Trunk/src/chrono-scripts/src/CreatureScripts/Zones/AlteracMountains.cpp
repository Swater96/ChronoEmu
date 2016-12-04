/*
 * ChronoEmu Scripts - Alterac Mountains Mobs
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by Seloria
 */

#include "StdAfx.h"
#include "../Setup.h"
#include "../../Common/Base.h"

/* TODO:	war quest support
			Elemental slave
			*/

//Archmage Ansirem Runeweaver
#define CN_ARCHMAGE_ANSIREM_RUNEWEAVER 2543
class ArchmageAnsiremRuneweaverAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ArchmageAnsiremRuneweaverAI, MoonScriptCreatureAI);
	ArchmageAnsiremRuneweaverAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Chains of Ice
		AddSpell(512, Target_Current, 15, 1.3, 0);
		//Frostbolt
		AddSpell(20792, Target_Current, 95, 3, 0);
		//Polymorph
		AddSpell(118, Target_RandomPlayer, 20, 1.5, 0);
		//Mage Sight
		//NPC casts this spell on quest turn in. Quest is http://www.wowhead.com/quest=602
		AddSpell(3659, Target_Current, 0, 0, 0);
	}  
};

//Bath'rah the Windwatcher
#define CN_BATH_RAH_THE_WINDWATCHER 6176
class BathrahtheWindwatcherAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BathrahtheWindwatcherAI, MoonScriptCreatureAI);
	BathrahtheWindwatcherAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Spirit Totem
//		AddSpell(25000);
		//Summon Cyclonian
//		AddSpell(8606);
	}  
};

//Argus Shadow Mage
#define CN_ARGUS_SHADOW_MAGE 2318
class ArgusShadowMageAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ArgusShadowMageAI, MoonScriptCreatureAI);
	ArgusShadowMageAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Shadow Bolt
		AddSpell(20816, Target_Current, 100, 3, 0);
	}  
};

//Baron Vardus
#define CN_BARON_VARDUS 2306
class BaronVardusAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BaronVardusAI, MoonScriptCreatureAI);
	BaronVardusAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Backhand
		AddSpell(6253, Target_Current, 20, 0, 0);
		//Frost Nova
		AddSpell(11831, Target_Self, 30, 0, 0);
		//Frostbolt
		AddSpell(20822, Target_Current, 90, 0, 0);
	}  
};

//Crushridge Enforcer
#define CN_CRUSHRIDGE_ENFORCER 2256
class CrushridgeEnforcerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CrushridgeEnforcerAI, MoonScriptCreatureAI);
	CrushridgeEnforcerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Head Crack
		AddSpell(9791, Target_Current, 40, 0, 0);
		//Intimidation
		AddSpell(7093, Target_RandomPlayer, 30, 0, 0);
	}  
};

//Crushridge Mage
#define CN_CRUSHRIDGE_MAGE 2255
class CrushridgeMageAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CrushridgeMageAI, MoonScriptCreatureAI);
	CrushridgeMageAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Frostbolt
		AddSpell(9672, Target_Current, 95, 3, 0);
		//Bloodlust
		AddSpell(6742, Target_Self, 20, 0, 30);
	}  
};

//Crushridge Mauler
#define CN_CRUSHRIDGE_MAULER 2254
class CrushridgeMaulerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CrushridgeMaulerAI, MoonScriptCreatureAI);
	CrushridgeMaulerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Backhand
		AddSpell(6253, Target_Current, 20, 0, 0);
		//Strike
		AddSpell(11976, Target_Current, 60, 0, 0);
	}  
};

//Crushridge Ogre
#define CN_CRUSHRIDGE_OGRE 2252
class CrushridgeOgreAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CrushridgeOgreAI, MoonScriptCreatureAI);
	CrushridgeOgreAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Strike
		AddSpell(11976, Target_Current, 60, 0, 0);
	}  
};

//Crushridge Plunderer
#define CN_CRUSHRIDGE_PLUNDERER 2416
class CrushridgePlundererAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CrushridgePlundererAI, MoonScriptCreatureAI);
	CrushridgePlundererAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Bomb
		AddSpell(9143, Target_RandomDestination, 30, 2, 0);
		//Cleave
		AddSpell(15496, Target_Current, 60, 0, 0);
	}  
};

//Crushridge Warmonger
#define CN_CRUSHRIDGE_WARMONGER 2287
class CrushridgeWarmongerAI : public MoonScriptCreatureAI
{
	SpellDesc *Frenzy;
	bool do_frenzy;
	MOONSCRIPT_FACTORY_FUNCTION(CrushridgeWarmongerAI, MoonScriptCreatureAI);
	CrushridgeWarmongerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		do_frenzy = false;
		Frenzy = AddSpell(8269, Target_Self, 0, 0, 0);
	}

	void AIUpdate() {
		if(!do_frenzy && (GetHealthPercent() < 30.0)) {
			do_frenzy = true;
			CastSpell(Frenzy);
		}
		ParentClass::AIUpdate();
	}
};

//Cyclonian
#define CN_CYCLONIAN 6239
class CyclonianAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CyclonianAI, MoonScriptCreatureAI);
	CyclonianAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Gust of Wind
		AddSpell(6982, Target_Self, 30, 0, 0);
		//Knock Away
		AddSpell(18670, Target_Current, 30, 0, 0);
		//Whirlwind
		AddSpell(17207, Target_Self, 90, 0, 30);
	}  
};

//Dalaran Summoner
#define CN_DALARAN_SUMMONER 2358
class DalaranSummonerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DalaranSummonerAI, MoonScriptCreatureAI);
	DalaranSummonerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Fireball
		AddSpell(20815, Target_Current, 99, 3, 0);
		//Summon Shield Guard
		AddSpell(3655, Target_Self, 15, 5, 0);
		//Summon Theurgist
		AddSpell(3658, Target_Self, 15, 5, 0);
	}  
};

//Dalaran Theurgist
#define CN_DALARAN_THEURGIST 2272
class DalaranTheurgistAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DalaranTheurgistAI, MoonScriptCreatureAI);
	DalaranTheurgistAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Frost Ward
		AddSpell(15044, Target_Self, 45, 0, 30);
		//Frostbolt
		AddSpell(20819, Target_Current, 99, 3, 0);
		//Summon Spirit of Old
		AddSpell(3652, Target_Self, 15, 4, 0);
	}  
};

//Drunken Footpad
#define CN_DRUNKEN_FOOTPAD 2440
class DrunkenFootpadAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DrunkenFootpadAI, MoonScriptCreatureAI);
	DrunkenFootpadAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Strike
		AddSpell(13584, Target_Current, 50, 0, 0);
	}  
};

//Elemental Slave   http://www.wowhead.com/npc=2359
//This mob is a bit of a challenge. He crumbles as he takes damage and is immune to nature
//He crumbles down at 75%, 50% and then at 25%
//Spell IDs for the crumbles are 3671, 3672, 3673
// He also makes and emote each time he crumbles. check comments on Wowhead
#define CN_ELEMENTAL_SLAVE 2359	
class ElementalSlaveAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ElementalSlaveAI, MoonScriptCreatureAI);
	ElementalSlaveAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		
	}  
};

//Giant Moss Creeper
#define CN_GIANT_MOSS_CREEPER 2349
class GiantMossCreeperAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GiantMossCreeperAI, MoonScriptCreatureAI);
	GiantMossCreeperAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Corrosive Poison
		AddSpell(3396, Target_Current, 30, 1.5, 0);
	}  
};


//Giant Yeti
#define CN_GIANT_YETI 2254
class GiantYetiAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GiantYetiAI, MoonScriptCreatureAI);
	GiantYetiAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Frost Breath
		AddSpell(3131, Target_Current, 50, .25, 10);
	}  
};


//Glommus
#define CN_GLOMMUS 2422
class GlommusAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GlommusAI, MoonScriptCreatureAI);
	GlommusAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Knockdown
		AddSpell(11428, Target_Current, 25, 0, 0);
		//Demoralizing Shout
		AddSpell(13730, Target_Self, 50, 0, 0);
		//Battle Shout
		AddSpell(9128, Target_Self, 95, 0, 90);
	}  
};


//Grey Bear
#define CN_GREY_BEAR 2351
class GreyBearAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GreyBearAI, MoonScriptCreatureAI);
	GreyBearAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Claw Rank 3
		AddSpell(16829, Target_Current, 35, 0, 0);
		//Claw Rank 4
		AddSpell(16830, Target_Current, 25, 0, 0);
		//Growl
		AddSpell(14917, Target_Current, 10, 0, 5);
		//Swipe
		AddSpell(53526, Target_Current, 30, 0, 0);
	}  
};


//Grel'borg the Miser
//Elite
#define CN_GREL_BORG_THE_MISER 2417
class GrelborgtheMiserAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GrelborgtheMiserAI, MoonScriptCreatureAI);
	GrelborgtheMiserAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Bloodlust
		AddSpell(6742, Target_Self, 10, 0, 0);
		//Flame Ward
		AddSpell(4979, Target_Self, 30, 0, 15);
		//Raid of Fire
		AddSpell(11990, Target_RandomDestination, 20, 0, 0);
	}  
};


//Jailor Borhuin
#define CN_JAILOR_BORHUIN 2431
class JailorBorhuinAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(JailorBorhuinAI, MoonScriptCreatureAI);
	JailorBorhuinAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Disarm
		AddSpell(8379, Target_Current, 10, 0, 0);
		//Net
		AddSpell(6533, Target_RandomPlayer, 15, 0, 0);
		//Pummel
		AddSpell(12555, Target_Current, 25, 0, 0);
	}  
};

//Jekyll Flandring 
#define CN_JEKYLL_FLANDRING 13219
class JekyllFlandringAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(JekyllFlandringAI, MoonScriptCreatureAI);
	JekyllFlandringAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Cleave
		AddSpell(19642, Target_Current, 30, 0, 0);
		//Strike
		AddSpell(19644, Target_Current, 50, 0, 0);
	}  
};


//Lieutenant Haggerdin
#define CN_LIEUTENANT_HAGGERDIN 13841
class LieutenantHaggerdinAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LieutenantHaggerdinAI, MoonScriptCreatureAI);
	LieutenantHaggerdinAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Revenge
		AddSpell(19130, Target_Current, 30, 0, 0);
		//Shield Block
		AddSpell(12169, Target_Self, 15, 0, 0);
	}  
};


//Lo'Grosh
#define CN_LO_GROSH 2453
class LoGroshAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LoGroshAI, MoonScriptCreatureAI);
	LoGroshAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Fire Shield III
		AddSpell(2601, Target_Self, 20, 0, 15);
		//Bloodlust
		AddSpell(6742, Target_Self, 10, 0, 0);
		//Flame Spike
		AddSpell(8814, Target_RandomDestination, 20, 0, 0);
	}  
};


//Lord Aliden Perenolde
#define CN_LORD_ALIDEN_PERENOLDE 2423
class LordAlidenPerenoldeAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LordAlidenPerenoldeAI, MoonScriptCreatureAI);
	LordAlidenPerenoldeAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Power Word: Shield
		AddSpell(11974, Target_Self, 20, 0, 0);
		//Renew
		AddSpell(8362, Target_Self, 20, 0, 0);
		//Sleep
		AddSpell(15970, Target_RandomPlayerNotCurrent, 75, 1, 0);
	}  
};


//Mountain Yeti
#define CN_MOUNTAIN_YETI 2250
class MountainYetiAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MountainYetiAI, MoonScriptCreatureAI);
	MountainYetiAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Frost Breath
		AddSpell(3131, Target_Current, 50, .25, 10);
	}  
};


//Muckrake
#define CN_MUCKRAKE 2421
class MuckrakeAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MuckrakeAI, MoonScriptCreatureAI);
	MuckrakeAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Disarm
		AddSpell(8379, Target_Current, 20, 0, 0);
		//Pummel
		AddSpell(12555, Target_Current, 30, 0, 0);
	}  
};


//Mug'thol
#define CN_MUG_THOL 2257
class MugTholAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MugTholAI, MoonScriptCreatureAI);
	MugTholAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Strike
		AddSpell(19644, Target_Current, 50, 0, 0);
		//Demoralizing Shout
		AddSpell(13730, Target_Self, 50, 0, 0);
	}  
};


//Narillasanz
#define CN_NARILLASANZ 2447
class NarillasanzAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(NarillasanzAI, MoonScriptCreatureAI);
	NarillasanzAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Renew
		AddSpell(8362, Target_Self, 20, 2, 0);
		//Flame Breath
		AddSpell(9573, Target_Current, 50, 2, 0);
		//Fireball
		AddSpell(20815, Target_Current, 20, 0, 0);
		//Flame Spike
		AddSpell(8814, Target_RandomDestination, 20, 0, 0);
	}  
};

//Skhowl
#define CN_SKHOWL 2452
class SkhowlAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SkhowlAI, MoonScriptCreatureAI);
	SkhowlAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Backhand
		AddSpell(6253, Target_Current, 20, 0, 0);
		//Demoralizing Roar
		AddSpell(15971, Target_Self, 30, 0, 0);
		//Intimidating Roar
		AddSpell(16508, Target_Current, 15, 0, 0);
	}  
};

//Stone Fury
#define CN_STONE_FURY 2258
class StoneFuryAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(StoneFuryAI, MoonScriptCreatureAI);
	StoneFuryAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Ground Tremor
		AddSpell(6524, Target_Self, 20, 0, 0);
		//Trample
		AddSpell(5568, Target_Self, 40, 0, 0);
	}  
};

//Syndicate Assassin
#define CN_SYNDICATE_ASSASSIN 2246
class SyndicateAssassinAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SyndicateAssassinAI, MoonScriptCreatureAI);
	SyndicateAssassinAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Poison
		AddSpell(744, Target_Current, 30, 0, 0);
	}  
};

//Syndicate Enforcer
#define CN_SYNDICATE_ENFORCER 2247
class SyndicateEnforcerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SyndicateEnforcerAI, MoonScriptCreatureAI);
	SyndicateEnforcerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Whirlwind
		AddSpell(17207, Target_Self, 30, 0, 0);
	}  
};

//Syndicate Footpad
#define CN_SYNDICATE_FOOTPAD 2240
class SyndicateFootpadAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SyndicateFootpadAI, MoonScriptCreatureAI);
	SyndicateFootpadAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Backstab
		AddSpell(7159, Target_Current, 40, 0, 0);
	}  
};

//SyndicateSaboteur
#define CN_SYNDICATE_SABOTEUR 2245
class SyndicateSaboteurAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SyndicateSaboteurAI, MoonScriptCreatureAI);
	SyndicateSaboteurAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Shoot
		AddSpell(6660, Target_Current, 99, 0, 0);
		//Fire Shot
		AddSpell(6980, Target_Current, 30, 0, 0);
	}  
};

//SyndicateSentry
#define CN_SYNDICATE_SENTRY 2243
class SyndicateSentryAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SyndicateSentryAI, MoonScriptCreatureAI);
	SyndicateSentryAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Shield Bash
		AddSpell(11972, Target_Current, 30, 0, 0);
		//Shield Wall
		AddSpell(15062, Target_Self, 15, 0, 0);
	}  
};

//SyndicateSpy
#define CN_SYNDICATE_SPY 2242
class SyndicateSpyAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SyndicateSpyAI, MoonScriptCreatureAI);
	SyndicateSpyAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Backstab
		AddSpell(7159, Target_Current, 40, 0, 0);
	}  
};

//SyndicateThief
#define CN_SYNDICATE_THIEF 2241
class SyndicateThiefAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SyndicateThiefAI, MoonScriptCreatureAI);
	SyndicateThiefAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Backstab
		AddSpell(7159, Target_Current, 40, 0, 0);		
		//Disarm
		AddSpell(6713, Target_Current, 15, 0, 0);
		//Poison
		AddSpell(744, Target_Current, 30, 0, 0);
	}  
};

//SyndicateWizard
#define CN_SYNDICATE_WIZARD 2319
class SyndicateWizardAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SyndicateWizardAI, MoonScriptCreatureAI);
	SyndicateWizardAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Fireball
		AddSpell(20815, Target_Current, 100, 3, 0);
		//Frost Armor
		ApplyAura(12544);
		//Polymorph
		AddSpell(13323, Target_RandomPlayer, 20, 1.5, 0);
	}  
};

//Targ
#define CN_TARG 2420
class TargAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TargAI, MoonScriptCreatureAI);
	TargAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Cleave
		AddSpell(19642, Target_Current, 30, 0, 0);
		//Thunderclap
		AddSpell(8147, Target_Self, 30, 0, 0);
	}  
};

//ThanthaldisSnowgleam
#define CN_THANTHALDIS_SNOWGLEAM 13217
class ThanthaldisSnowgleamAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ThanthaldisSnowgleamAI, MoonScriptCreatureAI);
	ThanthaldisSnowgleamAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Cleave
		AddSpell(19642, Target_Current, 30, 0, 0);
		//Strike
		AddSpell(19644, Target_Current, 50, 0, 0);
	}  
};

//VileFinTidecaller
#define CN_VILE_FIN_TIDECALLER 1958
class VileFinTidecallerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VileFinTidecallerAI, MoonScriptCreatureAI);
	VileFinTidecallerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Frost Nova
		AddSpell(11831, Target_Self, 30, 0, 0);
	}  
};

//Warden Belamoore
#define CN_WARDEN_BELAMOORE 2415
class WardenBelamooreAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WardenBelamooreAI, MoonScriptCreatureAI);
	WardenBelamooreAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Fireball
		AddSpell(20815, Target_Current, 100, 3, 0);
		//Frost Armor
		ApplyAura(12544);
		//Polymorph
		AddSpell(13323, Target_RandomPlayer, 20, 1.5, 0);
		//Cone of Cold
		AddSpell(20828, Target_Self, 20, 0, 0);
	}  
};

//WarmasterLaggrond
#define CN_WARMASTER_LAGGROND 13840
class WarmasterLaggrondAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WarmasterLaggrondAI, MoonScriptCreatureAI);
	WarmasterLaggrondAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		//Mortal Strike
		AddSpell(16856, Target_Current, 50, 0, 0);
		//Cleave
		AddSpell(19642, Target_Current, 30, 0, 0);
	}  
};

void SetupAlteracMountainsNPC(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_ARCHMAGE_ANSIREM_RUNEWEAVER, &ArchmageAnsiremRuneweaverAI::Create);
    mgr->register_creature_script(CN_BATH_RAH_THE_WINDWATCHER, &BathrahtheWindwatcherAI::Create);
	mgr->register_creature_script(CN_ARGUS_SHADOW_MAGE, &ArgusShadowMageAI::Create);
	mgr->register_creature_script(CN_BARON_VARDUS, &BaronVardusAI::Create);
	mgr->register_creature_script(CN_CRUSHRIDGE_ENFORCER, &CrushridgeEnforcerAI::Create);
	mgr->register_creature_script(CN_CRUSHRIDGE_MAGE, &CrushridgeMageAI::Create);
	mgr->register_creature_script(CN_CRUSHRIDGE_MAULER, &CrushridgeMaulerAI::Create);
	mgr->register_creature_script(CN_CRUSHRIDGE_OGRE, &CrushridgeOgreAI::Create);
	mgr->register_creature_script(CN_CRUSHRIDGE_PLUNDERER, &CrushridgePlundererAI::Create);
	mgr->register_creature_script(CN_CRUSHRIDGE_WARMONGER, &CrushridgeWarmongerAI::Create);
	mgr->register_creature_script(CN_CYCLONIAN, &CyclonianAI::Create);
	mgr->register_creature_script(CN_DALARAN_SUMMONER, &DalaranSummonerAI::Create);
	mgr->register_creature_script(CN_DALARAN_THEURGIST, &DalaranTheurgistAI::Create);
	mgr->register_creature_script(CN_DRUNKEN_FOOTPAD, &DrunkenFootpadAI::Create);
	mgr->register_creature_script(CN_ELEMENTAL_SLAVE, &ElementalSlaveAI::Create);
	mgr->register_creature_script(CN_GIANT_MOSS_CREEPER, &GiantMossCreeperAI::Create);
	mgr->register_creature_script(CN_GIANT_YETI, &GiantYetiAI::Create);
	mgr->register_creature_script(CN_GLOMMUS, &GlommusAI::Create);
	mgr->register_creature_script(CN_GREY_BEAR, &GreyBearAI::Create);
	mgr->register_creature_script(CN_GREL_BORG_THE_MISER, &GrelborgtheMiserAI::Create);
	mgr->register_creature_script(CN_JAILOR_BORHUIN, &JailorBorhuinAI::Create);
	mgr->register_creature_script(CN_JEKYLL_FLANDRING, &JekyllFlandringAI::Create);
	mgr->register_creature_script(CN_LIEUTENANT_HAGGERDIN, &LieutenantHaggerdinAI::Create);
	mgr->register_creature_script(CN_LO_GROSH, &LoGroshAI::Create);
	mgr->register_creature_script(CN_LORD_ALIDEN_PERENOLDE, &LordAlidenPerenoldeAI::Create);
	mgr->register_creature_script(CN_MOUNTAIN_YETI, &MountainYetiAI::Create);
	mgr->register_creature_script(CN_MUCKRAKE, &MuckrakeAI::Create);
	mgr->register_creature_script(CN_MUG_THOL, &MugTholAI::Create);
	mgr->register_creature_script(CN_NARILLASANZ, &NarillasanzAI::Create);
	mgr->register_creature_script(CN_SKHOWL, &SkhowlAI::Create);
	mgr->register_creature_script(CN_STONE_FURY, &StoneFuryAI::Create);
	mgr->register_creature_script(CN_SYNDICATE_ASSASSIN, &SyndicateAssassinAI::Create);
	mgr->register_creature_script(CN_SYNDICATE_ENFORCER, &SyndicateEnforcerAI::Create);
	mgr->register_creature_script(CN_SYNDICATE_FOOTPAD, &SyndicateFootpadAI::Create);
	mgr->register_creature_script(CN_SYNDICATE_SABOTEUR, &SyndicateSaboteurAI::Create);
	mgr->register_creature_script(CN_SYNDICATE_SENTRY, &SyndicateSentryAI::Create);
	mgr->register_creature_script(CN_SYNDICATE_SPY, &SyndicateSpyAI::Create);
	mgr->register_creature_script(CN_SYNDICATE_THIEF, &SyndicateThiefAI::Create);
	mgr->register_creature_script(CN_SYNDICATE_WIZARD, &SyndicateWizardAI::Create);
	mgr->register_creature_script(CN_THANTHALDIS_SNOWGLEAM, &ThanthaldisSnowgleamAI::Create);
	mgr->register_creature_script(CN_VILE_FIN_TIDECALLER, &VileFinTidecallerAI::Create);
	mgr->register_creature_script(CN_WARDEN_BELAMOORE, &WardenBelamooreAI::Create);
	mgr->register_creature_script(CN_WARMASTER_LAGGROND, &WarmasterLaggrondAI::Create);
}


//Forsaken Gaming
//1.12.1 Server
//Alterac Mountains
//Developed by Seloria