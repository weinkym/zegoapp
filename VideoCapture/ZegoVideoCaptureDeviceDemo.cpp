﻿#include "ZegoVideoCaptureDeviceDemo.h"
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
    m_image = QImage(400,400,QImage::Format_ARGB32);
    m_image.fill(QColor(0,123,12));
}

LJIncomingCapturedDataThread::~LJIncomingCapturedDataThread()
{

}

void LJIncomingCapturedDataThread::run()
{


//    Format_RGB32,
//    Format_ARGB32,
//    Format_ARGB32_Premultiplied,
//    Format_RGB16,
//    Format_ARGB8565_Premultiplied,
//    Format_RGB666,
//    Format_ARGB6666_Premultiplied,
//    Format_RGB555,
//    Format_ARGB8555_Premultiplied,
//    Format_RGB888,
//    Format_RGB444,
//    Format_ARGB4444_Premultiplied,
//    Format_RGBX8888,
//    Format_RGBA8888,
//    Format_RGBA8888_Premultiplied,
//    Format_BGR30,
//    Format_A2BGR30_Premultiplied,
//    Format_RGB30,
//    Format_A2RGB30_Premultiplied,
//    Format_Alpha8,
//    PIXEL_FORMAT_BGRA32 = 4,
//    PIXEL_FORMAT_RGBA32 = 5,
//    PIXEL_FORMAT_ARGB32 = 6,
//    PIXEL_FORMAT_ABGR32 = 7,
    if(!m_device)
    {
        return;
    }

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
        sendFrame();
    }
}

void LJIncomingCapturedDataThread::initVideo()
{
    unsigned version = avcodec_version();
    qDebug() << "version is:" << version;
    AVFormatContext *pFormatCtx = avformat_alloc_context();
//    m_filePath = "C:\\Users\\miao\\Videos\\test2.mp4";
    char file_path[] = "C:\\Users\\miao\\Videos\\test2.mp4";
    int ret = avformat_open_input(&pFormatCtx, file_path, NULL, NULL);
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
    QPainter painter(&m_image);
    painter.setPen(QPen(Qt::red));
    QFont font;
    font.setPixelSize(28);
    painter.setFont(font);
    QRect rect(0,0,m_image.width(),m_image.height());

    while (!m_device->m_bExit)
    {
        if (m_device->m_bCapture)
        {

            AVE::VideoCaptureFormat format;
            format.width = m_image.width();
            format.height = m_image.height();
            format.strides[0] = m_image.width() * 4;
            format.pixel_format = AVE::PIXEL_FORMAT_ARGB32;
            QDateTime datetime = QDateTime::currentDateTime();
            m_image.fill(QColor(0,255,0));

            painter.drawText(100,200,datetime.toString("yyyyMMdd hh:mm:ss.zzz"));
            painter.drawRect(rect);
            painter.drawEllipse(QRect(0,0,200,200));
            painter.drawLine(rect.topLeft(),rect.bottomRight());

            //            C_LOG_INFO("send data");
            m_device->callback_->OnIncomingCapturedData((char*)m_image.constBits(), m_image.width() * m_image.height() * 4,
                                                        format, datetime.toMSecsSinceEpoch(), 1000);
        }
        //        this->msleep(10);
        this->msleep(200);
    }
}

void SaveFrame(QImage &image,AVFrame *pFrame, int width, int height,int index)
{

//  FILE *pFile;
//  char szFilename[32];
  int  y;

  // Open file
//  sprintf(szFilename, "frame%d.ppm", index);
//  pFile=fopen(szFilename, "wb");
//  pFile=fopen("/Users/miaozw/Movies/test.png", "wb");

//  if(pFile==NULL)
//    return;

  // Write header
//  fprintf(pFile, "P6%d %d255", width, height);

  // Write pixel data
//  QImage image(width,height,QImage::Format_ARGB32);
  C_LOG_INFO(QString("width=%1,height=%2").arg(width).arg(height));
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
//        image.setPixelColor(x,y,QColor(r,b,g));
//        image.setPixelColor(x,y,QColor(b,r,g));
//        image.setPixelColor(x,y,QColor(b,g,r));
//        image.setPixelColor(x,y,QColor(g,b,r));
//        image.setPixelColor(x,y,QColor(g,r,b));
//          image.setPixelColor(x,y,QColor(b,0,0));
      }
//    printf("\n");
  }
//  image.save("/Users/miaozw/Movies/temp.png");

//  return image;
  // Close file
//  fclose(pFile);

}

void LJIncomingCapturedDataThread::sendFrame()
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


    int numBytes = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width,pCodecCtx->height);
    uint8_t *out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    C_LOG_INFO(QString("TTTTTTTTTT = numBytes= %1").arg(numBytes));

    C_LOG_INFO("TTTTTTTTTT");
    int numBytes2 = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height);

    C_LOG_INFO(QString("TTTTTTTTTT = numBytes2 = %1").arg(numBytes2));

    uint8_t *out_buffer2 = (uint8_t *) av_malloc(numBytes2 * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, out_buffer2, AV_PIX_FMT_RGB24,
                   pCodecCtx->width, pCodecCtx->height);

    int y_size = pCodecCtx->width * pCodecCtx->height;

    AVPacket *packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
    av_new_packet(packet, y_size); //分配packet的数据

//    av_dump_format(pFormatCtx, 0, file_path, 0); //输出视频信息

    QImage image(pCodecCtx->width,pCodecCtx->height,QImage::Format_ARGB32);
    int index = 0;
    int got_picture;
    int ret = 0;
    int64_t pre_pts = 0;

    C_LOG_INFO("TTTTTTTTTT");
    while (!m_device->m_bExit)
    {
        if(m_device->m_bCapture)
        {
            C_LOG_INFO("TTTTTTTTTT");
            if (av_read_frame(pFormatCtx, packet) < 0)
            {
                C_LOG_INFO("TTTTTTTTTT");
                av_seek_frame(pFormatCtx,0,0,0);
                C_LOG_INFO("TTTTTTTTTT");
                pre_pts = 0;
                //            break; //这里认为视频读取完了
                continue;
            }
            int64_t ms = 10;
            C_LOG_INFO(QString("packet->stream_index=%1,videoStream=%2").arg(packet->stream_index).arg(videoStream));

            if (packet->stream_index == videoStream) {
                C_LOG_INFO("TTTTTTTTTT");
                ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,packet);
                C_LOG_INFO("TTTTTTTTTT");

                if (ret < 0)
                {
                    C_LOG_INFO(QString("decode error."));
                    return;
                }

                if (got_picture) {
                    C_LOG_INFO("TTTTTTTTTT");
                    sws_scale(img_convert_ctx,
                              (uint8_t const * const *) pFrame->data,
                              pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                              pFrameRGB->linesize);

                    C_LOG_INFO(QString("pFrame=%1,pFrameRGB=%2").arg(pFrame->format).arg(pFrameRGB->format));
                    C_LOG_INFO("TTTTTTTTTT");

                    if(0)
                    {
                        SaveFrame(image,pFrameRGB, pCodecCtx->width,pCodecCtx->height,index++); //保存图片
                        C_LOG_INFO(QString("image.width()=%1,linesize=%2").arg(image.width()).arg(pFrameRGB->linesize[0]));
                        static int s_count = 0;
                        s_count++;
                        if(s_count == 8)
                        {
                            image.save("C:\\work\\test.png");
                        }

                        AVE::VideoCaptureFormat format;
                        format.width = image.width();
                        format.height = image.height();
                        format.strides[0] = image.width() * 4;
                        format.pixel_format = AVE::PIXEL_FORMAT_BGRA32;
                        QDateTime datetime = QDateTime::currentDateTime();
                        m_device->callback_->OnIncomingCapturedData((char*)image.constBits(), image.width() * image.height() * 4,
                                                                    format, datetime.toMSecsSinceEpoch(), 1000);
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
                            C_LOG_INFO(QString("pFrame->linesize[%1]=%2").arg(i).arg(pFrame->linesize[i]));
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
                        C_LOG_INFO(QString("ssindex=%1,numBytes=%2").arg(index).arg(numBytes));
                        format.pixel_format = AVE::PIXEL_FORMAT_I420;
                        QDateTime datetime = QDateTime::currentDateTime();
                        m_device->callback_->OnIncomingCapturedData((char*)out_buffer, numBytes,
                                                                    format, datetime.toMSecsSinceEpoch(), 1000);
                    }
                    int ses =  packet->pts * av_q2d(pCodecCtx->time_base) / 1000;
                    pre_pts = ses;
                    ms += ses;
                    C_LOG_INFO(QString("pts=%1,ses=%2,%3").arg(packet->pts).arg(ses).arg(ms));
                    C_LOG_INFO("TTTTTTTTTT");
                }
            }
            msleep(ms);
        }
        else
        {
            msleep(20);
        }
    }
    av_free_packet(packet);
    av_free(out_buffer2);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}
