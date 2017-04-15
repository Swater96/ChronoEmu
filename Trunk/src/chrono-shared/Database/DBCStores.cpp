/*
 * Chrono Emulator
 * Copyright (C) 2010 ChronoEmu Team <http://www.forsakengaming.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "DBCStores.h"
#include "DataStore.h"
#include "NGLog.h"

SERVER_DECL DBCStorage<AreaTriggerEntry> dbcAreaTrigger;
SERVER_DECL DBCStorage<WorldSafeLocs> dbcworldsafelocs;
SERVER_DECL DBCStorage<ItemSetEntry> dbcItemSet;
SERVER_DECL DBCStorage<Lock> dbcLock;
SERVER_DECL DBCStorage<SpellEntry> dbcSpell;
SERVER_DECL DBCStorage<SpellDuration> dbcSpellDuration;
SERVER_DECL DBCStorage<SpellRange> dbcSpellRange;
SERVER_DECL DBCStorage<emoteentry> dbcEmoteEntry;
SERVER_DECL DBCStorage<SpellRadius> dbcSpellRadius;
SERVER_DECL DBCStorage<SpellCastTime> dbcSpellCastTime;
SERVER_DECL DBCStorage<AreaTable> dbcArea;
SERVER_DECL DBCStorage<FactionTemplateDBC> dbcFactionTemplate;
SERVER_DECL DBCStorage<FactionDBC> dbcFaction;
SERVER_DECL DBCStorage<EnchantEntry> dbcEnchant;
SERVER_DECL DBCStorage<RandomProps> dbcRandomProps;
SERVER_DECL DBCStorage<skilllinespell> dbcSkillLineSpell;
SERVER_DECL DBCStorage<skilllineentry> dbcSkillLine;
SERVER_DECL DBCStorage<DBCTaxiNode> dbcTaxiNode;
SERVER_DECL DBCStorage<DBCTaxiPath> dbcTaxiPath;
SERVER_DECL DBCStorage<DBCTaxiPathNode> dbcTaxiPathNode;
SERVER_DECL DBCStorage<AuctionHouseDBC> dbcAuctionHouse;
SERVER_DECL DBCStorage<TalentEntry> dbcTalent;
SERVER_DECL DBCStorage<TalentTabEntry> dbcTalentTab;
SERVER_DECL DBCStorage<CreatureSpellDataEntry> dbcCreatureSpellData;
SERVER_DECL DBCStorage<CreatureFamilyEntry> dbcCreatureFamily;
SERVER_DECL DBCStorage<CharClassEntry> dbcCharClass;
SERVER_DECL DBCStorage<CharRaceEntry> dbcCharRace;
SERVER_DECL DBCStorage<MapEntry> dbcMap;
SERVER_DECL DBCStorage<ChatChannelDBC> dbcChatChannels;
SERVER_DECL DBCStorage<DurabilityQualityEntry> dbcDurabilityQuality;
SERVER_DECL DBCStorage<DurabilityCostsEntry> dbcDurabilityCosts;
SERVER_DECL DBCStorage<BankSlotPrice> dbcBankSlotPrices;
SERVER_DECL DBCStorage<BankSlotPrice> dbcStableSlotPrices;
SERVER_DECL DBCStorage<WorldMapOverlayEntry> dbcWorldMapOverlay;
SERVER_DECL DBCStorage<WMOAreaTableEntry> dbcWMOArea;

const char* AreaTriggerFormat = "uuffffffff";
const char* ItemSetFormat = "usxxxxxxxxuuuuuuuuuxxxxxxxxuuuuuuuuuuuuuuuuuu";
const char* LockFormat = "uuuuuuxxxuuuuuxxxuuuuuxxxxxxxxxxx";
const char* EmoteEntryFormat = "uxuuuuxuxuxxxxxxxxx";
const char* skilllinespellFormat = "uuuxxxxxuuuuxxu";
const char* EnchantEntrYFormat = "uuuuuuuuuuuuusxxxxxxxxuu";
const char* skilllineentrYFormat = "uuusxxxxxxxxxxxxxxxxxx";
const char* WMOAreaTableFormat = "niiixxxxxiixxxxxxxxx";
const char* spellentryFormat = 
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"i"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"u"
"f"
"u"
"u"
"uu"
"uuuuuuuu"
"uuuuuuuu"
"i"
"u"
"u"
"uuu"
"uuu"
"uuu"
"fff"
"fff"
"iii"
"iii"
"uuu"
"uuu"
"uuu"
"uuu"
"uuu"
"fff"
"uuu"
"uuu"
"uuu"
"uuu"
"fff"
"u"
"u"
"u"
"u"
"u"
"s"
"x"
"x"
"x"
"x"
"x"
"x"
"x"
"x"
"s"
"x"
"x"
"x"
"x"
"x"
"x"
"x"
"x"
"s"
"x"
"x"
"x"
"x"
"x"
"x"
"x"
"x"
"s"
"x"
"x"
"x"
"x"
"x"
"x"
"x"
"x"
"u"
"u"
"u"
"u"
"u"
"uu"
"u"
"u"
"u"
"i"
"fff"
"u"
"u"
"u";

const char* talententryFormat = "uuuuuuuuuxxxxuxxuxxxu";
const char* talenttabentryFormat = "uxxxxxxxxxxxuux";
const char* spellcasttimeFormat = "uuxx";
const char* spellradiusFormat = "ufxf";
const char* spellrangeFormat = "uffxxxxxxxxxxxxxxxxxxx";
const char* WorldMapOverlayfmt="uxuxxxxxxxxxxxxxx";
const char* spelldurationFormat = "uuuu";
const char* randompropsFormat = "usuuuxxxxxxxxxxx";
const char* areatableFormat = "uuuuuxxxuxusxxxxxxxxuxxxx";
const char* factiontemplatedbcFormat = "uuuuuuuuuuuuuu";
const char* auctionhousedbcFormat = "uuuuxxxxxxxxx";
const char* factiondbcFormat = "uiuuuuxxxxiiiixxxxusxxxxxxxxxxxxxxxxx";

const char* dbctaxinodeFormat = "uufffxxxxxxxxxuu";
const char* dbctaxipathFormat = "uuuu";
const char* dbctaxipathnodeFormat = "uuuufffuu";
const char* creaturespelldataFormat = "uuuuuuuuu";
const char* charraceFormat =  "uxxxxxxxuxxxxusxxxxxxxxxxxxxx";
const char* charclassFormat = "uxxuxxxxxxxxxxsxx";
const char* creaturefamilyFormat = "ufufuuuuxxsxxxxxxx";
const char* mapentryFormat = "usuusxxxxxxxxxxxxxxusxxxxxxxxsxxxxxxxxuxxx";

const char* chatchannelformat = "uuxsxxxxxxxxxxxxxxxxx";
const char* durabilityqualityFormat = "uf";
const char* durabilitycostsFormat = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";
const char* bankslotpriceformat = "uu";
const char* spellshapeshiftformformat = "uxxxxxxxxxxuix";
const char* worldsafelocsfmt = "uufffxxxxxxxxx";

template<class T>
bool loader_stub(const char * filename, const char * format, bool ind, T& l, bool loadstrs)
{
	Log.Notice("DBC", "Loading %s.", filename);
	return l.Load(filename, format, ind, loadstrs);
}

#define LOAD_DBC(filename, format, ind, stor, strings) if(!loader_stub(filename, format, ind, stor, strings)) { return false; } 

bool LoadDBCs()
{
	LOAD_DBC("DBC/AreaTrigger.dbc", AreaTriggerFormat, true, dbcAreaTrigger, false);
	LOAD_DBC("DBC/WorldSafeLocs.dbc", worldsafelocsfmt, true, dbcworldsafelocs, true);
	LOAD_DBC("DBC/ItemSet.dbc", ItemSetFormat, true, dbcItemSet, true);
	LOAD_DBC("DBC/Lock.dbc", LockFormat, true, dbcLock, false);
	LOAD_DBC("DBC/EmotesText.dbc", EmoteEntryFormat, true, dbcEmoteEntry, false);
	LOAD_DBC("DBC/SkillLineAbility.dbc", skilllinespellFormat, false, dbcSkillLineSpell, false);
	LOAD_DBC("DBC/SpellItemEnchantment.dbc", EnchantEntrYFormat, true, dbcEnchant, true);
	LOAD_DBC("DBC/SkillLine.dbc", skilllineentrYFormat, true, dbcSkillLine, true);
	LOAD_DBC("DBC/Spell.dbc", spellentryFormat, true, dbcSpell, true);
	LOAD_DBC("DBC/Talent.dbc", talententryFormat, true, dbcTalent, false);
	LOAD_DBC("DBC/TalentTab.dbc", talenttabentryFormat, true, dbcTalentTab, false);
	LOAD_DBC("DBC/SpellCastTimes.dbc", spellcasttimeFormat, true, dbcSpellCastTime, false);
	LOAD_DBC("DBC/SpellRadius.dbc", spellradiusFormat, true, dbcSpellRadius, false);
	LOAD_DBC("DBC/SpellRange.dbc", spellrangeFormat, true, dbcSpellRange, false);
	LOAD_DBC("DBC/SpellDuration.dbc", spelldurationFormat, true, dbcSpellDuration, false);
	LOAD_DBC("DBC/ItemRandomProperties.dbc", randompropsFormat, true, dbcRandomProps, false);
	LOAD_DBC("DBC/AreaTable.dbc", areatableFormat, true, dbcArea, true);
	LOAD_DBC("DBC/FactionTemplate.dbc", factiontemplatedbcFormat, true, dbcFactionTemplate, false);
	LOAD_DBC("DBC/Faction.dbc", factiondbcFormat, true, dbcFaction, true);
	LOAD_DBC("DBC/TaxiNodes.dbc", dbctaxinodeFormat, false, dbcTaxiNode, false);
	LOAD_DBC("DBC/TaxiPath.dbc", dbctaxipathFormat, false, dbcTaxiPath, false);
	LOAD_DBC("DBC/TaxiPathNode.dbc", dbctaxipathnodeFormat, false, dbcTaxiPathNode, false);
	LOAD_DBC("DBC/CreatureSpellData.dbc", creaturespelldataFormat, true, dbcCreatureSpellData, false);
	LOAD_DBC("DBC/CreatureFamily.dbc", creaturefamilyFormat, true, dbcCreatureFamily, true);
	LOAD_DBC("DBC/ChrRaces.dbc", charraceFormat, true, dbcCharRace, true);
	LOAD_DBC("DBC/ChrClasses.dbc", charclassFormat, true, dbcCharClass, true);
	LOAD_DBC("DBC/Map.dbc", mapentryFormat, true, dbcMap, true);
	LOAD_DBC("DBC/AuctionHouse.dbc", auctionhousedbcFormat, true, dbcAuctionHouse, false);
	LOAD_DBC("DBC/ChatChannels.dbc", chatchannelformat, true, dbcChatChannels, true);
	LOAD_DBC("DBC/DurabilityQuality.dbc", durabilityqualityFormat, true, dbcDurabilityQuality, false);
	LOAD_DBC("DBC/DurabilityCosts.dbc", durabilitycostsFormat, true, dbcDurabilityCosts, false);
	LOAD_DBC("DBC/BankBagSlotPrices.dbc", bankslotpriceformat, true, dbcBankSlotPrices, false);
	LOAD_DBC("DBC/StableSlotPrices.dbc", bankslotpriceformat, true, dbcStableSlotPrices, false);
	LOAD_DBC("DBC/WorldMapOverlay.dbc", WorldMapOverlayfmt, true, dbcWorldMapOverlay, true);
	LOAD_DBC("DBC/WMOAreaTable.dbc", WMOAreaTableFormat, true, dbcWMOArea, true);
	return true;
}
