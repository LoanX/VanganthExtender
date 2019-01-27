#pragma once

namespace License
{
	enum LicenseType
	{
		LicenseInstanceZone = 0,
		LicensePvPAnnounce,
		LicenseHtmlPIN,
		LicenseRebirth,
		LicenseSubStack,
		LicenseChampionNpc,
		LicenseDBNpcMaker,
		LicenseRecipeManager,
		LicenseExtUse,
		LicenseSkillMaster,
		LicenseChatManager
	};
	class CLicense
	{
	public:
		static CLicense& GetInstance();
		bool CanUse(LicenseType x);
		void SendPostRequest();
		string GetPostReply();
		void Init();
	};

}