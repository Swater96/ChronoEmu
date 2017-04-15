#include "StdAfx.h"

enum FixedPaladinSpells
{
	SPELL_SEAL_OF_RIGHTEOUSNESS_1 = 21084,
};

void ApplySinglePaladinSpellFixes(SpellEntry *sp)
{
	if (sp != nullptr)
	{
		switch (sp->Id)
		{
		case SPELL_SEAL_OF_RIGHTEOUSNESS_1:
		{
			sp->EffectTriggerSpell[0] = 20187;
			sp->procFlags = PROC_ON_MELEE_HIT;
		}break;
		// Seal of Command Proc Chance fix
		case 20375:
		{
			sLog.outDebug("Seal of Command fix!");
			sp->procChance = 100;
		}break;

		// Vindication Proc Chance fix		
		case 9452:
		case 26016:
		case 26021:
		{
			sp->procChance = 30;
		}break;

		// Eye for an Eye
		case 9799:
		case 25988:
		{
			sp->procFlags = PROC_TARGET_SPELL_CRIT;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 25997;
		}break;

		//Improved Seal of the Crusader
		case 20335:
		case 20336:
		case 20337:
		{
			sp->EffectSpellGroupRelation[0] = 536870912;
		}break;

		//Improved Devotion Aura
		case 20142:
		case 20141:
		case 20140:
		case 20139:
		case 20138:
		{
			sp->EffectSpellGroupRelation[0] = 64;
		}break;

		//Improved Concentration Aura
		case 20256:
		case 20255:
		case 20254:
		{
			sp->EffectSpellGroupRelation[0] = 131072;
		}break;

		//Healing Light
		case 20237:
		case 20239:
		{
			sp->EffectSpellGroupRelation[0] = 0x40000000 | 0x80000000;
		}break;

		//Improved Righteous Fury
		case 20468:
		case 20469:
		case 20470:
		{
			sp->EffectSpellGroupRelation[0] = 1;
			sp->EffectSpellGroupRelation[1] = 1;
		}break;

		//Holy Shield procfixes
		case 20925:
		case 20927:
		case 20928:
		case 20929:
		{
			sp->procFlags = PROC_ON_BLOCK;
		}break;

		// Improved Lay on Hands
		case 20234:
		case 20235:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;

		default:
			return;
		}
	}
}
