/*
 * ChronoEmu Scripts - Blackrock Depths Bosses & Events
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by SideWays
 */

#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"
#include "BlackrockDepths.h"

//Anvilrage Captain
#define CN_ANVILRAGE_CAPTAIN            8903
class AnvilrageCaptainAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AnvilrageCaptainAI, MoonScriptCreatureAI);
    AnvilrageCaptainAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                                 // shield bash when player's casting
		AddSpell(11972, Target_Current, 100, 0, 15);
                       	// Demoralizing shout
		AddSpell(13730, Target_Current, 100, 0, 30);
		               	// Mortal Strike
		AddSpell(13737, Target_Current, 100, 0, 8);
				// Shield block
		AddSpell(12169, Target_Current, 50, 0, 5);
		
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        ParentClass::OnCombatStop(pTarget);
    }
};

//Anvilrage Footman
#define CN_ANVILRAGE_FOOTMAN            8892
class AnvilrageFootmanAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AnvilrageFootmanAI, MoonScriptCreatureAI);
    AnvilrageFootmanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                        //Kick when player's casting
        AddSpell(15614, Target_RandomPlayer, 100, 0, 8);
		               	// Strike
		AddSpell(15580, Target_Current, 100, 0, 4);	
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        ParentClass::OnCombatStop(pTarget);
    }
};

//Anvilrage Guardsman
#define CN_ANVILRAGE_GUARDSMAN            8891
class AnvilrageGuardsmanAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AnvilrageGuardsmanAI, MoonScriptCreatureAI);
    AnvilrageGuardsmanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// disarm
		AddSpell(6713, Target_ClosestPlayer, 16, 0, 2);
		               	// Shield block
		AddSpell(12169, Target_Current, 30, 0, 10);	
		// Sunder armor
		AddSpell(11971, Target_Current, 100, 0, 8);	
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        ParentClass::OnCombatStop(pTarget);
    }
};

//Anvilrage Marshal
#define CN_ANVILRAGE_MARSHAL            8898
class AnvilrageMarshalAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AnvilrageMarshalAI, MoonScriptCreatureAI);
    AnvilrageMarshalAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Devotion Aura
		ApplyAura(8258);
		               	// Holy Strike 2nd player nearby
		AddSpell(13953, Target_ClosestPlayerNotCurrent, 15, 0, 5);
                        // Holy Strike
		AddSpell(13953, Target_Current, 35, 0, 5);
                        // Holy Light heal other
		AddSpell(15493, Target_WoundedFriendly, 10, 2.5, 8);	
	}
	void AIUpdate()
{
        if(GetHealthPercent()<=20) //Heal self phase
        {
            AddSpell(15493, Target_Self, 100, 2.5, 10);

        }
        ParentClass::AIUpdate();
}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        ParentClass::OnCombatStop(pTarget);
    }
};

//Anvilrage Medic
#define CN_ANVILRAGE_MEDIC            8894
class AnvilrageMedicAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AnvilrageMedicAI, MoonScriptCreatureAI);
    AnvilrageMedicAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Prayer of Healing
		AddSpell(15585, Target_Self, 100, 3, 10);
		               	// Fortitude to close npcs
		AddSpell(13864, Target_RandomFriendly, 100, 0, 60);
                        // Mind Blast random
		AddSpell(15587, Target_RandomPlayer, 50, 1.5, 5);	
		                // Heal Ally
		AddSpell(15586, Target_WoundedFriendly, 100, 3.5, 8);	
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        ParentClass::OnCombatStop(pTarget);
    }
};

//Anvilrage Officer
#define CN_ANVILRAGE_OFFICER                       8895
class AnvilrageOfficerAI : public MoonScriptCreatureAI
{
 MOONSCRIPT_FACTORY_FUNCTION(AnvilrageOfficerAI, MoonScriptCreatureAI);
    AnvilrageOfficerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
	 Phase = 1;
									 //bubble at 10%
		   Bubble = AddSpell(13874, Target_Self, 0, 0, 60);
			//Holy Light after bubble
		   Heal = AddSpell(13952, Target_Self, 0, 2.5, 60);   
	                              
							// Backhand stun tank
	  AddSpell(6253, Target_Current, 25, 0, 10);
					 // Backhand stun random close
	  AddSpell(6253, Target_ClosestPlayerNotCurrent, 10, 0, 5);
					  // Battle Shout
	  AddSpell(9128, Target_Self, 100, 0, 120);
							// Shoot Random
	  AddSpell(15587, Target_RandomPlayer, 50, 1, 5); 
	 }
	 void AIUpdate()
	{
		if(Phase == 1 && _unit->GetHealthPct() <= 10) //bubble and heal phase
		{
			 CastSpellNowNoScheduling(Bubble);
			 Phase = 0;
		}
		if(Phase == 0 && _unit->GetHealthPct() < 30)
			CastSpellNowNoScheduling(Heal);

		ParentClass::AIUpdate();
	}
    void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
         ParentClass::OnCombatStop(pTarget);
    }
protected:
int Phase;
SpellDesc* Bubble;
SpellDesc* Heal;
};

//Anvilrage Overseer
#define CN_ANVILRAGE_OVERSEER                      8889
class AnvilrageOverseerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AnvilrageOverseerAI, MoonScriptCreatureAI);
    AnvilrageOverseerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Haste Aura
        ApplyAura(13589);	
		                // Strike
		AddSpell(15580, Target_Current, 100, 0, 8);
                        // Strike attack other melee
		AddSpell(15580, Target_ClosestPlayerNotCurrent, 10, 0, 8);		
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        ParentClass::OnCombatStop(pTarget);
    }
};

//Anvilrage Reservist
#define CN_ANVILRAGE_RESERVIST                     8901
class AnvilrageReservistAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AnvilrageReservistAI, MoonScriptCreatureAI);
    AnvilrageReservistAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Battle Shout
		AddSpell(9128, Target_Self, 100, 0, 60);
                        // Hamstring closest player
		AddSpell(9080, Target_ClosestPlayerNotCurrent, 10, 0, 8);
                       // Hamstring current
		AddSpell(9080, Target_Current, 10, 0, 8);
                       // Strike
		AddSpell(11976, Target_Current, 100, 0, 6);	
		AddSpell(6660, Target_RandomPlayer, 100, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
	}
};      

//Anvilrage Soldier
#define CN_ANVILRAGE_SOLDIER                       8893
class AnvilrageSoldierAI : public MoonScriptCreatureAI 
{
    MOONSCRIPT_FACTORY_FUNCTION(AnvilrageSoldierAI, MoonScriptCreatureAI);
    AnvilrageSoldierAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
    reck = true;
    
        Recklessness = AddSpell(13847, Target_Self, 0, 0, 0);
                       	// Cleave
		AddSpell(15284, Target_Current, 75, 0, 6);
                        // Hamstring current
		AddSpell(9080, Target_Current, 10, 0, 10);
                       // Strike
		AddSpell(11976, Target_Current, 100, 0, 6);		
	}
   void AIUpdate()
    {
		if(GetHealthPercent() <= 20 && reck == true)
		{
		 CastSpellNowNoScheduling(Recklessness);
		 reck = false;
		}
	    
    ParentClass::AIUpdate();
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
    protected:
	SpellDesc* Recklessness;
    bool reck;
};                   

//Anvilrage Warden
#define CN_ANVILRAGE_WARDEN            8890
class AnvilrageWardenAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AnvilrageWardenAI, MoonScriptCreatureAI);
    AnvilrageWardenAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Hooked Net
		AddSpell(14030, Target_RandomPlayer, 30, 0.5, 5);
		               	// Shield bash
		AddSpell(11972, Target_Current, 30, 0, 5);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_ARENA_SPECTATOR            8916
class ArenaSpectatorAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ArenaSpectatorAI, MoonScriptCreatureAI);
    ArenaSpectatorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Curse of Tongues
		AddSpell(13338, Target_RandomPlayer, 100, 0, 5);
		               	// Shadow bolt
		AddSpell(9613, Target_Current, 100, 3, 1);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        ParentClass::OnCombatStop(pTarget);
    }
};

//Blazing Fireguard
#define CN_BLAZING_FIREGUARD       8910
class BlazingFireguardAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BlazingFireguardAI, MoonScriptCreatureAI);
    BlazingFireguardAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Immolate
		AddSpell(12742, Target_RandomPlayer, 100, 2, 10);
		               	// Fire Blast
		AddSpell(13341, Target_Current, 50, 0, 8);
		                // Scorch
		AddSpell(15241, Target_Current, 75, 1.5, 3);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Bloodhound
#define CN_BLOODHOUND                              8921
class BloodhoundAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BloodhoundAI, MoonScriptCreatureAI);
    BloodhoundAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Dire Growl
		AddSpell(13692, Target_Current, 50, 0, 15);
		                // Rend
		AddSpell(13445, Target_Current, 100, 0, 15);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Bloodhound Mastiff
#define CN_BLOODHOUND_MASTIFF                      8922
class BloodhoundMastiffAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BloodhoundMastiffAI, MoonScriptCreatureAI);
    BloodhoundMastiffAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Expose Weakness
		AddSpell(7140, Target_Current, 50, 0, 5);
		                // Ravenous Claw
		AddSpell(15608, Target_Current, 20, 0, 3);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Borer Beetle
#define CN_BORER_BEETLE                            8932
class BorerBeetleAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BorerBeetleAI, MoonScriptCreatureAI);
    BorerBeetleAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Pierce armor
		AddSpell(6016, Target_Current, 50, 0, 45);
		                // Putrid Enzyme
		AddSpell(14539, Target_RandomPlayer, 20, 0, 10);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Burning Spirit
#define CN_BURNING_SPIRIT                          9178
class BurningSpiritAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BurningSpiritAI, MoonScriptCreatureAI);
    BurningSpiritAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Burning Spirit AKA KABOOM
		AddSpell(13489, Target_Current, 100, 0, 10);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Burrowing Thundersnout
#define CN_BURROWING_THUNDERSNOUT                  8928
class BurrowingThundersnoutAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BurrowingThundersnoutAI, MoonScriptCreatureAI);
    BurrowingThundersnoutAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// disjunction
		AddSpell(14533, Target_RandomPlayer, 100, 0, 12);
		                // Lizard Bolt
		AddSpell(15611, Target_Current, 100, 2, 4);
		                // Thunderclap
		AddSpell(15548, Target_Current, 100, 0, 10);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Cave Creeper
#define CN_CAVE_CREEPER                            8933
class CaveCreeperAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CaveCreeperAI, MoonScriptCreatureAI);
    CaveCreeperAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Creeper Venom
		AddSpell(14532, Target_Current, 30, 0, 12);
		                // Web
		AddSpell(15611, Target_RandomPlayer, 50, 0, 8);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Dark Guard
#define CN_DARK_GUARD                              9445
class DarkGuardAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DarkGuardAI, MoonScriptCreatureAI);
    DarkGuardAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Strike
		AddSpell(15580, Target_Current, 50, 0, 5);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Dark Screecher
#define CN_DARK_SCREECHER         8927
class DarkScreecherAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DarkScreecherAI, MoonScriptCreatureAI);
    DarkScreecherAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Aural Shock
		AddSpell(14538, Target_Self, 30, 0, 30);
		                // Sonic Burst
		AddSpell(8281, Target_Current, 20, 0, 5);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Deep Stinger
#define CN_DEEP_STINGER                            8926
class DeepStingerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeepStingerAI, MoonScriptCreatureAI);
    DeepStingerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Barbed Sting
		AddSpell(14534, Target_Self, 30, 0, 30);
		                // Sonic Burst
		AddSpell(40504, Target_Current, 35, 0, 3);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Doomforge Arcanasmith
#define CN_DOOMFORGE_ARCANASMITH                   8900
class DoomforgeArcanasmithAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DoomforgeArcanasmithAI, MoonScriptCreatureAI);
    DoomforgeArcanasmithAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Arcane Bolt
		AddSpell(13748, Target_Current, 30, 1, 3);
		                // Arcane Explosion
		AddSpell(13745, Target_Current, 35, 1.5, 4);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Doomforge Craftsman
#define CN_DOOMFORGE_CRAFTSMAN                     8897
class DoomforgeCraftsmanAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DoomforgeCraftsmanAI, MoonScriptCreatureAI);
    DoomforgeCraftsmanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Bomb
		AddSpell(9143, Target_RandomPlayerDestination, 50, 2, 3);
		                // Throw wrench
		AddSpell(15619, Target_Current, 60, 0, 4);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Doomforge Dragoon
#define CN_DOOMFORGE_DRAGOON                       8899
class DoomforgeDragoonAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DoomforgeDragoonAI, MoonScriptCreatureAI);
    DoomforgeDragoonAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Cleave
		AddSpell(9143, Target_Current, 30, 0, 5);
		                // Explosive Shot
		AddSpell(15495, Target_RandomPlayer, 30, 1, 6);
		                // Shoot
		AddSpell(15620, Target_Current, 100, 0, 3);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Dredge Worm
#define CN_DREDGE_WORM                             8925
class DredgeWormAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DredgeWormAI, MoonScriptCreatureAI);
    DredgeWormAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Dredge Sickness
		AddSpell(14535, Target_Self, 30, 0, 20);
		                // Poison
		AddSpell(13298, Target_Current, 20, 0, 6);
		                // Venom Spit
		AddSpell(6917, Target_RandomPlayer, 50, 0, 5);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Fireguard
#define CN_FIREGUARD                               6917
class FireguardAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FireguardAI, MoonScriptCreatureAI);
    FireguardAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Fireball 
		AddSpell(15242, Target_Current, 100, 3, 5);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Fireguard Destroyer
#define CN_FIREGUARD_DESTROYER                     8911
class FireguardDestroyerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FireguardDestroyerAI, MoonScriptCreatureAI);
    FireguardDestroyerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Fireball Volley
		AddSpell(15285, Target_Current, 30, 0, 10);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Gnashjaw <Malgen Longspear's Pet>
#define CN_GNASHJAW            16095
class GnashjawAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GnashjawAI, MoonScriptCreatureAI);
    GnashjawAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Some selfheal spell forgot name
		AddSpell(15503, Target_Self, 20, 0, 10);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Grim Patron
#define CN_GRIM_PATRON                             9545
class GrimPatronAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GrimPatronAI, MoonScriptCreatureAI);
    GrimPatronAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Kick
		AddSpell(15503, Target_Current, 20, 0, 5);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Guzzling Patron
#define CN_GUZZLING_PATRON                         9547
class CuzzlingPatronAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CuzzlingPatronAI, MoonScriptCreatureAI);
    CuzzlingPatronAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// CoA
		AddSpell(14868, Target_RandomPlayer, 40, 0, 10);
		                // SB
		AddSpell(20825, Target_Current, 100, 3, 4);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Hammered Patro
#define CN_HAMMERED_PATRON                         9554
class HammeredPatronAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HammeredPatronAI, MoonScriptCreatureAI);
    HammeredPatronAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Backhand
		AddSpell(6253, Target_Current, 30, 0, 5);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Korv
#define CN_KORV                          16053                     
class KorvAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KorvAI, MoonScriptCreatureAI);
    KorvAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// drink healing potion
		AddSpell(15503, Target_Current, 10, 0, 10);
		               // Frost shock tank
		AddSpell(21401, Target_Current, 40, 0, 8);
		                // Frost shock other
		AddSpell(21401, Target_RandomUnitNotCurrent, 20, 0, 10);
		               // Healing Wave
		AddSpell(27624, Target_Self, 40, 1.5, 5);
		               // Purge
		AddSpell(27626, Target_RandomPlayer, 15, 0, 5);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Lefty
#define CN_LEFTY                                   16049
class LeftyAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LeftyAI, MoonScriptCreatureAI);
    LeftyAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// drink healing potion
		AddSpell(15503, Target_Current, 10, 0, 10);
		               // Knockback
		AddSpell(11428, Target_Current, 30, 0, 8);
		               // 
		//AddSpell(27672, Target_Self, 5, 0, 30); think this one might simply crash the server
		               // Snap Kick
		AddSpell(27620, Target_Current, 40, 0, 5);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Mistress Nagmara Has some kinky unique skills (quest giver, but can turn to hostile), probably can crash the server when using 'em

//Molten War Golem <- no abilities

//Ragereaver Golem
#define CN_RAGEREAVER_GOLEM              8906
class RavereaverGolemAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RavereaverGolemAI, MoonScriptCreatureAI);
    RavereaverGolemAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                                GoEnrage = true;
                       	// drink healing potion
	                    Enrage = AddSpell(12795, Target_Current, 0, 0, 0);		               
	}
	void AIUpdate()
    {
    if(GetHealthPercent() <= 40 && GoEnrage == true)
    {
     CastSpellNowNoScheduling(Enrage);
     GoEnrage = false;
    }
    
    ParentClass::AIUpdate();
 }
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
    protected:
	SpellDesc* Enrage;
    bool GoEnrage;
};

//Rezznik
#define CN_REZZNIK                                 16054
class RezznikAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RezznikAI, MoonScriptCreatureAI);
    RezznikAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Dark Iron Bomb
		AddSpell(19784, Target_RandomPlayerDestination, 30, 1, 10);
		               	// Drink Healing Potion
		AddSpell(15503, Target_Current, 20, 0, 30);	
		// Explosive Sheep
		// AddSpell(8209, Target_Current, 100, 0, 8);
                          //Goblin Dragon Gun
        AddSpell(27603, Target_Current, 20, 0, 10);	
                        //Recombobulate
        AddSpell(27677, Target_RandomFriendly, 20, 0, 20);
                        //Summon Arcanite Dragonling
        //AddSpell(27602, Target_Current, 20, 0, 30);	
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
};

//Ribbly's Crony
#define CN_RIBBLYS_CRONY                           10043
class RibblysCronyAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RibblysCronyAI, MoonScriptCreatureAI);
    RibblysCronyAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Evi
		AddSpell(15692, Target_Current, 30, 0, 10);
		               // Sinister Strike
		AddSpell(15581, Target_Current, 50, 0, 6);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        ParentClass::OnCombatStop(pTarget);
    }
}; 

#define CN_SHADOWFORGE_CITIZEN                           8902
class ShadowforgeCitizenAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowforgeCitizenAI, MoonScriptCreatureAI);
    ShadowforgeCitizenAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Fireblast
		AddSpell(13339, Target_Current, 100, 0, 5);
		               // Shadow bolt
		AddSpell(9613, Target_Current, 100, 3, 1);	
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        ParentClass::OnCombatStop(pTarget);
    }
}; 

//Shadowforge Flame Keeper
#define CN_SHADOWFORGE_FLAME_KEEPER                     9956
class ShadowforgeFlameKeeperAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowforgeFlameKeeperAI, MoonScriptCreatureAI);
    ShadowforgeFlameKeeperAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Battle Shout
		AddSpell(9128, Target_Self, 100, 0, 60);
                        // Hamstring closest player
		AddSpell(9080, Target_ClosestPlayerNotCurrent, 10, 0, 8);
                       // Hamstring current
		AddSpell(9080, Target_Current, 10, 0, 8);
                       // Strike
		AddSpell(11976, Target_Current, 100, 0, 6);		
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

#define CN_SHADOWFORGE_PEASANT                           8896
class ShadowforgePeasantAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowforgePeasantAI, MoonScriptCreatureAI);
    ShadowforgePeasantAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Fireblast
		AddSpell(13339, Target_Current, 100, 0, 5);
		               // Shadow bolt
		AddSpell(9613, Target_Current, 100, 3, 1);	
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        ParentClass::OnCombatStop(pTarget);
    }
}; 

#define CN_SHADOWFORGE_SENATOR	                         10043
class ShadowforgeSenatorAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowforgeSenatorAI, MoonScriptCreatureAI);
    ShadowforgeSenatorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Fireblast
		AddSpell(14868, Target_RandomPlayer, 100, 0, 5);
		               // Shadow bolt
		AddSpell(9053, Target_Current, 100, 3, 1);	
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        ParentClass::OnCombatStop(pTarget);
    }
}; 

//Snokh Blackspine
#define CN_SNOKH_BLACKSPINE                                                    16051
class SnokhBlackspineAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SnokhBlackspineAI, MoonScriptCreatureAI);
    SnokhBlackspineAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Scorch
		AddSpell(15241, Target_Current, 40, 1.5, 2);
		               // pyroblast
		AddSpell(17273, Target_RandomPlayer, 20, 6, 10);
		                // Drink heal pot
		AddSpell(15503, Target_Self, 30, 0, 20);
		               // Flamestrike
		AddSpell(20827, Target_RandomPlayerDestination, 20, 3, 15);
		                // Blink
		AddSpell(14514, Target_RandomDestination, 100, 0, 10);
		               // Polymorph
		AddSpell(13323, Target_RandomPlayer, 50, 1.5, 15);
		                // Blast Wave
		AddSpell(23039, Target_RandomPlayer, 60, 2, 15);
		
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Twilight Bodyguard
#define CN_TWILIGHT_BODYGUARD                      8914
class TwilightBodyguardAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TwilightBodyguardAI, MoonScriptCreatureAI);
    TwilightBodyguardAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Fist of Ragnaros
		AddSpell(13902, Target_Current, 100, 0, 5);
		               // Seal of Sacrifice
		AddSpell(13903, Target_RandomFriendly, 50, 0, 20);
		                // Pummel when casting
		AddSpell(15615, Target_Current, 100, 0, 8);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Twilight Emissary
#define CN_TWILIGHT_EMISSARY                       8913
class TwilightEmissaryAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TwilightEmissaryAI, MoonScriptCreatureAI);
    TwilightEmissaryAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Arcane Bolt
		AddSpell(13748, Target_RandomPlayer, 50, 1, 5);
		               // Arcane explosion
		AddSpell(13745, Target_RandomPlayer, 20, 1.5, 10);
		                // Fireball
		AddSpell(12466, Target_Current, 40, 3, 10);
		               // Frost Armor
		ApplyAura(12544);
		                // Frost Nova
		AddSpell(15063, Target_Self, 100, 0, 30);
		               // Frost Bolt
		AddSpell(15043, Target_RandomPlayer, 50, 3, 10);
		                // Fury of Ragnaros
		ApplyAura(15288);
		
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Twilight's Hammer Ambassador
#define CN_TWILIGHTS_HAMMER_AMBASSADOR             8915
class TwilightsHammerAbassadorAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TwilightsHammerAbassadorAI, MoonScriptCreatureAI);
    TwilightsHammerAbassadorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Aplify Damage
		AddSpell(12248, Target_Current, 100, 2, 15);
		               // Bloodlust
		AddSpell(6742, Target_ClosestFriendly, 100, 0, 20);
		                // Curse of Weakness
		AddSpell(11980, Target_RandomPlayer, 100, 0, 8);
		               // Earth Shock
		AddSpell(15501, Target_Current, 100, 0, 10);
		                // Flame Shock
		AddSpell(15096, Target_Current, 100, 0, 12);
		               // Frost Shock
		AddSpell(15499, Target_RandomPlayer, 100, 0, 8);
		                // Frost Shock
		AddSpell(15500, Target_Current, 100, 0, 14);
		                // Frost Armor
		ApplyAura(12544);
		
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Twilight's Hammer Executioner
#define CN_TWILIGHTS_HAMMER_EXECUTIONER            9398
class TwilightsHammerExecutionerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TwilightsHammerExecutionerAI, MoonScriptCreatureAI);
    TwilightsHammerExecutionerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Battle Fury
		AddSpell(3631, Target_Self, 100, 0, 100);
		               // Decimate
		AddSpell(13459, Target_Current, 100, 0, 20);		
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Twilight's Hammer Torturer
#define CN_TWILIGHTS_HAMMER_TORTURER            8912
class TwilightsHammerTorturerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TwilightsHammerTorturerAI, MoonScriptCreatureAI);
    TwilightsHammerTorturerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// SW:Pain
		AddSpell(14032, Target_RandomPlayer, 100, 0, 5);		
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Va'jashni
#define CN_VAJASHNI                                16055
class VajashniAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VajashniAI, MoonScriptCreatureAI);
    VajashniAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Dispel Magic
		AddSpell(27609, Target_RandomFriendly, 100, 0, 10);
                        // Drink hp pot
		AddSpell(15503, Target_Self, 30, 0, 15);
                        // Flash Heal
		AddSpell(27608, Target_WoundedFriendly, 50, 1.5, 8);
                        // SW:Shield
		AddSpell(27607, Target_WoundedFriendly, 40, 0, 10);
                        // renew
		AddSpell(27606, Target_WoundedFriendly, 100, 0, 10);
                        // SW:Pain
		AddSpell(27605, Target_RandomPlayer, 100, 0, 8);		
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Volida
#define CN_VOLIDA                               16058
class VolidaAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VolidaAI, MoonScriptCreatureAI);
    VolidaAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Blink
		AddSpell(14514, Target_RandomDestination, 100, 0, 10);
                        // Blizzard
		AddSpell(27618, Target_RandomPlayerDestination, 30, 5, 15);
                        // CoCold
		AddSpell(20828, Target_Current, 50, 0, 8);
                        // drink healing pot
		AddSpell(15503, Target_Self, 20, 0, 60);
                        // Frost nova
		AddSpell(15063, Target_Self, 100, 0, 8);
                        // Frostbolt
		AddSpell(20822, Target_RandomPlayer, 100, 3, 6);	
                        // Iceblock
		AddSpell(27619, Target_Self, 20, 0, 300);	
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 
 
 
 //Warbringer Construct
#define CN_WARBRINGER_CONSTRUCT                               8905
class WarbringerConstructAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WarbringerConstructAI, MoonScriptCreatureAI);
    WarbringerConstructAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Skull crack
		AddSpell(15621, Target_Current, 30, 0, 2);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Weapon Technician
#define CN_WEAPON_TECHNICIAN                       8920
class WeaponTechnicianAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WeaponTechnicianAI, MoonScriptCreatureAI);
    WeaponTechnicianAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	// Exploding Shot
		AddSpell(7896, Target_Current, 25, 1, 4);
		                // Frost Shot
		AddSpell(12551, Target_Current, 25, 0, 5);
		                // Multishot
		AddSpell(14443, Target_Current, 50, 0, 8);
		                // Skull crack
		AddSpell(15620, Target_Current, 75, 0, 2);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

//Wrath Hammer Construct
#define CN_WRATH_HAMMER_CONSTRUCT                  8907
class WrathHammerConstructAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WrathHammerConstructAI, MoonScriptCreatureAI);
    WrathHammerConstructAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
                       	//Flame Cannon
		AddSpell(15575, Target_Current, 25, 1.5, 4);
		                // Uppercut
		AddSpell(10966, Target_Current, 30, 0, 6);
	}
	   void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
                ParentClass::OnCombatStop(pTarget);
    }
}; 

void SetupBlackrockDepthsTrash(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_ANVILRAGE_CAPTAIN, &AnvilrageCaptainAI::Create);
	mgr->register_creature_script(CN_ANVILRAGE_FOOTMAN, &AnvilrageFootmanAI::Create);
	mgr->register_creature_script(CN_ANVILRAGE_GUARDSMAN, &AnvilrageGuardsmanAI::Create);
	mgr->register_creature_script(CN_ANVILRAGE_MARSHAL, &AnvilrageMarshalAI::Create);
	mgr->register_creature_script(CN_ANVILRAGE_MEDIC, &AnvilrageMedicAI::Create);
	mgr->register_creature_script(CN_ANVILRAGE_OFFICER, &AnvilrageOfficerAI::Create);
	mgr->register_creature_script(CN_ANVILRAGE_OVERSEER, &AnvilrageOverseerAI::Create);
	mgr->register_creature_script(CN_ANVILRAGE_RESERVIST, &AnvilrageReservistAI::Create);
	mgr->register_creature_script(CN_ANVILRAGE_SOLDIER, &AnvilrageSoldierAI::Create);
	mgr->register_creature_script(CN_ANVILRAGE_WARDEN, &AnvilrageWardenAI::Create);
	mgr->register_creature_script(CN_ARENA_SPECTATOR, &ArenaSpectatorAI::Create);
	mgr->register_creature_script(CN_BLAZING_FIREGUARD, &BlazingFireguardAI::Create);
	mgr->register_creature_script(CN_BLOODHOUND, &BloodhoundAI::Create);
	mgr->register_creature_script(CN_BLOODHOUND_MASTIFF, &BloodhoundMastiffAI::Create);
	mgr->register_creature_script(CN_BORER_BEETLE, &BorerBeetleAI::Create);
	mgr->register_creature_script(CN_BURNING_SPIRIT, &BurningSpiritAI::Create);
	mgr->register_creature_script(CN_BURROWING_THUNDERSNOUT, &BurrowingThundersnoutAI::Create);
	mgr->register_creature_script(CN_CAVE_CREEPER, &CaveCreeperAI::Create);
	mgr->register_creature_script(CN_DARK_GUARD, &DarkGuardAI::Create);
	mgr->register_creature_script(CN_DARK_SCREECHER, &DarkScreecherAI::Create);
	mgr->register_creature_script(CN_DEEP_STINGER, &DeepStingerAI::Create);
	mgr->register_creature_script(CN_DOOMFORGE_ARCANASMITH, &DoomforgeArcanasmithAI::Create);
	mgr->register_creature_script(CN_DOOMFORGE_CRAFTSMAN, &DoomforgeCraftsmanAI::Create);
	mgr->register_creature_script(CN_DOOMFORGE_DRAGOON, &DoomforgeDragoonAI::Create);
	mgr->register_creature_script(CN_DREDGE_WORM, &DredgeWormAI::Create);
	mgr->register_creature_script(CN_FIREGUARD, &FireguardAI::Create);
	mgr->register_creature_script(CN_FIREGUARD_DESTROYER, &FireguardDestroyerAI::Create);
	mgr->register_creature_script(CN_GNASHJAW, &GnashjawAI::Create);
	mgr->register_creature_script(CN_GRIM_PATRON, &GrimPatronAI::Create);
	mgr->register_creature_script(CN_GUZZLING_PATRON, &CuzzlingPatronAI::Create);
	mgr->register_creature_script(CN_HAMMERED_PATRON, &HammeredPatronAI::Create);
	mgr->register_creature_script(CN_KORV, &KorvAI::Create);
	mgr->register_creature_script(CN_LEFTY, &LeftyAI::Create);
	mgr->register_creature_script(CN_RAGEREAVER_GOLEM, &RavereaverGolemAI::Create);
	mgr->register_creature_script(CN_REZZNIK, &RezznikAI::Create);
	mgr->register_creature_script(CN_RIBBLYS_CRONY, &RibblysCronyAI::Create);
	mgr->register_creature_script(CN_SHADOWFORGE_CITIZEN, &ShadowforgeCitizenAI::Create);
	mgr->register_creature_script(CN_SHADOWFORGE_FLAME_KEEPER, &ShadowforgeFlameKeeperAI::Create);
	mgr->register_creature_script(CN_SHADOWFORGE_PEASANT, &ShadowforgePeasantAI::Create);
	mgr->register_creature_script(CN_SHADOWFORGE_SENATOR, &ShadowforgePeasantAI::Create);
	mgr->register_creature_script(CN_SNOKH_BLACKSPINE, &SnokhBlackspineAI::Create);
	mgr->register_creature_script(CN_TWILIGHT_BODYGUARD, &TwilightBodyguardAI::Create);
	mgr->register_creature_script(CN_TWILIGHT_EMISSARY, &TwilightEmissaryAI::Create);
	mgr->register_creature_script(CN_TWILIGHTS_HAMMER_AMBASSADOR, &TwilightsHammerAbassadorAI::Create);
	mgr->register_creature_script(CN_TWILIGHTS_HAMMER_EXECUTIONER, &TwilightsHammerExecutionerAI::Create);
	mgr->register_creature_script(CN_TWILIGHTS_HAMMER_TORTURER, &TwilightsHammerTorturerAI::Create);
	mgr->register_creature_script(CN_VOLIDA, &VolidaAI::Create);
	mgr->register_creature_script(CN_VAJASHNI, &VajashniAI::Create);
	mgr->register_creature_script(CN_WARBRINGER_CONSTRUCT, &WarbringerConstructAI::Create);
	mgr->register_creature_script(CN_WEAPON_TECHNICIAN, &WeaponTechnicianAI::Create);
	mgr->register_creature_script(CN_WRATH_HAMMER_CONSTRUCT, &WrathHammerConstructAI::Create);
}