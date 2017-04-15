/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
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

#ifndef __POINTERLOGGER_H
#define __POINTERLOGGER_H

//#define POINTER_LOGGER

#ifdef POINTER_LOGGER

#define POINTERLOGNEW(x) \
	CStackWalker walker; \
	walker.ShowCallstack(); \
	FILE* pointerlog=fopen("PointerLog.txt", "a"); \
	if (pointerlog == nullptr) return; \
	fprintf(pointerlog, "New pointer: 0x%p\nCallstack: %s\n", x, walker.CallBack.c_str()); \
	fclose(pointerlog);

#define POINTERLOGDELETE(x) \
	CStackWalker walker; \
	walker.ShowCallstack(); \
	FILE* pointerlog=fopen("PointerLog.txt", "a"); \
	if (pointerlog == nullptr) return; \
	fprintf(pointerlog, "Deleted pointer: 0x%p\nCallstack: %s\n", x, walker.CallBack.c_str()); \
	fclose(pointerlog);

#else

#define POINTERLOGNEW(x)
#define POINTERLOGDELETE(x)

#endif

#endif
