#include "ZegoVideoCaptureDeviceDemo.h"
#include <QDebug>

VideoCaptureDeviceGlue::VideoCaptureDeviceGlue()
{
    m_thread = new LJIncomingCapturedDataThread(this);
}

VideoCaptureDeviceGlue::~VideoCaptureDeviceGlue()
{

}

void VideoCaptureDeviceGlue::AllocateAndStart(Client* client) {
	client_ = client;
	callback_ = (VideoCaptureCallback*)client->GetInterface();

	m_bExit = false;
    m_thread->start();
	qDebug() << "vc alloced!";
//    LPDWORD tid = 0;
//    m_hVideoTimer = CreateThread(0, 0, &VideoCaptureDeviceGlue::thread_proc, this, 0, tid);
}

void VideoCaptureDeviceGlue::StopAndDeAllocate() {
	m_bExit = true;
	/*if (m_hVideoTimer) {
		WaitForSingleObject(m_hVideoTimer, INFINITE);
		CloseHandle(m_hVideoTimer);
		m_hVideoTimer = NULL;
	}
	*/
    C_LOG_TEST_LINE;
   if(m_thread->isRunning())
    {
        C_LOG_TEST_LINE;
        m_thread->wait();
        C_LOG_TEST_LINE;
    }
	qDebug() << "vc dealloced!";
	client_->Destroy();
	client_ = NULL;
}

int VideoCaptureDeviceGlue::StartCapture() {
	m_bCapture = true;
	qDebug() << "Start Capture!";
	return 0;
}

int VideoCaptureDeviceGlue::StopCapture() {
	m_bCapture = false;
	return 0;
}

void VideoCaptureDeviceGlue::OnVideoTimer() {
	while (!m_bExit) {
		if (m_bCapture) {
			/*int width, height;
			EnterCriticalSection(&cs_);
			width = new_width_;
			height = new_height_;
			LeaveCriticalSection(&cs_);

			if (width != width_ || height != height_) {
				if (data_) {
					free(data_);
					data_ = NULL;
				}
#if TEST_BGRA
				data_size_ = width * height * 4;
				data_ = (char*)malloc(data_size_);
				memset(data_, 0xFF, data_size_);
#else
				data_size_ = width * height * 3 / 2;
				data_ = (char*)malloc(data_size_);
				memset(data_, 0x46, width * height);
				memset(data_ + (width * height), 0xA2, width * height / 4);
				memset(data_ + (width * height * 5 / 4), 0xB1, width * height / 4);
#endif
				width_ = width;
				height_ = height;
			}

			AVE::VideoCaptureFormat format;
			format.width = width;
			format.height = height;
#if TEST_BGRA
			format.strides[0] = width * 4;
			format.pixel_format = AVE::PIXEL_FORMAT_BGRA32;
#else
			format.strides[0] = width;
			format.strides[1] = width / 2;
			format.strides[2] = width / 2;
			format.strides[3] = 0;
			format.pixel_format = AVE::PIXEL_FORMAT_I420;
#endif

			callback_->OnIncomingCapturedData(data_, data_size_, format, GetTickCount(), 1000);
			*/
		}

		//Sleep(m_nVideoTickPeriod);
	}

	return;
}


//====================================
LJIncomingCapturedDataThread::LJIncomingCapturedDataThread(VideoCaptureDeviceGlue *device, QObject *parent)
    :QThread(parent)
    ,m_device(device)
{
//    m_image = QImage(400,400,QImage::Format_ARGB32);
//    m_image.fill(QColor(0,123,12));
}

LJIncomingCapturedDataThread::~LJIncomingCapturedDataThread()
{

}

void LJIncomingCapturedDataThread::run()
{

    if(!m_device)
    {
        return;
    }

    LJGolbalConfigManager::DataType dataType = LJGolbalConfigManager::getInstance()->getDataType();
    if(dataType == LJGolbalConfigManager::DATA_TYPE_IMAGE)
    {
        sendImage();
    }
    else
    {
        C_LOG_INFO(QString("initVideo=%1").arg("befor"));
        initVideo();
        C_LOG_INFO(QString("initVideo=%1").arg("after"));
        C_LOG_INFO(QString("m_videoData.videoStream=%1").arg("m_videoData.videoStream"));
        if(m_videoData.videoStream < 0)
        {
            sendImage();
        }
        else
        {
            sendFrame(dataType == LJGolbalConfigManager::DATA_TYPE_YUV);
        }
    }

//    sendImage();
}

void LJIncomingCapturedDataThread::initVideo()
{
    unsigned version = avcodec_version();
    qDebug() << "version is:" << version;
    AVFormatContext *pFormatCtx = avformat_alloc_context();
//    m_filePath = "C:\\Users\\miao\\Videos\\test2.mp4";
//    char file_path[] = "C:\\Users\\miao\\Videos\\720.mp4";
    QString path = LJGolbalConfigManager::getInstance()->getVideoPath();
    C_LOG_INFO(QString("path=%1").arg(path.toLocal8Bit().data()));

    int ret = avformat_open_input(&pFormatCtx, path.toLocal8Bit().data(), NULL, NULL);
    C_LOG_INFO(QString("result=%1").arg(ret));
    if(ret != 0)
    {
        C_LOG_INFO(QString("avformat_open_input is error"));
        return;
    }
    C_LOG_INFO("TTTTTTTTTT");
//    AVFormatContext *context = avformat_alloc_context();
    C_LOG_INFO("TTTTTTTTTT");
    ret = avformat_find_stream_info(pFormatCtx,NULL);
    C_LOG_INFO("TTTTTTTTTT");
    if(ret != 0)
    {
        C_LOG_INFO(QString("avformat_find_stream_info is error"));
        return;
    }
    C_LOG_INFO("TTTTTTTTTT");

    int videoStream = -1;
    //循环查找视频中包含的流信息，直到找到视频类型的流
    //便将其记录下来 保存到videoStream变量中
    //这里我们现在只处理视频流  音频流先不管他
    for (int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
        }
    }
    if (videoStream == -1)
    {
        C_LOG_INFO(QString("videoStream is error"));
        return;
    }
    C_LOG_INFO(QString("TTTTTTTTTTvideoStreamIndex=%1").arg(videoStream));
    //查找解码器
    AVCodecContext *pCodecCtx = pFormatCtx->streams[videoStream]->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL)
    {
        C_LOG_INFO(QString("pCodec is null"));
        return;
    }
    //打开解码器
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        C_LOG_INFO(QString("avcodec_open2"));
        return;
    }
    this->m_videoData.pCodec = pCodec;
    this->m_videoData.pCodecCtx = pCodecCtx;
    this->m_videoData.pFormatCtx = pFormatCtx;
    this->m_videoData.videoStream = videoStream;
    C_LOG_INFO(QString("init is ok"));
}

void LJIncomingCapturedDataThread::sendImage()
{
    int fps = LJGolbalConfigManager::getInstance()->getFPS();
    int intervalMSes = 1000 / fps;
    QString imagePath = LJGolbalConfigManager::getInstance()->getImagePath();
    C_LOG_INFO(QString("imagePath=%1").arg(imagePath));
//    imagePath = ":/images/res/720P.jpg";

    m_image = QImage(imagePath).convertToFormat(QImage::Format_ARGB32);
    if(m_image.isNull())
    {
        C_LOG_INFO(QString("m_image is NULL"));
        return;
    }

    QFont font;
    font.setPixelSize(m_image.height() / 5);
    QFontMetrics fontMetrics(font);
    QDateTime preDateTime = QDateTime::currentDateTime();
    QTime preTime = preDateTime.time();
    QString text = preTime.toString("hh:mm:ss:zzz");
    QRect boundingRect = fontMetrics.boundingRect(text);
    QSize imageSize = m_image.size();
    QSize drawSize(boundingRect.width() + 10,boundingRect.height() + 10);

    QRect bgRect((imageSize.width() - drawSize.width()) / 2,(imageSize.height() - drawSize.height())/2,
                   drawSize.width(),drawSize.height());

    QImage bgImage = m_image.copy(bgRect);

    QImage drawImage = bgImage;

    QPainter painter(&drawImage);
    painter.setPen(QPen(Qt::red));
    painter.setFont(font);

    painter.setBrush(QBrush(bgImage));

    QRect rect(0,0,m_image.width(),m_image.height());


    while (!m_device->m_bExit)
    {
        int msec = 10;
        if (m_device->m_bCapture)
        {
            this->msleep(10);
            QDateTime dateTime = QDateTime::currentDateTime();
            QTime time = dateTime.time();

            painter.drawRect(QRect(0,0,drawSize.width(),drawSize.height()));

            QString text = time.toString("hh:mm:ss:zzz");
            QRect boundingRect = fontMetrics.boundingRect(text);
            QRect textRect((drawSize.width() - boundingRect.width()) / 2 ,(drawSize.height() - boundingRect.height())/2,
                           boundingRect.width(),boundingRect.height());
            painter.drawText(textRect,text);
            //            painter.drawEllipse(QRect(0,0,200,200));
            //            painter.drawLine(rect.topLeft(),rect.bottomRight());

            for(int i = 0;i < bgRect.width();i++)
            {
                for(int j = 0;j < bgRect.height();j++)
                {
                    m_image.setPixelColor(i + bgRect.x(),j + bgRect.y(),drawImage.pixelColor(i,j));
                }
            }
            AVE::VideoCaptureFormat format;
            format.width = m_image.width();
            format.height = m_image.height();
            format.strides[0] = m_image.width() * 4;
            format.pixel_format = AVE::PIXEL_FORMAT_BGRA32;
            m_device->callback_->OnIncomingCapturedData((char*)m_image.constBits(), m_image.width() * m_image.height() * 4,
                                                        format, dateTime.toMSecsSinceEpoch(), 1000);

            {
                QTime time = QTime::currentTime();
                int ms = preTime.msecsTo(QTime::currentTime());
                preTime = time;
                if(ms < intervalMSes)
                {
                    msec = intervalMSes - ms;
                }
            }
        }
        this->msleep(msec);
    }
}

void SaveFrame(QImage &image,AVFrame *pFrame, int width, int height,int index)
{

  int  y;
//  C_LOG_INFO(QString("width=%1,height=%2").arg(width).arg(height));
  for(y=0; y<height; y++)
  {
//    fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
      for(int x = 0; x < width;++x)
      {
//        printf("%d ",(pFrame->data[0]+y*pFrame->linesize[0])[x]);
          int r = (pFrame->data[0]+y*pFrame->linesize[0])[3*x];
          int g = (pFrame->data[0]+y*pFrame->linesize[0])[3*x + 1];
          int b = (pFrame->data[0]+y*pFrame->linesize[0])[3*x + 2];
          image.setPixelColor(x,y,QColor(r,g,b));
      }
  }
}

void LJIncomingCapturedDataThread::sendFrame(bool userYUV)
{
    AVFrame *pFrame = av_frame_alloc();
    AVFrame *pFrameRGB = av_frame_alloc();
    AVFormatContext *pFormatCtx = m_videoData.pFormatCtx;
    AVCodec *pCodec = m_videoData.pCodec;
    AVCodecContext *pCodecCtx = m_videoData.pCodecCtx;
    int videoStream = m_videoData.videoStream;
    C_LOG_INFO(QString("TTTTTTTTTT,%1,%2,%3,%4,%5,%6")
                .arg(pCodecCtx->width)
                .arg(pCodecCtx->height)
                .arg(pCodecCtx->pix_fmt)
                .arg(pCodecCtx->width)
                .arg(pCodecCtx->width)
                .arg(pCodecCtx->height));




    static struct SwsContext *img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                                     pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
                                     AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);


    int numBytes = avpicture_get_size(userYUV ? pCodecCtx->pix_fmt:AV_PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height);
    uint8_t *out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    C_LOG_INFO(QString("TTTTTTTTTT = numBytes= %1").arg(numBytes));


//    uint8_t *out_buffer2 = (uint8_t *) av_malloc(numBytes2 * sizeof(uint8_t));
    if(!userYUV)
    {
        avpicture_fill((AVPicture *) pFrameRGB, out_buffer, AV_PIX_FMT_RGB24,
                       pCodecCtx->width, pCodecCtx->height);
    }

    int y_size = pCodecCtx->width * pCodecCtx->height;

    AVPacket *packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
//    av_new_packet(packet, y_size); //分配packet的数据

    QImage image(pCodecCtx->width,pCodecCtx->height,QImage::Format_ARGB32);
    int index = 0;
    int got_picture;
    int ret = 0;
    int64_t pre_pts = 0;

    QFont font;
    font.setPixelSize(168);
    QFontMetrics fontMetrics(font);
    QDateTime preDateTime = QDateTime::currentDateTime();
    QTime preTime = preDateTime.time();
    QString text = preTime.toString("hh:mm:ss:zzz");
    QRect boundingRect = fontMetrics.boundingRect(text);
    QSize imageSize(pCodecCtx->width,pCodecCtx->height);
    QSize drawSize(boundingRect.width() + 10,boundingRect.height() + 10);
    QRect bgRect((imageSize.width() - drawSize.width()) / 2,(imageSize.height() - drawSize.height())/2,
                   drawSize.width(),drawSize.height());
    int fps = LJGolbalConfigManager::getInstance()->getFPS();
    int intervalMSes = 1000 / fps;
    QPainter painter(&image);
    painter.setPen(QPen(Qt::red));
    painter.setFont(font);


    C_LOG_INFO("TTTTTTTTTT");
    while (!m_device->m_bExit)
    {
        if(m_device->m_bCapture)
        {
            if (av_read_frame(pFormatCtx, packet) < 0)
            {
                av_seek_frame(pFormatCtx,0,0,0);
                pre_pts = 0;
                continue;
            }
            int64_t ms = 10;
            QTime sTime = QTime::currentTime();
            if (packet->stream_index == videoStream)
            {
                ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,packet);

                if (ret < 0)
                {
                    C_LOG_INFO(QString("decode error."));
                    return;
                }

                if (got_picture)
                {
                    if(!userYUV)
                    {
                        sws_scale(img_convert_ctx,
                                  (uint8_t const * const *) pFrame->data,
                                  pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                                  pFrameRGB->linesize);
                        SaveFrame(image,pFrameRGB, pCodecCtx->width,pCodecCtx->height,index++); //保存图片
//                        C_LOG_INFO(QString("image.width()=%1,linesize=%2").arg(image.width()).arg(pFrameRGB->linesize[0]));
                        if(!image.isNull())
                        {
                            static int s_count = 0;
                            s_count++;
                            if(s_count % 150 == 0)
                            {
//                                image.save(QString("C:\\work\\test_%1.png").arg(s_count));
                            }

                            AVE::VideoCaptureFormat format;
                            format.width = image.width();
                            format.height = image.height();
                            format.strides[0] = image.width() * 4;
                            format.pixel_format = AVE::PIXEL_FORMAT_BGRA32;
                            QDateTime datetime = QDateTime::currentDateTime();

                            QString text = datetime.time().toString("hh:mm:ss:zzz");
                            painter.drawText(bgRect,text);

                            m_device->callback_->OnIncomingCapturedData((char*)image.constBits(), image.width() * image.height() * 4,
                                                                        format, datetime.toMSecsSinceEpoch(), 1000);
                        }
                    }
                    else
                    {
                        int ysize = pCodecCtx->width * pCodecCtx->height;
                        AVE::VideoCaptureFormat format;
                        format.width =  pCodecCtx->width;
                        format.height =  pCodecCtx->height;
                        int index = 0;
                        for(int i = 0; i < 4;++i)
                        {
                            format.strides[i] = pFrame->linesize[i];
//                            C_LOG_INFO(QString("pFrame->linesize[%1]=%2").arg(i).arg(pFrame->linesize[i]));
                        }
                        for(int i = 0 ; i < ysize; ++i)
                        {
                            out_buffer[i] =  pFrame->data[0][i];
                            if(i < ysize / 4)
                            {
                                out_buffer[ysize+i] =  pFrame->data[1][i];
                                out_buffer[ysize + ysize / 4 + i] =  pFrame->data[2][i];
                            }
                        }
//                        C_LOG_INFO(QString("ssindex=%1,numBytes=%2").arg(index).arg(numBytes));
                        format.pixel_format = AVE::PIXEL_FORMAT_I420;
                        QDateTime datetime = QDateTime::currentDateTime();
                        m_device->callback_->OnIncomingCapturedData((char*)out_buffer, numBytes,
                                                                    format, datetime.toMSecsSinceEpoch(), 1000);
                    }
                }
                int ses =  packet->pts * av_q2d(pCodecCtx->time_base);
                ms = ses - pre_pts;
                pre_pts = ses;

                av_frame_free(&pFrame);
                pFrame = av_frame_alloc();
            }
            av_free_packet(packet);

            int dSec = qAbs(QTime::currentTime().msecsTo(sTime));
//                C_LOG_INFO(QString("pts=%1,ses=%2,ms=%3,dSec=%4").arg(packet->pts).arg(ses).arg(ms).arg(dSec));
            if(ms > dSec)
            {
                ms -= dSec;
            }
            else
            {
                ms = 1;
            }
            msleep(ms);
        }
        else
        {
            msleep(20);
        }
    }
    av_free_packet(packet);
    av_free(out_buffer);
    av_frame_free(&pFrameRGB);
    av_frame_free(&pFrame);

    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}
