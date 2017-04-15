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

#ifdef ASCENT_CORE

class UnstableAffliction : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(UnstableAffliction);
	UnstableAffliction(Spell * pSpell) : SpellScript(pSpell) {}

	void OnDispel(Aura* pDispelledAura, Spell* pDispellingSpell)
	{
		Unit *  caster=pDispelledAura->GetUnitCaster();

		if (pDispellingSpell->u_caster == nullptr || caster == nullptr)
			return;

		Spell* sp=new Spell(pDispellingSpell->u_caster, dbcSpell.LookupEntry(31117), true, nullptr);
		sp->forced_basepoints[0]=pDispelledAura->m_spellProto->EffectBasePoints[0] * 9;
		SpellCastTargets t;
		t.m_target = pDispellingSpell->u_caster;
		sp->prepare(&t);
	}
};

class CurseOfDoom : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(CurseOfDoom);
	CurseOfDoom(Spell * pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit *  u_target = _spell->GetUnitTarget();
		if (u_target != nullptr && u_target->IsPlayer())
			return SPELL_FAILED_TARGET_IS_PLAYER;

		return SPELL_CANCAST_OK;
	}

	void OnExpire()
	{
		Unit *  target=_spell->GetUnitTarget();

		if (target == nullptr || !target->isDead())
			return;

		if (RandomUInt(100) < 20)
			target->CastSpell(target, 18662, true);
	}
};

class LifeTap : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(LifeTap);
	int32 valueforplayer;
	LifeTap(Spell * pSpell) : SpellScript(pSpell) {}

	void CalculateEffect(uint32 EffectIndex, Unit *  target, int32* value)
	{
		if (_spell->u_caster == nullptr)
			return;

		*value += _spell->u_caster->GetSpellDmgBonus(_spell->u_caster, _spell->m_spellInfo, *value, false);

		if (_spell->u_caster->HasAura(18182))
			*value *= 1.1f;
		if (_spell->u_caster->HasAura(18183))
			*value *= 1.2f;

		valueforplayer = *value;
	}

	SpellCastError CanCast(bool tolerate)
	{
		if (_spell->u_caster == nullptr)
			return SPELL_FAILED_BAD_TARGETS;

		if (_spell->u_caster->GetUInt32Value(UNIT_FIELD_HEALTH) <= _spell->CalculateEffect(0, _spell->u_caster))
			return SPELL_FAILED_FIZZLE;

		return SPELL_CANCAST_OK;
	}

	void DummyEffect(uint32 EffectIndex)
	{
		if (_spell->u_caster == nullptr)
			return;

		_spell->u_caster->DealDamage(_spell->u_caster, valueforplayer, 0, 0, _spell->m_spellInfo->Id);
		_spell->u_caster->Energize(_spell->u_caster, _spell->m_spellInfo->Id, valueforplayer, 0);

		//give extra points to player's pet (mana feed)
		if (_spell->p_caster != nullptr && _spell->damage > valueforplayer && _spell->p_caster->GetSummon() != nullptr)
			_spell->u_caster->Energize(_spell->p_caster->GetSummon(), _spell->m_spellInfo->Id, _spell->damage - valueforplayer, 0);
	}
};

class SoulLink : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(SoulLink);

	SoulLink(Spell * pSpell) : SpellScript(pSpell) {}
	DamageSplitTarget splitter;
	Player* source;

	void DummyEffect(uint32 EffectIndex)
	{
		if (_spell->p_caster == nullptr || _spell->p_caster->GetSummon() == nullptr)
			return;

		splitter.m_flatDamageSplit = 0;
		splitter.m_spellId = _spell->m_spellInfo->Id;
		splitter.m_pctDamageSplit = 0.2f;
		splitter.damage_type = 64 | 32 | 16 | 8 | 4 | 2 | 1;
		splitter.creator = (void*)this;
		splitter.m_target = _spell->p_caster->GetSummon()->GetGUID();
		_spell->p_caster->m_damageSplitTargets.push_back(splitter);
		source = _spell->p_caster;
	}

	void OnRemove(Aura* pRemovedAura)
	{
		if (_spell->p_caster == nullptr)
			return;

		for (std::list<DamageSplitTarget>::iterator i=source->m_damageSplitTargets.begin(); i!=source->m_damageSplitTargets.end(); ++i)
		{
			if ((*i).creator == this)
			{
				source->m_damageSplitTargets.erase(i);
				break;
			}
		}
	}
};

class DrainLife : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(DrainLife);
	DrainLife(Spell * pSpell) : SpellScript(pSpell) {}

	void CalculateEffect(uint32 EffectIndex, Unit *  target, int32* value)
	{
		Unit *  u_target = _spell->GetUnitTarget();

		if (_spell->u_caster == nullptr || u_target == nullptr)
			return;

		float mod=1.0f;
		
		Aura* aur = nullptr;

		for(uint32 x = MAX_AURAS; x < MAX_PASSIVE_AURAS; ++x)
		{
			if (u_target->m_auras[x] != nullptr && u_target->m_auras[x]->m_spellProto->NameHash == SPELL_HASH_SOUL_SIPHON)
				aur = u_target->m_auras[x];
		}

		if (aur == nullptr)
			return;

		for(int i = MAX_POSITIVE_AURAS; i <= MAX_AURAS; i++)
			if(u_target->m_auras[i] && objmgr.GetSpellSkill(u_target->m_auras[i]->GetSpellId())->Id == SKILL_AFFLICTION)
					mod += aur->m_spellProto->EffectBasePoints[0] /100;

		if (mod > aur->m_spellProto->EffectBasePoints[1])
			mod = aur->m_spellProto->EffectBasePoints[1];

		*value *= mod;
	}
};

class WarlockPetScaling01 : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(WarlockPetScaling01);
	WarlockPetScaling01(Spell * pSpell) : SpellScript(pSpell) {}

	void OnCast()
	{

		if (_spell->u_caster == nullptr || !_spell->u_caster->IsPet())
			return;

		Pet* pet=static_cast<Pet*>(_spell->u_caster);

		if (pet->GetOwner() == nullptr)
			return;

		uint32 MaxSchoolBonus=0;
		for (int32 i=0; i<7; i++)
			if (pet->GetOwner()->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i) > MaxSchoolBonus)
				MaxSchoolBonus = pet->GetOwner()->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i);

		_spell->forced_basepoints[0]=pet->GetOwner()->GetUInt32Value(UNIT_FIELD_STAT2) * 0.3;
		_spell->forced_basepoints[1]=MaxSchoolBonus * 0.57;
		_spell->forced_basepoints[2]=MaxSchoolBonus * 0.15;
	}
};

class Conflagrate : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Conflagrate);
	Conflagrate(Spell * pSpell) : SpellScript(pSpell) {}
	Unit *  u_target;
	SpellCastError CanCast(bool tolerate)
	{
		u_target = _spell->GetUnitTarget();
		if( u_target == nullptr || !u_target->HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_IMMOLATE))
			return SPELL_FAILED_CASTER_AURASTATE;

		return SPELL_CANCAST_OK;
	}
	void OnEffect(uint32 EffectIndex)
	{
		if( u_target && u_target->HasAurasWithNameHash(SPELL_HASH_IMMOLATE) )
			u_target->RemoveAuraByNameHash(SPELL_HASH_IMMOLATE);
	}
};

class SoulShatter : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(SoulShatter);
	SoulShatter(Spell * pSpell) : SpellScript(pSpell) {}

	void DummyEffect(uint32 EffectIndex)
	{
		if (_spell->GetUnitTarget() == nullptr)
			return;

		_spell->GetUnitTarget()->GetAIInterface()->modThreatByPtr(_spell->u_caster, _spell->GetUnitTarget()->GetAIInterface()->getThreatByPtr(_spell->u_caster) * (_spell->m_spellInfo->EffectBasePoints[EffectIndex] / 100));
	}
};

class DemonicKnowledge : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(DemonicKnowledge);
	int32 amountadded;
	DemonicKnowledge(Spell * pSpell) : SpellScript(pSpell) { amountadded=0; }


	void DummyAura(bool apply, Aura* aura)
	{
		if ( aura->GetTarget()->IsPet() )
		{
			Unit *  PetOwner;
			if ( static_cast< Pet* >( aura->GetTarget() )->GetPetOwner() )
			{
				PetOwner = static_cast< Pet* >( aura->GetTarget() )->GetPetOwner();

				if (amountadded == 0.0f)
				{
					uint32 val1 = aura->GetTarget()->GetUInt32Value( UNIT_FIELD_STAT2 ); // stamina
					uint32 val2 = aura->GetTarget()->GetUInt32Value( UNIT_FIELD_STAT3 ); // intelect
					uint32 val0 = val1+val2;
					float amount = (float)(val0*aura->mod->m_amount)/100;
					amountadded=amount;
				}

				int32 val;

				if( apply )
					val = (int32)amountadded;
				else
					val = (int32)-amountadded;

				for (uint32 x=0;x<7;x++)
					PetOwner->ModUnsigned32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + x, val);
					
				PetOwner->CalcDamage();
			}
		}
	}
};

#endif

void SetupWarlockSpells(ScriptMgr * mgr)
{
#ifdef ASCENT_CORE
	mgr->register_spell_script(30108, &UnstableAffliction::Create);
	mgr->register_spell_script(30404, &UnstableAffliction::Create);
	mgr->register_spell_script(603, &CurseOfDoom::Create);
	mgr->register_spell_script(1454, &LifeTap::Create);
	mgr->register_spell_script(1455, &LifeTap::Create);
	mgr->register_spell_script(1456, &LifeTap::Create);
	mgr->register_spell_script(11687, &LifeTap::Create);
	mgr->register_spell_script(11688, &LifeTap::Create);
	mgr->register_spell_script(11689, &LifeTap::Create);
	mgr->register_spell_script(19028, &SoulLink::Create);
	mgr->register_spell_script(689, &DrainLife::Create);
	mgr->register_spell_script(699, &DrainLife::Create);
	mgr->register_spell_script(709, &DrainLife::Create);
	mgr->register_spell_script(7651, &DrainLife::Create);
	mgr->register_spell_script(11699, &DrainLife::Create);
	mgr->register_spell_script(11700, &DrainLife::Create);
	mgr->register_spell_script(17962, &Conflagrate::Create);
	mgr->register_spell_script(18930, &Conflagrate::Create);
	mgr->register_spell_script(18931, &Conflagrate::Create);
	mgr->register_spell_script(18932, &Conflagrate::Create);
	mgr->register_spell_script(27266, &Conflagrate::Create);
	mgr->register_spell_script(30912, &Conflagrate::Create);
	mgr->register_spell_script(29858, &SoulShatter::Create);
	mgr->register_spell_script(35696, &DemonicKnowledge::Create);
#endif
}

