/*
 * ChronoEmu Scripts - Uldaman Bosses
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by Light
 */


#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"



//BOSSES


//Ironaya 7228
#define CN_IRONAYA  7228
class Ironaya : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Ironaya, MoonScriptCreatureAI);
    Ironaya(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {     // Arcing Smash , Just tested it on retail , look like 50%
		AddSpell(8374, Target_Current, 50, 0, 0);
		// Knock away , kick the player away , Randomly kick'd my friend , look like a good 25%
		AddSpell(10101, Target_RandomPlayer, 25, 0, 0);
		// War Stomp , Stun everyone for 5s and does 300dmg , she did it 2time in a 1m fight , look like a 10-20%
		AddSpell(11876, Target_Self, 15, 0, 0);
	}
};


//Revelosh 6910
#define CN_REVELOSH 6910
class Revelosh : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Revelosh, MoonScriptCreatureAI);
    Revelosh(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Chain lightning.
		AddSpell(16006, Target_Current, 50, 3, 0);
		//Lightning bolt
		AddSpell(15801, Target_Current, 60, 3, 0);
    }
};


//Magregan Deepshadow 2932
#define CN_MAGREGAN_DEEPSHADOW 2932
class MagreganDeepshadow : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(MagreganDeepshadow, MoonScriptCreatureAI);
    MagreganDeepshadow(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Whirlwind
		AddSpell(16006, Target_Current, 50, 1, 0);
	}	
};


//Obsidian Sentinel 7023
#define CN_OBSIDIAN_SENTINEL 7023
class ObsidianSentinel : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ObsidianSentinel, MoonScriptCreatureAI);
    ObsidianSentinel(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Summon obsidian , Little lvl 40 normal mobs.
		AddSpell(10061, Target_Self, 100, 0, 3);
		// Splintered Obsidian , Reduce Dmg of himself by 40%.
		AddSpell(10072, Target_Current, 30, 0, 2);
    }
};
// Galgann firehammer 7291
#define CN_GALGANN_FIREHAMMER 7291
class GalgannFirehammer : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(GalgannFirehammer, MoonScriptCreatureAI);
    GalgannFirehammer(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {    //amplify Flames. 
		AddSpell(9482, Target_RandomPlayer, 25,1,0);
		// Fire nova
		AddSpell(26073, Target_Self, 40, 2, 0);
		// flame lash
		AddSpell(3356, Target_Current, 30, 1, 0);
	    // FLame shock
		AddSpell(8053, Target_RandomPlayer, 40, 1, 0);
	}
};

//Grimlok 4854
#define CN_GRIMLOK 7291
class Grimlok : public MoonScriptCreatureAI
{
	bool do_Enrage;
	SpellDesc *Enrage;
public:
	MOONSCRIPT_FACTORY_FUNCTION(Grimlok, MoonScriptCreatureAI);
    Grimlok(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {    //Chain Lightning 
		AddSpell(8292, Target_RandomPlayer, 25,2,0);
		// Lightning bolt
		AddSpell(9532, Target_Self, 30,3,0);
		// Shrink
		AddSpell(7289, Target_RandomPlayer, 30, 1, 0);
	    // tremor totem
		AddSpell(8143, Target_Self, 40, 1, 0);
		do_Enrage = false;
		Enrage = AddSpell(6742, Target_Self, 0, 0, 0);
	}

	void AIUpdate() {
		if(!do_Enrage && (GetHealthPercent() < 30)) {
			do_Enrage = true;
			CastSpell(Enrage);
		}
		ParentClass::AIUpdate();
	}
};

void SetupUldamanBosses(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_IRONAYA, &Ironaya::Create);
	mgr->register_creature_script(CN_REVELOSH, &Revelosh::Create);
	mgr->register_creature_script(CN_MAGREGAN_DEEPSHADOW, &MagreganDeepshadow::Create);
	mgr->register_creature_script(CN_OBSIDIAN_SENTINEL, &ObsidianSentinel::Create);
	mgr->register_creature_script(CN_GALGANN_FIREHAMMER, &GalgannFirehammer::Create);
	mgr->register_creature_script(CN_GRIMLOK, &Grimlok::Create);
}