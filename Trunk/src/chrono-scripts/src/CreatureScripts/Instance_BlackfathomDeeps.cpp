/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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

/************************************************************************/
#define CN_Ghamoora 4887
class GhamooraAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GhamooraAI, MoonScriptCreatureAI);
    GhamooraAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Trample
		AddSpell(5568, Target_Self, 12, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_LadySarevess 4831
class LadySarevessAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LadySarevessAI, MoonScriptCreatureAI);
    LadySarevessAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Forked Lightning
		AddSpell(8435, Target_Current, 10, 2, 0);
		// Frost Nova
		AddSpell(865, Target_Self, 15, 0, 25);
		// Slow
		AddSpell(246, Target_Current, 15, 0, 10);
		AddEmote(Event_OnCombatStart, "You should not be here! Slay them!", Text_Yell, 5799);
		//AddEmote(Event_OnTargetDied, "text", Text_Yell, 5801);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_Gelihast 6243
class GelihastAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GelihastAI, MoonScriptCreatureAI);
    GelihastAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Net
		AddSpell(6533, Target_RandomPlayer, 10, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_LorgusJett 12902
class LorgusJettAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LorgusJettAI, MoonScriptCreatureAI);
    LorgusJettAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Lightning Bolt
		AddSpell(12167, Target_Current, 20, 3, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define CN_BaronAquanis 12876
class BaronAquanisAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BaronAquanisAI, MoonScriptCreatureAI);
    BaronAquanisAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Frostbolt
		AddSpell(15043, Target_Current, 20, 3, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

//Candle GO ID's:
//Door: 21117 (-818.361f, -200.647f, -25.7911f, -1.5708f)
//Right front: 21121 (-823.88f, -158.535f, -24.5278f, 0.925024f) 
//Right back: 21120 (-823.955f, -170.407f, -24.526f7, 2.82743f)
//Left back: 21119 (-813.578f, -170.461f, -24.5276f, 0.10472f)
//Left front: 21118 (-813.47f, -158.535f, -24.5271f, -1.51844)
#define GO_FLAG_UNCLICKABLE 0x00000010
class Door : public GameObjectAIScript
{
public:
	Door(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new Door(GO);}

    void OnCreate()
	{
		_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
	}
		
	
};

#define GO_FLAG_UNCLICKABLE 0x00000010

class CandleRightFront : public GameObjectAIScript
{
public:
	CandleRightFront(GameObject* goinstance) : GameObjectAIScript(goinstance) {
	used = false;
	}
	static GameObjectAIScript *Create(GameObject* GO) { return new CandleRightFront(GO); }

	void OnActivate(Player* pPlayer)
	{
		int counter = 0;
		GameObject* CandleRightBack = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-813.578f, -170.461f, -24.5276f, 21120);
		GameObject* CandleLeftBack = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-823.955f, -170.407f, -24.5267f, 21119);
		GameObject* CandleLeftFront = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-823.88f, -158.535f, -24.5278f, 21118);

		if(CandleLeftFront->GetUInt32Value(GAMEOBJECT_FLAGS) == GO_FLAG_UNCLICKABLE)
			counter++;
		if(CandleLeftBack->GetUInt32Value(GAMEOBJECT_FLAGS) == GO_FLAG_UNCLICKABLE)
			counter++;
		if(CandleRightBack->GetUInt32Value(GAMEOBJECT_FLAGS) == GO_FLAG_UNCLICKABLE)
			counter++;
		if(used == false)
		{
			switch(counter)
			{
			case 0:
				{
					Creature* Right = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4825, -867.804321f, -153.597672f, -25.880451f, 6.270435f, true, false, 0, 0);			
					Creature* Left1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4825, -768.702393f, -164.399490f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4825, -768.711121f, -174.439697f, -25.871897f, 3.203451f, true, false, 0, 0);

					_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

					WayPoint * wp = Right->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -155.576004f;
					wp->z = -25.792299f;
					wp->o = 4.747290f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left1->GetAIInterface()->addWayPoint(wp);
					Left2->GetAIInterface()->addWayPoint(wp);
					Right->GetAIInterface()->addWayPoint(wp);

					Left1->GetAIInterface()->StopMovement(0);
					Left2->GetAIInterface()->StopMovement(0);
					Right->GetAIInterface()->StopMovement(0);

					Left1->GetAIInterface()->setWaypointToMove(0);
					Left2->GetAIInterface()->setWaypointToMove(0);
					Right->GetAIInterface()->setWaypointToMove(0);

					Left1->Despawn(600000, 0);
					Left2->Despawn(600000, 0);
					Right->Despawn(600000, 0);

					used = true;
				} break;
			case 1:
				{
					Creature* Right1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -868.096924f, -164.041473f, -25.873016f, 6.227231f, true, false, 0, 0);
					Creature* Right2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -868.096924f, -174.364594f, -25.871803f, 0.061856f, true, false, 0, 0);
					Creature* Left1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -768.711121f, -174.439697f, -25.871897f, 3.203451f, true, false, 0, 0);
					Creature* Left2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -768.759888f, -154.085938f, -25.880392f, 3.164181f, true, false, 0, 0);

					_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

					WayPoint * wp = Right1->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -155.576004f;
					wp->z = -25.792299f;
					wp->o = 4.747290f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left1->GetAIInterface()->addWayPoint(wp);
					Left2->GetAIInterface()->addWayPoint(wp);
					Right1->GetAIInterface()->addWayPoint(wp);
					Right2->GetAIInterface()->addWayPoint(wp);

					Left1->GetAIInterface()->StopMovement(0);
					Left2->GetAIInterface()->StopMovement(0);
					Right1->GetAIInterface()->StopMovement(0);
					Right2->GetAIInterface()->StopMovement(0);

					Left1->GetAIInterface()->setWaypointToMove(0);
					Left2->GetAIInterface()->setWaypointToMove(0);
					Right1->GetAIInterface()->setWaypointToMove(0);
					Right2->GetAIInterface()->setWaypointToMove(0);

					Left1->Despawn(600000, 0);
					Left2->Despawn(600000, 0);
					Right1->Despawn(600000, 0);
					Right2->Despawn(600000, 0);

					used = true;
				} break;
			case 2:
				{
					Creature* Right = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4978, -868.096924f, -164.041473f, -25.873016f, 6.227231f, true, false, 0, 0);
					Creature* Left = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4978, -768.702393f, -164.399490f, -25.874142f, 3.054228f, true, false, 0, 0);

					_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

					WayPoint * wp = Right->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -174.216004f;
					wp->z = -25.792299f;
					wp->o = 1.579253f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left->GetAIInterface()->addWayPoint(wp);
					Right->GetAIInterface()->addWayPoint(wp);

					Left->GetAIInterface()->StopMovement(0);
					Right->GetAIInterface()->StopMovement(0);

					Left->GetAIInterface()->setWaypointToMove(0);
					Right->GetAIInterface()->setWaypointToMove(0);

					Left->Despawn(600000, 0);
					Right->Despawn(600000, 0);

					used = true;
				} break;
			case 3:
				{
					GameObject* Door = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-818.361f, -200.647f, -25.7911f, 21117);
					Door->SetUInt32Value(GAMEOBJECT_STATE, 0);
					Door->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

					_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

					Creature* Right1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -174.364594f, -25.871803f, 0.061856f, true, false, 0, 0);
					Creature* Right2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -151.5f, -25.874142f, 0.061856f, true, false, 0, 0);
					Creature* Right3 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -156.5f, -25.874142f, 0.061856f, true, false, 0, 0);
					Creature* Right4 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -161.5f, -25.874142f, 0.061856f, true, false, 0, 0);
					Creature* Right5 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -166.5f, -25.874142f, 0.061856f, true, false, 0, 0);

					Creature* Left1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -174.399490f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -166.6f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left3 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -161.6f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left4 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -156.6f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left5 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -151.6f, -25.874142f, 3.054228f, true, false, 0, 0);

					WayPoint * wp = Right1->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -174.216004f;
					wp->z = -25.792299f;
					wp->o = 4.747290f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left1->GetAIInterface()->addWayPoint(wp);
					Left2->GetAIInterface()->addWayPoint(wp);
					Left3->GetAIInterface()->addWayPoint(wp);
					Left4->GetAIInterface()->addWayPoint(wp);
					Left5->GetAIInterface()->addWayPoint(wp);
					Right1->GetAIInterface()->addWayPoint(wp);
					Right2->GetAIInterface()->addWayPoint(wp);
					Right3->GetAIInterface()->addWayPoint(wp);
					Right4->GetAIInterface()->addWayPoint(wp);
					Right5->GetAIInterface()->addWayPoint(wp);

					Left1->GetAIInterface()->StopMovement(0);
					Left2->GetAIInterface()->StopMovement(0);
					Left3->GetAIInterface()->StopMovement(0);
					Left4->GetAIInterface()->StopMovement(0);
					Left5->GetAIInterface()->StopMovement(0);
					Right1->GetAIInterface()->StopMovement(0);
					Right2->GetAIInterface()->StopMovement(0);
					Right3->GetAIInterface()->StopMovement(0);
					Right4->GetAIInterface()->StopMovement(0);
					Right5->GetAIInterface()->StopMovement(0);

					Left1->GetAIInterface()->setWaypointToMove(0);
					Left2->GetAIInterface()->setWaypointToMove(0);
					Left3->GetAIInterface()->setWaypointToMove(0);
					Left4->GetAIInterface()->setWaypointToMove(0);
					Left5->GetAIInterface()->setWaypointToMove(0);
					Right1->GetAIInterface()->setWaypointToMove(0);
					Right2->GetAIInterface()->setWaypointToMove(0);
					Right3->GetAIInterface()->setWaypointToMove(0);
					Right4->GetAIInterface()->setWaypointToMove(0);
					Right5->GetAIInterface()->setWaypointToMove(0);

					Left1->Despawn(600000, 0);
					Left2->Despawn(600000, 0);
					Left3->Despawn(600000, 0);
					Left4->Despawn(600000, 0);
					Left5->Despawn(600000, 0);
					Right1->Despawn(600000, 0);
					Right2->Despawn(600000, 0);
					Right3->Despawn(600000, 0);
					Right4->Despawn(600000, 0);
					Right5->Despawn(600000, 0);

					used = true;
				} break;
			}
		}
	}

protected:
	bool used;
};

class CandleRightBack : public GameObjectAIScript
{
public:
	CandleRightBack(GameObject* goinstance) : GameObjectAIScript(goinstance) {
	used = false;
	}
	static GameObjectAIScript *Create(GameObject* GO) { return new CandleRightBack(GO); }

	void OnActivate(Player* pPlayer)
	{
		int counter = 0;
		GameObject* CandleRightFront = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-823.88f, -158.535f, -24.5278f, 21121);
		GameObject* CandleLeftBack = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-823.955f, -170.407f, -24.5267f, 21119);
		GameObject* CandleLeftFront = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-823.88f, -158.535f, -24.5278f, 21118);

		if(CandleLeftFront->GetUInt32Value(GAMEOBJECT_FLAGS) == GO_FLAG_UNCLICKABLE)
			counter++;
		if(CandleLeftBack->GetUInt32Value(GAMEOBJECT_FLAGS) == GO_FLAG_UNCLICKABLE)
			counter++;
		if(CandleRightFront->GetUInt32Value(GAMEOBJECT_FLAGS) == GO_FLAG_UNCLICKABLE)
			counter++;
	    if(used == true)
		{
			_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		}
		
		if(used == false)
		{
			switch(counter)
			{
			case 0:
				{
					Creature* Right = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4825, -867.804321f, -153.597672f, -25.880451f, 6.270435f, true, false, 0, 0);			
					Creature* Left1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4825, -768.702393f, -164.399490f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4825, -768.711121f, -174.439697f, -25.871897f, 3.203451f, true, false, 0, 0);

					WayPoint * wp = Right->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -155.576004f;
					wp->z = -25.792299f;
					wp->o = 4.747290f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left1->GetAIInterface()->addWayPoint(wp);
					Left2->GetAIInterface()->addWayPoint(wp);
					Right->GetAIInterface()->addWayPoint(wp);

					Left1->GetAIInterface()->StopMovement(0);
					Left2->GetAIInterface()->StopMovement(0);
					Right->GetAIInterface()->StopMovement(0);

					Left1->GetAIInterface()->setWaypointToMove(0);
					Left2->GetAIInterface()->setWaypointToMove(0);
					Right->GetAIInterface()->setWaypointToMove(0);

					Left1->Despawn(600000, 0);
					Left2->Despawn(600000, 0);
					Right->Despawn(600000, 0);

					used = true;
				} break;
			case 1:
				{
					Creature* Right1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -868.096924f, -164.041473f, -25.873016f, 6.227231f, true, false, 0, 0);
					Creature* Right2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -868.096924f, -174.364594f, -25.871803f, 0.061856f, true, false, 0, 0);
					Creature* Left1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -768.711121f, -174.439697f, -25.871897f, 3.203451f, true, false, 0, 0);
					Creature* Left2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -768.759888f, -154.085938f, -25.880392f, 3.164181f, true, false, 0, 0);

					WayPoint * wp = Right1->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -155.576004f;
					wp->z = -25.792299f;
					wp->o = 4.747290f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left1->GetAIInterface()->addWayPoint(wp);
					Left2->GetAIInterface()->addWayPoint(wp);
					Right1->GetAIInterface()->addWayPoint(wp);
					Right2->GetAIInterface()->addWayPoint(wp);

					Left1->GetAIInterface()->StopMovement(0);
					Left2->GetAIInterface()->StopMovement(0);
					Right1->GetAIInterface()->StopMovement(0);
					Right2->GetAIInterface()->StopMovement(0);

					Left1->GetAIInterface()->setWaypointToMove(0);
					Left2->GetAIInterface()->setWaypointToMove(0);
					Right1->GetAIInterface()->setWaypointToMove(0);
					Right2->GetAIInterface()->setWaypointToMove(0);

					Left1->Despawn(600000, 0);
					Left2->Despawn(600000, 0);
					Right1->Despawn(600000, 0);
					Right2->Despawn(600000, 0);

					used = true;
				} break;
			case 2:
				{
					Creature* Right = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4978, -868.096924f, -164.041473f, -25.873016f, 6.227231f, true, false, 0, 0);
					Creature* Left = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4978, -768.702393f, -164.399490f, -25.874142f, 3.054228f, true, false, 0, 0);

					WayPoint * wp = Right->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -174.216004f;
					wp->z = -25.792299f;
					wp->o = 1.579253f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left->GetAIInterface()->addWayPoint(wp);
					Right->GetAIInterface()->addWayPoint(wp);

					Left->GetAIInterface()->StopMovement(0);
					Right->GetAIInterface()->StopMovement(0);

					Left->GetAIInterface()->setWaypointToMove(0);
					Right->GetAIInterface()->setWaypointToMove(0);

					Left->Despawn(600000, 0);
					Right->Despawn(600000, 0);

					used = true;
				} break;
			case 3:
				{
					GameObject* Door = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-818.361f, -200.647f, -25.7911f, 21117);
					Door->SetUInt32Value(GAMEOBJECT_STATE, 0);
					Door->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

					Creature* Right1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -174.364594f, -25.871803f, 0.061856f, true, false, 0, 0);
					Creature* Right2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -151.5f, -25.874142f, 0.061856f, true, false, 0, 0);
					Creature* Right3 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -156.5f, -25.874142f, 0.061856f, true, false, 0, 0);
					Creature* Right4 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -161.5f, -25.874142f, 0.061856f, true, false, 0, 0);
					Creature* Right5 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -166.5f, -25.874142f, 0.061856f, true, false, 0, 0);

					Creature* Left1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -174.399490f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -166.6f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left3 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -161.6f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left4 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -156.6f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left5 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -151.6f, -25.874142f, 3.054228f, true, false, 0, 0);

					WayPoint * wp = Right1->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -174.216004f;
					wp->z = -25.792299f;
					wp->o = 4.747290f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left1->GetAIInterface()->addWayPoint(wp);
					Left2->GetAIInterface()->addWayPoint(wp);
					Left3->GetAIInterface()->addWayPoint(wp);
					Left4->GetAIInterface()->addWayPoint(wp);
					Left5->GetAIInterface()->addWayPoint(wp);
					Right1->GetAIInterface()->addWayPoint(wp);
					Right2->GetAIInterface()->addWayPoint(wp);
					Right3->GetAIInterface()->addWayPoint(wp);
					Right4->GetAIInterface()->addWayPoint(wp);
					Right5->GetAIInterface()->addWayPoint(wp);

					Left1->GetAIInterface()->StopMovement(0);
					Left2->GetAIInterface()->StopMovement(0);
					Left3->GetAIInterface()->StopMovement(0);
					Left4->GetAIInterface()->StopMovement(0);
					Left5->GetAIInterface()->StopMovement(0);
					Right1->GetAIInterface()->StopMovement(0);
					Right2->GetAIInterface()->StopMovement(0);
					Right3->GetAIInterface()->StopMovement(0);
					Right4->GetAIInterface()->StopMovement(0);
					Right5->GetAIInterface()->StopMovement(0);

					Left1->GetAIInterface()->setWaypointToMove(0);
					Left2->GetAIInterface()->setWaypointToMove(0);
					Left3->GetAIInterface()->setWaypointToMove(0);
					Left4->GetAIInterface()->setWaypointToMove(0);
					Left5->GetAIInterface()->setWaypointToMove(0);
					Right1->GetAIInterface()->setWaypointToMove(0);
					Right2->GetAIInterface()->setWaypointToMove(0);
					Right3->GetAIInterface()->setWaypointToMove(0);
					Right4->GetAIInterface()->setWaypointToMove(0);
					Right5->GetAIInterface()->setWaypointToMove(0);

					Left1->Despawn(600000, 0);
					Left2->Despawn(600000, 0);
					Left3->Despawn(600000, 0);
					Left4->Despawn(600000, 0);
					Left5->Despawn(600000, 0);
					Right1->Despawn(600000, 0);
					Right2->Despawn(600000, 0);
					Right3->Despawn(600000, 0);
					Right4->Despawn(600000, 0);
					Right5->Despawn(600000, 0);

					used = true;
				} break;
			}
		}
	}

protected:
	bool used;
};

class CandleLeftBack : public GameObjectAIScript
{
public:
	CandleLeftBack(GameObject* goinstance) : GameObjectAIScript(goinstance) {
	used = false;
	}
	static GameObjectAIScript *Create(GameObject* GO) { return new CandleLeftBack(GO); }

	void OnActivate(Player* pPlayer)
	{
		int counter = 0;
		GameObject* CandleRightFront = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-823.88f, -158.535f, -24.5278f, 21121);
		GameObject* CandleRightBack = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-813.578f, -170.461f, -24.5276f, 21120);
		GameObject* CandleLeftFront = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-823.88f, -158.535f, -24.5278f, 21118);

		if(CandleLeftFront->GetUInt32Value(GAMEOBJECT_FLAGS) == GO_FLAG_UNCLICKABLE)
			counter++;
		if(CandleRightBack->GetUInt32Value(GAMEOBJECT_FLAGS) == GO_FLAG_UNCLICKABLE)
			counter++;
		if(CandleRightBack->GetUInt32Value(GAMEOBJECT_FLAGS) == GO_FLAG_UNCLICKABLE)
			counter++;
		if(used == true)
		{
			_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		}
		if(used == false)
		{
			switch(counter)
			{
			case 0:
				{
					Creature* Right = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4825, -867.804321f, -153.597672f, -25.880451f, 6.270435f, true, false, 0, 0);			
					Creature* Left1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4825, -768.702393f, -164.399490f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4825, -768.711121f, -174.439697f, -25.871897f, 3.203451f, true, false, 0, 0);

					WayPoint * wp = Right->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -155.576004f;
					wp->z = -25.792299f;
					wp->o = 4.747290f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left1->GetAIInterface()->addWayPoint(wp);
					Left2->GetAIInterface()->addWayPoint(wp);
					Right->GetAIInterface()->addWayPoint(wp);

					Left1->GetAIInterface()->StopMovement(0);
					Left2->GetAIInterface()->StopMovement(0);
					Right->GetAIInterface()->StopMovement(0);

					Left1->GetAIInterface()->setWaypointToMove(0);
					Left2->GetAIInterface()->setWaypointToMove(0);
					Right->GetAIInterface()->setWaypointToMove(0);

					Left1->Despawn(600000, 0);
					Left2->Despawn(600000, 0);
					Right->Despawn(600000, 0);

					used = true;
				} break;
			case 1:
				{
					Creature* Right1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -868.096924f, -164.041473f, -25.873016f, 6.227231f, true, false, 0, 0);
					Creature* Right2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -868.096924f, -174.364594f, -25.871803f, 0.061856f, true, false, 0, 0);
					Creature* Left1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -768.711121f, -174.439697f, -25.871897f, 3.203451f, true, false, 0, 0);
					Creature* Left2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -768.759888f, -154.085938f, -25.880392f, 3.164181f, true, false, 0, 0);

					WayPoint * wp = Right1->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -155.576004f;
					wp->z = -25.792299f;
					wp->o = 4.747290f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left1->GetAIInterface()->addWayPoint(wp);
					Left2->GetAIInterface()->addWayPoint(wp);
					Right1->GetAIInterface()->addWayPoint(wp);
					Right2->GetAIInterface()->addWayPoint(wp);

					Left1->GetAIInterface()->StopMovement(0);
					Left2->GetAIInterface()->StopMovement(0);
					Right1->GetAIInterface()->StopMovement(0);
					Right2->GetAIInterface()->StopMovement(0);

					Left1->GetAIInterface()->setWaypointToMove(0);
					Left2->GetAIInterface()->setWaypointToMove(0);
					Right1->GetAIInterface()->setWaypointToMove(0);
					Right2->GetAIInterface()->setWaypointToMove(0);

					Left1->Despawn(600000, 0);
					Left2->Despawn(600000, 0);
					Right1->Despawn(600000, 0);
					Right2->Despawn(600000, 0);

					used = true;
				} break;
			case 2:
				{
					Creature* Right = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4978, -868.096924f, -164.041473f, -25.873016f, 6.227231f, true, false, 0, 0);
					Creature* Left = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4978, -768.702393f, -164.399490f, -25.874142f, 3.054228f, true, false, 0, 0);

					WayPoint * wp = Right->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -174.216004f;
					wp->z = -25.792299f;
					wp->o = 1.579253f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left->GetAIInterface()->addWayPoint(wp);
					Right->GetAIInterface()->addWayPoint(wp);

					Left->GetAIInterface()->StopMovement(0);
					Right->GetAIInterface()->StopMovement(0);

					Left->GetAIInterface()->setWaypointToMove(0);
					Right->GetAIInterface()->setWaypointToMove(0);

					Left->Despawn(600000, 0);
					Right->Despawn(600000, 0);

					used = true;
				} break;
			case 3:
				{
					GameObject* Door = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-818.361f, -200.647f, -25.7911f, 21117);
					Door->SetUInt32Value(GAMEOBJECT_STATE, 0);
					Door->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

					Creature* Right1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -174.364594f, -25.871803f, 0.061856f, true, false, 0, 0);
					Creature* Right2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -151.5f, -25.874142f, 0.061856f, true, false, 0, 0);
					Creature* Right3 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -156.5f, -25.874142f, 0.061856f, true, false, 0, 0);
					Creature* Right4 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -161.5f, -25.874142f, 0.061856f, true, false, 0, 0);
					Creature* Right5 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -166.5f, -25.874142f, 0.061856f, true, false, 0, 0);

					Creature* Left1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -174.399490f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -166.6f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left3 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -161.6f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left4 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -156.6f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left5 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -151.6f, -25.874142f, 3.054228f, true, false, 0, 0);

					WayPoint * wp = Right1->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -174.216004f;
					wp->z = -25.792299f;
					wp->o = 4.747290f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left1->GetAIInterface()->addWayPoint(wp);
					Left2->GetAIInterface()->addWayPoint(wp);
					Left3->GetAIInterface()->addWayPoint(wp);
					Left4->GetAIInterface()->addWayPoint(wp);
					Left5->GetAIInterface()->addWayPoint(wp);
					Right1->GetAIInterface()->addWayPoint(wp);
					Right2->GetAIInterface()->addWayPoint(wp);
					Right3->GetAIInterface()->addWayPoint(wp);
					Right4->GetAIInterface()->addWayPoint(wp);
					Right5->GetAIInterface()->addWayPoint(wp);

					Left1->GetAIInterface()->StopMovement(0);
					Left2->GetAIInterface()->StopMovement(0);
					Left3->GetAIInterface()->StopMovement(0);
					Left4->GetAIInterface()->StopMovement(0);
					Left5->GetAIInterface()->StopMovement(0);
					Right1->GetAIInterface()->StopMovement(0);
					Right2->GetAIInterface()->StopMovement(0);
					Right3->GetAIInterface()->StopMovement(0);
					Right4->GetAIInterface()->StopMovement(0);
					Right5->GetAIInterface()->StopMovement(0);

					Left1->GetAIInterface()->setWaypointToMove(0);
					Left2->GetAIInterface()->setWaypointToMove(0);
					Left3->GetAIInterface()->setWaypointToMove(0);
					Left4->GetAIInterface()->setWaypointToMove(0);
					Left5->GetAIInterface()->setWaypointToMove(0);
					Right1->GetAIInterface()->setWaypointToMove(0);
					Right2->GetAIInterface()->setWaypointToMove(0);
					Right3->GetAIInterface()->setWaypointToMove(0);
					Right4->GetAIInterface()->setWaypointToMove(0);
					Right5->GetAIInterface()->setWaypointToMove(0);

					Left1->Despawn(600000, 0);
					Left2->Despawn(600000, 0);
					Left3->Despawn(600000, 0);
					Left4->Despawn(600000, 0);
					Left5->Despawn(600000, 0);
					Right1->Despawn(600000, 0);
					Right2->Despawn(600000, 0);
					Right3->Despawn(600000, 0);
					Right4->Despawn(600000, 0);
					Right5->Despawn(600000, 0);

					used = true;
				} break;
			}
		}
	}

protected:
	bool used;
};

class CandleLeftFront : public GameObjectAIScript
{
public:
	CandleLeftFront(GameObject* goinstance) : GameObjectAIScript(goinstance) {
	used = false;
	}
	static GameObjectAIScript *Create(GameObject* GO) { return new CandleLeftFront(GO); }

	void OnActivate(Player* pPlayer)
	{
		int counter = 0;
		GameObject* CandleRightFront = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-823.88f, -158.535f, -24.5278f, 21121);
		GameObject* CandleRightBack = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-813.578f, -170.461f, -24.5276f, 21120);
		GameObject* CandleLeftBack = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-823.955f, -170.407f, -24.5267f, 21119);

		if(CandleRightFront->GetUInt32Value(GAMEOBJECT_FLAGS) == GO_FLAG_UNCLICKABLE)
			counter++;
		if(CandleLeftBack->GetUInt32Value(GAMEOBJECT_FLAGS) == GO_FLAG_UNCLICKABLE)
			counter++;
		if(CandleRightBack->GetUInt32Value(GAMEOBJECT_FLAGS) == GO_FLAG_UNCLICKABLE)
			counter++;
		if(used == true)
		{
			_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);
		}
		if(used == false)
		{
			switch(counter)
			{
			case 0:
				{
					Creature* Right = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4825, -867.804321f, -153.597672f, -25.880451f, 6.270435f, true, false, 0, 0);			
					Creature* Left1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4825, -768.702393f, -164.399490f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4825, -768.711121f, -174.439697f, -25.871897f, 3.203451f, true, false, 0, 0);

					_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

					WayPoint * wp = Right->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -155.576004f;
					wp->z = -25.792299f;
					wp->o = 4.747290f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left1->GetAIInterface()->addWayPoint(wp);
					Left2->GetAIInterface()->addWayPoint(wp);
					Right->GetAIInterface()->addWayPoint(wp);

					Left1->GetAIInterface()->StopMovement(0);
					Left2->GetAIInterface()->StopMovement(0);
					Right->GetAIInterface()->StopMovement(0);

					Left1->GetAIInterface()->setWaypointToMove(0);
					Left2->GetAIInterface()->setWaypointToMove(0);
					Right->GetAIInterface()->setWaypointToMove(0);

					Left1->Despawn(600000, 0);
					Left2->Despawn(600000, 0);
					Right->Despawn(600000, 0);

					used = true;
				} break;
			case 1:
				{
					Creature* Right1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -868.096924f, -164.041473f, -25.873016f, 6.227231f, true, false, 0, 0);
					Creature* Right2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -868.096924f, -174.364594f, -25.871803f, 0.061856f, true, false, 0, 0);
					Creature* Left1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -768.711121f, -174.439697f, -25.871897f, 3.203451f, true, false, 0, 0);
					Creature* Left2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4815, -768.759888f, -154.085938f, -25.880392f, 3.164181f, true, false, 0, 0);

					_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

					WayPoint * wp = Right1->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -155.576004f;
					wp->z = -25.792299f;
					wp->o = 4.747290f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left1->GetAIInterface()->addWayPoint(wp);
					Left2->GetAIInterface()->addWayPoint(wp);
					Right1->GetAIInterface()->addWayPoint(wp);
					Right2->GetAIInterface()->addWayPoint(wp);

					Left1->GetAIInterface()->StopMovement(0);
					Left2->GetAIInterface()->StopMovement(0);
					Right1->GetAIInterface()->StopMovement(0);
					Right2->GetAIInterface()->StopMovement(0);

					Left1->GetAIInterface()->setWaypointToMove(0);
					Left2->GetAIInterface()->setWaypointToMove(0);
					Right1->GetAIInterface()->setWaypointToMove(0);
					Right2->GetAIInterface()->setWaypointToMove(0);

					Left1->Despawn(600000, 0);
					Left2->Despawn(600000, 0);
					Right1->Despawn(600000, 0);
					Right2->Despawn(600000, 0);

					used = true;
				} break;
			case 2:
				{
					Creature* Right = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4978, -868.096924f, -164.041473f, -25.873016f, 6.227231f, true, false, 0, 0);
					Creature* Left = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4978, -768.702393f, -164.399490f, -25.874142f, 3.054228f, true, false, 0, 0);

					_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

					WayPoint * wp = Right->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -174.216004f;
					wp->z = -25.792299f;
					wp->o = 1.579253f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left->GetAIInterface()->addWayPoint(wp);
					Right->GetAIInterface()->addWayPoint(wp);

					Left->GetAIInterface()->StopMovement(0);
					Right->GetAIInterface()->StopMovement(0);

					Left->GetAIInterface()->setWaypointToMove(0);
					Right->GetAIInterface()->setWaypointToMove(0);

					Left->Despawn(600000, 0);
					Right->Despawn(600000, 0);

					used = true;
				} break;
			case 3:
				{
					GameObject* Door = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-818.361f, -200.647f, -25.7911f, 21117);
					Door->SetUInt32Value(GAMEOBJECT_STATE, 0);
					Door->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

					_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNCLICKABLE);

					Creature* Right1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -174.364594f, -25.871803f, 0.061856f, true, false, 0, 0);
					Creature* Right2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -151.5f, -25.874142f, 0.061856f, true, false, 0, 0);
					Creature* Right3 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -156.5f, -25.874142f, 0.061856f, true, false, 0, 0);
					Creature* Right4 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -161.5f, -25.874142f, 0.061856f, true, false, 0, 0);
					Creature* Right5 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -868.096924f, -166.5f, -25.874142f, 0.061856f, true, false, 0, 0);

					Creature* Left1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -174.399490f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left2 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -166.6f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left3 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -161.6f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left4 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -156.6f, -25.874142f, 3.054228f, true, false, 0, 0);
					Creature* Left5 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(4977, -768.702393f, -151.6f, -25.874142f, 3.054228f, true, false, 0, 0);

					WayPoint * wp = Right1->CreateWaypointStruct();
					wp->id = 1;
					wp->x = -818.831970f;
					wp->y = -174.216004f;
					wp->z = -25.792299f;
					wp->o = 4.747290f;
					wp->waittime = 0;
					wp->flags = WALK;
					wp->forwardemoteoneshot = 0;
					wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = 0;
					wp->backwardemoteid = 0;
					wp->forwardskinid = 0;
					wp->backwardskinid = 0;

					Left1->GetAIInterface()->addWayPoint(wp);
					Left2->GetAIInterface()->addWayPoint(wp);
					Left3->GetAIInterface()->addWayPoint(wp);
					Left4->GetAIInterface()->addWayPoint(wp);
					Left5->GetAIInterface()->addWayPoint(wp);
					Right1->GetAIInterface()->addWayPoint(wp);
					Right2->GetAIInterface()->addWayPoint(wp);
					Right3->GetAIInterface()->addWayPoint(wp);
					Right4->GetAIInterface()->addWayPoint(wp);
					Right5->GetAIInterface()->addWayPoint(wp);

					Left1->GetAIInterface()->StopMovement(0);
					Left2->GetAIInterface()->StopMovement(0);
					Left3->GetAIInterface()->StopMovement(0);
					Left4->GetAIInterface()->StopMovement(0);
					Left5->GetAIInterface()->StopMovement(0);
					Right1->GetAIInterface()->StopMovement(0);
					Right2->GetAIInterface()->StopMovement(0);
					Right3->GetAIInterface()->StopMovement(0);
					Right4->GetAIInterface()->StopMovement(0);
					Right5->GetAIInterface()->StopMovement(0);

					Left1->GetAIInterface()->setWaypointToMove(0);
					Left2->GetAIInterface()->setWaypointToMove(0);
					Left3->GetAIInterface()->setWaypointToMove(0);
					Left4->GetAIInterface()->setWaypointToMove(0);
					Left5->GetAIInterface()->setWaypointToMove(0);
					Right1->GetAIInterface()->setWaypointToMove(0);
					Right2->GetAIInterface()->setWaypointToMove(0);
					Right3->GetAIInterface()->setWaypointToMove(0);
					Right4->GetAIInterface()->setWaypointToMove(0);
					Right5->GetAIInterface()->setWaypointToMove(0);

					Left1->Despawn(600000, 0);
					Left2->Despawn(600000, 0);
					Left3->Despawn(600000, 0);
					Left4->Despawn(600000, 0);
					Left5->Despawn(600000, 0);
					Right1->Despawn(600000, 0);
					Right2->Despawn(600000, 0);
					Right3->Despawn(600000, 0);
					Right4->Despawn(600000, 0);
					Right5->Despawn(600000, 0);

					used = true;
				} break;
			}
		}
	}

protected:
	bool used;
};

#define OBJ_FathomStone 177964
class FathomStone : public GameObjectAIScript
{
public:
	FathomStone(GameObject*  goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject*  GO) { return new FathomStone(GO); }

	void OnActivate(Player*  pPlayer)
	{
		if(pPlayer->GetTeam() == 1 && BaronAquanis == false) // Horde
		{
			// Spawn Baron Aquanis
			_gameobject->GetMapMgr()->GetInterface()->SpawnCreature(CN_BaronAquanis, -782.021f, -63.5876f, -45.0935f, -2.44346f, true, false, 0, 0);
			BaronAquanis = true;

		}
	}

protected:
	bool BaronAquanis;
};

#define CN_TwilightLordKelris 4832
class KelrisAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KelrisAI, MoonScriptCreatureAI);
    KelrisAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddEmote(Event_OnCombatStart, "Who dares disturb my meditation?!", Text_Yell, 5802);
		AddEmote(Event_OnTargetDied, "Dust to dust.", Text_Yell, 5803);
		_unit->setEmoteState(EMOTE_STATE_SPELLPRECAST);

		// Sleep
		AddSpell(8399, Target_RandomPlayer, 12, 1.3f, 8, 0, 0, false, "Sleep...", Text_Yell, 5804);
		// Mind Blast
		AddSpell(15587, Target_Current, 16, 1.5f, 0);
	}
};

#define CN_Akumai 4829
class AkumaiAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AkumaiAI, MoonScriptCreatureAI);
    AkumaiAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		// Frenzied Rage
		AddSpell(3490, Target_Self, 12, 0, 0, 0, 0);
		// Poison Cloud
		AddSpell(3815, Target_Self, 16, 0, 45);
	}
	void OnDied(Unit* pKiller)
	{
		GameObject* Altar = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-839.619f, -477.904f, -33.7343f, 103016);
		Altar->SetFlag(GAMEOBJECT_FLAGS, 0);
		Player* Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(pKiller->GetPositionX(),pKiller->GetPositionY(),pKiller->GetPositionZ());

		if(Plr->GetTeam() == 1 && Morridune == false)
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(6729, -847.346863f, -474.907684f, -34.085144f, 1.675044f, true, false, 0, 0);
			Morridune = true;
		}
		ParentClass::OnDied(pKiller);
	}
protected:
	bool Morridune;
};
#define GS_Morridune 6729
class MorriduneGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player*  plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7247, plr);
        
		if(plr->GetTeam() == 0)
			Menu->AddItem( 0, "Please Teleport me to Darnassus.", 1);
		Menu->AddItem( 0, "I wish to leave this horrible place", 2);

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player*  plr, uint32 Id, uint32 IntId, const char * Code)
    {
		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
			break;
			case 1:
				// port to Darnassus
				plr->SafeTeleport(1, 0, 9951.52f, 2280.32f, 1341.39f, 0);
			break;
			case 2:
				// Port to entrance
				plr->SafeTeleport(1, 0, 4247.74f, 745.879f, -24.2967f, 4.36996f);
			break;
		}
	}

    void Destroy()
    {
        delete this;
    }

};

#define AKUMAI_SERVANT 4978
class AkumaiServant : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AkumaiServant, MoonScriptCreatureAI);
    AkumaiServant(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(865, Target_Current, 12, 0, 25);
		AddSpell(8398, Target_Current, 16, 2, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define AKUMAI_SNAPJAW 4825
class AkumaiSnapjaw : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AkumaiSnapjaw, MoonScriptCreatureAI);
    AkumaiSnapjaw(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(8391, Target_Current, 12, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define AQUA_GUARDIAN 6047
class AquaGuardian : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AquaGuardian, MoonScriptCreatureAI);
    AquaGuardian(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(865, Target_Current, 12, 0, 25);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define BLACKFATHOM_MYRMIDON 4807
class BlackfathomMyrmidon : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BlackfathomMyrmidon, MoonScriptCreatureAI);
    BlackfathomMyrmidon(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(8379, Target_Current, 12, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define BLACKFATHOM_SEA_WITCH 4805
class BlackfathomSeaWitch : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BlackfathomSeaWitch, MoonScriptCreatureAI);
    BlackfathomSeaWitch(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		FrostArmor = AddSpell(12544, Target_Self, 0, 0, 0);
		AddSpell(122, Target_Current, 12, 0, 25);
		AddSpell(6143, Target_Self, 12, 0, 30);
	}
	void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(FrostArmor);
		ParentClass::OnCombatStart(pTarget);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
	SpellDesc* FrostArmor;
};

#define BLINDLIGHT_MUCKDWELLER 4819
class MuckDweller : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MuckDweller, MoonScriptCreatureAI);
    MuckDweller(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(8382, Target_Current, 12, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define BLINDLIGHT_MURLOC 4818
class Murloc : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Murloc, MoonScriptCreatureAI);
    Murloc(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(11971, Target_Current, 12, 0, 0);
		Defensive = AddSpell(8382, Target_Self, 0, 0, 180);
	}
	void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(Defensive);
		ParentClass::OnCombatStart(pTarget);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
	SpellDesc* Defensive;
};

#define BLINDLIGHT_ORACLE 4820
class Oracle : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Oracle, MoonScriptCreatureAI);
    Oracle(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(6063, Target_RandomFriendly, 12, 0, 10);
		AddSpell(9532, Target_Current, 12, 3, 0);
		AddSpell(8362, Target_Self, 8, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define DEEP_POOL_THRESHFIN 4827
class DeepPoolThreshfin : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DeepPoolThreshfin, MoonScriptCreatureAI);
    DeepPoolThreshfin(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(3604, Target_Current, 12, 0, 20);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define FALLENROOTHELLCALLER 4799
class HellCaller : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HellCaller, MoonScriptCreatureAI);
    HellCaller(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(8129, Target_RandomPlayer, 10, 3, 0);
		AddSpell(9613, Target_RandomPlayer, 10, 3, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define FALLENROOTSHADOWSTALKER 4798
class ShadowStalker : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowStalker, MoonScriptCreatureAI);
    ShadowStalker(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(8129, Target_RandomPlayer, 10, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define SNAPCLAW 4815
class SnapClaw : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SnapClaw, MoonScriptCreatureAI);
    SnapClaw(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(8379, Target_Current, 10, 0, 10);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define TWILIGHT_ALCOLYTE 4809
class Alcolyte : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Alcolyte, MoonScriptCreatureAI);
    Alcolyte(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(8362, Target_RandomFriendly, 10, 2, 0);
		AddSpell(2055, Target_RandomFriendly, 10, 3, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define TWILIGHT_AQUAMANCER 4811
class Aquamancer : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Aquamancer, MoonScriptCreatureAI);
    Aquamancer(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(9672, Target_Current, 15, 3, 0);
		AddSpell(8372, Target_Self, 8, 0, 20);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};
#define TWILIGHT_ELEMENTALIST 4814
class Elementalist : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Elementalist, MoonScriptCreatureAI);
    Elementalist(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(13728, Target_Current, 10, 0, 6);
		AddSpell(15039, Target_Current, 8, 0, 12);
		AddSpell(12548, Target_Current, 10, 0, 0);
		AddSpell(11824, Target_Current, 8, 0, 6);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define TWILIGHT_LORESEEKER 4812
class LoreSeeker : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LoreSeeker, MoonScriptCreatureAI);
    LoreSeeker(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(18972, Target_Current, 15, 0, 10);
		AddSpell(8365, Target_Self, 8, 0, 20);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define TWILIGHT_REAVER 4810
class Reaver : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Reaver, MoonScriptCreatureAI);
    Reaver(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(8374, Target_Current, 10, 0, 0);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

#define TWILIGHT_SHADOWMAGE 4812
class ShadowMage : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowMage, MoonScriptCreatureAI);
    ShadowMage(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{	
		AddSpell(9613, Target_Current, 15, 3, 0);
		AddSpell(12746, Target_Self, 8, 0, 30);
		AddSpell(7645, Target_RandomPlayer, 10, 2, 15);
	}
	void OnCombatStop(Unit *pTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		ParentClass::OnCombatStop(pTarget);
    }
};

void SetupBlackfathomDeeps(ScriptMgr * mgr)
{
	//Bosses
	mgr->register_creature_script(CN_Ghamoora, &GhamooraAI::Create);
	mgr->register_creature_script(CN_LadySarevess, &LadySarevessAI::Create);
	mgr->register_creature_script(CN_Gelihast, &GelihastAI::Create);
	mgr->register_creature_script(CN_LorgusJett, &LorgusJettAI::Create);
	mgr->register_creature_script(CN_BaronAquanis, &BaronAquanisAI::Create);
	mgr->register_creature_script(CN_TwilightLordKelris, &KelrisAI::Create);
	mgr->register_creature_script(CN_Akumai, &AkumaiAI::Create);

	//GO's
	mgr->register_gameobject_script(21117, &Door::Create);
	mgr->register_gameobject_script(OBJ_FathomStone, &FathomStone::Create);
	mgr->register_gameobject_script(21121, &CandleRightFront::Create);
	mgr->register_gameobject_script(21120, &CandleRightBack::Create);
	mgr->register_gameobject_script(21119, &CandleLeftBack::Create);
	mgr->register_gameobject_script(21118, &CandleLeftFront::Create);

	//Gossip
	GossipScript * Morridune = (GossipScript*) new MorriduneGossip();
	mgr->register_gossip_script(GS_Morridune, Morridune);

	//Trash
	mgr->register_creature_script(TWILIGHT_LORESEEKER, &LoreSeeker::Create);
	mgr->register_creature_script(TWILIGHT_REAVER, &Reaver::Create);
	mgr->register_creature_script(TWILIGHT_SHADOWMAGE, &ShadowMage::Create);
	mgr->register_creature_script(TWILIGHT_ELEMENTALIST, &Elementalist::Create);
	mgr->register_creature_script(TWILIGHT_AQUAMANCER, &Aquamancer::Create);
	mgr->register_creature_script(TWILIGHT_ALCOLYTE, &Alcolyte::Create);
	mgr->register_creature_script(SNAPCLAW, &SnapClaw::Create);
	mgr->register_creature_script(FALLENROOTSHADOWSTALKER, &ShadowStalker::Create);
	mgr->register_creature_script(FALLENROOTHELLCALLER, &HellCaller::Create);
	mgr->register_creature_script(DEEP_POOL_THRESHFIN, &DeepPoolThreshfin::Create);
	mgr->register_creature_script(BLINDLIGHT_ORACLE, &Oracle::Create);
	mgr->register_creature_script(BLINDLIGHT_MURLOC, &Murloc::Create);
	mgr->register_creature_script(BLINDLIGHT_MUCKDWELLER, &MuckDweller::Create);
	mgr->register_creature_script(BLACKFATHOM_SEA_WITCH, &BlackfathomSeaWitch::Create);
	mgr->register_creature_script(BLACKFATHOM_MYRMIDON, &BlackfathomMyrmidon::Create);
	mgr->register_creature_script(BLACKFATHOM_SEA_WITCH, &BlackfathomSeaWitch::Create);
	mgr->register_creature_script(AQUA_GUARDIAN, &AquaGuardian::Create);
	mgr->register_creature_script(AKUMAI_SNAPJAW, &AkumaiSnapjaw::Create);
	mgr->register_creature_script(AKUMAI_SERVANT, &AkumaiServant::Create);
}