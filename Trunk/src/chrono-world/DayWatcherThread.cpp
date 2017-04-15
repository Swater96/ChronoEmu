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

/* Arena and Honor Point Calculation System
 *    Copyright (c) 2007 Burlex
 */

#include "StdAfx.h"
#ifdef WIN32
static HANDLE m_abortEvent = INVALID_HANDLE_VALUE;
#else
static pthread_cond_t abortcond;
static pthread_mutex_t abortmutex;
#endif

DayWatcherThread::DayWatcherThread()
{
	m_dirty = false;
}

DayWatcherThread::~DayWatcherThread()
{

}

void DayWatcherThread::terminate()
{
	m_threadRunning = false;
#ifdef WIN32
	SetEvent(m_abortEvent);
#else
	pthread_cond_signal(&abortcond);
#endif
}

void DayWatcherThread::dupe_tm_pointer(tm * returnvalue, tm * mypointer)
{
	memcpy(mypointer, returnvalue, sizeof(tm));
}

void DayWatcherThread::update_settings()
{
	CharacterDatabase.Execute("REPLACE INTO server_settings VALUES(\"last_dailies_reset_time\", %u)", last_arena_time);
}

void DayWatcherThread::load_settings()
{

	QueryResult * result = CharacterDatabase.Query("SELECT setting_value FROM server_settings WHERE setting_id = \"last_dailies_reset_time\"");
	if(result)
	{
		last_daily_reset_time = result->Fetch()[0].GetUInt32();
		delete result;
	}
	else
		last_daily_reset_time = 0;
}

void DayWatcherThread::set_tm_pointers()
{

	dupe_tm_pointer(localtime(&last_daily_reset_time), &local_last_daily_reset_time);
}

uint32 DayWatcherThread::get_timeout_from_string(const char * string, uint32 def)
{
	if(!stricmp(string, "weekly"))
		return WEEKLY;
	else if(!stricmp(string, "monthly"))
		return MONTHLY;
	else if(!stricmp(string, "daily"))
		return DAILY;
	else if(!stricmp(string, "hourly"))
		return HOURLY;
	else
		return def;
}

bool DayWatcherThread::has_timeout_expired(tm * now_time, tm * last_time, uint32 timeoutval)
{
	switch(timeoutval)
	{
	case WEEKLY:
		{
			if( (now_time->tm_mon != last_time->tm_mon) )
				return true;
            
			return ( (now_time->tm_mday / 7) != (last_time->tm_mday / 7) );
		}
		
	case MONTHLY:
		return (now_time->tm_mon != last_time->tm_mon);

	case HOURLY:
		return ((now_time->tm_hour != last_time->tm_hour) || (now_time->tm_mday != last_time->tm_mday) || (now_time->tm_mon != last_time->tm_mon));

	case DAILY:
		return ((now_time->tm_mday != last_time->tm_mday) || (now_time->tm_mday != last_time->tm_mday));
	}
	return false;
}

bool DayWatcherThread::run()
{
	Log.Notice("DayWatcherThread", "Started.");
	currenttime = UNIXTIME;
	dupe_tm_pointer(localtime(&currenttime), &local_currenttime);
	load_settings();
	set_tm_pointers();
	m_busy = false;
#ifdef WIN32
	m_abortEvent = CreateEvent(nullptr, 0, FALSE, nullptr);
#else
	struct timeval now;
	struct timespec tv;

	pthread_mutex_init(&abortmutex,nullptr);
	pthread_cond_init(&abortcond,nullptr);
#endif
	
	while(m_threadRunning)
	{
		m_busy=true;
		currenttime = UNIXTIME;
		dupe_tm_pointer(localtime(&currenttime), &local_currenttime);

		if(has_timeout_expired(&local_currenttime, &local_last_daily_reset_time, DAILY))
			update_daily();
        
		if(m_dirty)
			update_settings();

		m_busy=false;
		if(!m_threadRunning)
			break;

#ifdef WIN32
		WaitForSingleObject(m_abortEvent, 120000);
#else
		gettimeofday(&now, nullptr);
		tv.tv_sec = now.tv_sec + 120;
		tv.tv_nsec = now.tv_usec * 1000;
		pthread_mutex_lock(&abortmutex);
		pthread_cond_timedwait(&abortcond, &abortmutex, &tv);
		pthread_mutex_unlock(&abortmutex);
#endif
		if(!m_threadRunning)
			break;
	}
#ifdef WIN32
	CloseHandle(m_abortEvent);
#else
	pthread_mutex_destroy(&abortmutex);
	pthread_cond_destroy(&abortcond);
#endif
	return true;
}

void DayWatcherThread::update_daily()
{
	last_daily_reset_time = UNIXTIME;
	dupe_tm_pointer(localtime(&last_daily_reset_time), &local_last_daily_reset_time);
	m_dirty = true;
}

