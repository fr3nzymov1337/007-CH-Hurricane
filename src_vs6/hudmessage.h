#ifndef HUD_H
#define HUD_H

extern void DrawHudText();
extern void gSetHudMessage(const char* message);
extern StopTimer gHudTimer;
extern char gHudMessage[256];

#endif

