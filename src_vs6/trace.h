#ifndef TRACE_H
#define TRACE_H

struct strace_t
{
	bool finished;
	bool allsolid;
	bool startsolid;	
	float dist;
	float fraction;		
	float endpos[3];	
	bool hitsky;
};

mleaf_t* GetLeafFromPoint(float* point);
void TraceThickness(float* start, float* end, float thickness, strace_t* tr);

#endif
