//
// CHRONO EMU (C) 2016
//
// Spell System
//

#include "StdAfx.h"

/*********************************
 *                               *
 * DATABASE LOADED SPELL UPDATES *
 *                               *
 *********************************/


//
// Direction required for spell.
// SQL Table: spell_direction
//
// STRUCTURE:
// 
// EntryID      - ID of the Spell
// ReqDirection - Is Infront or Behind
//
void ObjectMgr::LoadSpellInFront()
{
	Log.Debug("ObjectMgr", "Loading spell_direction data");
	if (QueryResult* result = WorldDatabase.Query("SELECT * FROM spell_direction"))
    {
        do
        {
            Field *f = result->Fetch();
            uint32 sf_spellId = f[0].GetUInt32();
            uint32 sf_isinfront = f[1].GetUInt32();

			if (SpellEntry* sp = dbcSpell.LookupEntryForced(sf_spellId))
			{
                {
                    if (sf_isinfront)
                        sp->in_front_status = sf_isinfront;
                }
            }
        }
		while (result->NextRow());
        delete result;
    }
}

//
// Spell Proc on Name Hash
// SQL Table: spell_proc
//
// STRUCTURE:
//
// spellID - ID of the Spell
// ProcOnNameHash - Spell Name Hash see SpellNameHashes.h
//
void ObjectMgr::LoadSpellProcs()
{
	Log.Debug("ObjectMgr", "Loading spell_proc data");
	if (QueryResult* result = WorldDatabase.Query("SELECT * FROM spell_proc"))
	{
        do
        {
            Field *f = result->Fetch();
            uint32 spe_spellId = f[0].GetUInt32();
            uint32 spe_NameHash = f[1].GetUInt32();

			if (SpellEntry* sp = dbcSpell.LookupEntryForced(spe_spellId))
            {
                 int x;
                  for (x = 0; x < 3; ++x)
                      if (sp->ProcOnNameHash[x] == 0)
                          break;

                  if (x != 3)
                  {
                      sp->ProcOnNameHash[x] = spe_NameHash;
                  }
                  else
                      sLog.outError("Wrong ProcOnNameHash for Spell: %u!", spe_spellId);
            }
        }
		while (result->NextRow());
        delete result;
    }
}

//
// Spell Information
//
void ObjectMgr::LoadSpellFixes()
{
	Log.Debug("ObjectMgr", "Loading spell_information data");
	if (QueryResult* result = WorldDatabase.Query("SELECT * FROM spell_information"))
    {
        do
        {
            Field *f = result->Fetch();
            uint32 sf_spellId = f[0].GetUInt32();           // Entry
            uint32 sf_SchoolType = f[1].GetUInt32();        // School Type
            uint32 sf_SpellCategory = f[2].GetUInt32();     // Spell Category
            uint32 sf_DispelType = f[3].GetUInt32();        // Dispel Type
            uint32 sf_MechanicType = f[4].GetUInt32();      // Mechanics Type
            uint32 sf_Attributes = f[5].GetUInt32();        // Attributes
            uint32 sf_AttributesEx = f[6].GetUInt32();      // AttributesEx
            uint32 sf_Triggerspell = f[7].GetUInt32();      // Trigger Spell
            uint32 sf_procFlags = f[8].GetUInt32();         // Proc Flags
            uint32 sf_procChance = f[9].GetUInt32();        // Proc Chance
            uint32 sf_PPM = f[10].GetUInt32();              // Procs Per Minute
            uint32 sf_procCharges = f[11].GetUInt32();      // Proc Charges
            uint32 sf_Cooldown = f[12].GetUInt32();         // Cooldown
            uint32 sf_MaxStack = f[13].GetUInt32();         // Max Stack
            uint32 sf_SpellFamilyName = f[14].GetUInt32();  // Spell Family Name
            uint32 sf_SpellGroupType = f[15].GetUInt32();   // Spell Group Type
            uint32 sf_MaxTargets = f[16].GetUInt32();	    // Max Targets
                                                           
                                                            // CUSTOM BELOW THIS POINT
                                                            
            uint32 sf_CustomIsFlag = f[17].GetUInt32();     // Custom is Flag
            uint32 sf_ProcInterval = f[18].GetUInt32();     // Proc Interval
            uint32 sf_BuffType = f[19].GetUInt32();         // Buff Type
            uint32 sf_NameHash = f[20].GetUInt32();         // Name Hash
            bool sf_SpellCanCrit = f[21].GetBool();         // Spell Can Crit
            bool sf_IsRanged = f[22].GetBool();             // Is Ranged
            bool sf_IsMelee = f[23].GetBool();              // Is Melee

				if (SpellEntry* sp = dbcSpell.LookupEntryForced(sf_spellId))
				{
                    if (sf_SchoolType)
						sp->School = sf_SchoolType; 
                    if (sf_SpellCategory) 
						sp->Category = sf_SpellCategory; 
                    if (sf_DispelType) 
						sp->DispelType = sf_DispelType;
                    if (sf_MechanicType) 
						sp->MechanicsType = sf_MechanicType;
                    if (sf_Attributes)  
						sp->Attributes = sf_Attributes; 
                    if (sf_AttributesEx)
						sp->AttributesEx = sf_AttributesEx;
                    if (sf_Triggerspell)
						sp->EffectTriggerSpell[3] = sf_Triggerspell;
                    if (sf_procFlags) 
						sp->procFlags = sf_procFlags;
                    if (sf_procChance) 
						sp->procChance = sf_procChance; 
                    if (sf_PPM) 
						sp->ProcsPerMinute = sf_PPM; 
                    if (sf_procCharges)
						sp->procCharges = sf_procCharges;
                    if (sf_Cooldown)
						sp->RecoveryTime = sf_Cooldown;
                    if (sf_MaxStack)
						sp->maxstack = sf_MaxStack;
                    if (sf_SpellFamilyName)
						sp->SpellFamilyName = sf_SpellFamilyName;
                    if (sf_SpellGroupType)
						sp->SpellGroupType = sf_SpellGroupType;
                    if (sf_MaxTargets)
						sp->MaxTargets = sf_MaxTargets;

                    // Custom Below
                    if (sf_CustomIsFlag)
						sp->c_is_flags = sf_CustomIsFlag;
                    if (sf_ProcInterval)
						sp->proc_interval = sf_ProcInterval; 
                    if (sf_BuffType) 
						sp->buffType = sf_BuffType; 
                    if (sf_NameHash)
						sp->NameHash = sf_NameHash; 
					if (sf_SpellCanCrit)
						sp->spell_can_crit = sf_SpellCanCrit;
                    if (sf_IsRanged)
						sp->is_ranged_spell = sf_IsRanged;
                    if (sf_IsMelee) { sp->is_melee_spell = sf_IsMelee; }
                }
				else
				{
					Log.Error("ObjectMgr", "No spell_information data loaded...");
				}
        } 
		while (result->NextRow());
        delete result;
    }
}

//
//FORCED CREATURE SPELL TARGETING
//
void ObjectMgr::LoadSpellForcedTargets()
{
	Log.Debug("ObjectMgr", "Loading spell_forced_targets data");
	if (QueryResult* result = WorldDatabase.Query("SELECT * FROM spell_forced_targets"))
	{
        do
        {
            Field *f = result->Fetch();
            uint32 spellid = f[0].GetUInt32();
            uint32 forcedtarget = f[1].GetUInt32();

			if (SpellEntry* sp = dbcSpell.LookupEntryForced(spellid))
			{
                if (forcedtarget)
                     sp->forced_creature_target = forcedtarget;
            }
        } 
		while (result->NextRow());
        delete result;
    }
}

//
// Spell Coef Override
// This will allow you to override the default coefs for the spell.
//
void ObjectMgr::LoadSpellCoefOverride()
{
    QueryResult * result = WorldDatabase.Query("SELECT * FROM spell_coef_override");
	if (!result) return;

	Log.Debug("ObjectMgr", "Loading spell_coef_override data");
	if (QueryResult* result = WorldDatabase.Query("SELECT * FROM spell_coef_override"))
	{
		do
		{
            Field *f = result->Fetch();
            uint32 spellid = f[0].GetUInt32();
            uint32 DSpellOverride = f[1].GetFloat();
            uint32 OTSpellOverride = f[2].GetFloat();
            uint32 APSpellOverride = f[3].GetFloat();
            uint32 RAPSpellOverride = f[4].GetFloat();

			if (SpellEntry* sp = dbcSpell.LookupEntryForced(spellid))
			{
                 if (DSpellOverride)
                     sp->Dspell_coef_override = DSpellOverride;
                 if (OTSpellOverride)
                     sp->OTspell_coef_override = OTSpellOverride;
                 if (APSpellOverride)
                     sp->AP_coef_override = APSpellOverride;
                 if (RAPSpellOverride)
                     sp->RAP_coef_override = RAPSpellOverride;
           }
        } 
		while (result->NextRow());
        delete result;
    }
}