/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009-2010 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"
#include "Instance_ScarletMonastery.h"

/* todo:	Secret door for fairbanks should be opened by touching the torch, not door itself
			Mograine mass aggro
			Sleep can be resisted (also small range?)

			Light--Bug list
			collision doesnt work

			Scarlet Diviner Buffs you with '' Divine spirit''

			'' Scarlet gallant '' . Not Scripted.

			Scarlet Adept '' Heals you , Not theirseves ''

			Hound master Loksey , no Hound around him? not scripted.
*/

#define Location Coords
#define x mX
#define y mY
#define z mZ
#define o mO

/* Trash */

class AnguishedDeadAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(AnguishedDeadAI, MoonScriptCreatureAI); 
	AnguishedDeadAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_VEIL_OF_SHADOW, Target_Current, 30, 1.5, 10);
	}
};

class HauntingPhantasmAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(HauntingPhantasmAI, MoonScriptCreatureAI); 
	HauntingPhantasmAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_SUMMON_PHANTASM, Target_Self, 30, 2, 30);
	}
};

class ScarletAbbotAI:public MoonScriptCreatureAI
{
	SpellDesc *frenzy;
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletAbbotAI, MoonScriptCreatureAI); 
	ScarletAbbotAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		frenzy = AddSpell(SM_FRENZY, Target_Self, 0, 0, 0);
		AddSpell(SM_HEAL4, Target_WoundedFriendly, 30, 3, 10);
		AddSpell(SM_RENEW, Target_WoundedFriendly, 30, 2, 10);
		ApplyAura(SM_INNER_FIRE4);
	}

	void OnCombatStart(Unit *pTarget) {
		CastSpellNowNoScheduling(frenzy);
		ParentClass::OnCombatStart(pTarget);
	}
};

class ScarletAdeptAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletAdeptAI, MoonScriptCreatureAI); 
	ScarletAdeptAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_HEAL3, Target_WoundedFriendly, 30, 3, 10);
		AddSpell(SM_HOLY_SMITE, Target_Current, 30, 2.5, 0);
	}
};

class ScarletBeastmasterAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletBeastmasterAI, MoonScriptCreatureAI); 
	ScarletBeastmasterAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_EXPLODING_SHOT, Target_RandomPlayer, 15, 1, 15);
		AddSpell(SM_SHOOT, Target_Current, 50, 0, 0);
	}
};

class ScarletCenturionAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletCenturionAI, MoonScriptCreatureAI); 
	ScarletCenturionAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_BATTLE_SHOUT, Target_Self, 15, 1, 15);
	}
};

class ScarletChampionAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletChampionAI, MoonScriptCreatureAI); 
	ScarletChampionAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_HOLY_SMITE, Target_Current, 30, 1, 5);
	}
};

class ScarletChaplainAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletChaplainAI, MoonScriptCreatureAI); 
	ScarletChaplainAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_RENEW, Target_WoundedFriendly, 30, 2, 10);
		AddSpell(SM_PWSHIELD6, Target_WoundedFriendly, 30, 2, 15);
		ApplyAura(SM_INNER_FIRE4);
	}
};

class ScarletConjurorAI:public MoonScriptCreatureAI
{
	SpellDesc *summon;
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletConjurorAI, MoonScriptCreatureAI); 
	ScarletConjurorAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_FIREBALL, Target_Current, 60, 3, 0);
		summon = AddSpell(SM_SUMMON_FIRE_ELE, Target_Self, 0, 0, 0);
		CastSpell(summon);
	}
};

class ScarletDefenderAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletDefenderAI, MoonScriptCreatureAI); 
	ScarletDefenderAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_IMPROVED_BLOCKING, Target_Self, 30, 0, 15);
		AddSpell(SM_SHIELD_BASH, Target_Current, 30, 0, 10);
		ApplyAura(SM_DEFENSIVE_STANCE);
	}
};

class ScarletDivinerAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletDivinerAI, MoonScriptCreatureAI); 
	ScarletDivinerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_FIREBALL, Target_Current, 60, 3, 0);
	}
};

class ScarletEvokerAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletEvokerAI, MoonScriptCreatureAI); 
	ScarletEvokerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_FIREBALL, Target_Current, 60, 3, 0);
		AddSpell(SM_FIRE_SHIELD3, Target_Self, 15, 1, 30);
		AddSpell(SM_FLAMESTRIKE, Target_RandomDestination, 15, 3, 15);
	}
};

class ScarletGallantAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletGallantAI, MoonScriptCreatureAI); 
	ScarletGallantAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_CRUSADER_STRIKE, Target_Current, 30, 0, 4);
		AddSpell(SM_HAMMER_OF_JUSTICE, Target_Current, 15, 1, 30);
	}
};

class ScarletGuardsmanAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletGuardsmanAI, MoonScriptCreatureAI); 
	ScarletGuardsmanAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_DISARM, Target_Current, 30, 0, 8);
		ApplyAura(SM_DEFENSIVE_STANCE);
	}
};

class ScarletMonkAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletMonkAI, MoonScriptCreatureAI); 
	ScarletMonkAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_KICK, Target_Current, 30, 0, 8);
		AddSpell(SM_THRASH, Target_Self, 15, 0, 15);
	}
};

class ScarletMyrmidonAI:public MoonScriptCreatureAI
{
	SpellDesc *frenzy;
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletMyrmidonAI, MoonScriptCreatureAI); 
	ScarletMyrmidonAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		frenzy = AddSpell(SM_FRENZY, Target_Self, 0, 0, 0);
	}

	void OnCombatStart(Unit *pTarget) {
		CastSpellNowNoScheduling(frenzy);
		ParentClass::OnCombatStart(pTarget);
	}
};

class ScarletProtectorAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletProtectorAI, MoonScriptCreatureAI); 
	ScarletProtectorAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(SM_DEVOTION_AURA);
		AddSpell(SM_HOLY_LIGHT, Target_WoundedFriendly, 30, 2.5, 10);
	}
};

class ScarletScryerAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletScryerAI, MoonScriptCreatureAI); 
	ScarletScryerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_SHADOW_BOLT, Target_Current, 60, 3, 0);
		AddSpell(SM_SLEEP2, Target_RandomPlayerNotCurrent, 15, 1.5, 15);
	}
};

class ScarletSoldierAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletSoldierAI, MoonScriptCreatureAI); 
	ScarletSoldierAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_IMPROVED_BLOCKING, Target_Self, 30, 0, 15);
	}
};

class ScarletSorcererAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletSorcererAI, MoonScriptCreatureAI); 
	ScarletSorcererAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_FROSTBOLT, Target_Current, 60, 3, 0);
		AddSpell(SM_SLOW2, Target_RandomPlayer, 30, 0, 10);
	}
};

class ScarletWizardAI:public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ScarletWizardAI, MoonScriptCreatureAI); 
	ScarletWizardAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SM_FIRE_SHIELD3, Target_Self, 15, 1, 30);
		AddSpell(SM_ARCANE_EXPLOSION4, Target_Self, 30, 0, 2);
	}
};

/* Rare bosses */

class AzshirAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(AzshirAI, MoonScriptCreatureAI); 
	AzshirAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(AZ_CALL_OF_THE_GRAVE, Target_RandomPlayer, 30, 2, 10);
		AddSpell(AZ_SOUL_SIPHON, Target_Current, 30, 1, 1);
		AddSpell(AZ_TERRIFY, Target_RandomPlayerNotCurrent, 15, 0, 15);
	}
};

class FallenChampionAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(FallenChampionAI, MoonScriptCreatureAI); 
	FallenChampionAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(FC_CLEAVE, Target_Current, 30, 0, 5);
		ApplyAura(FC_BERSERKER_STANCE1);
	}
};

class IronspineAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(IronspineAI, MoonScriptCreatureAI); 
	IronspineAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(IS_CURSE_OF_WEAKNESS, Target_RandomPlayer, 30, 0, 10);
		AddSpell(IS_POISON_CLOUD, Target_Self, 15, 1, 15);
	}
};

/** Scarlet Monastery
	Graveyard Bosses
*/	

class VishasAI : public MoonScriptCreatureAI
{
	bool mSpeechOne;
	bool mSpeechTwo;

public:
	MOONSCRIPT_FACTORY_FUNCTION(VishasAI, MoonScriptCreatureAI); 
	VishasAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(IV_SHADOWWORD_PAIN, Target_RandomPlayer, 20, 0, 4);
	}

	void OnCombatStart(Unit* pTarget)
	{
		mSpeechOne = true;
		mSpeechTwo = true;
		Emote("Tell me...tell me everything.", Text_Yell, 5847);
		ParentClass::OnCombatStart(pTarget);
	}

	void OnTargetDied(Unit* pTarget)
	{
		if (IsAlive())    
		{
			Emote("Purged by pain!", Text_Yell, 5848);
		}
		ParentClass::OnTargetDied(pTarget);
	}
	
	void AIUpdate()
	{
		if(mSpeechOne && GetHealthPercent() < 60)
		{
			mSpeechOne = false;
			Emote("Naughty Secrets!", Text_Yell, 5849);
		}
		if(mSpeechTwo && _unit->GetHealthPct() < 30)
		{
			mSpeechTwo = false;
			Emote("I'll rip the secrets from your flesh!", Text_Yell, 5850);
		}
		ParentClass::AIUpdate();
	}
};

class ThalnosAI : public MoonScriptCreatureAI
{
	bool mSpeechOne;

public:
	MOONSCRIPT_FACTORY_FUNCTION(ThalnosAI, MoonScriptCreatureAI); 
	ThalnosAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BT_FIRE_NOVA, Target_Self, 15, 2, 15);
		AddSpell(BT_FLAME_SHOCK, Target_Current, 15, 0, 6);
		AddSpell(BT_FLAME_SPIKE, Target_RandomDestination, 15, 3, 20);
		AddSpell(BT_SHADOW_BOLT, Target_Current, 60, 3, 0);
	}

	void OnCombatStart(Unit* pTarget)
	{
		mSpeechOne = true;
		Emote("We hunger for vengeance!", Text_Yell, 5844);
		ParentClass::OnCombatStart(pTarget);
	}

	void OnTargetDied(Unit* pTarget)
	{
		if (IsAlive())    
		{
			Emote("More... More souls!", Text_Yell, 5845);
		}
		ParentClass::OnTargetDied(pTarget);
	}
	
	void AIUpdate()
	{
		if(mSpeechOne && GetHealthPercent() < 50)
		{
			mSpeechOne = false;
			Emote("No rest, for the angry dead!", Text_Yell, 5846);
		}
		ParentClass::AIUpdate();
	}
};

/** Scarlet Monastery
	Library Bosses
					**/
class LokseyAI : public MoonScriptCreatureAI
{
	bool Enrage;
	SpellDesc *bloodlust;
public:
	MOONSCRIPT_FACTORY_FUNCTION(LokseyAI, MoonScriptCreatureAI); 
	LokseyAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(HL_BATTLE_SHOUT, Target_Self, 15, 0, 10);
		bloodlust = AddSpell(HL_BLOODLUST, Target_Self, 0, 0, 0);
	}

	void OnCombatStart(Unit* pTarget)
	{
		Enrage = true;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Release the hounds!");
		_unit->PlaySoundToSet(5841);
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate()
	{
		if(Enrage && GetHealthPercent() < 25)
		{
			Enrage = false;
			CastSpell(bloodlust);
		}
		ParentClass::AIUpdate();
	}
}; 

class DoanAI : public MoonScriptCreatureAI
{
	bool mDetonate;
	SpellDesc* Bubble;
	SpellDesc* Detonation;

public:
	MOONSCRIPT_FACTORY_FUNCTION(DoanAI, MoonScriptCreatureAI); 
	DoanAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(AD_ARCANE_EXPLOSION, Target_Self, 30, 0, 5);
		AddSpell(AD_POLYMORPH, Target_SecondMostHated, 20, 1.5, 8);
		AddSpell(AD_SILENCE, Target_Self, 15, 1.5, 10);
		Detonation = AddSpell(AD_DETONATION, Target_Self, 0, 5, 0, 0, 30);
		Bubble = AddSpell(AD_ARCANE_BUBBLE, Target_Self, 0, 0, 0);
	}

	void OnCombatStart(Unit* pTarget)
	{
		mDetonate = true;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will not defile these mysteries!");
		_unit->PlaySoundToSet(5842);
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate()
	{
		if(mDetonate && GetHealthPercent() < 60)
		{
			mDetonate = false;
			CastSpellNowNoScheduling(Bubble);
			CastSpellNowNoScheduling(Detonation);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Burn in righteous fire!");
			_unit->PlaySoundToSet(5843);
		}
		ParentClass::AIUpdate();
	}
}; 

/** Scarlet Monastery
	Armory Boss	**/
#define TRAINEE_WAYPOINTS 6
static Location leftTrainee[TRAINEE_WAYPOINTS] = {
	{1952.163f, -445.795f, 14.101f, 0},
	{1966.263f, -451.595f, 11.272f, 0},
	{1977.005f, -447.240f, 11.272f, 0},
	{1982.505f, -431.090f, 11.272f, 0},
	{1964.005f, -417.465f, 06.177f, 0},
	{1964.894f, -431.772f, 06.177f, 0},
};

static Location rightTrainee[TRAINEE_WAYPOINTS] = {
	{1952.519f, -417.465f, 14.100f, 0},
	{1965.870f, -412.948f, 11.272f, 0},
	{1978.719f, -417.861f, 11.272f, 0},
	{1982.505f, -431.090f, 11.272f, 0},
	{1964.005f, -445.971f, 06.177f, 0},
	{1964.894f, -431.772f, 06.177f, 0},
};

class TraineeAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(TraineeAI, MoonScriptCreatureAI); 
	TraineeAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
};

class HerodAI : public MoonScriptCreatureAI
{
	SpellDesc* Charge;
	SpellDesc* Enrage;
	SpellDesc* WhirlWind;
	int32 mWhirldWindTimer;
	int mUnrootTimer;
	bool mCasting;
	bool mEnrage;

public:
	MOONSCRIPT_FACTORY_FUNCTION(HerodAI, MoonScriptCreatureAI); 
	HerodAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		WhirlWind = AddSpell(HEROD_WHIRLWIND, Target_Self, 0, 10, 0, 0, 5, false, "Blades of light!", Text_Yell, 5832);
		Charge = AddSpell(HEROD_RUSHING_CHARGE, Target_Current, 0, 0, 0);
		Enrage = AddSpell(HEROD_FRENZY, Target_Self, 0, 0, 0, 0, 0, false, "Light, give me strength!", Text_Yell, 5833);
		AddSpell(HEROD_CLEAVE, Target_Current, 30, 0, 5);
	}

	void OnCombatStart(Unit* pTarget)
	{
		mCasting = false;
		mEnrage = true;
		mUnrootTimer = 0;
		mWhirldWindTimer = AddTimer(10000);
		Emote("Ah, I've been waiting for a real challenge!", Text_Yell, 5830);
		CastSpellNowNoScheduling(Charge);
		ParentClass::OnCombatStart(pTarget);
	}

	void OnTargetDied(Unit* pTarget)
	{
		if(IsAlive()) {
			Emote("Ha, is that all?", Text_Yell, 5831);
		}
		ParentClass::OnTargetDied(pTarget);
	}

	void OnDied(Unit* pKiller)
	{
		MoonScriptCreatureAI *trainee;

		int i;
		int j;
		for(i=1 ; i<=10; i++) {
			trainee = SpawnCreature(CN_SCARLET_TRAINEE, 1947.026+RandomFloat(10)-10.0f, -437.272+RandomFloat(10)-10.0f, 16.5);
			if(trainee) {
				for(j=0; j<TRAINEE_WAYPOINTS; j++) {
					trainee->AddWaypoint(CreateWaypoint(j+1, 0, Flag_Run, leftTrainee[j]));
				}
			}
			trainee = SpawnCreature(CN_SCARLET_TRAINEE, 1947+RandomFloat(10)-10, -426+RandomFloat(10)-10, 16.5);
			if(trainee) {
				for(j=0; j<TRAINEE_WAYPOINTS; j++) {
					trainee->AddWaypoint(CreateWaypoint(j+1, 0, Flag_Run, rightTrainee[j]));
				}
			}
			if(trainee)
				trainee->Emote("The master has fallen! Avenge him, my brethen!", Text_Yell, 5834);
		}
		ParentClass::OnDied(pKiller);
	}
	void AIUpdate()
	{
		if(IsTimerFinished(mWhirldWindTimer))
		{
			ResetTimer(mWhirldWindTimer, 17000 + RandomUInt(20000));
			CastSpell(WhirlWind);
			_unit->Root();
			mUnrootTimer = 11;
			mCasting = true;
		}
		mUnrootTimer--;
		if(mCasting && !mUnrootTimer)
		{
			_unit->Unroot();
			mCasting = false;
		}
		if(mEnrage && GetHealthPercent() < 40)
		{
			mEnrage = false;
			CastSpell(Enrage);
		}
		ParentClass::AIUpdate();
	}

	void OnCombatStop(Unit *pTarget) {
		//remove enrage if group wipes
		RemoveAllAuras();
		ParentClass::OnCombatStop(pTarget);
	}
}; 

/** Scarlet Monastery
	Cathedral Bosses **/

class FairbanksAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(FairbanksAI, MoonScriptCreatureAI); 
	FairbanksAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(HIF_CURSE_OF_BLOOD, Target_RandomPlayer, 7, 2, 5);
		AddSpell(HIF_DISPEL_MAGIC, Target_RandomPlayer, 9, 0, 10);
		AddSpell(HIF_FEAR, Target_RandomPlayerNotCurrent, 8, 1.5, 10);
		AddSpell(HIF_HEAL, Target_Self, 7, 3, 5);
		AddSpell(HIF_POWERWORD_SHIELD, Target_Self, 10, 0, 15);
		AddSpell(HIF_SLEEP, Target_RandomPlayer, 7, 1.3f, 15);
	}
};

class MograineAI : public MoonScriptCreatureAI
{
	bool mShield;
	int phase;
	SpellDesc* DivineShield;
	SpellDesc* RetriAura;

public:
	MOONSCRIPT_FACTORY_FUNCTION(MograineAI, MoonScriptCreatureAI); 
	MograineAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SCM_CRUSADER_STRIKE, Target_Current, 30, 0, 5);
		AddSpell(SCM_HAMMER_OF_JUSTICE, Target_Current, 15, 0, 10);
		DivineShield = AddSpell(SCM_DIVINE_SHIELD, Target_Current, 0, 0, 0);
		ApplyAura(SCM_RETRIBUTION_AURA);
	}

	void SetPhase(int newphase) {
		phase = newphase;
	}

	int GetPhase() {
		return phase;
	}

	void OnCombatStart(Unit* pTarget)
	{
		mShield = true;
		//phase resets when after on combat start, so we will do this like that.
		if(phase != 2) {
			Emote("Infidels. They must be purified!", Text_Yell, 5835);
		}
		else {
			Emote("At your side, my lady!", Text_Yell, 5837);
		}
		ParentClass::OnCombatStart(pTarget);
	}

	void OnCombatStop(Unit *pTarget)
	{
		if(IsAlive()) {
			Creature *whitemane = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1173.01f, 1389.91f, 31.9723f, BOSS_HI_WHITEMANE);
			if(whitemane)
				whitemane->Despawn(0, 0);
		}
		ParentClass::OnCombatStop(pTarget);
	}

	void OnTargetDied(Unit* pTarget)
	{
		Emote("Unworthy.", Text_Yell, 5836);
		ParentClass::OnTargetDied(pTarget);
	}

	void AIUpdate()
	{
		if(mShield && GetHealthPercent() < 10)
		{
			mShield = false;
			CastSpell(DivineShield);
		}
		ParentClass::AIUpdate();
	}

	void OnDied(Unit *pKiller) {
		if(phase != 2) {
			GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1173.01f, 1389.91f, 31.9723f, 104600);
			if(door)
				door->SetState(0);

			MoonScriptCreatureAI *whitemane = SpawnCreature(BOSS_HI_WHITEMANE, 1202.0f, 1399.0f, 29.1f);
			if(whitemane) {
				Location waypoint1 = {1168.194f, 1398.985f, 32.0f, 3.15f};
				Location waypoint2;
				waypoint2.x = _unit->GetPositionX();
				waypoint2.y = _unit->GetPositionY();
				waypoint2.z = _unit->GetPositionZ();
				if(whitemane) {
					whitemane->AddWaypoint(CreateWaypoint(1, 0, Flag_Run, waypoint1));
					whitemane->AddWaypoint(CreateWaypoint(2, 0, Flag_Run, waypoint2));
				}
			}
			if(whitemane) {
				whitemane->Emote("Mograine has fallen? You shall pay for this treachery!", Text_Yell, 5838);
			}
//			Set Mograine not selectable (e.g. not lootable)
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			
		}
		ParentClass::OnDied(pKiller);
	}
};

// High Inquisitor Whitemane
class WhitemaneAI : public MoonScriptCreatureAI
{
	bool ressurect;
	bool mograine_shout;
	int timer;
	SpellDesc *Sleep;
	SpellDesc *RessurectSpell;

public:
	MOONSCRIPT_FACTORY_FUNCTION(WhitemaneAI, MoonScriptCreatureAI); 
	WhitemaneAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		Sleep = AddSpell(HIW_DEEP_SLEEP, Target_Self, 0, 0, 0);
		RessurectSpell = AddSpell(HIW_SCARLET_RESURRECTION, Target_Self, 0, 8, 0);
		AddSpell(HIW_DOMINATE_MIND, Target_RandomPlayerNotCurrent, 7, 2, 17);
		AddSpell(HIW_HEAL, Target_WoundedFriendly, 8, 3, 5);
		AddSpell(HIW_HOLY_SMITE, Target_Current, 15, 2.5, 1);
		AddSpell(HIW_POWERWORD_SHIELD, Target_WoundedFriendly, 7, 0, 25);
	}

	void OnCombatStart(Unit* pTarget) {
		ressurect = true;
		mograine_shout = true;
		timer = INVALIDATE_TIMER;
		ParentClass::OnCombatStart(pTarget);
	}

	void OnReachWP(uint32 wpid, bool forward) {
		if(wpid == 2) {
			StopWaypointMovement();
			AggroNearestPlayer(1);
		}
		ParentClass::OnReachWP(wpid, forward);
	}

	void AIUpdate()
	{
		if(IsTimerFinished(timer)) {
			timer = INVALIDATE_TIMER;
			Creature *mograine1 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1173.01f, 1389.91f, 31.9723f, BOSS_SC_MOGRAINE);

			if(mograine1) {
				MograineAI *mograine = static_cast<MograineAI*>(mograine1->GetScript());
				mograine->SetPhase(2);
				mograine1->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
				mograine->Despawn(0, 1);
			}
		}
		if(ressurect && GetHealthPercent() < 50)
		{
			ressurect = false;

			timer = AddTimer(10000);
			CastSpellNowNoScheduling(Sleep);
			Emote("Arise, my champion!", Text_Yell, 5840);
			CastSpell(RessurectSpell);
		}
		ParentClass::AIUpdate();
	}

	void OnTargetDied(Unit* pTarget)
	{
		Emote("The light has spoken!", Text_Yell, 5839);
		ParentClass::OnTargetDied(pTarget);
	}
};

void SetupScarletMonastery(ScriptMgr * mgr)
{
    mgr->register_creature_script(BOSS_HOUNDMASTER_LOKSEY, &LokseyAI::Create);
	mgr->register_creature_script(BOSS_ARCANIST_DOAN, &DoanAI::Create);
	mgr->register_creature_script(BOSS_INTERROGATOR_VISHAS, &VishasAI::Create);
	mgr->register_creature_script(BOSS_BLOODMAGE_THALNOS, &ThalnosAI::Create);
	mgr->register_creature_script(BOSS_HEROD, &HerodAI::Create); 
	mgr->register_creature_script(CN_SCARLET_TRAINEE, &TraineeAI::Create); 
	mgr->register_creature_script(BOSS_HI_FAIRBANKS, &FairbanksAI::Create);
	mgr->register_creature_script(BOSS_SC_MOGRAINE, &MograineAI::Create);
	mgr->register_creature_script(BOSS_HI_WHITEMANE, &WhitemaneAI::Create);

	mgr->register_creature_script(SM_ANGUISHED_SHADOW, &AnguishedDeadAI::Create);
	mgr->register_creature_script(SM_HAUNTING_PHANTASM, &HauntingPhantasmAI::Create);
	mgr->register_creature_script(SM_SCARLET_ABBOT, &ScarletAbbotAI::Create);
	mgr->register_creature_script(SM_SCARLET_ADEPT, &ScarletAdeptAI::Create);
	mgr->register_creature_script(SM_SCARLET_BEASTMASTER, &ScarletBeastmasterAI::Create);
	mgr->register_creature_script(SM_SCARLET_CENTURION, &ScarletCenturionAI::Create);
	mgr->register_creature_script(SM_SCARLET_CHAMPION, &ScarletChampionAI::Create);
	mgr->register_creature_script(SM_SCARLET_CHAPLAIN, &ScarletChaplainAI::Create);
	mgr->register_creature_script(SM_SCARLET_CONJUROR, &ScarletConjurorAI::Create);
	mgr->register_creature_script(SM_SCARLET_DEFENDER, &ScarletDefenderAI::Create);
	mgr->register_creature_script(SM_SCARLET_DIVINER, &ScarletDivinerAI::Create);
	mgr->register_creature_script(SM_SCARLET_EVOKER, &ScarletEvokerAI::Create);
	mgr->register_creature_script(SM_SCARLET_GALLANT, &ScarletGallantAI::Create);
	mgr->register_creature_script(SM_SCARLET_GUARDSMAN, &ScarletGuardsmanAI::Create);
	mgr->register_creature_script(SM_SCARLET_MONK, &ScarletMonkAI::Create);
	mgr->register_creature_script(SM_SCARLET_MYRMIDON, &ScarletMyrmidonAI::Create);
	mgr->register_creature_script(SM_SCARLET_PROTECTOR, &ScarletProtectorAI::Create);
	mgr->register_creature_script(SM_SCARLET_SCRYER, &ScarletScryerAI::Create);
	mgr->register_creature_script(SM_SCARLET_SOLDIER, &ScarletSoldierAI::Create);
	mgr->register_creature_script(SM_SCARLET_SORCERER, &ScarletSorcererAI::Create);
	mgr->register_creature_script(SM_SCARLET_WIZARD, &ScarletWizardAI::Create);
	mgr->register_creature_script(RARE_AZSHIR_THE_SLEEPLESS, &AzshirAI::Create);
	mgr->register_creature_script(RARE_FALLEN_CHAMPION, &FallenChampionAI::Create);
	mgr->register_creature_script(RARE_IRONSPINE, &IronspineAI::Create);
}