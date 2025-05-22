#ifndef TIMEHANDLING_H
#define TIMEHANDLING_H

class ClientTime 
{
public:
	static void   reportMapTime(float newMapTime);
	static float  mapTime; 

	static void   updateCurrentTime();
	static double current;    
	static unsigned int  current_ms; 
};
extern ClientTime clienttime;
extern void Clienttime();

class StopTimer
{
public:
	StopTimer() :expirationTime(0)       {}

	void  countdown( double seconds ) { expirationTime = ClientTime::current + seconds; }
	bool  running () { return (ClientTime::current<=expirationTime); }
	bool  expired () { return (ClientTime::current> expirationTime); }
	float timeleft() { return float(expirationTime-ClientTime::current);  }
	void  setExpired() { expirationTime=0;                           }
	void  setRunning() { expirationTime=1000000;                     }

protected:
	double expirationTime;
};

class RunningStopTimer : public StopTimer 
{ 
public: 
	RunningStopTimer(){expirationTime=1000000;} 
};

template < short Interval, int MaxEvents = 12 >
class EventCounter
{
public:
	
	int  get()
	{
		register int n = 0;
		while(reftime<ClientTime::current)
		{
			reftime += Interval;
			++n;
			if(n>MaxEvents) { reset(); return 1; }
		}
		return n;
	}

	EventCounter()  {reset();}

protected:
	void   reset() {reftime = ClientTime::current;}
	double reftime;
};

struct ScheduledCommand
{
	string cmd; 
	RunningStopTimer timer; 
};
typedef list<ScheduledCommand> ScheduleList;
extern ScheduleList scheduleList;

#endif
