#include "StdAfx.h"

enum FixedHunterSpells
{

};

void ApplySingleHunterSpellFixes(SpellEntry *sp)
{
	if(!sp)
		return;

		
	
	switch(sp->Id)
	{
		//Frost Trap
		case 13809:
			{	
				sp->procFlags = PROC_ON_TRAP_TRIGGER;
				sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
				sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
				sp->Effect[1] = 0;  //stop strange effects
				sp->Effect[2] = 0;  //stop strange effects
				sp->c_is_flags |= SPELL_FLAG_CANNOT_PROC_ON_SELF;
			}break;
			
		// Aspect of the Pack
		case 13159:
			{
				sp->procFlags = PROC_ON_TAKE_DAMAGE;
			}break;
		// Aspect of the Cheetah
		case 5118:
			{
				sp->procFlags = PROC_ON_TAKE_DAMAGE;
			}break;
			
		// Improved Wing Clip
		case 19228:
		case 19232:
		case 19233:
		case 19234:
		case 19235:
			{
				sp->procChance = (sp->RankNumber*4);
			}break;
			
		// Endurance Training
		case 19583:
		case 19584:
		case 19585:
		case 19586:
		case 19587:
			{
				sp->EffectSpellGroupRelation[0] = 134217728;
				sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
				sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
				sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			}break;
			
		// Thick Hide
		case 19609:
		case 19610:
		case 19612:
			{
				sp->EffectSpellGroupRelation[0] = 67108864;
				sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
				sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_FLAT_MODIFIER; // No armor support for pets yet -.-
				sp->EffectBasePoints[0] *= 10; 
				sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			}break;

		// Ferocity
		case 19598:
		case 19599:
		case 19600:
		case 19601:
		case 19602:
			{
				sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
				sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_CRIT_PERCENT;
				sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			}break;
	
		//- Bestial Swiftness
		case 19596:
			{
				sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
				sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_SPEED; 
				sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			}break;
		
		// Hawk Eye
		case 19498: //Rank 1
		case 19499: // Rank 2
		case 19500: // Rank 3
			{	
				sp->EffectSpellGroupRelation[0] = 1 | 512 | 2048 | 4096 | 16384 | 262144 | 32768 | 65536 | 131072 ;
				sp->EffectSpellGroupRelation_high[0] = 1 | 128 | 4096 ;
			}break;

		// Frenzy
		case 19621:
		case 19622:
		case 19623:
		case 19624:
		case 19625:
			{
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0] = 19615;
				sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				sp->procChance = sp->EffectBasePoints[0];
				sp->procFlags = PROC_ON_MELEE_CRIT;
				sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | CPROC_ON_SELF;
			}break;
	
		//Unleashed Fury
		case 19616:
		case 19617:
		case 19618:
		case 19619:
		case 19620:
			{
				sp->EffectSpellGroupRelation[0] = 33554432;
				sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
				sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				sp->EffectMiscValue[0] = 8; 
				sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			}break;

		// Pathfinding
		case 19559:
		case 19560:
			{
				sp->EffectSpellGroupRelation[0] = 2097152;
				sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
			}break;
			
		// Clever Traps
		case 19239:
		case 19245:
			{
			sp->EffectSpellGroupRelation[0] = 16 | 8;
			sp->EffectSpellGroupRelation[0] = 4;
			sp->EffectSpellGroupRelation[0] = 128;
				//sp->EffectSpellGroupRelation[0] = 128;
			}break;
			
	// Spirit Bond
	sp = dbcSpell.LookupEntryForced( 19578 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 19579;
	}

	sp = dbcSpell.LookupEntryForced( 20895 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 24529;
	}
	sp = dbcSpell.LookupEntryForced( 19579 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
		sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
		sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		sp->EffectBasePoints[1] = sp->EffectBasePoints[0];
		sp->EffectAmplitude[1] = sp->EffectAmplitude[0];
		sp->EffectDieSides[1] = sp->EffectDieSides[0];
	}
	sp = dbcSpell.LookupEntryForced( 24529 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
		sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
		sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		sp->EffectBasePoints[1] = sp->EffectBasePoints[0];
		sp->EffectAmplitude[1] = sp->EffectAmplitude[0];
		sp->EffectDieSides[1] = sp->EffectDieSides[0];
	}		
	
		//Hunter : Explosive Trap
		case 13813:
			{
				sp->procFlags = PROC_ON_TRAP_TRIGGER;
				sp->EffectTriggerSpell[1] = 13812;
			}break;

		case 14316:
		{
			sp->procFlags = PROC_ON_TRAP_TRIGGER;
			sp->EffectTriggerSpell[1] = 14314;
		}break;

		case 14317:
			{	
				sp->procFlags = PROC_ON_TRAP_TRIGGER;
				sp->EffectTriggerSpell[1] = 14315;
			}break;

		case 27025:
			{
				sp->procFlags = PROC_ON_TRAP_TRIGGER;
				sp->EffectTriggerSpell[1] = 27026;
			}break;

		// Entrapment
		case 19184:
		case 19387:
		case 19388:
		case 19389:
		case 19390:
			{
				sp->procFlags = PROC_ON_TRAP_TRIGGER;
			}break;
						
		// Mortal Shots
		case 19485:
		case 19487:
		case 19488:
		case 19489:
		case 19490:
			{
				sp->EffectSpellGroupRelation[0] = 1 | 2048 | 4096 | 131072 | 262144 ;
				sp->EffectSpellGroupRelation_high[0] = 1 ;
			}
						
		default:
		return;
	}
}
