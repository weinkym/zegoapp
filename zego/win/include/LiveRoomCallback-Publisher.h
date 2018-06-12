//
//  LiveRoomCallback-Publisher.h
//  zegoliveroom
//
//  Created by Randy Qiu on 2017/1/23.
//
//

#ifndef LiveRoomCallback_Publisher_h
#define LiveRoomCallback_Publisher_h

#include "./LiveRoomDefines-Publisher.h"
#include "./LiveRoomDefines.h"

namespace ZEGO
{
    namespace LIVEROOM
    {
        class ILivePublisherCallback
        {
        public:
            /**
             推流状态更新

             @param stateCode 状态码
             @param pszStreamID 流 ID
             @param oStreamInfo 推流信息
             */
            virtual void OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo) = 0;
            
            /**
             收到连麦请求

             @param seq 请求 seq
             @param pszFromUserId 来源用户 Id
             @param pszFromUserName 来源用户名
             @param pszRoomID 房间 ID
             */
            virtual void OnJoinLiveRequest(int seq, const char *pszFromUserId, const char *pszFromUserName, const char *pszRoomID) {}
            
            /**
             收到邀请连麦响应结果

             @param result 结果
             @param pszFromUserId 来源用户 Id
             @param pszFromUserName 来源用户名
             @param seq 请求 seq
             */
            virtual void OnInviteJoinLiveResponse(int result, const char *pszFromUserId, const char *pszFromUserName, int seq) {}
            
            /**
             结束连麦结果

             @param result 结果
             @param seq 请求 seq
             @param pszRoomID 房间 ID
             */
            virtual void OnEndJoinLive(int result, int seq, const char *pszRoomID) {}
            
            /**
             推流质量更新

             @param pszStreamID 流 ID
             @param quality 发布质量，0~3 分别对应优、良、中、差
             @param videoFPS 帧率
             @param videoKBS 码率
             */
            virtual void OnPublishQulityUpdate(const char* pszStreamID, int quality, double videoFPS, double videoKBS) {}
            
            /**
             推流质量更新

             @param pszStreamID 流 ID
             @param publishQuality ZegoPublishQuality 对象，内含各项推流质量数据
             */
            virtual void OnPublishQualityUpdate(const char* pszStreamID, ZegoPublishQuality publishQuality) {}
            
            /**
             采集视频的宽度和高度变化通知

             @param nWidth 视频宽度
             @param nHeight 视频高度
             @attention 发布直播成功后，当视频尺寸变化时，发布者会收到此通知
             */
            virtual void OnCaptureVideoSizeChanged(int nWidth, int nHeight) {}
            
            virtual void OnCaptureVideoSizeChanged(AV::PublishChannelIndex index, int nWidth, int nHegith) {}
            
            /**
             预览截图

             @param pImage 截图结果
             */
            virtual void OnPreviewSnapshot(void *pImage) {}
            
            virtual void OnPreviewSnapshot(AV::PublishChannelIndex index, void *pImage) {}
            
            /**
             混音数据输入回调

             @param pData 混音数据
             @param pDataLen 缓冲区长度。实际填充长度必须为 0 或是缓冲区长度。0 代表无混音数据
             @param pSampleRate 混音数据采样率，支持16k、32k、44.1k、48k
             @param pNumChannels 混音数据声道数，支持1、2
             @attention 用户调用该 API 将混音数据传递给 SDK
             @note 混音数据 bit depth 必须为 16
             */
            virtual void OnAuxCallback(unsigned char *pData, int *pDataLen, int *pSampleRate, int *pNumChannels) {}
            
            /**
             混流请求结果回调

             @param result 结果
             @param pszMixStreamID 混流 ID
             @param seq 请求 seq
             */
            virtual void OnMixStream(const AV::ZegoMixStreamResult& result, const char* pszMixStreamID, int seq) {}
            
            
            
            virtual ~ILivePublisherCallback() {}
            
            /**
             更新混流配置回调
 
             @note 不建议使用，请用 OnMixStream
             */
            virtual void OnUpdateMixStreamConfig(unsigned int uiErrorCode, const char* pszMixStreamID, const ZegoPublishingStreamInfo& oStreamInfo) {}
        };
    }
}
#endif /* LiveRoomCallback_Publisher_h */
