

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"

/************************************************************************/
/*  Instance_RazorfenDowns.cpp Script									*/
/************************************************************************/

#define AMNENNARS_WRATH 13009
#define FROST_NOVA		15531
#define FROSTBOLT		15530
#define SUMMON_FROST	12642

class AmnennarTheColdbringer : public MoonScriptCreatureAI
{
protected:
	bool m_summon2;
	bool m_summon1;
	SpellDesc* SummonFrostSpectres;
public:
	MOONSCRIPT_FACTORY_FUNCTION(AmnennarTheColdbringer, MoonScriptCreatureAI);
    AmnennarTheColdbringer(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(AMNENNARS_WRATH, Target_Current, 30, 0, 10);
		AddSpell(FROST_NOVA, Target_Current, 15, 0, 15);
		AddSpell(FROSTBOLT, Target_Current, 50, 3, 0);
		SummonFrostSpectres = AddSpell(SUMMON_FROST, Target_Self, 0, 0, 0);
	}
    
    void OnCombatStart(Unit* pTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You'll never leave this place alive...");
		_unit->PlaySoundToSet(5825);
		m_summon1 = true;
		m_summon2 = true;
		ParentClass::OnCombatStart(pTarget);
    }

	void OnTargetDied(Unit* pTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Too... Easy.");
		_unit->PlaySoundToSet(5826);
		ParentClass::OnTargetDied(pTarget);
    }

    void OnDied(Unit* pKiller)
    {
		_unit->PlaySoundToSet(5827);
		ParentClass::OnDied(pKiller);
    }

	void AIUpdate()
	{
		if(m_summon1 && GetHealthPercent() <= 70)
		{
			m_summon1 = false;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "To me, my servants!");
			_unit->PlaySoundToSet(5828);
			CastSpellNowNoScheduling(SummonFrostSpectres);
		}
		else if(m_summon2 && GetHealthPercent() <= 30)
		{
			m_summon2 = false;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Come, spirits - attend your master!");
			_unit->PlaySoundToSet(5829);
			CastSpellNowNoScheduling(SummonFrostSpectres);
		}
		ParentClass::AIUpdate();
	}
};

#define DISEASE_CLOUD	12627
#define FRENZY			12795

class Glutton : public MoonScriptCreatureAI
{
	SpellDesc* mFrenzy;
	int frenzyTimer;

	MOONSCRIPT_FACTORY_FUNCTION(Glutton, MoonScriptCreatureAI);
	Glutton(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		//spells
		ApplyAura(DISEASE_CLOUD);
		mFrenzy = AddSpell(FRENZY, Target_Self, 0, 0, 0);
	}

	void AIUpdate() {
		if(IsTimerFinished(frenzyTimer)) {
			ResetTimer(frenzyTimer, 12000);
			CastSpellNowNoScheduling(mFrenzy);
			Emote("Glutton is getting hungry!", Text_Emote);
		}
		ParentClass::AIUpdate();
	}

	void OnCombatStart(Unit* pTarget)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Me smell stench of the living!");
		_unit->PlaySoundToSet(5823);
		frenzyTimer = AddTimer(12000);
		ParentClass::OnCombatStart(pTarget);
	}
};

#define FIRE_NOVA		12470
#define FIREBALL		12466

class MordreshFireEye : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MordreshFireEye, MoonScriptCreatureAI);
	MordreshFireEye(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(FIRE_NOVA, Target_Self, 10, 2, 10);
		AddSpell(FIREBALL, Target_Current, 50, 3, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define PUTRID_STENCH	12946

class PlaguemawTheRotting : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(PlaguemawTheRotting, MoonScriptCreatureAI);
    PlaguemawTheRotting(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(PUTRID_STENCH, Target_RandomPlayer, 15, 0, 30);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define DOMINATE_MIND	7645
#define R_HEAL			12039
#define SHADOW_BOLT		12471
#define SW_PAIN			11639

class Ragglesnout : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Ragglesnout, MoonScriptCreatureAI);
    Ragglesnout(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DOMINATE_MIND, Target_RandomPlayer, 30, 2, 20);
		AddSpell(R_HEAL, Target_Self, 10, 3, 10);
		AddSpell(SHADOW_BOLT, Target_Current, 30, 3, 0);
		AddSpell(SW_PAIN, Target_RandomPlayer, 30, 0, 15);
    }
	
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

static Coords LeftWP[]=
{
	{ 0, 0, 0, 0 },
	{ 2503.136963f, 847.830383f, 47.618874f, 0.894241f },
	{ 2522.405029f, 867.359741f, 47.678150f, 0.462269f },
	{ 2539.962891f, 861.849854f, 50.216164f, 5.759779f }
};
static Coords RightWP[]=
{
	{ 0, 0, 0, 0 },
	{ 2541.868164f, 880.968079f, 47.697533f, 0.803934f },
	{ 2527.501465f, 866.749756f, 47.678471f, 3.635294f },
	{ 2518.943604f, 852.886780f, 47.678471f, 4.518867f },
	{ 2540.315430f, 849.281677f, 50.280037f, 6.282084f }
};
static Coords MiddleWP[]=
{
	{ 0, 0, 0, 0 },
	{ 2515.131348f, 848.164795f, 47.678207f, 1.491149f },
	{ 2522.227051f, 862.957397f, 47.678207f, 1.019909f },
	{ 2537.234863f, 864.401733f, 48.923866f, 6.062165f }
};

#define CN_TOMB_FIEND		7349
class TombFiendAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(TombFiendAI, MoonScriptCreatureAI);
    TombFiendAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
    }
};

#define GO_FLAG_UNCLICKABLE 0x00000010
#define RUN 256

class GongOfRazorfen : public GameObjectAIScript
{
public:
	GongOfRazorfen(GameObject* goinstance) : GameObjectAIScript(goinstance) {
		
		m_spawn = m_active = false;
		waves = 0;
		Lefta = Leftb = Leftc = Leftd = Lefte = Middlea = Middleb = Righta = Rightb = Rightc = Rightd = EliteLefta = EliteLeftb = EliteRighta = EliteRightb = Tutenkash = nullptr;
	}

	void OnActivate( Player* pPlayer )
	{		
		int i = 0;
		
		switch (waves) {
			case 0: {
					//5 Tomb fiends on the left side
					Lefta = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7349, 2489.9375f, 828.71814f, 44.181084f, 1.145883f, true, false, 0, 0);
					Lefta = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2489.9375f, 828.71814f, 44.181084f, 7349);
					if(Lefta)
					{
						for(i=1;i<4;i++)
						{
							WayPoint * wp = Lefta->CreateWaypointStruct();
							wp->id = i;
							wp->x = LeftWP[i].mX;
							wp->y = LeftWP[i].mY;
							wp->z = LeftWP[i].mZ;
							wp->o = LeftWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Lefta->GetAIInterface()->addWayPoint(wp);
						}
						Lefta->GetAIInterface()->StopMovement(0);
						Lefta->GetAIInterface()->setWaypointToMove(0);
						Lefta->Despawn(600000, 0);	
					}

					Leftb = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7349, 2491.322021f, 831.42151f, 44.714779f, 0.948184f, true, false, 0, 0);	
					Leftb = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2491.322021f, 831.42151f, 44.714779f, 7349);
					if(Leftb)
					{
						for(i=1;i<4;i++)
						{
							WayPoint * wp = Leftb->CreateWaypointStruct();
							wp->id = i;
							wp->x = LeftWP[i].mX;
							wp->y = LeftWP[i].mY;
							wp->z = LeftWP[i].mZ;
							wp->o = LeftWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Leftb->GetAIInterface()->addWayPoint(wp);
						}
						Leftb->GetAIInterface()->StopMovement(0);
						Leftb->GetAIInterface()->setWaypointToMove(0);
						Leftb->Despawn(600000, 0);	
					}

					Leftc = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7349, 2487.222656f, 830.148438f, 43.851303f, 0.590827f, true, false, 0, 0);	
					Leftc = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2487.222656f, 830.148438f, 43.851303f, 7349);
					if(Leftc)
					{
						for(i=1;i<4;i++)
						{
							WayPoint * wp = Leftc->CreateWaypointStruct();
							wp->id = i;
							wp->x = LeftWP[i].mX;
							wp->y = LeftWP[i].mY;
							wp->z = LeftWP[i].mZ;
							wp->o = LeftWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Leftc->GetAIInterface()->addWayPoint(wp);
						}
						Leftc->GetAIInterface()->StopMovement(0);
						Leftc->GetAIInterface()->setWaypointToMove(0);
						Leftc->Despawn(600000, 0);
					}

					Leftd = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7349, 2492.007080f, 827.447144f, 44.594097f, 1.124897f, true, false, 0, 0);
					Leftd = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2492.007080f, 827.447144f, 44.594097f, 7349);
					if(Leftd)
					{
						for(i=1;i<4;i++)
						{
							WayPoint * wp = Leftd->CreateWaypointStruct();
							wp->id = i;
							wp->x = LeftWP[i].mX;
							wp->y = LeftWP[i].mY;
							wp->z = LeftWP[i].mZ;
							wp->o = LeftWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Leftd->GetAIInterface()->addWayPoint(wp);
						}
						Leftd->GetAIInterface()->StopMovement(0);
						Leftd->GetAIInterface()->setWaypointToMove(0);
						Leftd->Despawn(600000, 0);
					}

					Lefte = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7349, 2488.033936f, 826.437134f, 43.707855f, 0.865715f,  true, false, 0, 0); 
					Lefte = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2488.033936f, 826.437134f, 43.707855f, 7349);
					if(Lefte)
					{
						for(i=1;i<4;i++)
						{
							WayPoint * wp = Lefte->CreateWaypointStruct();
							wp->id = i;
							wp->x = LeftWP[i].mX;
							wp->y = LeftWP[i].mY;
							wp->z = LeftWP[i].mZ;
							wp->o = LeftWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Lefte->GetAIInterface()->addWayPoint(wp);
						}
						Lefte->GetAIInterface()->StopMovement(0);
						Lefte->GetAIInterface()->setWaypointToMove(0);
						Lefte->Despawn(600000, 0);
					}

					//2 Tomb fiends in the middle
					Middlea = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7349, 2526.669189f, 833.028137f, 48.029835f, 1.868563f, true, false, 0, 0);
					Middlea = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2526.669189f, 833.028137f, 48.029835f, 7349);
					if(Middlea)
					{
						for(i=1;i<4;i++)
						{
							WayPoint * wp = Middlea->CreateWaypointStruct();
							wp->id = i;
							wp->x = MiddleWP[i].mX;
							wp->y = MiddleWP[i].mY;
							wp->z = MiddleWP[i].mZ;
							wp->o = MiddleWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Middlea->GetAIInterface()->addWayPoint(wp);
						}
						Middlea->GetAIInterface()->StopMovement(0);
						Middlea->GetAIInterface()->setWaypointToMove(0);
						Middlea->Despawn(600000, 0);
					}

					Middleb = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7349, 2522.113770f, 831.423828f, 48.344925f, 1.528463f, true, false, 0, 0);
					Middleb = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2522.113770f, 831.423828f, 48.344925f, 7349);
					if(Middleb)
					{
						for(i=1;i<4;i++)
						{
							WayPoint * wp = Middleb->CreateWaypointStruct();
							wp->id = i;
							wp->x = MiddleWP[i].mX;
							wp->y = MiddleWP[i].mY;
							wp->z = MiddleWP[i].mZ;
							wp->o = MiddleWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Middleb->GetAIInterface()->addWayPoint(wp);
						}
						Middleb->GetAIInterface()->StopMovement(0);
						Middleb->GetAIInterface()->setWaypointToMove(0);
						Middleb->Despawn(600000, 0);
					}			

					//4 Tomb fiends on the right side
					Righta = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7349, 2545.95750f, 901.841431f, 47.36185f, 4.421257f, true, false, 0, 0);
					Righta = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2545.95750f, 901.841431f, 47.36185f, 7349);
					if(Righta)
					{
						for(i=1;i<5;i++)
						{
							WayPoint * wp = Righta->CreateWaypointStruct();
							wp->id = i;
							wp->x = RightWP[i].mX;
							wp->y = RightWP[i].mY;
							wp->z = RightWP[i].mZ;
							wp->o = RightWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Righta->GetAIInterface()->addWayPoint(wp);
						}
						Righta->GetAIInterface()->StopMovement(0);
						Righta->GetAIInterface()->setWaypointToMove(0);
						Righta->Despawn(600000, 0);
					}	

					Rightb = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7349, 2543.848877f, 902.117676f, 47.336006f, 4.735526f, true, false, 0, 0);
					Rightb = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2543.848877f, 902.117676f, 47.336006f, 7349);
					if(Rightb)
					{
						for(i=1;i<5;i++)
						{
							WayPoint * wp = Rightb->CreateWaypointStruct();
							wp->id = i;
							wp->x = RightWP[i].mX;
							wp->y = RightWP[i].mY;
							wp->z = RightWP[i].mZ;
							wp->o = RightWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Rightb->GetAIInterface()->addWayPoint(wp);
						}
						Rightb->GetAIInterface()->StopMovement(0);
						Rightb->GetAIInterface()->setWaypointToMove(0);
						Rightb->Despawn(600000, 0);
					}	
					Rightc = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7349, 2548.646973f, 902.748535f, 47.436337f, 4.271524f, true, false, 0, 0);
					Rightc = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2548.646973f, 902.748535f, 47.436337f, 7349);
					if(Rightc)
					{
						for(i=1;i<5;i++)
						{
							WayPoint * wp = Rightc->CreateWaypointStruct();
							wp->id = i;
							wp->x = RightWP[i].mX;
							wp->y = RightWP[i].mY;
							wp->z = RightWP[i].mZ;
							wp->o = RightWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Rightc->GetAIInterface()->addWayPoint(wp);
						}
						Rightc->GetAIInterface()->StopMovement(0);
						Rightc->GetAIInterface()->setWaypointToMove(0);
						Rightc->Despawn(600000, 0);
					}	

					Rightd = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7349, 2546.062012f, 898.561340f, 47.849136f, 4.624953f, true, false, 0, 0);
					Rightd = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2546.062012f, 898.561340f, 47.849136f, 7349); 
					if(Rightd)
					{
						for(i=1;i<5;i++)
						{
							WayPoint * wp = Rightd->CreateWaypointStruct();
							wp->id = i;
							wp->x = RightWP[i].mX;
							wp->y = RightWP[i].mY;
							wp->z = RightWP[i].mZ;
							wp->o = RightWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Rightd->GetAIInterface()->addWayPoint(wp);
						}
						Rightd->GetAIInterface()->StopMovement(0);
						Rightd->GetAIInterface()->setWaypointToMove(0);
						Rightd->Despawn(600000, 0);
					}	

				waves++;
				m_active = true;
				m_spawn = true;
				_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
					}
			break;
					
			case 1:
				{
					//2 Elite spiders left
					EliteLefta = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7351, 2491.322021f, 831.42151f, 44.714779f, 0.948184f, true, false, 0, 0);
					EliteLefta = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2491.322021f, 831.42151f, 44.714779f, 7351);
					if(EliteLefta)
					{
						for(i=1;i<4;i++)
						{
							WayPoint * wp = EliteLefta->CreateWaypointStruct();
							wp->id = i;
							wp->x = LeftWP[i].mX;
							wp->y = LeftWP[i].mY;
							wp->z = LeftWP[i].mZ;
							wp->o = LeftWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							EliteLefta->GetAIInterface()->addWayPoint(wp);
						}
						EliteLefta->GetAIInterface()->StopMovement(0);
						EliteLefta->GetAIInterface()->setWaypointToMove(0);
						EliteLefta->Despawn(600000, 0);
					}	

					EliteLeftb = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7351, 2492.007080f, 827.447144f, 44.594097f, 1.124897f, true, false, 0, 0);
					EliteLeftb = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2492.007080f, 827.447144f, 44.594097f, 7351);
					if(EliteLeftb)
					{
						for(i=1;i<4;i++)
						{
							WayPoint * wp = EliteLeftb->CreateWaypointStruct();
							wp->id = i;
							wp->x = LeftWP[i].mX;
							wp->y = LeftWP[i].mY;
							wp->z = LeftWP[i].mZ;
							wp->o = LeftWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							EliteLeftb->GetAIInterface()->addWayPoint(wp);
						}
						EliteLeftb->GetAIInterface()->StopMovement(0);
						EliteLeftb->GetAIInterface()->setWaypointToMove(0);
						EliteLeftb->Despawn(600000, 0);
					}
					
					//2 Elite spiders right
					EliteRighta = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7351, 2545.95750f, 901.841431f, 47.36185f, 4.421257f, true, false, 0, 0);
					EliteRighta = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2545.95750f, 901.841431f, 47.36185f, 7351);
					if(EliteRighta)
					{
						for(i=1;i<5;i++)
						{
							WayPoint * wp = EliteRighta->CreateWaypointStruct();
							wp->id = i;
							wp->x = RightWP[i].mX;
							wp->y = RightWP[i].mY;
							wp->z = RightWP[i].mZ;
							wp->o = RightWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							EliteRighta->GetAIInterface()->addWayPoint(wp);
						}
						EliteRighta->GetAIInterface()->StopMovement(0);
						EliteRighta->GetAIInterface()->setWaypointToMove(0);
						EliteRighta->Despawn(600000, 0);
					}
					
					EliteRightb = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7351, 2546.062012f, 898.561340f, 47.849136f, 4.624953f, true, false, 0, 0);
					EliteRightb = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2546.062012f, 898.561340f, 47.849136f, 7351);
					if(EliteRightb)
					{
						for(i=1;i<5;i++)
						{
							WayPoint * wp = EliteRightb->CreateWaypointStruct();
							wp->id = i;
							wp->x = RightWP[i].mX;
							wp->y = RightWP[i].mY;
							wp->z = RightWP[i].mZ;
							wp->o = RightWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							EliteRightb->GetAIInterface()->addWayPoint(wp);
						}
						EliteRightb->GetAIInterface()->StopMovement(0);
						EliteRightb->GetAIInterface()->setWaypointToMove(0);
						EliteRightb->Despawn(600000, 0);
					}
				m_active = true;
				m_spawn = true;
				waves++;
				_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
				}
			break;
						
			case 2:
				{
					//Tutenkash
					Tutenkash = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7355, 2489.9375f, 828.71814f, 44.181084f, 1.145883f, true, false, 0, 0);
					Tutenkash = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2489.9375f, 828.71814f, 44.181084f, 7355);
					if(Tutenkash)
					{
						for(i=1;i<5;i++)
						{
							WayPoint * wp = Tutenkash->CreateWaypointStruct();
							wp->id = i;
							wp->x = LeftWP[i].mX;
							wp->y = LeftWP[i].mY;
							wp->z = LeftWP[i].mZ;
							wp->o = LeftWP[i].mO;
							wp->waittime = 0;
							wp->flags = RUN;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Tutenkash->GetAIInterface()->addWayPoint(wp);
						}
						Tutenkash->GetAIInterface()->StopMovement(0);
						Tutenkash->GetAIInterface()->setWaypointToMove(0);
					}

					m_active = true;
					m_spawn = true;
					waves++;	
					_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
				}
			break;
					
			default:
				break;
		}
		Clicker = pPlayer;
		RegisterAIUpdateEvent(500);
	}
	
	void AIUpdate()
	{
		if(m_spawn == true)
		{
			switch(waves)
			{
			case 0:
				{
					Creature* Lefta = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2489.9375f, 828.71814f, 44.181084f, 7349);
					Creature* Leftb = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2491.322021f, 831.42151f, 44.714779f, 7349);
					Creature* Leftc = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2487.222656f, 830.148438f, 43.851303f, 7349);
					Creature* Leftd = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2492.007080f, 827.447144f, 44.594097f, 7349);
					Creature* Lefte = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2488.033936f, 826.437134f, 43.707855f, 7349); 

					Creature* Middlea = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2526.669189f, 833.028137f, 48.029835f, 7349);
					Creature* Middleb = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2522.113770f, 831.423828f, 48.344925f, 7349);
			 
					Creature* Righta = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2545.95750f, 901.841431f, 47.36185f, 7349);
					Creature* Rightb = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2543.848877f, 902.117676f, 47.336006f, 7349);
					Creature* Rightc = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2548.646973f, 902.748535f, 47.436337f, 7349);
					Creature* Rightd = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2546.062012f, 898.561340f, 47.849136f, 7349); 
					m_spawn = false;
				} break;
			case 1:
				{
					Creature* EliteLefta = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2491.322021f, 831.42151f, 44.714779f, 7351);
					Creature* EliteLeftb = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2492.007080f, 827.447144f, 44.594097f, 7351);

					Creature* EliteRighta = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2545.95750f, 901.841431f, 47.36185f, 7351);
					Creature* EliteRightb = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2546.062012f, 898.561340f, 47.849136f, 7351);
					m_spawn = false;
				} break;
			case 2:
				{
					Creature* Tutenkash = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2489.9375f, 828.71814f, 44.181084f, 7355);
					m_spawn = false;
				} break;
			}
		}

		if(m_active == true)
		{
			switch(waves)
			{
			case 0:
				{
					if(((!Lefta->isAlive()) || !Lefta) && ((!Leftb->isAlive()) || !Leftb) && ((!Leftc->isAlive()) || !Leftc) && ((!Leftd->isAlive()) || !Leftd) && ((!Lefte->isAlive()) || !Lefte) && 
						((!Middlea->isAlive()) || !Middlea) && ((!Middleb->isAlive()) || !Middleb) && ((!Righta->isAlive()) || !Righta) && ((!Rightb->isAlive()) || !Rightb) && ((!Rightc->isAlive()) || !Rightc) && 
						((!Rightd->isAlive()) || !Rightd))
					{
						m_active = false;
						_gameobject->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
					}
				} break;
			case 1:
				{
					if(((!EliteLefta->isAlive()) || !EliteLefta) && ((!EliteLeftb->isAlive()) || !EliteLeftb) && ((!EliteRighta->isAlive()) || !EliteRighta) && ((!EliteRightb->isAlive()) || !EliteRightb))
					{
						m_active = false;
						_gameobject->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
					}
				} break;
			case 2:
				{
					if(Tutenkash && !Tutenkash->isAlive())
						m_active = false;
				} break;
			}
		}

		if ( Clicker && ( !Clicker->isAlive() || !Clicker->IsInInstance()) )
		{	
			if ( Lefta != nullptr && Lefta->isAlive() ) {
				Lefta->GetAIInterface()->StopMovement(1);
				Lefta->Despawn(500, 0);
				waves = 0;
			}
			if ( Leftb != nullptr && Leftb->isAlive() ) {
				Leftb->GetAIInterface()->StopMovement(1);
				Leftb->Despawn(500, 0);
				waves = 0;
			}
			if ( Leftc != nullptr && Leftc->isAlive() ) {
				Leftc->GetAIInterface()->StopMovement(1);
				Leftc->Despawn(500, 0);
				waves = 0;
			}
			if ( Leftd != nullptr && Leftd->isAlive() ) {
				Leftd->GetAIInterface()->StopMovement(1);
				Leftd->Despawn(500, 0);
				waves = 0;
			}
			if ( Lefte != nullptr && Lefte->isAlive() ) {
				Lefte->GetAIInterface()->StopMovement(1);
				Lefte->Despawn(500, 0);
				waves = 0;
			} 

			if ( Middlea != nullptr && Middlea->isAlive() ) {
				Middlea->GetAIInterface()->StopMovement(1);
				Middlea->Despawn(500, 0);
				waves = 0; 
			} 
			if ( Middleb != nullptr && Middleb->isAlive() ) {
				Middleb->GetAIInterface()->StopMovement(1);
				Middleb->Despawn(500, 0);
				waves = 0; 
			} 

			if ( Righta != nullptr && Righta->isAlive() ) {
				Righta->GetAIInterface()->StopMovement(1);
				Righta->Despawn(500, 0);
				waves = 0; 
			} 
			if ( Rightb != nullptr && Rightb->isAlive() ) {
				Rightb->GetAIInterface()->StopMovement(1);
				Rightb->Despawn(500, 0);
				waves = 0; 
			} 
			if ( Rightc != nullptr && Rightc->isAlive() ) {
				Rightc->GetAIInterface()->StopMovement(1);
				Rightc->Despawn(500, 0);
				waves = 0; 
			} 
			if ( Rightd != nullptr && Rightd->isAlive() ) {
				Rightd->GetAIInterface()->StopMovement(1);
				Rightd->Despawn(500, 0);
				waves = 0; 
			} 
			
			if ( EliteLefta != nullptr && EliteLefta->isAlive() ) {
				EliteLefta->GetAIInterface()->StopMovement(1);
				EliteLefta->Despawn(500, 0);
				waves = 1;
			}
			if ( EliteLeftb != nullptr && EliteLeftb->isAlive() ) {
				EliteLeftb->GetAIInterface()->StopMovement(1);
				EliteLeftb->Despawn(500, 0);
				waves = 1;
			}

			if ( EliteRighta != nullptr && EliteRighta->isAlive() ) {
				EliteRighta->GetAIInterface()->StopMovement(1);
				EliteRighta->Despawn(500, 0);
				waves = 1;
			}
			if ( EliteRightb != nullptr && EliteRightb->isAlive() ) {
				EliteRightb->GetAIInterface()->StopMovement(1);
				EliteRightb->Despawn(500, 0);
				waves = 1;
			}

			if ( Tutenkash != nullptr && Tutenkash->isAlive() ) {
				Tutenkash->GetAIInterface()->StopMovement(1);
				Tutenkash->Despawn(500, 0);
				waves = 2;
			}
			_gameobject->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
			m_active = false;
			m_spawn = false;
		} 
	}
	static GameObjectAIScript *Create(GameObject* GO) { return new GongOfRazorfen(GO); }

protected:
	bool m_active;
	bool m_spawn;

	int waves;

	Creature* Lefta;
	Creature* Leftb;
	Creature* Leftc;
	Creature* Leftd;
	Creature* Lefte;

	Creature* Middlea;
	Creature* Middleb;

	Creature* Righta;
	Creature* Rightb;
	Creature* Rightc;
	Creature* Rightd;

	Creature* EliteLefta;
	Creature* EliteLeftb;

	Creature* EliteRighta;
	Creature* EliteRightb;

	Creature* Tutenkash;
	Player* Clicker;
};

#define CURSE_OF_TUTENKASH	12255
#define WEB_SPRAY			12252
class Tutenkash : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Tutenkash, MoonScriptCreatureAI);
    Tutenkash(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(CURSE_OF_TUTENKASH, Target_Self, 30, 0, 60);
		AddSpell(WEB_SPRAY, Target_Current, 15, 0, 20);
    }
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

//TRASH
#define BONEFLAYER_GHOUL 7347
class BoneflayerGhoul : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(BoneflayerGhoul, MoonScriptCreatureAI);
	BoneflayerGhoul(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Cleave
		AddSpell(15496, Target_Current, 30, 0, 5);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define DEATHS_HEAD_GEOMANCER 7335
class DeathsHeadGeomancer : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DeathsHeadGeomancer, MoonScriptCreatureAI);
	DeathsHeadGeomancer(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(9053, Target_Current, 8, 3, 0);
		AddSpell(6725, Target_RandomDestination, 7, 3, 0);
		AddSpell(11436, Target_Current, 8, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define DEATHS_HEAD_NECROMANCER	7337
class DeathsHeadNecromancer : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DeathsHeadNecromancer, MoonScriptCreatureAI);
	DeathsHeadNecromancer(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		BoneArmor = AddSpell(11445, Target_Self, 0, 0, 0);
		AddSpell(11443, Target_Current, 7, 3, 0);
		AddSpell(9613, Target_Current, 10, 3, 0);
		AddSpell(12420, Target_Self, 5, 0, 0);
	}

	void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(BoneArmor);
		ParentClass::OnCombatStart(pTarget);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
	SpellDesc* BoneArmor;
};

#define FREEZING_SPIRIT 7353
class FreezingSpirit : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(FreezingSpirit, MoonScriptCreatureAI);
	FreezingSpirit(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15532, Target_Current, 11, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define FROZEN_SOUL 7352
class FrozenSoul : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(FrozenSoul, MoonScriptCreatureAI);
	FrozenSoul(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(12528, Target_RandomPlayer, 10, 1.5f, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SKELETAL_FROSTWEAVER 7341
class SkeletalFrostweaver : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(SkeletalFrostweaver, MoonScriptCreatureAI);
	SkeletalFrostweaver(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(9672, Target_Current, 30, 3, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SKELETAL_SUMMONER 7342
class SkeletalSummoner : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(SkeletalSummoner, MoonScriptCreatureAI);
	SkeletalSummoner(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(11980, Target_Current, 8, 0, 10);
		AddSpell(9532, Target_Current, 25, 3, 0);
		AddSpell(12258, Target_Self, 7, 5, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SKELETAL_SHADOWCASTER 7340
class SkeletalShadowcaster : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(SkeletalShadowcaster, MoonScriptCreatureAI);
	SkeletalShadowcaster(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(12248, Target_Current, 10, 2, 0);
		AddSpell(9613, Target_Current, 11, 3, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SPLINTERBONE_CAPTAIN 7345
class SplinterboneCaptain : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(SplinterboneCaptain, MoonScriptCreatureAI);
	SplinterboneCaptain(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(12461, Target_Current, 8, 0, 0);
		AddSpell(9128, Target_Self, 5, 0, 100);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SPLINTERBONE_CENTURION 7346
class SplinterboneCenturion : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(SplinterboneCenturion, MoonScriptCreatureAI);
	SplinterboneCenturion(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(8078, Target_Current, 11, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define THORN_EATER_GHOUL 7348
class ThornEaterGhoul : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ThornEaterGhoul, MoonScriptCreatureAI);
	ThornEaterGhoul(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(12538, Target_Current, 11, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define TOMB_REAVER 7351
class TombReaver : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(TombReaver, MoonScriptCreatureAI);
	TombReaver(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(745, Target_Current, 11, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define WITHERED_QUILGUARD 7329
class WitheredQuilguard : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(WitheredQuilguard, MoonScriptCreatureAI);
	WitheredQuilguard(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(8599, Target_Self, 11, 0, 100);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define WITHERED_REAVER 7328
class WitheredReaver : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(WitheredReaver, MoonScriptCreatureAI);
	WitheredReaver(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(8599, Target_Self, 11, 0, 100);
		AddSpell(11397, Target_Current, 11, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define WITHERED_SPEARHIDE 7332
class WitheredSpearhide : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(WitheredSpearhide, MoonScriptCreatureAI);
	WitheredSpearhide(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(8599, Target_Self, 11, 0, 100);
		AddSpell(40504, Target_Current, 11, 0, 0);
		AddSpell(6660, Target_Current, 20, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define WITHERED_WARRIOR 7327
class WitheredWarrior : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(WitheredWarrior, MoonScriptCreatureAI);
	WitheredWarrior(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(8599, Target_Self, 11, 0, 100);
		AddSpell(6268, Target_Current, 11, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};


void SetupRazorfenDowns(ScriptMgr * mgr)
{
	mgr->register_creature_script(7358, &AmnennarTheColdbringer::Create);
	mgr->register_creature_script(8567, &Glutton::Create);
	mgr->register_creature_script(7357, &MordreshFireEye::Create);
	mgr->register_creature_script(7356, &PlaguemawTheRotting::Create);
	mgr->register_creature_script(7354, &Ragglesnout::Create);
	mgr->register_creature_script(7355, &Tutenkash::Create);
	//mgr->register_gameobject_script(148917, &GongOfRazorfen::Create);
	//Trash
	mgr->register_creature_script(WITHERED_WARRIOR, &WitheredWarrior::Create);
	mgr->register_creature_script(WITHERED_SPEARHIDE, &WitheredSpearhide::Create);
	mgr->register_creature_script(WITHERED_REAVER, &WitheredReaver::Create);
	mgr->register_creature_script(WITHERED_QUILGUARD, &WitheredQuilguard::Create);
	mgr->register_creature_script(TOMB_REAVER, &TombReaver::Create);
	mgr->register_creature_script(THORN_EATER_GHOUL, &ThornEaterGhoul::Create);
	mgr->register_creature_script(SPLINTERBONE_CENTURION, &SplinterboneCenturion::Create);
	mgr->register_creature_script(SPLINTERBONE_CAPTAIN, &SplinterboneCaptain::Create);
	mgr->register_creature_script(SKELETAL_SHADOWCASTER, &SkeletalShadowcaster::Create);
	mgr->register_creature_script(SKELETAL_SUMMONER, &SkeletalSummoner::Create);
	mgr->register_creature_script(SKELETAL_FROSTWEAVER, &SkeletalFrostweaver::Create);
	mgr->register_creature_script(SKELETAL_SHADOWCASTER, &AmnennarTheColdbringer::Create);
	mgr->register_creature_script(FROZEN_SOUL, &FrozenSoul::Create);
	mgr->register_creature_script(FREEZING_SPIRIT, &FreezingSpirit::Create);
	mgr->register_creature_script(DEATHS_HEAD_NECROMANCER, &DeathsHeadNecromancer::Create);
	mgr->register_creature_script(DEATHS_HEAD_GEOMANCER, &DeathsHeadGeomancer::Create);
	mgr->register_creature_script(BONEFLAYER_GHOUL, &BoneflayerGhoul::Create);
}