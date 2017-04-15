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

bool ColdSnap(uint32 i, Spell * pSpell)
{
    if(!pSpell->p_caster) return true;
    pSpell->p_caster->ClearCooldownsOnLine(6, pSpell->m_spellInfo->Id);
    return true;
}

#ifdef ASCENT_CORE

class IceBlock : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(IceBlock);
	IceBlock(Spell * pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (_spell->u_caster == nullptr || _spell->u_caster->HasAura(41425))
			return SPELL_FAILED_DAMAGE_IMMUNE;

		return SPELL_CANCAST_OK;
	}

	void OnCast()
	{
		if (_spell->u_caster != nullptr)
			_spell->u_caster->CastSpell(_spell->u_caster, 41425, true);
	}
};

#endif

void SetupMageSpells(ScriptMgr * mgr)
{
    mgr->register_dummy_spell(11958, &ColdSnap);
#ifdef ASCENT_CORE
	mgr->register_spell_script(45438, &IceBlock::Create);
#endif
}
