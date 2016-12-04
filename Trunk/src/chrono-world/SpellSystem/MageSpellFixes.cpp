#include "StdAfx.h"

enum FixedMageSpells
{

};

/**
Mage Spells Not Working:

**/

void ApplySingleMageSpellFixes(SpellEntry *sp)
{
	if(!sp)
		return;

		
	
	switch(sp->Id)
	{				
	
		//////////////////////////////////////////
		// FIRE TALENTS							//
		//////////////////////////////////////////
		
		// Master of Elements - Not working
		case 29074:
		case 29075:
		case 29076:
			{
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0] = 29077;
				sp->procFlags = PROC_ON_SPELL_CRIT | CPROC_ON_SELF;
				sp->procChance = 100;
			}break;
			
		// Improved Scorch - Working
		case 11095:
		case 12872:
		case 12873:
			{
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->procChance = (sp->RankNumber * 33);
				sp->procFlags = PROC_ON_CAST_SPELL;
			}break;
			
		// Burning Soul - Working
		case 11083:
		case 12351:
			{
				sp->EffectSpellGroupRelation[0] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
			}break;
			
		// Fire Power - Working
		case 11124:
		case 12398:
		case 12399:
		case 12400:
		case 12378:
			{
				sp->EffectSpellGroupRelation[0] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
				sp->EffectSpellGroupRelation[1] = 8388608 | 16 | 2 | 4 | 8 | 262144 | 4194304 | 1;
			}break;
			
		// Combustion - Not Working
		case 11129:
			{
				sp->procFlags = PROC_ON_CAST_SPELL | PROC_ON_SPELL_CRIT | CPROC_ON_SELF;
				sp->procCharges = 0;
				sp->c_is_flags |= SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
			}break;

		//////////////////////////////////////////
		// FROST TALENTS						//
		//////////////////////////////////////////			
		
		// Frostbite - Procs on self sometimes
		case  11071:
		case  12496:
		case  12497:
			{
					sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_ROOT;
					sp->EffectTriggerSpell[0] = 12494;
					sp->procChance = (sp->RankNumber * 5); // up to 15%
					sp->procFlags = PROC_ON_CAST_SPELL;
					sp->School = SCHOOL_FROST;
					sp->c_is_flags |= SPELL_FLAG_CANNOT_PROC_ON_SELF;
			}break;

		// Permafrost - Working
		case 11175:
		case 12569:
		case 12571:
			{
				sp->EffectSpellGroupRelation[1] = 1048576;
			}break;
		
		// Piercing Ice - Working
		case 11151:
		case 12952:
		case 12953:
		case 12954:
		case 12957:
			{
				sp->EffectSpellGroupRelation[0] = 736;
			}break;
			
		// Ice Shards - Working
		case 11207:
		case 12672:	
		case 15047:	
		case 15052:	
		case 15053:
			{
				sp->EffectSpellGroupRelation[0] = 736;
				sp->EffectSpellGroupRelation[0] = 524288 | 131072;
			}break;
		
		// Improved Blizzard - Working
		case 11185:
		case 12487:
		case 12488:
			{
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0] = 12486;
				sp->procFlags = PROC_ON_CAST_SPELL;
			}break;
			
		// Frost Channeling - Working
		case 11160:
		case 12518:
		case 12519:
			{
				sp->EffectSpellGroupRelation[0] = 524288;
			}break;
			
		//////////////////////////////////////////
		// ARCANE TALENTS						//
		//////////////////////////////////////////

		// Arcane Focus - Seems to work
		case 11222:
		case 12839:
		case 12840:
		case 12841:
			{
				sp->EffectSpellGroupRelation[0] = 2359296;
			}break;
		
		// Arcane Power - No increase on mana usage?
		case 12042:
			{
				sp->EffectSpellGroupRelation[1] = 1 | 8192 | 4194304 | 8388608 | 262144 | 131072 | 536870912 | 524352 | 4 | 4096 | 2 | 2048 | 16;
				sp->EffectSpellGroupRelation[2] = 1 | 8192 | 4194304 | 8388608 | 262144 | 131072 | 536870912 | 524352 | 4 | 4096 | 2 | 2048 | 16;
			}break;
				
		// Magic Attunement - Working
		case 11247:
		case 12606:
			{
				sp->EffectSpellGroupRelation[0] = 8192;
			}break;
			
		// Arcane Instability - Working
		case 15058:
		case 15059:
		case 15060:
			{
				sp->EffectSpellGroupRelation[1] = 4194437;
				sp->EffectSpellGroupRelation[2] = 14686967;
			}break;
				
		// Arcane Concentration - Not Tested
		case  11213:
		case  12574:
		case  12575:
		case  12576:
		case  12577:
			{
				sp->procFlags = PROC_ON_SPELL_HIT | CPROC_ON_SELF;
			}break;
								
			default:
			return;
	}
}
