#include "stdafx.h"
#include "License.h"

#pragma optimize("",off)

namespace License
{
	CLicense& CLicense::GetInstance()
	{
		static CLicense m_instance;
		return m_instance;
	}
	void CLicense::Init()
	{

	}
	bool CLicense::CanUse(LicenseType x)
	{
		return true;
	}
	string CLicense::GetPostReply()
	{
		return "608f488597ee8de18664b975b04d8be8";
	}
	void CLicense::SendPostRequest()
	{

	}
}

#pragma optimize("",on)