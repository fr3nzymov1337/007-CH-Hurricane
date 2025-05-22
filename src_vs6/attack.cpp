#include "client.h"

enum {FIRESEQ_INACTIVE=0,FIRESEQ_STARTDELAY=1,FIRESEQ_ACTIVE=2,FIRESEQ_RELEASEDELAY=3};

bool bAttacking = false;
static bool bAttack2 = false, bFireCycle  = true;
static int  nFireSequence = FIRESEQ_INACTIVE;

void AttackHandling()
{
	static double timer=0;

 	if( !cvar.burst_ftime ) { cvar.burst_ftime = 0.1f; }

	if (iTargetID!=0)
	{
		switch (nFireSequence)
		{
		case FIRESEQ_INACTIVE:
			if( cvar.burst_sdelay )
			{
				nFireSequence =  FIRESEQ_STARTDELAY;
				timer = ClientTime::current;

			} else {
				nFireSequence =  FIRESEQ_ACTIVE;
				timer = ClientTime::current;
				bFireCycle  = true;
				bAttacking = true;
			}
			break;
		case FIRESEQ_STARTDELAY:
			if( ClientTime::current >= (timer+cvar.burst_sdelay) )
			{
				nFireSequence =  FIRESEQ_ACTIVE;
				timer = ClientTime::current;
				bFireCycle  = true;
				bAttacking = true;
			}
			break;
		case FIRESEQ_ACTIVE:
			if( bFireCycle )
			{
				if( (ClientTime::current >= (timer+cvar.burst_ftime)) && (cvar.burst_dtime!=0))
				{
					bAttacking = false;
					timer = ClientTime::current;
					bFireCycle = false;
				}

			} else {
				if( ClientTime::current >= (timer+cvar.burst_dtime) )
				{
					bAttacking = true;
					timer = ClientTime::current;
					bFireCycle = true;
				}
			}
			break;
		case FIRESEQ_RELEASEDELAY:
			nFireSequence =  FIRESEQ_ACTIVE;
			timer = ClientTime::current;
			bFireCycle  = true;
			bAttacking = true;
			break;
		}
	} 
	else 
	{
		switch (nFireSequence)
		{
		case FIRESEQ_INACTIVE:
			break;
		case FIRESEQ_STARTDELAY:
			nFireSequence =  FIRESEQ_INACTIVE;
			break;
		case FIRESEQ_ACTIVE:
			if( !bFireCycle || (cvar.burst_rdelay==0) )
			{
				bAttacking = false;
				nFireSequence =  FIRESEQ_INACTIVE;

			} else {
				timer = ClientTime::current;
				nFireSequence = FIRESEQ_RELEASEDELAY;
			}
			break;
		case FIRESEQ_RELEASEDELAY:
			if( ClientTime::current >= (timer+cvar.burst_rdelay) )
			{
				bAttacking = false;
				nFireSequence =  FIRESEQ_INACTIVE;
			}
			break;
		}
	}
}
