#include "StdAfx.h"

enum FixedWarlockSpells
{

};

void ApplySingleWarlockSpellFixes(SpellEntry *sp)
{
	if(!sp)
		return;

		
	
	switch(sp->Id)
	{
		// Unholy Power
		case  18769:
		case  18770:
		case  18771:
		case  18772:
		case  18773:
			{
				sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
				sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
				sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
				sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
				sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
				sp->EffectMiscValue[1] = SCHOOL_NORMAL;
				sp->EffectBasePoints[1]	=	sp->EffectBasePoints[0]	;
			}break;
			
		// Devastation
		case 18130:
		case 18131:
		case 18132:
		case 18133:
		case 18134:
			{
				sp->EffectSpellGroupRelation[0] = 4 | 1 | 64 | 256 | 32 | 128 | 512;
				sp->EffectSpellGroupRelation_high[0] = 64 | 128 |4096;
			}break;
			
		// Improved Searing Pain
		case 17927:
		case 17929:
		case 17930:
			{
				sp->EffectSpellGroupRelation[0] = 256;
			}break;
		
		// Shadow Mastery
		case 18271:
		case 18272:
		case 18273:
		case 18274:
		case 18275:
			{
				sp->EffectSpellGroupRelation[0] =  2147483648UL | 4194304 | 1 | 2 | 16384 | 1024 | 8 | 262144 | 524288 | 2147483648UL | 16777216UL | 128 | 16 | 32768;
				sp->EffectSpellGroupRelation[1] =  2147483648UL | 4194304 | 1 | 2 | 16384 | 1024 | 8 | 262144 | 524288 | 2147483648UL | 16777216UL | 128 | 16 | 32768;
			}break;

		// Suppression
		case 18174:
		case 18175:
		case 18176:
		case 18177:
		case 18178:
			{
				sp->EffectSpellGroupRelation[0] = 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
			}break;
		
		// Ruin
		case 17959:
			{
				sp->EffectSpellGroupRelation[0] = 1 | 4 | 32 | 64 | 128 | 256 | 512 ;
				sp->EffectSpellGroupRelation_high[0] = 64 | 128 | 4096 ;
			}break;
	
		//Emberstorm
		case 17954:
		case 17955:
		case 17956:
		case 17957:
		case 17958:
			{
				sp->EffectSpellGroupRelation[0] = 32 | 64 | 4 | 256 | 512;
				sp->EffectSpellGroupRelation_high[0] = 64 | 128;
				sp->EffectSpellGroupRelation[1] = 4;
			}break;
			
		//Aftermath
		case 18123:
		case 18122:
		case 18121:
		case 18120:
		case 18119:
			{
					sp->procFlags = PROC_ON_CAST_SPELL;
			}break;		
	
		//Pyroclasm
		case 18073:
		case 18096:
			{
				sp->Effect[0] = 0;
				sp->EffectTriggerSpell[1] = 18093;
				sp->procFlags = PROC_ON_SPELL_HIT;
				sp->procChance = (sp->RankNumber*13);
			}break;
	
		//warlock -  soul link
		/*case  19028:
			{
				sp->Effect[0]=SPELL_EFFECT_APPLY_AURA;
				sp->EffectApplyAuraName[0] = SPELL_AURA_SPLIT_DAMAGE;
				sp->EffectMiscValue[0]=20;
				sp->c_is_flags |=	SPELL_FLAG_IS_EXPIREING_WITH_PET;
			}break;*/
			
			
		//warlock: Nightfall
		case 18094:
		case 18095:
			{
				sp->EffectSpellGroupRelation[0] = 1;
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0] = 17941;
				sp->procFlags = PROC_ON_CAST_SPELL;
				sp->procChance = (sp->RankNumber*2);
			}break;
			
		// Fel Intellect
		case 18731:
		case 18743:
		case 18744:
		case 18745:
		case 18746:
			{
				//sp->EffectSpellGroupRelation[0] = 268435456;
				sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
				sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
				sp->EffectMiscValue[0] = 3;
				sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			}break;
	
		// Fel Stamina
		case 18748:
		case 18749:
		case 18750:
		case 18751:
		case 18752:
			{
				//sp->EffectSpellGroupRelation[0] = 134217728;
				sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
				sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
				sp->EffectMiscValue[0] = 2;
				sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			}break;
			
		// Shadowburn
		case 17877:
		case 18867:
		case 18868:
		case 18869:
		case 18870:
		case 18871:
			{
				sp->EffectSpellGroupRelation[0] = 6265;
			}break;
			
		//Improved Imp
		case 18694:
		case 18695:
		case 18696:
			{
				sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
				sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				sp->EffectSpellGroupRelation[0] = 4096 | 8388608;
			}break;
			
		//Improved Voidwalker
		case 18705:
		case 18706:
		case 18707:
			{
				sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
				sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				sp->EffectSpellGroupRelation[0] = 33554432;
			}break;	
			
		//Improved Succubus
		case 18754:
		case 18755:
		case 18756:
			{
				sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ;
				sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
				sp->EffectSpellGroupRelation[0] = 8192 | 1073741824;
				sp->EffectSpellGroupRelation[1] = 1073741824;
			}break;			
			
		// This proc's on any spell that kills the creature
		// That gives a HUGE benefit to warlock, so temp removed.
		//warlock - Improved Drain Soul
		/*case 18213:
		case 18372:
			{
				sp->procFlags = CPROC_TARGET_DIE | CPROC_ON_SELF;
				sp->procChance = 100;
				sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0] = 18371;
				sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
				sp->EffectSpellGroupRelation[1] = 2 | 8 | 32768 | 2147483648UL | 1024 | 16384 | 262144 | 16 | 524288 | 4194304;
				sp->Effect[2] = 0 ; 
			}break;*/
			
			
			
	//Master Demononologist -- Taken from AspireDev. Shitload of spells for it.
	//warlock - Master Demonologist - 25 spells here
	sp = dbcSpell.LookupEntryForced( 23785 ); // 23875
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 23784;
	}
	sp = dbcSpell.LookupEntryForced( 23822 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 23830;
	}
	sp = dbcSpell.LookupEntryForced( 23823 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 23831;
	}
	sp = dbcSpell.LookupEntryForced( 23824 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 23832;
	}
	sp = dbcSpell.LookupEntryForced( 23825 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
		sp->EffectTriggerSpell[0] = 35708;
	}
	//and the rest
	sp = dbcSpell.LookupEntryForced( 23784 );
	if( sp != nullptr )
		sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
	sp = dbcSpell.LookupEntryForced( 23830 );
	if( sp != nullptr )
		sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
	sp = dbcSpell.LookupEntryForced( 23831 );
	if( sp != nullptr )
		sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
	sp = dbcSpell.LookupEntryForced( 23832 );
	if( sp != nullptr )
		sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
	sp = dbcSpell.LookupEntryForced( 35708 );
	if( sp != nullptr )
		sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
	sp = dbcSpell.LookupEntryForced( 23759 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
	}
	sp = dbcSpell.LookupEntryForced( 23760 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
	}
	sp = dbcSpell.LookupEntryForced( 23761 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
	}
	sp = dbcSpell.LookupEntryForced( 23762 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
	}
	sp = dbcSpell.LookupEntryForced( 23826 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
	}
	sp = dbcSpell.LookupEntryForced( 23827 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
	}
	sp = dbcSpell.LookupEntryForced( 23828 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
	}
	sp = dbcSpell.LookupEntryForced( 23829 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
	}
	for(uint32 i=23833;i<=23844;i++)
	{
		sp = dbcSpell.LookupEntryForced( i );
		if( sp != nullptr )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		}
	}
	sp = dbcSpell.LookupEntryForced( 35702 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		sp->Effect[1] = 0; //hacks, we are handling this in another way
	}
	sp = dbcSpell.LookupEntryForced( 35703 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		sp->Effect[1] = 0; //hacks, we are handling this in another way
	}
	sp = dbcSpell.LookupEntryForced( 35704 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		sp->Effect[1] = 0; //hacks, we are handling this in another way
	}
	sp = dbcSpell.LookupEntryForced( 35705 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		sp->Effect[1] = 0; //hacks, we are handling this in another way
	}
	sp = dbcSpell.LookupEntryForced( 35706 );
	if( sp != nullptr )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
		sp->Effect[1] = 0; //hacks, we are handling this in another way
	}
			
			
		default:
		return;
	}
}
