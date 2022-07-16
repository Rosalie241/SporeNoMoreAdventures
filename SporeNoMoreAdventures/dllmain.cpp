//
// SporeCrashFix - https://github.com/Rosalie241/SporeFixCrash
//  Copyright (C) 2021 Rosalie Wanders <rosalie@mailbox.org>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 3.
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see <https://www.gnu.org/licenses/>.
//

// dllmain.cpp : Defines the entry point for the DLL application.
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"

#include <Windows.h>

using namespace Simulator;

//
// Helper Functions
//

static void DisplayError(const char* fmt, ...)
{
	char buf[200];

	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	MessageBoxA(NULL, buf, "SporeNoMoreAdventures", MB_OK | MB_ICONERROR);
}

//
// Detoured Functions
//

class cMissionManager
{
public:
	// padding
	int m1; int m2;	int m3;	int m4;	int m5;	int m6; int m7; int m8; int m9; int m10;
	int m11; int m12; int m13; int m14; int m15; int m16; int m17; int m18; int m19;
	int m20; int m21; int m22; int m23;

	// 0x5C
	eastl::map<uint32_t, int> m_missionMap;
};

member_detour(ChooseMissionFunctionDetour, cMissionManager, cMission*(cEmpire*, cPlanetRecord*))
{
	cMission* detoured(cEmpire* cEmpire, cPlanetRecord* cPlanetRecord)
	{
		// set mission Adventures101 to completed
		m_missionMap[0x2AB57FAC] = 1;

		return original_function(this, cEmpire, cPlanetRecord);
	}
};


//
// Exported Functions
//

void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
}

void Dispose()
{
	// This method is called when the game is closing
}

void AttachDetours()
{
	ChooseMissionFunctionDetour::attach(Address(ModAPI::ChooseAddress(0xFEF450, 0xFEEBC0)));

	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

