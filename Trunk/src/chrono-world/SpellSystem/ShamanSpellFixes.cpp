#include "StdAfx.h"

enum FixedShamanSpells
{

};

void ApplySingleShamanSpellFixes(SpellEntry *sp)
{
	if(!sp)
		return;

		
	
	
	switch(sp->Id)
	{		
		
		// Elemental Focus
		case 16164:
			{
				sp->procFlags	=	PROC_ON_SPELL_HIT;
			}break;
		// Elemental Focus - Clearcasting
		case 16246:
			{
				sp->procFlags	=	PROC_ON_CAST_SPELL;
				//sp->procCharges++; //	first	charge gets	lost when	it gets	procced
			}break;
										
		// Restorative Totems
		case 16187:
		case 16205:
		case 16206:
		case 16207:
		case 16208: 
			{
				sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
				sp->EffectMiscValue[0] = SMT_DAMAGE_DONE;
				sp->EffectApplyAuraName[1] = SPELL_AURA_ADD_PCT_MODIFIER;
			}break;
			
		// Stormstrike
		case 17364:
			{
				sp->procFlags=PROC_ON_SPELL_HIT;
				sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
			}break;
			
		// Lightning Mastery
		case 16578:
		case 16579:
		case 16580:
		case 16581:
		case 16582:
			{
				sp->EffectSpellGroupRelation[0]=1|2;
			}break;
			
		// Purge
		case 370:
		case 8012:
		case 27626:
			{
				sp->DispelType = DISPEL_MAGIC;
			}break;
			
		// Weapon Mastery
		case 20504:
		case 20505:
			{
				sp->EffectSpellGroupRelation[0] = 1073741824;
			}break;		
			
		// Shaman - Storm Reach
		case 28999:
		case 29000:
			{
				sp->EffectSpellGroupRelation[0] = 2 | 1;
			}break;
			
		//shaman - Elemental Weapons
		case 29080: 
		case 29079:
		case 16266:
			{
				sp->EffectSpellGroupRelation[0] = 0x400000; // Rockbiter
				sp->EffectMiscValue[0] = SMT_DAMAGE_DONE;
				sp->EffectSpellGroupRelation[1] = 0x200000 | 0x1000000;
				sp->EffectSpellGroupRelation_high[2] = 0x800;
				sp->EffectMiscValue[1] = SMT_DAMAGE_DONE;
				sp->EffectMiscValue[2] = SMT_DAMAGE_DONE;
			}break;
	
		// Windfury Totem
		case 8516:
		case 10608:
		case 10610:
			{
				sp->procChance = 10;
			}break;
			
		// Elemental mastery 
		case 16166:
			{
				sp->EffectSpellGroupRelation[0] = 0xFFFFFFFF;//nature+fire+frost is all that shaman can do
				sp->EffectSpellGroupRelation_high[0] = 0xFFFFFFFF;//nature+fire+frost is all that shaman can do
				sp->EffectSpellGroupRelation[1] = 0xFFFFFFFF;//nature+fire+frost is all that shaman can do
				sp->EffectSpellGroupRelation_high[1] = 0xFFFFFFFF;//nature+fire+frost is all that shaman can do
			}break;
			
		// Healing Grace
		case 29191: 
		case 29189: 
		case 29187:
			{
				sp->EffectSpellGroupRelation[0] = 64 | 128 | 256;
				sp->EffectSpellGroupRelation[1] = 0xFFFFFFFF; // all spells
				sp->EffectSpellGroupRelation_high[1] = 0xFFFFFFFF; // all spells
			}break;
			
		// Totemic Mastery
		case 16189:
			{
				sp->EffectSpellGroupRelation_high[0] = 256;
				sp->EffectSpellGroupRelation[0] = 67362816;
			}break;
	
		// Restorative Totems
		sp = dbcSpell.LookupEntryForced( 16187 ); 
		sp = dbcSpell.LookupEntryForced( 16205 ); 
		sp = dbcSpell.LookupEntryForced( 16206 ); 
		sp = dbcSpell.LookupEntryForced( 16207 ); 
		sp = dbcSpell.LookupEntryForced( 16208 ); 
			{
				sp->EffectSpellGroupRelation[0] = 0x00002000;
				sp->EffectSpellGroupRelation[1] = 0x00004000;
			}break;
			
		// Guardian Totems
		case 16293:
		case 16258:
			{
				sp->EffectSpellGroupRelation[0] = 0x8000;
				sp->EffectSpellGroupRelation[1] = 0x40000;
			}break;
			
		// Enhancing Totems
		case 16259:
		case 16295:
			{
				sp->EffectSpellGroupRelation[0] = 0x00010000 | 0x00020000;
			}break;
			
		// Improved Weapon Totems
		case 29193: 
		case 29192: 
			{
				sp->EffectApplyAuraName[0]=SPELL_AURA_ADD_PCT_MODIFIER;
				sp->EffectApplyAuraName[1]=SPELL_AURA_ADD_PCT_MODIFIER;
				sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
				sp->EffectMiscValue[1] = SMT_SPELL_VALUE;
				sp->EffectSpellGroupRelation_high[0] = 0x2;
				sp->EffectSpellGroupRelation[1] = 0x2000000;
			}break;
		
		// Earth's Grasp
		case 16130: 
		case 16043: 
			{
				sp->EffectSpellGroupRelation[0] = 0x8;
				sp->EffectSpellGroupRelation_high[1] = 0x1;
			}break;
			
		default:
		return;
	}
}
