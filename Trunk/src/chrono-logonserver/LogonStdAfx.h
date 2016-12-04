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

#include "Common.h"
#include <Network/Network.h>

#include "../chrono-shared/Log.h"
#include "../chrono-shared/Util.h"
#include "../chrono-shared/ByteBuffer.h"
#include "../chrono-shared/Config/ConfigEnv.h"
#include <zlib.h>

#include "../chrono-shared/Database/DatabaseEnv.h"
#include "../chrono-shared/Database/DBCStores.h"
#include "../chrono-shared/Database/dbcfile.h"

#include "../chrono-shared/Auth/BigNumber.h"
#include "../chrono-shared/Auth/Sha1.h"
#include "../chrono-shared/Auth/WowCrypt.h"
#include "../chrono-shared/CrashHandler.h"

#include "LogonOpcodes.h"
#include "../chrono-logonserver/Main.h"
#include "../chrono-world/NameTables.h"
#include "AccountCache.h"
#include "PeriodicFunctionCall_Thread.h"
#include "../chrono-logonserver/AutoPatcher.h"
#include "../chrono-logonserver/AuthSocket.h"
#include "../chrono-logonserver/AuthStructs.h"
#include "../chrono-logonserver/LogonOpcodes.h"
#include "../chrono-logonserver/LogonCommServer.h"
#include "../chrono-logonserver/LogonConsole.h"
#include "../chrono-shared/WorldPacket.h"

// database decl
extern Database* sLogonSQL;
