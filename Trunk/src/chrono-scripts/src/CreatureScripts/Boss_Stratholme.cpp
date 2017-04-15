/*
 * ChronoEmu Scripts - Stratholme Bosses
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by Swater
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"

/* TODO:	Baron run:
				Baroness Anastari - enslave spell
				Ash'ari crystal = 10415
				Abominations walk towards big door. - 40, 40, 80
				Skeleton guards spawn
			Living side:
				Crimson hammersmith spawn
				Fras Siabi spawn
				Postmaster Malown spawn

				Setup cannon event
				Timmy spawn
				Scourge invasion
*/

#define BOSS_MAGISTRATE_BARTHILAS 10435
static Coords MagistrateWaypoint = {3725.92f, -3600.35f, 142.37f, 0.58f};
#define SERVICE_GATE 175368
class ServiceGate : public GameObjectAIScript
{
public:
	ServiceGate(GameObject* goinstance) : GameObjectAIScript(goinstance) { }
	static GameObjectAIScript *Create(GameObject* GO) { return new ServiceGate(GO); }

	void OnActivate( Player* pPlayer )
	{
		Creature *magistrateCre;
		MoonScriptCreatureAI *magistrate;
		magistrateCre = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3662.73f, -3619.25f, 137.88f, BOSS_MAGISTRATE_BARTHILAS);
		if(magistrateCre) {
			magistrate = (MoonScriptCreatureAI*)magistrateCre->GetScript();
			magistrate->Emote("Intruders at the Service Gate! Baron Rivendare must be warned!");
			magistrate->AddWaypoint(magistrate->CreateWaypoint(1, 0, Flag_Run, MagistrateWaypoint));
			magistrateCre->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
			magistrateCre->GetAIInterface()->setWaypointToMove(0);	
		}
	}
};
class MagistrateBarthilasAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(MagistrateBarthilasAI, MoonScriptCreatureAI);
    MagistrateBarthilasAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Crowd pummel
		AddSpell(10887, Target_Self, 15, 0, 15);
		//Draining blow
		AddSpell(16793, Target_Current, 30, 0, 5);
		//Furious anger
		AddSpell(16791, Target_Self, 30, 0, 5);
		//Mighty blow
		AddSpell(14099, Target_Current, 15, 0, 10);
	}		

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 1)
		{
			SpawnCreature(BOSS_MAGISTRATE_BARTHILAS, 4072.12f, -3536.45f, 123.35f, 2.61f);
			Despawn();
		}
		ParentClass::OnReachWP(iWaypointId, bForwards);
	}
};

#define BOSS_BARONESS_ANASTARI 10436
class BaronessAnastariAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(BaronessAnastariAI, MoonScriptCreatureAI);
    BaronessAnastariAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Banshee Curse	16867
		AddSpell(16867, Target_Current, 40, 0, 12);
		// Silence			18327
		AddSpell(18327, Target_Current, 40, 0, 20);
		// Banshee Wail		16565
		AddSpell(16565, Target_Current, 20, 0, 10);
	}
	void OnDied(Unit *pKiller) {
		GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3853.0f, -3721.0f, 138.5f, 175380);
		if(door)
			door->SetState(0);
		ParentClass::OnDied(pKiller);
	}
};

#define BOSS_NERUBENKAN 10437
class NerubenkanAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(NerubenkanAI, MoonScriptCreatureAI);
    NerubenkanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Crypt Scarabs 16418
		AddSpell(16418, Target_Self, 30, 1, 10);
		//Encasing Webs 4962
		AddSpell(4962, Target_Current, 15, 2, 15);
		//Pierce Armor 6016
		AddSpell(6016, Target_Current, 20, 0, 45);
		//Raise Undead Scarab 17235
		AddSpell(17235, Target_Self, 15, 1, 20);
	}
	void OnDied(Unit *pKiller) {
		GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3852.0f, -3523.0f, 144.0f, 175379);
		if(door)
			door->SetState(0);
		ParentClass::OnDied(pKiller);
	}
};

#define BOSS_MALEKI_PALID 10438
class MalekiPalidAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(MalekiPalidAI, MoonScriptCreatureAI);
    MalekiPalidAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Drain Life 17238,
		AddSpell(17238, Target_RandomPlayer, 30, 5, 0);
		//Drain Mana 6524,
		AddSpell(6524, Target_RandomPlayer, 30, 5, 0);
		//Frostbolt 17503
		AddSpell(17503, Target_Current, 30, 2, 0);
		//Ice Tomb 16869
		AddSpell(16869, Target_RandomPlayer, 40, 1.5, 15);
	}
	void OnDied(Unit *pKiller) {
		GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4037.0f, -3649.0f, 135.6f, 175381);
		if(door)
			door->SetState(0);
		ParentClass::OnDied(pKiller);
	}
};

#define CN_MINDLESS_UNDEAD 11030
class MindlessUndeadAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(MindlessUndeadAI, MoonScriptCreatureAI);
    MindlessUndeadAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
	}
	void OnDied(Unit *pKiller) {
		Despawn(2);
		ParentClass::OnDied(pKiller);
	}
};

#define BOSS_RAMSTEIN 10439
#define CN_BLACK_GUARD_SENTRY 10394
static Coords RamsteinWaypoint1 = {4032.0f, -3390.0f, 119.7f, 0.0f};
static Coords MindlessUndeadWaypoint1 = {3930.0f, -3392.0f, 119.7f, 0.0f};
static Coords MindlessUndeadWaypoint2 = {3962.0f, -3393.0f, 119.7f, 0.0f};
static Coords RamsteinWaypoint2 = {4032.0f, -3414.0f, 116.0f, 0.0f};
class RamsteinAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(RamsteinAI, MoonScriptCreatureAI);
    RamsteinAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Knockout
		AddSpell(17307, Target_Current, 50, 0, 10);
		//Trample
		AddSpell(5568, Target_Current, 50, 0, 15);
	}
	void OnReachWP(uint32 wpid, bool forward) {
		if(wpid == 1) {
			GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4032.0f, -3390.0f, 119.7f, 175405);
			if(door)
				door->SetState(1);
		}
		ParentClass::OnReachWP(wpid, forward);
	}
	void OnDied(Unit *pKiller) {
		GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3962.0f, -3393.0f, 119.7f, 175358);
		if(door)
			door->SetState(0);
		MoonScriptCreatureAI *undead;
		for(int i = 0; i<30; i++) {
			undead = nullptr;
			undead = SpawnCreature(CN_MINDLESS_UNDEAD, 3963.0f, -3394.0f, 119.6f, 0.0f);
			undead->AddWaypoint(CreateWaypoint(1, 0, Flag_Run, MindlessUndeadWaypoint1));
			undead->AddWaypoint(CreateWaypoint(2, 0, Flag_Run, MindlessUndeadWaypoint2));
			undead->AddWaypoint(CreateWaypoint(3, 0, Flag_Run, RamsteinWaypoint2));
			undead->GetUnit()->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
			undead->GetUnit()->GetAIInterface()->setWaypointToMove(0);
		}
				MoonScriptCreatureAI *skeletonGuard;
				for(int i = 0; i<5; i++) {
					skeletonGuard = SpawnCreature(CN_BLACK_GUARD_SENTRY, 4032.0f, -3384.6f, 120.0f);
					skeletonGuard->AddWaypoint(CreateWaypoint(1, 0, Flag_Walk, RamsteinWaypoint1));
					skeletonGuard->AddWaypoint(CreateWaypoint(2, 0, Flag_Walk, RamsteinWaypoint2));
					skeletonGuard->GetUnit()->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
					skeletonGuard->GetUnit()->GetAIInterface()->setWaypointToMove(0);
				}
				if(skeletonGuard)
					skeletonGuard->Emote("Who dares to disturb our master?");
				door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4032.0f, -3390.0f, 119.7f, 175405);
				if(door)
					door->SetState(0);
		ParentClass::OnDied(pKiller);
	}
};

#define BARON_RUN_WATCHER		15607
#define BOSS_BARON_RIVENDARE	10440
//Abominations before rivandare
#define MAX_ABOMINATIONS		-7
class BaronRunWatcherAI: public MoonScriptCreatureAI
{
	int acolytes;
	int abominations;
	int guards;
	MoonScriptCreatureAI *rivendare;
public:
	MOONSCRIPT_FACTORY_FUNCTION(BaronRunWatcherAI, MoonScriptCreatureAI);
    BaronRunWatcherAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		acolytes = 0;
		abominations = 0;
		guards = 0;
	}
	int RemoveAcolyte() {
		if(acolytes>0)
			acolytes = 0;
		return --acolytes;
	}
	int RemoveAbomination() {
		if(abominations>0)
			abominations = 0;
		return --abominations;
	}
	int RemoveGuard() {
		if(guards>0)
			guards = 0;
		return --guards;
	}
	void SetBaron(MoonScriptCreatureAI* baron) {
		rivendare = baron;
	}
	MoonScriptCreatureAI *GetBaron() {
		return rivendare;
	}
};

#define CN_THUZADIN_ACOLYTE		10399
class ThuzadinAcolyteAI: public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ThuzadinAcolyteAI, MoonScriptCreatureAI);
    ThuzadinAcolyteAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
	}		

	void OnDied(Unit *pKiller) {
		BaronRunWatcherAI *watcher = (BaronRunWatcherAI *)_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3748.95f, -3591.59f, 142.0f, BARON_RUN_WATCHER);
		if(!watcher) {
			Emote("Did not found watcher!");
		}
		else {
			GameObject *door;
			char buffer[100];
			int acolytes = watcher->RemoveAcolyte();
			sprintf(buffer, "Watcher: %d!", acolytes);
			Emote(buffer);
			switch(acolytes) {
				case -5: Emote("One of the Ash'ari Crystal has been destroyed! Slay the intruders!");
					break;
				case -10: Emote("An Ash'ari Crystal has fallen! Stay true to the Lich King, my brethren, and attempt to resummon it.");
					break;
				case -15: Emote("An Ash'ari Crystal have been destroyed! The Slaughterhouse is vulnerable!");
					door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4039.0f, -3505.0f, 121.7f, 175374);
					if(door)
						door->SetState(0);
					else
						Emote("Door1 not found!");
					door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4039.0f, -3470.0f, 121.7f, 175373);
					if(door)
						door->SetState(0);
					else
						Emote("Door2 not found!");
					break;
				default:
					break;
			}
		}
		ParentClass::OnDied(pKiller);
	}
};

//Black Guard Sentry
#define CN_BLACK_GUARD_SENTRY 10394
class BlackGuardSentryAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BlackGuardSentryAI, MoonScriptCreatureAI);
	BlackGuardSentryAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Cleave
		AddSpell(15496, Target_Current, 25, 0, 0);
		// Shadow Shock
		AddSpell(17439, Target_Current, 20, 0, 0);
	}
	void OnReachWP(uint32 wpid, bool forward) {
		if(wpid == 1) {
			GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4032.0f, -3390.0f, 119.7f, 175405);
			if(door)
				door->SetState(1);
		}
		ParentClass::OnReachWP(wpid, forward);
	}
	void OnDied(Unit *pKiller) {
		BaronRunWatcherAI *watcher = (BaronRunWatcherAI *)_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3748.95f, -3591.59f, 142.0f, BARON_RUN_WATCHER);
		if(!watcher) {
			Emote("Did not found watcher!");
		}
		else {
			char buffer[100];
			int guards = watcher->RemoveGuard();
			sprintf(buffer, "Watcher: %d!", guards);
			Emote(buffer);
			if(guards == -5) {
				MoonScriptCreatureAI *rivendare = watcher->GetBaron();
				if(rivendare)
					rivendare->Emote("Time to take matters into my own hands. Come. Enter my domain and challenge the might of the Scourge!");
				else
					Emote("Baron not found");
				GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4032.0f, -3390.0f, 119.7f, 175405);
				if(door)
					door->SetState(0);
				else
					Emote("door1 not found");
				door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4032.0f, -3365.0f, 115.0f, 175796);
				if(door)
					door->SetState(0);
				else
					Emote("door2 not found");
			}
		}
		ParentClass::OnDied(pKiller);
	}
};

static Coords skellyPoints[6] = {
	{4017.6f, -3338.3f, 115.5f, 0.0f},
	{4012.3f, -3353.9f, 115.5f, 0.0f},
	{4014.3f, -3364.1f, 115.5f, 0.0f},
	{4049.7f, -3339.1f, 115.5f, 0.0f},
	{4052.3f, -3350.5f, 115.5f, 0.0f},
	{4049.8f, -3363.7f, 115.5f, 0.0f}
};
class BaronRivendareAI : public MoonScriptBossAI
{
	int timer;
	bool skelliesUp;
	MoonScriptCreatureAI *skelly[6];
public:
	MOONSCRIPT_FACTORY_FUNCTION(BaronRivendareAI, MoonScriptBossAI);
    BaronRivendareAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
    {
		//Cleave
		AddSpell(15284, Target_Current, 30, 0, 5);
		//Mortal strike
		AddSpell(15708, Target_Current, 20, 0, 10);
		//Shadow bolt
		AddSpell(17393, Target_Current, 15, 2, 5);
		//Unholy aura
		ApplyAura(17467);
	}
	void AIUpdate() {
		int i;
		int bonushealth;
		int health, maxhealth;
		if(IsTimerFinished(timer)) {
			if(!skelliesUp) {
				ResetTimer(timer, 12000);
				skelliesUp = true;
				Emote("Baron Rivendare raises an undead servants back to life!", Text_Emote);
				for(i = 0; i<6; i++) {
					skelly[i] = SpawnCreature(CN_MINDLESS_UNDEAD, skellyPoints[i].mX, skellyPoints[i].mY, skellyPoints[i].mZ, skellyPoints[i].mO);
					skelly[i]->AggroNearestPlayer();
				}
			}
			else {
				ResetTimer(timer, 20000);
				skelliesUp = false;
				Emote("Baron Rivendare attempts to cast Death Pact on his servants!", Text_Emote);
				bonushealth = 0;
				for(i = 0; i<6; i++) {
					if(skelly[i] && skelly[i]->IsAlive()) {
						skelly[i]->Despawn();
						skelly[i] = nullptr;
						bonushealth += 2000;
					}
				}
				if(bonushealth) {
					health = _unit->GetHealthPct() + bonushealth;
					maxhealth = _unit->GetHealthPct() == 100;
					if(health > maxhealth)
						_unit->SetHealthPct(maxhealth);
					else
						_unit->SetHealthPct(health);
				}
			}
		}
		ParentClass::AIUpdate();
	}
	void OnCombatStart(Unit *pTarget) {
		GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4032.0f, -3365.0f, 115.0f, 175796);
		if(door)
			door->SetState(1);
		else
			Emote("Door not found!");
		timer = AddTimer(20000);
		skelliesUp = false;
		ParentClass::OnCombatStart(pTarget);
	}
	void OnCombatStop(Unit *pTarget) {
		GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4032.0f, -3365.0f, 115.0f, 175796);
		if(door)
			door->SetState(0);
		else
			Emote("Baron door not found!");
		door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4039.0f, -3505.0f, 121.7f, 175374);
		if(door)
			door->SetState(0);
		else
			Emote("Door1 not found!");
		for(int i = 0; i<6; i++) {
			if(skelly[i] && skelly[i]->IsAlive()) {
				skelly[i]->Despawn();
				skelly[i] = nullptr;
			}
		}
		RemoveTimer(timer);
		timer = INVALIDATE_TIMER;
		ParentClass::OnCombatStop(pTarget);
	}
};

//Bile Spewer
#define CN_BILE_SPEWER 10416
#define CN_BILE_SLIME  10697
class BileSpewerAI : public MoonScriptCreatureAI
{
	int timer;
	MOONSCRIPT_FACTORY_FUNCTION(BileSpewerAI, MoonScriptCreatureAI);
	BileSpewerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Cleave
		AddSpell(40504, Target_Current, 20, 1, 0);
	}

	void OnCombatStart(Unit *pTarget) {
		timer = AddTimer(5000);
		ParentClass::OnCombatStart(pTarget);
		GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4039.0f, -3505.0f, 121.7f, 175374);
		if(door)
			door->SetState(1);
		ParentClass::OnCombatStart(pTarget);
	}
	void AIUpdate() {
		if(IsTimerFinished(timer)) {
			ResetTimer(timer, 15000);
			SpawnCreature(CN_BILE_SLIME);
			Emote("Bile Spewer belches out a disgusting Bile Slime!", Text_Emote);
		}
		ParentClass::AIUpdate();
	}
	void OnDied(Unit *pKiller) {
		Emote("Bile Spewer explodes and releases several Bile Slimes!", Text_Emote);
		for(int i = 0; i<4; i++) {
			SpawnCreature(CN_BILE_SLIME);
		}
		BaronRunWatcherAI *watcher = (BaronRunWatcherAI *)_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3748.95f, -3591.59f, 142.0f, BARON_RUN_WATCHER);
		if(!watcher) {
			Emote("Did not found watcher!");
		}
		else {
			char buffer[100];
			int abominations = watcher->RemoveAbomination();
			sprintf(buffer, "Watcher: %d!", abominations);
			Emote(buffer);
			if(abominations == MAX_ABOMINATIONS) {
				MoonScriptCreatureAI *rivendare = SpawnCreature(BOSS_BARON_RIVENDARE, 4035.67f, -3337.03f, 115.0f, 4.64f);
				watcher->SetBaron(rivendare);
				if(rivendare)
					rivendare->Emote("So you see fit to toy with Lich King's creations? Ramstein, be sure to give the intruders the proper greeting.");
				else
					Emote("Baron not found!");
				MoonScriptCreatureAI *ramstein = SpawnCreature(BOSS_RAMSTEIN, 4032.0f, -3384.6f, 120.0f);
				if(ramstein) {
					ramstein->Emote("Ramstein hunger for flesh!");
					ramstein->AddWaypoint(CreateWaypoint(1, 0, Flag_Walk, RamsteinWaypoint1));
					ramstein->AddWaypoint(CreateWaypoint(2, 0, Flag_Walk, RamsteinWaypoint2));
					ramstein->GetUnit()->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
					ramstein->GetUnit()->GetAIInterface()->setWaypointToMove(0);
				}
				else
					Emote("Ramstein not found!");
				GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4032.0f, -3390.0f, 119.7f, 175405);
				if(door)
					door->SetState(0);
				else
					Emote("Door not found!");
			}
		}
		ParentClass::OnDied(pKiller);
	}
};

//Venom Belcher
#define CN_VENOM_BELCHER 10417
class VenomBelcherAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VenomBelcherAI, MoonScriptCreatureAI);
	VenomBelcherAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Venom Spit
		AddSpell(16866, Target_Current, 30, 2.5, 3);
	}
	void OnCombatStart(Unit *pTarget) {
		GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4039.0f, -3505.0f, 121.7f, 175374);
		if(door)
			door->SetState(1);
		ParentClass::OnCombatStart(pTarget);
	}
	void OnDied(Unit *pKiller) {
		BaronRunWatcherAI *watcher = (BaronRunWatcherAI *)_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3748.95f, -3591.59f, 142.0f, BARON_RUN_WATCHER);
		if(!watcher) {
			Emote("Did not found watcher!");
		}
		else {
			char buffer[100];
			int abominations = watcher->RemoveAbomination();
			sprintf(buffer, "Watcher: %d!", abominations);
			Emote(buffer);
			if(abominations == MAX_ABOMINATIONS) {
				MoonScriptCreatureAI *rivendare = SpawnCreature(BOSS_BARON_RIVENDARE, 4035.67f, -3337.03f, 115.0f, 4.64f);
				if(rivendare) {
					rivendare->Emote("So you see fit to toy with Lich King's creations? Ramstein, be sure to give the intruders the proper greeting.");
					rivendare->RegisterAIUpdateEvent(12000);
				}
				MoonScriptCreatureAI *ramstein = SpawnCreature(BOSS_RAMSTEIN, 4032.0f, -3384.6f, 120.0f);
				if(ramstein) {
					ramstein->Emote("Ramstein hunger for flesh!");
					ramstein->AddWaypoint(CreateWaypoint(1, 0, Flag_Walk, RamsteinWaypoint1));
					ramstein->AddWaypoint(CreateWaypoint(2, 0, Flag_Walk, RamsteinWaypoint2));
					ramstein->GetUnit()->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
					ramstein->GetUnit()->GetAIInterface()->setWaypointToMove(0);
				}
				GameObject *door = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(4032.0f, -3390.0f, 119.7f, 175405);
				if(door)
					door->SetState(0);
			}
		}
		ParentClass::OnDied(pKiller);
	}
};

#define BOSS_ARCHIVIST_GALFORD 10811
class ArchivistGalford : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(ArchivistGalford, MoonScriptCreatureAI);
    ArchivistGalford(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Burning Winds
		AddSpell(17293, Target_Current, 30, 0, 16);
		//Fire Nova
		AddSpell(17366, Target_Self, 30, 0, 10);
		//Pyroblast
		AddSpell(17274, Target_Current, 30, 0, 12);
	}
};

#define BOSS_CANNON_MASTER_WILLEY 10997
class CannonMasterWilley : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(CannonMasterWilley, MoonScriptCreatureAI);
    CannonMasterWilley(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Shoot					10101
		AddSpell(10101, Target_Current, 60, 0, 4);
		// Pummel					15615
		AddSpell(15615, Target_Current, 10, 0, 10);
		// Knock Away				16496
		AddSpell(16496, Target_RandomPlayer, 10, 0, 15);
		// Summon Crimson Rifleman	17279
//		AddSpell(17279, Target_Self, 20, 0, 5);
	}
};

#define BOSS_CRIMSON_HAMMERSMITH 11120
class CrimsonHammersmith : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(CrimsonHammersmith, MoonScriptCreatureAI);
    CrimsonHammersmith(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Bonesmelt - 17445
		AddSpell(17445, Target_Current, 70, 0, 15);
	}
	void OnCombatStart(Unit *ptarget) {
		Emote("Thieves! I shall smelt your remains!", Text_Say);
		ParentClass::OnCombatStart(ptarget);
	}
};
#define BLACKSMITHING_PLANS 173232
class BlacksmithingPlans : public GameObjectAIScript
{
public:
	BlacksmithingPlans(GameObject* goinstance) : GameObjectAIScript(goinstance) { }
	static GameObjectAIScript *Create(GameObject* GO) { return new BlacksmithingPlans(GO); }

	void OnActivate( Player* pPlayer )
	{
		_gameobject->GetMapMgr()->GetInterface()->SpawnCreature(BOSS_CRIMSON_HAMMERSMITH, _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ(), 0, true, false, 0, 0);
	}
};

//Fras Siabi <-- corrected by light.
#define CN_FRAS_SIABI 11058
class FrasSiabi : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(FrasSiabi, MoonScriptCreatureAI);
    FrasSiabi(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Flame Breath 17294
		AddSpell(17294, Target_Current, 25, 2, 0);
		// Smoke bomb 17294
		AddSpell(7964, Target_Self, 15, 0, 0);
    }
	void OnCombatStart(Unit *ptarget) {
		Emote("Looking for these???? You'll never have em!", Text_Say);
		ParentClass::OnCombatStart(ptarget);
	}
};
#define PREMIUM_TOBACCO 176248
class PremiumTobacco : public GameObjectAIScript
{
public:
	PremiumTobacco(GameObject* goinstance) : GameObjectAIScript(goinstance) { }
	static GameObjectAIScript *Create(GameObject* GO) { return new PremiumTobacco(GO); }

	void OnActivate( Player* pPlayer )
	{
		_gameobject->GetMapMgr()->GetInterface()->SpawnCreature(CN_FRAS_SIABI, _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ(), 0, true, false, 0, 0);
	}
};

//Hearthsinger Forresten <--corrected by light.
#define CN_HEARTSINGER_FORRESTEN 10558
class HeartsingerForrestenAI : public MoonScriptCreatureAI
{
public: 
	MOONSCRIPT_FACTORY_FUNCTION(HeartsingerForrestenAI, MoonScriptCreatureAI);
	HeartsingerForrestenAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Demoralizing Shout
		AddSpell(16244, Target_Self, 25, 0, 0);
		// enchanting lulaby
		AddSpell(16798, Target_RandomPlayer, 20, 1, 0);
		// Multi shot
		AddSpell(14443, Target_RandomPlayer, 20, 0, 0);
		// Shoot
		AddSpell(16100, Target_Current, 50, 0.5, 0);
	}
};


//Malor the Zealous <- Corrected Light
#define CN_MALOR_THE_ZEALOUS 11032
class MalorTheZealous : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(MalorTheZealous, MoonScriptCreatureAI);
    MalorTheZealous(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// Ground Smash 12734
		AddSpell(12734, Target_Current, 15, 0, 2);
		// Head Crack 16172
		AddSpell(16172, Target_RandomPlayer, 20, 0, 0);
		// Holy Light 15493
		AddSpell(15493, Target_Self, 10, 2, 5);
		// Holy Light 17233
	    AddSpell(17233, Target_Self, 9, 0, 60);
    }
};

//Postmaster Malown <- corrected Light
#define CN_POSTMASTER_MALOWN 11143
class PostmasterMalown : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(PostmasterMalown, MoonScriptCreatureAI);
    PostmasterMalown(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Backhand - 6253
		AddSpell(6253, Target_Current, 30, 0, 5);
		//Curse of Tongues - 13338
		AddSpell(13338, Target_RandomPlayer, 20, 0, 0);
		//Curse of Weakness - 12741
		AddSpell(12741, Target_RandomPlayer, 20, 0, 0);
		//Fear - 12542
	    AddSpell(12542, Target_RandomPlayer, 15, 1.5, 0);
    }
	void OnCombatStart(Unit * pTarget)
	{
		 Emote("Prepare to be Malowned!", Text_Say);
		 ParentClass::OnCombatStart(pTarget);
	}
	void OnTargetDied(Unit * pTarget)
	{
		 Emote("You've been MALOWNED!", Text_Say);
		 ParentClass::OnCombatStart(pTarget);
	}
};

//Skul
#define CN_SKUL 10393
class Skul : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Skul, MoonScriptCreatureAI);
    Skul(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Arcane Bolt - 15230
		AddSpell(15230, Target_Current, 30, 1, 15);
		//Frost Armor - 12544
		ApplyAura(12544);
		//Frost Shock - 15499
		AddSpell(15499, Target_Current, 30, 0, 20);
		//Frostbolt - 16799
		AddSpell(16799, Target_Current, 40, 1, 0);
	}
};

//Stonespine
#define CN_STONESPINE 10809
class Stonespine : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Stonespine, MoonScriptCreatureAI);
    Stonespine(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Vicious Rend - 14331
		AddSpell(14331, Target_Current, 90, 0, 10);
	}
};

//Stratholme Courier
#define CN_STRATHOLME_COURIER 11082
class StratholmeCourier : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(StratholmeCourier, MoonScriptCreatureAI);
    StratholmeCourier(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Pummel - 15615
		AddSpell(15615, Target_Current, 80, 1, 15);
		//Snap Kick - 15618
		AddSpell(15618, Target_Current, 20, 0, 10);
	}
};

//Vengeful Phantom
#define CN_VENGEFUL_PHANTOM 10387
class VengefulPhantomAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VengefulPhantomAI, MoonScriptCreatureAI);
	VengefulPhantomAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Fireball
		AddSpell(15089, Target_Current, 90, 3, 0);
	}
};
//The Unforgiven
#define CN_THE_UNFORGIVEN 10516
class TheUnforgiven : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(TheUnforgiven, MoonScriptCreatureAI);
    TheUnforgiven(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		//Frost Nova - 14907
		AddSpell(14907, Target_Current, 90, 0, 20);
	}
	void OnCombatStart(Unit *pTarget) {
		for(int i = 0; i<4; i++) {
			SpawnCreature(CN_VENGEFUL_PHANTOM);
		}
		ParentClass::OnCombatStart(pTarget);
	}
};

//Timmy the Cruel
#define CN_TIMMY_THE_CRUEL	10808
class TimmyTheCruel : public MoonScriptCreatureAI
{
	SpellDesc *Enrage;
	bool do_Enrage;
public:
	MOONSCRIPT_FACTORY_FUNCTION(TimmyTheCruel, MoonScriptCreatureAI);
	TimmyTheCruel(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {
		//Ravenous Claw - 17470
		AddSpell(17470, Target_Current, 90, 0, 8);
		//Enrage @ 30s - 8599
		Enrage = AddSpell(8599, Target_Self, 0, 0, 0);
	}
     
	void OnCombatStart(Unit * pTarget)
	{
		 Emote("TIMMY!");
		 do_Enrage = false;
		 ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate() {
		if(!do_Enrage && (GetHealthPercent() < 30.0)) {
			do_Enrage = true;
			CastSpell(Enrage);
		}
		ParentClass::AIUpdate();
	}

	void OnCombatStop(Unit * pTarget)
	{
		RemoveAllAuras();
		ParentClass::OnCombatStop(pTarget);
	}
};
#define TRIGGER_TIMMY		15563
#define TIMMY_WAYPOINTS			4
static Coords timmyWaypoints[TIMMY_WAYPOINTS] = {
	{3662.1f, -3203.3f, 127.0f, 0.0f},
	{3688.6f, -3189.2f, 127.0f, 0.0f},
	{3685.4f, -3162.3f, 126.8f, 0.0f},
	{3662.9f, -3167.4f, 127.2f, 0.0f}
};
class TriggerTimmy : public MoonScriptCreatureAI
{
	bool timmySpawned;
public:
	MOONSCRIPT_FACTORY_FUNCTION(TriggerTimmy, MoonScriptCreatureAI);
	TriggerTimmy(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {
		// AIUpdate registering
		RegisterAIUpdateEvent(1000);
		timmySpawned = false;
	}

	void AIUpdate() {
		if(!timmySpawned && _unit->GetMapMgr()->GetInterface()->GetPlayerCountInRadius(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 10.0) > 0)
		{
			timmySpawned = true;
			MoonScriptCreatureAI *timmy = SpawnCreature(CN_TIMMY_THE_CRUEL, 3610.7f, -3190.0f, 131.7f);
			if(timmy) {
				for(int i=0; i<TIMMY_WAYPOINTS; i++) {
					timmy->AddWaypoint(CreateWaypoint(i+1, 0, Flag_Walk, timmyWaypoints[i]));
				}
				timmy->GetUnit()->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
				timmy->GetUnit()->GetAIInterface()->setWaypointToMove(0);
			}
			else
				Emote("Timmy not found!");
		}
		ParentClass::AIUpdate();
	}
};

//Dathrohan
#define BOSS_DATHROHAN 10812
#define BOSS_BALNAZZAR 10813
class DathrohanAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(DathrohanAI, MoonScriptCreatureAI);
	DathrohanAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {
		//Cursader strike
		AddSpell(17281, Target_Current, 30, 0, 5);
		//Crusader's hammer
		AddSpell(17286, Target_Self, 15, 0, 15);
		//Holy strike
		AddSpell(17284, Target_Current, 30, 0, 10);
	}
     
	void OnCombatStart(Unit * pTarget)
	{
		 Emote("Today you have unmade what took me years to create! For this you shall all die by my hand!");
		 ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate() {
		if(GetHealthPercent() < 30.0) {
			SpawnCreature(BOSS_BALNAZZAR);
			Despawn();
		}
		ParentClass::AIUpdate();
	}
};
//Balnazzar
class BalnazzarAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(BalnazzarAI, MoonScriptCreatureAI);
	BalnazzarAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {
		//Domination
		AddSpell(17405, Target_RandomPlayer, 30, 0, 20);
		//Mind blast
		AddSpell(17287, Target_Current, 30, 0, 10);
		//Shadow shock
		AddSpell(17399, Target_Current, 30, 0, 10);
		//Psychinc scream
		AddSpell(13704, Target_Self, 30, 0, 20);
		//Sleep
		AddSpell(12098, Target_RandomPlayerNotCurrent, 30, 0, 20);
	}
     
	void OnCombatStart(Unit * pTarget)
	{
		Emote("You fools think you can defeat me so easily? Face the true might of the Nathrezim!");
		ParentClass::OnCombatStart(pTarget);
	}
	void OnCombatStop(Unit * pTarget)
	{
		if(IsAlive()) {
			SpawnCreature(BOSS_DATHROHAN);
			Despawn();
		}
		ParentClass::OnCombatStop(pTarget);
	}
	void OnDied(Unit *pKiller) {
		Emote("Damn you mortals! All my plans of revenge, all my hate... all burned to ash...", Text_Say);
		ParentClass::OnDied(pKiller);
	}
};

/*
Dungeon 2 quest - disabled for now
//Jarien <-- Corrected by light
// Cleave 15284
// Cripple 20812
//Mortal strike 16856
// Shadow Shock 16101
// Vengeance 27650
#define CN_JARIEN 16101
class Jarien : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Jarien, MoonScriptCreatureAI);
    Jarien(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(15284, Target_Current, 15, 0, 0);
		AddSpell(20812, Target_RandomPlayer, 15, 1, 0);
		AddSpell(13729, Target_RandomPlayer, 10, 0, 0);
	    AddSpell(16101, Target_Current, 25, 0, 0);
		AddSpell(27650, Target_Self, 8, 0, 15);
	}
};

//Sothos
//---------------
//npc=16102
//http://www.wowhead.com/npc=16102
//Fear - 27641
//Shadow Bolt Volley - 27646
//Shield Block - 12169
//Shield Charge - 15749
//Shield Slam - 15655
//Vengeance - 27650
#define CN_SOTHOS 16102
class Sothos : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(Sothos, MoonScriptCreatureAI);
    Sothos(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
    {
		AddSpell(27641, Target_Current, 20, 0, 30);
		AddSpell(27646, Target_Current, 20, 0, 25);
		AddSpell(12169, Target_Current, 20, 0, 20);
		AddSpell(15749, Target_Current, 10, 0, 15);
		AddSpell(15655, Target_Current, 10, 0, 10);
		AddSpell(27650, Target_Current, 20, 0, 12);
	}
};
*/

void SetupStratholmeBosses(ScriptMgr * mgr)
{
	mgr->register_creature_script(BARON_RUN_WATCHER, &BaronRunWatcherAI::Create);
	mgr->register_creature_script(BOSS_MAGISTRATE_BARTHILAS, &MagistrateBarthilasAI::Create);
	mgr->register_gameobject_script(SERVICE_GATE, &ServiceGate::Create);

	mgr->register_creature_script(BOSS_BARONESS_ANASTARI, &BaronessAnastariAI::Create);
	mgr->register_creature_script(BOSS_NERUBENKAN, &NerubenkanAI::Create);
	mgr->register_creature_script(BOSS_MALEKI_PALID, &MalekiPalidAI::Create);
	mgr->register_creature_script(CN_THUZADIN_ACOLYTE, &ThuzadinAcolyteAI::Create);

	mgr->register_creature_script(CN_BILE_SPEWER, &BileSpewerAI::Create);
	mgr->register_creature_script(CN_VENOM_BELCHER, &VenomBelcherAI::Create);
	mgr->register_creature_script(BOSS_RAMSTEIN, &RamsteinAI::Create);
	mgr->register_creature_script(CN_BLACK_GUARD_SENTRY, &BlackGuardSentryAI::Create);
	mgr->register_creature_script(CN_MINDLESS_UNDEAD, &MindlessUndeadAI::Create);
	mgr->register_creature_script(BOSS_BARON_RIVENDARE, &BaronRivendareAI::Create);

	mgr->register_creature_script(BOSS_ARCHIVIST_GALFORD, &ArchivistGalford::Create);
    mgr->register_creature_script(BOSS_CANNON_MASTER_WILLEY, &CannonMasterWilley::Create);
	mgr->register_gameobject_script(BLACKSMITHING_PLANS, &BlacksmithingPlans::Create);
    mgr->register_creature_script(BOSS_CRIMSON_HAMMERSMITH, &CrimsonHammersmith::Create);
	mgr->register_gameobject_script(PREMIUM_TOBACCO, &PremiumTobacco::Create);
	mgr->register_creature_script(CN_FRAS_SIABI, &FrasSiabi::Create);
	mgr->register_creature_script(CN_HEARTSINGER_FORRESTEN, &HeartsingerForrestenAI::Create);
    mgr->register_creature_script(CN_MALOR_THE_ZEALOUS, &MalorTheZealous::Create);
	mgr->register_creature_script(CN_POSTMASTER_MALOWN, &PostmasterMalown::Create);
    mgr->register_creature_script(CN_SKUL, &Skul::Create);
    mgr->register_creature_script(CN_STONESPINE, &Stonespine::Create);
    mgr->register_creature_script(CN_STRATHOLME_COURIER, &StratholmeCourier::Create);
	mgr->register_creature_script(CN_VENGEFUL_PHANTOM, &VengefulPhantomAI::Create);
    mgr->register_creature_script(CN_THE_UNFORGIVEN, &TheUnforgiven::Create);
	mgr->register_creature_script(CN_TIMMY_THE_CRUEL, &TimmyTheCruel::Create);
	mgr->register_creature_script(TRIGGER_TIMMY, &TriggerTimmy::Create);
	mgr->register_creature_script(BOSS_DATHROHAN, &DathrohanAI::Create);
	mgr->register_creature_script(BOSS_BALNAZZAR, &BalnazzarAI::Create);
//    mgr->register_creature_script(CN_JARIEN, &Jarien::Create);
//    mgr->register_creature_script(CN_SOTHOS, &Sothos::Create);
}
