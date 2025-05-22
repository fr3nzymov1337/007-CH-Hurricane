/////
///// CBOBY V2 [Revision by ETK]
/////
///// By [boy_scout][boyscout_etk@hotmail.com]
/////
///// [2011][www.etalking.com.ar]
/////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef  AUTOADJUST_H_
#define  AUTOADJUST_H_

class AutoAdjust
{
public:	
        bool MyFree(float* from,float* to);
        void NextPos(float *nowpos,float *viewangles,float step,float *newpos);
        void SetAngle(float *viewangles);
        bool IsMyfree(float step,float *viewangles);
        bool Isjumpfree(float step,float *viewangles);
        bool Jump(float *viewangles);
		bool direction(float *viewangles);
};

extern AutoAdjust autoadjust;

#endif