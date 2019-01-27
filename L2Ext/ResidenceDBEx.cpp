#include "StdAfx.h"

CResidenceDBEx::CResidenceDBEx()
{
}

CResidenceDBEx::~CResidenceDBEx()
{

}

void CResidenceDBEx::Load()
{
	ifstream file("..\\Script\\CastleDataEx.txt");
	if(file)
	{
		string sLine;
		int nCastle = 0;
		int nAgit = 0;
		while(getline(file, sLine))
		{
			if(sLine.find("//") == 0)
				continue;
			if(sLine.find("castle_begin") == 0)
			{
				ResidenceDataEx rd;
				memset(&rd, 0, sizeof(rd));
				rd.castle = TRUE;
				rd.residence_id = CParser::GetValueInt(sLine, "castle_id");
				rd.reward_count = CParser::GetValueInt(sLine, "win_reward_count");
				rd.reward_id = CParser::GetValueInt(sLine, "win_reward_id");
				mData.insert(pair<int, ResidenceDataEx>(rd.residence_id, rd));
				nCastle++;
			}else if(sLine.find("agit_begin") == 0)
			{
				ResidenceDataEx rd;
				memset(&rd, 0, sizeof(rd));
				rd.castle = FALSE;
				rd.residence_id = CParser::GetValueInt(sLine, "agit_id");
				rd.reward_count = CParser::GetValueInt(sLine, "win_reward_count");
				rd.reward_id = CParser::GetValueInt(sLine, "win_reward_id");
				mData.insert(pair<int, ResidenceDataEx>(rd.residence_id, rd));
				nAgit++;
			}
		}
		g_Log.Add(CLog::Blue, "[%s] Loaded %d Castles and %d Agits.", __FUNCTION__, nCastle, nAgit);
		file.close();
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Missing file [..\\Script\\CastleDataEx.txt]!", __FUNCTION__);
	}

	ResidenceDataEx invalid_res;
	memset(&invalid_res, 0, sizeof(invalid_res));
	mData.insert(pair<int, ResidenceDataEx>(invalid_res.residence_id, invalid_res));
}

const ResidenceDataEx& CResidenceDBEx::GetData(int residence_id)
{
	ResidenceExMap::iterator Iter = mData.find(residence_id);
	if(Iter!=mData.end())
		return Iter->second;
	
	return mData[0];
}