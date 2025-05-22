#include "client.h"

ClientTime clienttime;

typedef list<ScheduledCommand> ScheduleList;
ScheduleList scheduleList;

float  ClientTime::mapTime    = 1;
double ClientTime::current    = 1;
unsigned int ClientTime::current_ms = 1;

void ClientTime::reportMapTime(float mapTimeNew)
{
	mapTime = mapTimeNew;
	updateCurrentTime();
}

void ClientTime::updateCurrentTime()
{
	current_ms = timeGetTime();
	current = (double)current_ms / 1000.0;
}

void Clienttime()
{
	ClientTime::reportMapTime(g_Engine.GetClientTime());
	for( ScheduleList::iterator pos = scheduleList.begin();
	pos != scheduleList.end();
	++pos)
	{
		if( (*pos).timer.expired() )
		{
			cmd.exec( const_cast<char*>((*pos).cmd.c_str()) );
			scheduleList.erase(pos);
			break;
		}	
	}
}