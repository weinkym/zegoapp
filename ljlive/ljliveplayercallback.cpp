#include "ljliveplayercallback.h"


void ZEGO::LIVEROOM::LJLivePlayerCallback::OnPlayStateUpdate(int stateCode, const char *pszStreamID)
{

}

void ZEGO::LIVEROOM::LJLivePlayerCallback::OnPlayQualityUpdate(const char *pszStreamID, int quality, double videoFPS, double videoKBS)
{

}

void ZEGO::LIVEROOM::LJLivePlayerCallback::OnPlayQualityUpdate(const char *pszStreamID, ZEGO::LIVEROOM::ZegoPlayQuality playQuality)
{

}

void ZEGO::LIVEROOM::LJLivePlayerCallback::OnJoinLiveResponse(int result, const char *pszFromUserId, const char *pszFromUserName, int seq)
{

}

void ZEGO::LIVEROOM::LJLivePlayerCallback::OnRecvEndJoinLiveCommand(const char *pszFromUserId, const char *pszFromUserName, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJLivePlayerCallback::OnInviteJoinLiveRequest(int seq, const char *pszFromUserId, const char *pszFromUserName, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJLivePlayerCallback::OnVideoSizeChanged(const char *pStreamID, int nWidth, int nHeight)
{

}

void ZEGO::LIVEROOM::LJLivePlayerCallback::OnSnapshot(void *pImage, const char *pszStreamID)
{

}

ZEGO::LIVEROOM::LJLivePlayerCallback::~LJLivePlayerCallback()
{

}

void ZEGO::LIVEROOM::LJZegoVideoRenderCallback::OnVideoDataCallback(const unsigned char *pData, int dataLen, const char *pszStreamID, int width, int height, int strides[])
{

}

void ZEGO::LIVEROOM::LJZegoVideoRenderCallback::OnVideoDataCallback2(const unsigned char **pData, int *dataLen, const char *pszStreamID, int width, int height, int strides[], AVE::VideoPixelFormat pixel_format) {}
