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
