/*
 * ChronoEmu Scripts - Scholomance Bosses & Events
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by SideWays
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"
#include "Scholomance.h"


#define GO_FLAG_UNCLICKABLE 0x00000010
/*class SkeletonDoor : public GameObjectAIScript
{
public:
	SkeletonDoor(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	void OnCreate()
	{
		_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
	}
};*/
		
class BrazierOfTheHerald : public GameObjectAIScript
{
public:
	BrazierOfTheHerald(GameObject* goinstance) : GameObjectAIScript(goinstance) {
	}	
	void OnActivate( Player* pPlayer )
	{	
		if(pPlayer->GetItemInterface()->GetItemCount(13523, false))
		{
			pPlayer->GetItemInterface()->RemoveItemAmt(13523, 1);
			Creature* NewCreature = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(10506, 332.728790f, 95.272408f, 106.263931f, 3.188245f, true, false, 0, 0); 
			for(int i=1;i<3;i++)
			{
				WayPoint * wp = NewCreature->CreateWaypointStruct();
				wp->id = i;
				wp->x = KirtonosWP[i].mX;
				wp->y = KirtonosWP[i].mY;
				wp->z = KirtonosWP[i].mZ;
				wp->o = KirtonosWP[i].mO;
				wp->waittime = 0;
				wp->flags = WALK;
				wp->forwardemoteoneshot = 0;
				wp->forwardemoteid = 0;
				wp->backwardemoteoneshot = 0;
				wp->backwardemoteid = 0;
				wp->forwardskinid = 0;
				wp->backwardskinid = 0;
				NewCreature->GetAIInterface()->addWayPoint(wp);
			}
			NewCreature->GetAIInterface()->StopMovement(0);
			NewCreature->GetAIInterface()->setWaypointToMove(0);

			GameObject* Gate = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(287.941864f, 92.195206f, 115.257523f, 175570);
			if(Gate != nullptr)
			{
				Gate->SetUInt32Value(GAMEOBJECT_STATE, 1);
				Gate->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
			}
			_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);			
		}
		else
		{
			_gameobject->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
			pPlayer->BroadcastMessage("Missing required item : Blood of Innocents");
			return;
		}
	}
		static GameObjectAIScript *Create(GameObject* GO) { return new BrazierOfTheHerald(GO); }
};

class KirtonosAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(KirtonosAI, MoonScriptCreatureAI);
    KirtonosAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//mTransform = INVALIDATE_TIMER;
		AddSpell(CURSE_OF_TONGUES, Target_RandomPlayerNotCurrent, 15, 0, 15);
		AddSpell(DISARM, Target_Current, 5, 0, 10);
		AddSpell(PIERCE_ARMOR, Target_Current, 12, 0, 6);
		AddSpell(SHADOWBOLT_VOLLEY, Target_Current, 14, 0, 4);
		AddSpell(SWOOP, Target_Current, 21, 0, 10);
		AddSpell(WING_FLAP, Target_Current, 9, 0, 10);
		Transform = AddSpell(KIRTONOS_TRANSFORM, Target_Self, 0, 0, 0);
		//AddSpell(K_DOMINATE_MIND, Target_RandomPlayer, 100, 2, 15);
	}		

	void OnCombatStart(Unit* pTarget)
    {
		morphcheck = true;
        ParentClass::OnCombatStart(pTarget);
    }
	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 1)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			/*_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, X);
			mTransform = AddTimer(30000);*/
		}
		ParentClass::OnReachWP(iWaypointId, bForwards);
	}
	void OnDied(Unit* pKiller)
    {
		GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(287.941864f, 92.195206f, 115.257523f, 175570);
		Gate->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		Gate->SetUInt32Value(GAMEOBJECT_STATE, 0);
	   ParentClass::OnDied(pKiller);
    }
	void OnCombatStop(Unit* pTarget)
	{
		GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(287.941864f, 92.195206f, 115.257523f, 175570);
		GameObject* Brazier = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(325.122f, 93.6436f, 101.637f, 175570);
		Gate->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		Gate->SetUInt32Value(GAMEOBJECT_STATE, 0);
		Brazier->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		Brazier->SetUInt32Value(GAMEOBJECT_STATE, 0);
		_unit->Despawn(2000, 0);
		ParentClass::OnCombatStop(pTarget);
	}

	void AIUpdate()
    {
		/*if(IsTimerFinished(mTransform))
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, UNIT_FIELD_NATIVEDISPLAYID);*/
		ParentClass::AIUpdate();
    }

protected:
	//int mTransform;
	SpellDesc* Transform;
	bool morphcheck;
};

class DoctorTheolenKrastinovAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DoctorTheolenKrastinovAI, MoonScriptCreatureAI);
	DoctorTheolenKrastinovAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		FRENZY_LIMITER = 0;
		
		AddSpell(REND, Target_Current, 20, 0, 25);
		Backh = AddSpell(BACKHAND, Target_Current, 0, 0, 0);
		Frenzydesc = AddSpell(FRENZY, Target_Current, 0, 0, 180, 0, 0, false, "Doctor Theolen Krastinov goes into a frenzy!", Text_Emote, 639);
	}		
	void OnCombatStart(Unit* pTarget)
    {
		BackhCD = 10;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The doctor is in!");
		FRENZY_LIMITER = 0;
        ParentClass::OnCombatStart(pTarget);
    }

	void OnDied(Unit* pKiller)
    {
		Polkelt = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(274.877014f, 1.333660f, 85.311699f, 10901);
		Illucia = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(265.955994f, 0.903429f, 75.333397f, 10502);
		Malicia = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(86.663399f, -1.960390f, 85.311699f, 10505);
		Ravenian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(103.305000f, -1.677520f, 75.218300f, 10507);
		Alexei = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(178.723999f, -91.023201f, 70.856796f, 10504);
		
		if((!Illucia || !Illucia->isAlive()) && (!Polkelt || !Polkelt->isAlive()) && (!Malicia || !Malicia->isAlive())
			 && (!Alexei || !Alexei->isAlive()) && (!Ravenian || !Ravenian->isAlive()))
		{
			Creature* Gandling = _unit->GetMapMgr()->GetInterface()->SpawnCreature(1853, 180.927994f, -9.374770f, 75.506699f, 1.610830f, true, false, 0, 0);
			Gandling->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "School is in session!");
		}
		BackhCD = 15;
		FRENZY_LIMITER = 0;
		ParentClass::OnDied(pKiller);
    }

	void AIUpdate()
    {
		BackhCD--;
		if(BackhCD <= 0)
		{
			BackhCD = 10;
			CastSpellNowNoScheduling(Backh);
			ClearHateList();
			_unit->GetAIInterface()->GetNextTarget();
		}

		if(_unit->GetHealthPct() <= 50 && FRENZY_LIMITER == 0)
        {
			CastSpellNowNoScheduling(Frenzydesc);
			FRENZY_LIMITER = 1;
		}
		ParentClass::AIUpdate();
    }
	
protected:

	SpellDesc* Backh;
	SpellDesc* Frenzydesc;
	int BackhCD;
	Creature* Alexei;
	Creature* Ravenian;
	Creature* Illucia;
	Creature* Malicia;
	Creature* Krastinov;
	Creature* Polkelt;
	uint32 FRENZY_LIMITER;
};
		

class RattlegoreAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(RattlegoreAI, MoonScriptCreatureAI);
    RattlegoreAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(KNOCK_AWAY, Target_Current, 30, 0, 8);
		AddSpell(STRIKE, Target_Current, 25, 0, 3);
		AddSpell(WAR_STOMP, Target_Current, 12, 0, 5);
	}		
};

class MardukBlackpoolAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION(MardukBlackpoolAI, MoonScriptCreatureAI);
    MardukBlackpoolAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(MB_CLEAVE, Target_Current, 30, 0, 4);
		DefilingAura = AddSpell(MB_DEFILING_AURA, Target_Self, 0, 0, 0);
		AddSpell(MB_SHADOWBOLT_V, Target_Current, 15, 0, 0.5);
		ShadowShield = AddSpell(MB_SHADOW_SHIELD, Target_Self, 0, 0, 20);
	}		
	void OnCombatStart(Unit* pTarget)
    {
		Creature* Vectus = nullptr;
		Vectus = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(150.368f, 116.22f, 104.661f, 10432);
		Vectus->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What is this?! How dare you!");
		Vectus->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
		Creature* NewCreature = nullptr;

		for(int i = 0;i<32;i++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(StudentSpawn[i].mX, StudentSpawn[i].mY, StudentSpawn[i].mZ, 10475);
			NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 16);
			NewCreature->GetAIInterface()->SetAIState(STATE_ATTACKING);
			if(i>26)
			{
				NewCreature->GetAIInterface()->setMoveRunFlag(true);
				NewCreature->GetAIInterface()->MoveTo(150.368f, 116.22f, 104.661f, 3.83972f);
				NewCreature->GetAIInterface()->SetNextTarget(pTarget);
			}
			NewCreature = nullptr;
		}	
		Vectus->GetAIInterface()->setMoveRunFlag(true);
		Vectus->GetAIInterface()->MoveTo(150.368f, 116.22f, 104.661f, 3.83972f);
		Vectus->GetAIInterface()->SetNextTarget(pTarget);
		Vectus->GetAIInterface()->SetAllowedToEnterCombat(true);
		Vectus->GetAIInterface()->SetAIState(STATE_ATTACKING);
		ShieldCD = 15;
		CastSpellNowNoScheduling(DefilingAura);
		ParentClass::OnCombatStart(pTarget);
    }

	void AIUpdate()
    {	
		ShieldCD--;
		if(ShieldCD <= 0)
		{
			CastSpellNowNoScheduling(ShadowShield);
			ShieldCD = 45;
		}
		ParentClass::AIUpdate();
    }

protected:
	SpellDesc* DefilingAura;
	SpellDesc* ShadowShield;
	int ShieldCD;
};

class InstructorMaliciaAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(InstructorMaliciaAI, MoonScriptCreatureAI);
    InstructorMaliciaAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(CALL_OF_GRAVE, Target_RandomPlayer, 30, 0, 61);
		AddSpell(CORRUPTION, Target_RandomPlayer, 15, 2, 8);
		AddSpell(SLOW, Target_Current, 14, 0, 10);
		RenewDesc = AddSpell(RENEW, Target_Self, 0, 2, 0);
		HealDesc = AddSpell(HEAL, Target_Self, 0, 3.5, 0);
		FHealDesc = AddSpell(FLASH_HEAL, Target_Self, 0, 1.5, 0);
	}		

	void OnCombatStart(Unit* pTarget)
	{
		heals = false;
		HealCD = 7;
		ParentClass::OnCombatStart(pTarget);
	}
	void OnDied(Unit* pKiller)
    {
		Polkelt = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(274.877014f, 1.333660f, 85.311699f, 10901);
		Illucia = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(265.955994f, 0.903429f, 75.333397f, 10502);
		Krastinov = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(182.246002f, -95.437103f, 85.311699f, 11261);
		Ravenian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(103.305000f, -1.677520f, 75.218300f, 10507);
		Alexei = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(178.723999f, -91.023201f, 70.856796f, 10504);
		
		if((!Illucia || !Illucia->isAlive()) && (!Polkelt || !Polkelt->isAlive()) && (!Krastinov || !Krastinov->isAlive())
			 && (!Alexei || !Alexei->isAlive()) && (!Ravenian || !Ravenian->isAlive()))
		{
			Creature* Gandling = _unit->GetMapMgr()->GetInterface()->SpawnCreature(1853, 180.927994f, -9.374770f, 75.506699f, 1.610830f, true, false, 0, 0);
			Gandling->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "School is in session!");
		}
	   ParentClass::OnDied(pKiller);
    }
	
	void AIUpdate()
	{
		if(HealCD <= 0)
			heals = true;
		if(heals == false)
			HealCD--;
		if(_unit->GetHealthPct() <= 60 && heals == true)
		{
			int i = 0 + RandomFloat(3);
			switch(i)
			{
			case 1:
				{
					CastSpellNowNoScheduling(FHealDesc);
					HealCD = 7;
					heals = false;
				} break;
			case 2:
				{
					CastSpellNowNoScheduling(HealDesc);
					HealCD = 7;
					heals = false;
				} break;
			case 3:
				{
					CastSpellNowNoScheduling(RenewDesc);
					HealCD = 7;
					heals = false;
				} break;
			}
			
		}
		ParentClass::AIUpdate();
	}

protected:
	SpellDesc* FHealDesc;
	SpellDesc* HealDesc;
	SpellDesc* RenewDesc;

	bool heals;
	int HealCD;

	Creature* Alexei;
	Creature* Ravenian;
	Creature* Illucia;
	Creature* Malicia;
	Creature* Krastinov;
	Creature* Polkelt;
};

class TheRavenianAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION(TheRavenianAI, MoonScriptCreatureAI);
    TheRavenianAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(TRAMPLE, Target_Current, 20, 0, 5);
		AddSpell(RAVENIANCLEAVE, Target_Current, 15, 0, 4);
		AddSpell(SUNDERINCLEAVE, Target_Current, 12, 0, 7);
		AddSpell(KNOCKAWAY, Target_Current, 30, 0, 8);
	}		

	void OnDied(Unit* pKiller)
    {
		Polkelt = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(274.877014f, 1.333660f, 85.311699f, 10901);
		Illucia = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(265.955994f, 0.903429f, 75.333397f, 10502);
		Krastinov = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(182.246002f, -95.437103f, 85.311699f, 11261);
		Malicia = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(86.663399f, -1.960390f, 85.311699f, 10505);
		Alexei = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(178.723999f, -91.023201f, 70.856796f, 10504);
		
		if((!Illucia || !Illucia->isAlive()) && (!Polkelt || !Polkelt->isAlive()) && (!Krastinov || !Krastinov->isAlive()) && (!Malicia || !Malicia->isAlive())
			 && (!Alexei || !Alexei->isAlive()))
		{
			Creature* Gandling = _unit->GetMapMgr()->GetInterface()->SpawnCreature(1853, 180.927994f, -9.374770f, 75.506699f, 1.610830f, true, false, 0, 0);
			Gandling->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "School is in session!");
		}
		ParentClass::OnDied(pKiller);
    }

protected:
	Creature* Alexei;
	Creature* Ravenian;
	Creature* Illucia;
	Creature* Malicia;
	Creature* Krastinov;
	Creature* Polkelt;
};

class LadyIlluciaBarovAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION(LadyIlluciaBarovAI, MoonScriptCreatureAI);
    LadyIlluciaBarovAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(CURSE_OF_AGONY, Target_RandomPlayer, 13, 0, 10);
		AddSpell(SHADOW_SHOCK, Target_Current, 9, 0, 4);
		AddSpell(SILENCE, Target_RandomPlayer, 16, 1.5, 12);
		AddSpell(ILLUCIA_FEAR, Target_Current, 18, 1.5, 10);
		//AddSpell(DOMINATE_MIND, Target_RandomPlayer, 100, 2, 15);
	}		
	void AIUpdate()
	{
		ParentClass::AIUpdate();
	}

	void OnDied(Unit* pKiller)
    {
		Polkelt = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(274.877014f, 1.333660f, 85.311699f, 10901);
		Krastinov = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(182.246002f, -95.437103f, 85.311699f, 11261);
		Malicia = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(86.663399f, -1.960390f, 85.311699f, 10505);
		Ravenian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(103.305000f, -1.677520f, 75.218300f, 10507);
		Alexei = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(178.723999f, -91.023201f, 70.856796f, 10504);
		
		if((!Polkelt || !Polkelt->isAlive()) && (!Krastinov || !Krastinov->isAlive()) && (!Malicia || !Malicia->isAlive())
			 && (!Alexei || !Alexei->isAlive()) && (!Ravenian || !Ravenian->isAlive()))
		{
			Creature* Gandling = _unit->GetMapMgr()->GetInterface()->SpawnCreature(1853, 180.927994f, -9.374770f, 75.506699f, 1.610830f, true, false, 0, 0);
			Gandling->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "School is in session!");
		}
		ParentClass::OnDied(pKiller);
    }

protected:
	Creature* Alexei;
	Creature* Ravenian;
	Creature* Illucia;
	Creature* Malicia;
	Creature* Krastinov;
	Creature* Polkelt;
};

class RasFrostwhisperAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION(RasFrostwhisperAI, MoonScriptCreatureAI);
    RasFrostwhisperAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(FROSTBOLT, Target_Current, 30, 1.5, 0.5);
		IceArmor = AddSpell(ICE_ARMOR, Target_Self, 0, 0, 0);
		AddSpell(FREEZE, Target_RandomPlayer, 21, 0, 10);
		AddSpell(RAS_FEAR, Target_RandomPlayer, 9, 0, 11);
		AddSpell(CHILL_NOVA, Target_Self, 25, 0, 7);
		AddSpell(FROSTBOLT_VOLLEY, Target_Self, 20, 0, 5);
		AddSpell(RAS_KNOCK, Target_Current, 10, 0, 10);
	}		
	void OnCombatStart(Unit* pTarget)
    {
		CastSpellNowNoScheduling(IceArmor);
		ParentClass::OnCombatStart(pTarget);
    }
	SpellDesc* IceArmor;
};

class JandiceBarovAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION(JandiceBarovAI, MoonScriptCreatureAI);
    JandiceBarovAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		Summon = AddSpell(SUMMON_ILLUSION, Target_Self, 0, 0, 0);
		AddSpell(CURSE_OF_BLOOD, Target_RandomPlayer, 100, 2, 15);
		AddSpell(BANISH, Target_RandomPlayerNotCurrent, 100, 1, 10);	
	}		
	void OnCombatStart(Unit* pTarget)
    {
		SummonCD = 15;
		ParentClass::OnCombatStart(pTarget);
    }
	void AIUpdate()
	{
		SummonCD--;
		if(SummonCD <= 0)
		{

			CastSpellNowNoScheduling(Summon);	
			SummonAdds();
			SummonCD = 15;
		}
		ParentClass::AIUpdate();
	}
	void SummonAdds()
	{
		Creature* JandiceAdd = nullptr;
		for(int i=0;i<8;i++)
		{
			JandiceAdd = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11439, _unit->GetPositionX()+RandomFloat(3), _unit->GetPositionY()+RandomFloat(3), _unit->GetPositionZ(), 1.0f, true, false, 0,0);
			JandiceAdd->Despawn(14000, 0);
			JandiceAdd = nullptr;
		}
		for(int i=0;i<7;i++)
		{
			JandiceAdd = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11439, _unit->GetPositionX()-RandomFloat(3), _unit->GetPositionY()-RandomFloat(3), _unit->GetPositionZ(), 1.0f, true, false, 0,0);
			JandiceAdd->Despawn(14000, 0);
			JandiceAdd = nullptr;
		}
		DespawnAdds();
	}
	void DespawnAdds()
	{
		 _unit->Despawn(1000, 0);
		  Creature* JandiceSpawn = _unit->GetMapMgr()->GetInterface()->SpawnCreature(10503, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 3.0f, true, false, 0, 0);
		  JandiceSpawn->SetHealthPct(_unit->GetHealthPct());
	}

protected:
	int SummonCD;
	SpellDesc* Summon;
};

// KormokAI
//leave it till we release d2, needs a LOT of work
/*#define CN_KORMOK 14491	

#define SHADOW_BOLT_VOLLEY	20741            
#define BONE_SHIELD			27688
#define SUMMON_RISEY_LACKEY 17618	

class KormokAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(KormokAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    KormokAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		}

		spells[0].info = dbcSpell.LookupEntry(SHADOW_BOLT_VOLLEY);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].perctrigger = 11.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(BONE_SHIELD);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SUMMON_RISEY_LACKEY);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 4.0f;
		spells[2].attackstoptimer = 1000;
	}		
	void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(1000);
		_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

	void OnDied(Unit* mKiller)
    {
       RemoveAIUpdateEvent();
    }

	void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }
    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == nullptr && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = nullptr;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						target = _unit->GetAIInterface()->GetNextTarget();
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); 
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void Destroy()
	{
		delete this;
	};

protected:

	int nrspells;
};
*/
// VectusAI

class VectusAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION(VectusAI, MoonScriptCreatureAI);
    VectusAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		FRENZY_LIMITER = 0;
		AddSpell(FLAMESTRIKE, Target_Destination, 21, 3, 10);
		AddSpell(BLAST_WAVE, Target_Self, 30, 0, 6);
		Frenzy = AddSpell(V_FRENZY, Target_Self, 0, 0, 0);
	}		
	void OnCombatStart(Unit* pTarget)
    {
		FRENZY_LIMITER = 0;
		ParentClass::OnCombatStart(pTarget);
    }

    void OnCombatStop(Unit* pTarget)
    {
		FRENZY_LIMITER = 0;
		ParentClass::OnCombatStop(pTarget);
    }

	void OnDied(Unit* pKiller)
    {
		FRENZY_LIMITER = 0;
		ParentClass::OnDied(pKiller);
    }
	
protected:
	SpellDesc* Frenzy;
	uint32 FRENZY_LIMITER;
};

class LordAlexeiBarovAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION(LordAlexeiBarovAI, MoonScriptCreatureAI);
    LordAlexeiBarovAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		UnholyAura = AddSpell(UNHOLY_AURA, Target_Self, 0, 0, 0);
		AddSpell(VEIL_OF_SHADOW, Target_Current, 30, 1.5, 13);
	}		
	void OnCombatStart(Unit* pTarget)
    {
        CastSpellNowNoScheduling(UnholyAura);
		ParentClass::OnCombatStart(pTarget);
    }

	void OnDied(Unit* pKiller)
    {
		Polkelt = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(274.877014f, 1.333660f, 85.311699f, 10901);
		Illucia = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(265.955994f, 0.903429f, 75.333397f, 10502);
		Krastinov = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(182.246002f, -95.437103f, 85.311699f, 11261);
		Malicia = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(86.663399f, -1.960390f, 85.311699f, 10505);
		Ravenian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(103.305000f, -1.677520f, 75.218300f, 10507);
		
		if((!Illucia || !Illucia->isAlive()) && (!Polkelt || !Polkelt->isAlive()) && (!Krastinov || !Krastinov->isAlive()) && (!Malicia || !Malicia->isAlive())
			 && (!Ravenian || !Ravenian->isAlive()))
		{
			Creature* Gandling = _unit->GetMapMgr()->GetInterface()->SpawnCreature(1853, 180.927994f, -9.374770f, 75.506699f, 1.610830f, true, false, 0, 0);
			Gandling->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "School is in session!");
		}
		ParentClass::OnDied(pKiller);
    }

protected:
	SpellDesc* UnholyAura;
	Creature* Alexei;
	Creature* Ravenian;
	Creature* Illucia;
	Creature* Malicia;
	Creature* Krastinov;
	Creature* Polkelt;
};

class LorekeeperPolkeltAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION(LorekeeperPolkeltAI, MoonScriptCreatureAI);
    LorekeeperPolkeltAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(VOLATILE_INFECTION, Target_RandomPlayer, 17, 2, 8);
		AddSpell(CORROSIVE_ACID, Target_Current, 24, 1.5, 10);
		AddSpell(NOXIOUS_CATALYST, Target_Current, 19, 2, 15);
	}		

	void OnDied(Unit* pKiller)
    {
		Illucia = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(265.955994f, 0.903429f, 75.333397f, 10502);
		Krastinov = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(182.246002f, -95.437103f, 85.311699f, 11261);
		Malicia = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(86.663399f, -1.960390f, 85.311699f, 10505);
		Alexei = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(178.723999f, -91.023201f, 70.856796f, 10504);
		Ravenian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(103.305000f, -1.677520f, 75.218300f, 10507);
		
		if((!Illucia || !Illucia->isAlive()) && (!Krastinov || !Krastinov->isAlive()) && (!Malicia || !Malicia->isAlive())
			 && (!Alexei || !Alexei->isAlive()) && (!Ravenian || !Ravenian->isAlive()))
		{
			Creature* Gandling = _unit->GetMapMgr()->GetInterface()->SpawnCreature(1853, 180.927994f, -9.374770f, 75.506699f, 1.610830f, true, false, 0, 0);
			Gandling->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "School is in session!");
		}
		ParentClass::OnDied(pKiller);
    }

protected:
	Creature* Alexei;
	Creature* Ravenian;
	Creature* Illucia;
	Creature* Malicia;
	Creature* Krastinov;
	Creature* Polkelt;
};

class DarkmasterGandlingAI : public MoonScriptCreatureAI
{
public:
    MOONSCRIPT_FACTORY_FUNCTION(DarkmasterGandlingAI, MoonScriptCreatureAI);
    DarkmasterGandlingAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		Plr = nullptr;
		Gatea = Gateb = Gatec = Gated = Gatee = Gatef = nullptr;
		Miniona = Minionb = Minionc = Miniond = Minione = Minionf = Miniong = Minionh = Minioni = nullptr;
		Minionj = Minionk = Minionl = Minionm = Minionn = Miniono = Minionp = Minionq = Minionr = nullptr;
		closegatea = closegateb = closegatec = closegated = closegatee = closegated = false;
		PortID = 0;
		
		AddSpell(ARCANE_MISSILES, Target_Current, 30, 1, 1);
		AddSpell(CURSE_OF_THE_DARKMASTER, Target_RandomPlayer, 20, 2, 10);
		AddSpell(SHADOW_SHIELD, Target_Self, 30, 0, 12);
		ShadowPortal = AddSpell(SHADOW_PORTAL, Target_RandomPlayer, 0, 1, 0);
	}		

	void OnCombatStart(Unit* pTarget)
    {
		_unit->GetAIInterface()->setOutOfCombatRange(100000);
		TeleportCD = 20;
		Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ());

		GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(179.865280f, 27.955908f, 88.738251f, 177374);
		Gate->SetUInt32Value(GAMEOBJECT_STATE, 1);
		Gate->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
        ParentClass::OnCombatStart(pTarget);
    }

    void OnCombatStop(Unit* pTarget)
    {
		TeleportCD = 20;
		GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(179.865280f, 27.955908f, 88.738251f, 177374);
		Gate->SetUInt32Value(GAMEOBJECT_STATE, 0);
		Gate->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		OpenDoors();

		ParentClass::OnCombatStop(pTarget);
    }

	void OnDied(Unit* pKiller)
    {
		TeleportCD = 20;
		OpenDoors();

		GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(179.865280f, 27.955908f, 88.738251f, 177374);
		Gate->SetUInt32Value(GAMEOBJECT_STATE, 0);
		Gate->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		ParentClass::OnDied(pKiller);
    }

	void AIUpdate()
    {
		if(Plr == nullptr)
		{
			Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ());
			if(Plr == nullptr)
			{
				switch (PortID)
				{
				case 1:
					Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(265.955994f, 0.903429f, 75.333397f);
					break;
				case 2:
					Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(103.305000f, -1.677520f, 75.218300f);
					break;
				case 3:
					Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(178.723999f, -91.023201f, 70.856796f);
					break;
				case 4:
					Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(86.663399f, -1.960390f, 85.311699f);
					break;
				case 5:
					Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(274.877014f, 1.333660f, 85.311699f);
					break;
				case 6:
					Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(182.246002f, -95.437103f, 85.311699f);
					break;
				}
			}
		}
		
		TeleportCD--;
		if(TeleportCD <= 0)
		{
			TeleportCD = 15;
			CastSpellNowNoScheduling(ShadowPortal);
			TeleportPlayer();
		}

		if(PortID > 0)
		{
			switch(PortID)
			{
			case 1:
				{
				GameObject* Gatea = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(204.326f, 0.011124f, 76.1678f, 177371);
				if(Gatea != nullptr && (!Miniona->isAlive() && !Minionb->isAlive() && !Minionc->isAlive()))
				{
					Gatea->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
					Gatea->SetUInt32Value(GAMEOBJECT_STATE, 0);	
					closegatea = false;
				}
				} break;
			case 2:
				{
				GameObject* Gateb = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(159.746f, -0.767021f, 76.1678f, 177372);
				if(Gateb != nullptr && (!Miniond->isAlive() && !Minione->isAlive() && !Minionf->isAlive()))
				{
					Gateb->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
					Gateb->SetUInt32Value(GAMEOBJECT_STATE, 0);	
					closegateb = false;
				}
				} break;
			case 3:
				{
				GameObject* Gatec = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(181.054f, -21.3466f, 76.1317f, 177373);
				if(Gatec != nullptr && (!Miniong->isAlive() && !Minionh->isAlive() && !Minioni->isAlive()))
				{
					Gatec->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
					Gatec->SetUInt32Value(GAMEOBJECT_STATE, 0);
					closegatec = false;
				}
				} break;
			case 4:
				{
				GameObject* Gated = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(151.773f, -0.602287f, 88.9039f, 177375);
				if(Gated != nullptr && (!Minionj->isAlive() && !Minionk->isAlive() && !Minionl->isAlive()))
				{
					Gated->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
					Gated->SetUInt32Value(GAMEOBJECT_STATE, 0);		
					closegated = false;
				} 
				} break;
			case 5:
				{
				GameObject* Gatee = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(209.065f, 0.011116f, 88.9113f, 177376);
				if(Gatee != nullptr && (!Minionm->isAlive() && !Minionn->isAlive() && !Miniono->isAlive()))
				{
					Gatee->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
					Gatee->SetUInt32Value(GAMEOBJECT_STATE, 0);
					closegatee = false;
				}
				} break;
			case 6:
				{
				GameObject* Gatef = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(181.244f, -29.1116f, 88.9104f, 177377);
				if(Gatef != nullptr && (!Minionp->isAlive() && !Minionq->isAlive() && !Minionr->isAlive()))
				{
					Gatef->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
					Gatef->SetUInt32Value(GAMEOBJECT_STATE, 0);	
					closegatef = false;
				}
				} break;
			}
		}
		ParentClass::AIUpdate();
    }
	
	void TeleportPlayer()
	{
		Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ());

		if(!Plr)
			return;

		int SpawnLocation = RandomFloat(6);
		switch(SpawnLocation)
		{
		case 1:
		{
			closegatea = true;
			PortID = 1;
			Plr->SafeTeleport(289, Plr->GetInstanceID(), 265.955994f, 0.903429f, 75.333397f, 1.0f);

			ClearHateList();
			AggroNearestUnit();

			GameObject* Gatea = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(204.326f, 0.011124f, 76.1678f, 177371);
			Gatea->SetUInt32Value(GAMEOBJECT_STATE, 1);
			Gatea->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

			Miniona = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 226.562531f, -6.939580f, 72.673050f, 1.701396f, true, false, 0, 0);

			WayPoint * wp = Miniona->CreateWaypointStruct();
			wp->id = 1;
			wp->x = 225.104904f;
			wp->y = 6.399918f;
			wp->z = 72.673050f;
			wp->o = 2.121980f;
			wp->waittime = 0;
			wp->flags = WALK;
			wp->forwardemoteoneshot = 0;
			wp->forwardemoteid = 0;
			wp->backwardemoteoneshot = 0;
			wp->backwardemoteid = 0;
			wp->forwardskinid = 0;
			wp->backwardskinid = 0;
			Miniona->GetAIInterface()->addWayPoint(wp);

			Miniona->GetAIInterface()->StopMovement(0);
			Miniona->GetAIInterface()->setWaypointToMove(0);
			Miniona->Despawn(600000, 0);

			Minionb = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 233.114746f, -0.432222f, 72.673050f, 3.103729f, true, false, 0, 0);

			WayPoint * wps = Minionb->CreateWaypointStruct();
			wps->id = 1;
			wps->x = 215.365555f;
			wps->y = -0.317770f;
			wps->z = 72.671555f;
			wps->o = 3.179128f;
			wps->waittime = 0;
			wps->flags = WALK;
			wps->forwardemoteoneshot = 0;
			wps->forwardemoteid = 0;
			wps->backwardemoteoneshot = 0;
			wps->backwardemoteid = 0;
			wps->forwardskinid = 0;
			wps->backwardskinid = 0;
			Minionb->GetAIInterface()->addWayPoint(wps);

			Minionb->GetAIInterface()->StopMovement(0);
			Minionb->GetAIInterface()->setWaypointToMove(0);
			Minionb->Despawn(600000, 0);

			Minionc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 222.168152f, -5.164414f, 72.671791f, 0.158496f, true, false, 0, 0);
			Minionc->Despawn(60000, 0);

		} break;
		case 2:
		{
			closegateb = true;
			PortID = 2;
			Plr->SafeTeleport(289, Plr->GetInstanceID(), 103.305000f, -1.677520f, 75.218300f, 1.0f);

			ClearHateList();
			AggroNearestUnit();

			GameObject* Gateb = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(159.746f, -0.767021f, 76.1678f, 177372);
			Gateb->SetUInt32Value(GAMEOBJECT_STATE, 1);
			Gateb->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

			Miniond = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 139.879272f, 9.124151f, 75.398422f, 3.426537f, true, false, 0, 0);

			WayPoint * wp = Miniond->CreateWaypointStruct();
			wp->id = 1;
			wp->x = 129.850647f;
			wp->y = 6.186618f;
			wp->z = 75.398422f;
			wp->o = 3.045619f;
			wp->waittime = 0;
			wp->flags = WALK;
			wp->forwardemoteoneshot = 0;
			wp->forwardemoteid = 0;
			wp->backwardemoteoneshot = 0;
			wp->backwardemoteid = 0;
			wp->forwardskinid = 0;
			wp->backwardskinid = 0;
			Miniond->GetAIInterface()->addWayPoint(wp);

			Miniond->GetAIInterface()->StopMovement(0);
			Miniond->GetAIInterface()->setWaypointToMove(0);
			Miniond->Despawn(600000, 0);

			Minione = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 139.107758f, -6.609929f, 75.398422f, 3.045619f, true, false, 0, 0);

			WayPoint * wps = Minione->CreateWaypointStruct();
			wps->id = 1;
			wps->x = 129.940140f;
			wps->y = 6.186618f;
			wps->z = 75.398422f;
			wps->o = 3.045619f;
			wps->waittime = 0;
			wps->flags = WALK;
			wps->forwardemoteoneshot = 0;
			wps->forwardemoteid = 0;
			wps->backwardemoteoneshot = 0;
			wps->backwardemoteid = 0;
			wps->forwardskinid = 0;
			wps->backwardskinid = 0;
			Minione->GetAIInterface()->addWayPoint(wps);

			Minione->GetAIInterface()->StopMovement(0);
			Minione->GetAIInterface()->setWaypointToMove(0);
			Minione->Despawn(600000, 0);

			Minionf = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 130.073456f, -1.218730f, 75.398422f, 3.165784f, true, false, 0, 0);
			Minionf->Despawn(60000, 0);

		} break;
		case 3:
		{
			closegatec = true;
			PortID = 3;
			Plr->SafeTeleport(289, Plr->GetInstanceID(), 178.723999f, -91.023201f, 70.856796f, 1.0f);

			ClearHateList();
			AggroNearestUnit();

			GameObject* Gatec = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(181.054f, -21.3466f, 76.1317f, 177373);
			Gatec->SetUInt32Value(GAMEOBJECT_STATE, 1);
			Gatec->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

			Miniong = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 188.2821014f, -45.184914f, 75.398392f, 4.830834f, true, false, 0, 0);
			Miniong->Despawn(60000, 0);

			Minionh = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 181.723648f, -44.317497f, 75.398392f, 4.830834f, true, false, 0, 0);
			Minionh->Despawn(60000, 0);

			Minioni = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 174.579865f, -44.336052f, 75.398392f, 4.830834f, true, false, 0, 0);
			Minioni->Despawn(60000, 0);

		} break;
		case 4:
		{
			closegated = true;
			PortID = 4;
			Plr->SafeTeleport(289, Plr->GetInstanceID(), 86.663399f, -1.960390f, 85.311699f, 1.0f);

			ClearHateList();
			AggroNearestUnit();

			GameObject* Gated = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(151.773f, -0.602287f, 88.9039f, 177375);
			Gated->SetUInt32Value(GAMEOBJECT_STATE, 1);
			Gated->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

			Minionj = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 118.282600f, -7.205003f, 85.228279f, 2.845497f, true, false, 0, 0);

			WayPoint * wp = Minionj->CreateWaypointStruct();
			wp->id = 1;
			wp->x = 108.705986f;
			wp->y = 1.596299f;
			wp->z = 85.376427f;
			wp->o = 2.319280f;
			wp->waittime = 0;
			wp->flags = WALK;
			wp->forwardemoteoneshot = 0;
			wp->forwardemoteid = 0;
			wp->backwardemoteoneshot = 0;
			wp->backwardemoteid = 0;
			wp->forwardskinid = 0;
			wp->backwardskinid = 0;
			Minionj->GetAIInterface()->addWayPoint(wp);

			Minionj->GetAIInterface()->StopMovement(0);
			Minionj->GetAIInterface()->setWaypointToMove(0);
			Minionj->Despawn(600000, 0);

			Minionk = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 108.074348f, -10.772270f, 85.228271f, 0.834876f, true, false, 0, 0);

			WayPoint * wps = Minionk->CreateWaypointStruct();
			wps->id = 1;
			wps->x = 117.339935f;
			wps->y = 3.634001f;
			wps->z = 85.376427f;
			wps->o = 1.035152f;
			wps->waittime = 0;
			wps->flags = WALK;
			wps->forwardemoteoneshot = 0;
			wps->forwardemoteid = 0;
			wps->backwardemoteoneshot = 0;
			wps->backwardemoteid = 0;
			wps->forwardskinid = 0;
			wps->backwardskinid = 0;
			Minionk->GetAIInterface()->addWayPoint(wps);

			Minionk->GetAIInterface()->StopMovement(0);
			Minionk->GetAIInterface()->setWaypointToMove(0);
			Minionk->Despawn(600000, 0);

			Minionl = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 120.994621f, -1.306361f, 85.228271f, 3.321448f, true, false, 0, 0);
			Minionl->Despawn(60000, 0);

		} break;
		case 5:
		{
			closegatee = true;
			PortID = 5;
			Plr->SafeTeleport(289, Plr->GetInstanceID(), 274.877014f, 1.333660f, 85.311699f, 1.0f);

			ClearHateList();
			AggroNearestUnit();

			GameObject* Gatee = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(209.065f, 0.011116f, 88.9113f, 177376);
			Gatee->SetUInt32Value(GAMEOBJECT_STATE, 1);
			Gatee->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

			Minionm = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 245.526672f, -2.554565f, 84.840790f, 6.261378f, true, false, 0, 0);
			Minionm->Despawn(60000, 0);

			Minionn = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 240.948685f, 0.337413f, 84.840790f, 6.261378f, true, false, 0, 0);
			Minionn->Despawn(60000, 0);

			Miniono = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 244.533249f, 3.470865f, 84.840790f, 6.261378f, true, false, 0, 0);
			Miniono->Despawn(60000, 0);

		} break;
		case 6:
		{
			closegatef = true;
			PortID = 6;
			Plr->SafeTeleport(289, Plr->GetInstanceID(), 182.246002f, -95.437103f, 85.311699f, 1.0f);

			ClearHateList();
			AggroNearestUnit();

			GameObject* Gatef = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(181.244f, -29.1116f, 88.9104f, 177377);
			Gatef->SetUInt32Value(GAMEOBJECT_STATE, 1);
			Gatef->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

			Minionp = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 188.614746f, -52.012680f, 85.228271f, 4.796611f, true, false, 0, 0);
			Minionp->Despawn(60000, 0);

			Minionq = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 181.682953f, -52.801571f, 85.228271f, 4.796611f, true, false, 0, 0);
			Minionq->Despawn(60000, 0);

			Minionr = _unit->GetMapMgr()->GetInterface()->SpawnCreature(11598, 174.469116f, -52.266697f, 85.228271f, 4.796611f, true, false, 0, 0);
			Minionr->Despawn(60000, 0);

		} break;
		}
	}
	
	void OpenDoors()
	{
		int i = 1;
		for(i = 1; i < 7; i++)
		{
			switch(i)
			{
			case 1:
				{
					if(closegatea == true)
					{
						GameObject* Gatea = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(204.326f, 0.011124f, 76.1678f, 177371);
						if(Gatea != nullptr)
						{
							Gatea->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
							Gatea->SetUInt32Value(GAMEOBJECT_STATE, 0);	
							closegatea = false;
						}
					}
				} break;
			case 2:
				{
					if(closegateb == true)
					{
						GameObject* Gateb = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(159.746f, -0.767021f, 76.1678f, 177372);
						if(Gateb != nullptr)
						{
							Gateb->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
							Gateb->SetUInt32Value(GAMEOBJECT_STATE, 0);	
							closegateb = false;
						}
					}
				} break;
			case 3:
				{
					if(closegatec == true)
					{
						GameObject* Gatec = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(181.054f, -21.3466f, 76.1317f, 177373);
						if(Gatec != nullptr)
						{
							Gatec->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
							Gatec->SetUInt32Value(GAMEOBJECT_STATE, 0);
							closegatec = false;
						}
					}
				} break;
			case 4:
				{
					if(closegated == true)
					{
						GameObject* Gated = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(151.773f, -0.602287f, 88.9039f, 177375);
						if(Gated != nullptr)
						{
							Gated->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
							Gated->SetUInt32Value(GAMEOBJECT_STATE, 0);	
							closegated = false;
						}
					}
				} break;
			case 5:
				{
					if(closegatee == true)
					{
						GameObject* Gatee = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(209.065f, 0.011116f, 88.9113f, 177376);
						if(Gatee != nullptr)
						{
							Gatee->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
							Gatee->SetUInt32Value(GAMEOBJECT_STATE, 0);
							closegatee = false;
						}
					}
				} break;
			case 6:
				{
					if(closegatef == true)
					{
						GameObject* Gatef = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(181.244f, -29.1116f, 88.9104f, 177377);
						if(Gatef != nullptr)
						{
							Gatef->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
							Gatef->SetUInt32Value(GAMEOBJECT_STATE, 0);			
							closegatef = false;
						}
					}
				} break;
			}
		}
	}
	
protected:
	SpellDesc* ShadowPortal;
	SpellDesc* ShadowShield;

	GameObject* Gatea;
	GameObject* Gateb;
	GameObject* Gatec;
	GameObject* Gated;
	GameObject* Gatee;
	GameObject* Gatef;

	Creature* Miniona;
	Creature* Minionb;
	Creature* Minionc;
	Creature* Miniond;
	Creature* Minione;
	Creature* Minionf;
	Creature* Miniong;
	Creature* Minionh;
	Creature* Minioni;
	Creature* Minionj;
	Creature* Minionk;
	Creature* Minionl;
	Creature* Minionm;
	Creature* Minionn;
	Creature* Miniono;
	Creature* Minionp;
	Creature* Minionq;
	Creature* Minionr;

	Player* Plr;

	bool closegatea;
	bool closegateb;
	bool closegatec;
	bool closegated;
	bool closegatee;
	bool closegatef;

	int PortID;
	int TeleportCD;

};

class JandiceTrigger : public GameObjectAIScript
{
public:
	JandiceTrigger(GameObject* goinstance) : GameObjectAIScript(goinstance) {
	}	
	void OnActivate( Player* pPlayer )
	{	
		GameObject* SecretDoor = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(172.046f, 99.4433f, 105.135f, 175610);
		SecretDoor->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		SecretDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
	}
		static GameObjectAIScript *Create(GameObject* GO) { return new BrazierOfTheHerald(GO); }
};

void SetupScholomanceBosses(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_KIRTONOS, &KirtonosAI::Create);
	mgr->register_creature_script(11261, &DoctorTheolenKrastinovAI::Create);
	mgr->register_creature_script(CN_RATTLEGORE, &RattlegoreAI::Create);
	mgr->register_creature_script(CN_MARDUK_BLACKPOOL, &MardukBlackpoolAI::Create);
	mgr->register_creature_script(10505, &InstructorMaliciaAI::Create);
	mgr->register_creature_script(10507, &TheRavenianAI::Create);
	mgr->register_creature_script(10502, &LadyIlluciaBarovAI::Create);
	mgr->register_creature_script(CN_RAS_FROSTWHISPER, &RasFrostwhisperAI::Create);
	mgr->register_creature_script(CN_JANDICE_BAROV, &JandiceBarovAI ::Create);
	//mgr->register_creature_script(CN_KORMOK, &KormokAI::Create);
	mgr->register_creature_script(CN_VECTUS, &VectusAI::Create);
	mgr->register_creature_script(10504, &LordAlexeiBarovAI::Create);
	mgr->register_creature_script(10901, &LorekeeperPolkeltAI::Create);
	mgr->register_creature_script(CN_DARKMASTER_GANDLING, &DarkmasterGandlingAI::Create);
	mgr->register_gameobject_script(600001, &BrazierOfTheHerald::Create);
	mgr->register_gameobject_script(177385, &JandiceTrigger::Create);
	//mgr->register_gameobject_script(179724, &SkeletonDoor::Create);
}
