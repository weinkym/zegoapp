#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clogsetting.h"


#include "LiveRoom.h"
#include "LiveRoom-Publisher.h"
#include "LiveRoom-Player.h"
#include "LiveRoom-IM.h"



static unsigned long g_dwAppID2 = 4133572644;
static unsigned char g_bufSignKey_Udp[] =
{
    0xc3,0x33,0x8e,0xeb,0x85,0x39,0xcb,0xbe,0x65,0x79,0x02,0xf5,0x07,0x3b,
    0xe4,0x9b,0x76,0x2c,0xd6,0xe0,0x5f,0xfa,0xcd,0x54,0x1c,0x31,0x56,0x4d,
    0x10,0xa7,0x2f,0x0c
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  ,m_pAVSignal(NULL)
  ,m_videoCaptureFactoryGlue(NULL)
  ,m_init(false)
  ,m_login(false)
  ,m_push(false)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    doClose();
    delete ui;
}

bool MainWindow::initSDK()
{
    if(m_init)
    {
        return true;
    }
    //初始化SDK
    // 设置回调对象指针
    if(m_pAVSignal == NULL)
    {
        m_pAVSignal = new QZegoAVSignal;
        connect(m_pAVSignal,SIGNAL(sigInitSDK(int)),this,SLOT(onInitSDK(int)));
        connect(m_pAVSignal,SIGNAL(sigLoginRoom(int,QString,QVector<StreamPtr>)),this,SLOT(onLoginRoom(int,QString,QVector<StreamPtr>)));
        connect(m_pAVSignal,SIGNAL(sigPublishStateUpdate(int,QString,StreamPtr)),this,SLOT(onPublishStateUpdate(int,QString,StreamPtr)));
    }
    QString userId = "yxtuserid";
    QString username = "mzw";

    // 测试环境开关
    LIVEROOM::SetUseTestEnv(true);

    if(m_videoCaptureFactoryGlue == NULL)
    {
        m_videoCaptureFactoryGlue = new VideoCaptureFactoryGlue;
    }

    LIVEROOM::SetVideoCaptureFactory(m_videoCaptureFactoryGlue);


    // 设置 UserID 和 UserName。
    LIVEROOM::SetUser(userId.toLocal8Bit().data(), username.toLocal8Bit().data());
    LIVEROOM::SetLivePublisherCallback(m_pAVSignal);
    LIVEROOM::SetLivePlayerCallback(m_pAVSignal);
    LIVEROOM::SetRoomCallback(m_pAVSignal);
    LIVEROOM::SetIMCallback(m_pAVSignal);
    LIVEROOM::SetDeviceStateCallback(m_pAVSignal);
    bool ret = LIVEROOM::InitSDK(g_dwAppID2, g_bufSignKey_Udp, 32);
    C_VALUE_LOG_INFO(ret);
    if(!ret)
    {
        return false;
    }
    QString roomId = "m123456";
    QString roomName = "mzw";
    ret = ZEGO::LIVEROOM::LoginRoom(roomId.toLocal8Bit().data(), ZEGO::COMMON::ZegoRoomRole::Anchor, roomName.toLocal8Bit().data());
    C_VALUE_LOG_INFO(ret);
}

void MainWindow::on_pushButtonStart_clicked()
{
    if(!m_init)
    {
        return;
    }
    if(!m_login)
    {
        return;
    }
//    LIVEROOM::SetVideoFPS(m_pAVSettings->GetFps());
//    LIVEROOM::SetVideoBitrate(m_pAVSettings->GetBitrate());
//    LIVEROOM::SetVideoCaptureResolution(m_pAVSettings->GetResolution().cx, m_pAVSettings->GetResolution().cy);
//    LIVEROOM::SetVideoEncodeResolution(m_pAVSettings->GetResolution().cx, m_pAVSettings->GetResolution().cy);

    //配置View
//    LIVEROOM::SetPreviewView((void *)AVViews.last()->winId());
//    LIVEROOM::SetPreviewView((void *)ui->widget);
//    LIVEROOM::SetPreviewViewMode(LIVEROOM::ZegoVideoViewModeScaleAspectFit);
//    LIVEROOM::StartPreview();


    QString roomId = "m123456";
    QString roomName = "mzw";
//    QString streamID = m_strPublishStreamID;
    qDebug() << "start publishing!";
    //    setWaterPrint();
    //    LIVEROOM::StartPublishing(m_pChatRoom->getRoomName().toStdString().c_str(), streamID.toStdString().c_str(), LIVEROOM::ZEGO_MIX_STREAM, "");
    LIVEROOM::StartPublishing(roomName.toLocal8Bit().data(),roomId.toLocal8Bit().data(), LIVEROOM::ZEGO_JOIN_PUBLISH, "");
}

void MainWindow::on_pushButtonStop_clicked()
{
    C_VALUE_LOG_INFO(m_push);
    if(!m_push)
    {
//        return;
    }
    bool ret = LIVEROOM::StopPublishing();
    C_VALUE_LOG_INFO(ret);
}

void MainWindow::onInitSDK(int nError)
{
    m_init = (nError == 0);
    C_VALUE_LOG_INFO(nError);
}

void MainWindow::onLoginRoom(int errorCode, const QString &roomId, QVector<StreamPtr> vStreamList)
{
    C_VALUE_LOG_INFO(errorCode);
    C_VALUE_LOG_INFO(roomId);
    C_VALUE_LOG_INFO(vStreamList.count());
    if(errorCode == 0)
    {
        m_login = true;
    }
}

void MainWindow::onPublishStateUpdate(int stateCode, const QString &streamId, StreamPtr streamInfo)
{
    C_VALUE_LOG_INFO(stateCode);
    C_VALUE_LOG_INFO(streamId);
//    C_VALUE_LOG_INFO(streamInfo);

    m_push = (stateCode == 0);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    doClose();
    return QMainWindow::closeEvent(event);
}

void MainWindow::doClose()
{
//    if(m_push)
    {
        on_pushButtonStop_clicked();
    }
//    if(m_login)
    {
        LIVEROOM::LogoutRoom();
    }
}
