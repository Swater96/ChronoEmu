
#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"

/************************************************************************/
/* Instance_Deadmines.cpp Script										*/
/************************************************************************/
#define rhk_slam 6304
static Coords RahkZorPatrol[] =
{
	{ 0, 0, 0, 0 },
	{ -115.935257f, -393.639557f, 56.585880f, 3.844707f },
	{ -126.897156f, -402.931152f, 58.730247f, 3.712760f },
	{ -132.423828f, -407.367920f, 57.747334f, 3.170049f },
	{ -142.107635f, -406.949432f, 58.469383f, 2.632051f },
	{ -161.788971f, -396.371918f, 56.850971f, 3.924032f },
	{ -175.909073f, -410.408661f, 56.228081f, 4.030846f },
	{ -194.484467f, -433.414825f, 53.515301f, 4.895568f },
	{ -190.664108f, -460.454102f, 54.555592f, 4.851583f },
	{ -187.494629f, -488.569855f, 54.047359f, 4.828808f },
	{ -185.976471f, -501.551422f, 53.025299f, 3.433941f },
	{ -199.283859f, -506.088776f, 53.126282f, 3.461430f },
	{ -211.889130f, -507.714111f, 50.848007f, 2.141961f },
	{ -192.621002f, -451.197998f, 54.432598f, 1.668860f }, // RhahkZor Coords
	{ -193.557281f, -437.71830f, 53.375732f, 1.145287f},
	{ -183.677490f, -421.232147f, 54.230988f, 1.058893f}
};
static Coords SneedPatrol[] = // Post - Boss Patrol Coords
{
	{ 0, 0, 0, 0 },
	{ -218.698029f, -504.447021f, 50.705734f, 1.939036f },
	{ -223.969040f, -491.469055f, 48.083599f, 2.465254f },
	{ -235.209320f, -482.447357f, 49.138290f, 3.118654f },
	{ -246.257996f, -482.497620f, 49.448624f, 3.146143f },
	{ -263.677826f, -482.576874f, 49.448624f, 3.350346f },
	{ -278.097809f, -485.631622f, 48.852283f, 3.454489f },
	{ -290.460938f, -490.285431f, 49.863270f, 4.675783f },
	{ -290.146210f, -512.073181f, 49.723511f, 4.726833f },
	{ -290.198212f, -530.489136f, 49.479790f, 4.730761f },
	{ -288.927094f, -556.141602f, 49.447243f, 4.626302f },
	{ -288.747803f, -568.546814f, 49.204010f, 3.856614f },
	{ -300.211700f, -578.343994f, 47.446377f, 4.249312f },
	{ -306.063354f, -595.038696f, 48.094700f, 5.843667f }
};
static Coords MachinePatrol[] = // Sneed Machine Patrol Coords
{
	{0, 0, 0, 0 },
	{ -289.823456f, -528.582581f, 49.769726f, 1.557625f },
	{ -291.187042f, -509.102905f, 49.867317f, 1.557625f },
	{ -291.002472f, -498.935242f, 50.117516f, 3.165752f },
	{ -297.752319f, -499.398529f, 49.519905f, 4.596747f },
	{ -298.758453f, -508.060333f, 49.441177f, 4.565816f },
	{ -299.142334f, -515.390320f, 51.159740f, 3.497674f },
	{ -305.768494f, -517.884460f, 53.302509f, 2.645517f },
	{ -311.303772f, -515.336609f, 54.291870f, 1.428149f },
	{ -309.806610f, -509.386444f, 57.014729f, 1.432076f },
	{ -311.303772f, -515.336609f, 54.291870f, 0.072174f },
	{ -305.768494f, -517.884460f, 53.302509f, 0.068247f },
	{ -299.142334f, -515.390320f, 51.159740f, 1.573070f },
	{ -298.758453f, -508.060333f, 49.441177f, 0.039972f },
	{ -290.572174f, -499.577087f, 50.117523f, 1.510238f },
	{ -290.121918f, -492.150696f, 49.950386f, 0.881973f },
	{ -284.302063f, -485.739288f, 49.952930f, 0.021961f },
	{ -278.943359f, -485.621582f, 49.908035f, 4.534074f },
	{ -283.763550f, -510.152527f, 49.157421f, 5.535455f },
	{ -274.205994f, -519.455383f, 49.882935f, 5.535455f }
};
static Coords GilnidPatrol[] = // Post - Boss Patrol Coords
{
	{ 0, 0, 0, 0 },
	{ -258.974731f, -578.541199f, 51.149902f, 0.002661f },
	{ -243.854004f, -578.577454f, 51.149723f, 6.281919f },
	{ -231.124527f, -578.693542f, 51.228798f, 1.298567f },
	{ -223.494064f, -556.498535f, 51.230316f, 0.164452f },
	{ -197.026047f, -552.106140f, 51.230316f, 5.473743f },
	{ -183.632919f, -564.888489f, 51.230316f, 4.998577f },
	{ -181.750595f, -571.638489f, 49.180130f, 4.823277f },
	{ -179.746521f, -589.637268f, 42.034912f, 3.817969f },
	{ -192.170914f, -601.852600f, 34.957771f, 3.951487f },
	{ -210.035843f, -607.675659f, 27.887510f, 2.467082f },
	{ -221.558578f, -600.173462f, 22.617739f, 2.317857f },
	{ -229.587997f, -592.080994f, 19.306490f, 1.885888f },
	{ -232.489319f, -580.447327f, 19.306490f, 1.815202f },
	{ -228.121643f, -560.797424f, 19.306490f, 0.432901f },
	{ -213.502792f, -552.276001f, 19.306490f, 6.233067f },
	{ -195.057510f, -552.693481f, 19.306490f, 5.486939f },
	{ -183.535126f, -564.359985f, 19.306490f, 5.135866f },
	{ -176.750626f, -579.411560f, 19.309629f, 0.003287f },
	{ -162.578506f, -579.664734f, 19.314703f, 0.015068f },
	{ -148.820068f, -579.457458f, 19.315142f, 0.015068f }
};
/*static Coords Cannon[] =
{
	{ 0, 0, 0, 0 },
	{ -97.020081f, -719.613159f, 8.504338f, 1.506573f },
	{ -96.117409f, -706.574402f, 8.852776f, 1.608675f },
	{ -96.676445f, -691.822815f, 8.237902f, 1.775179f },
	{ -100.284500f, -677.437500f, 7.417490f, 1.926761f },
	{ -96.122841f, -675.890259f, 7.414724f, 1.926761f }
};
*/

class RhahkZorAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(RhahkZorAI);
    RhahkZorAI(Creature* pCreature) : CreatureAIScript(pCreature) 
	{		
		slam = dbcSpell.LookupEntry(rhk_slam);
	}
    void OnDied(Unit * mKiller)
    {
		int i = 0;

		RemoveAIUpdateEvent();

		GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-190.860092f, -456.332184f, 54.496822f, 13965);
		pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-190.860092f, -456.332184f, 54.496822f, 13965);
        if(pDoor == 0)
            return;

        pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		// Patrol
		Creature* Evoker1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(1729, -98.943825f, -395.818573f, 58.528511f, 3.030249f, true, false, 0, 0);
		Evoker1 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-98.443825f, -394.818573f, 58.528511f, 1729);
		Creature* Evoker2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(1729, -91.905466f, -398.727721f, 58.592327f, 3.030249f, true, false, 0, 0);
		Evoker2 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-91.905466f, -398.727721f, 58.592327f, 1729);
		Creature* Overseer = _unit->GetMapMgr()->GetInterface()->SpawnCreature(634, -95.384322f, -392.204529f, 58.971893f, 3.030249f, true, false, 0, 0);
		Overseer = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-96.984322f, -392.204529f, 58.971893f, 634);
				
		if(Evoker1)
				{
					for(i=1;i<10;i++)
						{
							WayPoint * wp = Evoker1->CreateWaypointStruct();
							wp->id = i;
							wp->x = RahkZorPatrol[i].mX;
							wp->y = RahkZorPatrol[i].mY;
							wp->z = RahkZorPatrol[i].mZ;
							wp->o = RahkZorPatrol[i].mO;
							wp->waittime = 0;
							wp->flags = WALK;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Evoker1->GetAIInterface()->addWayPoint(wp);
						}
					Evoker1->GetAIInterface()->SetAIState(STATE_MOVEWP);
				}

		if(Evoker2)
				{
					for(i=1;i<10;i++)
						{
							WayPoint * wp = Evoker2->CreateWaypointStruct();
							wp->id = i;
							wp->x = RahkZorPatrol[i].mX;
							wp->y = RahkZorPatrol[i].mY;
							wp->z = RahkZorPatrol[i].mZ;
							wp->o = RahkZorPatrol[i].mO;
							wp->waittime = 0;
							wp->flags = WALK;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Evoker2->GetAIInterface()->addWayPoint(wp);
						}
					Evoker2->GetAIInterface()->SetAIState(STATE_MOVEWP);
				}

		if(Overseer)
				{
					for(i=1;i<10;i++)
						{
							WayPoint * wp = Overseer->CreateWaypointStruct();
							wp->id = i;
							wp->x = RahkZorPatrol[i].mX;
							wp->y = RahkZorPatrol[i].mY;
							wp->z = RahkZorPatrol[i].mZ;
							wp->o = RahkZorPatrol[i].mO;
							wp->waittime = 0;
							wp->flags = WALK;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Overseer->GetAIInterface()->addWayPoint(wp);
						}
					Overseer->GetAIInterface()->SetAIState(STATE_MOVEWP);
				}
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        RemoveAIUpdateEvent();
    }

    void OnCombatStart(Unit* mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Van Cleef pay big for your heads!");
        _unit->PlaySoundToSet(5774);
		_unit->setEmoteState(14); // Roar
		timer = 10000 + RandomUInt(4000);
		RegisterAIUpdateEvent(timer);
    }

    void AIUpdate()
	{
		timer = 10000 + RandomUInt(4000);

		Unit *target = nullptr;
		target = _unit->GetAIInterface()->GetNextTarget();
		_unit->CastSpell(target, slam, true);
		RemoveAIUpdateEvent();
		RegisterAIUpdateEvent(timer);
	}

    void Destroy()
    {
        delete (RhahkZorAI*)this;
    }

protected:
	SpellEntry *slam;
	int timer;
};


#define terrify 7399
#define distracting_pain 3603
class SneedsShredderAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(SneedsShredderAI, MoonScriptCreatureAI);
    SneedsShredderAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
				AddSpell(terrify, Target_Self, 8, 0, 15);
				AddSpell(distracting_pain, Target_Current, 15, 0, 6);
	}

    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
		spawnposx = _unit->GetPositionX();
		spawnposy = _unit->GetPositionY();
		spawnposz = _unit->GetPositionZ();
		spawnfacing = _unit->GetOrientation();

		_unit->GetMapMgr()->GetInterface()->SpawnCreature(643, spawnposx, spawnposy, spawnposz, spawnfacing, true, false, 0, 0);
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
	}

    void Destroy()
    {
        delete (SneedsShredderAI*)this;
    }
protected:
	float spawnposx, spawnposy, spawnposz, spawnfacing;
};

class SneedAI : public CreatureAIScript
{
public:
    SneedAI(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnCombatStop(Unit *mTarget) // Sneed Reset
    {
		int i = 0;
		Creature* Shredder = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 642);
		if(_unit->isAlive() && _unit->GetHealthPct() > 0)
		{
			Creature* Shredder1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(642, -289.823456f, -528.582581f, 49.769726f, 1.557625f, true, false, 0, 0);
			Shredder1->GetAIInterface()->SetAIState(STATE_MOVEWP);
			for(i=1;i<21;i++)
			{
				WayPoint * wp = Shredder1->CreateWaypointStruct();
				wp->id = i;
				wp->x = MachinePatrol[i].mX;
				wp->y = MachinePatrol[i].mY;
				wp->z = MachinePatrol[i].mZ;
				wp->o = MachinePatrol[i].mO;
				wp->waittime = 0;
				wp->flags = WALK;
				wp->forwardemoteoneshot = 0;
				wp->forwardemoteid = 0;
				wp->backwardemoteoneshot = 0;
				wp->backwardemoteid = 0;
				wp->forwardskinid = 0;
				wp->backwardskinid = 0;
				Shredder1->GetAIInterface()->addWayPoint(wp);
			}
		}
			Shredder->Despawn(1000, 0);
			_unit->Despawn(1000, 0);
	}
		void OnDied(Unit * mKiller)
		{
			int i = 0;

			GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(
            -289.691650f, -535.988953f, 49.440678f, 16400);
			if(pDoor == 0)
            return;
			pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);

			Creature* Overseer1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(634, -218.364044f, -503.967743f, 50.749466f, 1.357839f, true, false, 0, 0);
			Overseer1 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-218.364044f, -503.967743f, 50.749466f, 634);

				if(Overseer1)
				{
					for(i=1;i<10;i++)
						{
							WayPoint * wp = Overseer1->CreateWaypointStruct();
							wp->id = i;
							wp->x = SneedPatrol[i].mX;
							wp->y = SneedPatrol[i].mY;
							wp->z = SneedPatrol[i].mZ;
							wp->o = SneedPatrol[i].mO;
							wp->waittime = 0;
							wp->flags = WALK;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Overseer1->GetAIInterface()->addWayPoint(wp);
						}
					Overseer1->GetAIInterface()->SetAIState(STATE_MOVEWP);
				}

			Creature* Overseer2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(634, -217.359039f, -507.278564f, 50.886860f, 1.357839f, true, false, 0, 0);
			Overseer2 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-217.359039f, -507.278564f, 50.886860f, 634);

				if(Overseer2)
				{
					for(i=1;i<10;i++)
						{
							WayPoint * wp = Overseer2->CreateWaypointStruct();
							wp->id = i;
							wp->x = SneedPatrol[i].mX;
							wp->y = SneedPatrol[i].mY;
							wp->z = SneedPatrol[i].mZ;
							wp->o = SneedPatrol[i].mO;
							wp->waittime = 0;
							wp->flags = WALK;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Overseer2->GetAIInterface()->addWayPoint(wp);
						}
					Overseer2->GetAIInterface()->SetAIState(STATE_MOVEWP);
				}


			Creature* Taskmaster = _unit->GetMapMgr()->GetInterface()->SpawnCreature(4417, -216.359039f, -511.450592f, 50.921513f, 1.357839f, true, false, 0, 0);
			Taskmaster = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-216.359039f, -511.450592f, 50.921513f, 4417);

				if(Taskmaster)
				{
					for(i=1;i<10;i++)
						{
							WayPoint * wp = Taskmaster->CreateWaypointStruct();
							wp->id = i;
							wp->x = SneedPatrol[i].mX;
							wp->y = SneedPatrol[i].mY;
							wp->z = SneedPatrol[i].mZ;
							wp->o = SneedPatrol[i].mO;
							wp->waittime = 0;
							wp->flags = WALK;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Taskmaster->GetAIInterface()->addWayPoint(wp);
						}
					Taskmaster->GetAIInterface()->SetAIState(STATE_MOVEWP);
				}
    }

    void Destroy()
    {
        delete (SneedAI*)this;
	}
   static CreatureAIScript * Create(Creature * c) { return new SneedAI(c); }
};


// Gilnid -> Opens Foundary door on death.
#define molten_metal 5213        
#define melt_ore 5159       
class GilnidAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GilnidAI);
    GilnidAI(Creature* pCreature) : CreatureAIScript(pCreature) 
	{
	molten = dbcSpell.LookupEntry(molten_metal);
	melt = dbcSpell.LookupEntry(melt_ore);
	}
	
    void OnDied(Unit * mKiller)
    {
		int i = 0;

		GameObject * pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-176.569f, -577.640991f, 19.311600f, 16399);
        if(pDoor == 0)
            return;
		pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);

			Creature* Taskmaster1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(4417, -264.409607f, -579.255737f, 50.541454f, 0.010515f, true, false, 0, 0);
			Taskmaster1 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-264.409607f, -579.255737f, 50.541454f, 4417);

				if(Taskmaster1)
				{
					for(i=1;i<18;i++)
						{
							WayPoint * wp = Taskmaster1->CreateWaypointStruct();
							wp->id = i;
							wp->x = GilnidPatrol[i].mX;
							wp->y = GilnidPatrol[i].mY;
							wp->z = GilnidPatrol[i].mZ;
							wp->o = GilnidPatrol[i].mO;
							wp->waittime = 0;
							wp->flags = WALK;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Taskmaster1->GetAIInterface()->addWayPoint(wp);
						}
					Taskmaster1->GetAIInterface()->SetAIState(STATE_MOVEWP);
				}

			Creature* Wizard1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(4418, -266.426117f, -579.374573f, 50.505814f, 0.010515f, true, false, 0, 0);
			Wizard1 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-266.426117f, -579.374573f, 50.505814f, 4418);
				if(Wizard1)
				{
					for(i=1;i<18;i++)
						{
							WayPoint * wp = Wizard1->CreateWaypointStruct();
							wp->id = i;
							wp->x = GilnidPatrol[i].mX;
							wp->y = GilnidPatrol[i].mY;
							wp->z = GilnidPatrol[i].mZ;
							wp->o = GilnidPatrol[i].mO;
							wp->waittime = 0;
							wp->flags = WALK;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Wizard1->GetAIInterface()->addWayPoint(wp);
						}
					Wizard1->GetAIInterface()->SetAIState(STATE_MOVEWP);
				}


			Creature* Taskmaster2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(4417, -269.181335f, -579.536926f, 50.097893f, 0.010515f, true, false, 0, 0);
			Taskmaster2 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-269.181335f, -579.536926f, 50.097893f, 4417);

				if(Taskmaster2)
				{
					for(i=1;i<18;i++)
						{
							WayPoint * wp = Taskmaster2->CreateWaypointStruct();
							wp->id = i;
							wp->x = GilnidPatrol[i].mX;
							wp->y = GilnidPatrol[i].mY;
							wp->z = GilnidPatrol[i].mZ;
							wp->o = GilnidPatrol[i].mO;
							wp->waittime = 0;
							wp->flags = WALK;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Taskmaster2->GetAIInterface()->addWayPoint(wp);
						}
					Taskmaster2->GetAIInterface()->SetAIState(STATE_MOVEWP);
				}


			Creature* Wizard2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(4418, -271.756866f, -579.688660f, 49.775177f, 0.010515f, true, false, 0, 0);
			Wizard2 = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-271.756866f, -579.688660f, 49.775177f, 4418);

				if(Wizard2)
				{
					for(i=1;i<18;i++)
						{
							WayPoint * wp = Wizard2->CreateWaypointStruct();
							wp->id = i;
							wp->x = GilnidPatrol[i].mX;
							wp->y = GilnidPatrol[i].mY;
							wp->z = GilnidPatrol[i].mZ;
							wp->o = GilnidPatrol[i].mO;
							wp->waittime = 0;
							wp->flags = WALK;
							wp->forwardemoteoneshot = 0;
							wp->forwardemoteid = 0;
							wp->backwardemoteoneshot = 0;
							wp->backwardemoteid = 0;
							wp->forwardskinid = 0;
							wp->backwardskinid = 0;
							Wizard2->GetAIInterface()->addWayPoint(wp);
						}
					Wizard2->GetAIInterface()->SetAIState(STATE_MOVEWP);
				}
    }

    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
	}	
	
    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        RemoveAIUpdateEvent();
    }
	
	void AIUpdate()
	{
		meltcount++;
		moltencount++;
		timer1 = 6 + RandomUInt(3);
		timer2 = 13 + RandomUInt(3);

		if(moltencount >= timer1 && _unit->GetCurrentSpell() == nullptr && _unit->GetAIInterface()->GetNextTarget())
		{
			Unit *plr = nullptr;
			plr = _unit->GetAIInterface()->GetNextTarget();
			_unit->CastSpell(plr, molten, true);
			moltencount = 0;
		}
		if(meltcount >= timer2 && _unit->GetCurrentSpell() == nullptr && _unit->GetAIInterface()->GetNextTarget())
		{
			Unit *plr = nullptr;
			plr = _unit->GetAIInterface()->GetNextTarget();
			_unit->CastSpell(plr, melt, true);
			meltcount = 0;
		}
	}

    void Destroy()
    {
        delete (GilnidAI*)this;
    }

protected:
	SpellEntry * molten;
	SpellEntry * melt;
	int timer1, timer2;
	int meltcount, moltencount;
};

#define THRASH 3391
class VanCleefAI : public CreatureAIScript
{
public:
    VanCleefAI(Creature *pCreature) : CreatureAIScript(pCreature)
    {
        mPhase = 0;
		thrash = dbcSpell.LookupEntry(THRASH);
    }

    void OnCombatStart(Unit* mTarget)
    {
		_unit->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, _unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME) / 2);
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "None may challenge the brotherhood.");
        _unit->PlaySoundToSet(5780);
    }

	void OnCombatStop(Unit* mTarget)
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		mPhase = 0;
		RemoveAIUpdateEvent();
	}

    void OnTargetDied(Unit* mTarget)
    {
        char msg[200];
        if(mTarget->GetTypeId() == TYPEID_PLAYER)
            sprintf(msg, "And stay down, %s.", static_cast<Player*>(mTarget)->GetName());
        else if (mTarget->GetTypeFromGUID() == HIGHGUID_TYPE_PET)
            sprintf(msg, "And stay down, %s.", static_cast<Pet*>(mTarget)->GetName().c_str());

        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg);
        _unit->PlaySoundToSet(5781);
    }

    void OnDamageTaken(Unit* mAttacker, float fAmount)
    {
        if(fAmount < 5) return;

        // <100% hp -> We go to phase 1
        if(_unit->GetHealthPct() <= 75 && mPhase == 0) {
            ChangeToPhase1();
        }

        // <67% hp -> We go to phase 2
        if(_unit->GetHealthPct() <= 50 && mPhase == 1) {
            ChangeToPhase2();
        }

        // <34% hp -> We go to phase 3
        if(_unit->GetHealthPct() <= 25 && mPhase == 2) {
            ChangeToPhase3();
        }
    }

    void ChangeToPhase1()
    {
        // Set phase var
        mPhase = 1;
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lap dogs, all of you.");
        _unit->PlaySoundToSet(5782); 
    }

    void ChangeToPhase2()
    {
        // Set phase var
        mPhase = 2;
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Fools! Our cause is righteous.");
        _unit->PlaySoundToSet(5783); 
    }

    void ChangeToPhase3()
    {
        // Set phase var
        mPhase = 3;
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The brotherhood shall remain.");
        _unit->PlaySoundToSet(5784);
    }

	void AIUpdate()
	{
		timer = 3000 + RandomUInt(4000);
		_unit->CastSpell(_unit, thrash, true);
		RemoveAIUpdateEvent();
		RegisterAIUpdateEvent(timer);
	}

    void Destroy()
    {
        delete (VanCleefAI*)this;
    }

    static CreatureAIScript * Create(Creature * c) { return new VanCleefAI(c); }

protected:
    uint32 mPhase;  // NPC has 3 phases
	int32 timer;
	SpellEntry* thrash;
};


#define stomp 6432
#define hammer_buff 6436
class MrSmiteAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MrSmiteAI);

    MrSmiteAI(Creature * pCreature) : CreatureAIScript(pCreature) 
	{
		smite_stomp = dbcSpell.LookupEntry(stomp);
		smite_buff = dbcSpell.LookupEntry(hammer_buff);
	}

    void OnCombatStart(Unit * mTarget)
	{
        phase = 0;
		move = 0;
		looting = 0;
		position = 0;
    }

    void OnDamageTaken(Unit * mAttacker, float fAmount)
    {
        if(_unit->GetHealthPct() <= 66 && phase == 0)
        {
			Playr = _unit->GetAIInterface()->GetNextTarget();

            _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You landlubbers are tougher than I thought. I'll have to improvise!");
            _unit->PlaySoundToSet(5778);
			_unit->CastSpell(Playr, smite_stomp, true);
            phase++;
			move = 1;
			position = 1;
            RegisterAIUpdateEvent(500);
        }
        else if(_unit->GetHealthPct() <= 33 && phase == 1)
        {
			Playr = _unit->GetAIInterface()->GetNextTarget();

            _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "D'ah! Now you're making me angry!");
            _unit->PlaySoundToSet(5779);
			_unit->CastSpell(Playr, smite_stomp, true);
            phase++;
			move = 1;
			position = 1;
            RegisterAIUpdateEvent(500);
        }
    }

    void OnCombatStop(Unit * mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        RemoveAIUpdateEvent();
        phase = 0;
		looting = 0;
		move = 0;
		position = 0;
        weapons();
    }

    void OnDied(Unit * mKiller)
    {
        phase = 0;
		looting = 0;
		move = 0;
		position = 0;
        RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if(looting == 1)
		{
			looting = 0;
			weapons();
			_unit->Emote(EMOTE_ONESHOT_NONE);
			_unit->SetStandState(0);
			_unit->GetAIInterface()->SetAIState(STATE_ATTACKING);
			_unit->Unroot();
		}

		if(_unit->GetPositionX() == 1.100060f && _unit->GetPositionY() == -780.026367f && _unit->GetPositionZ() == 9.811194f && position == 1)
		{
	        _unit->Emote(EMOTE_ONESHOT_KNEEL);
			_unit->SetStandState(8);
			looting = 1;
			position = 0;
			ModifyAIUpdateEvent(7000);
		}

		if(move == 1)
		{
			move = 0;
			movetochest();
		}
    }

	void movetochest()
	{	
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->MoveTo(1.100060f, -780.026367f, 9.811194f, 5.452218f);
	}

    void weapons()
    {	
		
        switch(phase)
        {
        case 0:
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 7420);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 33490690);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 768);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 0);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 0);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 0);
			_unit->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, 1483);   // 1483 is taken from NCDB creature_proto
            break;
        case 1:
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 7419);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 33490690);	
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 781);
			_unit->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, _unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME) / 2);
            break;
        case 2:
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 19610);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 33488898);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 0);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 0);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 0);
            _unit->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 0);
			_unit->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, _unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME) * 2);
			_unit->CastSpell(_unit, smite_buff, true);
        }
        RemoveAIUpdateEvent();
    }

protected:
	SpellEntry * smite_stomp;
	SpellEntry * smite_buff;
	Unit * Playr;
    int phase , move, looting, position;
};

class RLever : public GameObjectAIScript // Rahk' Lever
{
public:
	RLever(GameObject*  goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject*  GO) { return new RLever(GO); }

	void OnActivate(Player* pPlayer)
	{
		GameObject* RDoor = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-190.860092f, -456.332184f, 54.496822f, 13965);
		if(RDoor->GetUInt32Value(GAMEOBJECT_STATE) == 0)
		{
			RDoor->SetUInt32Value(GAMEOBJECT_STATE, 1);
		}
		else
		{
			RDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		}
	}
};

class FactoryLever : public GameObjectAIScript
{
public:
	FactoryLever(GameObject*  goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject*  GO) { return new FactoryLever(GO); }

	void OnActivate(Player* pPlayer)
	{
		GameObject* FADoor = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-289.691650f, -535.988953f, 49.440678f, 16400);
		if(FADoor->GetUInt32Value(GAMEOBJECT_STATE) == 0)
		{
			FADoor->SetUInt32Value(GAMEOBJECT_STATE, 1);
		}
		else
		{
			FADoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		}
	}
};

class FoundryLever : public GameObjectAIScript
{
public:
	FoundryLever(GameObject*  goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject*  GO) { return new FoundryLever(GO); }

	void OnActivate(Player* pPlayer)
	{
		GameObject* FODoor = _gameobject->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-176.569f, -577.640991f, 19.311600f, 16399);
		if(FODoor->GetUInt32Value(GAMEOBJECT_STATE) == 0)
		{
			FODoor->SetUInt32Value(GAMEOBJECT_STATE, 1);
		}
		else
		{
			FODoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		}
	}
};



/*class DefiasCannon : public GameObjectAIScript
{
public:
	DefiasCannon(GameObject*  goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject*  GO) { return new DefiasCannon(GO); }


	void OnActivate(Player*  pPlayer)
	{

		if(pPlayer->GetItemInterface()->GetItemCount(5397, false))
		{
		pPlayer->GetItemInterface()->RemoveItemAmt(5397, 1);
		Creature* Smite = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-22.8471f, -797.283f, 20.3745f, 646);
		Creature* Pirate1 = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-96.339203f, -721.271973f, 8.455710f, 657);
		GameObject* IDoor = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-100.358414f, -665.665161f, 7.420489f, 16397);
		GameObject* Cannon = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-107.130325f, -656.549622f, 7.422509f, 16398);

		IDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
		Cannon->Despawn(1000);

		if(Smite != nullptr)
		{
			//Smite->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You there, check out that noise!");
			pPlayer->PlaySoundToSet(5775);
		}
			if(Pirate1 != nullptr)
			{
				Pirate1->GetAIInterface()->MoveTo(-96.122841f, -675.890259f, 7.414724f, 1.926761f);
			}
		}
	}
}; */

// cookie
#define acid_splash 6306
#define cookie_cooking 5174

class CookieAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CookieAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    CookieAI(Creature* pCreature) : CreatureAIScript(pCreature) 
	{
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(acid_splash);
		spells[0].targettype = TARGET_ATTACKING;	
		spells[0].instant = true;
		spells[0].cooldown = 6;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 100;

		heal = dbcSpell.LookupEntry(cookie_cooking);
	}

	void OnCombatStart(Unit* mTarget) 
	{
		RegisterAIUpdateEvent(1500);
	}

	void OnCombatStop(Unit* mTarget) 
	{
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        RemoveAIUpdateEvent();
	}

	void OnDied(Unit *mKiller) 
	{
        RemoveAIUpdateEvent();
	}

	void AIUpdate() 
	{
		healcount++;
		timer = 13 + RandomUInt(3);

		if(healcount >= timer && _unit->GetCurrentSpell() == nullptr && _unit->GetAIInterface()->GetNextTarget())
		{
			if(_unit->GetHealthPct() <= 90)
			{
				_unit->CastSpell(_unit, heal, false);
			}
			healcount = 0;
		}
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
	}

    void Destroy()
    {
        delete (CookieAI*)this;
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == nullptr && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = nullptr;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
					case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
							break;
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
protected:
	int nrspells,healcount;
	int timer;
		SpellEntry * heal;
};

#define BLACKGUARD 636
class Blackguard : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Blackguard, MoonScriptCreatureAI);
    Blackguard(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(1785, Target_Self, 100, 0, 30);
	}
};

#define EVOKER 1729
class Evoker : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Evoker, MoonScriptCreatureAI);
    Evoker(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		Frost = AddSpell(12544, Target_Self, 100, 0, 1800);
		AddSpell(4979, Target_Self, 8, 1.5, 30);
		AddSpell(11829, Target_RandomDestination, 15, 3, 18);
	}
		void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(Frost);
		ParentClass::OnCombatStart(pTarget);
	}
protected:
SpellDesc* Frost;
};

#define MINER 598
class Miner : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Miner, MoonScriptCreatureAI);
    Miner(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6016, Target_Current, 10, 0, 0);
		_unit->setEmoteState(EMOTE_STATE_WORK);
	}
};

#define OVERSEER 634
class Overseer : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Overseer, MoonScriptCreatureAI);
    Overseer(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(5115, Target_Self, 5, 0, 30);
	}
};

#define SQUALLSHAPER 1732
class Squallshaper : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Squallshaper, MoonScriptCreatureAI);
    Squallshaper(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		Frost = AddSpell(12544, Target_Self, 100, 0, 1800);
		AddSpell(122, Target_Self, 8, 0, 15);
		AddSpell(2138, Target_Current, 12, 0, 8);

	}
			void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(Frost);
		ParentClass::OnCombatStart(pTarget);
	}
protected:
SpellDesc* Frost;
};

#define STRIP_MINER 4416
class Stripminer : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Stripminer, MoonScriptCreatureAI);
    Stripminer(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6016, Target_Current, 10, 0, 30);
		_unit->setEmoteState(EMOTE_STATE_WORK);
	}
};

#define TASKMASTER 4417
class Taskmaster : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Taskmaster, MoonScriptCreatureAI);
    Taskmaster(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6685, Target_Current, 100, 1, 30);
		AddSpell(6660, Target_Current, 20, 1, 5);
	}
};

#define WATCHMAN 1725
class Watchman : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Watchman, MoonScriptCreatureAI);
    Watchman(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6660, Target_Current, 100, 1, 3);
	}
};


#define WIZARD 4418
class Wizard : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Wizard, MoonScriptCreatureAI);
    Wizard(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(113, Target_RandomPlayer, 10, 1.5, 18);
		AddSpell(9053, Target_Current, 20, 3, 5);
		AddSpell(4979, Target_Self, 5, 1.5, 30);
		}
};

#define CRAFTSMAN 1731
class Craftsman : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Craftsman, MoonScriptCreatureAI);
    Craftsman(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(5159, Target_Current, 15, 1.5, 10);
		}
};


#define ENGINEER 622
class Engineer : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Engineer, MoonScriptCreatureAI);
    Engineer(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		AddSpell(6660, Target_Current, 15, 1, 6);
		AddSpell(3605, Target_Self, 6, 3, 60);
	}
};


#define WOODCARVER 641
class Woodcarver : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Woodcarver, MoonScriptCreatureAI);
    Woodcarver(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
		Throw = AddSpell(6466, Target_Current, 100, 1.0f, 60);
		AddSpell(15496, Target_Current, 12, 0, 6);

	}
			void OnCombatStart(Unit* pTarget)
	{
		CastSpellNowNoScheduling(Throw);
		ParentClass::OnCombatStart(pTarget);
	}
protected:
SpellDesc* Throw;
};

#define GOLEM 2520
class Golem : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(Golem, MoonScriptCreatureAI);
    Golem(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{}
	void OnCombatStart(Unit* pTarget)
	{
		AggroNearestPlayer(100);
		ParentClass::OnCombatStart(pTarget);
	}
	void OnCombatStop(Unit* mTarget)
	{
		_unit->Despawn(1000, 0);
	}
};


void SetupDeadmines(ScriptMgr * mgr)
{
	mgr->register_creature_script(645, &CookieAI::Create);
	mgr->register_creature_script(643, &SneedAI::Create);
    mgr->register_creature_script(644, &RhahkZorAI::Create);
    mgr->register_creature_script(642, &SneedsShredderAI::Create);
    mgr->register_creature_script(1763, &GilnidAI::Create);
    mgr->register_creature_script(639, &VanCleefAI::Create);
    mgr->register_creature_script(646, &MrSmiteAI::Create);
	//mgr->register_gameobject_script(16398, &DefiasCannon::Create);
	mgr->register_gameobject_script(101831, &RLever::Create);
	mgr->register_gameobject_script(101832, &FactoryLever::Create);
	mgr->register_gameobject_script(101834, &FoundryLever::Create);
	mgr->register_creature_script(BLACKGUARD, &Blackguard::Create);
	mgr->register_creature_script(EVOKER, &Evoker::Create);
	mgr->register_creature_script(MINER, &Miner::Create);
	mgr->register_creature_script(OVERSEER, &Overseer::Create);
	mgr->register_creature_script(SQUALLSHAPER, &Squallshaper::Create);
	mgr->register_creature_script(STRIP_MINER, &Stripminer::Create);
	mgr->register_creature_script(TASKMASTER, &Taskmaster::Create);
	mgr->register_creature_script(WATCHMAN, &Watchman::Create);
	mgr->register_creature_script(WIZARD, &Wizard::Create);
	mgr->register_creature_script(CRAFTSMAN, &Craftsman::Create);
	mgr->register_creature_script(ENGINEER, &Engineer::Create);
	mgr->register_creature_script(WOODCARVER, &Woodcarver::Create);
	mgr->register_creature_script(GOLEM, &Golem::Create);
}
