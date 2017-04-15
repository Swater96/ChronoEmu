#include "StdAfx.h"

enum FixedDruidSpells
{
	SPELL_DRUID_NATURES_GRASP_1 = 16689,
	SPELL_DRUID_NATURES_GRASP_2 = 16810,
	SPELL_DRUID_NATURES_GRASP_3 = 16811,
	SPELL_DRUID_NATURES_GRASP_4 = 16812,
	SPELL_DRUID_NATURES_GRASP_5 = 16813,
	SPELL_DRUID_NATURES_GRASP_6 = 17329,
    SPELL_DRUID_NATURES_GRACE	= 16886,
	SPELL_DRUID_NATURES_GRACE_1 = 16880,
    SPELL_DRUID_CLEARCASTING 	= 16870,
	SPELL_DRUID_OMEN_OF_CLARITY = 16864,
};

void ApplySingleDruidSpellFixes(SpellEntry *sp)
{
	if(!sp)
		return;
	
	
	
	switch(sp->Id)
	{
		case SPELL_DRUID_NATURES_GRASP_1:
			{
				sp->Effect[0]	=	6; 
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0]	=	339; 
				sp->Effect[1]	=	0; 
				sp->procFlags	=	PROC_TARGET_MELEE_HIT	|	CPROC_REMOVEONUSE;
				sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
				sp->procChance = 100;
			}break;
		case SPELL_DRUID_NATURES_GRASP_2:
			{
				sp->Effect[0]	=	6; 
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0]	=	1062; 
				sp->Effect[1]	=	0; 
				sp->procFlags	=	PROC_TARGET_MELEE_HIT	|	CPROC_REMOVEONUSE;
				sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
				sp->procChance = 100;
			}break;
		case SPELL_DRUID_NATURES_GRASP_3:
			{
				sp->Effect[0]	=	6; 
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0]	=	5195; 
				sp->Effect[1]	=	0; 
				sp->procFlags	=	PROC_TARGET_MELEE_HIT	|	CPROC_REMOVEONUSE;
				sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
				sp->procChance = 100;
			}break;
		case SPELL_DRUID_NATURES_GRASP_4:
			{
				sp->Effect[0]	=	6; 
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0]	=	5196; 
				sp->Effect[1]	=	0; 
				sp->procFlags	=	PROC_TARGET_MELEE_HIT	|	CPROC_REMOVEONUSE;
				sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
				sp->procChance = 100;
			}break;
		case SPELL_DRUID_NATURES_GRASP_5:
			{
				sp->Effect[0]	=	6; 
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0]	=	9852; 
				sp->Effect[1]	=	0; 
				sp->procFlags	=	PROC_TARGET_MELEE_HIT	|	CPROC_REMOVEONUSE;
				sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
				sp->procChance = 100;
			}break;
		case SPELL_DRUID_NATURES_GRASP_6:
			{
				sp->Effect[0]	=	6; 
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0]	=	9853; 
				sp->Effect[1]	=	0; 
				sp->procFlags	=	PROC_TARGET_MELEE_HIT	|	CPROC_REMOVEONUSE;
				sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
				sp->procChance = 100;
			}break;
			
		// Feral Instinct
		case 16947:
		case 16948:
		case 16949:
		case 16950:
		case 16951:
			{
				sp->EffectSpellGroupRelation[1] = 33554432;
			}break;
			
		// Subtlety
		case 17118:
		case 17119:
		case 17120:
		case 17121:
		case 17122:
			{
				sp->EffectSpellGroupRelation[0] = 240;
			}break;
			
		case SPELL_DRUID_OMEN_OF_CLARITY:
			{
				sp->procFlags = PROC_ON_MELEE_HIT;
				sp->procChance = 6;
				sp->proc_interval = 10000;
			}break;
			

		sp = dbcSpell.LookupEntryForced( 9635 );
		if( sp != nullptr )
			sp->EffectRealPointsPerLevel[1] = 0;
			
		//druid - Blood Frenzy
		sp = dbcSpell.LookupEntryForced( 16954 );
		if( sp != nullptr ) 
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 16953;
			sp->procFlags = PROC_ON_MELEE_CRIT;
			sp->procChance = 100;
		}
		sp = dbcSpell.LookupEntryForced( 16952 );
		if( sp != nullptr ) 
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 16953;
			sp->procFlags = PROC_ON_MELEE_CRIT;
			sp->procChance = 50;
		}		
	}
}
