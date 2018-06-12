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

using namespace AVE;

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
    VideoCaptureDeviceGlue *m_device;
    QImage m_image;
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
