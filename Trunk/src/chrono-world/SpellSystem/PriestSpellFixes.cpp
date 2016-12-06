#include "StdAfx.h"

enum FixedPriestSpells
{

};

void ApplySinglePriestSpellFixes(SpellEntry *sp)
{
	if(!sp)
		return;

	
	switch(sp->Id)
	{							
		//Holy Nova
		case 15237:
			{
				sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
				sp->EffectTriggerSpell[1]	=	23455;
			}break;
		case 15430:
			{
				sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
				sp->EffectTriggerSpell[1]	=	23458;
			}break;
		case 15431:
			{
				sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
				sp->EffectTriggerSpell[1]	=	23459;
			}break;
		case 27799:
			{
				sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
				sp->EffectTriggerSpell[1]	=	27803;
			}break;
		case 27800:
			{
				sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
				sp->EffectTriggerSpell[1]	=	27804;
			}break;
		case 27801:
			{
				sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
				sp->EffectTriggerSpell[1]	=	27805;
			}break;
			
		// Improved Inner Fire
		case 14747:
		case 14770:
		case 14771:
			{
				sp->EffectSpellGroupRelation[0] = 2;
			}break;

		// Mental Agility
		case 14520:
		case 14780:
		case 14781: 
		case 14782:
		case 14783: 
			{
				sp->EffectSpellGroupRelation[0] = 2147483648UL | 65536 | 67108864UL | 4 | 1 | 64 | 32 | 4194304UL | 32768 | 8388608UL | 8 | 16384 | 2 | 256 | 16777216UL | 2097152UL | 33554432UL;
				sp->EffectSpellGroupRelation_high[0] = 1 | 64 | 2;
			}break;
		
		// Force of Will
		case 18544:
		case 18547:
		case 18548:
		case 18549:
		case 18550:
			{
				sp->EffectSpellGroupRelation[0] = 139993232;
				sp->EffectSpellGroupRelation[1] = 5775504;
				sp->EffectSpellGroupRelation[2] = 10518528;
			}break;

		// Holy Specialization
		case 14889:
		case 15008:
		case 15009:
		case 15010:
		case 15011:
			{
				sp->EffectSpellGroupRelation[0] = 156507776;
			}break;
	
	// Improved Renew
		case 14908:
		case 15020:
		case 17191:
			{
				sp->EffectSpellGroupRelation[0] = 64;
			}break;

		// Blessed Recovery
		case 27811:
			{
				sp->EffectTriggerSpell[0] = 27813;
				sp->procFlags = PROC_ON_MELEE_CRIT;
			}break;
		case 27815:
			{
				sp->EffectTriggerSpell[0] = 27817;
				sp->procFlags = PROC_ON_MELEE_CRIT;
			}break;
		case 27816:
			{
				sp->EffectTriggerSpell[0] = 27818;
				sp->procFlags = PROC_ON_MELEE_CRIT;
			}break;
	
		// Inspiration Proc
		case  14893 :
		case  15357 :
		case  15359 :
			{
				sp->rangeIndex = 4;
			}break;
			
		// Shadow Affinity
		case 15318:
		case 15272:
		case 15320:
			{
				sp->EffectSpellGroupRelation[0] = 109830400;
			}break;
		
		// Shadow Focus
		case 15260:
		case 15327:
		case 15328:
		case 15329:
		case 15330:
			{
				sp->EffectSpellGroupRelation[0] = 109813760;
			}break;
			
		// Spirit Tap - 15271 is triggered spell
		case 15270:
		case 15335:
		case 15336:
		case 15337:
		case 15338:
			{
				sp->procFlags = PROC_ON_KILL | CPROC_TARGET_DIE;
			}break;
	
		default:
		return;
	}
}
