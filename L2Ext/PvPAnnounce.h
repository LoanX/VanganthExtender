#pragma once

namespace PvP
{
	class CPvPAnnounce
	{
		bool m_Enabled;
		bool m_IncludePK;
		bool m_IncludeBattleZone;
		time_t m_KillReuse;
		typedef struct
		{
			INT32 killCount;
			INT32 messageId;
			UINT range;
		} MessageInfo;
		typedef vector<MessageInfo> Messages;
		Messages m_Messages;

		CPvPAnnounce();
		CPvPAnnounce(const CPvPAnnounce& copy);
		void LoadINI();
		const MessageInfo& GetMessage(INT32 killCount) const;
	public:
		static CPvPAnnounce& GetInstance();
		void Init();
		bool OnKill(User *pKiller, User *pVictim, bool isPK, bool battleZone) const;
		inline bool Enabled() { return m_Enabled; }
	};
}