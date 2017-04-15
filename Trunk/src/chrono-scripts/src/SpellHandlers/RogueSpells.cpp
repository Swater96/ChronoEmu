/****************************************************************************
 *
 * SpellHandler Plugin
 * Copyright (c) 2007 Team Ascent
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons,
 * 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
 *
 * EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU
 * ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES
 * ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 */

#include "StdAfx.h"
#include "Setup.h"

bool Preparation(uint32 i, Spell * pSpell)
{
    Player * playerTarget = pSpell->p_caster;
    if(playerTarget == 0) return true;

    playerTarget->ClearCooldownForSpell( 5277 );	// Evasion Rank 1
    playerTarget->ClearCooldownForSpell( 26669 );	// Evasion Rank 2
    playerTarget->ClearCooldownForSpell( 2983 );	// Sprint Rank 1
    playerTarget->ClearCooldownForSpell( 8696 );	// Sprint Rank 2
    playerTarget->ClearCooldownForSpell( 11305 );	// Sprint Rank 3
    playerTarget->ClearCooldownForSpell( 1856 );	// Vanish Rank 1
    playerTarget->ClearCooldownForSpell( 1857 );	// Vanish Rank 2
    playerTarget->ClearCooldownForSpell( 26889 );	// Vanish Rank 3
    playerTarget->ClearCooldownForSpell( 14177 );	// Cold Blood
    playerTarget->ClearCooldownForSpell( 14183 );	// Premeditation
    playerTarget->ClearCooldownForSpell( 36554 );	// Shadowstep
    return true;
}

#ifdef ASCENT_CORE

class Ambush : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Ambush);
	Ambush(Spell * pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (_spell->u_caster == nullptr || !_spell->u_caster->InStealth())
			return SPELL_FAILED_ONLY_STEALTHED;

		if (_spell->p_caster != nullptr)
		{
			Item * pMainHand = _spell->p_caster->GetItemInterface()->GetInventoryItem( INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_MAINHAND );
			if( !pMainHand || pMainHand->GetProto()->Class != 2 || pMainHand->GetProto()->SubClass != 15 )
				return SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND;
		}

		return SPELL_CANCAST_OK;
	}
};

class Backstab : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Backstab);
	Backstab(Spell * pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (_spell->p_caster != nullptr)
		{
			Item * pMainHand = _spell->p_caster->GetItemInterface()->GetInventoryItem( INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_MAINHAND );
			if( !pMainHand || pMainHand->GetProto()->Class != 2 || pMainHand->GetProto()->SubClass != 15 )
				return SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND;
		}

		return SPELL_CANCAST_OK;
	}
};

class Stealth : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Stealth);
	Stealth(Spell * pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (_spell->u_caster == nullptr || _spell->u_caster->CombatStatus.IsInCombat())
			return SPELL_FAILED_TARGET_IN_COMBAT;

		if (_spell->p_caster != nullptr && _spell->p_caster->m_bgHasFlag)
			return SPELL_FAILED_SPELL_UNAVAILABLE;

		return SPELL_CANCAST_OK;
	}
};

class Vanish : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Vanish);
	Vanish(Spell * pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (_spell->p_caster != nullptr && _spell->p_caster->m_bgHasFlag)
			return SPELL_FAILED_SPELL_UNAVAILABLE;

		return SPELL_CANCAST_OK;
	}
};

class Garrote : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Garrote);
	Garrote(Spell * pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit *  target, int32* value)
	{
		// WoWWiki says +( 0.18 * attack power / number of ticks )
		// Tooltip gives no specific reading, but says ", increased by your attack power.".
		if( _spell->u_caster != nullptr )
		{
			float ap = (float)_spell->u_caster->GetAP();
			if( EffectIndex == 0 )
			{
				*value += (uint32) ceilf( ( ap * 0.18f ) / 6 );
			}
		}
	}
};

class Rupture : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Rupture);
	Rupture(Spell * pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit *  target, int32* value)
	{
		/* 
		1pt = Attack Power * 0.04 + x
		2pt = Attack Power * 0.10 + y
		3pt = Attack Power * 0.18 + z
		4pt = Attack Power * 0.21 + a
		5pt = Attack Power * 0.24 + b
		*/
		if( _spell->u_caster->IsPlayer() )
		{
			float ap = (float)_spell->u_caster->GetAP();
			float cp = (float)TO_PLAYER(_spell->u_caster)->m_comboPoints;
			*value += (uint32) ceilf( ( ap * ( 0.04f * cp ) ) / ( ( 6 + ( cp * 2 ) ) / 2 ) );
		}
	}
};

class Eviscerate : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Eviscerate);
	Eviscerate(Spell * pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit *  target, int32* value)
	{
		if( _spell->p_caster != nullptr && EffectIndex == 0 )//Envenom
		{
			value += (uint32)( _spell->p_caster->GetAP() * ( 0.03 * _spell->p_caster->m_comboPoints ) );
		}
	}
};

#endif

void SetupRogueSpells(ScriptMgr * mgr)
{
    //mgr->register_dummy_spell(14185, &Preparation);
	
#ifdef ASCENT_CORE
	mgr->register_spell_script(53, &Backstab::Create);
	mgr->register_spell_script(2589, &Backstab::Create);
	mgr->register_spell_script(2590, &Backstab::Create);
	mgr->register_spell_script(2591, &Backstab::Create);
	mgr->register_spell_script(8721, &Backstab::Create);
	mgr->register_spell_script(11279, &Backstab::Create);
	mgr->register_spell_script(11280, &Backstab::Create);
	mgr->register_spell_script(11281, &Backstab::Create);
	mgr->register_spell_script(25300, &Backstab::Create);
	mgr->register_spell_script(8676, &Ambush::Create);
	mgr->register_spell_script(8724, &Ambush::Create);
	mgr->register_spell_script(8725, &Ambush::Create);
	mgr->register_spell_script(11267, &Ambush::Create);
	mgr->register_spell_script(11268, &Ambush::Create);
	mgr->register_spell_script(11269, &Ambush::Create);
	mgr->register_spell_script(1784, &Stealth::Create);
	mgr->register_spell_script(1785, &Stealth::Create);
	mgr->register_spell_script(1786, &Stealth::Create);
	mgr->register_spell_script(1787, &Stealth::Create);
	mgr->register_spell_script(1856, &Vanish::Create);
	mgr->register_spell_script(1857, &Vanish::Create);
	mgr->register_spell_script(703, &Garrote::Create);
	mgr->register_spell_script(8631, &Garrote::Create);
	mgr->register_spell_script(8632, &Garrote::Create);
	mgr->register_spell_script(8633, &Garrote::Create);
	mgr->register_spell_script(11289, &Garrote::Create);
	mgr->register_spell_script(11290, &Garrote::Create);
	mgr->register_spell_script(1943, &Rupture::Create);
	mgr->register_spell_script(8639, &Rupture::Create);
	mgr->register_spell_script(8640, &Rupture::Create);
	mgr->register_spell_script(11273, &Rupture::Create);
	mgr->register_spell_script(11274, &Rupture::Create);
	mgr->register_spell_script(11275, &Rupture::Create);
	mgr->register_spell_script(2098, &Eviscerate::Create);
	mgr->register_spell_script(6760, &Eviscerate::Create);
	mgr->register_spell_script(6761, &Eviscerate::Create);
	mgr->register_spell_script(6762, &Eviscerate::Create);
	mgr->register_spell_script(8623, &Eviscerate::Create);
	mgr->register_spell_script(8624, &Eviscerate::Create);
	mgr->register_spell_script(11299, &Eviscerate::Create);
	mgr->register_spell_script(11300, &Eviscerate::Create);
	mgr->register_spell_script(31016, &Eviscerate::Create);
#endif
}
