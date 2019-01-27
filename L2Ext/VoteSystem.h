#ifndef VOTESYSTEM_H
#define VOTESYSTEM_H
#include "VotePage.h"

typedef list<CVotePage*> VotePageList;

class CVoteSystem
{
protected:
	CRITICAL_SECTION m_lock;
private:
	inline void Lock() { EnterCriticalSection(&m_lock); }
	inline void Unlock() { LeaveCriticalSection(&m_lock); }

	BOOL m_enabled;
	VotePageList m_votePages;
	void LoadINI();
public:
	CVoteSystem();
	~CVoteSystem();
	void Init();
	void TimerExpired();
};

extern CVoteSystem g_VoteSystem;

#endif

