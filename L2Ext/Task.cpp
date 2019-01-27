#include "StdAfx.h"

CTask::CTask()
{
	type = NONE;
};

CTask::~CTask()
{
}

CTask::TYPE CTask::GetType()
{
	return type;
}

void CTask::SetType(CTask::TYPE Type)
{
	type = Type;
}

CTask::ERROR_CODE CTask::Do()
{
	g_Log.Add(CLog::Error, "[%s] for type [%d]", __FUNCTION__, type);
	return INVALID_TASK_TYPE;
}

//CPrivateMessageTask

CPrivateMessageTask::CPrivateMessageTask()
{
	SetType(PRIVATE_MESSAGE);
}

CPrivateMessageTask::CPrivateMessageTask(std::wstring Sender, std::wstring Receiver, std::wstring Message)
{
	SetType(PRIVATE_MESSAGE);
	wSender = Sender;
	wReceiver = Receiver;
	wMessage = Message;
}

wstring CPrivateMessageTask::GetReceiver()
{
	return wReceiver;
}

wstring CPrivateMessageTask::GetSender()
{
	return wSender;
}

wstring CPrivateMessageTask::GetMessage()
{
	return wMessage;
}

CTask::ERROR_CODE CPrivateMessageTask::Do()
{
	User *pUser = g_UserDB.GetUserByName(wReceiver);
	if(pUser->ValidUser())
	{
		pUser->SendSystemMessage(wSender.c_str(), wMessage.c_str());
		g_Logger.Add(L"[%S] Sender[%s] Receiver[%s] Message[%s]", __FUNCTION__, wSender.c_str(), wReceiver.c_str(), wMessage.c_str());
	}else
	{
		g_Log.Add(CLog::Error, "[%s] User[%S] cannot be found. Sender[%S]", __FUNCTION__, wReceiver.c_str(), wSender.c_str());
		return CANNOT_FIND_CHARACTER;
	}
	return DONE;
}

CKickTask::CKickTask()
{
	SetType(KICK);
}

CKickTask::CKickTask(std::wstring Name)
{
	SetType(KICK);
	wName = Name;
}

CTask::ERROR_CODE CKickTask::Do()
{
	User *pUser = g_UserDB.GetUserByName(wName);
	if(pUser->ValidUser())
	{
		pUser->pSocket->Close();
		g_Logger.Add(L"[%S] User[%s]", __FUNCTION__, wName.c_str());
	}else
	{
		g_Log.Add(CLog::Error, "[%s] User[%S] cannot be found.", __FUNCTION__, wName.c_str());
		return CANNOT_FIND_CHARACTER;
	}

	return DONE;
}

wstring CKickTask::GetPlayerName()
{
	return wName;
}

CGiveItemTask::CGiveItemTask()
{
	ItemID = 0;
	ItemCount = 0;
}

CGiveItemTask::CGiveItemTask(std::wstring Name, int ID, int Count)
{
	wName = Name;
	ItemID = ID;
	ItemCount = Count;
}

CGiveItemTask::~CGiveItemTask()
{
}

CTask::ERROR_CODE CGiveItemTask::Do()
{
	User *pUser = g_UserDB.GetUserByName(wName);
	if(pUser->ValidUser())
	{
		if(ItemID > 0 && ItemCount > 0)
		{
			pUser->AddItemToInventory(ItemID, ItemCount);
			g_Logger.Add(L"[%S] User[%s] item_id[%d] count[%d]", __FUNCTION__, wName.c_str(), ItemID, ItemCount);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] Invalid item id/count [%d][%d]", __FUNCTION__, wName.c_str(), ItemID, ItemCount);
			return INVALID_ITEM_COUNT;
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] User[%S] cannot be found.", __FUNCTION__, wName.c_str());
		return CANNOT_FIND_CHARACTER;
	}

	return DONE;
}