#include "StdAfx.h"

CTaskHandler::CTaskHandler()
{
};

CTaskHandler::~CTaskHandler()
{
};

void CTaskHandler::AddTask(CTask *pTask)
{
	Lock();
	try
	{
		if(pTask->GetType() != CTask::NONE)
		{
			lData.push_back(pTask);
		}else
			g_Log.Add(CLog::Error, "[%s] Trying to add invalid task.", __FUNCTION__);
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	Unlock();
}

CTask::ERROR_CODE CTaskHandler::Handle(CTask *pTask)
{
	return pTask->Do();
}

int CTaskHandler::GetTaskCount()
{
	int nSize = 0;
	Lock();
	nSize = (int)lData.size();
	Unlock();
	return nSize;
}

void CTaskHandler::TimerExpired()
{
	Lock();
	if(lData.size() > 0)
	{
		g_Log.Add(CLog::Blue, "[%s] Waiting Tasks[%d]", __FUNCTION__, (int)lData.size());
		for(TaskList::iterator Iter = lData.begin(); Iter != lData.end(); Iter++)
		{
			CTask *pTask = (*Iter);
			Handle(pTask);
			delete pTask;
		}
		lData.clear();
	}
	Unlock();
}

void CTaskHandler::CreateTask(PCHAR Buff, SOCKET s)
{
	//packet format
	//PrivateMessage d(Password)d(TaskType)S(Sender)S(Receiver)S(Message)
	//Kick d(Password)d(TaskType)S(UserName)
	//GiveItem d(Password)d(TaskType)S(UserName)d(ItemID)d(Count)
	CPacket packet((PUCHAR)Buff);
	CTask::ERROR_CODE ErrorCode = CTask::INVALID_PASSWORD;
	int Password = packet.ReadD();
	if(Password == g_AdminSocket.GetPassword())
	{
		ErrorCode = CTask::INVALID_TASK_TYPE;
		int TaskType = packet.ReadD();
		switch(TaskType)
		{
		case CTask::PRIVATE_MESSAGE:
			{
				wstring wSender = packet.ReadS();
				wstring wReceiver = packet.ReadS();
				wstring wMessage = packet.ReadS();
				if(wMessage.size() > 0 && wReceiver.size() > 0 && wSender.size() > 0)
				{
					CPrivateMessageTask Task(wSender, wReceiver, wMessage);
					ErrorCode = Handle(&Task);
				}else
					ErrorCode = CTask::INVALID_PARAMETERS;
				break;
			}
		case CTask::KICK:
			{
				wstring wUserName = packet.ReadS();
				if(wUserName.size() > 0)
				{
					CKickTask Task(wUserName);
					ErrorCode = Handle(&Task);
				}else
					ErrorCode = CTask::INVALID_PARAMETERS;
				break;
			}
		case CTask::GIVE_ITEM:
			{
				wstring wUser = packet.ReadS();
				int ItemID = packet.ReadD();
				int Count = packet.ReadD();
				if(wUser.size() > 0 && ItemID > 0 && Count > 0)
				{
					CGiveItemTask Task(wUser, ItemID, Count);
					ErrorCode = Handle(&Task);
				}else
					ErrorCode = CTask::INVALID_PARAMETERS;
				break;
			}
		case CTask::DELETE_ITEM:
			{
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] Invalid Task Type[%d]", __FUNCTION__, TaskType);
				break;
			}
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid password[%d]", __FUNCTION__, Password);
	}

	CPacket outPck;
	outPck.WriteH(6);
	outPck.WriteD(ErrorCode);
	send(s, (const char*)outPck.GetBuff(), (int)outPck.GetSize(), 0);
}