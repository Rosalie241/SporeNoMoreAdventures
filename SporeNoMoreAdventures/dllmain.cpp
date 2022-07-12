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

//
// Helper functions
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
// Exported Functions
//

void Initialize()
{
	BOOL ret;
	DWORD_PTR address = (DWORD_PTR)GetModuleHandle(nullptr) + 0xBEEDA2;

	SIZE_T bytesWritten = 0;
	SIZE_T bytesRead = 0;
	const SIZE_T bytesSize = 31;
	BYTE readBytes[bytesSize] = { 0 };
	BYTE originalBytes[] =
	{
		0x8B, 0x44, 0x24, 0x40, 0x53, 0x55, 0x50, 0x68, 0xAC, 0x7F,
		0xB5, 0x2A, 0x8B, 0xCE, 0xE8, 0x6B, 0xD6, 0xFF, 0xFF, 0x89,
		0x44, 0x24, 0x14, 0x3B, 0xC3, 0x0F, 0x85, 0xF7, 0x01, 0x00,
		0x00
	};
	BYTE replacementBytes[] = 
	{
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0x90
	};

	ret = ReadProcessMemory(GetCurrentProcess(), (LPVOID)address, readBytes, bytesSize, &bytesRead);
	if (!ret)
	{
		DisplayError("ReadProcessMemory() Failed!");
		return;
	}

	if (bytesRead != bytesSize)
	{
		DisplayError("bytesRead != bytesSize");
		return;
	}

	// make sure we aren't replacing random bytes,
	// but the original bytes we expect
	if (memcmp(readBytes, originalBytes, bytesSize) != 0)
	{
		DisplayError("bytes don't match! Are you running a supported version of Spore?");
		return;
	}

	ret = WriteProcessMemory(GetCurrentProcess(), (LPVOID)address, replacementBytes, bytesSize, &bytesWritten);
	if (!ret)
	{
		DisplayError("WriteProcessMemory() Failed!");
		return;
	}

	if (bytesWritten != bytesSize)
	{
		DisplayError("bytesWritten != sizeof(bytes)");
		return;
	}


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

