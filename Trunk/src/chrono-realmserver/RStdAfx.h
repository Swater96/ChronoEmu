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

#pragma once

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

#include <zlib.h>

#include "../chrono-shared/Database/DatabaseEnv.h"
#include "../chrono-shared/Database/DBCStores.h"
#include "../chrono-shared/Database/dbcfile.h"
#include "../chrono-shared/FastQueue.h"

#include <Network/Network.h>

#include "../chrono-shared/Auth/MD5.h"
#include "../chrono-shared/Auth/BigNumber.h"
#include "../chrono-shared/Auth/Sha1.h"
#include "../chrono-shared/Auth/WowCrypt.h"
#include "../chrono-shared/CrashHandler.h"

#include "../chrono-world/NameTables.h"
#include "../chrono-world/Opcodes.h"
#include "../chrono-shared/CallBack.h"

#include "Master.h"
#include "WorkerOpcodes.h"
#include "WorkerServerSocket.h"
#include "Structures.h"
#include "WorldSocket.h"
#include "Session.h"
#include "WorkerServer.h"
#include "ClusterManager.h"
#include "ClientManager.h"
#include "LogonCommClient.h"
#include "LogonCommHandler.h"
#include "../chrono-shared/Storage.h"
#include "ObjectStorage.h"
