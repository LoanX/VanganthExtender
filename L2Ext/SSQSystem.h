#pragma once

struct SSQPart
{
	static const int none = 0;
	static const int twilight = 1;	//Dusk
	static const int dawn = 2;	//Dawn
};

namespace SSQSeal
{
	static const int none = 0;
	static const int seal1 = 1;	//Avarice
	static const int seal2 = 2;	//Gnosis
	static const int seal3 = 3;	//Strife
};

class SSQSystem
{
public:
	static SSQSystem* GetInstance();
	int GetWinner();
	int GetSealOwner(int nSealNum);
	bool AddPoint(User *pUser, int nPart, int nPoint)
	{
		if(this)
		{
			typedef bool (*f)(SSQSystem*, User*, int, int);
			return f(0x7D7D00L)(this, pUser, nPart, nPoint);
		}
		return false;
	};
	bool ChangeUserPoint(User *pUser, int nPoint)
	{
		if(this)
		{
			typedef bool (*f)(SSQSystem*, User*, int);
			return f(0x7D8B30L)(this, pUser, nPoint);
		}
		return false;
	}
	static void ValidateCpSkill(User *pUser);
};