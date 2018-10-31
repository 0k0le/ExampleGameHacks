/*
 * Project: CodeCave
 * File: main.cpp
 *
 * Author: Matthew Todd Geiger
 * 
 * Time: 01:17
 *
 * Brief: This file contains the main routine of the DLL
 */

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdlib.h>
#include <TlHelp32.h>
#include <process.h>

#include "Signature.h"
#include "asmfunc.h"
#include "Threadfuncs.h"


void DllProcess() {
	// Find memory addresses
	DWORD dwAddress = FindAddress("ac_client.exe",
		"xxxxx??xx??x????xxx?xxxxx??xxxxxxxxxxxxx",
		"\xff\x0e\x57\x8b\x7c\x00\x00\x8d\x74\x00\x00\xe8\x00\x00\x00\x00\x5f\x5e\xb0\x00\x5b\x8b\xe5\x5d\xc2\x00\x00\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\x55");

	DWORD dwAddressAxis = FindAddress("ac_client.exe",
		"xx?xx?xx?xx",
		"\x89\x4e\x00\xd9\x46\x00\xd8\x66\x00\x8b\xc6");

	DWORD dwHealthAddress = FindAddress("ac_client.exe",
		"xx????xx?xx",
		"\x8b\x86\x00\x00\x00\x00\x83\xc4\x00\x50\x68");

	DWORD dwAmmoConstAddress = FindAddress("ac_client.exe",
		"xxxxx??x????x",
		"\x8b\x02\x50\x8d\x4c\x00\x00\x68\x00\x00\x00\x00\x51");

	DWORD dwAmmoSpeedAddress = FindAddress("ac_client.exe",
		"xx?????xx?xxxx",
		"\x0f\xbf\x00\x00\x00\x00\x00\x8b\x56\x00\x89\x0a\x8b\x76");

	DWORD dwRecoilAddress = FindAddress("ac_client.exe",
		"xx?xxxx",
		"\xd8\x4b\x00\xde\xc9\xd8\x4b");

	DWORD dwRecoilBackAddress = FindAddress("ac_client.exe",
		"xxxx?xx?xx??xx??xx??xx??xx??xxxx?xxx??xxxxxxx",
		"\x8b\x08\x8b\x50\x00\x8b\x40\x00\xd9\x5c\x00\x00\x89\x44\x00\x00\x89\x4c\x00\x00\x8d\x44\x00\x00\x89\x54\x00\x00\x8b\x16\x8b\x52\x00\x50\x8d\x4c\x00\x00\x51\x8b\xce\xff\xd2\x8b\x46");

	dwPublicRecoilAddress = dwRecoilAddress;

	dwAddressAxis -= 3;

	MessageBoxAddress(dwAddress, false);
	MessageBoxAddress(dwAddressAxis, false);
	MessageBoxAddress(dwHealthAddress, false);
	MessageBoxAddress(dwAmmoConstAddress, false);
	MessageBoxAddress(dwAmmoSpeedAddress, false);
	MessageBoxAddress(dwRecoilBackAddress, false);

	// JMP addresses after injected assembler
	dwAmmoJmpBack = dwAddress + 0x7;
	dwAxisJmpBack = dwAddressAxis + 0x6;
	dwHealthJmpBack = dwHealthAddress + 0x6;
	dwAmmoConstJmpBack = dwAmmoConstAddress + 0x7;
	dwAmmoSpeedJmpBack = dwAmmoSpeedAddress + 0x7;
	//dwRecoilJmpBack = dwRecoilAddress + 0x5;
	dwRecoilBackJmpBack = dwRecoilBackAddress + 0x8;

	WriteMemoryJmp((BYTE*)dwAddress, (DWORD)InfiniteAmmo, 7);
	WriteMemoryJmp((BYTE*)dwAddressAxis, (DWORD)FlyHack, 6);
	WriteMemoryJmp((BYTE*)dwHealthAddress, (DWORD)HealthHack, 6);
	WriteMemoryJmp((BYTE*)dwAmmoConstAddress, (DWORD)AmmoAddressHack, 7);
	WriteMemoryJmp((BYTE*)dwAmmoSpeedAddress, (DWORD)AmmoSpeedHack, 7);
	WriteMemoryJmp((BYTE*)dwRecoilBackAddress, (DWORD)RecoilBackHack, 8);
}

bool __stdcall DllMain(HINSTANCE hInstance,
					   DWORD fdwReason,
					   LPVOID lpReserved)
{
	switch(fdwReason){
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hInstance);
		DllProcess();

		// Launch controller threads
		_beginthreadex(0, 0, &FlyHackThread, 0, 0, 0);
		_beginthreadex(0, 0, &HealthHackThread, 0, 0, 0);
		_beginthreadex(0, 0, &AmmoHackThread, 0, 0, 0);
		break;
	}

	return true;
}