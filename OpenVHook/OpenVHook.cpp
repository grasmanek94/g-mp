#include "Input\InputHook.h"
#include "Scripting\ScriptEngine.h"
#include "Scripting\ScriptManager.h"
#include "Utility\Console.h"
#include "Utility\General.h"
#include "Utility\Pattern.h"

using namespace Utility;

void External_Init(HINSTANCE hModule);
void External_Cleanup();

DWORD WINAPI Run() {

#ifdef _DEBUG
	GetConsole()->Allocate();
#endif

	LOG_PRINT( "Initializing..." );

	if ( !InputHook::Initialize() ) {

		LOG_ERROR( "Failed to initialize InputHook" );
		return 0;
	}

	if ( !ScriptEngine::Initialize() ) {

		LOG_ERROR( "Failed to initialize ScriptEngine" );
		return 0;
	}

	ScriptEngine::CreateThread( &g_ScriptManagerThread );

	//INIT_HERE
	External_Init(Utility::GetOurModuleHandle());

	LOG_PRINT( "Initialization finished" );

	return 1;
}

void Cleanup() {

	LOG_PRINT( "Cleanup" );

	External_Cleanup();
	// Maybe kill threads n shit

	InputHook::Remove();

	if ( GetConsole()->IsAllocated() ) {
		GetConsole()->DeAllocate();
	}
}

BOOL APIENTRY DllMain( HINSTANCE hModule, DWORD dwReason, LPVOID lpvReserved ) {

	switch ( dwReason ) {
		case DLL_PROCESS_ATTACH: {

			SetOurModuleHanlde( hModule );
			CloseHandle( CreateThread( NULL, NULL, (LPTHREAD_START_ROUTINE)Run, NULL, NULL, NULL ) );
			break;
		}
		case DLL_PROCESS_DETACH: {

			Cleanup();
			break;
		}
	}

	return TRUE;
}
