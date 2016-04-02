#include "Main.h"

//#define UNPROXY_DIRECTINPUT

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	srand((unsigned int)(time(0)));

	if(SharedUtility::IsProcessRunning(L"GTAVLauncher.exe") || SharedUtility::IsProcessRunning(L"GTA5.exe"))
	{
		MessageBox(NULL, L"Grand Theft Auto V is already running!", L"V+ Launcher", MB_SYSTEMMODAL | MB_ICONERROR | MB_OK);
		return 0;
	}

	wchar_t
		wszLauncherPath[MAX_PATH],
		wszGamePath[MAX_PATH],
		wszLibraryPath[MAX_PATH];

#ifdef UNPROXY_DIRECTINPUT
	wchar_t
		wszDirectInputProxyPath[MAX_PATH],
		wszMovedDirectInputProxyPath[MAX_PATH];

	bool bMovedDirectInputProxy = false;
#endif

	swprintf_s(wszLauncherPath, MAX_PATH, L"%s%s", SharedUtility::GetExecutablePath(), L"GTAVLauncher.exe");
	swprintf_s(wszGamePath, MAX_PATH, L"%s%s", SharedUtility::GetExecutablePath(), L"GTA5.exe");

	swprintf_s(wszLibraryPath, MAX_PATH, L"%s%s", SharedUtility::GetExecutablePath(), /*L"OpenVHook.dll"); */L"v-plus.dll");

#ifdef UNPROXY_DIRECTINPUT
	swprintf_s(wszDirectInputProxyPath, MAX_PATH, L"%s%s", SharedUtility::GetExecutablePath(), L"dinput8.dll");
	swprintf_s(wszMovedDirectInputProxyPath, MAX_PATH, L"%s%s", SharedUtility::GetExecutablePath(), L"__dinput8.dll");
#endif

	if(!SharedUtility::IsFileExists(wszLauncherPath) || !SharedUtility::IsFileExists(wszGamePath))
	{
		MessageBox(NULL, L"Can't find Grand Theft Auto V executables.\nPlease move V+ client to your game folder.", L"V+ Launcher", MB_SYSTEMMODAL | MB_ICONERROR | MB_OK);
		return 0;
	}

	if(!SharedUtility::IsFileExists(wszLibraryPath))
	{
		MessageBox(NULL, L"Can't find V+ library.", L"V+ Launcher", MB_SYSTEMMODAL | MB_ICONERROR | MB_OK);
		return 0;
	}

	DWORD dwCurrentProcessId = GetCurrentProcessId();

	while(SharedUtility::GetProcessIdFromProcessName(L"v-plus.exe", NULL, dwCurrentProcessId))
	{
		SharedUtility::KillProcess(L"v-plus.exe");

		Sleep(25);
	}

#ifdef UNPROXY_DIRECTINPUT
	if(SharedUtility::IsFileExists(wszDirectInputProxyPath))
	{
		MoveFile(wszDirectInputProxyPath, wszMovedDirectInputProxyPath);

		bMovedDirectInputProxy = true;
	}
#endif

	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInfo;

	memset(&startupInfo, 0x00, sizeof(STARTUPINFO));
	memset(&processInfo, 0x00, sizeof(PROCESS_INFORMATION));

	startupInfo.cb = sizeof(STARTUPINFO);

	if(!CreateProcess(NULL, wszLauncherPath, NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
#ifdef UNPROXY_DIRECTINPUT
		if(bMovedDirectInputProxy)
		{
			MoveFile(wszMovedDirectInputProxyPath, wszDirectInputProxyPath);

			bMovedDirectInputProxy = false;
		}
#endif

		MessageBox(NULL, L"Failed to start Grand Theft Auto V. (#1)\nTry running the launcher with higher privileges.", L"V+ Launcher", MB_SYSTEMMODAL | MB_ICONERROR | MB_OK);
		return 0;
	}

	Sleep(1000);

	while(SharedUtility::IsProcessRunning(L"GTAVLauncher.exe"))
	{
		if(SharedUtility::IsProcessRunning(L"GTA5.exe"))
		{
			break;
		}

		Sleep(50);
	}

	if(!SharedUtility::IsProcessRunning(L"GTAVLauncher.exe") || !SharedUtility::IsProcessRunning(L"GTA5.exe"))
	{
#ifdef UNPROXY_DIRECTINPUT
		if(bMovedDirectInputProxy)
		{
			MoveFile(wszMovedDirectInputProxyPath, wszDirectInputProxyPath);

			bMovedDirectInputProxy = false;
		}
#endif

		MessageBox(NULL, L"Failed to start Grand Theft Auto V. (#2)", L"V+ Launcher", MB_SYSTEMMODAL | MB_ICONERROR | MB_OK);
		return 0;
	}

	DWORD dwGameProcessId = NULL;

	if(!SharedUtility::GetProcessIdFromProcessName(L"GTA5.exe", &dwGameProcessId))
	{
#ifdef UNPROXY_DIRECTINPUT
		if(bMovedDirectInputProxy)
		{
			MoveFile(wszMovedDirectInputProxyPath, wszDirectInputProxyPath);

			bMovedDirectInputProxy = false;
		}
#endif

		MessageBox(NULL, L"Failed to start Grand Theft Auto V. (#3)", L"V+ Launcher", MB_SYSTEMMODAL | MB_ICONERROR | MB_OK);
		return 0;
	}

	if(!SharedUtility::InjectDllW(wszLibraryPath, dwGameProcessId))
	{
		SharedUtility::KillProcess(L"GTAVLauncher.exe");
		SharedUtility::KillProcess(L"GTA5.exe");

#ifdef UNPROXY_DIRECTINPUT
		if(bMovedDirectInputProxy)
		{
			MoveFile(wszMovedDirectInputProxyPath, wszDirectInputProxyPath);

			bMovedDirectInputProxy = false;
		}
#endif

		MessageBox(NULL, L"Failed to inject the V+ library into the game.\nTry running the launcher with higher privileges.", L"V+ Launcher", MB_SYSTEMMODAL | MB_ICONERROR | MB_OK);
		return 0;
	}

#ifdef UNPROXY_DIRECTINPUT
	if(bMovedDirectInputProxy)
	{
		MoveFile(wszMovedDirectInputProxyPath, wszDirectInputProxyPath);

		bMovedDirectInputProxy = false;
	}
#endif

	while(SharedUtility::IsProcessRunning(L"GTA5.exe"))
	{
		Sleep(500);
	}

	if(!SharedUtility::IsProcessRunning(L"GTA5.exe") && SharedUtility::IsProcessRunning(L"GTAVLauncher.exe"))
	{
		SharedUtility::KillProcess(L"GTAVLauncher.exe");
	}

	return 0;
}
