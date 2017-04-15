#include "StdAfx.h"
#include "Setup.h"

class Zenn_Foulhoof : public QuestScript
{ 
public:

  void OnQuestComplete(Player * mTarget, QuestLogEntry *qLogEntry)
  {
    if(!mTarget)
		return;

    Creature * creat = mTarget->GetMapMgr()->GetSqlIdCreature(43727);
    if(creat == nullptr)
      return;

    creat->SetUInt32Value(UNIT_FIELD_DISPLAYID,901);
	creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Ribbit! No! This cannot...ribbit...be! You have duped me with...ribbit..your foul trickery! Ribbit!");

	sEventMgr.AddEvent(TO_OBJECT(creat), &Object::EventSetUInt32Value, (uint32)UNIT_FIELD_DISPLAYID, (uint32)10035, EVENT_UNK, 50000, 0, 1);
  }
};

void SetupTeldrassil(ScriptMgr *mgr)
{	
  QuestScript *Zenn_FoulhoofQuest = (QuestScript*) new Zenn_Foulhoof();
  mgr->register_quest_script(489, Zenn_FoulhoofQuest);
}