/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/EasyFunctions.h"

class OrbOfCommand : public GameObjectAIScript
{
public:
	OrbOfCommand(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new OrbOfCommand(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->HasFinishedQuest(7761) && pPlayer->getLevel() >= 58 && pPlayer->InGroup() == true)
		{
			pPlayer->SafeTeleport(469, 0, -7672.939941f, -1107.307617f, 396.649994f, 0.616532f);
		}
		else if(pPlayer->getLevel() <= 57 || pPlayer->HasFinishedQuest(7761) == false)
		{
			pPlayer->BroadcastMessage("You need to be level 58 and have completed the quest : Blackhand's Command");
		}
		else if(pPlayer->HasFinishedQuest(7761) == true && pPlayer->getLevel() >= 58 && pPlayer->InGroup() == false)
		{
			pPlayer->BroadcastMessage("You need to be in a raid group to be able to enter this instance");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class Blacksmithing_Plans_Use : public GameObjectAIScript
{
public:
	Blacksmithing_Plans_Use(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new Blacksmithing_Plans_Use(GO); }

	void OnLootTaken(Player * pLooter, ItemPrototype *pItemInfo)
	{
		float SSX = pLooter->GetPositionX();
		float SSY = pLooter->GetPositionY();
		float SSZ = pLooter->GetPositionZ();
		float SSO = pLooter->GetOrientation();

		Creature * NewCreature = pLooter->GetMapMgr()->GetInterface()->SpawnCreature(11120, SSX, SSY, SSZ, SSO, true, false, 0, 0);
		if ( NewCreature != nullptr )
			NewCreature->Despawn(600000, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class GongOfBethekk : public GameObjectAIScript
{
public:
	GongOfBethekk(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new GongOfBethekk(GO); }

	void OnActivate(Player * pPlayer)
	{
		Creature * NewCreature = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(14515, -11556.3f, -1628.32f, 41.299f, 4.1f, true, false, 0, 0);
		if ( NewCreature != nullptr )
			NewCreature->Despawn(1200000, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class VilebranchKidnapper : public GameObjectAIScript
{
public:
	VilebranchKidnapper(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new VilebranchKidnapper(GO); }

	void OnActivate(Player * pPlayer)
	{
		float SSX = pPlayer->GetPositionX();
		float SSY = pPlayer->GetPositionY();
		float SSZ = pPlayer->GetPositionZ();
		float SSO = pPlayer->GetOrientation();

		Creature * NewCreature = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(14748, SSX+1, SSY, SSZ, SSO, true, false, 0, 0);
		if ( NewCreature != nullptr )
			NewCreature->Despawn(600000, 0);

		NewCreature = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(14748, SSX, SSY, SSZ, SSO, true, false, 0, 0);
		if ( NewCreature != nullptr )
			NewCreature->Despawn(600000, 0);

		NewCreature = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(14748, SSX-1, SSY, SSZ, SSO, true, false, 0, 0);
		if ( NewCreature != nullptr )
			NewCreature->Despawn(600000, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class GongOfZulFarrak : public GameObjectAIScript
{
public:
	GongOfZulFarrak(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new GongOfZulFarrak(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetItemInterface()->GetItemCount(9240, 1))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();

			Creature * NewCreature = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7273, SSX+1, SSY, SSZ, SSO, true, false, 0, 0);
			if ( NewCreature != nullptr )
				NewCreature->Despawn(1800000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required item : Mallet of Zul'Farrak");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class Corrupt_Minor_Manifestation_Water_Object : public GameObjectAIScript
{
public:
	Corrupt_Minor_Manifestation_Water_Object(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new Corrupt_Minor_Manifestation_Water_Object(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetQuestLogForEntry(63))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();

			Creature * NewCreature = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(5894, SSX, SSY+1, SSZ, SSO, true, false, 0, 0);
			if ( NewCreature != nullptr )
				NewCreature->Despawn(600000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required quest : Call of Water");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class UlagTheCleaver : public GameObjectAIScript
{
public:
	UlagTheCleaver(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new UlagTheCleaver(GO); }

	void OnActivate(Player * pPlayer)
	{

		Creature * Ulag = NULLCREATURE;
		GameObject * pDoor = NULLGOB;
		Ulag = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2390.101807f, 336.676788f, 40.015614f, 6390);
		pDoor = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2388.480029f, 338.3901f, 40.092899f, 176594);
		QuestLogEntry * en = pPlayer->GetQuestLogForEntry(1819);
		if(!en || pDoor==nullptr || Ulag)
		return;

		Creature * NewCreature = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(6390, 2390.101807f, 336.676788f, 40.015614f, 2.259590f, true, false, 0, 0);
		if ( NewCreature != nullptr )
		{
			NewCreature->Despawn(180000, 0);
			NewCreature->GetAIInterface()->SetNextTarget(pPlayer);
			NewCreature->GetAIInterface()->AttackReaction(pPlayer, 1);
			pDoor->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
			pDoor->SetByte(GAMEOBJECT_STATE, 0, 0);
		};
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class DustySpellbooks : public GameObjectAIScript
{
public:
	DustySpellbooks(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new DustySpellbooks(GO); }

	void OnLootTaken(Player * pLooter, ItemPrototype *pItemInfo)
	{
		QuestLogEntry * en = pLooter->GetQuestLogForEntry(422);
		if(!en)
		return;

		float SSX = pLooter->GetPositionX();
		float SSY = pLooter->GetPositionY();
		float SSZ = pLooter->GetPositionZ();
		float SSO = pLooter->GetOrientation();

		Creature * NewCreature = pLooter->GetMapMgr()->GetInterface()->SpawnCreature(1770, SSX, SSY, SSZ, SSO, true, false, 0, 0);
		if ( NewCreature != nullptr )
			NewCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Sons of Arugal will rise against all who challenge the power of the Moonrage!");
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class CatFigurine : public GameObjectAIScript
{
public:
	CatFigurine(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new CatFigurine(GO); }

	void OnActivate(Player * pPlayer)
	{
		uint32 Chance = RandomUInt(100);

		if(Chance <= 10)
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();

			Creature * NewCreature = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(3619, SSX, SSY+1, SSZ, SSO, true, false, 0, 0);
			if ( NewCreature != nullptr )
				NewCreature->Despawn(600000, 0);
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class AlterofTidalMastery : public GameObjectAIScript
{
public:
	AlterofTidalMastery(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new AlterofTidalMastery(GO); }

	void OnActivate(Player * pPlayer)
	{
		pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(16292, 7934.343750f, -7637.020996f, 112.694130f, 3.098388f, true, false, 0, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class APlagueUponThee : public GameObjectAIScript
{
public:
	APlagueUponThee(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new APlagueUponThee(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;

		GameObject * barel = sEAS.SpawnGameobject(pPlayer, 177491, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 1, 0, 0, 0, 0);
		sEAS.GameobjectDelete(barel, 2*60*1000);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class SerpentStatue : public GameObjectAIScript
{
public:
	SerpentStatue(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new SerpentStatue(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(6027);
		if(qle == nullptr)
			  return;

		Creature * naga = sEAS.SpawnCreature(pPlayer, 12369, 246.741, 2953.3, 5.8631, 1.078, 0);
		if ( naga != nullptr )
			naga->Despawn(6*60*1000, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class CuregosGold : public GameObjectAIScript
{
public:
	CuregosGold(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new CuregosGold(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(2882);
		if(qle == nullptr)
			return;

		Creature * pirate = sEAS.SpawnCreature(pPlayer, 7899, pPlayer->GetPositionX()+RandomFloat(5.0f), pPlayer->GetPositionY()+RandomFloat(5.0f), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
		if ( pirate != nullptr )
			pirate->Despawn(6*60*1000, 0);

		pirate = sEAS.SpawnCreature(pPlayer, 7899, pPlayer->GetPositionX()-RandomFloat(5.0f), pPlayer->GetPositionY()+RandomFloat(5.0f), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
		if ( pirate != nullptr )
			pirate->Despawn(6*60*1000, 0);

		pirate = sEAS.SpawnCreature(pPlayer, 7901, pPlayer->GetPositionX()+RandomFloat(5.0f), pPlayer->GetPositionY()-RandomFloat(5.0f), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
		if ( pirate != nullptr )
			pirate->Despawn(6*60*1000, 0);

		pirate = sEAS.SpawnCreature(pPlayer, 7901, pPlayer->GetPositionX()+RandomFloat(5.0f), pPlayer->GetPositionY()+RandomFloat(5.0f), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
		if ( pirate != nullptr )
			pirate->Despawn(6*60*1000, 0);

		pirate = sEAS.SpawnCreature(pPlayer, 7902, pPlayer->GetPositionX()-RandomFloat(5.0f), pPlayer->GetPositionY()-RandomFloat(5.0f), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
		if ( pirate != nullptr )
			pirate->Despawn(6*60*1000, 0);

		GameObject * gobj = sEAS.SpawnGameobject(pPlayer, 142194, pPlayer->GetPositionX()+5, pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 1, 0, 0, 0, 0);
		sEAS.GameobjectDelete(gobj, 10*60*1000);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class DreadmaulRock : public GameObjectAIScript
{
public:
	DreadmaulRock(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new DreadmaulRock(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(3821);
		if(qle == nullptr)
			return;

		Creature * shaghost = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 9136);
		if(shaghost)
			return;

		Creature * shaghostspawn = sEAS.SpawnCreature(pPlayer, 9136, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
		if ( shaghostspawn != nullptr )
			shaghostspawn->Despawn(2*60*1000, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class HandofIruxos : public GameObjectAIScript
{
public:
	HandofIruxos(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new HandofIruxos(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(5381);
		if(qle == nullptr)
			return;

		Creature * demon = sEAS.SpawnCreature(pPlayer, 11876, -348.231, 1763.85, 138.371, 4.42728, 0);
		if ( demon != nullptr )
			demon->Despawn(6*60*1000, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class LegionPortals : public GameObjectAIScript
{
public:
	LegionPortals(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new LegionPortals(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(5581);
		if(qle == nullptr)
			return;

		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
		return;
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class TabletoftheSeven : public GameObjectAIScript
{
public:
	TabletoftheSeven(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new TabletoftheSeven(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(4296);
		if(qle == nullptr)
			return;

		if(pPlayer->GetItemInterface()->GetItemCount(11470, 0) < 1)
			sEAS.AddItem(11470, pPlayer);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class TestofEndurance : public GameObjectAIScript
{
public:
	TestofEndurance(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new TestofEndurance(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(1150);
		if(qle == nullptr)
			return;

		Creature * grenka = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 4490);
		if(grenka)
		{
			if(!grenka->isAlive())
				grenka->Despawn( 5000, 120000 );
			else
				return;
		}

		Creature * grenkaspawn = sEAS.SpawnCreature(pPlayer, 4490, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
		if ( grenkaspawn != nullptr )
			grenkaspawn->Despawn(6*60*1000, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class TheRootofAllEvil : public GameObjectAIScript
{
public:
	TheRootofAllEvil(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new TheRootofAllEvil(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(8481);
		if(qle == nullptr)
			return;

		Creature * xandivious = sEAS.SpawnCreature(pPlayer, 15623, pPlayer->GetPositionX()+5, pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
		if ( xandivious != nullptr )
			xandivious->Despawn(6*60*1000, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

 class SacredFireofLife : public GameObjectAIScript
{
public:
	SacredFireofLife(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new SacredFireofLife(GO); }

	void OnActivate(Player * pPlayer)
	{
		float SSX = pPlayer->GetPositionX();
		float SSY = pPlayer->GetPositionY();
		float SSZ = pPlayer->GetPositionZ();
		float SSO = pPlayer->GetOrientation();

		Creature * NewCreature = pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(10882, SSX, SSY, SSZ, SSO, true, false, 0, 0);
		if ( NewCreature != nullptr )
			NewCreature->Despawn(600000, 0);
	}
};

void SetupGoHandlers(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(179879, &OrbOfCommand::Create);
	mgr->register_gameobject_script(173232, &Blacksmithing_Plans_Use::Create);
	mgr->register_gameobject_script(180526, &GongOfBethekk::Create);
	mgr->register_gameobject_script(179910, &VilebranchKidnapper::Create);
	mgr->register_gameobject_script(141832, &GongOfZulFarrak::Create);
	mgr->register_gameobject_script(113791, &Corrupt_Minor_Manifestation_Water_Object::Create);
	mgr->register_gameobject_script(104593, &UlagTheCleaver::Create);
	mgr->register_gameobject_script(1571, &DustySpellbooks::Create);

	mgr->register_gameobject_script(13873, &CatFigurine::Create);
	mgr->register_gameobject_script(177490, &APlagueUponThee::Create);
	mgr->register_gameobject_script(177673, &SerpentStatue::Create);
	mgr->register_gameobject_script(142189, &CuregosGold::Create);
	mgr->register_gameobject_script(160445, &DreadmaulRock::Create);
	mgr->register_gameobject_script(176581, &HandofIruxos::Create);
	mgr->register_gameobject_script(177400, &LegionPortals::Create);
	mgr->register_gameobject_script(169294, &TabletoftheSeven::Create);
	mgr->register_gameobject_script(20447, &TestofEndurance::Create);
	mgr->register_gameobject_script(180672, &TheRootofAllEvil::Create);
	mgr->register_gameobject_script(175944, &SacredFireofLife::Create);
}
