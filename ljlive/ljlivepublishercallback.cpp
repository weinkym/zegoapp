#include "ljlivepublishercallback.h"


void ZEGO::LIVEROOM::LJLivePublisherCallback::OnPublishStateUpdate(int stateCode, const char *pszStreamID, const ZegoPublishingStreamInfo &oStreamInfo)
{

}

void ZEGO::LIVEROOM::LJLivePublisherCallback::OnJoinLiveRequest(int seq, const char *pszFromUserId, const char *pszFromUserName, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJLivePublisherCallback::OnInviteJoinLiveResponse(int result, const char *pszFromUserId, const char *pszFromUserName, int seq)
{

}

void ZEGO::LIVEROOM::LJLivePublisherCallback::OnEndJoinLive(int result, int seq, const char *pszRoomID)
{

}

void ZEGO::LIVEROOM::LJLivePublisherCallback::OnPublishQulityUpdate(const char *pszStreamID, int quality, double videoFPS, double videoKBS)
{

}

void ZEGO::LIVEROOM::LJLivePublisherCallback::OnPublishQualityUpdate(const char *pszStreamID, ZegoPublishQuality publishQuality)
{

}

void ZEGO::LIVEROOM::LJLivePublisherCallback::OnCaptureVideoSizeChanged(int nWidth, int nHeight) {}

void ZEGO::LIVEROOM::LJLivePublisherCallback::OnCaptureVideoSizeChanged(AV::PublishChannelIndex index, int nWidth, int nHegith)
{

}

void ZEGO::LIVEROOM::LJLivePublisherCallback::OnPreviewSnapshot(void *pImage)
{

}

void ZEGO::LIVEROOM::LJLivePublisherCallback::OnPreviewSnapshot(AV::PublishChannelIndex index, void *pImage)
{

}

void ZEGO::LIVEROOM::LJLivePublisherCallback::OnAuxCallback(unsigned char *pData, int *pDataLen, int *pSampleRate, int *pNumChannels)
{

}

void ZEGO::LIVEROOM::LJLivePublisherCallback::OnMixStream(const AV::ZegoMixStreamResult &result, const char *pszMixStreamID, int seq)
{

}

ZEGO::LIVEROOM::LJLivePublisherCallback::~LJLivePublisherCallback()
{

}

void ZEGO::LIVEROOM::LJLivePublisherCallback::OnUpdateMixStreamConfig(unsigned int uiErrorCode, const char *pszMixStreamID, const ZegoPublishingStreamInfo &oStreamInfo)
{

}
