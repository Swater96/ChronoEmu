#include "StdAfx.h"

enum FixedRogueSpells
{

};

void ApplySingleRogueSpellFixes(SpellEntry *sp)
{
	if(!sp)
		return;

		
	
	switch(sp->Id)
	{
		// Rogue - Endurance 
		case 13742: 
		case 13872:
			{
				sp->EffectSpellGroupRelation[0] = 64 | 32;	//Sprint + Evasion
			}break;
			
		// Rogue - Camouflage.
		case  13975:
		case  14062:
		case  14063:
		case  14064:
		case  14065:	
			{
				sp->EffectSpellGroupRelation[0] = 4194304;
				sp->EffectMiscValue[0] = SMT_SPELL_VALUE;
				sp->EffectSpellGroupRelation[1] = 4194304;
			}break;
			
		// Rogue - Seal Fate
		case 14186:
		case 14190:
		case 14193:
		case 14194:
		case 14195:
			{
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0] = 14189;
				sp->procFlags = PROC_ON_MELEE_CRIT;
				sp->procChance = (sp->RankNumber * 20);
			}break;
			
		// Rogue - Elusiveness
		case 13981:
		case 14066:
			{
				sp->EffectSpellGroupRelation[0] = 0x00000800 | 0x01000000;
			}break;		
	
		// Vile Poisons
		case 16513: 
		case 16514: 
		case 16515: 
		case 16719: 
		case 16720: 
			{
				sp->EffectSpellGroupRelation[0] = 0x00002000 | 0x00004000 | 0x00008000 | 0x00010000 | 0x10000000;
				sp->EffectSpellGroupRelation[1] = 0x00800000; //maybe this is mixed up with 0 grouprelation ?
				sp->EffectSpellGroupRelation[2] = 0x00002000 | 0x00004000 | 0x00008000 | 0x00010000 | 0x10000000;
			}break;
		
		// Improved Poisons
		case 14113: 
		case 14114: 
		case 14115: 
		case 14116: 
		case 14117: 
		case 21881: 
			{
				sp->EffectSpellGroupRelation[0] = 0x00002000 | 0x00004000 | 0x00008000 | 0x00010000 | 0x10000000;
			}break;

		// Improved Expose Armor
		case 14168: 
		case 14169: 
			{
				sp->EffectSpellGroupRelation[0] = 0x00080000;
			}break;
			
		// Relentless Strikes
		/*case 14179:
			{
				sp->EffectSpellGroupRelation[0]= 4063232 | 262144 | 2097152 | 8388608 | 8519680 | 524288 | 1048576 | 8388608;
			}break;*/
			
		//Relentless Strikes
		case 14179:
			{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;//proc	spell
					sp->procFlags	=	PROC_ON_CAST_SPELL;
			}break;
			
		// Cold Blood
		case 14177:
			{
				sp->EffectSpellGroupRelation[0] = 131590;
			}break;
	
		// Spell 14057 Group Relation (Opportunity Rank 1)
		case 14057:
		case 14072:
		case 14073:
		case 14074:
		case 14075:
			{
				sp->EffectSpellGroupRelation[0] = 516;
			}break;
			
	// Dirty Deeds
	sp = dbcSpell.LookupEntryForced( 14082 ); 
	if( sp != nullptr )
		sp->EffectSpellGroupRelation[0] = 1024 | 256; // Cheap Shot + Garrote
	sp = dbcSpell.LookupEntryForced( 14083 ); 
	if( sp != nullptr )
		sp->EffectSpellGroupRelation[0] = 1024 | 256; // Cheap Shot + Garrote
		
		//Vanish should dispel all movement impairing effects
		case 1856:
		case 1857:
 		case 27617:
 			{
				sp->EffectTriggerSpell[1] = 0; //this workaround is fixed somewhere else in core
 			}break;

		case 13743:
		case 13875:
			{
				sp->EffectTriggerSpell[0] = 0; //this is just a disableling needs a fix! or extra workaround :p
			}break;

		
		default:
		return;
	}
}
