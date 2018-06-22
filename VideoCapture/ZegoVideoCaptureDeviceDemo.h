#pragma once

#include "video_capture.h"
#include "stdlib.h"
#include "string.h"
#include <QTimer>
#include <QThread>
#include <QImage>
#include <QDateTime>
#include "clogsetting.h"
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QPen>
#include <QFile>
#include "ljgolbalconfigmanager.h"
#include <QFontMetrics>

using namespace AVE;


extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include <libavutil/time.h>
    #include "libavutil/pixfmt.h"
    #include "libswscale/swscale.h"
    #include "libswresample/swresample.h"
}

struct LJVideoData{
    LJVideoData():pFormatCtx(NULL),pCodecCtxA(NULL),pCodecCtxV(NULL),pCodecA(NULL),pCodecV(NULL),videoStream(-1),audioStream(-1){};
    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtxA;
    AVCodecContext *pCodecCtxV;
    AVCodec *pCodecA;
    AVCodec *pCodecV;
    int videoStream;
    int audioStream;
};

class VideoCaptureDeviceGlue;
class LJIncomingCapturedDataThread : public QThread
{
    Q_OBJECT
public:
    LJIncomingCapturedDataThread(VideoCaptureDeviceGlue *device,QObject *parent = Q_NULLPTR);
    ~LJIncomingCapturedDataThread();

protected:
    void run();

private:
    void initVideo();

    void sendImage();

    void sendFrame(bool userYUV);

private:
    VideoCaptureDeviceGlue *m_device;
    QImage m_image;
    LJVideoData m_videoData;
};


class VideoCaptureDeviceGlue : public AVE::VideoCaptureDeviceBase {
public:
	VideoCaptureDeviceGlue();
	virtual ~VideoCaptureDeviceGlue();

	virtual void AllocateAndStart(Client* client) override;
	virtual void StopAndDeAllocate() override;
	virtual int StartCapture() override;
	virtual int StopCapture() override;

	virtual VideoPixelBufferType SupportBufferType() override {
		return VideoPixelBufferType::PIXEL_BUFFER_TYPE_MEM;
	}

	virtual void* GetInterface() override {
		return NULL;
	}

	void OnVideoTimer();

	Client *client_;
	VideoCaptureCallback *callback_;

private:
    friend class LJIncomingCapturedDataThread;
    LJIncomingCapturedDataThread *m_thread;
	bool m_bExit;
	bool m_bCapture;
	size_t data_size_;
	char* data_;
	int width_;
	int height_;
	QTimer *timer;
};
