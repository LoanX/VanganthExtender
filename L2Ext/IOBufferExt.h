#pragma once

extern "C"
{
	void ReallocateIOBuffer1();
	void ReallocateIOBuffer2();
	void ReallocateIOBuffer3();
	void ReallocateIOBuffer4();
	void ReallocateIOBuffer5();
	void ReallocateIOBuffer6();
	void ReallocateIOBuffer7();
	void ReallocateIOBuffer8();
	void ReallocateIOBuffer9();
	void ReallocateIOBuffer10();
	void ReallocateIOBuffer11();
	void ReallocateIOBuffer12();
	void ReallocateIOBuffer13();
	void ReallocateIOBuffer14();
	void ReallocateIOBuffer15();
	void ReallocateIOBuffer16();
	void ReallocateIOBuffer17();
	void ReallocateIOBuffer17();
	void ReallocateIOBuffer18();
	void ReallocateIOBuffer19();
	void ReallocateIOBuffer20();
	void ReallocateIOBuffer21();
	void ReallocateIOBuffer22();
	void ReallocateIOBuffer23();
	void ReallocateIOBuffer24();
	void ReallocateIOBuffer25();
	void ReallocateIOBuffer26();
	void ReallocateIOBuffer27();
	void ReallocateIOBuffer28();
	void ReallocateIOBuffer29();
	void ReallocateIOBuffer30();
	void ReallocateIOBuffer31();
	void ReallocateIOBuffer32();
	void ReallocateIOBuffer33();
	void ReallocateIOBuffer34();
	void ReallocateIOBuffer35();
	void ReallocateIOBuffer36();
	void ReallocateIOBuffer37();
	void ReallocateIOBuffer38();
	void ReallocateIOBuffer39();
	void ReallocateIOBuffer40();
	void ReallocateIOBuffer41();
	void ReallocateIOBuffer42();
	void ReallocateIOBuffer43();
	void ReallocateIOBuffer44();
	void ReallocateIOBuffer45();
	void ReallocateIOBuffer46();
	void ReallocateIOBuffer47();
	void ReallocateIOBuffer48();
	void ReallocateIOBuffer49();
	void ReallocateIOBuffer50();
	void ReallocateIOBuffer51();
	void ReallocateIOBuffer52();
	void ReallocateIOBuffer53();
	void ReallocateIOBuffer54();
	void ReallocateIOBuffer55();
	void ReallocateIOBuffer56();
	void ReallocateIOBuffer57();
	void ReallocateIOBuffer58();
	void ReallocateIOBuffer59();
	void ReallocateIOBuffer60();
	void ReallocateIOBuffer61();
	void ReallocateIOBuffer62();
	void ReallocateIOBuffer63();
	void ReallocateIOBuffer64();
	void ReallocateIOBuffer65();
	void ReallocateIOBuffer66();
	void ReallocateIOBuffer67();
	void ReallocateIOBuffer68();
	void ReallocateIOBuffer69();
	void ReallocateIOBuffer70();
	void ReallocateIOBuffer71();
	void ReallocateIOBuffer72();
	void ReallocateIOBuffer73();
};

class CIOBufferExt
{
public:
	static void Init();
	static void MemMove(UINT64 pDest, UINT64 pSrc, UINT64 size);
	static LPVOID Malloc(UINT64 size);
};

