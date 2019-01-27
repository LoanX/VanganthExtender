#include "stdafx.h"
#include "NWindow.h"


INT32 g_AutoType[200] = { 0 };
INT32 g_AutoTypeCount = 0;

CNWindow g_NWindow;

CNWindow::CNWindow() : m_Initialized(FALSE)
{
}

CNWindow::~CNWindow()
{
}

__declspec(naked) void IsAutoType()
{
	__asm
	{
			test eax, eax
			jz NotAuto
			mov ecx, 0
		Check:
			cmp eax, dword ptr[g_AutoType + ecx * 4]
			je Auto
			inc ecx
			cmp ecx, g_AutoTypeCount
			jl Check
			jmp NotAuto

		Auto:
			mov eax, 1
			retn 4

		NotAuto:
			mov eax, 0
			retn 4
	};
}

void CNWindow::Init()
{
	if(!m_Initialized)
	{
		m_Initialized = TRUE;
		const TCHAR* section = _T("AutoItem");

		TCHAR configFile[260] = { 0 };
		GetCurrentDirectory(sizeof(configFile), configFile);
		lstrcat(configFile, L"\\Client.ini");

		TCHAR temp[16484] = { 0 };
		if(GetPrivateProfileString(section, _T("AutoItemList"), 0, temp, 16384, configFile))
		{
			wstringstream itemStream(temp);
			int itemId = 0;
			while(itemStream >> itemId)
			{
				if(itemId > 0)
				{
					if(g_AutoTypeCount < 199)
					{
						g_AutoType[g_AutoTypeCount] = itemId;
						g_AutoTypeCount++;
					}else
					{
						MessageBox(0, L"Too many items in AutoItemList!", L"Client.ini", MB_OK | MB_TOPMOST);
						break;
					}
				}
			}

			if(g_AutoTypeCount > 0)
			{
				HMODULE hNWindow = GetModuleHandle(L"NWindow.dll");
				if(hNWindow)
				{
					UINT addr = (UINT)hNWindow;
					addr += 0x149F09;
					Memory::WriteJump(addr, IsAutoType);
					addr = (UINT)hNWindow;
					addr += 0x146CE0;
					Memory::WriteJump(addr, IsAutoType);
				}
			}
		}
	}
}