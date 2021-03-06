#include "stdafx.h"
#include "sdkapi.h"
//#include "dect.h"
#include <random>

PSDK_CONTEXT SDK_CONTEXT_GLOBAL;

void* vLocalPlayer;
int HealSpell;

bool UseHeal;

namespace UtilityHeal
{
	void Execute() {
		bool bDead;
		SdkIsObjectDead(vLocalPlayer, &bDead);
		if (bDead)
			return;

		SDK_SPELL Spell;
		SdkGetAISpell(vLocalPlayer, HealSpell, &Spell);
		float Time;
		SdkGetGameTime(&Time);
		if (Time < Spell.CooldownExpires)
			return;

		SDK_HEALTH playerHealth;
		SdkGetUnitHealth(vLocalPlayer, &playerHealth);

		if (playerHealth.Current < 30)
		{
			bool canCast = false;
			SdkCanAICast(vLocalPlayer, &canCast);
			if (canCast && playerHealth.Current <= 30)
			{
				SdkCastSpellLocalPlayer(vLocalPlayer, NULL, (unsigned char)HealSpell, SPELL_CAST_START);
			}
		}
	}
}

void __cdecl DrawOverlayScene(_In_ void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	SdkUiCheckbox("Use Heal", &UseHeal, NULL);
}

void __cdecl UtilityHealTick(void* UserData) {
	UNREFERENCED_PARAMETER(UserData);

	if (UseHeal) {

		UtilityHeal::Execute();


	}
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	UNREFERENCED_PARAMETER(hinstDLL);

	if (fdwReason != DLL_PROCESS_ATTACH)
		return TRUE;

	SDK_EXTRACT_CONTEXT(lpvReserved);
	if (!SDK_CONTEXT_GLOBAL || !SDKSTATUS_SUCCESS(SdkNotifyLoadedModule("UtilityHeal", SDK_VERSION)))
		return FALSE;

	SdkRegisterOverlayScene(DrawOverlayScene, NULL);

	SdkRegisterGameScene(UtilityHealTick, NULL);

	SdkGetLocalPlayer(&vLocalPlayer);

	for (int i = 0; i < SPELL_SLOT_MAX; ++i)
	{
		SDK_SPELL Spell;
		SdkGetAISpell(vLocalPlayer, i, &Spell);
		if (strcmp(Spell.DisplayName, "game_spell_displayname_SummonerHeal") == 0)
		{
			HealSpell = Spell.Slot;
			break;
		}
	}
	return TRUE;
}
