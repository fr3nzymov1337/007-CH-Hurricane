#ifndef AIM_H
#define AIM_H

extern int iTargetID;
extern float fAimAngles[3];
extern float fOldFOV;
extern float fOldDistance;

extern float AimCalcFOV(Vector one,Vector two,float lena=0,float lenb=0);
extern void SortTargetByFov(int iIndex);
extern void SortTargetByDistance(int iIndex);
extern void Aimbot(usercmd_s *cmd);
extern void TriggerBot(usercmd_s *cmd);
extern void KnifeBot(usercmd_s *cmd, int i);
extern void AimAuto(usercmd_s *cmd, int i);
extern void ApplyAngles(usercmd_s *cmd, int i);
extern void DrawAim();
extern void CalcCLAngles(int i);
extern void CalcHUDAngles(int i);
extern int PathFree(float* from,float* to);

#endif