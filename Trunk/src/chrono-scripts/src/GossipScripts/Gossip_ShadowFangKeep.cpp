#include "StdAfx.h"
#include "Setup.h"

class DSAdamantGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		/*"At last! Someone to free me from this cell! High Executor Hadrec sent us to gather information on the Keep so that a plan could be formulated to overthrow Arugal once and for all. 
		But the old wizard has many tricks up his sleeve and we were detected by a magical ward. I was thrown in this prison. Vincent was not so lucky. I must return to Hadrec to debrief him at once. 
		But first I will pick the lock to the courtyard door for you. Perhaps you can try your luck against the foes that lurk beyond." */
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 500 , Plr);
		
		Menu->AddItem(0, "Please unlock the courtyard door.", 1);
		if(AutoSend)
			Menu->SendTo(Plr);
	}
	
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
			if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;
		switch(IntId)
		{
			case 1:
				Creature* Adamant = Plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-243.712006f, 2113.719971f, 81.262901f, 3849);

				Adamant->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Free from this wretched cell at last! Let me show you the courtyard..");
				Adamant->GetAIInterface()->StopMovement(0);
				Adamant->SetUInt32Value(UNIT_NPC_FLAGS, 0);
				Adamant->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				Adamant->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				Adamant->GetAIInterface()->setWaypointToMove(1);
				Adamant->GetAIInterface()->SetAllowedToEnterCombat(false);
				Adamant->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
				break;
		}
	}
	void Destroy()
	{
		delete this;
	}
};

class SorcererAshcrombeGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		//needs text
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1 , Plr);
		
		Menu->AddItem(0, "Please unlock the courtyard door.", 1);
		if(AutoSend)
			Menu->SendTo(Plr);
	}
	
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
			if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;
			case 1:
				Creature* Sorcerer = Plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-234.673996f, 2134.550049f, 81.179604f, 3850);
				
				Sorcerer->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Follow me and I'll open the courtyard for you.");
				Sorcerer->GetAIInterface()->StopMovement(0);
				Sorcerer->SetUInt32Value(UNIT_NPC_FLAGS, 0);
				Sorcerer->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				Sorcerer->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				Sorcerer->GetAIInterface()->setWaypointToMove(1);
				Sorcerer->GetAIInterface()->SetAllowedToEnterCombat(false);
				Sorcerer->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
				Sorcerer->m_noRespawn = true;

				break;
		}
	}
	void Destroy()
	{
		delete this;
	}
};

void SetupShadowFangKeepGossip(ScriptMgr * mgr)
{
	GossipScript * dsGossip = (GossipScript*) new DSAdamantGossip();
	mgr->register_gossip_script( 3849, dsGossip );
	GossipScript * saGossip = (GossipScript*) new SorcererAshcrombeGossip();
	mgr->register_gossip_script( 3850, saGossip );
}