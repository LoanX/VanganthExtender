#pragma once

class CLotteryManager
{
	bool m_Enabled;
	int m_PriceId;
	int m_RewardId;
	double m_RewardMultipler;
	CLotteryManager();
	CLotteryManager(const CLotteryManager& other);
public:
	static CLotteryManager& Get();
	void Init();
	inline bool Enabled() { return m_Enabled; }
	inline int PriceId() { return m_PriceId; }
	inline int RewardId() { return m_RewardId; }
	inline double RewardMultipler() { return m_RewardMultipler; }
	static void OnSEAddAtomicItemProcess_BuyTicket(int userIndex, int itemId, int amount, int enchant, int blessed);
	static int OnCalculateReward(PVOID instancePtr, int roundNumber, int flagNumber, PUINT statePtr);
	static void OnSEAddAtomicItemProcess_GiveReward(int userIndex, int itemId, int amount, int enchant, int blessed);
	static int OnGetAccumulatedReward(PVOID instancePtr);

};