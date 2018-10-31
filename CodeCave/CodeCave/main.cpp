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

bool bMsgBox = false;

void DllProcess() {
	printf("Finding Assembler Instruction Locations");

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
		"xxx??xxxxxxx?xx",
		"\x50\x8d\x4c\x00\x00\x51\x8b\xce\xff\xd2\x8b\x46\x00\x3b\x05");


	dwAddressAxis -= 3;

	printf("Writing Addresses to %s\n", szFile);

	char szUnAmmo[] = "Unlimited Ammo";
	char szSuJump[] = "Super Jump";
	char szUnHealth[] = "Unlimited Health";
	char szAmmoLim[] = "Ammo Limiter";
	char szSuperFullAuto[] = "Super Full Auto";
	char szNoRecoil[] = "No Recoil";

	// Writing to debug file
	AddToWriteBuffer(dwAddress, szUnAmmo);
	AddToWriteBuffer(dwAddressAxis, szSuJump);
	AddToWriteBuffer(dwHealthAddress, szUnHealth);
	AddToWriteBuffer(dwAmmoConstAddress, szAmmoLim);
	AddToWriteBuffer(dwAmmoSpeedAddress, szSuperFullAuto);
	AddToWriteBuffer(dwRecoilAddress, szNoRecoil);

	WriteToFile();

	if (bMsgBox) {
		printf("MessageBox Flag Recognized, Displaying Addresses...\n");

		MessageBoxAddress(dwAddress, false);
		MessageBoxAddress(dwAddressAxis, false);
		MessageBoxAddress(dwHealthAddress, false);
		MessageBoxAddress(dwAmmoConstAddress, false);
		MessageBoxAddress(dwAmmoSpeedAddress, false);
		MessageBoxAddress(dwRecoilAddress, false);
	}

	// JMP addresses after injected assembler
	printf("Writing Offests...\n");
	dwAmmoJmpBack = dwAddress + 0x7;
	dwAxisJmpBack = dwAddressAxis + 0x6;
	dwHealthJmpBack = dwHealthAddress + 0x6;
	dwAmmoConstJmpBack = dwAmmoConstAddress + 0x7;
	dwAmmoSpeedJmpBack = dwAmmoSpeedAddress + 0x7;
	dwRecoilJmpBack = dwRecoilAddress + 0xA;

	// Code Injection
	printf("Injecting Code...\n");
	WriteMemoryJmp((BYTE*)dwAddress, (DWORD)InfiniteAmmo, 7);
	WriteMemoryJmp((BYTE*)dwAddressAxis, (DWORD)FlyHack, 6);
	WriteMemoryJmp((BYTE*)dwHealthAddress, (DWORD)HealthHack, 6);
	WriteMemoryJmp((BYTE*)dwAmmoConstAddress, (DWORD)AmmoAddressHack, 7);
	WriteMemoryJmp((BYTE*)dwAmmoSpeedAddress, (DWORD)AmmoSpeedHack, 7);
	WriteMemoryJmp((BYTE*)dwRecoilAddress, (DWORD)RecoilHack, 10);
}

bool __stdcall DllMain(HINSTANCE hInstance,
	DWORD fdwReason,
	LPVOID lpReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		Sleep(500);

		printf("Hack Initiated...\n");

		DisableThreadLibraryCalls(hInstance);
		DllProcess();

		// Launch controller threads
		_beginthreadex(0, 0, &FlyHackThread, 0, 0, 0);
		_beginthreadex(0, 0, &HealthHackThread, 0, 0, 0);
		_beginthreadex(0, 0, &AmmoHackThread, 0, 0, 0);

		printf("\nProcess Seems Intact...\nClosing Console In 5 Seconds...\n ");
		Sleep(5000);

		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_HIDE);

		FreeConsole();
		break;
	}

	return true;
}