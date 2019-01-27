#pragma once


class CTask
{
public:
	enum TYPE
	{
		NONE,
		PRIVATE_MESSAGE,
		KICK,
		GIVE_ITEM,
		DELETE_ITEM,
	};
	enum ERROR_CODE
	{
		DONE,
		INVALID_TASK_TYPE,
		CANNOT_FIND_CHARACTER,
		INVALID_ITEM_COUNT,
		INVALID_MESSAGE,
		INVALID_PASSWORD,
		INVALID_PARAMETERS
	};
	CTask(void);
	virtual ~CTask(void);
	virtual TYPE GetType();
	virtual void SetType(TYPE Type);
	virtual ERROR_CODE Do();
private:
	TYPE type;
};

class CPrivateMessageTask : public CTask
{
	wstring wSender;
	wstring wReceiver;
	wstring wMessage;
public:
	CPrivateMessageTask();
	CPrivateMessageTask(wstring Sender, wstring Receiver, wstring Message);
	virtual ERROR_CODE Do();
	wstring GetSender();
	wstring GetReceiver();
	wstring GetMessage();
};

class CKickTask : public CTask
{
	wstring wName;
public:
	CKickTask();
	CKickTask(wstring Name);
	virtual ERROR_CODE Do();
	wstring GetPlayerName();
};

class CGiveItemTask : public CTask
{
	wstring wName;
	int ItemID;
	int ItemCount;
public:
	CGiveItemTask();
	CGiveItemTask(wstring Name, int ID, int Count);
	~CGiveItemTask();
	virtual ERROR_CODE Do();
};