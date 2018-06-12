#include "ljroomcallback.h"
#include "clogsetting.h"

void ZEGO::LIVEROOM::LJRoomCallback::OnInitSDK(int nError)
{

}

void ZEGO::LIVEROOM::LJRoomCallback::OnLoginRoom(int errorCode, const char *pszRoomID, const ZEGO::COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
    C_VALUE_LOG_INFO(errorCode);
    C_VALUE_LOG_INFO(pszRoomID);
    C_VALUE_LOG_INFO(streamCount);
//    C_VALUE_LOG_INFO(errorCode);
}

void ZEGO::LIVEROOM::LJRoomCallback::OnLogoutRoom(int errorCode, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJRoomCallback::OnKickOut(int reason, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJRoomCallback::OnDisconnect(int errorCode, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJRoomCallback::OnReconnect(int errorCode, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJRoomCallback::OnTempBroken(int errorCode, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJRoomCallback::OnStreamUpdated(ZEGO::COMMON::ZegoStreamUpdateType type, ZEGO::COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJRoomCallback::OnStreamExtraInfoUpdated(ZEGO::COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJRoomCallback::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJRoomCallback::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJAVEngineCallback::OnAVEngineStop()
{

}

ZEGO::LIVEROOM::LJAVEngineCallback::~LJAVEngineCallback(){}
