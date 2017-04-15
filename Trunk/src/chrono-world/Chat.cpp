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

#include "StdAfx.h"

initialiseSingleton( ChatHandler );
initialiseSingleton(CommandTableStorage);

ChatCommand * ChatHandler::getCommandTable()
{
	ASSERT(false);
	return 0;
}

ChatCommand * CommandTableStorage::GetSubCommandTable(const char * name)
{
	if(!strcmp(name, "modify"))
		return _modifyCommandTable;
	else if(!strcmp(name, "debug"))
		return _debugCommandTable;
	else if(!strcmp(name, "waypoint"))
		return _waypointCommandTable;
	else if(!strcmp(name, "gmTicket"))
		return _GMTicketCommandTable;
	else if(!strcmp(name, "gobject"))
		return _GameObjectCommandTable;
	else if(!strcmp(name, "battleground"))
		return _BattlegroundCommandTable;
	else if(!strcmp(name, "npc"))
		return _NPCCommandTable;
	else if(!strcmp(name, "cheat"))
		return _CheatCommandTable;
	else if(!strcmp(name, "account"))
		return _accountCommandTable;
	else if(!strcmp(name, "pet"))
		return _petCommandTable;
	else if(!strcmp(name, "recall"))
		return _recallCommandTable;
	else if(!strcmp(name, "honor"))
		return _honorCommandTable;
	else if(!strcmp(name, "guild"))
		return _GuildCommandTable;
	else if(!strcmp(name, "title"))
		return _TitleCommandTable;
	else if(!strcmp(name, "quest"))
		return _questCommandTable;
	return 0;
}

#define dupe_command_table(ct, dt) this->dt = (ChatCommand*)allocate_and_copy(sizeof(ct)/* / sizeof(ct[0])*/, ct)
CHRONO_INLINE void* allocate_and_copy(uint32 len, void * pointer)
{
	void * data = (void*)malloc(len);
	memcpy(data, pointer, len);
	return data;
}

void CommandTableStorage::Load()
{
	QueryResult * result = WorldDatabase.Query("SELECT * FROM command_overrides");
	if(!result) return;

	do 
	{
		const char * name = result->Fetch()[0].GetString();
		const char * level = result->Fetch()[1].GetString();
		Override(name, level);
	} while(result->NextRow());
	delete result;
}

void CommandTableStorage::Override(const char * command, const char * level)
{
	ASSERT(level[0] != '\0');
	char * cmd = strdup(command);

	// find the command we're talking about
	char * sp = strchr(cmd, ' ');
	const char * command_name = cmd;
	const char * subcommand_name = 0;

	if(sp != 0)
	{
		// we're dealing with a subcommand.
		*sp = 0;
		subcommand_name = sp + 1;
	}

	size_t len1 = strlen(command_name);
	size_t len2 = subcommand_name ? strlen(subcommand_name) : 0;

	// look for the command.
	ChatCommand * p = &_commandTable[0];
	while(p->Name != 0)
	{
		if(!strnicmp(p->Name, command_name, len1))
		{
			// this is the one we wanna modify
			if(!subcommand_name)
			{
				// no subcommand, we can change it.
				p->CommandGroup = level[0];
				printf("Changing command level of command `%s` to %c.\n", p->Name, level[0]);
			}
			else
			{
				// assume this is a subcommand, loop the second set.
				ChatCommand * p2 = p->ChildCommands;
				if(!p2)
				{
					printf("Invalid command specified for override: %s\n", command_name);
				}
				else
				{
					while(p2->Name != 0)
					{
						if(!strnicmp("*",subcommand_name,1))
						{
								p2->CommandGroup = level[0];
								printf("Changing command level of command (wildcard) `%s`:`%s` to %c.\n", p->Name, p2->Name, level[0]);
						}else{
							if(!strnicmp(p2->Name, subcommand_name, len2))
							{
								// change the level
								p2->CommandGroup = level[0];
								printf("Changing command level of command `%s`:`%s` to %c.\n", p->Name, p2->Name, level[0]);
								break;
							}
						}
						p2++;
					}
					if(p2->Name == 0)
					{
						if(strnicmp("*",subcommand_name,1)) //Hacky.. meh.. -DGM
						{
							printf("Invalid subcommand referenced: `%s` under `%s`.\n", subcommand_name, p->Name);
						}
						break;
					}
				}
			}
			break;
		}
		++p;
	}

	if(p->Name == 0)
	{
		printf("Invalid command referenced: `%s`\n", command_name);
	}

	free(cmd);
}

void CommandTableStorage::Dealloc()
{
	free( _modifyCommandTable );
	free( _debugCommandTable );
	free( _waypointCommandTable );
	free( _GMTicketCommandTable );
	free( _GameObjectCommandTable );
	free( _BattlegroundCommandTable );
	free( _NPCCommandTable );
	free( _CheatCommandTable );
	free( _accountCommandTable );
	free( _petCommandTable );
	free( _recallCommandTable );
	free( _honorCommandTable );
	free( _GuildCommandTable);
	free( _TitleCommandTable);
	free( _questCommandTable );
	free( _commandTable );
}

void CommandTableStorage::Init()
{
	static ChatCommand modifyCommandTable[] =
	{
		{ "hp",		 'm', nullptr,	"Health Points/HP",	nullptr, UNIT_FIELD_HEALTH,	UNIT_FIELD_MAXHEALTH, 1 },
		{ "mana",	   'm', nullptr,	"Mana Points/MP",	  nullptr, UNIT_FIELD_POWER1,	UNIT_FIELD_MAXPOWER1, 1 },
		{ "rage",	   'm', nullptr,	"Rage Points",		 nullptr, UNIT_FIELD_POWER2,	UNIT_FIELD_MAXPOWER2, 1 },
		{ "energy",	 'm', nullptr,	"Energy Points",	   nullptr, UNIT_FIELD_POWER4,	UNIT_FIELD_MAXPOWER4, 1 },
		{ "level",	  'm', &ChatHandler::HandleModifyLevelCommand,"Level", nullptr, 0, 0, 0 },
		{ "armor",	  'm', nullptr,	"Armor",			   nullptr, UNIT_FIELD_STAT1,			  0,		   1 },
		{ "holy",	   'm', nullptr,	"Holy Resistance",	 nullptr, UNIT_FIELD_RESISTANCES_01,	 0,		   1 },
		{ "fire",	   'm', nullptr,	"Fire Resistance",	 nullptr, UNIT_FIELD_RESISTANCES_02,	 0,		   1 },
		{ "nature",	 'm', nullptr,	"Nature Resistance",   nullptr, UNIT_FIELD_RESISTANCES_03,	 0,		   1 },
		{ "frost",	  'm', nullptr,	"Frost Resistance",	nullptr, UNIT_FIELD_RESISTANCES_04,	 0,		   1 },
		{ "shadow",	 'm', nullptr,	"Shadow Resistance",   nullptr, UNIT_FIELD_RESISTANCES_05,	 0,		   1 },
		{ "arcane",	 'm', nullptr,	"Arcane Resistance",   nullptr, UNIT_FIELD_RESISTANCES_06,	 0,		   1 },
		{ "damage",	 'm', nullptr,	"Unit Damage Min/Max", nullptr, UNIT_FIELD_MINDAMAGE,  UNIT_FIELD_MAXDAMAGE,2 },
		{ "scale",	  'm', nullptr,	"Size/Scale",		  nullptr, OBJECT_FIELD_SCALE_X, 0,					2 },
		{ "gold",	   'm', &ChatHandler::HandleModifyGoldCommand,  "Gold/Money/Copper",	  nullptr,   0,  0,  0 },
		{ "speed",	  'm', &ChatHandler::HandleModifySpeedCommand, "Movement Speed",		 nullptr,   0,  0,  0 },
		{ "nativedisplayid", 'm', nullptr, "Native Display ID", nullptr, UNIT_FIELD_NATIVEDISPLAYID, 0,			  1 },
		{ "displayid" , 'm', nullptr,	"Display ID",		  nullptr, UNIT_FIELD_DISPLAYID,	   0,			  1 },
		{ "flags" ,	 'm', nullptr,	"Unit Flags",		  nullptr, UNIT_FIELD_FLAGS,		   0,			  1 },
		{ "faction",	'm', nullptr,	"Faction Template",	nullptr, UNIT_FIELD_FACTIONTEMPLATE, 0,			  1 },
		{ "dynamicflags",'m',nullptr,	"Dynamic Flags",	   nullptr, UNIT_DYNAMIC_FLAGS,		 0,			  1 },
		{ "talentpoints",'m',nullptr,	"Talent Points",	   nullptr, PLAYER_CHARACTER_POINTS1,   0,			  1 },
		{ "happiness",	'm', nullptr,	"Happiness",			 nullptr, UNIT_FIELD_POWER5,	UNIT_FIELD_MAXPOWER5, 1 },
		{ "spirit",	 'm', nullptr,	"Spirit",			  nullptr, UNIT_FIELD_STAT0,		   0,			  1 },
		{ "boundingraidius",'m',nullptr,  "Bounding Radius",	 nullptr, UNIT_FIELD_BOUNDINGRADIUS,		 0,			  2 },
		{ "combatreach",'m',nullptr,	 "Combat Reach",		nullptr, UNIT_FIELD_COMBATREACH, 0, 2 },
		{ "emotestate",'m', nullptr,	 "NPC Emote State",	 nullptr, UNIT_NPC_EMOTESTATE, 0, 1 },
		{ "bytes",'m',nullptr,"Bytes",nullptr,UNIT_FIELD_BYTES_0,0,1},
		{ nullptr,		  0, nullptr,	 "",					nullptr, 0, 0  }
	};
	dupe_command_table(modifyCommandTable, _modifyCommandTable);

	static ChatCommand debugCommandTable[] =
	{
		{ "retroactivequest", 'd', &ChatHandler::HandleDebugRetroactiveQuestAchievements, "",		nullptr, 0, 0, 0},
		{ "infront",	 'd', &ChatHandler::HandleDebugInFrontCommand,  "",							   nullptr, 0, 0, 0},
		{ "showreact",   'd', &ChatHandler::HandleShowReactionCommand,  "",							   nullptr, 0, 0, 0},
		{ "aimove",	  'd', &ChatHandler::HandleAIMoveCommand,		"",							   nullptr, 0, 0, 0},
		{ "dist",		'd', &ChatHandler::HandleDistanceCommand,	  "",							   nullptr, 0, 0, 0},
		{ "face",		'd', &ChatHandler::HandleFaceCommand,		  "",							   nullptr, 0, 0, 0},
		{ "moveinfo",	'd', &ChatHandler::HandleMoveInfoCommand,	  "",							   nullptr, 0, 0, 0},
		{ "setbytes",	'd', &ChatHandler::HandleSetBytesCommand,	  "",							   nullptr, 0, 0, 0},
		{ "getbytes",	'd', &ChatHandler::HandleGetBytesCommand,	  "",							   nullptr, 0, 0, 0},
		{ "unroot",	  'd', &ChatHandler::HandleDebugUnroot,		  "",							   nullptr, 0, 0, 0},
		{ "root",		'd', &ChatHandler::HandleDebugRoot,			"",							   nullptr, 0, 0, 0},
		{ "landwalk",	'd', &ChatHandler::HandleDebugLandWalk,		"",							   nullptr, 0, 0, 0},
		{ "waterwalk",   'd', &ChatHandler::HandleDebugWaterWalk,	   "",							   nullptr, 0, 0, 0},
		{ "castspellne", 'd', &ChatHandler::HandleCastSpellNECommand,   ".castspellne <spellid> - Casts spell on target (only plays animations, doesnt handle effects or range/facing/etc.", nullptr, 0, 0, 0 },
		{ "aggrorange",  'd', &ChatHandler::HandleAggroRangeCommand,	".aggrorange - Shows aggro Range of the selected Creature.", nullptr, 0, 0, 0 },
		{ "knockback ",  'd', &ChatHandler::HandleKnockBackCommand,	 ".knockback <hspeed> <vspeed> - Knocks selected player back.", nullptr, 0, 0, 0 },
		{ "fade ",	   'd', &ChatHandler::HandleFadeCommand,		  ".fade <value> - calls ModThreatModifyer().", nullptr, 0, 0, 0 },
		{ "threatMod ",  'd', &ChatHandler::HandleThreatModCommand,	 ".threatMod <value> - calls ModGeneratedThreatModifyer().", nullptr, 0, 0, 0 },
		{ "calcThreat ", 'd', &ChatHandler::HandleCalcThreatCommand,	".calcThreat <dmg> <spellId> - calculates threat.", nullptr, 0, 0, 0 },
		{ "threatList ", 'd', &ChatHandler::HandleThreatListCommand,	".threatList  - returns all AI_Targets of the selected Creature.", nullptr, 0, 0, 0 },
		{ "gettptime",   'd', &ChatHandler::HandleGetTransporterTime,   "grabs transporter travel time",nullptr, 0, 0, 0 },
		{ "itempushresult",'d',&ChatHandler::HandleSendItemPushResult,  "sends item push result", nullptr, 0, 0, 0 },
		{ "setbit",	  'd',  &ChatHandler::HandleModifyBitCommand,	"",							   nullptr, 0, 0, 0},
		{ "setvalue",	'd', &ChatHandler::HandleModifyValueCommand,   "",							   nullptr, 0, 0, 0},
		{ "aispelltestbegin", 'd', &ChatHandler::HandleAIAgentDebugBegin, "", nullptr, 0, 0, 0 },
		{ "aispelltestcontinue", 'd', &ChatHandler::HandleAIAgentDebugContinue, "", nullptr, 0, 0, 0 },
		{ "aispelltestskip", 'd', &ChatHandler::HandleAIAgentDebugSkip, "", nullptr, 0, 0, 0 },
		{ "dumpcoords", 'd', &ChatHandler::HandleDebugDumpCoordsCommmand, "", nullptr, 0, 0, 0 },
        { "sendpacket", 'd', &ChatHandler::HandleSendpacket, "<opcode ID>, <data>", nullptr, 0, 0, 0 },
		{ "sqlquery", 'd', &ChatHandler::HandleSQLQueryCommand, "<sql query>", nullptr, 0, 0, 0 },
		{ "rangecheck", 'd', &ChatHandler::HandleRangeCheckCommand, "Checks the 'yard' range and internal range between the player and the target.", nullptr, 0, 0, 0 },
		{ "setallratings", 'd', &ChatHandler::HandleRatingsCommand, "Sets rating values to incremental numbers based on their index.", nullptr, 0, 0, 0 },
		{ nullptr,		   0, nullptr,									  "",							   nullptr, 0, 0  }
	};
	dupe_command_table(debugCommandTable, _debugCommandTable);

	static ChatCommand waypointCommandTable[] =
	{
		{ "add",		 'w', &ChatHandler::HandleWPAddCommand,		 "Add wp at current pos",		  nullptr, 0, 0, 0},
		{ "show",		'w', &ChatHandler::HandleWPShowCommand,		"Show wp's for creature",		 nullptr, 0, 0, 0},
		{ "hide",		'w', &ChatHandler::HandleWPHideCommand,		"Hide wp's for creature",		 nullptr, 0, 0, 0},
		{ "delete",	  'w', &ChatHandler::HandleWPDeleteCommand,	  "Delete selected wp",			 nullptr, 0, 0, 0},
		{ "movehere",	'w', &ChatHandler::HandleWPMoveHereCommand,	"Move to this wp",				nullptr, 0, 0, 0},
		{ "flags",	   'w', &ChatHandler::HandleWPFlagsCommand,	   "Wp flags",					   nullptr, 0, 0, 0},
		{ "waittime",	'w', &ChatHandler::HandleWPWaitCommand,		"Wait time at this wp",		   nullptr, 0, 0, 0},
		{ "emote",	   'w', &ChatHandler::HandleWPEmoteCommand,	   "Emote at this wp",			   nullptr, 0, 0, 0},
		{ "skin",		'w', &ChatHandler::HandleWPSkinCommand,		"Skin at this wp",				nullptr, 0, 0, 0},
		{ "change",	  'w', &ChatHandler::HandleWPChangeNoCommand,	"Change at this wp",			  nullptr, 0, 0, 0},
		{ "info",		'w', &ChatHandler::HandleWPInfoCommand,		"Show info for wp",			   nullptr, 0, 0, 0},
		{ "movetype",	'w', &ChatHandler::HandleWPMoveTypeCommand,	"Movement type at wp",			nullptr, 0, 0, 0},
		{ "generate",	'w', &ChatHandler::HandleGenerateWaypoints,	"Randomly generate wps",		  nullptr, 0, 0, 0},
		{ "save",		 'w', &ChatHandler::HandleSaveWaypoints,		"Save all waypoints",			  nullptr, 0, 0, 0},
		{ "deleteall",	 'w', &ChatHandler::HandleDeleteWaypoints,	  "Delete all waypoints",			  nullptr, 0, 0, 0},
		{ "addfly", 'w', &ChatHandler::HandleWaypointAddFlyCommand, "Adds a flying waypoint", nullptr, 0, 0, 0 },
		{ nullptr,			0, nullptr,									 "",							   nullptr, 0, 0  }
	};
	dupe_command_table(waypointCommandTable, _waypointCommandTable);

	static ChatCommand GMTicketCommandTable[] =
	{
		{ "get",		 'c', &ChatHandler::HandleGMTicketGetAllCommand,  "Gets GM Ticket",			   nullptr, 0, 0, 0},
		{ "getId",	   'c', &ChatHandler::HandleGMTicketGetByIdCommand, "Gets GM Ticket by ID",		 nullptr, 0, 0, 0},
		{ "delId",	   'c', &ChatHandler::HandleGMTicketDelByIdCommand, "Deletes GM Ticket by ID",	  nullptr, 0, 0, 0},
		{ nullptr,			0, nullptr,									   "",							 nullptr, 0, 0  }
	};
	dupe_command_table(GMTicketCommandTable, _GMTicketCommandTable);

	static ChatCommand GuildCommandTable[] =
	{
		{ "create",		 'm', &ChatHandler::CreateGuildCommand,  "Creates a guild.",			   nullptr, 0, 0, 0},
		{ "rename",	   'm', &ChatHandler::HandleRenameGuildCommand, "Renames a guild.",		 nullptr, 0, 0, 0},
		{ "members",	   'm', &ChatHandler::HandleGuildMembersCommand, "Lists guildmembers and their ranks.",	  nullptr, 0, 0, 0},
		{ "removeplayer",	   'm', &ChatHandler::HandleGuildRemovePlayerCommand, "Removes a player from a guild.",		 nullptr, 0, 0, 0},
		{ "disband",	   'm', &ChatHandler::HandleGuildDisbandCommand, "Disbands the guild of your target.",		 nullptr, 0, 0, 0},
		{ "setleader", 'm', &ChatHandler::HandleGuildSetLeaderCommand, "Sets the guild leader of the target's guild to <player>", nullptr, 0, 0, 0 },
		{ nullptr,			0, nullptr,									   "",							 nullptr, 0, 0  }
	};
	dupe_command_table(GuildCommandTable, _GuildCommandTable);

	static ChatCommand GameObjectCommandTable[] =
	{
		{ "select",	  'o', &ChatHandler::HandleGOSelect,   "Selects the nearest GameObject to you",	nullptr, 0, 0, 0},
		{ "delete",	  'o', &ChatHandler::HandleGODelete,   "Deletes selected GameObject",			  nullptr, 0, 0, 0},
		{ "spawn",	   'o', &ChatHandler::HandleGOSpawn,	"Spawns a GameObject by ID",				nullptr, 0, 0, 0},
		{ "info",		'o', &ChatHandler::HandleGOInfo,	 "Gives you informations about selected GO", nullptr, 0, 0, 0},
		/*{ "activate",	'o', &ChatHandler::HandleGOActivate, "Activates/Opens the selected GO.",		 nullptr, 0, 0, 0},
		{ "enable",	  'o', &ChatHandler::HandleGOEnable,   "Enables the selected GO for use.",		 nullptr, 0, 0, 0},
		{ "scale",	   'o', &ChatHandler::HandleGOScale,	"Sets scale of selected GO",				nullptr, 0, 0, 0},
		{ "animprogress",'o', &ChatHandler::HandleGOAnimProgress, "Sets anim progress",				   nullptr, 0, 0, 0 },
		{ "export",	  'o', &ChatHandler::HandleGOExport,   "Exports the current GO selected",		  nullptr, 0, 0, 0 },*/
		{ "move", 'g', &ChatHandler::HandleGOMove, "Moves gameobject to player xyz", nullptr, 0, 0, 0 },
		{ "rotate", 'g', &ChatHandler::HandleGORotate, "Rotates gameobject x degrees", nullptr, 0, 0, 0 },
		{ "f_flags", 'g', nullptr, "Flags", nullptr, GAMEOBJECT_FLAGS, 0, 3 },
		{ "f_dynflags", 'g', nullptr, "Dynflags", nullptr, GAMEOBJECT_DYN_FLAGS, 0, 3 },
		{ nullptr,			0, nullptr,						   "",										 nullptr, 0, 0  }
	};
	dupe_command_table(GameObjectCommandTable, _GameObjectCommandTable);

	static ChatCommand BattlegroundCommandTable[] = 
	{
		{ "setbgscore",		'e', &ChatHandler::HandleSetBGScoreCommand,	"<Teamid> <Score> - Sets battleground score. 2 Arguments. ", nullptr, 0, 0, 0},
		{ "startbg",		'e', &ChatHandler::HandleStartBGCommand,	   "Starts current battleground match.",  nullptr, 0, 0, 0},
		{ "pausebg",		'e', &ChatHandler::HandlePauseBGCommand,	   "Pauses current battleground match.",  nullptr, 0, 0, 0},
		{ "bginfo",			'e', &ChatHandler::HandleBGInfoCommnad,		"Displays information about current battleground.", nullptr, 0, 0, 0},
		{ "battleground",	'e', &ChatHandler::HandleBattlegroundCommand,  "Shows BG Menu",					   nullptr, 0, 0, 0 },
		{ "setworldstate",	'e', &ChatHandler::HandleSetWorldStateCommand, "<var> <val> - Var can be in hex. WS Value.", nullptr, 0, 0, 0 },
		{ "playsound",		'e', &ChatHandler::HandlePlaySoundCommand,	 "<val>. Val can be in hex.",		   nullptr, 0, 0, 0 },
		{ "setbfstatus",	'e', &ChatHandler::HandleSetBattlefieldStatusCommand,".setbfstatus - NYI.",		   nullptr, 0, 0, 0 },
		{ "leave",			'e', &ChatHandler::HandleBattlegroundExitCommand, "Leaves the current battleground.", nullptr, 0, 0, 0 },
		{ "forcestart",		'e', &ChatHandler::HandleBattlegroundForcestartCommand, "Forcestart current battlegrounds.", nullptr, 0, 0, 0 },
		{ nullptr,			0, nullptr,									 "",									nullptr, 0, 0  }
	};
	dupe_command_table(BattlegroundCommandTable, _BattlegroundCommandTable);

	static ChatCommand NPCCommandTable[] =
	{
		{ "vendoradditem",   'z', &ChatHandler::HandleItemCommand,	  "Adds to vendor",				 nullptr, 0, 0, 0},
		{ "vendorremoveitem",'z', &ChatHandler::HandleItemRemoveCommand,"Removes from vendor.",		   nullptr, 0, 0, 0},
		{ "flags",	   'n', &ChatHandler::HandleNPCFlagCommand,	   "Changes NPC flags",			  nullptr, 0, 0, 0},
		{ "emote",	   'n', &ChatHandler::HandleEmoteCommand,		 ".emote - Sets emote state",	  nullptr, 0, 0, 0 },
		{ "delete",	  'n', &ChatHandler::HandleDeleteCommand,		"Deletes mob from db and world.", nullptr, 0, 0, 0},
		{ "info",		'n', &ChatHandler::HandleNpcInfoCommand,	   "Displays NPC information",	   nullptr, 0, 0, 0},
		{ "addAgent",	'n', &ChatHandler::HandleAddAIAgentCommand,	".npc addAgent <agent> <procEvent> <procChance> <procCount> <spellId> <spellType> <spelltargetType> <spellCooldown> <floatMisc1> <Misc2>",nullptr, 0, 0, 0},
		{ "listAgent",   'n', &ChatHandler::HandleListAIAgentCommand,   ".npc listAgent",nullptr, 0, 0, 0},
		{ "say",		 'n', &ChatHandler::HandleMonsterSayCommand,	".npc say <text> - Makes selected mob say text <text>.", nullptr, 0, 0, 0 },
		{ "yell",		'n', &ChatHandler::HandleMonsterYellCommand,   ".npc yell <Text> - Makes selected mob yell text <text>.", nullptr, 0, 0, 0},
		{ "come",		'n', &ChatHandler::HandleNpcComeCommand,	   ".npc come - Makes npc move to your position", nullptr, 0, 0, 0 },
		{ "castself",	'n', &ChatHandler::HandleNpcCastSelfCommand,	   ".npc castself <SpellId> - Makes the npc cast SpellId on himself", nullptr, 0, 0, 0 },
		{ "return",	  'n', &ChatHandler::HandleNpcReturnCommand,	 ".npc return - Returns ncp to spawnpoint.", nullptr, 0, 0, 0 },
		{ "spawn", 'n', &ChatHandler::HandleCreatureSpawnCommand, ".npc spawn <id> <save> - Spawns npc of entry <id>", nullptr, 0, 0, 0 },
		{ "spawnlink", 'n', &ChatHandler::HandleNpcSpawnLinkCommand, ".spawnlink sqlentry", nullptr, 0, 0, 0 },
		{ "possess", 'n', &ChatHandler::HandleNpcPossessCommand, ".npc possess - Possess an npc (mind control)", nullptr, 0, 0, 0 },
		{ "unpossess", 'n', &ChatHandler::HandleNpcUnPossessCommand, ".npc unpossess - Unposses any currently possessed npc.", nullptr, 0, 0, 0 },
		{ "select", 'n', &ChatHandler::HandleNpcSelectCommand, ".npc select - selects npc closest", nullptr, 0, 0, 0 },
		{ nullptr,		  2, nullptr,						   "",										   nullptr, 0, 0  }
	};
	dupe_command_table(NPCCommandTable, _NPCCommandTable);

	static ChatCommand CheatCommandTable[] =
	{
		{ "status",	 'm', &ChatHandler::HandleShowCheatsCommand, "Shows active cheats.",			 nullptr, 0, 0, 0 },
		{ "taxi",	   'm', &ChatHandler::HandleTaxiCheatCommand,	 "Enables all taxi nodes.",	   nullptr, 0, 0, 0},
		{ "cooldown",   'm', &ChatHandler::HandleCooldownCheatCommand, "Enables no cooldown cheat.",	nullptr, 0, 0, 0 },
		{ "casttime",   'm', &ChatHandler::HandleCastTimeCheatCommand, "Enables no cast time cheat.",   nullptr, 0, 0, 0 },
		{ "power",	  'm', &ChatHandler::HandlePowerCheatCommand, "Disables mana consumption etc.",   nullptr, 0, 0, 0 },
		{ "god",		'm', &ChatHandler::HandleGodModeCommand, "Sets god mode, prevents you from taking damage.", nullptr, 0, 0, 0 },
		{ "fly",		'm', &ChatHandler::HandleFlyCommand, "Sets fly mode",						   nullptr, 0, 0, 0 },
		{ "land",	   'm', &ChatHandler::HandleLandCommand, "Unsets fly mode",						nullptr, 0, 0, 0 },
		{ "explore",	'm', &ChatHandler::HandleExploreCheatCommand, "Reveals the unexplored parts of the map.", nullptr, 0, 0, 0 },
		{ "stack",	  'm', &ChatHandler::HandleStackCheatCommand, "Enables aura stacking cheat.", nullptr, 0, 0, 0 },
		{ "triggerpass", 'm', &ChatHandler::HandleTriggerpassCheatCommand, "Ignores area trigger prerequisites.", nullptr, 0, 0, 0 },
		{ nullptr,		   0, nullptr,							"",									   nullptr, 0, 0, 0 },
	};
	dupe_command_table(CheatCommandTable, _CheatCommandTable);

	static ChatCommand accountCommandTable[] =
	{
		{ "ban",	  'a', &ChatHandler::HandleAccountBannedCommand,   "Ban account. .account ban name timeperiod", nullptr, 0, 0, 0 },
		{ "unban",	  'z', &ChatHandler::HandleAccountUnbanCommand,		"Unbans account x.", nullptr, 0, 0, 0 },
		{ "level",	  'z', &ChatHandler::HandleAccountLevelCommand,    "Sets gm level on account. Pass it username and 0,1,2,3,az, etc.", nullptr, 0, 0, 0 },
		{ "mute",	  'a', &ChatHandler::HandleAccountMuteCommand,		"Mutes account for <timeperiod>.", nullptr, 0, 0, 0 },
		{ "unmute",	  'a', &ChatHandler::HandleAccountUnmuteCommand,	"Unmutes account <x>", nullptr, 0, 0, 0 },

		{ nullptr, 0, nullptr, "", nullptr, 0, 0, 0},
	};
	dupe_command_table(accountCommandTable, _accountCommandTable);

	/*static ChatCommand honorCommandTable[] =
	{
		{ "addpoints",   'm', &ChatHandler::HandleAddHonorCommand,	  "Adds x amount of honor points/currency",nullptr,0,0,0},
		{ "addkills",	 'm', &ChatHandler::HandleAddKillCommand,	   "Adds x amount of honor kills", nullptr, 0, 0, 0 },
		{ "globaldailyupdate", 'm', &ChatHandler::HandleGlobalHonorDailyMaintenanceCommand, "Daily honor field moves", nullptr, 0, 0, 0},
		{ "singledailyupdate", 'm', &ChatHandler::HandleNextDayCommand, "Daily honor field moves for selected player only", nullptr,0,0,0},
		{ "pvpcredit", 'm', &ChatHandler::HandlePVPCreditCommand, "Sends PVP credit packet, with specified rank and points", nullptr,0,0,0},
		{ nullptr,0,nullptr,"",nullptr,0,0,0},
	};
	dupe_command_table(honorCommandTable, _honorCommandTable);*/

	static ChatCommand petCommandTable[] = 
	{
		{ "createpet",'m',&ChatHandler::HandleCreatePetCommand, "Creates a pet with <entry>.", nullptr, 0, 0, 0 },
		{ "renamepet",'m',&ChatHandler::HandleRenamePetCommand, "Renames a pet to <name>.", nullptr, 0, 0, 0 },
		{ "addspell",'m',&ChatHandler::HandleAddPetSpellCommand, "Teaches pet <spell>.", nullptr, 0, 0, 0 },
		{ "removespell",'m',&ChatHandler::HandleRemovePetSpellCommand, "Removes pet spell <spell>.", nullptr, 0, 0, 0 },
		{ nullptr,0,nullptr,"",nullptr,0,0,0},
	};
	dupe_command_table(petCommandTable, _petCommandTable);

	static ChatCommand recallCommandTable[] =
	{
		{ "list",		'q', &ChatHandler::HandleRecallListCommand,	   "List recall locations",		  nullptr, 0, 0, 0},
		{ "port",		'q', &ChatHandler::HandleRecallGoCommand,		 "Port to recalled location",	  nullptr, 0, 0, 0},
		{ "add",		 'q', &ChatHandler::HandleRecallAddCommand,		"Add recall location",			nullptr, 0, 0, 0},
		{ "del",		 'q', &ChatHandler::HandleRecallDelCommand,		"Remove a recall location",	   nullptr, 0, 0, 0},
		{ "portplayer", 'm', &ChatHandler::HandleRecallPortPlayerCommand, "recall ports player", nullptr, 0, 0, 0 },
		{ nullptr,		   0,  nullptr,										"",							   nullptr, 0, 0, 0},
	};
	dupe_command_table(recallCommandTable, _recallCommandTable);

	static ChatCommand questCommandTable[] =
	{
		{ "addboth",   '2', &ChatHandler::HandleQuestAddBothCommand,	"Add quest <id> to the targeted NPC as start & finish",	nullptr, 0, 0, 0},
		{ "addfinish", '2', &ChatHandler::HandleQuestAddFinishCommand,	"Add quest <id> to the targeted NPC as finisher",		nullptr, 0, 0, 0},
		{ "addstart",  '2', &ChatHandler::HandleQuestAddStartCommand,	"Add quest <id> to the targeted NPC as starter",		nullptr, 0, 0, 0},
		{ "delboth",   '2', &ChatHandler::HandleQuestDelBothCommand,	"Delete quest <id> from the targeted NPC as start & finish",	nullptr, 0, 0, 0},
		{ "delfinish", '2', &ChatHandler::HandleQuestDelFinishCommand,	"Delete quest <id> from the targeted NPC as finisher",	nullptr, 0, 0, 0},
		{ "delstart",  '2', &ChatHandler::HandleQuestDelStartCommand,	"Delete quest <id> from the targeted NPC as starter",	nullptr, 0, 0, 0},
		{ "complete",  '2', &ChatHandler::HandleQuestFinishCommand,	    "Complete/Finish quest <id>",							nullptr, 0, 0, 0},
		{ "finisher",  '2', &ChatHandler::HandleQuestFinisherCommand,	"Lookup quest finisher for quest <id>",					nullptr, 0, 0, 0},
		{ "item",	   '2', &ChatHandler::HandleQuestItemCommand,		"Lookup itemid necessary for quest <id>",				nullptr, 0, 0, 0},
		{ "list",	   '2', &ChatHandler::HandleQuestListCommand,		"Lists the quests for the npc <id>",					nullptr, 0, 0, 0},
		{ "load",	   '2', &ChatHandler::HandleQuestLoadCommand,		"Loads quests from database",							nullptr, 0, 0, 0},
		{ "lookup",	   '2', &ChatHandler::HandleQuestLookupCommand,		"Looks up quest string x",								nullptr, 0, 0, 0},
		{ "giver",	   '2', &ChatHandler::HandleQuestGiverCommand,		"Lookup quest giver for quest <id>",					nullptr, 0, 0, 0},
		{ "remove",	   '2', &ChatHandler::HandleQuestRemoveCommand,		"Removes the quest <id> from the targeted player",		nullptr, 0, 0, 0},
		{ "reward",	   '2', &ChatHandler::HandleQuestRewardCommand,		"Shows reward for quest <id>",							nullptr, 0, 0, 0},
		{ "status",	   '2', &ChatHandler::HandleQuestStatusCommand,		"Lists the status of quest <id>",						nullptr, 0, 0, 0},
		{ "spawn",	   '2', &ChatHandler::HandleQuestSpawnCommand,		"Port to spawn location for quest <id>",				nullptr, 0, 0, 0},
		{ "start",	   '2', &ChatHandler::HandleQuestStartCommand,		"Starts quest <id>",									nullptr, 0, 0, 0},
		{ nullptr,		    0,  nullptr,										"",														nullptr, 0, 0, 0},
	};
	dupe_command_table(questCommandTable, _questCommandTable);

	static ChatCommand commandTable[] = {
		//{ "renameguild", 'a', &ChatHandler::HandleRenameGuildCommand, "Renames a guild.", nullptr, 0, 0, 0 },
		{ "commands",	'0', &ChatHandler::HandleCommandsCommand,		"Shows Commands",				 nullptr, 0, 0, 0},
		{ "help",		'0', &ChatHandler::HandleHelpCommand,			"Shows help for command",		 nullptr, 0, 0, 0},
		{ "announce",	'u', &ChatHandler::HandleAnnounceCommand,	  "Sends Msg To All",			   nullptr, 0, 0, 0},
		{ "wannounce",   'u', &ChatHandler::HandleWAnnounceCommand,	 "Sends Widescreen Msg To All",	nullptr, 0, 0, 0},
		{ "appear",	  'v', &ChatHandler::HandleAppearCommand,		"Teleports to x's position.",	 nullptr, 0, 0, 0},
		{ "summon",	  'v', &ChatHandler::HandleSummonCommand,		"Summons x to your position",	 nullptr, 0, 0, 0},
		{ "banchar",	 'b', &ChatHandler::HandleBanCharacterCommand,  "Bans character x with or without reason",			  nullptr, 0, 0, 0},
		{ "unbanchar",   'b', &ChatHandler::HandleUnBanCharacterCommand,"Unbans character x",			 nullptr, 0, 0, 0},
		{ "kick",		'b', &ChatHandler::HandleKickCommand,		  "Kicks player from server",	   nullptr, 0, 0, 0},
		{ "kill",		'r', &ChatHandler::HandleKillCommand,		  ".kill - Kills selected unit.",   nullptr, 0, 0, 0},
		{ "killplr" ,   'r', &ChatHandler::HandleKillByPlrCommand,         ".killplr <name> - Kills specified player" , nullptr , 0 , 0 , 0 },
		{ "revive",	  'r', &ChatHandler::HandleReviveCommand,		"Revives you.",				   nullptr, 0, 0, 0},
		{ "reviveplr",   'r', &ChatHandler::HandleReviveStringcommand,  "Revives player specified.",	  nullptr, 0, 0, 0},
		{ "demorph",	 'm', &ChatHandler::HandleDeMorphCommand,	   "Demorphs from morphed model.",   nullptr, 0, 0, 0},
		{ "mount",	   'm', &ChatHandler::HandleMountCommand,		 "Mounts into modelid x.",		 nullptr, 0, 0, 0},
		{ "dismount",	  'h', &ChatHandler::HandleDismountCommand,	  "Dismounts.",					 nullptr, 0, 0, 0},
		{ "gm",		  '0', &ChatHandler::HandleGMListCommand,		"Shows active GM's",			  nullptr, 0, 0, 0},
		{ "gmoff",	   't', &ChatHandler::HandleGMOffCommand,		 "Sets GM tag off",				nullptr, 0, 0, 0},
		{ "gmon",		't', &ChatHandler::HandleGMOnCommand,		  "Sets GM tag on",				 nullptr, 0, 0, 0},
		{ "gps",		 '0', &ChatHandler::HandleGPSCommand,		   "Shows Position",				 nullptr, 0, 0, 0},
		{ "info",		'0', &ChatHandler::HandleInfoCommand,		  "Server info",					nullptr, 0, 0, 0},
		{ "worldport",   'v', &ChatHandler::HandleWorldPortCommand,	 "",							   nullptr, 0, 0, 0},
		{ "save",		's', &ChatHandler::HandleSaveCommand,		  "Save's your character",		  nullptr, 0, 0, 0},
		{ "saveall",	 'z', &ChatHandler::HandleSaveAllCommand,	   "Save's all playing characters",  nullptr, 0, 0, 0},
		{ "start",	   'm', &ChatHandler::HandleStartCommand,		 "Teleport's you to a starting location",							   nullptr, 0, 0, 0},
		{ "additem",	 'm', &ChatHandler::HandleAddInvItemCommand,	"",							   nullptr, 0, 0, 0},
		{ "removeitem",  'm', &ChatHandler::HandleRemoveItemCommand,	"Removes item %u count %u.", nullptr, 0, 0, 0 },
		{ "invincible",  'j', &ChatHandler::HandleInvincibleCommand,	".invincible - Toggles INVINCIBILITY (mobs won't attack you)", nullptr, 0, 0, 0},
		{ "invisible",   'i', &ChatHandler::HandleInvisibleCommand,	 ".invisible - Toggles INVINCIBILITY and INVISIBILITY (mobs won't attack you and nobody can see you, but they can see your chat messages)", nullptr, 0, 0, 0},
		{ "resetreputation", 'n',&ChatHandler::HandleResetReputationCommand, ".resetreputation - Resets reputation to start levels. (use on characters that were made before reputation fixes.)", nullptr, 0, 0, 0},
		{ "resetspells", 'n', &ChatHandler::HandleResetSpellsCommand,   ".resetspells - Resets all spells to starting spells of targeted player. DANGEROUS.", nullptr, 0, 0, 0 },
		{ "resettalents",'n', &ChatHandler::HandleResetTalentsCommand,  ".resettalents - Resets all talents of targeted player to that of their current level. DANGEROUS.", nullptr, 0, 0, 0 },
		{ "resetskills", 'n', &ChatHandler::HandleResetSkillsCommand ,  ".resetskills - Resets all skills.", nullptr, 0, 0, 0 },
		{ "learn",	   'm', &ChatHandler::HandleLearnCommand,		 "Learns spell",				   nullptr, 0, 0, 0},
		{ "unlearn",	 'm', &ChatHandler::HandleUnlearnCommand,	   "Unlearns spell",				 nullptr, 0, 0, 0},
		{ "getskilllevel", 'm', &ChatHandler::HandleGetSkillLevelCommand, "Gets the current level of a skill",nullptr,0,0,0}, //DGM (maybe add to playerinfo?)
        { "getskillinfo", 'm', &ChatHandler::HandleGetSkillsInfoCommand, "Gets all the skills from a player",nullptr,0,0,0},
		{ "learnskill",  'm', &ChatHandler::HandleLearnSkillCommand,	".learnskill <skillid> (optional) <value> <maxvalue> - Learns skill id skillid.", nullptr, 0, 0, 0},
		{ "advanceskill",'m', &ChatHandler::HandleModifySkillCommand,   "advanceskill <skillid> <amount, optional, default = 1> - Advances skill line x times..", nullptr, 0, 0, 0},
		{ "removeskill", 'm', &ChatHandler::HandleRemoveSkillCommand,   ".removeskill <skillid> - Removes skill",		 nullptr, 0, 0, 0 },
		{ "increaseweaponskill", 'm', &ChatHandler::HandleIncreaseWeaponSkill, ".increaseweaponskill <count> - Increase eqipped weapon skill x times (defaults to 1).", nullptr, 0, 0, 0},
		{ "playerinfo",  'm', &ChatHandler::HandlePlayerInfo,		   ".playerinfo - Displays informations about the selected character (account...)", nullptr, 0, 0, 0 },

		{ "modify",		'm', nullptr,									 "",				 modifyCommandTable, 0, 0, 0},
		{ "waypoint",	  'w', nullptr,									 "",			   waypointCommandTable, 0, 0, 0},
		{ "debug",		 'd', nullptr,									 "",				  debugCommandTable, 0, 0, 0},
		{ "gmTicket",	  'c', nullptr,									 "",			   GMTicketCommandTable, 0, 0, 0},
		{ "gobject",	   'o', nullptr,									 "",			 GameObjectCommandTable, 0, 0, 0},
		{ "battleground",  'e', nullptr,									 "",		   BattlegroundCommandTable, 0, 0, 0},
		{ "npc"		 ,  'n', nullptr,									 "",					NPCCommandTable, 0, 0, 0},
		{ "cheat"	   ,  'm', nullptr,									 "",				  CheatCommandTable, 0, 0, 0},
		{ "account"	   ,  'a', nullptr,									 "",				  accountCommandTable, 0, 0, 0},
		{ "quest",		'q', nullptr,									 "",				 questCommandTable, 0, 0, 0},
		{ "pet",		   'm', nullptr,									 "",					petCommandTable, 0, 0, 0},
		{ "recall",		'q', nullptr,									 "",				 recallCommandTable, 0, 0, 0},
		{ "guild",		'm', nullptr,									 "",				 GuildCommandTable, 0, 0, 0},
		{ "getpos"	  ,  'd', &ChatHandler::HandleGetPosCommand,		"",							   nullptr, 0, 0, 0},
		{ "clearcooldowns", 'm', &ChatHandler::HandleClearCooldownsCommand, "Clears all cooldowns for your class.", nullptr, 0, 0, 0 },
		{ "removeauras",   'm', &ChatHandler::HandleRemoveAurasCommand,   "Removes all auras from target",  nullptr, 0, 0, 0},
		{ "paralyze",	  'b', &ChatHandler::HandleParalyzeCommand,	  "Roots/Paralyzes the target.",	nullptr, 0, 0, 0 },
		{ "unparalyze",	'b', &ChatHandler::HandleUnParalyzeCommand,	"Unroots/Unparalyzes the target.",nullptr, 0, 0, 0 },
		{ "setmotd",	   'm', &ChatHandler::HandleSetMotdCommand,	   "Sets MOTD",					  nullptr, 0, 0, 0 },
		{ "additemset",	'm', &ChatHandler::HandleAddItemSetCommand,	"Adds item set to inv.",		  nullptr, 0, 0, 0 },
		{ "gotrig",		'v', &ChatHandler::HandleTriggerCommand,	   "Warps to areatrigger <id>",	  nullptr, 0, 0, 0 },
		{ "exitinstance",  'm', &ChatHandler::HandleExitInstanceCommand,  "Exits current instance, return to entry point.", nullptr, 0, 0, 0 },
		{ "reloadtable",	  'm', &ChatHandler::HandleDBReloadCommand,	  "Reloads some of the database tables", nullptr, 0, 0, 0 },
		{ "shutdown", 'z', &ChatHandler::HandleShutdownCommand, "Initiates server shutdown in <x> seconds.", nullptr, 0, 0, 0 },
		{ "restart", 'z', &ChatHandler::HandleShutdownRestartCommand, "Initiates server restart in <x> seconds.", nullptr, 0, 0, 0 },
		{ "allowwhispers", 'c', &ChatHandler::HandleAllowWhispersCommand, "Allows whispers from player <s> while in gmon mode.", nullptr, 0, 0, 0 },
		{ "blockwhispers", 'c', &ChatHandler::HandleBlockWhispersCommand, "Blocks whispers from player <s> while in gmon mode.", nullptr, 0, 0, 0 },
		{ "advanceallskills", 'm', &ChatHandler::HandleAdvanceAllSkillsCommand, "Advances all skills <x> points.", nullptr, 0, 0, 0 },
		{ "killbyplayer", 'f', &ChatHandler::HandleKillByPlayerCommand, "Disconnects the player with name <s>.", nullptr, 0, 0, 0 },
		{ "killbyaccount", 'f', &ChatHandler::HandleKillBySessionCommand, "Disconnects the session with account name <s>.", nullptr, 0, 0, 0 },
		{ "castall", 'z', &ChatHandler::HandleCastAllCommand, "Makes all players online cast spell <x>.", nullptr, 0, 0, 0},
		{ "dispelall", 'z', &ChatHandler::HandleDispelAllCommand, "Dispels all negative (or positive w/ 1) auras on all players.",nullptr,0,0,0},
		{ "castspell",   'd', &ChatHandler::HandleCastSpellCommand,	 ".castspell <spellid> - Casts spell on target.",  nullptr, 0, 0, 0 },
		{ "modperiod" , 'm', &ChatHandler::HandleModPeriodCommand, "Changes period of current transporter.", nullptr, 0, 0, 0 },
		{ "npcfollow", 'm', &ChatHandler::HandleNpcFollowCommand, "Sets npc to follow you", nullptr, 0, 0, 0 },
		{ "nullfollow", 'm', &ChatHandler::HandleNullFollowCommand, "Sets npc to not follow anything", nullptr, 0, 0, 0 },
		{ "formationlink1", 'm', &ChatHandler::HandleFormationLink1Command, "Sets formation master.", nullptr, 0, 0, 0 },
		{ "formationlink2", 'm', &ChatHandler::HandleFormationLink2Command, "Sets formation slave with distance and angle", nullptr, 0, 0, 0 },
		{ "formationclear", 'm', &ChatHandler::HandleFormationClearCommand, "Removes formation from creature", nullptr, 0, 0, 0 },
		{ "playall", 'z', &ChatHandler::HandleGlobalPlaySoundCommand, "Plays a sound to the entire server.", nullptr, 0, 0, 0 },
		{ "addipban", 'm', &ChatHandler::HandleIPBanCommand, "Adds an address to the IP ban table: <address> [duration]\nDuration must be a number optionally followed by a character representing the calendar subdivision to use (h>hours, d>days, w>weeks, m>months, y>years, default minutes)\nLack of duration results in a permanent ban.", nullptr, 0, 0, 0 },
		{ "delipban", 'm', &ChatHandler::HandleIPUnBanCommand, "Deletes an address from the IP ban table: <address>", nullptr, 0, 0, 0},
		{ "renamechar", 'm', &ChatHandler::HandleRenameCommand, "Renames character x to y.", nullptr, 0, 0, 0 },
		{ "forcerenamechar", 'm', &ChatHandler::HandleForceRenameCommand, "Forces character x to rename his char next login", nullptr, 0, 0, 0 },
		{ "getstanding", 'm', &ChatHandler::HandleGetStandingCommand, "Gets standing of faction %u.", nullptr, 0, 0, 0 },
		{ "setstanding", 'm', &ChatHandler::HandleSetStandingCommand, "Sets stanging of faction %u.", nullptr, 0, 0, 0 },
		{ "lookupitem", 'l', &ChatHandler::HandleLookupItemCommand, "Looks up item string x.", nullptr, 0, 0, 0 },
		{ "lookupquest", 'l', &ChatHandler::HandleQuestLookupCommand, "Looks up quest string x.", nullptr, 0, 0, 0 },
		{ "lookupcreature", 'l', &ChatHandler::HandleLookupCreatureCommand, "Looks up item string x.", nullptr, 0, 0, 0 },
		//{ "reloadscripts", 'w', &ChatHandler::HandleReloadScriptsCommand, "Reloads GM Scripts", nullptr, 0, 0, 0 },
		{ "rehash", 'z', &ChatHandler::HandleRehashCommand, "Reloads config file.", nullptr, 0, 0, 0 },
		{ "whisperblock", 'g', &ChatHandler::HandleWhisperBlockCommand, "Blocks like .gmon except without the <GM> tag", nullptr, 0, 0, 0 },
		{ "logcomment" , '1' , &ChatHandler::HandleGmLogCommentCommand, "Adds a comment to the GM log for the admins to read." , nullptr , 0 , 0 , 0 },
		{ "showitems", 'm', &ChatHandler::HandleShowItems, "test for ItemIterator", nullptr, 0, 0, 0 },
		{ "testlos", 'm', &ChatHandler::HandleCollisionTestLOS, "tests los", nullptr, 0, 0, 0 },
		{ "testindoor", 'm', &ChatHandler::HandleCollisionTestIndoor, "tests indoor", nullptr, 0, 0, 0 },
		{ "getheight", 'm', &ChatHandler::HandleCollisionGetHeight, "Gets height", nullptr, 0, 0, 0 },
		{ "renameallinvalidchars", 'z', &ChatHandler::HandleRenameAllCharacter, "Renames all invalid character names", nullptr, 0,0, 0 },
		{ "removesickness",   'm', &ChatHandler::HandleRemoveRessurectionSickessAuraCommand,   "Removes ressurrection sickness from the target",  nullptr, 0, 0, 0},
		{ "fixscale", 'm', &ChatHandler::HandleFixScaleCommand, "", nullptr, 0, 0, 0 },
		{ "addtrainerspell", 'g', &ChatHandler::HandleAddTrainerSpellCommand, "", nullptr, 0, 0, 0 },
		{ "clearcorpses", 'm', &ChatHandler::HandleClearCorpsesCommand, "", nullptr, 0, 0, 0 },
		{ "clearbones", 'm', &ChatHandler::HandleClearBonesCommand, "", nullptr, 0, 0, 0 },

		{ "multimute", 'b', &ChatHandler::HandleMultiMuteCommand, "mutes multiple , .multimute <reason> <player1> <player2> ...", nullptr, 0, 0, 0 },
		{ "multiban", 'b', &ChatHandler::HandleMultiBanCommand, "bans multiple , .multimute <reason> <player1> <player2> ...", nullptr, 0, 0, 0 },
		{ "multiaccountban", 'b', &ChatHandler::HandleMultiAccountBanCommand, "account bans multiple , .multimute <reason> <player1> <player2> ...", nullptr, 0, 0, 0 },
		{ "multikick", 'b', &ChatHandler::HandleMultiKickCommand, "kicks multiple , .multimute <reason> <player1> <player2> ...", nullptr, 0, 0, 0 },
	
		{ nullptr,		  0, nullptr,										 "",							   nullptr, 0, 0  }
	};
	dupe_command_table(commandTable, _commandTable);

	/* set the correct pointers */
	ChatCommand * p = &_commandTable[0];
	while(p->Name != 0)
	{
		if(p->ChildCommands != 0)
		{
			// Set the correct pointer.
			ChatCommand * np = GetSubCommandTable(p->Name);
			ASSERT(np);
			p->ChildCommands = np;
		}
		++p;
	}
}

ChatHandler::ChatHandler()
{
	new CommandTableStorage;
	CommandTableStorage::getSingleton().Init();
	SkillNameManager = new SkillNameMgr;
}

ChatHandler::~ChatHandler()
{
	CommandTableStorage::getSingleton().Dealloc();
	delete CommandTableStorage::getSingletonPtr();
	delete SkillNameManager;
}

bool ChatHandler::hasStringAbbr(const char* s1, const char* s2)
{
	for(;;)
	{
		if( !*s2 )
			return true;
		else if( !*s1 )
			return false;
		else if( tolower( *s1 ) != tolower( *s2 ) )
			return false;
		s1++; s2++;
	}
}

void ChatHandler::SendMultilineMessage(WorldSession *m_session, const char *str)
{
	char * start = (char*)str, *end;
	for(;;)
	{
        end = strchr(start, '\n');
		if(!end)
			break;

		*end = '\0';
		SystemMessage(m_session, start);
		start = end + 1;
	}
	if(*start != '\0')
		SystemMessage(m_session, start);
}

bool ChatHandler::ExecuteCommandInTable(ChatCommand *table, const char* text, WorldSession *m_session)
{
	std::string cmd = "";

	// get command
	while (*text != ' ' && *text != '\0')
	{
		cmd += *text;
		text++;
	}

	while (*text == ' ') text++; // skip whitespace

	if(!cmd.length())
		return false;

	for(uint32 i = 0; table[i].Name != nullptr; i++)
	{
		if(!hasStringAbbr(table[i].Name, cmd.c_str()))
			continue;

		if(table[i].CommandGroup != '0' && !m_session->CanUseCommand(table[i].CommandGroup))
			continue;

		if(table[i].ChildCommands != nullptr)
		{
			if(!ExecuteCommandInTable(table[i].ChildCommands, text, m_session))
			{
				if(table[i].Help != "")
					SendMultilineMessage(m_session, table[i].Help.c_str());
				else
				{
					GreenSystemMessage(m_session, "Available Subcommands:");
					for(uint32 k=0; table[i].ChildCommands[k].Name;k++)
					{
						if(table[i].ChildCommands[k].CommandGroup != '0' && m_session->CanUseCommand(table[i].ChildCommands[k].CommandGroup))
							BlueSystemMessage(m_session, " %s - %s", table[i].ChildCommands[k].Name, table[i].ChildCommands[k].Help.size() ? table[i].ChildCommands[k].Help.c_str() : "No Help Available");
					}
				}
			}

			return true;
		}
		
		// Check for field-based commands
		if(table[i].Handler == nullptr && (table[i].MaxValueField || table[i].NormalValueField))
		{
			bool result = false;
			if(strlen(text) == 0)
			{
				RedSystemMessage(m_session, "No values specified.");
			}
			if(table[i].ValueType == 2)
				result = CmdSetFloatField(m_session, table[i].NormalValueField, table[i].MaxValueField, table[i].Name, text);
			else if(table[i].ValueType == 3)
			{
				result = true;
				if( m_session->GetPlayer()->m_GM_SelectedGO != nullptr )
					m_session->GetPlayer()->m_GM_SelectedGO->SetUInt32Value(table[i].NormalValueField, atoi(text));
				else
					result = false;
			}
			else
				result = CmdSetValueField(m_session, table[i].NormalValueField, table[i].MaxValueField, table[i].Name, text);

			if(!result)
				RedSystemMessage(m_session, "Must be in the form of (command) <value>, or, (command) <value> <maxvalue>");
		}
		else
		{
			if(!(this->*(table[i].Handler))(text, m_session))
			{
				if(table[i].Help != "")
					SendMultilineMessage(m_session, table[i].Help.c_str());
				else
				{
					RedSystemMessage(m_session, "Incorrect syntax specified. Try .help %s for the correct syntax.", table[i].Name);
				}
			}
		}

		return true;
	}
	return false;
}

int ChatHandler::ParseCommands(const char* text, WorldSession *session)
{
	if (!session)
		return 0;

	if(!*text)
		return 0;

	if(session->GetPermissionCount() == 0 && sWorld.m_reqGmForCommands)
		return 0;

	if(text[0] != '!' && text[0] != '.') // let's not confuse users
		return 0;

	/* skip '..' :P that pisses me off */
	if(text[1] == '.')
		return 0;

	text++;

	if(!ExecuteCommandInTable(CommandTableStorage::getSingleton().Get(), text, session))
	{
		SystemMessage(session, "There is no such command, or you do not have access to it.");
	}

	return 1;
}

WorldPacket * ChatHandler::FillMessageData( uint32 type, int32 language, const char *message,uint64 guid , uint8 flag) const
{
	//Packet    structure
	//uint8	    type;
	//uint32	language;
	//uint64	guid;
	//uint64	guid;
	//uint32	len_of_text;
	//char	    text[];		 // not sure ? i think is nullptr terminated .. not nullptr terminated
	//uint8	    afk_state;
	ASSERT(type != CHAT_MSG_CHANNEL);
	//channels are handled in channel handler and so on
	

	std::string strmessage=message;

	replace(strmessage, "||", "|", 1);
	replace(strmessage, "|", "||", 1);

	message=strmessage.c_str();

	uint32 messageLength = (uint32)strlen((char*)message) + 1;
	
	WorldPacket *data = new WorldPacket(SMSG_MESSAGECHAT, messageLength + 30);

	*data << (uint8)type;
	*data << language;

	*data << guid;

	// This should not be sent in whispers. 
	// Otherwise it sends the wrong field resulting in whispers not working
    if (type == CHAT_MSG_SAY || type == CHAT_MSG_YELL || type == CHAT_MSG_PARTY )
        *data << guid;

	*data << messageLength;
	*data << message;

	*data << uint8(flag);
	return data;
}

WorldPacket* ChatHandler::FillSystemMessageData(const char *message) const
{
	uint32 messageLength = (uint32)strlen((char*)message) + 1;

	WorldPacket * data = new WorldPacket(SMSG_MESSAGECHAT, 20 + messageLength);
	*data << (uint8)CHAT_MSG_SYSTEM;
	*data << (uint32)LANG_UNIVERSAL;
	*data << (uint64)0; // Who cares about guid when there's no nickname displayed heh ?

	*data << messageLength;
	*data << message;

	*data << uint8(0);

	return data;
}

Player * ChatHandler::getSelectedChar(WorldSession *m_session, bool showerror)
{
	uint64 guid;
	Player *chr;
	
	guid = m_session->GetPlayer()->GetSelection();
	
	if (guid == 0)
	{
		if(showerror) 
			GreenSystemMessage(m_session, "Auto-targeting self.");
		chr = m_session->GetPlayer(); // autoselect
	}
	else
		chr = m_session->GetPlayer()->GetMapMgr()->GetPlayer((uint32)guid);
	
	if(chr == nullptr)
	{
		if(showerror) 
			RedSystemMessage(m_session, "This command requires that you select a player.");
		return nullptr;
	}

	return chr;
}

Creature * ChatHandler::getSelectedCreature(WorldSession *m_session, bool showerror)
{
	uint64 guid;
	Creature *creature = nullptr;

	guid = m_session->GetPlayer()->GetSelection();
	if(GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_PET)
		creature = m_session->GetPlayer()->GetMapMgr()->GetPet( GET_LOWGUID_PART(guid) );
	else if(GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_UNIT)
		creature = m_session->GetPlayer()->GetMapMgr()->GetCreature( GET_LOWGUID_PART(guid) );
	
	if(creature != nullptr)
		return creature;
	else
	{
		if(showerror) 
			RedSystemMessage(m_session, "This command requires that you select a creature.");
		return nullptr;
	}
}

void ChatHandler::SystemMessage(WorldSession *m_session, const char* message, ...)
{
	if( !message ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024, message,ap);
	WorldPacket * data = FillSystemMessageData(msg1);
	if(m_session != nullptr) 
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::ColorSystemMessage(WorldSession *m_session, const char* colorcode, const char *message, ...)
{
	if( !message ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024, message,ap);
	char msg[1024];
	snprintf(msg, 1024, "%s%s|r", colorcode, msg1);
	WorldPacket * data = FillSystemMessageData(msg);
	if(m_session != nullptr) 
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::RedSystemMessage(WorldSession *m_session, const char *message, ...)
{
	if( !message ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024,message,ap);
	char msg[1024];
	snprintf(msg, 1024,"%s%s|r", MSG_COLOR_LIGHTRED/*MSG_COLOR_RED*/, msg1);
	WorldPacket * data = FillSystemMessageData(msg);
	if(m_session != nullptr) 
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::GreenSystemMessage(WorldSession *m_session, const char *message, ...)
{
	if( !message ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024, message,ap);
	char msg[1024];
	snprintf(msg, 1024, "%s%s|r", MSG_COLOR_GREEN, msg1);
	WorldPacket * data = FillSystemMessageData(msg);
	if(m_session != nullptr) 
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::BlueSystemMessage(WorldSession *m_session, const char *message, ...)
{
	if( !message ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024, message,ap);
	char msg[1024];
	snprintf(msg, 1024,"%s%s|r", MSG_COLOR_LIGHTBLUE, msg1);
	WorldPacket * data = FillSystemMessageData(msg);
	if(m_session != nullptr) 
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::RedSystemMessageToPlr(Player* plr, const char *message, ...)
{
	if( !message || !plr->GetSession() ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024,message,ap);
	RedSystemMessage(plr->GetSession(), (const char*)msg1);
}

void ChatHandler::GreenSystemMessageToPlr(Player* plr, const char *message, ...)
{
	if( !message || !plr->GetSession() ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024,message,ap);
	GreenSystemMessage(plr->GetSession(), (const char*)msg1);
}

void ChatHandler::BlueSystemMessageToPlr(Player* plr, const char *message, ...)
{
	if( !message || !plr->GetSession() ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024,message,ap);
	BlueSystemMessage(plr->GetSession(), (const char*)msg1);
}

void ChatHandler::SystemMessageToPlr(Player *plr, const char* message, ...)
{
	if( !message || !plr->GetSession() ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024,message,ap);
	SystemMessage(plr->GetSession(), msg1);
}

bool ChatHandler::CmdSetValueField(WorldSession *m_session, uint32 field, uint32 fieldmax, const char *fieldname, const char *args)
{
	if(!args) return false;
	char* pvalue = strtok((char*)args, " ");
	uint32 mv, av;

	if (!pvalue)
		return false;
	else
		av = atol(pvalue);

	if(fieldmax)
	{
		char* pvaluemax = strtok(nullptr, " ");   
		if (!pvaluemax)
			return false;  
		else
			mv = atol(pvaluemax);
	}
	else
	{
		mv = 0;
	}

	if (av <= 0 && mv > 0)
	{  
		RedSystemMessage(m_session, "Values are invalid. Value must be < max (if max exists), and both must be > 0.");
		return true;   
	}
	if(fieldmax)
	{
		if(mv < av || mv <= 0)
		{
			RedSystemMessage(m_session, "Values are invalid. Value must be < max (if max exists), and both must be > 0.");
			return true;  
		}
	}

	Player *plr = getSelectedChar(m_session, false);
	if(plr)
	{  
		sGMLog.writefromsession(m_session, "used modify field value: %s, %u on %s", fieldname, av, plr->GetName());
		if(fieldmax)
		{
			BlueSystemMessage(m_session, "You set the %s of %s to %d/%d.", fieldname, plr->GetName(), av, mv);
			GreenSystemMessageToPlr(plr, "%s set your %s to %d/%d.", m_session->GetPlayer()->GetName(), fieldname, av, mv);
		}
		else
		{
			BlueSystemMessage(m_session, "You set the %s of %s to %d.", fieldname, plr->GetName(), av);
			GreenSystemMessageToPlr(plr, "%s set your %s to %d.", m_session->GetPlayer()->GetName(), fieldname, av);
		}

		if(field == UNIT_FIELD_STAT1) av /= 2;
		if(field == UNIT_FIELD_BASE_HEALTH) 
		{
			plr->SetUInt32Value(UNIT_FIELD_HEALTH, av);
		}

		plr->SetUInt32Value(field, av);

		if(fieldmax) {
			plr->SetUInt32Value(fieldmax, mv);
		}
	}
	else
	{
		Creature *cr = getSelectedCreature(m_session, false);
		if(cr)
		{
			if(!(field < UNIT_END && fieldmax < UNIT_END)) return false;
			std::string creaturename = "Unknown Being";
			if(cr->GetCreatureName())
				creaturename = cr->GetCreatureName()->Name;
			if(fieldmax)
				BlueSystemMessage(m_session, "Setting %s of %s to %d/%d.", fieldname, creaturename.c_str(), av, mv);
			else
				BlueSystemMessage(m_session, "Setting %s of %s to %d.", fieldname, creaturename.c_str(), av);
			sGMLog.writefromsession(m_session, "used modify field value: [creature]%s, %u on %s", fieldname, av, creaturename.c_str());
			if(field == UNIT_FIELD_STAT1) av /= 2;
			if(field == UNIT_FIELD_BASE_HEALTH) 
				cr->SetUInt32Value(UNIT_FIELD_HEALTH, av);

			switch(field)
			{
			case UNIT_FIELD_FACTIONTEMPLATE:
				{
					if(cr->m_spawn)
						WorldDatabase.Execute("UPDATE creature_spawns SET faction = %u WHERE entry = %u", av, cr->m_spawn->entry);
				}break;
			case UNIT_NPC_FLAGS:
				{
					if(cr->proto)
						WorldDatabase.Execute("UPDATE creature_proto SET npcflags = %u WHERE entry = %u", av, cr->proto->Id);
				}break;
			}

			cr->SetUInt32Value(field, av);

			if(fieldmax) {
				cr->SetUInt32Value(fieldmax, mv);
			}
			// reset faction
			if(field == UNIT_FIELD_FACTIONTEMPLATE)
				cr->_setFaction();

			cr->SaveToDB();
		}
		else
		{
			RedSystemMessage(m_session, "Invalid Selection.");
		}
	}
	return true;
}

bool ChatHandler::CmdSetFloatField(WorldSession *m_session, uint32 field, uint32 fieldmax, const char *fieldname, const char *args)
{
	char* pvalue = strtok((char*)args, " ");
	float mv, av;

	if (!pvalue)
		return false;
	else
		av = (float)atof(pvalue);

	if(fieldmax)
	{
		char* pvaluemax = strtok(nullptr, " ");   
		if (!pvaluemax)
			return false;  
		else
			mv = (float)atof(pvaluemax);
	}
	else
	{
		mv = 0;
	}

	if (av <= 0)
	{  
		RedSystemMessage(m_session, "Values are invalid. Value must be < max (if max exists), and both must be > 0.");
		return true;   
	}
	if(fieldmax)
	{
		if(mv < av || mv <= 0)
		{
			RedSystemMessage(m_session, "Values are invalid. Value must be < max (if max exists), and both must be > 0.");
			return true;  
		}
	}

	Player *plr = getSelectedChar(m_session, false);
	if(plr)
	{  
		sGMLog.writefromsession(m_session, "used modify field value: %s, %f on %s", fieldname, av, plr->GetName());
		if(fieldmax)
		{
			BlueSystemMessage(m_session, "You set the %s of %s to %.1f/%.1f.", fieldname, plr->GetName(), av, mv);
			GreenSystemMessageToPlr(plr, "%s set your %s to %.1f/%.1f.", m_session->GetPlayer()->GetName(), fieldname, av, mv);
		}
		else
		{
			BlueSystemMessage(m_session, "You set the %s of %s to %.1f.", fieldname, plr->GetName(), av);
			GreenSystemMessageToPlr(plr, "%s set your %s to %.1f.", m_session->GetPlayer()->GetName(), fieldname, av);
		}
		plr->SetFloatValue(field, av);
		if(fieldmax) plr->SetFloatValue(fieldmax, mv);
	}
	else
	{
		Creature *cr = getSelectedCreature(m_session, false);
		if(cr)
		{
			if(!(field < UNIT_END && fieldmax < UNIT_END)) return false;
			std::string creaturename = "Unknown Being";
			if(cr->GetCreatureName())
				creaturename = cr->GetCreatureName()->Name;
			if(fieldmax)
				BlueSystemMessage(m_session, "Setting %s of %s to %.1f/%.1f.", fieldname, creaturename.c_str(), av, mv);
			else
				BlueSystemMessage(m_session, "Setting %s of %s to %.1f.", fieldname, creaturename.c_str(), av);
			cr->SetFloatValue(field, av);
			sGMLog.writefromsession(m_session, "used modify field value: [creature]%s, %f on %s", fieldname, av, creaturename.c_str());
			if(fieldmax) {
				cr->SetFloatValue(fieldmax, mv);
			}
			//cr->SaveToDB();
		}
		else
		{
			RedSystemMessage(m_session, "Invalid Selection.");
		}
	}
	return true;
}

bool ChatHandler::HandleGetPosCommand(const char* args, WorldSession *m_session)
{
	/*if(m_session->GetPlayer()->GetSelection() == 0) return false;
	Creature *creature = objmgr.GetCreature(m_session->GetPlayer()->GetSelection());

	if(!creature) return false;
	BlueSystemMessage(m_session, "Creature Position: \nX: %f\nY: %f\nZ: %f\n", creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ());
	return true;*/

	uint32 spell = atol(args);
	SpellEntry *se = dbcSpell.LookupEntry(spell);
	if(se)
		BlueSystemMessage(m_session, "SpellIcon for %d is %d", se->Id, se->dummy);
	return true;
}


bool ChatHandler::HandleDebugRetroactiveQuestAchievements(const char *args, WorldSession *m_session)
{
	Player * pTarget = getSelectedChar(m_session, true );
	if(!pTarget) return true;

	//pTarget->RetroactiveCompleteQuests();
	m_session->GetPlayer()->BroadcastMessage("Done.");
	return true;
}
