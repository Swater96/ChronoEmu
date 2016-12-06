#include "StdAfx.h"
#include "Setup.h"

class NeutralGuard : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NeutralGuard);
	NeutralGuard(Creature * pCreature) : CreatureAIScript(pCreature) {}
};

void SetupNeutralGuards(ScriptMgr* mgr)
{
    mgr->register_creature_script(4624, &NeutralGuard::Create); //BB bruiser
    mgr->register_creature_script(4624, &NeutralGuard::Create);   //BB bruiser
    mgr->register_creature_script(15088, &NeutralGuard::Create); //BB elite
    mgr->register_creature_script(11102, &NeutralGuard::Create); //Argent Rider
    mgr->register_creature_script(16378, &NeutralGuard::Create); //Argent Sentry
    mgr->register_creature_script(15184, &NeutralGuard::Create); //Cen Hold Infantry
    mgr->register_creature_script(9460, &NeutralGuard::Create); //Gadgetzan bruiser
    mgr->register_creature_script(21448, &NeutralGuard::Create); //Gadgetzan sniper
    mgr->register_creature_script(3502, &NeutralGuard::Create); //Ratchet bruiser
}