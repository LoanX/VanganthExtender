#pragma once


class CUserExPacket
{
public:
	static bool Handle(CUserSocket *pSocket, const unsigned char* packet);
	static bool RequestListPartyMatchingWaitingRoom(CUserSocket *pSocket, const unsigned char* packet);
	static bool RequestExitPartyMatchingWaitingRoom(CUserSocket *pSocket, const unsigned char* packet);
	static bool AskJoinPartyRoom(CUserSocket *pSocket, const unsigned char* packet);
	static bool AnswerJoinPartyRoom(CUserSocket *pSocket, const unsigned char* packet);
	static bool RequestTargetGround(CUserSocket *pSocket, const unsigned char* packet);
};

static bool RequestExShowMPCCPartyMemberInfo(CUserSocket *pSocket, const unsigned char* packet);