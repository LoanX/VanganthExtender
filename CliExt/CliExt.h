// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CLIEXT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CLIEXT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CLIEXT_EXPORTS
#define CLIEXT_API __declspec(dllexport)
#else
#define CLIEXT_API __declspec(dllimport)
#endif


CLIEXT_API BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);
void InitializeExtender(HMODULE hModule);