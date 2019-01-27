#pragma once

extern "C"
{
	void ATHook();
	void ATHook2();
}

class ATSystem
{
public:
	static void Initialize();
};