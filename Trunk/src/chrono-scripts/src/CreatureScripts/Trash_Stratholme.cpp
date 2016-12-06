//Stratholme Trash
//Forsaken Gaming <http://www.forsakengaming.com/>
//Copyright goes to Light and Forsaken Gaming.
//Developed by Light / Joe
//Edited and reviewed by Swater

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"

//Trash

//Spectral Citizen
#define CN_SPECTRAL_CITIZEN 10384
class SpectralCitizenAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpectralCitizenAI, MoonScriptCreatureAI);
	SpectralCitizenAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Debilitating Touch
		AddSpell(16333, Target_RandomPlayer, 30, 0, 0);
		// Haunting Phantoms
		AddSpell(16336, Target_RandomPlayer, 25, 2, 0);
		// Slap!
		AddSpell(6754, Target_Current, 35, 0, 5);
	}
};
 
//Aurius
#define CN_AURIUS 10917
class AuriusAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AuriusAI, MoonScriptCreatureAI);
	AuriusAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Crusader Strike
		AddSpell(14517, Target_Current, 50, 0, 0);
		// Holy Light
		AddSpell(13952, Target_Self, 30, 2.5, 0);
		// Exorcism
		AddSpell(17149, Target_Current, 20, 0, 0);
		//  Divine Shield
		AddSpell(13874, Target_Current, 10, 0, 60);

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
	}
	void AIUpdate() {
		if(IsTimerFinished(timer)) {
			ResetTimer(timer, 15000);
			SpawnCreature(CN_BILE_SLIME);
		}
		ParentClass::AIUpdate();
	}
	void OnDied(Unit *pKiller) {
		for(int i = 0; i<4; i++) {
			SpawnCreature(CN_BILE_SLIME);
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
};

//Broken Cadaver
#define CN_BROKEN_CADAVER 10383
class BrokenCadaverAI : public MoonScriptCreatureAI
{
	SpellDesc *explode;
	int timer;
	MOONSCRIPT_FACTORY_FUNCTION(BrokenCadaverAI, MoonScriptCreatureAI);
	BrokenCadaverAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		explode = AddSpell(16141, Target_Self, 0, 0, 0);
		timer = AddTimer(5000);
	}

	void AIUpdate() {
		if(IsTimerFinished(timer)) {
			CastSpell(explode);
			RemoveTimer(timer);
			timer = INVALIDATE_TIMER;
		}
		ParentClass::AIUpdate();
	}
};

//Crimson Battle Mage
#define CN_CRIMSON_BATTLE_MAGE 10425
class CrimsonBattleMageAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CrimsonBattleMageAI, MoonScriptCreatureAI);
	CrimsonBattleMageAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		//Arcane Explosion
		AddSpell(15253, Target_Self, 40, 1.5, 0);
		//Blast wave
		AddSpell(17145, Target_Self, 30, 0, 15);
	}
};

//Crimson Conjuror
#define CN_CRIMSON_CONJUROR 10419
class CrimsonConjurorAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(CrimsonConjurorAI, MoonScriptCreatureAI);
	CrimsonConjurorAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Frost nova
		AddSpell(12674, Target_Self, 30, 0, 0);
		// Frostbolt
		AddSpell(12675, Target_Current, 50, 2.5, 0);
		// Scorch
		AddSpell(17195, Target_Current, 50, 1.5, 0);
		// Elemental
		CastSpell(AddSpell(17162, Target_Self, 0, 2, 0));
	}
};

//Crimson Defender
#define CN_CRIMSON_DEFENDER 10421
class CrimsonDefenderAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(CrimsonDefenderAI, MoonScriptCreatureAI);
	CrimsonDefenderAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Devotion Aura
		ApplyAura(8258);
		// Divine Shield
		AddSpell(13874, Target_Self, 15, 0, 60);
		// Exorcism
		AddSpell(17149, Target_Current, 50, 1.5, 0);
		// Hammer of justice
		AddSpell(13005, Target_RandomPlayer, 40, 0, 10);
		// Chains of Ice
		AddSpell(15493, Target_Self, 20, 2.5, 0);
	}
};

//Crimson Gallant
#define CN_CRIMSON_GALLANT 10424
class CrimsonGallantAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(CrimsonGallantAI, MoonScriptCreatureAI);
	CrimsonGallantAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Crusader strike
		AddSpell(14518, Target_Current, 50, 0, 0);
		// Holy Strike
		AddSpell(17143, Target_Self, 30, 0, 0);
		// Retribution Aura
		ApplyAura(8990);
	}
};

//Crimson Guardsman
#define CN_CRIMSON_GUARDSMAN 10418
class CrimsonGuardsmanAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(CrimsonGuardsmanAI, MoonScriptCreatureAI);
	CrimsonGuardsmanAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Disarm
		AddSpell(6713, Target_Current, 30, 0, 15);
		// Shield bash
		AddSpell(11972, Target_Current, 40, 0, 0);
		// Shield Charge
		AddSpell(15749, Target_RandomPlayer, 20, 0, 5);
	}
};

//Crimson Initiate
#define CN_CRIMSON_INITIATE 10420
class CrimsonIniniateAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(CrimsonIniniateAI, MoonScriptCreatureAI);
	CrimsonIniniateAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Flash heal
		AddSpell(17138, Target_WoundedFriendly, 30, 2, 10);
		// Mind Blast
		AddSpell(17194, Target_Current, 50, 2, 0);
		// Renew
		AddSpell(8362, Target_WoundedFriendly, 20, 2, 10);
	}
};

//Crimson Monk
#define CN_CRIMSON_MONK 11043
class CrimsonMonkAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(CrimsonMonkAI, MoonScriptCreatureAI);
	CrimsonMonkAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Kick
		AddSpell(11978, Target_Current, 20, 0, 0);
		// Trash
		AddSpell(3391, Target_Self, 40, 0, 10);
	}
};

//Crimson Priest
#define CN_CRIMSON_PRIEST 10423
class CrimsonPriestAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(CrimsonPriestAI, MoonScriptCreatureAI);
	CrimsonPriestAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Heal
		AddSpell(15586, Target_WoundedFriendly, 30, 3.5, 5);
		// Holy fire
		AddSpell(17142, Target_Current, 90, 5, 6);
		// Holy Smite
		AddSpell(15238, Target_Current, 60, 2.5, 0);
	}
};



//Crimson Rifleman
#define CN_CRIMSON_RIFLEMAN 11054
class CrimsonRiflemanAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(CrimsonRiflemanAI, MoonScriptCreatureAI);
	CrimsonRiflemanAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Shoot
		AddSpell(17353, Target_Current, 75, 0, 1);
	}
};

//Crimson Sorcerer
#define CN_CRIMSON_SORCERER 10422
class CrimsonSorcererAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(CrimsonSorcererAI, MoonScriptCreatureAI);
	CrimsonSorcererAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Arcane Bolt
		AddSpell(15230, Target_Current, 50, 2, 0);
		// Fire Blast
		AddSpell(14145, Target_RandomPlayer, 50, 0, 15);
		// Polymorph
		AddSpell(13323, Target_RandomPlayer, 30, 2, 8);
	}
};

//Crypt Beast
#define CN_CRYPT_BEAST 10413
class CryptBeastAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(CryptBeastAI, MoonScriptCreatureAI);
	CryptBeastAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Fireball
		AddSpell(4962, Target_RandomPlayer, 75, 2, 5);
	}
};

//Crypt Crawler
#define CN_CRYPT_CRAWLER 10412
class CryptCrawlerAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(CryptCrawlerAI, MoonScriptCreatureAI);
	CryptCrawlerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Crypt  Scarabs
		AddSpell(31601, Target_Current, 40, 1.5, 0);
		// Enveloping Web
		AddSpell(15471, Target_RandomPlayer, 70, 2, 15);
	}
};

//Rockwing Gargoyle
#define CN_ROCKING_GARGOYLE 10408
class RockwingGargoyleAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RockwingGargoyleAI, MoonScriptCreatureAI);
	RockwingGargoyleAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Sunder Armor
		AddSpell(13444, Target_Current, 50, 0, 3);
	}
};

//Rockwing Screecher
#define CN_ROCKWING_SCREECHER 10409
class RockingScreecherAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RockingScreecherAI, MoonScriptCreatureAI);
	RockingScreecherAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Demoralizing Shout
		AddSpell(13730, Target_Current, 90, 0, 30);
		// Gargoyle Strike
		AddSpell(16997, Target_Current, 50, 1.5, 0);
		// Terrifying howl
		AddSpell(8715, Target_RandomPlayer, 75, 0, 30);
	}
};

//Eye of Naxxramas
#define CN_EYE_OF_NAXXRAMAS 10411
class EyeOfNaxxramasAI : public MoonScriptCreatureAI
{	
	int timer;
	MOONSCRIPT_FACTORY_FUNCTION(EyeOfNaxxramasAI, MoonScriptCreatureAI);
	EyeOfNaxxramasAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		//  Greater invisibility
		ApplyAura(16380);
	}

	//after 5 secs spawn 2 gargoyles
	void OnCombatStart(Unit *pTarget) {
		timer = AddTimer(5000);
		ParentClass::OnCombatStart(pTarget);
	}
	void AIUpdate() {
		if(IsTimerFinished(timer)) {
			SpawnCreature(CN_ROCKWING_SCREECHER);
			SpawnCreature(CN_ROCKWING_SCREECHER);
		}
		ParentClass::AIUpdate();
	}
};
//Fleshflayer Ghoul
#define CN_FLESHFLAYER_GHOUL 10407
class FleshflayerGhoulAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(FleshflayerGhoulAI, MoonScriptCreatureAI);
	FleshflayerGhoulAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Rend
		AddSpell(13738, Target_Current, 50, 0, 5);
	}
};

//Ghostly Citizen
#define CN_GHOSTLY_CITIZEN 10385
class GhostlyCitizenAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GhostlyCitizenAI, MoonScriptCreatureAI);
	GhostlyCitizenAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Veil of Shadow
		AddSpell(7068, Target_RandomPlayer, 40, 0, 5);
		// Haunting Phantoms
		AddSpell(16336, Target_RandomPlayer, 30, 2, 0);
		// Slap!
		AddSpell(6754, Target_Current, 50, 0, 5);
	}
};

//Ghoul Ravener
#define CN_GHOUL_RAVENER 10406
class GhoulRavenerAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(GhoulRavenerAI, MoonScriptCreatureAI);
	GhoulRavenerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Head Crack
		AddSpell(16172, Target_Current, 30, 0, 4);
		// Ravenous Claw
		AddSpell(16553, Target_Current, 15, 0, 3);
 	}
};

// Mangled Cadaver
#define CN_MANGLED_CADAVER 10382
class MangledCadaverAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(MangledCadaverAI, MoonScriptCreatureAI);
	MangledCadaverAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Cadaver Worms
		AddSpell(16143, Target_Current, 50, 0, 5);
	}
};

//PatchWork Horror
#define CN_PATCHWORK_HORROR 10406
class PatchworkHorrorAI : public MoonScriptCreatureAI
{	
	MOONSCRIPT_FACTORY_FUNCTION(PatchworkHorrorAI, MoonScriptCreatureAI);
	PatchworkHorrorAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Disease Cloud
		ApplyAura(10414);
		// Knock Away
		AddSpell(10101, Target_Current, 50, 1, 10);

 	}
};
	
//Plague Ghoul
#define CN_PLAGUE_GHOUL 10405
class PlagueGhoulAI : public MoonScriptCreatureAI
{
	SpellDesc *Enrage;
	bool do_Enrage;

	MOONSCRIPT_FACTORY_FUNCTION(PlagueGhoulAI, MoonScriptCreatureAI);
	PlagueGhoulAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		do_Enrage = false;
		Enrage = AddSpell(8599, Target_Self, 0, 0, 0);
		// Cleave
		AddSpell(40505, Target_Current, 30, 0, 0);
		// Ghoul Plagues
		AddSpell(16458, Target_RandomPlayer, 40, 0, 5);
	}

	void AIUpdate() {
		if(!do_Enrage && (GetHealthPercent() < 20.0)) {
			do_Enrage = true;
			CastSpell(Enrage);
		}
		ParentClass::AIUpdate();
	}
};

//Ravaged Cadaver
#define CN_RAVAGED_CADAVER 10381
class RavagedCadaverAI : public MoonScriptCreatureAI
{
	SpellDesc *summon;
	MOONSCRIPT_FACTORY_FUNCTION(RavagedCadaverAI, MoonScriptCreatureAI);
	RavagedCadaverAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Strike
		AddSpell(13446, Target_Current, 50, 0, 0);
		// Summon Broken Cadaver
		summon = AddSpell(16324, Target_Self, 0, 0, 0);
	}

	void OnDied(Unit *pKiller) {
		CastSpellNowNoScheduling(summon);
		ParentClass::OnDied(pKiller);
	}
};
//Shrieking Banshee
#define CN_SHRIEKING_BANSHEE 10463
class ShriekingBansheeAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShriekingBansheeAI, MoonScriptCreatureAI);
	ShriekingBansheeAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Banshee Wail
		AddSpell(16868, Target_Current, 50, 1.5, 0);
		// Deafening Screech
		AddSpell(3589, Target_Current, 30, 0, 5);
	}
};

//Skeletal Berserker
#define CN_SKELETAL_BERSERKER 10391
class SkeletalBerserkerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SkeletalBerserkerAI, MoonScriptCreatureAI);
	SkeletalBerserkerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Cleave
		AddSpell(15496, Target_Current, 35, 0, 0);
		// Demoloralizing shout
		AddSpell(13730, Target_Self, 90, 0, 30);
		// Hamstring
		AddSpell(9080, Target_Current, 40, 0, 5);
	}
};

//Skeletal Guardian
#define CN_SKELETAL_GUARDIAN 10390
class SkeletalGuardianAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SkeletalGuardianAI, MoonScriptCreatureAI);
	SkeletalGuardianAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Frostbolt
		AddSpell(9672, Target_RandomPlayer, 40, 3, 0);
		// Shadowbolt
		AddSpell(9613, Target_Current, 70, 3, 0);
		// Arcane Bolt
		AddSpell(37361, Target_RandomPlayer, 40, 2, 0);
		// Demon Armor
		ApplyAura(13787);
	}
};

//Thuzadin Necromancer
#define CN_THUZADIN_NECROMANCER 10400
class ThuzadinNecromancerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ThuzadinNecromancerAI, MoonScriptCreatureAI);
	ThuzadinNecromancerAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Bone Armor
		ApplyAura(16431);
		// Soul Tap
		AddSpell(16430, Target_Current, 50, 2, 2);
		// Summon Skeletal
		AddSpell(12420, Target_Self, 20, 3, 5);
	}
};

//Thuzadin Shadowcaster
#define CN_THUZADIN_SHADOWCASTER 10398
class ThuzadinShadowcasterAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ThuzadinShadowcasterAI, MoonScriptCreatureAI);
	ThuzadinShadowcasterAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Cripple
		AddSpell(11443, Target_RandomPlayer, 10, 3, 0);
		// Piercing shadow
		AddSpell(16429, Target_RandomPlayer, 20, 2, 0);
		// Shadowbolt
		AddSpell(15232, Target_Current, 80, 3, 0);
	}
};

//Undead Postman
#define CN_UNDEAD_POSTMAN 11142
class UndeadPostmanAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(UndeadPostmanAI, MoonScriptCreatureAI);
	UndeadPostmanAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Call of the Grave
		AddSpell(5137, Target_RandomPlayer, 30, 3, 0);
		// Chains of Ice
		AddSpell(7713, Target_RandomPlayer, 42, 2, 0);
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
};

//Wailing Banshee
#define CN_WAILING_BANSHEE 10464
class WailingBansheeAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WailingBansheeAI, MoonScriptCreatureAI);
	WailingBansheeAI(Creature* pCreature):MoonScriptCreatureAI(pCreature)
	{
		// Anti-magic shield
		AddSpell(1964, Target_Self, 70, 2, 30);
		// Banshee Curse
		AddSpell(17105, Target_RandomPlayer, 25, 0, 0);

	}
};

void SetupStratholmeTrash(ScriptMgr * mgr)
{
	//TRASH
	mgr->register_creature_script(CN_SPECTRAL_CITIZEN, &SpectralCitizenAI::Create);
	mgr->register_creature_script(CN_AURIUS, &AuriusAI::Create);
	mgr->register_creature_script(CN_BILE_SPEWER, &BileSpewerAI::Create);
	mgr->register_creature_script(CN_BLACK_GUARD_SENTRY, &BlackGuardSentryAI::Create);
	mgr->register_creature_script(CN_BROKEN_CADAVER, &BrokenCadaverAI::Create);
	mgr->register_creature_script(CN_CRIMSON_BATTLE_MAGE, &CrimsonBattleMageAI::Create);
	mgr->register_creature_script(CN_CRIMSON_CONJUROR, &CrimsonConjurorAI::Create);
	mgr->register_creature_script(CN_CRIMSON_DEFENDER, &CrimsonDefenderAI::Create);
	mgr->register_creature_script(CN_CRIMSON_GALLANT, &CrimsonGallantAI::Create);
	mgr->register_creature_script(CN_CRIMSON_GUARDSMAN, &CrimsonGuardsmanAI::Create);
	mgr->register_creature_script(CN_CRIMSON_INITIATE, &CrimsonIniniateAI::Create);
	mgr->register_creature_script(CN_CRIMSON_MONK, &CrimsonMonkAI::Create);
	mgr->register_creature_script(CN_CRIMSON_PRIEST, &CrimsonPriestAI::Create);
	mgr->register_creature_script(CN_CRIMSON_RIFLEMAN, &CrimsonRiflemanAI::Create);
	mgr->register_creature_script(CN_CRIMSON_SORCERER, &CrimsonSorcererAI::Create);
	mgr->register_creature_script(CN_CRYPT_BEAST, &CryptBeastAI::Create);
	mgr->register_creature_script(CN_CRYPT_CRAWLER, &CryptCrawlerAI::Create);
	mgr->register_creature_script(CN_EYE_OF_NAXXRAMAS, &EyeOfNaxxramasAI::Create);
	mgr->register_creature_script(CN_FLESHFLAYER_GHOUL, &FleshflayerGhoulAI::Create);
	mgr->register_creature_script(CN_GHOSTLY_CITIZEN, &GhostlyCitizenAI::Create);
	mgr->register_creature_script(CN_GHOUL_RAVENER, &GhoulRavenerAI::Create);
	mgr->register_creature_script(CN_MANGLED_CADAVER, &MangledCadaverAI::Create);
	mgr->register_creature_script(CN_PATCHWORK_HORROR, &PatchworkHorrorAI::Create);
	mgr->register_creature_script(CN_PLAGUE_GHOUL, &PlagueGhoulAI::Create);
	mgr->register_creature_script(CN_RAVAGED_CADAVER, &RavagedCadaverAI::Create);
	mgr->register_creature_script(CN_ROCKING_GARGOYLE, &RockwingGargoyleAI::Create);
	mgr->register_creature_script(CN_ROCKWING_SCREECHER, &RockingScreecherAI::Create);
	mgr->register_creature_script(CN_SHRIEKING_BANSHEE, &ShriekingBansheeAI::Create);
	mgr->register_creature_script(CN_SKELETAL_BERSERKER, &SkeletalBerserkerAI::Create);
	mgr->register_creature_script(CN_SKELETAL_GUARDIAN, &SkeletalGuardianAI::Create);
	mgr->register_creature_script(CN_THUZADIN_NECROMANCER, &ThuzadinNecromancerAI::Create);
	mgr->register_creature_script(CN_THUZADIN_SHADOWCASTER, &ThuzadinShadowcasterAI::Create);
	mgr->register_creature_script(CN_UNDEAD_POSTMAN, &UndeadPostmanAI::Create);
	mgr->register_creature_script(CN_VENGEFUL_PHANTOM, &VengefulPhantomAI::Create);
	mgr->register_creature_script(CN_VENOM_BELCHER, &VenomBelcherAI::Create);
	mgr->register_creature_script(CN_WAILING_BANSHEE, &WailingBansheeAI::Create);
}