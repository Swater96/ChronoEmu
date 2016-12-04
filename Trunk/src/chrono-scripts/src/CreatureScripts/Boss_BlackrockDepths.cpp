/*
 * ChronoEmu Scripts - Blackrock Depths Bosses & Events
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by SideWays
 */

#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"
#include "BlackrockDepths.h"

/* Instance Data
Instance Name: Blackrock Depths
Instance Complete Percentage: 97%
Instance Comment: Marshal Windsor not added. Flamethrowers cast but dont deal damage.
End Instance Data */

/**
	Blackrock Depths Quests
					  **/

class Quest_RocknotsAle : public QuestScript 
{ 
public:

	void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
	{
		switch(RandomUInt(1))
		{
		case 0:
			{
				Creature* Rocknot = NULL;
				Rocknot = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(891.198, -197.924, -43.6204, 9503);
				if(Rocknot && Rocknot->isAlive())
					Rocknot->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Ah, hits the spot!");
			} break;
		case 1:
			{
				Creature* Rocknot = NULL;
				Rocknot = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(891.198, -197.924, -43.6204, 9503);
				if(Rocknot && Rocknot->isAlive())
				{
					Rocknot->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Ah, hits the spot!\n");
					Rocknot->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I want more ale! Give me more ale!");
					for(int i=1; i<4;i++)
					{
						WayPoint * wp = Rocknot->CreateWaypointStruct();
						wp->id = i;
						wp->x = RocknotWP[i].mX;
						wp->y = RocknotWP[i].mY;
						wp->z = RocknotWP[i].mZ;
						wp->o = RocknotWP[i].mO;
						if(i == 1)
							wp->waittime = 4000;
						if(i == 2)
							wp->waittime = 1000;
						if(i == 3)
							wp->waittime = 10000; 
						wp->flags = WALK;
						wp->forwardemoteoneshot = 0;
						wp->forwardemoteid = 0;
						wp->backwardemoteoneshot = 0;
						wp->backwardemoteid = 0;
						wp->forwardskinid = 0;
						wp->backwardskinid = 0;
						Rocknot->GetAIInterface()->addWayPoint(wp);
					}
					Rocknot->GetAIInterface()->StopMovement(0);
					Rocknot->GetAIInterface()->setWaypointToMove(0);
				}				
			} break;
		}
	}
};

class PrivateRocknot : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(PrivateRocknot, MoonScriptCreatureAI);
    PrivateRocknot(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {}
	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 1)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "That one's empty!!");
			_unit->CastSpell(_unit, 6603, false);
		}
		if(iWaypointId == 2)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Ah, empty again!");
			_unit->Despawn(1000, 0);
			GameObject* ArenaDoor2 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(607.668, -229.653, -52.8988, 161525); //Add ArenaDoor2 info
			ArenaDoor2->SetUInt32Value(GAMEOBJECT_STATE, 0);
		}
		if(iWaypointId == 3)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "ALE!");
			_unit->CastSpell(_unit, 6603, false);
			mExplodeTimer = AddTimer( 10000 );
			mEventTimer = AddTimer( 8000 );
		}
		ParentClass::OnReachWP(iWaypointId, bForwards);
	}
	void AIUpdate()
	{
		if(IsTimerFinished( mExplodeTimer ))
		{
			RemoveTimer( mExplodeTimer);
			GameObject* KegShotgun = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(848.268, -197.164, -43.6743, 170607);
			KegShotgun->SetUInt32Value(GAMEOBJECT_STATE, 0);
		}
		if(IsTimerFinished( mEventTimer ))
		{
			RemoveTimer( mEventTimer );
			Creature* Phalanx = NULL;
			Phalanx = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(847.848, -230.067, -43.614, 9502);
			for(int i=1;i<4;i++)
			{
				WayPoint * wps = Phalanx->CreateWaypointStruct();
				wps->id = i;
				wps->x = PhalanxWP[i].mX;
				wps->y = PhalanxWP[i].mY+1;
				wps->z = PhalanxWP[i].mZ;
				wps->o = PhalanxWP[i].mO;
				wps->waittime = 0;
				wps->flags = RUN;
				wps->forwardemoteoneshot = 0;
				wps->forwardemoteid = 0;
				wps->backwardemoteoneshot = 0;
				wps->backwardemoteid = 0;
				wps->forwardskinid = 0;
				wps->backwardskinid = 0;
				Phalanx->GetAIInterface()->addWayPoint(wps);
			}
			Phalanx->GetAIInterface()->setWaypointToMove(0);	
			Phalanx->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);

			GameObject* BarDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(870.693, -228.936, -43.7509, 170571);
			BarDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		}
		ParentClass::AIUpdate();
	}
	int32 mExplodeTimer;
	int32 mEventTimer;
};

/**
	Blackrock Depths Gossips
					  **/

class SCRIPT_DECL RibblyGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		//needs text
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		Menu->AddItem( 0, "You're good for nothing, Ribbly. It's time to pay for your wickedness!", 1);

		if(AutoSend)
			Menu->SendTo(Plr);
	}
	
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
			case 0:
				GossipHello(pObject, Plr, true);
				break;
			case 1:
				{
					Creature* Ribbly = NULL;
					Ribbly = Plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(RibblySpawns[1].mX, RibblySpawns[1].mY, RibblySpawns[1].mZ, 9543);
					Ribbly->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
					Ribbly->GetAIInterface()->GetNextTarget();
					Ribbly->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "No! Get away from me! Help!!");
					Ribbly = NULL;
					for(int i=2;i<5;i++)
					{
						Creature* RibblyCrony = NULL;
						RibblyCrony = Plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(RibblySpawns[i].mX, RibblySpawns[i].mY, RibblySpawns[i].mZ, 10043);
						RibblyCrony->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
						RibblyCrony->GetAIInterface()->GetNextTarget();
					}	
				} break;
		}
	}
};

/*class SCRIPT_DECL GroomrelGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		//needs text
		if(/*plr->GetSkill = mining xD - need to find this)
        {
           // uint32 FactMessages[2] = { , };
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			Menu->AddItem( 0, "Gimme Smelt Dark Iron naowwww!", 1);
        }
        else
        {
            // Send "not mining, noob"
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2, plr);
            
        }
		if(AutoSend)
			Menu->SendTo(Plr);
	}
	
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
			case 0:
				GossipHello(pObject, Plr, true);
				break;
			case 1:
				
				break;
		}
	}
	void Destroy()
	{
		delete this;
	}
};*/

class SCRIPT_DECL DoomrelGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		//needs text
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		Menu->AddItem( 0, "Your bondage is at an end, Doom'rel. I challenge you!", 1);

		if(AutoSend)
			Menu->SendTo(Plr);
	}
	
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
			case 0:
				GossipHello(pObject, Plr, true);
				break;
			case 1:
				{
					Creature* Haterel = NULL;
					Haterel = Plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1246.18, -189.969, -85.5895, 9034);
					if(Haterel && Haterel->isAlive())
					{
						Haterel->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
						Haterel->GetAIInterface()->MoveTo(Plr->GetPositionX(), Plr->GetPositionY(), Plr->GetPositionZ(), 2.0);
						Haterel->GetAIInterface()->setMoveRunFlag(true);
						Haterel->GetAIInterface()->GetNextTarget();
					}
				} break;
		}
	}
};

/**
	Blackrock Depths GameObjects
					  **/

class KegOne : public GameObjectAIScript
{
public:
	KegOne(GameObject* goinstance) : GameObjectAIScript(goinstance) {
	}	
	void OnActivate( Player* pPlayer )
	{	
		
		Creature* BlackbreathCrony = NULL;
		Creature* Hurley = NULL;

		for(int i=0; i<3;i++)
		{
			BlackbreathCrony = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(9541, 849.18, -138.12, -51.10, 5.35, true, false, 0, 0);
			for(int i=1; i<4;i++)
			{
				WayPoint * wps = BlackbreathCrony->CreateWaypointStruct();
				wps->id = i;
				wps->x = KegWP[i].mX+RandomFloat(2);
				wps->y = KegWP[i].mY+RandomFloat(1);
				wps->z = KegWP[i].mZ;
				wps->o = KegWP[i].mO;
				wps->waittime = 0;
				wps->flags = RUN;
				wps->forwardemoteoneshot = 0;
				wps->forwardemoteid = 0;
				wps->backwardemoteoneshot = 0;
				wps->backwardemoteid = 0;
				wps->forwardskinid = 0;
				wps->backwardskinid = 0;
				BlackbreathCrony->GetAIInterface()->addWayPoint(wps);
			}
			BlackbreathCrony->GetAIInterface()->StopMovement(0);
			BlackbreathCrony->GetAIInterface()->setWaypointToMove(0);
			BlackbreathCrony->Despawn(600000, 0);
			BlackbreathCrony = NULL;
		}
		Hurley = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(9537, 849.18, -138.12, -51.10, 5.35, true, false, 0, 0); 
		Hurley->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Get away from those kegs!");
		for(int i=1;i<4;i++)
		{
			WayPoint * wps = Hurley->CreateWaypointStruct();
			wps->id = i;
			wps->x = KegWP[i].mX;
			wps->y = KegWP[i].mY+1;
			wps->z = KegWP[i].mZ;
			wps->o = KegWP[i].mO;
			wps->waittime = 0;
			wps->flags = RUN;
			wps->forwardemoteoneshot = 0;
			wps->forwardemoteid = 0;
			wps->backwardemoteoneshot = 0;
			wps->backwardemoteid = 0;
			wps->forwardskinid = 0;
			wps->backwardskinid = 0;
			Hurley->GetAIInterface()->addWayPoint(wps);
		}
		Hurley->GetAIInterface()->StopMovement(0);
		Hurley->GetAIInterface()->setWaypointToMove(0);
		Hurley->Despawn(600000, 0);
		Hurley = NULL;

		_gameobject->SetUInt32Value(GAMEOBJECT_STATE, 0);
		_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
	}
	static GameObjectAIScript *Create(GameObject* GO) { return new KegOne(GO); }
};

class GiantMechanism : public GameObjectAIScript
{
public:
	GiantMechanism(GameObject* goinstance) : GameObjectAIScript(goinstance) {
	}	
	void OnActivate( Player* pPlayer )
	{	
		if(pPlayer->GetItemInterface()->GetItemCount(11000, false))
		{
			GameObject* GiantLock = NULL;
			GiantLock = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(614.021, -46.8877, -59.636, 161460); 
			GameObject* GiantDoor = NULL;
			GiantDoor = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(723.106, -105.874, -71.4839, 157923); 
			
			GiantLock->SetUInt32Value(GAMEOBJECT_STATE, 0);
			GiantLock->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

			GiantDoor->SetUInt32Value(GAMEOBJECT_STATE, 1);
			GiantDoor->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

			_gameobject->SetUInt32Value(GAMEOBJECT_STATE, 0); //open
			_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		}
		else
		{
			_gameobject->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
			pPlayer->BroadcastMessage("Missing required item : Shadowforge Key");
			return;
		}
	}
	static GameObjectAIScript *Create(GameObject* GO) { return new GiantMechanism(GO); }
};

class BrazierLeft : public GameObjectAIScript
{
public:
	BrazierLeft(GameObject* goinstance) : GameObjectAIScript(goinstance) {
	}	
	void OnActivate( Player* pPlayer )
	{	
		GameObject* BrazierRight = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1330.0, -508.858, -88.8709, 174744); //Add BrazierRight info
		if(BrazierRight->GetUInt32Value(GAMEOBJECT_STATE) == 0)
		{
			GameObject* FlameDoor1 = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1389.55, -555.851, -92.0093, 170573); //Add FlameDoor info
			GameObject* FlameDoor2 = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1370.65, -555.846, -92.0093, 170574); //Add FlameDoor info
			FlameDoor1->SetUInt32Value(GAMEOBJECT_STATE, 0);
			FlameDoor1->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
			FlameDoor2->SetUInt32Value(GAMEOBJECT_STATE, 0);
			FlameDoor2->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		}			
		_gameobject->SetUInt32Value(GAMEOBJECT_STATE, 0); //open
		_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
	}
	static GameObjectAIScript *Create(GameObject* GO) { return new BrazierLeft(GO); }
};

class BrazierRight : public GameObjectAIScript
{
public:
	BrazierRight(GameObject* goinstance) : GameObjectAIScript(goinstance) {
	}	
	void OnActivate( Player* pPlayer )
	{	
		GameObject* BrazierLeft = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1431.13, -508.938, -88.8709, 174745); //Add BrazierLeft info
		if(BrazierLeft->GetUInt32Value(GAMEOBJECT_STATE) == 0)
		{
			GameObject* FlameDoor1 = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1389.55, -555.851, -92.0093, 170573); //Add FlameDoor info
			GameObject* FlameDoor2 = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1370.65, -555.846, -92.0093, 170574); //Add FlameDoor info
			FlameDoor1->SetUInt32Value(GAMEOBJECT_STATE, 0);
			FlameDoor1->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
			FlameDoor2->SetUInt32Value(GAMEOBJECT_STATE, 0);
			FlameDoor2->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		}			
		_gameobject->SetUInt32Value(GAMEOBJECT_STATE, 0); //open
		_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
	}
	static GameObjectAIScript *Create(GameObject* GO) { return new BrazierRight(GO); }
};

class KegShotgun : public GameObjectAIScript
{
public:
	KegShotgun(GameObject* goinstance) : GameObjectAIScript(goinstance) {
	FirstUse = true;}	
	void OnActivate( Player* pPlayer )
	{	
		_gameobject->SetUInt32Value(GAMEOBJECT_STATE, 0); //open
		_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		RegisterAIUpdateEvent( 5000 );
	}
	void AIUpdate()
	{
		if(FirstUse == true)
		{
			GameObject* BackDoor = NULL;
			BackDoor = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(870.693, -228.936, -43.7509, 170571);
			//BackDoor == 0000000x00  -- WTF?!
			BackDoor->SetUInt32Value(GAMEOBJECT_STATE, 0); //open

			Creature* Phalanx = NULL;
			Phalanx = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(847.848, -230.067, -43.614, 9502);
			for(int i=1;i<4;i++)
			{
				WayPoint * wps = Phalanx->CreateWaypointStruct();
				wps->id = i;
				wps->x = PhalanxWP[i].mX;
				wps->y = PhalanxWP[i].mY+1;
				wps->z = PhalanxWP[i].mZ;
				wps->o = PhalanxWP[i].mO;
				wps->waittime = 0;
				wps->flags = RUN;
				wps->forwardemoteoneshot = 0;
				wps->forwardemoteid = 0;
				wps->backwardemoteoneshot = 0;
				wps->backwardemoteid = 0;
				wps->forwardskinid = 0;
				wps->backwardskinid = 0;
				Phalanx->GetAIInterface()->addWayPoint(wps);
			}
			Phalanx->GetAIInterface()->setWaypointToMove(0);	
			Phalanx->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
			FirstUse = false;
		}
	}
	static GameObjectAIScript *Create(GameObject* GO) { return new KegShotgun(GO); }
protected:
	bool FirstUse;
};

class KeeperPortrait : public GameObjectAIScript
{
public:
	KeeperPortrait(GameObject* goinstance) : GameObjectAIScript(goinstance) {
		Clicker = NULL;
		PlusRandom = 0;
	}	
	void OnActivate( Player* pPlayer )
	{	
			SpawnKeeper();
			switch(PlusRandom)
			{
			case 0:
				pPlayer->BroadcastMessage("Bethek's wanderings take him deep within Blackrock. The secrets of the mountain beckon him. May his spirit never falter.");
				break;
			case 1:
				pPlayer->BroadcastMessage("Those fallen before Dark Keeper Ofgut's hammer cannot be counted. The soldiers of the West Garrison bow in awe as he passes. For who among them can look into the eyes of Ofgut and see the secrets the dark key whispers?");
				break;
			case 2:
				pPlayer->BroadcastMessage("Dark Keeper Pelver is our most honored disciple. He has borne the key for longer than any, and it has cost him dearly. When he is called for his burden, he is guarded in the Domicile.");
				break;
			case 3:
				pPlayer->BroadcastMessage("Uggel is a skilled and solemn chiseler, and when not burdened with the dark key, his hands shape great works in the Hall Of Crafting.");
				break;
			case 4:
				pPlayer->BroadcastMessage("Vorfalk works the store room of the Grim Guzzler, and his sacrifice to the Dark Keepers runs deep. May his spirit hold strong.");
				break;
			case 5:
				pPlayer->BroadcastMessage("When the madness of the dark key takes hold of Zimrel, only the screams of the dying can soothe him. For his sacrifice he will always have a bench above the arena. May our blood sports temper the rage in his heart.");
				break;
			}
			_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
	}

	void SpawnKeeper()
	{
		Creature* Keeper = NULL;
		Creature* Guards = NULL;

		switch(RandomUInt(6))
		{
		case 0:
			{
			PlusRandom = 0;
			Keeper = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(KEEPER_BETHEK, BethekSpawns[0].mX, BethekSpawns[0].mY, BethekSpawns[0].mZ, BethekSpawns[0].mO, true, false, 0, 0);
			for(int i=1;i<5;i++)
			{
				Guards = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(KEEPER_GUARD, BethekSpawns[i].mX, BethekSpawns[i].mY, BethekSpawns[i].mZ, BethekSpawns[PlusRandom+i].mO, true, false, 0, 0);
				Guards = NULL;
			}
			} break;
		case 1:
			{
			PlusRandom = 1;
			Keeper = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(KEEPER_OFGUT, OfgutSpawns[0].mX, OfgutSpawns[0].mY, OfgutSpawns[0].mZ, OfgutSpawns[0].mO, true, false, 0, 0);
			for(int i=1;i<5;i++)
			{
				Guards = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(KEEPER_GUARD, OfgutSpawns[i].mX, OfgutSpawns[i].mY, OfgutSpawns[i].mZ, OfgutSpawns[i].mO, true, false, 0, 0);
				Guards = NULL;
			}
			} break;
		case 2:
			{
			PlusRandom = 2;
			Keeper = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(KEEPER_PELVER, PelverSpawns[0].mX, PelverSpawns[0].mY, PelverSpawns[0].mZ, PelverSpawns[0].mO, true, false, 0, 0);
			for(int i=1;i<5;i++)
			{
				Guards = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(KEEPER_GUARD, PelverSpawns[i].mX, PelverSpawns[i].mY, PelverSpawns[i].mZ, PelverSpawns[i].mO, true, false, 0, 0);
				Guards = NULL;
			}
			} break;
		case 3:
			{
			PlusRandom = 3;
			Keeper = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(KEEPER_UGGEL, UggelSpawns[0].mX, UggelSpawns[0].mY, UggelSpawns[0].mZ, UggelSpawns[0].mO, true, false, 0, 0);
			for(int i=1;i<5;i++)
			{
				Guards = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(KEEPER_GUARD, UggelSpawns[i].mX, UggelSpawns[i].mY, UggelSpawns[i].mZ, UggelSpawns[i].mO, true, false, 0, 0);
				Guards = NULL;
			}
			} break;
		case 4:
			{
			PlusRandom = 4;
			Keeper = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(KEEPER_VORFALK, VorfalkSpawns[0].mX, VorfalkSpawns[0].mY, VorfalkSpawns[0].mZ, VorfalkSpawns[0].mO, true, false, 0, 0);
			for(int i=1;i<5;i++)
			{
				Guards = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(KEEPER_GUARD, VorfalkSpawns[i].mX, VorfalkSpawns[i].mY, VorfalkSpawns[i].mZ, VorfalkSpawns[i].mO, true, false, 0, 0);
				Guards = NULL;
			}
			} break;
		case 5:
			{
			PlusRandom = 5;
			Keeper = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(KEEPER_ZIMREL, ZimrelSpawns[0].mX, ZimrelSpawns[0].mY, ZimrelSpawns[0].mZ, ZimrelSpawns[0].mO, true, false, 0, 0);
			for(int i=1;i<5;i++)
			{
				Guards = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(KEEPER_GUARD, ZimrelSpawns[i].mX, ZimrelSpawns[i].mY, ZimrelSpawns[i].mZ, ZimrelSpawns[i].mO, true, false, 0, 0);
				Guards = NULL;
			}
			} break;
		}		
	}
	static GameObjectAIScript *Create(GameObject* GO) { return new KeeperPortrait(GO); }
protected:
	Player* Clicker;
	int PlusRandom;
};

/**
	Blackrock Depths Triggers
	Undead Side
					  **/

RingOfLawTrigger::RingOfLawTrigger( Creature* pCreature ) : MoonScriptCreatureAI( pCreature )
{
	_unit->GetAIInterface()->MoveTo( _unit->GetSpawnX(), _unit->GetSpawnY(), _unit->GetSpawnZ(), _unit->GetSpawnO() );
	SetCanEnterCombat( false );
	SetFlyMode( true );
	_unit->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );

	mFirstWave = false;
	mSecondWave = false;
	mThirdWave = false;
	mBossSpawn = false;
	mDontRepeat = true;
	mEndEvent = false;

	RolBoss = LastMoba = LastMobb = LastMobc = NULL;

	RandomCreatureID = 0;

	// AIUpdate registering
	RegisterAIUpdateEvent( 1000 );
};

void RingOfLawTrigger::Destroy()
{
	delete this;
}

void RingOfLawTrigger::AIUpdate()
{
	if(_unit->GetMapMgr()->GetInterface()->GetPlayerCountInRadius(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 4.0) > 0 && mDontRepeat == true)
	{
		mFirstWave = true;
		mDontRepeat = false;
		mWaveOneTimer = AddTimer( WAVE_ONE_TIMER );
		StartEvent();
	}
	if(mDontRepeat == false && _unit->GetMapMgr()->GetInterface()->GetPlayerCountInRadius(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 30.0) < 1)
	{
		//Add Reset Event
	}

	if ( _unit->GetMapMgr() == NULL )
		return;

	if ( _unit->GetPositionX() != _unit->GetSpawnX() || _unit->GetPositionY() != _unit->GetSpawnY() || _unit->GetPositionZ() != _unit->GetSpawnZ() )
		_unit->GetAIInterface()->MoveTo( _unit->GetSpawnX(), _unit->GetSpawnY(), _unit->GetSpawnZ(), _unit->GetSpawnO() );

	if(mFirstWave == true && IsTimerFinished(mWaveOneTimer))
	{
		mWaveTwoTimer = AddTimer( WAVE_TWO_TIMER );
		RemoveTimer( mWaveOneTimer );
		SpawnWaveOne();
		mFirstWave = false;
		mSecondWave = true;
	}

	if(LastMoba != NULL && LastMobb != NULL && LastMobc != NULL && mBossSpawn == false)
	{
		if(!LastMoba->isAlive() && !LastMobb->isAlive() && !LastMobc->isAlive())
		{
			Creature* Grimstone = NULL;
			Grimstone = _unit->GetMapMgr()->GetInterface()->SpawnCreature(10096 , 627.22, -199.51, -52.72, 2.88, true, false, 0, 0); //Add Arena NPC info here
			if(Grimstone)
			{
				if(Grimstone->isAlive())
				{
					Grimstone->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );
					Grimstone->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Haha! I bet you thought you were done!");
					for(int i=3; i<5; i++)
					{
						WayPoint * wp = Grimstone->CreateWaypointStruct();
						wp->id = i;
						wp->x = GrimstoneWP[i].mX;
						wp->y = GrimstoneWP[i].mY;
						wp->z = GrimstoneWP[i].mZ;
						wp->o = GrimstoneWP[i].mO;
						wp->waittime = 0;
						wp->flags = WALK;
						wp->forwardemoteoneshot = 0;
						wp->forwardemoteid = 0;
						wp->backwardemoteoneshot = 0;
						wp->backwardemoteid = 0;
						wp->forwardskinid = 0;
						wp->backwardskinid = 0;
						Grimstone->GetAIInterface()->addWayPoint(wp);
					}
					Grimstone->GetAIInterface()->setWaypointToMove(2);
					Grimstone->GetAIInterface()->StopMovement(0);
					mBossSpawn = true;
					mBossTimer = AddTimer( BOSS_SPAWN_TIMER );
				}
			}
			GameObject* ArenaDoor2 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(607.668, -229.653, -52.8988, 161525); //Add ArenaDoor info
			ArenaDoor2->SetUInt32Value(GAMEOBJECT_STATE, 1);
		}
	}

	if(mSecondWave == true && IsTimerFinished( mWaveTwoTimer ))
	{
		SpawnWaveTwo();
		RemoveTimer( mWaveTwoTimer );
		mSecondWave = false;
		mThirdWave = true;
		mWaveThreeTimer = AddTimer( WAVE_THREE_TIMER );
	}
	if(mThirdWave == true && IsTimerFinished( mWaveThreeTimer ))
	{
		SpawnWaveThree();
		RemoveTimer( mWaveThreeTimer );
		mThirdWave = false;
	}

	if(mBossSpawn == true && IsTimerFinished( mBossTimer ))
	{
		SpawnRolBoss();
		mBossSpawn = false;
		RemoveTimer( mBossTimer );
		_unit->Despawn( 500, 0 );
	}
	ParentClass::AIUpdate();
}

void RingOfLawTrigger::StartEvent()
{
	Creature* Grimstone = NULL;
	Grimstone = _unit->GetMapMgr()->GetInterface()->SpawnCreature(10096 , 627.22, -199.51, -52.72, 2.88, true, false, 0, 0); //Add Arena NPC info here
	if(Grimstone)
	{
		if(Grimstone->isAlive())
		{
			Grimstone->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );
			GameObject* ArenaDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(555.751, -199.744, -52.908, 161523); //Add ArenaDoor info
			//Close the door you came from
			ArenaDoor->SetUInt32Value(GAMEOBJECT_STATE, 1);
			Grimstone->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You have been sentenced to death for cromes against the Dark Iron nation!");
			for(int i=1; i<3; i++)
			{
				WayPoint * wp = Grimstone->CreateWaypointStruct();
				wp->id = i;
				wp->x = GrimstoneWP[i].mX;
				wp->y = GrimstoneWP[i].mY;
				wp->z = GrimstoneWP[i].mZ;
				wp->o = GrimstoneWP[i].mO;
				wp->waittime = 0;
				wp->flags = WALK;
				wp->forwardemoteoneshot = 0;
				wp->forwardemoteid = 0;
				wp->backwardemoteoneshot = 0;
				wp->backwardemoteid = 0;
				wp->forwardskinid = 0;
				wp->backwardskinid = 0;
				Grimstone->GetAIInterface()->addWayPoint(wp);
			}
			Grimstone->GetAIInterface()->StopMovement(0);
			Grimstone->GetAIInterface()->setWaypointToMove(0);
		}
	}
}
void RingOfLawTrigger::SpawnRolBoss()
{
	switch(RandomCreatureID)
	{
	case 8925:
		{
			RolBoss = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9027, 671.85, -168.29, -55.86, 3.51, true, false, 0, 0); //Add Boss here 
		} break;
	case 8926:
		{
			RolBoss = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9031, 671.85, -168.29, -55.86, 3.51, true, false, 0, 0); //Add Boss here 
		} break;
	case 8927:
		{
			RolBoss = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9029, 671.85, -168.29, -55.86, 3.51, true, false, 0, 0); //Add Boss here 
		} break;
	case 8928:
		{
			RolBoss = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9028, 671.85, -168.29, -55.86, 3.51, true, false, 0, 0); //Add Boss here 
		} break;
	case 8932:
		{
			RolBoss = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9030, 671.85, -168.29, -55.86, 3.51, true, false, 0, 0); //Add Boss here 
		} break;
	case 8933:
		{
			RolBoss = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9032, 671.85, -168.29, -55.86, 3.51, true, false, 0, 0); //Add Boss here 
		} break;
	}

	if(RolBoss != NULL)
	{
		for(int i=1;i<4;i++)
		{
			WayPoint * wp = RolBoss->CreateWaypointStruct();
			wp->id = i;
			wp->x = RolBossWP[i].mX;
			wp->y = RolBossWP[i].mY;
			wp->z = RolBossWP[i].mZ;
			wp->o = RolBossWP[i].mO;
			wp->waittime = 0;
			wp->flags = RUN;
			wp->forwardemoteoneshot = 0;
			wp->forwardemoteid = 0;
			wp->backwardemoteoneshot = 0;
			wp->backwardemoteid = 0;
			wp->forwardskinid = 0;
			wp->backwardskinid = 0;
			RolBoss->GetAIInterface()->addWayPoint(wp);
		}
	}
	RolBoss->GetAIInterface()->StopMovement(0);
	RolBoss->GetAIInterface()->setWaypointToMove(0);
	RolBoss->Despawn(600000, 0);
	RolBoss = NULL;
}

void RingOfLawTrigger::SpawnWaveOne()
{
	Creature* NewCreature = NULL;

	switch(RandomUInt(5))
	{
	case 0:
		{
			RandomCreatureID = 8925;
		} break;
	case 1:
		{
			RandomCreatureID = 8926;
		} break;
	case 2:
		{
			RandomCreatureID = 8927;
		} break;
	case 3:
		{
			RandomCreatureID = 8932;
		} break;
	case 4:
		{
			RandomCreatureID = 8933;
		} break;
	case 5:
		{
			RandomCreatureID = 8928;
		} break;
	}
	for(int i=0;i<3;i++)
	{
		if(i>1)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(RandomCreatureID, 622.11+RandomFloat(2), -279.99, -59.25, 1.83, true, false, 0, 0); //Add Wave 1 here 
		}
		else
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(RandomCreatureID, 622.11-RandomFloat(2), -279.99, -59.25, 1.83, true, false, 0, 0); //Add Wave 1 here 
		}

		for(int k=1;k<5;k++)
		{
			WayPoint * wp = NewCreature->CreateWaypointStruct();
			wp->id = k;
			wp->x = WaveOneWP[k].mX+RandomFloat(2);
			wp->y = WaveOneWP[k].mY;
			wp->z = WaveOneWP[k].mZ;
			wp->o = WaveOneWP[k].mO;
			wp->waittime = 0;
			wp->flags = RUN;
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
		NewCreature->Despawn(600000, 0);
		NewCreature = NULL;
	}
}

void RingOfLawTrigger::SpawnWaveTwo()
{
	Creature* NewCreature = NULL;
	for(int i=0;i<3;i++)
	{
		if(i>1)
			NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(RandomCreatureID, 622.11+rand()%2, -279.99, -59.25, 1.83, true, false, 0, 0); //Add Wave 1 here 
		else
			NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(RandomCreatureID, 622.11-rand()%2, -279.99, -59.25, 1.83, true, false, 0, 0); //Add Wave 1 here 

		for(int k=1;k<4;k++)
		{
			WayPoint * wp = NewCreature->CreateWaypointStruct();
			wp->id = k;
			wp->x = WaveOneWP[k].mX+RandomFloat(2);
			wp->y = WaveOneWP[k].mY;
			wp->z = WaveOneWP[k].mZ;
			wp->o = WaveOneWP[k].mO;
			wp->waittime = 0;
			wp->flags = RUN;
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
		NewCreature->Despawn(600000, 0);
		NewCreature = NULL;
	}
}

void RingOfLawTrigger::SpawnWaveThree()
{
	LastMoba = _unit->GetMapMgr()->GetInterface()->SpawnCreature(RandomCreatureID, 622.11+rand()%2, -279.99, -59.25, 1.83, true, false, 0, 0); //Add Wave 1 here 
	for(int i=1;i<4;i++)
	{
		WayPoint * wp = LastMoba->CreateWaypointStruct();
		wp->id = i;
		wp->x = WaveOneWP[i].mX+1;
		wp->y = WaveOneWP[i].mY;
		wp->z = WaveOneWP[i].mZ;
		wp->o = WaveOneWP[i].mO;
		wp->waittime = 0;
		wp->flags = RUN;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		LastMoba->GetAIInterface()->addWayPoint(wp);
	}
	LastMoba->GetAIInterface()->StopMovement(0);
	LastMoba->GetAIInterface()->setWaypointToMove(0);
	LastMoba->Despawn(600000, 0);

	LastMobb = _unit->GetMapMgr()->GetInterface()->SpawnCreature(RandomCreatureID, 622.11, -279.99, -59.25, 1.83, true, false, 0, 0); //Add Wave 1 here 
	for(int j=1;j<4;j++)
	{
		WayPoint * wp = LastMobb->CreateWaypointStruct();
		wp->id = j;
		wp->x = WaveOneWP[j].mX-1;
		wp->y = WaveOneWP[j].mY;
		wp->z = WaveOneWP[j].mZ;
		wp->o = WaveOneWP[j].mO;
		wp->waittime = 400;
		wp->flags = RUN;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		LastMobb->GetAIInterface()->addWayPoint(wp);
	}
	LastMobb->GetAIInterface()->StopMovement(0);
	LastMobb->GetAIInterface()->setWaypointToMove(0);
	LastMobb->Despawn(600000, 0);

	LastMobc = _unit->GetMapMgr()->GetInterface()->SpawnCreature(RandomCreatureID, 622.11-rand()%2, -279.99, -59.25, 1.83, true, false, 0, 0); //Add Wave 1 here 
	for(int k=1;k<4;k++)
	{
		WayPoint * wp = LastMobc->CreateWaypointStruct();
		wp->id = k;
		wp->x = WaveOneWP[k].mX;
		wp->y = WaveOneWP[k].mY;
		wp->z = WaveOneWP[k].mZ;
		wp->o = WaveOneWP[k].mO;
		wp->waittime = 200;
		wp->flags = RUN;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		LastMobc->GetAIInterface()->addWayPoint(wp);
	}
	LastMobc->GetAIInterface()->StopMovement(0);
	LastMobc->GetAIInterface()->setWaypointToMove(0);
	LastMobc->Despawn(600000, 0);
}

class HighJusticeGrimstone : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(HighJusticeGrimstone, MoonScriptCreatureAI);
    HighJusticeGrimstone(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {}
	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 1)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Sons of Thaurissan shall watch you perish in the Ring of the Law!");
		}
		if(iWaypointId == 2)
		{
			//_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Unleash the fury and let it be done!");
			_unit->Despawn(1000, 0);
			GameObject* ArenaDoor2 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(607.668, -229.653, -52.8988, 161525); //Add ArenaDoor2 info
			ArenaDoor2->SetUInt32Value(GAMEOBJECT_STATE, 0);
		}
		if(iWaypointId == 3)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "But your real punishment lies ahead.");
		}
		if(iWaypointId == 4)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Good riddance!");
			_unit->Despawn(1000, 0);
			GameObject* ArenaDoor3 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(637.64, -177.801, -52.8988, 161522); //Add ArenaDoor3 info
			ArenaDoor3->SetUInt32Value(GAMEOBJECT_STATE, 0);
		}
		ParentClass::OnReachWP(iWaypointId, bForwards);
	}
};

/**
	Blackrock Depths
	Lord Roccor
					**/

class LordRoccor : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(LordRoccor, MoonScriptCreatureAI);
    LordRoccor(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(13728, Target_Current, 10, 0, 0);
		AddSpell(6524, Target_Current, 8, 0, 0);
		AddSpell(13729, Target_RandomPlayer, 10, 0, 0);
	}
};

/**
	Blackrock Depths
	Bael'Gar
					**/

class BaelGar : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(BaelGar, MoonScriptCreatureAI);
    BaelGar(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Summon spell, needs checking
		AddSpell(13895, Target_Self, 10, 0, 0);
	}
};

/**
	Blackrock Depths
	Houndmaster Grebmar
					**/

class HoundmasterGrebmar : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(HoundmasterGrebmar, MoonScriptCreatureAI);
    HoundmasterGrebmar(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15615, Target_Current, 10, 0, 0);
		AddSpell(21049, Target_Self, 5, 0, 40);
		AddSpell(12040, Target_Self, 6, 0, 40);
		AddSpell(14032, Target_Current, 10, 0, 0);
	}
};

/**
	Blackrock Depths
	High Interrogator Gerstahn
					**/

class HighInterrogatorGerstahn : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(HighInterrogatorGerstahn, MoonScriptCreatureAI);
    HighInterrogatorGerstahn(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(14033, Target_RandomPlayer, 10, 3, 1);
		AddSpell(13704, Target_Current, 6, 0, 10);
		AddSpell(14032, Target_Current, 10, 0, 10);
		AddSpell(12040, Target_Self, 15, 0, 15);
    }
};

/**
	Blackrock Depths 
	"Ring of Law"
					  **/

class AnubShiah : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(AnubShiah, MoonScriptCreatureAI);
    AnubShiah(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15471, Target_RandomPlayerNotCurrent, 6, 0, 0);
		DemonArmor = AddSpell(13787, Target_Self, 0, 0, 0);
		AddSpell(12493, Target_Current, 6, 0, 0);
		AddSpell(15470, Target_RandomPlayer, 6, 0, 0);
		AddSpell(8994, Target_RandomPlayerNotCurrent, 6, 1, 0);
		AddSpell(15472, Target_Current, 10, 3, 0);
	}

	void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(DemonArmor);
		ParentClass::OnCombatStart(pTarget);
	}
	void OnDied(Unit* pKiller)
	{
		GameObject* ArenaDoor1 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(555.751, -199.744, -52.908, 161523);		
		ArenaDoor1->SetUInt32Value(GAMEOBJECT_STATE, 0);
		GameObject* ArenaDoor3 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(637.64, -177.801, -52.8988, 161522);
		ArenaDoor3->SetUInt32Value(GAMEOBJECT_STATE, 1);
		GameObject* ArenaDoor4 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(585.739, -147.762, -52.8988, 161524);
		ArenaDoor4->SetUInt32Value(GAMEOBJECT_STATE, 0);
		Creature* NewCreature = NULL;
		for(int i=0;i<10;i++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SoldierSpawns[i].mX, SoldierSpawns[i].mY, SoldierSpawns[i].mZ, 8893);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		for(int j=0;j<14;j++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(PeasantSpawns[j].mX, PeasantSpawns[j].mY, PeasantSpawns[j].mZ, 8896);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		for(int k=0;k<25;k++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SpectatorSpawns[k].mX, SpectatorSpawns[k].mY, SpectatorSpawns[k].mZ, 8916);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		ParentClass::OnDied(pKiller);
	}
SpellDesc* DemonArmor;
};

class OkThorTheBreaker : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(OkThorTheBreaker, MoonScriptCreatureAI);
    OkThorTheBreaker(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15453, Target_Self, 6, 1.5, 0);
		AddSpell(15254, Target_Current, 10, 1, 0);
		AddSpell(13323, Target_RandomPlayerNotCurrent, 4, 1.5, 0);
		AddSpell(13747, Target_Current, 8, 0, 10);
    }
	void OnDied(Unit* pKiller)
	{
		GameObject* ArenaDoor1 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(555.751, -199.744, -52.908, 161523);		
		ArenaDoor1->SetUInt32Value(GAMEOBJECT_STATE, 0);
		GameObject* ArenaDoor3 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(637.64, -177.801, -52.8988, 161522);
		ArenaDoor3->SetUInt32Value(GAMEOBJECT_STATE, 1);
		GameObject* ArenaDoor4 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(585.739, -147.762, -52.8988, 161524);
		ArenaDoor4->SetUInt32Value(GAMEOBJECT_STATE, 0);
		Creature* NewCreature = NULL;
		for(int i=0;i<10;i++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SoldierSpawns[i].mX, SoldierSpawns[i].mY, SoldierSpawns[i].mZ, 8893);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		for(int j=0;j<14;j++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(PeasantSpawns[j].mX, PeasantSpawns[j].mY, PeasantSpawns[j].mZ, 8896);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		for(int k=0;k<25;k++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SpectatorSpawns[k].mX, SpectatorSpawns[k].mY, SpectatorSpawns[k].mZ, 8916);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		ParentClass::OnDied(pKiller);
	}
};

class Eviscerator : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Eviscerator, MoonScriptCreatureAI);
    Eviscerator(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(7121, Target_Self, 6, 2, 10);
		AddSpell(15245, Target_Current, 8, 3, 0);
		AddSpell(14331, Target_Current, 10, 0, 13);
	}
	void OnDied(Unit* pKiller)
	{
		GameObject* ArenaDoor1 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(555.751, -199.744, -52.908, 161523);
		ArenaDoor1->SetUInt32Value(GAMEOBJECT_STATE, 0);
		GameObject* ArenaDoor3 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(637.64, -177.801, -52.8988, 161522);
		ArenaDoor3->SetUInt32Value(GAMEOBJECT_STATE, 1);
		GameObject* ArenaDoor4 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(585.739, -147.762, -52.8988, 161524);
		ArenaDoor4->SetUInt32Value(GAMEOBJECT_STATE, 0);
		Creature* NewCreature = NULL;
		for(int i=0;i<10;i++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SoldierSpawns[i].mX, SoldierSpawns[i].mY, SoldierSpawns[i].mZ, 8893);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		for(int j=0;j<14;j++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(PeasantSpawns[j].mX, PeasantSpawns[j].mY, PeasantSpawns[j].mZ, 8896);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		for(int k=0;k<25;k++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SpectatorSpawns[k].mX, SpectatorSpawns[k].mY, SpectatorSpawns[k].mZ, 8916);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		ParentClass::OnDied(pKiller);
	}
};

class GoroshTheDervish : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(GoroshTheDervish, MoonScriptCreatureAI);
    GoroshTheDervish(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(21049, Target_Self, 5, 0, 40);
		AddSpell(15708, Target_Current, 10, 0, 0);
		//this one or 13736
		AddSpell(15589, Target_Current, 10, 0, 0);
	}
	void OnDied(Unit* pKiller)
	{
		GameObject* ArenaDoor1 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(555.751, -199.744, -52.908, 161523);
		ArenaDoor1->SetUInt32Value(GAMEOBJECT_STATE, 0);
		GameObject* ArenaDoor3 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(637.64, -177.801, -52.8988, 161522);
		ArenaDoor3->SetUInt32Value(GAMEOBJECT_STATE, 1);
		GameObject* ArenaDoor4 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(585.739, -147.762, -52.8988, 161524);
		ArenaDoor4->SetUInt32Value(GAMEOBJECT_STATE, 0);
		Creature* NewCreature = NULL;
		for(int i=0;i<10;i++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SoldierSpawns[i].mX, SoldierSpawns[i].mY, SoldierSpawns[i].mZ, 8893);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		for(int j=0;j<14;j++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(PeasantSpawns[j].mX, PeasantSpawns[j].mY, PeasantSpawns[j].mZ, 8896);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		for(int k=0;k<25;k++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SpectatorSpawns[k].mX, SpectatorSpawns[k].mY, SpectatorSpawns[k].mZ, 8916);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		ParentClass::OnDied(pKiller);
	}
};

class Grizzle : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Grizzle, MoonScriptCreatureAI);
    Grizzle(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(6524, Target_Current, 6, 0, 0);
		AddSpell(40504, Target_Current, 8, 0, 0);
		Frenzy = AddSpell(8269, Target_Self, 0, 0, 0);
	}
	void OnCombatStart(Unit* pTarget)
	{
		frenzyb = false;
		ParentClass::OnCombatStart(pTarget);
	}
	void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 40 && frenzyb == false)
		{
			CastSpellNowNoScheduling(Frenzy);
			frenzyb = true;
		}
		ParentClass::AIUpdate();
	}
	void OnDied(Unit* pKiller)
	{
		GameObject* ArenaDoor1 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(555.751, -199.744, -52.908, 161523);
		ArenaDoor1->SetUInt32Value(GAMEOBJECT_STATE, 0);
		GameObject* ArenaDoor3 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(637.64, -177.801, -52.8988, 161522);
		ArenaDoor3->SetUInt32Value(GAMEOBJECT_STATE, 1);
		GameObject* ArenaDoor4 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(585.739, -147.762, -52.8988, 161524);
		ArenaDoor4->SetUInt32Value(GAMEOBJECT_STATE, 0);
		Creature* NewCreature = NULL;
		for(int i=0;i<10;i++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SoldierSpawns[i].mX, SoldierSpawns[i].mY, SoldierSpawns[i].mZ, 8893);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		for(int j=0;j<14;j++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(PeasantSpawns[j].mX, PeasantSpawns[j].mY, PeasantSpawns[j].mZ, 8896);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		for(int k=0;k<25;k++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SpectatorSpawns[k].mX, SpectatorSpawns[k].mY, SpectatorSpawns[k].mZ, 8916);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		ParentClass::OnDied(pKiller);
	}
protected:
	SpellDesc* Frenzy;
	bool frenzyb;
};

class HedrumTheCreeper : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(HedrumTheCreeper, MoonScriptCreatureAI);
    HedrumTheCreeper(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15475, Target_Current, 10, 0, 0);
		AddSpell(15474, Target_Current, 7, 0, 0);
		AddSpell(3609, Target_Current, 6, 0, 0);
    }
	void OnDied(Unit* pKiller)
	{
		GameObject* ArenaDoor1 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(555.751, -199.744, -52.908, 161523);		
		ArenaDoor1->SetUInt32Value(GAMEOBJECT_STATE, 0);
		GameObject* ArenaDoor3 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(637.64, -177.801, -52.8988, 161522);
		ArenaDoor3->SetUInt32Value(GAMEOBJECT_STATE, 1);
		GameObject* ArenaDoor4 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(585.739, -147.762, -52.8988, 161524);
		ArenaDoor4->SetUInt32Value(GAMEOBJECT_STATE, 0);
		Creature* NewCreature = NULL;
		for(int i=0;i<10;i++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SoldierSpawns[i].mX, SoldierSpawns[i].mY, SoldierSpawns[i].mZ, 8893);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		for(int j=0;j<14;j++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(PeasantSpawns[j].mX, PeasantSpawns[j].mY, PeasantSpawns[j].mZ, 8896);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		for(int k=0;k<25;k++)
		{
			NewCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SpectatorSpawns[k].mX, SpectatorSpawns[k].mY, SpectatorSpawns[k].mZ, 8916);
			if(NewCreature)
			{
				NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 32);
				NewCreature->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			}
			NewCreature = NULL;
		}
		ParentClass::OnDied(pKiller);
	}
};

/**
	Blackrock Depths
	Pyromancer Loregrain
					**/

class PyromancerLoregrain : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(PyromancerLoregrain, MoonScriptCreatureAI);
    PyromancerLoregrain(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Scorch totem, needs check
		AddSpell(15038, Target_Self, 5, 0, 15);
		AddSpell(15095, Target_Current, 10, 2, 0);
		AddSpell(15616, Target_Current, 7, 0, 0);
		AddSpell(15041, Target_Self, 5, 0, 5);
	}
};

/**
	Blackrock Depths
	General Angerforge
					**/

class GeneralAngerforge : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(GeneralAngerforge, MoonScriptCreatureAI);
    GeneralAngerforge(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		Help = true;
		AddSpell(15572, Target_Current, 15, 0, 0);
	}
	void AIUpdate()
	{
		if(_unit->GetHealthPct() < 30 && Help == true)
		{
			Help = false;
			CallforHelp();
			_unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, "General Angerforge cries out an alarm!");
		}
		ParentClass::AIUpdate();
	}
	void CallforHelp()
	{
		Creature* Melee = NULL;
		for(int i=0; i<10; i++)
		{
			Melee = _unit->GetMapMgr()->GetInterface()->SpawnCreature(8901, 672.49+RandomFloat(4), 48.04, -60.0, 4.05, true, false, 0, 0);
			Melee->GetAIInterface()->StopMovement(0);
			Melee->GetAIInterface()->setMoveRunFlag(true);
			Melee->GetAIInterface()->MoveTo(_unit->GetPositionX()+RandomFloat(4), _unit->GetPositionY(), _unit->GetPositionZ(), 4.0);
			Melee->GetAIInterface()->GetNextTarget();
			Melee = NULL;
		}
		Creature* Healer = NULL;
		for(int j=0; j<3; j++)
		{
			Healer = _unit->GetMapMgr()->GetInterface()->SpawnCreature(8894, 670.49, 48.04, -60.0, 4.05, true, false, 0, 0);
			Healer->GetAIInterface()->StopMovement(0);
			Healer->GetAIInterface()->setMoveRunFlag(true);
			Healer->GetAIInterface()->MoveTo(_unit->GetPositionX()+RandomFloat(4), _unit->GetPositionY(), _unit->GetPositionZ(), 4.0);
			Healer->GetAIInterface()->GetNextTarget();
			Healer = NULL;
		}
	}
	bool Help;
};

/**
	Blackrock Depths
	Golem Lord Argelmach
					**/

class GolemLordArgelmach : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(GolemLordArgelmach, MoonScriptCreatureAI);
    GolemLordArgelmach(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15305, Target_Current, 8, 3, 6);
		AddSpell(15605, Target_Current, 10, 0, 6);
		AddSpell(15507, Target_Self, 5, 0, 15);
	}
	void OnCombatStart(Unit* pTarget)
	{
		for(int i=1; i<3;i++)
		{
			WayPoint * wp = _unit->CreateWaypointStruct();
			wp->id = i;
			wp->x = ArgelmachWP[i].mX;
			wp->y = ArgelmachWP[i].mY;
			wp->z = ArgelmachWP[i].mZ;
			wp->o = 1.6;
			wp->waittime = 0;
			wp->flags = RUN;
			wp->forwardemoteoneshot = 0;
			wp->forwardemoteid = 0;
			wp->backwardemoteoneshot = 0;
			wp->backwardemoteid = 0;
			wp->forwardskinid = 0;
			wp->backwardskinid = 0;
			_unit->GetAIInterface()->addWayPoint(wp);
		}
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		//_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->StopMovement(0);
		_unit->GetAIInterface()->setWaypointToMove(0);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Intruders in the Manufactory! My constructs will destroy you!");
		ParentClass::OnCombatStart(pTarget);
	}
	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 2)
		{
			_unit->GetAIInterface()->SetAIState(STATE_ATTACKING);
			_unit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			Creature* Technician1 = NULL;
			Technician1 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(804.53, 50.84, -53.58, 8920);
			Creature* Technician2 = NULL;
			Technician2 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(806.09, 54.24, -53.57, 8920);
			Creature* Technician3 = NULL;
			Technician3 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(810.03, 40.61, -53.64, 8920);
			Creature* Ragereaver = NULL;
			Ragereaver = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(807.14, 52.12, -53.57, 8920);
			if(Technician1 && Technician1->isAlive())
				Technician1->GetAIInterface()->GetNextTarget();
			if(Technician2 && Technician2->isAlive())
				Technician2->GetAIInterface()->GetNextTarget();
			if(Technician3 && Technician3->isAlive())
				Technician3->GetAIInterface()->GetNextTarget();
			if(Ragereaver && Ragereaver->isAlive())
				Ragereaver->GetAIInterface()->GetNextTarget();
		}
		ParentClass::OnReachWP(iWaypointId, bForwards);
	}
};

/**
	Blackrock Depths 
	"Grim Guzzler"
					  **/

class PluggerSpazzring : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(PluggerSpazzring, MoonScriptCreatureAI);
    PluggerSpazzring(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		DemonArmor = AddSpell(13787, Target_Self, 0, 0, 0);
		AddSpell(12739, Target_Current, 10, 2, 0);
		AddSpell(13338, Target_RandomPlayer, 6, 0, 0);
		AddSpell(8994, Target_RandomPlayerNotCurrent, 6, 1, 0);
		AddSpell(12742, Target_Current, 10, 3, 0);
	}

	void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(DemonArmor);
		ParentClass::OnCombatStart(pTarget);
	}
SpellDesc* DemonArmor;
};

class HurleyBlackbreath : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(HurleyBlackbreath, MoonScriptCreatureAI);
    HurleyBlackbreath(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(14872, Target_Current, 6, 0, 15);
		AddSpell(9573, Target_Current, 10, 2, 0);
	}
	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 3)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		}
	}
	void OnCombatStart(Unit* pTarget)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You'll pay for that!");
		ParentClass::OnCombatStart(pTarget);
	}
};

class RibblyScrewspigot : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(RibblyScrewspigot, MoonScriptCreatureAI);
    RibblyScrewspigot(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		RunNow = false;
		AddSpell(12540, Target_Current, 8, 0, 10);
		AddSpell(9080, Target_Current, 8, 0, 0);
	}
	void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 20 && RunNow == false)
		{
			_unit->GetAIInterface()->MoveTo(845.65, -131.62, -54.12, 2.15);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, "Ribbly Screwspigot attempts to run away in fear!");
			_unit->GetAIInterface()->setMoveRunFlag(true);
			RunNow = true;
		}
		ParentClass::AIUpdate();
	}
	
	bool RunNow;
};

class Phalanx : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Phalanx, MoonScriptCreatureAI);
    Phalanx(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15285, Target_Current, 10, 0, 0);
		AddSpell(15588, Target_Current, 6, 0, 0);
		AddSpell(14099, Target_Current, 6, 0, 0);
	}
	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 3)
		{
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);			
		}
		ParentClass::OnReachWP(iWaypointId, bForwards);
	}
};

/**
	Blackrock Depths 
	Lord Incendius
					  **/

class LordIncendius : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(LordIncendius, MoonScriptCreatureAI);
    LordIncendius(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(26977, Target_RandomPlayer, 6, 0, 0);
		AddSpell(13900, Target_Current, 100, 1.5, 8);
		AddSpell(13899, Target_Current, 100, 2, 10);
		AddSpell(14099, Target_Current, 100, 0, 10);
	}
};

/**
	Blackrock Depths 
	Fineous Darkvire
					  **/

class FineousDarkvire : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(FineousDarkvire, MoonScriptCreatureAI);
    FineousDarkvire(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15346, Target_Self, 100, 0, 30);
		AddSpell(13953, Target_Current, 10, 0, 0);
		AddSpell(11978, Target_Current, 5, 0, 2);
		AddSpell(15493, Target_WoundedFriendly, 4, 2.5, 5);
		DevoAura = AddSpell(8258, Target_Self, 0, 0, 0);
	}
	void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(DevoAura);
		ParentClass::OnCombatStart(pTarget);
	}
	SpellDesc* DevoAura;
};

/**
	Blackrock Depths 
	Warder Stilgiss & Verek
					  **/

class WarderStilgiss : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(WarderStilgiss, MoonScriptCreatureAI);
    WarderStilgiss(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		ApplyAura(12544);
		AddSpell(12674, Target_Current, 7, 0, 0);
		AddSpell(15044, Target_Self, 5, 0, 5);
		AddSpell(12675, Target_Current, 10, 3, 0);
	}
};

class Verek : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Verek, MoonScriptCreatureAI);
    Verek(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15042, Target_Current, 8, 0, 0);
		AddSpell(8599, Target_Self, 6, 0, 20);
	}
};

/**
	Blackrock Depths 
	"The Vault"
					  **/

class DarkKeeperBethek : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DarkKeeperBethek, MoonScriptCreatureAI);
    DarkKeeperBethek(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(12738, Target_Current, 7, 0, 10);
		AddSpell(15228, Target_Current, 20, 3, 0);
	}
};

class DarkKeeperOfgut : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DarkKeeperOfgut, MoonScriptCreatureAI);
    DarkKeeperOfgut(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(7121, Target_Self, 7, 2, 13);
		AddSpell(15228, Target_Current, 20, 1, 0);
	}
};

class DarkKeeperPelver : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DarkKeeperPelver, MoonScriptCreatureAI);
    DarkKeeperPelver(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		ApplyAura(12544);
		AddSpell(12675, Target_Current, 20, 3, 0);
	}
};

class DarkKeeperUggel : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DarkKeeperUggel, MoonScriptCreatureAI);
    DarkKeeperUggel(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(14875, Target_Current, 7, 0, 13);
		AddSpell(15232, Target_Current, 20, 3, 0);
	}
};

class DarkKeeperVorfalk : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DarkKeeperVorfalk, MoonScriptCreatureAI);
    DarkKeeperVorfalk(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15234, Target_Current, 20, 3, 0);
		AddSpell(15236, Target_Current, 7, 0, 6);
	}
};

class DarkKeeperZimrel : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DarkKeeperZimrel, MoonScriptCreatureAI);
    DarkKeeperZimrel(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(11974, Target_WoundedFriendly, 7, 0, 6);
		AddSpell(15238, Target_Current, 20, 2.5, 0);
	}
};

/**
	Blackrock Depths 
	Ambassador Flamelash
					  **/

class AmbassadorFlamelash : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(AmbassadorFlamelash, MoonScriptCreatureAI);
    AmbassadorFlamelash(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		mMinionTimer = INVALIDATE_TIMER;
		DarkDwarfRune1 = NULL;
		DarkDwarfRune2 = NULL;
		DarkDwarfRune3 = NULL;
		DarkDwarfRune4 = NULL;
		DarkDwarfRune5 = NULL;
		DarkDwarfRune6 = NULL;
		DarkDwarfRune7 = NULL;
	}
	void OnCombatStart(Unit* pTarget)
	{
		DarkDwarfRune1 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1029.22, -257.987, -61.9462, 170578);
		DarkDwarfRune2 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1046.23, -241.007, -61.9462, 170579);
		DarkDwarfRune3 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1012.28, -274.911, -61.9462, 170580);
		DarkDwarfRune4 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1029.29, -224.058, -61.9462, 170581);
		DarkDwarfRune5 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(995.262, -257.966, -61.9462, 170582);
		DarkDwarfRune6 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1012.31, -207.095, -61.9462, 170583);
		DarkDwarfRune7 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(978.344, -240.967, -61.9462, 170584);

		DarkDwarfRune1->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		DarkDwarfRune1->SetUInt32Value(GAMEOBJECT_STATE, 0);
		DarkDwarfRune2->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		DarkDwarfRune2->SetUInt32Value(GAMEOBJECT_STATE, 0);
		DarkDwarfRune3->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		DarkDwarfRune3->SetUInt32Value(GAMEOBJECT_STATE, 0);
		DarkDwarfRune4->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		DarkDwarfRune4->SetUInt32Value(GAMEOBJECT_STATE, 0);
		DarkDwarfRune5->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		DarkDwarfRune5->SetUInt32Value(GAMEOBJECT_STATE, 0);
		DarkDwarfRune6->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		DarkDwarfRune6->SetUInt32Value(GAMEOBJECT_STATE, 0);
		DarkDwarfRune7->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		DarkDwarfRune7->SetUInt32Value(GAMEOBJECT_STATE, 0);
		mMinionTimer = AddTimer( 4000 );
		ParentClass::OnCombatStart(pTarget);
	}
	void AIUpdate()
	{
		if(IsTimerFinished( mMinionTimer ))
		{
			ResetTimer( mMinionTimer, 20000 ); 		
			SpawnMinions();
		}
		ParentClass::AIUpdate();
	}
	void SpawnMinions()
	{
		Creature* Minion = NULL;
		for(int i=0; i<7;i++)
		{
			Minion = _unit->GetMapMgr()->GetInterface()->SpawnCreature(9178, AmbassadorRunes[i].mX, AmbassadorRunes[i].mY, AmbassadorRunes[i].mZ, 2.0, true, false, 0, 0);
			WayPoint * wp = Minion->CreateWaypointStruct();
			wp->id = 1;
			wp->x = _unit->GetPositionX();
			wp->y = _unit->GetPositionY();
			wp->z = _unit->GetPositionZ();
			wp->o = 1.0;
			wp->waittime = 0;
			wp->flags = RUN;
			wp->forwardemoteoneshot = 0;
			wp->forwardemoteid = 0;
			wp->backwardemoteoneshot = 0;
			wp->backwardemoteid = 0;
			wp->forwardskinid = 0;
			wp->backwardskinid = 0;

			Minion->GetAIInterface()->addWayPoint(wp);
			Minion->GetAIInterface()->StopMovement(0);
			Minion->GetAIInterface()->setWaypointToMove(0);
			Minion = NULL;
		}
	}

protected:
	int32 mMinionTimer;
	GameObject* DarkDwarfRune1;
	GameObject* DarkDwarfRune2;
	GameObject* DarkDwarfRune3;
	GameObject* DarkDwarfRune4;
	GameObject* DarkDwarfRune5;
	GameObject* DarkDwarfRune6;
	GameObject* DarkDwarfRune7;
};

class AmbassadorMinion : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(AmbassadorMinion, MoonScriptCreatureAI);
    AmbassadorMinion(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		BurningSpirit = AddSpell(13489, Target_Self, 0, 0, 0);
		//Aura* BS = AddSpell(14744, Target_Self, 0, 0, 0);
	}
	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 1)
		{
			CastSpellNowNoScheduling(BurningSpirit);
			_unit->Despawn(3000, 0);
			Creature* Ambassador = NULL;
			Ambassador = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 9156);
			if(Ambassador && Ambassador->isAlive())
			{
				//Ambassador->CastSpell(Ambassador, 9156, false);
				 Aura* Burning = new Aura(dbcSpell.LookupEntry(14744), (uint32)-1, _unit, Ambassador);
				if (Burning != NULL)
				{
					Ambassador->AddAura(Burning);
				}
				//Ambassador->AddAuraVisual(14744, 1, true);
			}
		}
		ParentClass::OnReachWP(iWaypointId, bForwards);
	}
	SpellDesc* BurningSpirit;
};

/**
	Blackrock Depths 
	"The Seven"
					  **/

class Haterel : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Haterel, MoonScriptCreatureAI);
    Haterel(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(14033, Target_RandomPlayer, 8, 3, 0);
		AddSpell(15232, Target_Self, 10, 3, 0);
		AddSpell(12040, Target_Self, 5, 0, 15);
		AddSpell(15580, Target_Current, 10, 0, 0);
	}
	void OnDied(Unit* pKiller)
	{
		Creature* Angerel = NULL;
		Angerel = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1215.05, -220.65, -85.59, 9035);
		if(Angerel != NULL)
		{
			Angerel->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14); //dont know if its the right one
			Angerel->GetAIInterface()->setMoveRunFlag(true);
			Angerel->GetAIInterface()->GetNextTarget();
			Angerel->GetAIInterface()->MoveTo(pKiller->GetPositionX(), pKiller->GetPositionY(), pKiller->GetPositionZ(), 2.0);
		}
		ParentClass::OnDied(pKiller);
	}
};

class Angerel : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Angerel, MoonScriptCreatureAI);
    Angerel(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(11971, Target_Current, 8, 0, 0);
		ShieldWall = AddSpell(15062, Target_Self, 0, 0, 0);
		AddSpell(12169, Target_Self, 8, 0, 15);
		AddSpell(15580, Target_Current, 10, 0, 0);
	}
	void OnCombatStart(Unit* pTarget)
	{
		shieldw = false;
		ParentClass::OnCombatStart(pTarget);
	}
	void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 20 && shieldw == false)
		{
			CastSpellNowNoScheduling(ShieldWall);
			shieldw = true;
		}
		ParentClass::AIUpdate();
	}
	void OnDied(Unit* pKiller)
	{
		Creature* Seethrel = NULL;
		Seethrel = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1286.56, -253.331, -78.0612, 9038);
		if(Seethrel != NULL)
		{
			Seethrel->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14); //dont know if its the right one
			Seethrel->GetAIInterface()->setMoveRunFlag(true);
			Seethrel->GetAIInterface()->GetNextTarget();
			Seethrel->GetAIInterface()->MoveTo(pKiller->GetPositionX(), pKiller->GetPositionY(), pKiller->GetPositionZ(), 2.0);
		}
		ParentClass::OnDied(pKiller);
	}
	SpellDesc* ShieldWall;
	bool shieldw;
};

class Seethrel : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Seethrel, MoonScriptCreatureAI);
    Seethrel(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(9080, Target_Current, 6, 0, 0);
		AddSpell(13737, Target_Current, 8, 0, 0);
		AddSpell(13847, Target_Self, 5, 0, 15);
		AddSpell(40504, Target_Current, 8, 0, 0);
	}
	void OnDied(Unit* pKiller)
	{
		Creature* Doperel = NULL;
		Doperel = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1236.55, -291.534, -78.1351, 9040);
		if(Doperel != NULL)
		{
			Doperel->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14); //dont know if its the right one
			Doperel->GetAIInterface()->setMoveRunFlag(true);
			Doperel->GetAIInterface()->GetNextTarget();
			Doperel->GetAIInterface()->MoveTo(pKiller->GetPositionX(), pKiller->GetPositionY(), pKiller->GetPositionZ(), 2.0);
		}
		ParentClass::OnDied(pKiller);
	}
};

class Doperel : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Doperel, MoonScriptCreatureAI);
    Doperel(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(12540, Target_Current, 6, 0, 0);
		AddSpell(15581, Target_Current, 8, 0, 0);
		AddSpell(15087, Target_Self, 5, 0, 15);
		AddSpell(15583, Target_Current, 6, 0, 0);
		AddSpell(15582, Target_Current, 6, 0, 0);
	}
	void OnDied(Unit* pKiller)
	{
		Creature* Gloomrel = NULL;
		Gloomrel = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1226.69, -246.124, -85.5903, 9037);
		if(Gloomrel != NULL)
		{
			Gloomrel->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14); //dont know if its the right one
			Gloomrel->GetAIInterface()->setMoveRunFlag(true);
			Gloomrel->GetAIInterface()->GetNextTarget();
			Gloomrel->GetAIInterface()->MoveTo(pKiller->GetPositionX(), pKiller->GetPositionY(), pKiller->GetPositionZ(), 2.0);
		}
		ParentClass::OnDied(pKiller);
	}
	void Destroy()
	{
		delete this;
	}
};

class Gloomrel : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Gloomrel, MoonScriptCreatureAI);
    Gloomrel(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(12675, Target_Current, 10, 0, 0);
		AddSpell(12674, Target_Current, 8, 0, 0);
		AddSpell(15044, Target_Self, 5, 0, 15);
		AddSpell(40504, Target_Current, 8, 0, 0);
		ApplyAura(12544);
	}
	void OnDied(Unit* pKiller)
	{
		Creature* Vilerel = NULL;
		Vilerel = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1272.32, -199.817, -85.5905, 9036);
		if(Vilerel != NULL)
		{
			Vilerel->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14); //dont know if its the right one
			Vilerel->GetAIInterface()->setMoveRunFlag(true);
			Vilerel->GetAIInterface()->GetNextTarget();
			Vilerel->GetAIInterface()->MoveTo(pKiller->GetPositionX(), pKiller->GetPositionY(), pKiller->GetPositionZ(), 2.0);
		}
		ParentClass::OnDied(pKiller);
	}
	SpellDesc* FrostArmor;
};

class Vilerel : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Vilerel, MoonScriptCreatureAI);
    Vilerel(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15587, Target_Current, 8, 1.5, 0);
		Heal = AddSpell(15586, Target_Self, 0, 0, 0);
		AddSpell(11974, Target_Self, 5, 0, 20);
		AddSpell(15585, Target_Self, 5, 3, 0);
	}
	void OnCombatStart(Unit* pTarget)
	{
		heals = false;
		ParentClass::OnCombatStart(pTarget);
	}
	void AIUpdate()
	{
		if(_unit->GetHealthPct() <= 30 && heals == false)
		{
			CastSpellNowNoScheduling(Heal);
			heals = true;
		}
		ParentClass::AIUpdate();
	}
	void OnDied(Unit* pKiller)
	{
		Creature* Doomrel = NULL;
		Doomrel = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1281.14, -282.151, -78.1359, 9039);
		if(Doomrel != NULL)
		{
			Doomrel->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14); //dont know if its the right one
			Doomrel->GetAIInterface()->setMoveRunFlag(true);
			Doomrel->GetAIInterface()->GetNextTarget();
			Doomrel->GetAIInterface()->MoveTo(pKiller->GetPositionX(), pKiller->GetPositionY(), pKiller->GetPositionZ(), 2.0);
		}
		ParentClass::OnDied(pKiller);
	}
	bool heals;
	SpellDesc* Heal;
};

class Doomrel : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Doomrel, MoonScriptCreatureAI);
    Doomrel(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(8994, Target_RandomPlayerNotCurrent, 8, 1, 0);
		AddSpell(12493, Target_Current, 6, 0, 0);
		AddSpell(15092, Target_Self, 5, 0, 15);
		AddSpell(12742, Target_Current, 8, 0, 0);
		AddSpell(15245, Target_Current, 7, 3, 0);
		ApplyAura(13787);
	}
	void OnDied(Unit* pKiller)
	{
		GameObject* SevenDoor1 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1230.33, -205.21, -85.6843, 170576);	//Add SevenDoor1 here
		GameObject* SevenDoor2 = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1262.85, -293.797, -78.1676, 170577);	//Add SevenDoor2 here
		GameObject* SevenChest = NULL;
		SevenChest = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(169243, 1266.01, -284.82, -78.21, 2.5, true, 0, 0);
		//Open both doors
		SevenDoor1->SetUInt32Value(GAMEOBJECT_STATE, 0);	
		SevenDoor1->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		SevenDoor2->SetUInt32Value(GAMEOBJECT_STATE, 0);		
		SevenDoor2->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		ParentClass::OnDied(pKiller);
	}
};

/**
	Blackrock Depths 
	Magmus & Flamethrowers
					  **/

class Magmus : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Magmus, MoonScriptCreatureAI);
    Magmus(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		mRandomCastTimer = INVALIDATE_TIMER;
		AddSpell(15668, Target_Destination, 10, 2, 0);
		AddSpell(15593, Target_Current, 8, 0, 8);
	}
	void OnCombatStart(Unit* pTarget)
	{
		//WOOOORK FFFSSSSSSS
		/*for(int i=0;i<3;i++)
		{
			RandomGuardCast();
		}
		mRandomCastTimer = AddTimer ( 10000 );*/
		ParentClass::OnCombatStart(pTarget);
	}
	void AIUpdate()
	{
		if(IsTimerFinished( mRandomCastTimer ))
		{
			RandomGuardCast();
			ResetTimer( mRandomCastTimer, 10000 );
		}
		ParentClass::AIUpdate();
	}
	void RandomGuardCast()
	{
		switch(RandomUInt(5))
		{
		case 0:
			{
				Creature* IronhandGuardian = NULL;
				IronhandGuardian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(RandomGuardSpawn[0].mX, RandomGuardSpawn[0].mY, RandomGuardSpawn[0].mZ, 8982);
				if(IronhandGuardian)
				{
					if(IronhandGuardian->isAlive())
					{
						IronhandGuardian->CastSpell(_unit, 15538/*15529*/, false);
					}
				}
				IronhandGuardian = NULL;
			} break;
		case 1:
			{
				Creature* IronhandGuardian = NULL;
				IronhandGuardian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(RandomGuardSpawn[1].mX, RandomGuardSpawn[1].mY, RandomGuardSpawn[1].mZ, 8982);
				if(IronhandGuardian)
				{
					if(IronhandGuardian->isAlive())
					{
						IronhandGuardian->CastSpell(_unit, 15538/*15529*/, false);
					}
				}
				IronhandGuardian = NULL;
			} break;
		case 2:
			{
				Creature* IronhandGuardian = NULL;
				IronhandGuardian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(RandomGuardSpawn[2].mX, RandomGuardSpawn[2].mY, RandomGuardSpawn[2].mZ, 8982);
				if(IronhandGuardian)
				{
					if(IronhandGuardian->isAlive())
					{
						IronhandGuardian->CastSpell(_unit, 15538/*15529*/, false);
					}
				}
				IronhandGuardian = NULL;
			} break;
		case 3:
			{
				Creature* IronhandGuardian = NULL;
				IronhandGuardian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(RandomGuardSpawn[3].mX, RandomGuardSpawn[3].mY, RandomGuardSpawn[3].mZ, 8982);
				if(IronhandGuardian)
				{
					if(IronhandGuardian->isAlive())
					{
						IronhandGuardian->CastSpell(_unit, 15538/*15529*/, false);
					}
				}
				IronhandGuardian = NULL;
			} break;
		case 4:
			{
				Creature* IronhandGuardian = NULL;
				IronhandGuardian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(RandomGuardSpawn[4].mX, RandomGuardSpawn[4].mY, RandomGuardSpawn[4].mZ, 8982);
				if(IronhandGuardian)
				{
					if(IronhandGuardian->isAlive())
					{
						IronhandGuardian->CastSpell(_unit, 15538/*15529*/, false);
					}
				}
				IronhandGuardian = NULL;
			} break;
		case 5:
			{
				Creature* IronhandGuardian = NULL;
				IronhandGuardian = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(RandomGuardSpawn[5].mX, RandomGuardSpawn[5].mY, RandomGuardSpawn[5].mZ, 8982);
				if(IronhandGuardian)
				{
					if(IronhandGuardian->isAlive())
					{
						IronhandGuardian->CastSpell(_unit, 15538/*15529*/, false);
					}
				}
				IronhandGuardian = NULL;
			} break;
		}
	}
	int32 mRandomCastTimer;
};

class IronhandGuardian : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(IronhandGuardian, MoonScriptCreatureAI);
    IronhandGuardian(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		Flames = AddSpell(15529, Target_Destination, 0, 10, 0);
		_unit->GetAIInterface()->MoveTo( _unit->GetSpawnX(), _unit->GetSpawnY(), _unit->GetSpawnZ(), _unit->GetSpawnO() );
		SetCanEnterCombat( false );
		SetFlyMode( true );
		_unit->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );
		RegisterAIUpdateEvent( 1000 );
		mFlamesTimer = AddTimer( 5000 );
	}
	void AIUpdate()
	{
		if(IsTimerFinished( mFlamesTimer ))
		{
			ResetTimer( mFlamesTimer, 15000 );
			CastSpellNowNoScheduling(Flames);
		}
		ParentClass::AIUpdate();
	}
	int32 mFlamesTimer;
	SpellDesc* Flames;
};

/**
	Blackrock Depths 
	Princess Moira Bronzebeard 
	& Emperor Thaurissan
					  **/

class PrincessMoiraBronzebeard : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(PrincessMoiraBronzebeard, MoonScriptCreatureAI);
    PrincessMoiraBronzebeard(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15586, Target_Self, 5, 3.5, 0);
		AddSpell(8362, Target_WoundedFriendly, 3, 2, 0);
		AddSpell(15654, Target_Current, 8, 0, 18);
		AddSpell(15587, Target_Current, 8, 0, 0);
		AddSpell(15537, Target_Current, 10, 3, 0);
	}
};

class EmperorDagranThaurissan : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(EmperorDagranThaurissan, MoonScriptCreatureAI);
    EmperorDagranThaurissan(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15636, Target_Self, 8, 0, 0);
		AddSpell(17492, Target_Current, 10, 0, 0);
	}
	void OnCombatStart(Unit* pTarget)
	{
		int counter = 0;
		Creature* Dragoon = NULL;
		Creature* TwilightAmbassador = NULL;
		Creature* Senator = NULL;

		//Do this several times for different mob ID's -- this fails
		for(int i = 1;i<9;i++)
		{
			Dragoon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(EmperorRoomDragoon[i].mX, EmperorRoomDragoon[i].mY, EmperorRoomDragoon[i].mZ, 8899);
			if(Dragoon == NULL)
			{
				counter++;
				return;
			}
			else
			{
				Dragoon->GetAIInterface()->setMoveRunFlag(true);
				Dragoon->GetAIInterface()->MoveTo(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 1.0f);
				Dragoon->GetAIInterface()->GetNextTarget();
				Dragoon = NULL;
			}
		}
		for(int i = 1;i<29;i++)
		{
			Senator = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(EmperorRoomSenator[i].mX, EmperorRoomSenator[i].mY, EmperorRoomSenator[i].mZ, 8904);
		    if(Senator == NULL)
			{
				counter++;
				return;
			}
			else
			{
				Senator->GetAIInterface()->setMoveRunFlag(true);
				Senator->GetAIInterface()->MoveTo(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 1.0f);
				Senator->GetAIInterface()->GetNextTarget();
				Senator = NULL;
			}
		}
		for(int i = 1;i<17;i++)
		{
			TwilightAmbassador = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(EmperorRoomTwilight[i].mX, EmperorRoomTwilight[i].mY, EmperorRoomTwilight[i].mZ, 8915);
			if(TwilightAmbassador == NULL)
			{
				counter++;
				return;
			}
			else
			{
				TwilightAmbassador->GetAIInterface()->setMoveRunFlag(true);
				TwilightAmbassador->GetAIInterface()->MoveTo(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 1.0f);
				TwilightAmbassador->GetAIInterface()->GetNextTarget();
				TwilightAmbassador = NULL;
			}
		}
		if(counter>1)
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Thank you for clearing out those foolish senators. Now prepare to meet your doom at the hands of Ragnaros' most powerful servant.");
		else
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Come to the aid of the Throne!");

		ParentClass::OnCombatStart(pTarget);
	}
	void OnDied(Unit* pKiller)
	{
		Creature* Princess = NULL;
		Princess = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 8929);
		if(Princess != NULL)
		{
			if(Princess->isAlive())
			{
				Princess->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35); //find friendly ID
				Princess->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
				Princess->GetAIInterface()->MoveTo(Princess->GetSpawnX(), Princess->GetSpawnY(), Princess->GetSpawnZ(), Princess->GetSpawnO());
			}
		}
		ParentClass::OnDied(pKiller);
	}
};

void SetupBlackrockDepthsBosses(ScriptMgr * mgr)
{
	//Bosses using WoWhead order
	mgr->register_creature_script(9025, &LordRoccor::Create);
	mgr->register_creature_script(9016, &BaelGar::Create);
	mgr->register_creature_script(9319, &HoundmasterGrebmar::Create);
	mgr->register_creature_script(9018, &HighInterrogatorGerstahn::Create);

	//Ring of Law NPCs ----------------------------------------------------
	mgr->register_creature_script(10096, &HighJusticeGrimstone::Create);
	mgr->register_creature_script(9031, &AnubShiah::Create);
	mgr->register_creature_script(9029, &Eviscerator::Create);
	mgr->register_creature_script(9028, &Grizzle::Create);
	mgr->register_creature_script(9027, &GoroshTheDervish::Create);
	mgr->register_creature_script(9030, &OkThorTheBreaker::Create);
	mgr->register_creature_script(9032, &HedrumTheCreeper::Create);
	//---------------------------------------------------------------------

	mgr->register_creature_script(9024, &PyromancerLoregrain::Create);
	mgr->register_creature_script(9033, &GeneralAngerforge::Create);
	mgr->register_creature_script(8983, &GolemLordArgelmach::Create);

	//Grim Guzzler NPCs ---------------------------------------------------
	mgr->register_creature_script(9543, &RibblyScrewspigot::Create);	
	mgr->register_creature_script(9537, &HurleyBlackbreath::Create);
	mgr->register_creature_script(9499, &PluggerSpazzring::Create);
	mgr->register_creature_script(9502, &Phalanx::Create);
	/*** Gossip Ribbly ***/
	GossipScript * RibGossip = (GossipScript*) new RibblyGossip();
	mgr->register_gossip_script( 9543, RibGossip );
	//---------------------------------------------------------------------

	mgr->register_creature_script(9017, &LordIncendius::Create);
	mgr->register_creature_script(9056, &FineousDarkvire::Create);
	mgr->register_creature_script(9041, &WarderStilgiss::Create);
	mgr->register_creature_script(9042, &Verek::Create);

	//The Vault NPCs ------------------------------------------------------
	mgr->register_creature_script(9438, &DarkKeeperBethek::Create);
	mgr->register_creature_script(9442, &DarkKeeperOfgut::Create);
	mgr->register_creature_script(9443, &DarkKeeperPelver::Create);
	mgr->register_creature_script(9439, &DarkKeeperUggel::Create);
	mgr->register_creature_script(9437, &DarkKeeperVorfalk::Create);
	mgr->register_creature_script(9441, &DarkKeeperZimrel::Create);
	//---------------------------------------------------------------------

	mgr->register_creature_script(9156, &AmbassadorFlamelash::Create);
	mgr->register_creature_script(9178, &AmbassadorMinion::Create);

	//The Seven NPCs ------------------------------------------------------
	mgr->register_creature_script(9034, &Haterel::Create);
	mgr->register_creature_script(9035, &Angerel::Create);
	mgr->register_creature_script(9036, &Vilerel::Create);
	mgr->register_creature_script(9037, &Gloomrel::Create);
	mgr->register_creature_script(9038, &Seethrel::Create);
	mgr->register_creature_script(9039, &Doomrel::Create);
	mgr->register_creature_script(9040, &Doperel::Create);
	/*** Doomrel Gossip ***/
	GossipScript * DoomGossip = (GossipScript*) new DoomrelGossip();
	mgr->register_gossip_script( 9039, DoomGossip );
	//---------------------------------------------------------------------

	mgr->register_creature_script(9938, &Magmus::Create);
	mgr->register_creature_script(8982, &IronhandGuardian::Create);
	mgr->register_creature_script(8929, &PrincessMoiraBronzebeard::Create);
	mgr->register_creature_script(9019, &EmperorDagranThaurissan::Create);
	
	//Triggers ------------------------------------------------------------
	mgr->register_creature_script(ARENA_TRIGGER, &RingOfLawTrigger::Create);
	//---------------------------------------------------------------------	

	//Quests --------------------------------------------------------------
	mgr->register_creature_script(9503, &PrivateRocknot::Create);
	QuestScript *RocknotsAle = (QuestScript*) new Quest_RocknotsAle();
	mgr->register_quest_script(4295, RocknotsAle);
	//---------------------------------------------------------------------	
	
	//GameObjects ---------------------------------------------------------
	mgr->register_gameobject_script(161461, &GiantMechanism::Create);
	mgr->register_gameobject_script(164913, &KegOne::Create);
	mgr->register_gameobject_script(174745, &BrazierLeft::Create);
	mgr->register_gameobject_script(174744, &BrazierRight::Create);
	mgr->register_gameobject_script(170607, &KegShotgun::Create);
	mgr->register_gameobject_script(161456, &KeeperPortrait::Create);
	//---------------------------------------------------------------------	
}