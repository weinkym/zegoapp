#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "LiveRoom.h"
#include "LiveRoom-Publisher.h"
#include "LiveRoom-Player.h"
#include "LiveRoom-IM.h"


#include "ljlivepublishercallback.h"
#include "ljroomcallback.h"
#include "clogsetting.h"

#include "ZegoSDKSignal.h"

using namespace ZEGO;

bool g_useTestEnv = true;

//const unsigned int g_dwAppID2 = 4133572644;
//const unsigned char *g_bufSignKey2 = "0xc3,0x33,0x8e,0xeb,0x85,0x39,0xcb,0xbe,0x65,0x79,0x02,0xf5,0x07,0x3b,0xe4,0x9b,0x76,0x2c,0xd6,0xe0,0x5f,0xfa,0xcd,0x54,0x1c,0x31,0x56,0x4d,0x10,0xa7,0x2f,0x0c";


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.initSDK();

//    QString userId = "yxtuserid";
//    QString username = "mzw";
//    QString appId = "4133572644";
//    QString appSign = "0xc3,0x33,0x8e,0xeb,0x85,0x39,0xcb,0xbe,0x65,0x79,0x02,0xf5,0x07,0x3b,0xe4,0x9b,0x76,0x2c,0xd6,0xe0,0x5f,0xfa,0xcd,0x54,0x1c,0x31,0x56,0x4d,0x10,0xa7,0x2f,0x0c";
//    ZEGO::LIVEROOM::LJLivePublisherCallback *m_pPublisherSignal = new ZEGO::LIVEROOM::LJLivePublisherCallback;
//    ZEGO::LIVEROOM::LJRoomCallback *m_pRoomSignal = new ZEGO::LIVEROOM::LJRoomCallback;


//    QZegoAVSignal *m_pAVSignal = new QZegoAVSignal;

//    QString roomId = "m123456";
//    QString roomName = "mzw";

    // 测试环境开关
//    LIVEROOM::SetUseTestEnv(g_useTestEnv);
//    LIVEROOM::SetUseTestEnv(true);

    // 通过宏定义选择是否打开调试信息，仅供参考
    #ifdef DEBUG

    // 调试信息开关
    LIVEROOM::SetVerbose(true);

    #endif

//    // 设置 UserID 和 UserName。
//    LIVEROOM::SetUser(userId.toLocal8Bit().data(), username.toLocal8Bit().data());

//    // 设置回调对象指针
//    LIVEROOM::SetLivePublisherCallback(m_pAVSignal);
//    LIVEROOM::SetLivePlayerCallback(m_pAVSignal);
//    LIVEROOM::SetRoomCallback(m_pAVSignal);
//    LIVEROOM::SetIMCallback(m_pAVSignal);
//    LIVEROOM::SetDeviceStateCallback(m_pAVSignal);

    //初始化SDK
//    bool ret = LIVEROOM::InitSDK(g_dwAppID2, g_bufSignKey_Udp, 32);
//    C_VALUE_LOG_INFO(ret);
//    C_VALUE_LOG_INFO(ZEGO::LIVEROOM::GetSDKVersion());

//    ret = ZEGO::LIVEROOM::LoginRoom(roomId.toLocal8Bit().data(), ZEGO::COMMON::ZegoRoomRole::Anchor, roomName.toLocal8Bit().data());
//    C_VALUE_LOG_INFO(ret);
    return a.exec();
}
