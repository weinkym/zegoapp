#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ZegoSDKSignal.h"

#include "VideoCapture/ZegoVideoCaptureDeviceDemo.h"
#include "VideoCapture/ZegoVideoCaptureFactoryDemo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool initSDK();

private slots:
    void on_pushButtonStart_clicked();

    void on_pushButtonStop_clicked();


    void onInitSDK(int nError);
    void onLoginRoom(int errorCode, const QString& roomId, QVector<StreamPtr> vStreamList);
//    void onLogoutRoom(int errorCode, const QString& roomId);
//    void onDisconnect(int errorCode, const QString& roomId);
//    void onKickOut(int reason, const QString& roomId);
//    void onSendRoomMessage(int errorCode, const QString& roomId, int sendSeq, unsigned long long messageId);
//    void onRecvRoomMessage(const QString& roomId, QVector<RoomMsgPtr> vRoomMsgList);
    void onStreamUpdated(const QString& roomId, QVector<StreamPtr> vStreamList, LIVEROOM::ZegoStreamUpdateType type);
    void onPublishStateUpdate(int stateCode, const QString& streamId, StreamPtr streamInfo);
//    void onPlayStateUpdate(int stateCode, const QString& streamId);
//    void onPublishQualityUpdate(const QString& streamId, int quality, double videoFPS, double videoKBS);
//    void onPlayQualityUpdate(const QString& streamId, int quality, double videoFPS, double videoKBS);
//    void onAuxInput(unsigned char* pData, int* pDataLen, int pDataLenValue, int* pSampleRate, int* pNumChannels);
    void onJoinLiveRequest(int seq, const QString& fromUserId, const QString& fromUserName, const QString& roomId);
//    void onJoinLiveResponse(int result, const QString& fromUserId, const QString& fromUserName, int seq);
//    void onAudioDeviceChanged(AV::AudioDeviceType deviceType, const QString& strDeviceId, const QString& strDeviceName, AV::DeviceState state);
//    void onVideoDeviceChanged(const QString& strDeviceId, const QString& strDeviceName, AV::DeviceState state);
//    void onUserUpdate(QVector<QString> userIDs, QVector<QString> userNames, QVector<int> userFlags, QVector<int> userRoles, unsigned int userCount, LIVEROOM::ZegoUserUpdateType type);
//    void onMixStream(unsigned int errorCode, const QString& hlsUrl, const QString& rtmpUrl, const QString& mixStreamID, int seq);
//    void onRecvEndJoinLiveCommand(const QString& userId, const QString& userName, const QString& roomId);
//#if (defined Q_OS_WIN32) && (defined USE_SURFACE_MERGE)
//    void onSurfaceMergeResult(unsigned char *surfaceMergeData, int datalength);
//#endif
//    void onPreviewSnapshot(void *pImage);
//    void onSnapshot(void *pImage, const QString &streamID);


protected:
    void closeEvent(QCloseEvent *event);


private:
    void doClose();

private:
    Ui::MainWindow *ui;
    QZegoAVSignal *m_pAVSignal;
    VideoCaptureFactoryGlue *m_videoCaptureFactoryGlue;
    bool m_init;
    bool m_login;
    bool m_push;
};

#endif // MAINWINDOW_H
