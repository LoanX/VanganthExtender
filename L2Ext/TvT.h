#ifndef TVT_H
#define TVT_H
#include "TvTMatch.h"

namespace TvT
{
	class CTvT : public CThreadSafe
	{
	private:
		BOOL m_enabled;
		list<TvT::TvTInfo*> m_tvtInfo;
		
		TvT::CMatch* m_lpEvent;

		void LoadINI();
	public:
		CTvT();
		~CTvT();
		void Init();
		void TimerExpired();
		void OnExit(User *pUser);
		void Register(User *pUser, int team);
		void Unregister(User *pUser);
		void OnDie(User *pUser, CCreature *pKiller);
		void OnNpcSpawn(CNPC *pNpc);	//NotUsed
		void TimerExpired(User *pUser);
		bool CanUse(CItem *pItem);
		void ValidateItems(User *pUser);
		bool IsSkillBlocked(INT32 skillId);
	};
};

extern TvT::CTvT g_TvT;

#endif
