#include "StdAfx.h"
#include "../Setup.h"
#include "../../Common/Base.h"
      
       
class ArracheaAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(ArracheaAI, MoonScriptCreatureAI);
    ArracheaAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
            AddSpell(6730, Target_Current, 15, 0, 0);
        }
    };
			
class Baeldun_AppraiserAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Baeldun_AppraiserAI, MoonScriptCreatureAI);
	Baeldun_AppraiserAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(2052, Target_WoundedFriendly, 60, 2, 0);
	}
};
	
class Baeldun_DiggerAI : public MoonScriptCreatureAI
{
	public:
	MOONSCRIPT_FACTORY_FUNCTION(Baeldun_DiggerAI, MoonScriptCreatureAI);
	Baeldun_DiggerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
};
	
class BluffwatcherAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(BluffwatcherAI, MoonScriptCreatureAI);
    BluffwatcherAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
		{
		AddSpell(6253, Target_Current, 20, 0, 0);
        AddSpell(12024, Target_Current, 15, 0, 0);
        AddSpell(23337, Target_Current, 40, 0, 0);
        AddSpell(11976, Target_Current, 60, 0, 0);			
		}
    };
    
class Bristleback_BattleboarAI : public MoonScriptCreatureAI
    {
	SpellDesc *Boar_Charge;
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Bristleback_BattleboarAI, MoonScriptCreatureAI);
    Bristleback_BattleboarAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        Boar_Charge = AddSpell(3385, Target_Self, 90, 0, 10);
        }
		void OnCombatStart(Unit *pTarget)
			{
			CastSpellNowNoScheduling(Boar_Charge);
			ParentClass::OnCombatStart(pTarget);
			}
    };
    
class Bristleback_InterloperAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Bristleback_InterloperAI, MoonScriptCreatureAI);
    Bristleback_InterloperAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(12166, Target_Current, 20, 0, 0);
        }
    };
    
class Bristleback_ShamanAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Bristleback_ShamanAI, MoonScriptCreatureAI);
    Bristleback_ShamanAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(9532, Target_Current, 80, 3, 0);
        }
    };
    
class Elder_PlainstriderAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Elder_PlainstriderAI, MoonScriptCreatureAI);
    Elder_PlainstriderAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(7272, Target_Current, 10, 0, 0);
        }
    };
    
class Flatland_CougarAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Flatland_CougarAI, MoonScriptCreatureAI);
    Flatland_CougarAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(16828, Target_Current, 30, 0, 0);
        }
    };
    
class Galak_OutrunnerAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Galak_OutrunnerAI, MoonScriptCreatureAI);
    Galak_OutrunnerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(6660, Target_Current, 90, 0, 0);
        }
    };
    
class Kodo_BullAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Kodo_BullAI, MoonScriptCreatureAI);
    Kodo_BullAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(5568, Target_Current, 20, 0, 0);
        }
    };
    
class Kodo_CalfAI : public MoonScriptCreatureAI
{
	SpellDesc *Rushing_Charge;
public:
    MOONSCRIPT_FACTORY_FUNCTION(Kodo_CalfAI, MoonScriptCreatureAI);
    Kodo_CalfAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		Rushing_Charge = AddSpell(6268, Target_Self, 0, 0, 0);
	}
	void OnCombatStart(Unit *pTarget)
	{
		CastSpellNowNoScheduling(Rushing_Charge);
		ParentClass::OnCombatStart(pTarget);
	}
};
    
class Kodo_MatriarchAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Kodo_MatriarchAI, MoonScriptCreatureAI);
    Kodo_MatriarchAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(5568, Target_Current, 20, 0, 0);
        }
    };
    
class MazzranacheAI : public MoonScriptCreatureAI
    {
	SpellDesc *Rushing_Charge;
public:
    MOONSCRIPT_FACTORY_FUNCTION(MazzranacheAI, MoonScriptCreatureAI);
    MazzranacheAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(3583, Target_Current, 15, 0, 0);
		Rushing_Charge = AddSpell(6268, Target_Self, 0, 0, 0);
        }
		void OnCombatStart(Unit *pTarget)
			{
			CastSpellNowNoScheduling(Rushing_Charge);
			ParentClass::OnCombatStart(pTarget);
			}
    };
 
class Palemane_PoacherAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Palemane_PoacherAI, MoonScriptCreatureAI);
    Palemane_PoacherAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(1516, Target_Current, 80, 0, 60);
        AddSpell(6660, Target_Current, 80, 0, 0);
        }
    };
 
class Palemane_SkinnerAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Palemane_SkinnerAI, MoonScriptCreatureAI);
    Palemane_SkinnerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(774, Target_WoundedFriendly, 60, 0, 0);
        }
    };
 
class Palemane_TannerAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Palemane_TannerAI, MoonScriptCreatureAI);
    Palemane_TannerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(9739, Target_Current, 80, 2, 0);
        }
    };
 
class Prairie_StalkerAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Prairie_StalkerAI, MoonScriptCreatureAI);
    Prairie_StalkerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(17255, Target_Current, 20, 0, 0);
		AddSpell(24604, Target_Self, 80, 0, 40);
        }
    };
 
class Prairie_WolfAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Prairie_WolfAI, MoonScriptCreatureAI);
    Prairie_WolfAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(5781, Target_Current, 80, 0, 45);
        }
    };
 
class Prairie_Wolf_AlphaAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Prairie_Wolf_AlphaAI, MoonScriptCreatureAI);
    Prairie_Wolf_AlphaAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(5781, Target_Current, 80, 0, 45);
        AddSpell(24604, Target_Self, 80, 0, 40);
        AddSpell(17255, Target_Current, 20, 0, 0);		
        }
    };
 
class Sister_HatelashAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Sister_HatelashAI, MoonScriptCreatureAI);
    Sister_HatelashAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(6960, Target_RandomFriendly, 90, 0, 1);
        AddSpell(9532, Target_Current, 80, 3, 0);
        }
    };
 
class SnagglespearAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(SnagglespearAI, MoonScriptCreatureAI);
    SnagglespearAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(12024, Target_Current, 15, 0, 0);
        }
    };
 
class SwoopAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(SwoopAI, MoonScriptCreatureAI);
    SwoopAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
		AddSpell(24424, Target_Current, 80, 0, 10);
		AddSpell(17255, Target_Current, 20, 0, 0);
        AddSpell(5708, Target_Current, 5, 0, 0);
        }
    };
 
class The_RakeAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(The_RakeAI, MoonScriptCreatureAI);
    The_RakeAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(12166, Target_Current, 20, 0, 0);
        }
    };
 
class Venture_Co_SupervisorAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Venture_Co_SupervisorAI, MoonScriptCreatureAI);
    Venture_Co_SupervisorAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(6673, Target_Self, 40, 0, 120);
        }
    };
 
class Windfury_MatriarchAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Windfury_MatriarchAI, MoonScriptCreatureAI);
    Windfury_MatriarchAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(332, Target_WoundedFriendly, 60, 2, 0);
        AddSpell(9532, Target_Current, 80, 3, 0);		
        }
    };
 
class Windfury_SorceressAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Windfury_SorceressAI, MoonScriptCreatureAI);
    Windfury_SorceressAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(13322, Target_Current, 80, 3, 0);
        }
    };
 
class Windfury_Wind_WitchAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Windfury_Wind_WitchAI, MoonScriptCreatureAI);
    Windfury_Wind_WitchAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(6982, Target_Self, 10, 2, 0);
        AddSpell(9532, Target_Current, 80, 3, 0);		
        }
    };
 
class Wiry_SwoopAI : public MoonScriptCreatureAI
    {
    public:
    MOONSCRIPT_FACTORY_FUNCTION(Wiry_SwoopAI, MoonScriptCreatureAI);
    Wiry_SwoopAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
        {
        AddSpell(5708, Target_Current, 15, 0, 0);
        }
    };


void SetupMulgoreNPC(ScriptMgr *mgr)
{
	mgr->register_creature_script(3058, &ArracheaAI::Create);
	mgr->register_creature_script(2990, &Baeldun_AppraiserAI::Create);
	mgr->register_creature_script(2989, &Baeldun_DiggerAI::Create);
	mgr->register_creature_script(3084, &BluffwatcherAI::Create);
	mgr->register_creature_script(2954, &Bristleback_BattleboarAI::Create);
	mgr->register_creature_script(3232, &Bristleback_InterloperAI::Create);
	mgr->register_creature_script(2953, &Bristleback_ShamanAI::Create);
	mgr->register_creature_script(2957, &Elder_PlainstriderAI::Create);
	mgr->register_creature_script(3035, &Flatland_CougarAI::Create);
  	mgr->register_creature_script(2968, &Galak_OutrunnerAI::Create);
  	mgr->register_creature_script(2973, &Kodo_BullAI::Create);
	mgr->register_creature_script(2972, &Kodo_CalfAI::Create);
	mgr->register_creature_script(2974, &Kodo_MatriarchAI::Create);
	mgr->register_creature_script(3068, &MazzranacheAI::Create);
	mgr->register_creature_script(2951, &Palemane_PoacherAI::Create);
	mgr->register_creature_script(2950, &Palemane_SkinnerAI::Create);
	mgr->register_creature_script(2949, &Palemane_TannerAI::Create);
	mgr->register_creature_script(2959, &Prairie_StalkerAI::Create);
	mgr->register_creature_script(2958, &Prairie_WolfAI::Create);
	mgr->register_creature_script(2960, &Prairie_Wolf_AlphaAI::Create);
	mgr->register_creature_script(5785, &Sister_HatelashAI::Create);
	mgr->register_creature_script(5786, &SnagglespearAI::Create);
	mgr->register_creature_script(2970, &SwoopAI::Create);
	mgr->register_creature_script(5807, &The_RakeAI::Create);
	mgr->register_creature_script(2979, &Venture_Co_SupervisorAI::Create);
	mgr->register_creature_script(2965, &Windfury_MatriarchAI::Create);
	mgr->register_creature_script(2964, &Windfury_SorceressAI::Create);
	mgr->register_creature_script(2963, &Windfury_Wind_WitchAI::Create);
	mgr->register_creature_script(2969, &Wiry_SwoopAI::Create);
}