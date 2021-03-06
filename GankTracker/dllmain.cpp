#include "stdafx.h"
#include "Windows.h"
#include "sdkapi.h"
#include <sal.h>
#include <algorithm>
#include <string>
#include <vector>
//Defina
#pragma once
#pragma warning(push)
#pragma warning(disable: 4201)
//Code
PSDK_CONTEXT SDK_CONTEXT_GLOBAL;
void* localPlayer = NULL;
void* localtarget = NULL;
//#define SdkDrawLine(StartWorld, EndWorld, Width, Color, Texture)
bool c_Gank;
int target;
bool SDKCOLOR
bool PSDKCOLOR
int texture = false

typedef union _SDKCOLOR
{
	struct
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
		unsigned char A;
	};

	unsigned int BGRA;
} SDKCOLOR, *PSDKCOLOR;


namespace GankPlayer {
	void Execute() {
		if (!SDKSTATUS_SUCCESS(SdkDrawLine(localPlayer(), localtarget(), 1, _SDKCOLOR(255, 255, 255, 255), texture))) { 
			SdkUiConsoleWrite("[SDK] Error! Could not move to pos.\n");
		}
	}
}

void __cdecl OverMenuScene(_In_ void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	SdkUiCheckbox("GankTracker", &c_Gank, NULL);

}

void __cdecl AwObjectLoop(_In_ void* Object, _In_opt_ void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);

	if (!SDKSTATUS_SUCCESS(SdkIsObjectHero_t(Object)))
	{
		return true;
	}

	bool targetnotidead;
	SdkIsObjectDead(Object, &targetnotidead);
	if (targetnotidead)
	{
		return true;
	}
	bool NoInForg;
	SdkIsUnitVisible(Object, &NoInForg);
	if (!NoInForg)
	
		return true;
	}

	SDKVECTOR Position;
	SdkGetObjectPosition(Object, &Position);
}


void __cdecl GankOnTick(void* UserData) {
	UNREFERENCED_PARAMETER(UserData);

	if (c_Gank) {
		GankPlayer::Execute();
	}
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD worldFd, _In_ LPVOID lpvReserved)

{
	UNREFERENCED_PARAMETER(hinstDLL);

	if (worldFd != DLL_PROCESS_ATTACH)
		return TRUE;

	SDK_EXTRACT_CONTEXT(lpvReserved);

	if (!SDK_CONTEXT_GLOBAL || !SDKSTATUS_SUCCESS(SdkNotifyLoadedModule("GankTracker", SDK_VERSION)))
		return FALSE;

	SdkRegisterOverlayScene(OverMenuScene, AwObjectLoop, NULL);

	SdkRegisterGameScene(GankOnTick, NULL);


	return TRUE;
}
