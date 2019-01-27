#pragma once


class CSharedRWLock
{
public:
	CSharedRWLock(const WCHAR* sName, DWORD dwNo, LPVOID reserved_psi, DWORD dwSpinCount)
	{
		typedef void(*_f)(CSharedRWLock*, const WCHAR*, DWORD, LPVOID, DWORD);
		_f f = (_f) 0x78E610;
		f(this, sName, dwNo, reserved_psi, dwSpinCount);
	}

	~CSharedRWLock()
	{
		typedef void (*_f)(CSharedRWLock*);
		_f f = (_f) 0x78DFB0;
		f(this);
	}
	void ReadLock(bool bConcession)
	{
		typedef void (*_f)(CSharedRWLock*, bool);
		_f f = (_f) 0x78E0E0;
		f(this, bConcession);
	}
	void WriteLock()
	{
		typedef void (*_wl)(CSharedRWLock*);
		_wl wl = (_wl) 0x0078E290;
		wl(this);
	}
	void Done()
	{
		typedef void (*_done)(CSharedRWLock*);
		_done done = (_done) 0x0078E420;
		done(this);
	};
};