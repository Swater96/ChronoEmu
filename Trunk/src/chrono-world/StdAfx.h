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

#ifndef __STDAFX_H
#define __STDAFX_H

#define POINTER_LOGGING

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#define _GAME

template< class T, class U > T TO(U u) { return static_cast< T >(u); }

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
//#include <fstream>

#include "../chrono-shared/Common.h"
#include "../chrono-shared/MersenneTwister.h"
#include "../chrono-shared/WorldPacket.h"
#include "../chrono-shared/Log.h"
#include "../chrono-shared/NGLog.h"
#include "../chrono-shared/ByteBuffer.h"
#include "../chrono-shared/StackBuffer.h"
#include "../chrono-shared/Config/ConfigEnv.h"
#include "../chrono-shared/crc32.h"
#include "../chrono-shared/LocationVector.h"

#include "../chrono-shared/Collision/vmap/IVMapManager.h"
#include "../chrono-shared/Collision/vmap/VMapManager.h"

#include <zlib.h>

#include "../chrono-shared/Database/DatabaseEnv.h"
#include "../chrono-shared/Database/DBCStores.h"
#include "../chrono-shared/Database/dbcfile.h"

#include <Network/Network.h>

#include "../chrono-shared/Auth/MD5.h"
#include "../chrono-shared/Auth/BigNumber.h"
#include "../chrono-shared/Auth/Sha1.h"
#include "../chrono-shared/Auth/WowCrypt.h"
#include "../chrono-shared/CrashHandler.h"
#include "../chrono-shared/FastQueue.h"
#include "../chrono-shared/CircularQueue.h"
#include "../chrono-shared/Threading/RWLock.h"
#include "../chrono-shared/Threading/Condition.h"
#include "../chrono-shared/chrono_getopt.h"
#include "../chrono-shared/BufferPool.h"

// Warden System
#include "../chrono-shared/MersenneTwister.h"
#include "../chrono-shared/Auth/SARC4.h"
#include "../chrono-shared/Auth/HMACSHA1.h"
#include "../chrono-shared/Auth/WardenKeyGeneration.h"
#include "Warden/WardenBase.h"
#include "Warden/WardenDataStorage.h"
#include "Warden/WardenMac.h"
#include "Warden/WardenWin.h"

#include "Const.h"
#include "UpdateFields.h"
#include "UpdateMask.h"
#include "Opcodes.h"
#include "WorldStates.h"

#ifdef CLUSTERING
	#include "../heathstone-realmserver/WorkerOpcodes.h"
#endif

#include "SharedEnums.h"
#include "PointerLogger.h"
#include "Packets.h"

#include "../chrono-shared/CallBack.h"
#include "WordFilter.h"
#include "EventMgr.h"
#include "EventableObject.h"
#include "Object.h"
#include "LootMgr.h"
#include "Unit.h"
#include "WUtil.h"

#include "AddonMgr.h"
#include "AIInterface.h"
#include "AreaTrigger.h"
#include "BattlegroundMgr.h"
#include "AlteracValley.h"
#include "ArathiBasin.h"
#include "CellHandler.h"
#include "SpellSystem\ClassFixes.h"
#include "Chat.h"
#include "Corpse.h"
#include "Quest.h"
#include "QuestMgr.h"
#include "Creature.h"
#include "DynamicObject.h"
#include "GameObject.h"
#include "Group.h"
#include "Guild.h"
#include "HonorHandler.h"
#include "ItemPrototype.h"
#include "Item.h"
#include "Container.h"
#include "AuctionHouse.h"
#include "AuctionMgr.h"
#include "MailSystem.h"
#include "Map.h"
#include "MapCell.h"
#include "TerrainMgr.h"
#include "MiscHandler.h"
#include "NameTables.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "WorldSocket.h"
#include "WorldSession.h"
#include "WorldStateManager.h"
#include "MapMgr.h"
#include "MapScriptInterface.h"
#include "Player.h"
#include "faction.h"
#include "Skill.h"
#include "SkillNameMgr.h"
#include "SpellNameHashes.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellSystem/ClassFixes.h"
#include "TaxiMgr.h"
#include "TransporterHandler.h"
#include "WarsongGulch.h"
#include "WeatherMgr.h"
#include "World.h"
#include "ItemInterface.h"
#include "Stats.h"
#include "WorldCreator.h"
#include "ChainAggroEntity.h"

#include "ObjectMgr.h"

#include "ScriptMgr.h"

#include "Channel.h"
#include "ChannelMgr.h"

#include "LogonCommClient.h"
#include "LogonCommHandler.h"
#include "MainServerDefines.h"
#include "WorldRunnable.h"

#include "../chrono-shared/Storage.h"
#include "ObjectStorage.h"
#include "DatabaseCleaner.h"
#include "DayWatcherThread.h"
#include "LocalizationMgr.h"

#ifdef CLUSTERING
	#include "WorkerServerClient.h"
	#include "ClusterInterface.h"
#endif

#include "CollideInterface.h"

#include "Master.h"
#include "BaseConsole.h"
#include "CConsole.h"
//#define COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS 1

#endif
