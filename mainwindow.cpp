#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clogsetting.h"


#include "LiveRoom.h"
#include "LiveRoom-Publisher.h"
#include "LiveRoom-Player.h"
#include "LiveRoom-IM.h"

#include <QMessageBox>

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
  ,gridLayout(NULL)
  ,m_init(false)
  ,m_login(false)
  ,m_push(false)
  ,m_streamStatus(STREAM_STATUS_NORMAL)
{
    ui->setupUi(this);
    m_timer.setInterval(100);
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(onSendTimeout()));
    connect(ui->checkBoxSound,SIGNAL(stateChanged(int)),this,SLOT(onCheckedSoundChanged(int)));
    connect(ui->checkBoxMicphone,SIGNAL(stateChanged(int)),this,SLOT(onCheckedSoundChanged(int)));
    onCheckedSoundChanged(0);
    ui->radioButtonHD->setChecked(true);
    updateStreamStatus(STREAM_STATUS_NORMAL);
    ui->pushButtonTimer->setText("send time");
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
    QString userId = ui->lineEditUserId->text();
    QString username = ui->lineEditUsername->text();
    QString roomId = ui->lineEditRoomId->text();
    QString roomName = ui->lineEditRoomName->text();
    if(userId.isEmpty() || username.isEmpty() || roomId.isEmpty() || roomName.isEmpty())
    {
        appendDebugInfo("userId username roomid and roomname can not empty");
        return false;
    }
    ui->lineEditUsername->setReadOnly(true);
    ui->lineEditUserId->setReadOnly(true);
    ui->lineEditRoomId->setReadOnly(true);
    ui->lineEditRoomName->setReadOnly(true);
    LJGolbalConfigManager::getInstance()->setFPS(ui->spinBoxFps->value());
    ui->spinBoxFps->setReadOnly(true);

    ui->rbImage->setEnabled(false);
    ui->rbVideo->setEnabled(false);
    ui->rbYUV->setEnabled(false);
    ui->radioButtonHD->setEnabled(false);
    ui->radioButtonSD->setEnabled(false);
    ui->radioButtonLD->setEnabled(false);

    //初始化SDK
    // 设置回调对象指针
    if(m_pAVSignal == NULL)
    {
        m_pAVSignal = new QZegoAVSignal;
        connect(m_pAVSignal,SIGNAL(sigInitSDK(int)),this,SLOT(onInitSDK(int)));
        connect(m_pAVSignal,SIGNAL(sigLoginRoom(int,QString,QVector<StreamPtr>)),this,SLOT(onLoginRoom(int,QString,QVector<StreamPtr>)));
        connect(m_pAVSignal,SIGNAL(sigPublishStateUpdate(int,QString,StreamPtr)),this,SLOT(onPublishStateUpdate(int,QString,StreamPtr)));
        connect(m_pAVSignal,SIGNAL(sigJoinLiveRequest(int,QString,QString,QString)),this,SLOT(onJoinLiveRequest(int,QString,QString,QString)));
        connect(m_pAVSignal,SIGNAL(sigStreamUpdated(QString,QVector<StreamPtr>,LIVEROOM::ZegoStreamUpdateType)),
                this,SLOT(onStreamUpdated(QString,QVector<StreamPtr>,LIVEROOM::ZegoStreamUpdateType)));
        connect(m_pAVSignal,SIGNAL(sigUserUpdate(QVector<QString>,QVector<QString>,QVector<int>,QVector<int>,uint,LIVEROOM::ZegoUserUpdateType)),
                this,SLOT(onUserUpdate(QVector<QString>,QVector<QString>,QVector<int>,QVector<int>,uint,LIVEROOM::ZegoUserUpdateType)));
        connect(m_pAVSignal,SIGNAL(sigRecvRoomMessage(QString,QVector<RoomMsgPtr>)),
                this,SLOT(onRecvRoomMessage(QString,QVector<RoomMsgPtr>)));
//        connect(m_pAVSignal,SIGNAL(sigPublishQualityUpdate(QString,int,double,double)),this,SLOT(onPublishQualityUpdate(QString,int,double,double)));
        connect(m_pAVSignal,SIGNAL(sigPublishQualityUpdate2(const char*,QVariant)),
                this,SLOT(onPublishQualityUpdate2(const char*,QVariant)));
    }


    // 测试环境开关
    LIVEROOM::SetUseTestEnv(true);

    if(m_videoCaptureFactoryGlue == NULL)
    {
        m_videoCaptureFactoryGlue = new VideoCaptureFactoryGlue;
    }

    LIVEROOM::SetVideoCaptureFactory(m_videoCaptureFactoryGlue);

    bool ret = false;

    LIVEROOM::SetNetType(2);
    // 设置 UserID 和 UserName。
    LIVEROOM::SetUser(userId.toLocal8Bit().data(), username.toLocal8Bit().data());
    LIVEROOM::SetLivePublisherCallback(m_pAVSignal);
    LIVEROOM::SetLivePlayerCallback(m_pAVSignal);
    LIVEROOM::SetRoomCallback(m_pAVSignal);
    ret = LIVEROOM::SetIMCallback(m_pAVSignal);
    C_VALUE_LOG_INFO("LIVEROOM::SetIMCallback");
    C_VALUE_LOG_INFO(ret);

    LIVEROOM::SetDeviceStateCallback(m_pAVSignal);
    ret = LIVEROOM::InitSDK(g_dwAppID2, g_bufSignKey_Udp, 32);
    C_VALUE_LOG_INFO(ret);
    if(!ret)
    {
        appendDebugInfo("init sdk has error");
        return false;
    }

    ret = ZEGO::LIVEROOM::LoginRoom(roomId.toLocal8Bit().data(), ZEGO::COMMON::ZegoRoomRole::Anchor, roomName.toLocal8Bit().data());
    C_VALUE_LOG_INFO(ret);

    //为了调用OnUserUpdate
    LIVEROOM::SetRoomConfig(true, true);
}

void MainWindow::on_pushButtonStart_clicked()
{
    initSDK();
    if(!m_login)
    {
        return;
    }

    if(m_streamStatus == STREAM_STATUS_STARTING)
    {
        return;
    }
    if(m_streamStatus == STREAM_STATUS_NORMAL)
    {
        doStart();
    }
    else if(m_streamStatus == STREAM_STATUS_STARTED)
    {
        doStop();
    }


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
    if(errorCode == 0 && !m_login)
    {
        m_login = true;
        on_pushButtonStart_clicked();
    }
}

void MainWindow::onRecvRoomMessage(const QString &roomId, QVector<RoomMsgPtr> vRoomMsgList)
{
    C_LOG_FUNCTION;
    for(auto obj:vRoomMsgList)
    {
        appendDebugInfo(QString("roomId=%1,name=%2,content=%3").arg(roomId).arg(obj.data()->getUserName()).arg(obj.data()->getContent()));
    }
}

void MainWindow::onStreamUpdated(const QString &roomId, QVector<StreamPtr> vStreamList, COMMON::ZegoStreamUpdateType type)
{
    C_LOG_FUNCTION;
    C_VALUE_LOG_INFO(roomId);
    C_VALUE_LOG_INFO(type);

    if(type == COMMON::StreamAdded)
    {
        for(auto obj : vStreamList)
        {
            QString id = obj.data()->getStreamId();
            LJUserViewWidget *view = new LJUserViewWidget;
            view->setStreamId(id);
            view->setContent(obj.data()->getUserName());
            m_viewList.append(view);
            //配置View
            LIVEROOM::SetViewMode(LIVEROOM::ZegoVideoViewModeScaleAspectFit, id.toStdString().c_str());
            LIVEROOM::StartPlayingStream(id.toStdString().c_str(), (void *)view->getViewId());
        }
    }
    else
    {
        for(auto obj : vStreamList)
        {
            QString id = obj.data()->getStreamId();
            int index = -1;
            for(int i = 0; i < m_viewList.count(); ++i)
            {
                if(m_viewList.at(i)->getStreamId() == id)
                {
                    index = i;
                    break;
                }
            }
            if(index >= 0)
            {
                LJUserViewWidget *view = m_viewList.takeAt(index);
                view->deleteLater();
            }
            //配置View
            LIVEROOM::StopPlayingStream(id.toStdString().c_str());
        }
    }
    updateViewLayout();

}

void MainWindow::onPublishStateUpdate(int stateCode, const QString &streamId, StreamPtr streamInfo)
{
    C_VALUE_LOG_INFO(stateCode);
    C_VALUE_LOG_INFO(streamId);
    C_VALUE_LOG_INFO(streamInfo.data()->getStreamId());
    C_VALUE_LOG_INFO(streamInfo.data()->getExtraInfo());
    C_VALUE_LOG_INFO(streamInfo.data()->getUserId());
    C_VALUE_LOG_INFO(streamInfo.data()->getUserName());

    if(stateCode == 0)
    {
        updateStreamStatus(STREAM_STATUS_STARTED);
    }
    else
    {
        appendDebugInfo("start has error");
        updateStreamStatus(STREAM_STATUS_NORMAL);
    }
}

void MainWindow::onPublishQualityUpdate(const QString &streamId, int quality, double videoFPS, double videoKBS)
{
    C_VALUE_LOG_INFO(streamId);
    C_VALUE_LOG_INFO(quality);
    C_VALUE_LOG_INFO(videoFPS);
    C_VALUE_LOG_INFO(videoKBS);
}

void MainWindow::onJoinLiveRequest(int seq, const QString &fromUserId, const QString &fromUserName, const QString &roomId)
{
    QMessageBox box(QMessageBox::Warning, tr("提示"), QString(tr("%1正在请求连麦")).arg(fromUserId));
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    if (box.exec() == QMessageBox::Ok)
    {
        LIVEROOM::RespondJoinLiveReq(seq, 0);
    }
    else
    {
        LIVEROOM::RespondJoinLiveReq(seq, -1);
    }
}

void MainWindow::onUserUpdate(QVector<QString> userIDs, QVector<QString> userNames, QVector<int> userFlags, QVector<int> userRoles, unsigned int userCount, COMMON::ZegoUserUpdateType type)
{
    C_LOG_FUNCTION;
    C_LOG_INFO(QString("userCount=%1").arg(userCount));
    C_LOG_INFO(QString("type=%1").arg(type));
   if(type == COMMON::UPDATE_TOTAL)
   {
       ui->listWidget->clear();
       for(int i = 0; i < userCount;++i)
       {
           int role = userRoles.at(i);
           int flag = userFlags.at(i);
           QString id = userIDs.at(i);
           QString name = userNames.at(i);
           C_LOG_INFO(QString("name=%1").arg(name));
           C_LOG_INFO(QString("flag=%1").arg(flag));
           QListWidgetItem *item = new QListWidgetItem();
           item->setData(Qt::UserRole,id);
           item->setText(QString("%1:%2").arg(role == 1 ? "Anchor":"Audience").arg(name));
           ui->listWidget->addItem(item);
       }
   }
   else
   {
       for(int i = 0; i < userCount;++i)
       {
           int role = userRoles.at(i);
           int flag = userFlags.at(i);
           QString id = userIDs.at(i);
           QString name = userNames.at(i);
           C_LOG_INFO(QString("name=%1").arg(name));
           C_LOG_INFO(QString("flag=%1").arg(flag));
           if(flag == COMMON::USER_ADDED)
           {
               QListWidgetItem *item = new QListWidgetItem();
               item->setData(Qt::UserRole,id);
               item->setText(QString("%1:%2").arg(role == 1 ? "Anchor":"Audience").arg(name));
               ui->listWidget->addItem(item);
           }
           else
           {
               int index = -1;
               for(int i = 0; i < ui->listWidget->count(); ++i)
               {
                   if(ui->listWidget->item(i)->data(Qt::UserRole).toString() == id)
                   {
                       index = i;
                       break;
                   }
               }
               if(index >= 0)
               {
                   QListWidgetItem *item = ui->listWidget->takeItem(index);
                   delete item;
               }
           }
       }
   }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    doClose();
    return QMainWindow::closeEvent(event);
}

void MainWindow::doClose()
{
    if(m_streamStatus == STREAM_STATUS_STARTED)
    {
        doStop();
    }
    LIVEROOM::LogoutRoom();
}

void MainWindow::doStart()
{
    initSDK();
    updateStreamStatus(STREAM_STATUS_STARTING);
    int fps = LJGolbalConfigManager::getInstance()->getFPS();
//    QSize c_size = LJGolbalConfigManager::getInstance()->getVideoCaptureResolution();
//    QSize e_size = LJGolbalConfigManager::getInstance()->getVideoEncodeResolution();
    QSize c_size = LJGolbalConfigManager::getInstance()->getVideoCaptureResolution();
    if(ui->radioButtonHD->isChecked())
    {
        c_size = QSize(1280,720);
    }
    else if(ui->radioButtonLD->isChecked())
    {
        c_size = QSize(720,480);
    }
    else
    {
        c_size = QSize(480,360);
    }
    QString sizeString = QString("push size %1,%2").arg(c_size.width()).arg(c_size.height());
    appendDebugInfo(sizeString);
    this->setStatusTip(sizeString);
    QString videoPath = (QApplication::applicationDirPath()+"/720P.mp4");

    LJGolbalConfigManager::getInstance()->setVideoPath(QDir::toNativeSeparators(videoPath));
    LJGolbalConfigManager::getInstance()->setSizeType(c_size.height());
    LJGolbalConfigManager::getInstance()->setDataType(ui->rbImage->isChecked() ? LJGolbalConfigManager::DATA_TYPE_IMAGE : (ui->rbVideo->isChecked() ? LJGolbalConfigManager::DATA_TYPE_VIDEO : LJGolbalConfigManager::DATA_TYPE_YUV));
    QSize e_size = c_size;

    LIVEROOM::SetVideoFPS(fps);
//    LIVEROOM::SetVideoBitrate(1024);
    LIVEROOM::SetVideoCaptureResolution(c_size.width(), c_size.height());
    LIVEROOM::SetVideoEncodeResolution(e_size.width(), e_size.height());
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
    bool ret = LIVEROOM::StartPublishing(roomName.toLocal8Bit().data(),roomId.toLocal8Bit().data(), LIVEROOM::ZEGO_JOIN_PUBLISH, "");

    updateStreamStatus(ret ? STREAM_STATUS_STARTING : STREAM_STATUS_NORMAL);
}

void MainWindow::doStop()
{
    on_pushButtonTimer_clicked();
    bool ret = LIVEROOM::StopPublishing();
    updateStreamStatus(STREAM_STATUS_NORMAL);
    if(!ret)
    {
        appendDebugInfo("stop puslishing has error");
    }
}

void MainWindow::updateStreamStatus(MainWindow::StreamStatus status)
{
    m_streamStatus = status;

    switch (status)
    {
    case STREAM_STATUS_NORMAL:
        ui->pushButtonStart->setText("Start");
        break;
    case STREAM_STATUS_STARTING:
        ui->pushButtonStart->setText("Starting...");
        break;
    case STREAM_STATUS_STARTED:
        ui->pushButtonStart->setText("Stop");
        break;
    default:
        break;
    }
}

void MainWindow::updateViewLayout()
{
    if(gridLayout)
    {
        gridLayout->deleteLater();
        gridLayout = NULL;
    }
    gridLayout = new QGridLayout;

    gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    ui->hLayout->addLayout(gridLayout);

    int viewCount = m_viewList.count();
    for (int i = 0; i < viewCount; i++)
    {
        int row, col;
        if (viewCount >= 1 && viewCount <= 4)
        {
            row = i / 2;
            col = i % 2;
        }
        else if (viewCount >= 5 && viewCount <= 9)
        {
            row = i / 3;
            col = i % 3;
        }
        else if (viewCount >= 10 && viewCount <= 12)
        {
            row = i / 4;
            col = i % 4;
        }
        qDebug() << "current row = " << row << " col = " << col;
        gridLayout->addWidget(m_viewList[i], row, col, 1, 1);
        gridLayout->setRowStretch(row, 1);
        gridLayout->setColumnStretch(col, 1);
    }
}

void MainWindow::appendDebugInfo(const QString &info)
{
    static int s_count = 0;
    s_count++;
    if(s_count > 1000)
    {
        s_count = 0;
        ui->textEdit->clear();
    }
    ui->textEdit->append(info);
}

void MainWindow::on_pushButtonSend_clicked()
{
    QString text = ui->lineEditInput->text().simplified();
    ui->lineEditInput->clear();
    if(text.isEmpty())
    {
        return;
    }
    if(text == "time")
    {
        text = QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss:zzz");
    }
    LIVEROOM::SendRoomMessage(ROOM::ZegoMessageType::Text, ROOM::ZegoMessageCategory::Chat, ROOM::ZegoMessagePriority::Default, text.toStdString().c_str());
}

void MainWindow::onSendTimeout()
{
    QString text = QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss:zzz");
    LIVEROOM::SendRoomMessage(ROOM::ZegoMessageType::Text, ROOM::ZegoMessageCategory::Chat, ROOM::ZegoMessagePriority::Default, text.toStdString().c_str());
}

void MainWindow::on_pushButtonTimer_clicked()
{
    if(m_timer.isActive())
    {
        m_timer.stop();
        ui->pushButtonTimer->setText("send time");
    }
    else
    {
        m_timer.start();
        ui->pushButtonTimer->setText("sending time");
    }
}

void MainWindow::onPublishQualityUpdate2(const char *pszStreamID, const QVariant &value)
{
    ZEGO::LIVEROOM::ZegoPublishQuality obj = value.value<ZEGO::LIVEROOM::ZegoPublishQuality>();
    QStringList infos;
    infos.append(QString("pszStreamID=%1").arg(pszStreamID));
    infos.append(QString("akbps=%1").arg(obj.akbps));
    infos.append(QString("fps=%1").arg(obj.fps));
    infos.append(QString("kbps=%1").arg(obj.kbps));
    infos.append(QString("pktLostRate=%1").arg(obj.pktLostRate));
    infos.append(QString("quality=%1").arg(obj.quality));
    infos.append(QString("rtt=%1").arg(obj.rtt));
    QString paramString = infos.join("   ");
    C_LOG_INFO(paramString);
    appendDebugInfo(paramString);
}

void MainWindow::onCheckedSoundChanged(int state)
{
    C_LOG_FUNCTION;
    LIVEROOM::EnableSpeaker(ui->checkBoxSound->isChecked());
    LIVEROOM::EnableMic(ui->checkBoxMicphone->isChecked());

}
