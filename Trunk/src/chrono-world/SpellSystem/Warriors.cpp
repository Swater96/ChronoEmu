/*
* OpenBasic MMORPG Server
* Copyright (C) 2009 - 2010 OpenBasic Team <http://openbasicemu.org/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "StdAfx.h"

enum FixedWarriorSpells
{
	SPELL_WARRIOR_BERSERKER_RAGE = 18499,
};

void ApplySingleWarriorSpellFixes(SpellEntry *sp)
{
	if(!sp)
		return;

	
	switch(sp->Id)
	{
		case SPELL_WARRIOR_BERSERKER_RAGE:	
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;//Forcing a dummy aura, so we can add the missing 4th effect.
			sp->Effect[1] = 0;
			sp->Effect[2] = 0;
			sp->EffectMiscValue[2] = 30;
		}break;
		
		//warrior - improved berserker rage
		sp = dbcSpell.LookupEntryForced( 20500 );
		if( sp != nullptr )
			sp->procFlags = PROC_ON_CAST_SPELL | CPROC_ON_SELF;
		sp = dbcSpell.LookupEntryForced( 20501 );
		if( sp != nullptr )
			sp->procFlags = PROC_ON_CAST_SPELL | CPROC_ON_SELF;

		//warrior - berserker rage is missing 1 effect = regenerate rage
		sp = dbcSpell.LookupEntryForced( 18499 );
		if( sp != nullptr )
		{
			sp->Effect[2] = 6;
			sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2] = 9174; //not sure if this is the one. In my time this adds 30 rage
			sp->procFlags = PROC_ON_TAKE_DAMAGE | CPROC_ON_SELF;
		}
				
								/** Flurry **/
		case 12319: case 12971: case 12972: case 12973: case 12974:
		case 16256: case 16281: case 16282: case 16283: case 16284:
			{
				sp->procFlags	=	PROC_ON_MELEE_CRIT;
			}break;
		
							/** Flurry Proc **/
		case 12966: case 12967: case 12968: case 12969: case 12970:
		case 16257: case 16277: case 16278: case 16279: case 16280:
			{
				sp->procFlags	=	PROC_ON_MELEE_HIT;
				sp->procCharges++;
			}break;
		
		
		// Enrage Proc, Do it like this or it won't remove charges
		case 12880:
		case 14201:
		case 14202:
		case 14203:
		case 14204:		
			{
				sp->procFlags	=	PROC_ON_MELEE_HIT;
				sp->procCharges++;
			}break;	

		// Bloodthirst
		case 23881:
			{
				sp->Effect[1]	=	64;
				sp->EffectTriggerSpell[1]	=	23885;
			}break;
		case 23892:
			{
				sp->Effect[1]	=	64;
				sp->EffectTriggerSpell[1]	=	23886;
			}break;
		case 23893:
			{
				sp->Effect[1]	=	64;
				sp->EffectTriggerSpell[1]	=	23887;
			}break;
		case 23894:
			{
				sp->Effect[1]	=	64;
				sp->EffectTriggerSpell[1]	=	23888;
			}break;	
			
		// Retaliation
		case 20230:
			{
				sp->Effect[0] = 6;
				sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				sp->EffectTriggerSpell[0] = 22858;
				sp->procFlags = PROC_TARGET_MELEE_HIT;
			}break;
			
		default:
		return;
	}
}
