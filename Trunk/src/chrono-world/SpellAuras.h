/*
 * Chrono Emulator
 * Copyright (C) 2010 ChronoEmu Team <http://www.forsakengaming.com/>
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

//! 4-bit flag
enum AURA_FLAGS
{
	AFLAG_EMPTY 			= 0x00,
    AFLAG_VISIBLE           = 0x01,
    AFLAG_EFF_INDEX_1       = 0x02,
    AFLAG_EFF_INDEX_2       = 0x04,
    AFLAG_NOT_GUID          = 0x08,
	AFLAG_SET 				= 0x09,
    AFLAG_CANCELLABLE       = 0x10,
    AFLAG_HAS_DURATION      = 0x20,
    AFLAG_UNK2              = 0x40,
    AFLAG_NEGATIVE          = 0x80,
    AFLAG_POSITIVE          = 0x1F,
    AFLAG_MASK              = 0xFF
};

enum AURAIMMUNE_FLAG
{
	AURAIMMUNE_NONE				= 0x0,
	AURAIMMUNE_CONFUSE			= 0X1,			//SPELL_AURA_MOD_CONFUSE = 5
	AURAIMMUNE_CHARM			= 0x2,			//SPELL_AURA_MOD_CHARM = 6
	AURAIMMUNE_FEAR				= 0X4,			//SPELL_AURA_MOD_FEAR = 7
	AURAIMMUNE_STUN				= 0x8,			//SPELL_AURA_MOD_STUN = 12
	AURAIMMUNE_PACIFY			= 0x10,			//SPELL_AURA_MOD_PACIFY = 25
	AURAIMMUNE_ROOT				= 0x20,			//SPELL_AURA_MOD_ROOT = 26	
	AURAIMMUNE_SILENCE			= 0x40,			//SPELL_AURA_MOD_SILENCE = 27
	AURAIMMUNE_INCSPEED			= 0x80,         //SPELL_AURA_MOD_INCREASE_SPEED = 31
	AURAIMMUNE_DECSPEED			= 0x100,		//SPELL_AURA_MOD_DECREASE_SPEED = 33
	AURAIMMUNE_TRANSFORM		= 0x200,		//SPELL_AURA_TRANSFORM = 56
	AURAIMMUNE_TAUNT			= 0x400,		//SPELL_AURA_MOD_TAUNT = 11
	AURAIMMUNE_BLEED			= 0x800,		//MECHANIC BLEED = 15
	AURAIMMUNE_POISON			= 0x1000,		//DISPEL TYPE POISON = 4
	AURAIMMUNE_MANALEECH		= 0x2000,		//SPELL_AURA_PERIODIC_MANA_LEECH = 64,
	AURAIMMUNE_LEECH			= 0x4000,		//SPELL_AURA_PERIODIC_LEECH = 53

	AURAIMMUNE_GENERIC_BOSS     = AURAIMMUNE_CONFUSE | AURAIMMUNE_CHARM | AURAIMMUNE_FEAR |
								  AURAIMMUNE_STUN | AURAIMMUNE_PACIFY | AURAIMMUNE_ROOT |
								  AURAIMMUNE_SILENCE | AURAIMMUNE_INCSPEED | AURAIMMUNE_DECSPEED |
								  AURAIMMUNE_TRANSFORM, //1023
};

enum AUARA_STAE_FLAGS
{
	AURASTATE_FLAG_DODGE_BLOCK			= 1,        //1
    AURASTATE_FLAG_HEALTH20             = 2,        //2
    AURASTATE_FLAG_BERSERK              = 4,        //3
    AURASTATE_FLAG_JUDGEMENT            = 16,       //5
    AURASTATE_FLAG_PARRY                = 64,       //7
    AURASTATE_FLAG_LASTKILLWITHHONOR    = 512,      //10
    AURASTATE_FLAG_CRITICAL             = 1024,     //11
    AURASTATE_FLAG_HEALTH35             = 4096,     //13
    AURASTATE_FLAG_IMMOLATE             = 8192,     //14
    AURASTATE_FLAG_REJUVENATE           = 16384,    //15 //where do i use this ?
    AURASTATE_FLAG_POISON               = 32768,    //16
};

enum AuraTickFlags
{
    FLAG_PERIODIC_DAMAGE            = 2,
    FLAG_PERIODIC_TRIGGER_SPELL     = 4,
    FLAG_PERIODIC_HEAL              = 8,
    FLAG_PERIODIC_LEECH             = 16,
    FLAG_PERIODIC_ENERGIZE          = 32,
};

struct Modifier
{
//    uint32 m_actamt;       // actual amt, for percent and stuff
    uint32 m_type;           // What does it modify? (str,int,hp)
    int32 m_amount;          // By how much does it mod?
    int32 m_miscValue;       // Misc Value
    uint32 i;

    ///needed for per level effect
    int32 realamount;
    
	//need this to store % values or they cannot be reverted corectly (i think :D )
    int32 fixed_amount[7];

	// float fixed amounts
	float fixed_float_amount[7];
};


struct ProcTriggerSpellOnSpell
{
    uint32 origId;
    uint32 spellId;
    uint64 caster;
    uint32 procChance;
    uint32 procFlags;
    uint32 RemainingCharges;
    uint32 LastTrigger;
    void  *owner;//mark the owner of this proc to know which one to delete
};

struct DamageProc
{
    uint32 m_spellId;
    uint32 m_damage;
  //  uint64 m_caster;//log is: some reflects x arcane/nature damage to 'attacker' no matter who casted
    uint32 m_school;
    uint32 m_flags;
    void  *owner;//mark the owner of this proc to know which one to delete
};

struct ProcTriggerSpell
{
	uint32 origId;
	uint32 spellId;
	uint64 caster;
	uint32 procChance;
	uint32 procFlags;
	uint32 procCharges;
	uint32 LastTrigger;
	uint32 ProcType; //0=triggerspell/1=triggerclassspell
	bool deleted;
};

struct SpellCharge
{
    uint32 spellId;
    uint32 count;
    uint32 ProcFlag;
    uint32 lastproc;
    uint32 procdiff;
};

typedef set<uint64> AreaAuraList;

class SERVER_DECL Aura : public EventableObject
{
	uint64 periodic_target;
public:
    Aura(SpellEntry *proto, int32 duration,Object* caster, Unit *target);
	~Aura();

	bool m_wasremoved;
	
	SpellScript* m_spellScript;

    void Remove(bool del = true);
    void Expire();
    void AddMod(uint32 t, int32 a,uint32 miscValue,uint32 i);

    CHRONO_INLINE SpellEntry* GetSpellProto() const { return m_spellProto; }
    CHRONO_INLINE uint32 GetSpellId() const {return m_spellProto->Id; }
    CHRONO_INLINE bool IsPassive(){ if(!m_spellProto) return false; return (m_spellProto->Attributes & ATTRIBUTES_PASSIVE && !m_areaAura);}

    CHRONO_INLINE int32 GetDuration() const { return m_duration; }
    void SetDuration(int32 duration) { m_duration = duration; }

    CHRONO_INLINE uint8 GetAuraSlot() const { return m_auraSlot; }
	void SetAuraSlot(uint8 slot) { m_auraSlot = slot; }

	CHRONO_INLINE bool IsPositive() { return m_positive>0; }
	void SetNegative(signed char value=1) { m_positive -= value; }
	void SetPositive(signed char value=1) { m_positive += value; }

	Object* GetCaster();
	CHRONO_INLINE uint64 GetCasterGUID(){return m_casterGuid;}
	Unit* GetUnitCaster();
	CHRONO_INLINE Unit* GetTarget() { return m_target; }

	Aura* StrongerThat(Aura *aur);
	void ApplyModifiers(bool apply);
	void AddAuraVisual();
	void UpdateDuration();
	void RemoveAuraVisual();
	void EventUpdateAA(float r);
	void RemoveAA();
	void AttemptDispel(Unit* pCaster, bool canResist = true);
	bool m_dispelled;
		

	CHRONO_INLINE uint32 GetTimeLeft()//in sec
	{
		if(m_duration==-1)return (uint32)-1;
		int32 n=int32((UNIXTIME-time_t(timeleft))*1000);
		if(n>=m_duration) return 0;
		else 
		return (m_duration-n);
	}

		// Aura Handlers
	void SpellAuraNULL(bool apply);
	void SpellAuraBindSight(bool apply);
	void SpellAuraModPossess(bool apply);
	void SpellAuraPeriodicDamage(bool apply);
	void SpellAuraDummy(bool apply);
	void SpellAuraModConfuse(bool apply);
	void SpellAuraModCharm(bool apply);
	void SpellAuraModFear(bool apply);
	void SpellAuraPeriodicHeal(bool apply);
	void SpellAuraModAttackSpeed(bool apply);
	void SpellAuraModThreatGenerated(bool apply);
	void SpellAuraModTaunt(bool apply);
	void SpellAuraModStun(bool apply);
	void SpellAuraModDamageDone(bool apply);
	void SpellAuraModDamageTaken(bool apply);
	void SpellAuraDamageShield(bool apply);
	void SpellAuraModStealth(bool apply);
	void SpellAuraModDetect(bool apply);
	void SpellAuraModInvisibility(bool apply);
	void SpellAuraModInvisibilityDetection(bool apply);
	void SpellAuraModTotalHealthRegenPct(bool apply);
	void SpellAuraModTotalManaRegenPct(bool apply);
	void SpellAuraModResistance(bool apply);
	void SpellAuraPeriodicTriggerSpell(bool apply);
	void SpellAuraPeriodicEnergize(bool apply);
	void SpellAuraModPacify(bool apply);
	void SpellAuraModRoot(bool apply);
	void SpellAuraModSilence(bool apply);
	void SpellAuraReflectSpells(bool apply);
	void SpellAuraModStat(bool apply);
	void SpellAuraModSkill(bool apply);
	void SpellAuraModIncreaseSpeed(bool apply);
	void SpellAuraModDecreaseSpeed(bool apply);
	void SpellAuraModIncreaseHealth(bool apply);
	void SpellAuraModIncreaseEnergy(bool apply);
	void SpellAuraModShapeshift(bool apply);
	void SpellAuraModEffectImmunity(bool apply);
	void SpellAuraModStateImmunity(bool apply);
	void SpellAuraModSchoolImmunity(bool apply);
	void SpellAuraModDmgImmunity(bool apply);
	void SpellAuraModDispelImmunity(bool apply);
	void SpellAuraProcTriggerSpell(bool apply);
	void SpellAuraProcTriggerDamage(bool apply);
	void SpellAuraTrackCreatures(bool apply);
	void SpellAuraTrackResources(bool apply);	
	void SpellAuraModParryPerc(bool apply);	
	void SpellAuraModDodgePerc(bool apply);	
	void SpellAuraModBlockPerc(bool apply);
	void SpellAuraModCritPerc(bool apply);
	void SpellAuraPeriodicLeech(bool apply);
	void SpellAuraModHitChance(bool apply);
	void SpellAuraModSpellHitChance(bool apply);
	void SpellAuraTransform(bool apply);
	void SpellAuraModSpellCritChance(bool apply);
	void SpellAuraIncreaseSwimSpeed(bool apply);	
	void SpellAuraModCratureDmgDone(bool apply);
	void SpellAuraPacifySilence(bool apply);
	void SpellAuraModScale(bool apply);
	void SpellAuraPeriodicHealthFunnel(bool apply);	
	void SpellAuraPeriodicManaLeech(bool apply);
	void SpellAuraModCastingSpeed(bool apply);
	void SpellAuraFeignDeath(bool apply);
	void SpellAuraModDisarm(bool apply);
	void SpellAuraModStalked(bool apply);
	void SpellAuraSchoolAbsorb(bool apply);	
	void SpellAuraModSpellCritChanceSchool(bool apply);
	void SpellAuraModPowerCost(bool apply);
	void SpellAuraModPowerCostSchool(bool apply);
	void SpellAuraReflectSpellsSchool(bool apply);
	void SpellAuraModLanguage(bool apply);
	void SpellAuraAddFarSight(bool apply);
	void SpellAuraMechanicImmunity(bool apply);
	void SpellAuraMounted(bool apply);
	void SpellAuraModDamagePercDone(bool apply);
	void SpellAuraModPercStat(bool apply);
	void SpellAuraSplitDamage(bool apply);
	void SpellAuraWaterBreathing(bool apply);
	void SpellAuraModBaseResistance(bool apply);
	void SpellAuraModRegen(bool apply);
	void SpellAuraModPowerRegen(bool apply);
	void SpellAuraModHealingPct(bool apply);
	void SpellAuraChannelDeathItem(bool apply);
	void SpellAuraModDamagePercTaken(bool apply);
	void SpellAuraModRegenPercent(bool apply);
	void SpellAuraPeriodicDamagePercent(bool apply);
	void SpellAuraModResistChance(bool apply);
	void SpellAuraModDetectRange(bool apply);
	void SpellAuraPreventsFleeing(bool apply);
	void SpellAuraModUnattackable(bool apply);
	void SpellAuraInterruptRegen(bool apply);
	void SpellAuraGhost(bool apply);
	void SpellAuraMagnet(bool apply);
	void SpellAuraManaShield(bool apply);
	void SpellAuraSkillTalent(bool apply);
	void SpellAuraModAttackPower(bool apply);
	void SpellAuraVisible(bool apply);
	void SpellAuraModResistancePCT(bool apply);
	void SpellAuraModCreatureAttackPower(bool apply);
	void SpellAuraModTotalThreat(bool apply);
	void SpellAuraWaterWalk(bool apply);
	void SpellAuraFeatherFall(bool apply);
	void SpellAuraHover(bool apply);
	void SpellAuraAddFlatModifier(bool apply);
	void SpellAuraAddPctMod(bool apply);
	void SpellAuraAddTargetTrigger(bool apply);
	void SpellAuraModPowerRegPerc(bool apply);	
	void SpellAuraOverrideClassScripts(bool apply);
	void SpellAuraModRangedDamageTaken(bool apply);	
	void SpellAuraModHealing(bool apply);
	void SpellAuraIgnoreRegenInterrupt(bool apply);
	void SpellAuraModMechanicResistance(bool apply);
	void SpellAuraModHealingPCT(bool apply);   
	void SpellAuraEmphaty(bool apply);
	void SpellAuraModOffhandDamagePCT(bool apply);
	void SpellAuraModPenetration(bool apply);
	void SpellAuraModRangedAttackPower(bool apply);
	void SpellAuraModMeleeDamageTaken(bool apply);
	void SpellAuraModMeleeDamageTakenPct(bool apply);
	void SpellAuraRAPAttackerBonus(bool apply);	
	void SpellAuraModIncreaseSpeedAlways(bool apply);
	void SpellAuraModIncreaseMountedSpeed(bool apply);
	void SpellAuraModCreatureRangedAttackPower(bool apply);
	void SpellAuraModIncreaseEnergyPerc(bool apply);
	void SpellAuraModIncreaseHealthPerc(bool apply);
	void SpellAuraModManaRegInterrupt(bool apply);
	void SpellAuraModHealingDone(bool apply);
	void SpellAuraModHealingDonePct(bool apply);
	void SpellAuraModTotalStatPerc(bool apply);
	void SpellAuraModHaste(bool apply);
	void SpellAuraForceReaction(bool apply);
	void SpellAuraModRangedHaste(bool apply);
	void SpellAuraModRangedAmmoHaste(bool apply);
	void SpellAuraModBaseResistancePerc(bool apply);
	void SpellAuraModResistanceExclusive(bool apply);
	void SpellAuraSafeFall(bool apply);
	void SpellAuraRetainComboPoints(bool apply);
	void SpellAuraResistPushback(bool apply);
	void SpellAuraModShieldBlockPCT(bool apply);
	void SpellAuraTrackStealthed(bool apply);
	void SpellAuraModDetectedRange(bool apply);
	void SpellAuraSplitDamageFlat(bool apply);
	void SpellAuraModStealthLevel(bool apply);
	void SpellAuraModUnderwaterBreathing(bool apply);
	void SpellAuraModReputationAdjust(bool apply);
	void SpellAuraNoPVPCredit(bool apply);
	void SpellAuraModHealthRegInCombat(bool apply);
	void SpellAuraPowerBurn(bool apply);
	void SpellAuraModCritDmgPhysical(bool apply);
	void SpellAuraModPAttackPower(bool apply);
	void SpellAuraModRangedAttackPowerPct(bool apply);
	void SpellAuraAPAttackerBonus(bool apply);
	void SpellAuraIncreaseDamageTypePCT(bool apply);
	void SpellAuraIncreaseCricticalTypePCT(bool apply);
	void SpellAuraIncreasePartySpeed(bool apply);
	void SpellAuraIncreaseSpellDamageByAttribute(bool apply);
	void SpellAuraIncreaseHealingByAttribute(bool apply);
	void SpellAuraIncreaseArmorByPctInt(bool apply);
	void SpellAuraReduceAttackerMHitChance(bool apply);
	void SpellAuraReduceAttackerRHitChance(bool apply);
	void SpellAuraReduceAttackerSHitChance(bool apply);
	void SpellAuraReduceEnemyMCritChance(bool apply);
	void SpellAuraReduceEnemyRCritChance(bool apply);
	void SpellAuraLimitSpeed(bool apply);
	void SpellAuraIncreaseTimeBetweenAttacksPCT(bool apply);
//	void SpellAuraIncreaseSpellDamageByInt(bool apply);
//	void SpellAuraIncreaseHealingByInt(bool apply);
	void SpellAuraIncreaseAllWeaponSkill(bool apply);
	void SpellAuraModAttackerCritChance(bool apply);
	void SpellAuraIncreaseHitRate(bool apply);
	void SpellAuraReduceCritMeleeAttackDmg(bool apply);
	void SpellAuraReduceCritRangedAttackDmg(bool apply);
	void SpellAuraEnableFlight(bool apply);
	void SpellAuraEnableFlightWithUnmountedSpeed(bool apply);
	void SpellAuraIncreaseRageFromDamageDealtPCT(bool apply);
	void SpellAuraIncreaseFlightSpeed(bool apply);
	void SpellAuraIncreaseMovementAndMountedSpeed(bool apply);
	void SpellAuraIncreaseRating(bool apply);
	void SpellAuraIncreaseCastTimePCT(bool apply);
	void SpellAuraRegenManaStatPCT(bool apply);
	void SpellAuraSpellHealingStatPCT(bool apply);
	void SpellAuraIncreaseMaxHealth(bool apply);
	void SpellAuraSpiritOfRedemption(bool apply);
	void SpellAuraIncreaseAttackerSpellCrit(bool apply);
	void SpellAuraIncreaseRepGainPct(bool apply);
	void SpellAuraIncreaseRangedAPStatPCT(bool apply);
	//void SpellAuraModRangedDamageTakenPCT(bool apply);
	void SpellAuraModBlockValue(bool apply);
	void SpellAuraFinishingMovesCannotBeDodged(bool apply);
	void SpellAuraAxeSkillModifier(bool apply);
	void SpellAuraDrinkNew(bool apply);
	void SpellAuraModPossessPet(bool apply);
	void EventPeriodicDrink(uint32 amount);

	void SendModifierLog(int32 ** m,int32 v,uint64 mask,uint8 type,bool pct = false);
	void SendDummyModifierLog(std::map<SpellEntry*,uint32> * m,SpellEntry * spellInfo,uint32 i,bool apply,bool pct = false);

	// Events
	void EventPeriodicDamage(uint32);
	void EventPeriodicDamagePercent(uint32);
	void EventPeriodicHeal(uint32);
	void EventPeriodicTriggerSpell(SpellEntry* spellInfo);
	void EventPeriodicEnergize(uint32,uint32);
	void EventPeriodicHeal1(uint32);
	void EventPeriodicLeech(uint32);
	void EventPeriodicBurn(uint32,uint32);
	void EventPeriodicHealthFunnel(uint32);
	void EventPeriodicManaLeech(uint32);
	void EventPeriodicHealPct(float);
	void EventPeriodicManaPct(float);
	void EventPeriodicRegenManaStatPct(uint32 perc,uint32 stat);
	void RelocateEvents();
	int32 event_GetInstanceID();

	CHRONO_INLINE void SendPeriodicHealAuraLog(uint32 amt)
	{
		WorldPacket data(32);
		data.SetOpcode(SMSG_PERIODICAURALOG);
		data << m_target->GetNewGUID();
		FastGUIDPack(data, m_casterGuid);
		data << GetSpellProto()->Id;
		data << uint32(1);
		data << uint32(FLAG_PERIODIC_HEAL);
		data << uint32(amt);
		m_target->SendMessageToSet(&data,true);
	}
	// log message's
	CHRONO_INLINE void SendPeriodicAuraLog(Unit * Caster, Unit * Target, uint32 SpellID, uint32 School, uint32 Amount, uint32 abs_dmg, uint32 resisted_damage, uint32 Flags)
	{
		WorldPacket data(SMSG_PERIODICAURALOG, 46);
		data << Target->GetNewGUID();		   // target guid
		data << Caster->GetNewGUID();		   // caster guid
		data << SpellID;						// spellid
		data << (uint32)1;					  // unknown? need research?
		data << uint32(Flags | 0x1);			// aura school
		data << Amount;						 // amount of done to target / heal / damage
		data << g_spellSchoolConversionTable[School];
		data << uint32(abs_dmg);
		data << uint32(resisted_damage);

		Caster->SendMessageToSet(&data, true);
	}

	void SendPeriodicAuraLog(const uint64& CasterGuid, Unit * Target, uint32 SpellID, uint32 School, uint32 Amount, uint32 abs_dmg, uint32 resisted_damage, uint32 Flags)
	{
		WorldPacket data(SMSG_PERIODICAURALOG, 46);
		data << Target->GetNewGUID();		   // target guid
		FastGUIDPack(data, CasterGuid);		 // caster guid
		data << SpellID;						// spellid
		data << (uint32)1;					  // unknown?? need resource?
		data << uint32(Flags | 0x1);			// aura school
		data << Amount;						 // amount of done to target / heal / damage
		data << g_spellSchoolConversionTable[School];
		data << uint32(abs_dmg);
		data << uint32(resisted_damage);

		Target->SendMessageToSet(&data, true);
	}

	bool WasCastInDuel() { return m_castInDuel; }

	SpellEntry * m_spellProto;
	Modifier * mod;
	AreaAuraList targets;//this is only used for AA
	uint64 m_casterGuid;

	uint32 m_auraSlot;

	uint32 m_castedItemId;
	bool m_areaAura;		// Area aura stuff -> never passive.
	uint8 m_visualSlot;
	uint32 pSpellId; //this represents the triggering spell id

	// this stuff can be cached in spellproto.
	CHRONO_INLINE bool IsCombatStateAffecting()
	{
		for(uint32 x = 0; x < 3; ++x) {
			if(m_spellProto->EffectApplyAuraName[x] == SPELL_AURA_PERIODIC_DAMAGE || 
				m_spellProto->EffectApplyAuraName[x] == SPELL_AURA_PERIODIC_DAMAGE_PERCENT ||
				m_spellProto->EffectApplyAuraName[x] == SPELL_AURA_PERIODIC_TRIGGER_SPELL ||
				m_spellProto->EffectApplyAuraName[x] == SPELL_AURA_PERIODIC_LEECH ||
				m_spellProto->EffectApplyAuraName[x] == SPELL_AURA_PERIODIC_MANA_LEECH)
			{
				return true;
			}
		}

		return false;
	}

	CHRONO_INLINE bool HasMechanic(uint32 MechanicsType)
	{
		//return Spell::HasMechanic(m_spellProto, MechanicsType);
		return false;
	}
	CHRONO_INLINE uint32 GetMechanic()
	{
		//return Spell::GetMechanic(m_spellProto);
		return 0;
	}
	
	bool m_castInDuel;

private:
	uint32 GetCasterFaction() { return m_casterfaction; }
	void SetCasterFaction(uint32 faction){ m_casterfaction = faction; }

	CHRONO_INLINE bool IsInrange(float x1,float y1, float z1, Object * o,float square_r)
	{
		float t;
		float r;
		t=x1-o->GetPositionX();
		r=t*t;
		t=y1-o->GetPositionY();
		r+=t*t;
		t=z1-o->GetPositionZ();
		r+=t*t;
		return ( r<=square_r);
	}
	
	Unit* m_target;
	Player * p_target;
	uint32 timeleft;
	int32 m_duration; // in msecs
//	bool m_positive;
	signed char m_positive;

	uint32 m_modcount;
	Modifier m_modList[3];

	uint32 m_dynamicValue;

protected:
	uint32 m_casterfaction;


	void SendInterrupted(uint8 result, Object * m_caster);
	void SendChannelUpdate(uint32 time, Object * m_caster);
public:
	bool m_deleted;
	int16 m_interrupted;
	bool m_ignoreunapply; // "special" case, for unapply
	CHRONO_INLINE bool IsInterrupted() { return ( m_interrupted >= 0 ); }
};

typedef void(Aura::*pSpellAura)(bool apply);
