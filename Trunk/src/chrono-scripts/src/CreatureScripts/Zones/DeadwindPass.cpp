//Deadwind Pass zone by Swater

#include "StdAfx.h"
#include "../Setup.h"
#include "../../Common/Base.h"


//Trash

#define CN_DAMNED_SOUL 12378
class DamnedSoulAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DamnedSoulAI, MoonScriptCreatureAI);
	DamnedSoulAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(13748, Target_Current, 30,1,5);
		AddSpell(20825, Target_Current, 30,3,10);
	}
};

#define CN_DEADWIND_MAULER 7371
class DeadwindMaulerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeadwindMaulerAI, MoonScriptCreatureAI);
	DeadwindMaulerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(10966, Target_Current, 60,0,10);
	}
};

#define CN_DEADWIND_OGRE_MAGE 7379
class DeadwindOgreMageAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeadwindOgreMageAI, MoonScriptCreatureAI);
	DeadwindOgreMageAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(15039, Target_Current, 10,0,12);
		AddSpell(20824, Target_Current, 30,3,5);
		AddSpell(6742, Target_Self, 30,0,30);
		AddSpell(12550, Target_Self, 30,0,10);
	}
};

#define CN_DEADWIND_WARLOCK 7372
class DeadwindWarlockAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeadwindWarlockAI, MoonScriptCreatureAI);
	DeadwindWarlockAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(11980, Target_Current, 30,0,120);
		AddSpell(20787, Target_Current, 30,2,21);
		AddSpell(20825, Target_Current, 30,3,5);
	}
};

#define CN_RESTLESS_SHADE 7370
class RestlessShadeAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RestlessShadeAI, MoonScriptCreatureAI);
	RestlessShadeAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(18267, Target_Current, 30,0,30);
	}
};

#define CN_SKY_SHADOW 7376
class SkyShadowAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SkyShadowAI, MoonScriptCreatureAI);
	SkyShadowAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(13443, Target_Current, 70,0,10);
	}
};

#define CN_UNLIVING_CARETAKER 12379
class UnlivingCaretakerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(UnlivingCaretakerAI, MoonScriptCreatureAI);
	UnlivingCaretakerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(8599, Target_Self, 30,0,120);
	}
};

#define CN_UNLIVING_RESIDENT 12380
class UnlivingResidentAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(UnlivingResidentAI, MoonScriptCreatureAI);
	UnlivingResidentAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(11978, Target_Current, 30,0,10);
		AddSpell(11971, Target_Current, 60,0,6);
	}
};

#define CN_WAILING_SPECTRE 12377
class WailingSpectreAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WailingSpectreAI, MoonScriptCreatureAI);
	WailingSpectreAI(Creature* pCreature):MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(7713, Target_Self, 60,0,7);
	}
};

void SetupDeadwindPassNPC(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_DAMNED_SOUL, &DamnedSoulAI::Create);
	mgr->register_creature_script(CN_DEADWIND_MAULER, &DeadwindMaulerAI::Create);
	mgr->register_creature_script(CN_DEADWIND_OGRE_MAGE, &DeadwindOgreMageAI::Create);
	mgr->register_creature_script(CN_DEADWIND_WARLOCK, &DeadwindWarlockAI::Create);
	mgr->register_creature_script(CN_RESTLESS_SHADE, &RestlessShadeAI::Create);
	mgr->register_creature_script(CN_SKY_SHADOW, &SkyShadowAI::Create);
	mgr->register_creature_script(CN_UNLIVING_CARETAKER, &UnlivingCaretakerAI::Create);
	mgr->register_creature_script(CN_UNLIVING_RESIDENT, &UnlivingResidentAI::Create);
	mgr->register_creature_script(CN_WAILING_SPECTRE, &WailingSpectreAI::Create);
}