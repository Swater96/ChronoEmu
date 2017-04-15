/*
 * WEmu MMORPG Server
 * Copyright (C) 2008 WEmu Team
 *
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/EasyFunctions.h"

bool NixxsPledgeOfSecrecy(uint32 i, Spell * pSpell)
{
	if ( pSpell == nullptr || pSpell->u_caster == nullptr || !pSpell->u_caster->IsPlayer() )
		return true;
    
	Player * pPlayer = TO_PLAYER( pSpell->u_caster );
    sEAS.AddItem( 11270, pPlayer );

	return true;
}

bool SummonCyclonian(uint32 i, Spell * pSpell)
{
	if ( pSpell == nullptr || pSpell->u_caster == nullptr )
		return true;
    
	Unit * pUnit = pSpell->u_caster;
	Creature * pCreature = pUnit->GetMapMgr()->GetInterface()->SpawnCreature( 6239, pUnit->GetPositionX(), pUnit->GetPositionY(), pUnit->GetPositionZ(), pUnit->GetOrientation(), true, false, 0, 0 );
	if ( pCreature != nullptr )
	{
		pCreature->Despawn( 600000, 0 );
	}

	return true;
}

bool AwakenPeon(uint32 i, Spell * pSpell)
{
	if ( pSpell == nullptr || pSpell->p_caster == nullptr )
		return true;

	Player * pPlayer = pSpell->p_caster;
	Unit * pUnit = pSpell->GetUnitTarget();
	if ( pUnit == nullptr || !pUnit->IsCreature() )
		return true;

	Creature * pTarget = TO_CREATURE( pUnit );
	if ( !pTarget->HasAura( 17743 ) )
		return true;

	if ( pTarget->GetEntry() == 10556 )
	{
		WorldPacket data(SMSG_PLAY_OBJECT_SOUND, 12);
		data << uint32( 6197 ) << pTarget->GetGUID();
		pPlayer->SendMessageToSet(&data, true);
		pTarget->RemoveAllAuras();
		string Text;
		Text = "Ow!  Ok, I'll get back to work, ";
		Text += pPlayer->GetName();
		Text += "!";
		pTarget->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Text.c_str() );
		pTarget->Emote( EMOTE_STATE_WORK_NOSHEATHE_CHOPWOOD );
		sQuestMgr.OnPlayerKill( pPlayer, pTarget );
	}

	return true;
}

bool IncendiaPowder(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	QuestLogEntry *en = pSpell->p_caster->GetQuestLogForEntry(5088);
	if(pSpell->p_caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pSpell->p_caster->GetPositionX(), 
		pSpell->p_caster->GetPositionY(), pSpell->p_caster->GetPositionZ(), 175944) && en)
	{
		
		if(en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
		{
			en->SetMobCount(0, 1);
			en->SendUpdateAddKill(0);
			en->UpdatePlayerFields();
			return false;
		}
	}

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(10882, -5005.387695f, -2103.348145f, 83.862106f, 6.265121f, true, false, 0, 0);

	return true;
}

bool GemOfTheSerpent(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12369, 254.924606f, 2967.358154f, 1.382471f, 0.918914f, true, false, 0, 0);

	return true;
}

bool KarangsBanner(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	Player * pPlayer = pSpell->p_caster;

	// Banner Aura
	pPlayer->CastSpell(pPlayer, dbcSpell.LookupEntry(20746), true);

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12921, 2231.710205f, -1543.603027f, 90.694946f, 4.700579f, true, false, 0, 0);
	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12921, 2232.534912f, -1556.983276f, 89.744415f, 1.527570f, true, false, 0, 0);

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(12757, 2239.357178f, -1546.649536f, 89.671097f, 3.530336f, true, false, 0, 0);

	return true;
}

bool SpragglesCanteen(uint32 i, Spell * pSpell) // Quest: Lost!
{
  if(!pSpell->u_caster->IsPlayer())
    return true;

  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  
  Creature * target = pPlayer->GetMapMgr()->GetCreature( GET_LOWGUID_PART( pPlayer->GetSelection() ) );
  if(target == nullptr)
    return true;

  if(target->GetEntry() != 9999)
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(4492);
  if(qle == nullptr)
    return true;

  target->SetStandState(0);
  target->setDeathState(ALIVE);

  target->Despawn(30*1000, 1*60*1000);

  qle->SetMobCount(0, 1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();

  return true;
}

bool AllAlongtheWatchtowers(uint32 i, Spell * pSpell)
{
	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(5097);
	if(qle == nullptr)
	{
		qle = pPlayer->GetQuestLogForEntry(5098);
		if(qle == nullptr)
			return true;
	}

    GameObject * tower1 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1304, -1318, 64, 300030);
	GameObject * tower2 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1311, -1589, 61, 300030);
	GameObject * tower3 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1481, -1414, 67, 300030);
	GameObject * tower4 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(1557, -1470, 68, 300030);

	if(tower1 != nullptr)
	{
		if(pPlayer->CalcDistance(pPlayer, tower1) < 30)
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
	if(tower2 != nullptr)
	{
		if(pPlayer->CalcDistance(pPlayer, tower2) < 30)
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			qle->SetMobCount(1, qle->GetMobCount(1)+1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
		}
	}
	if(tower3 != nullptr)
	{
		if(pPlayer->CalcDistance(pPlayer, tower3) < 30)
		if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
		{
			qle->SetMobCount(2, qle->GetMobCount(2)+1);
			qle->SendUpdateAddKill(2);
			qle->UpdatePlayerFields();
		}
	}
	if(tower4 != nullptr)
	{
		if(pPlayer->CalcDistance(pPlayer, tower4) < 30)
		if(qle->GetMobCount(3) < qle->GetQuest()->required_mobcount[3])
		{
			qle->SetMobCount(3, qle->GetMobCount(3)+1);
			qle->SendUpdateAddKill(3);
			qle->UpdatePlayerFields();
		}
	}
	return true;
}

bool YennikuRelease(uint32 i, Spell * pSpell)
{
  	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  	if(!pPlayer)
		return true;

  	if(!pSpell->u_caster->IsPlayer())
    	return true;

  	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(592);
  	if(qle == nullptr)
    	return true;
  
  	Creature * yenniku = TO_CREATURE(pSpell->GetUnitTarget());
  	if(!yenniku)
		return true;

  	yenniku->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 29);
  	yenniku->GetAIInterface()->WipeTargetList();
  	yenniku->Despawn(30*1000, 60*1000);

  	return true;
}

bool ScrollOfMyzrael(uint32 i, Spell * pSpell) 
{
  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  if(!pPlayer)
	  return true;

  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(656);
  if(qle == nullptr)
    return true;

  const float MyzraelPos[] = {-940.7374f, -3111.1953f, 48.9566f, 3.327f};

  Creature * myzrael = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(MyzraelPos[0], MyzraelPos[1], MyzraelPos[2], 2755);
  if(myzrael != nullptr)
  {
    if(!myzrael->isAlive())
      myzrael->Delete();
    else
      return true;
  }

  myzrael = sEAS.SpawnCreature(pPlayer, 2755, MyzraelPos[0], MyzraelPos[1], MyzraelPos[2], MyzraelPos[3], 0);
  return true;
}

bool TheBaitforLarkorwi1(uint32 i, Spell * pSpell)
{
  	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  	if(!pPlayer)
	  	return true;

  	if(!pSpell->u_caster->IsPlayer())
    		return true;

  	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(4292);
  	if(qle == nullptr)
    		return true;
  
	GameObject * obj = NULLGOB;

	obj = sEAS.SpawnGameobject(pPlayer, 169216, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 1, 0, 0, 0, 0);
     	sEAS.GameobjectDelete(obj, 1*60*1000);
     	
  	return true;
}

bool TheBaitforLarkorwi2(uint32 i, Spell * pSpell)
{
  	Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  	if(!pPlayer)
	  	return true;

  	if(!pSpell->u_caster->IsPlayer())
    		return true;

  	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(4292);
  	if(qle == nullptr)
    		return true;
  
	Creature * larkowi = sEAS.SpawnCreature(pPlayer, 9684, pPlayer->GetPositionX()+2, pPlayer->GetPositionY()+3, pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
  	larkowi->Despawn(5*60*1000, 0);

  	return true;
}

bool ZappedGiants(uint32 i, Spell * pSpell)
{
  Player * pPlayer = TO_PLAYER(pSpell->u_caster);
  if(!pPlayer)
	  return true;
  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(7003);
  if(qle == nullptr)
  {
	  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(7725);
	  if(qle == nullptr)
		  return true;
  }

  Creature * creat = TO_CREATURE(pSpell->GetUnitTarget());
  if (creat == nullptr)
  	return true;

  float X = creat->GetPositionX();
  float Y = creat->GetPositionY();
  float Z = creat->GetPositionZ();

  uint32 cit = creat->GetEntry();

  switch(cit)
  {
  case 5360:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature * zappedcreat = sEAS.SpawnCreature(pPlayer, 14639, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  case 5361:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature * zappedcreat = sEAS.SpawnCreature(pPlayer, 14638, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  case 5359:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature * zappedcreat = sEAS.SpawnCreature(pPlayer, 14603, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  case 5358:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature * zappedcreat = sEAS.SpawnCreature(pPlayer, 14640, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  case 5357:
	  {
	  creat->Despawn(1000, 6*60*1000);
	  Creature * zappedcreat = sEAS.SpawnCreature(pPlayer, 14604, X, Y, Z, 0);
	  zappedcreat->Despawn(3*60*1000, 0);
	  }
	  break;
  }
  return true;
}

void SetupQuestItems(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(3607, &YennikuRelease);
	mgr->register_dummy_spell(4141, &ScrollOfMyzrael);
	mgr->register_dummy_spell(8606, &SummonCyclonian);
	mgr->register_dummy_spell(14209, &NixxsPledgeOfSecrecy);
	mgr->register_dummy_spell(15118, &TheBaitforLarkorwi1);
  	mgr->register_dummy_spell(15119, &TheBaitforLarkorwi2);
	mgr->register_dummy_spell(15591, &SpragglesCanteen);
	mgr->register_dummy_spell(16996, &IncendiaPowder);
	mgr->register_dummy_spell(17016, &AllAlongtheWatchtowers);
	mgr->register_dummy_spell(19470, &GemOfTheSerpent);
	mgr->register_dummy_spell(19938, &AwakenPeon);
	mgr->register_dummy_spell(20737, &KarangsBanner);
	mgr->register_dummy_spell(23359, &ZappedGiants);
}