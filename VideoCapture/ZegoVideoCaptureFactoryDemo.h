#include "video_capture.h"
#include "ZegoVideoCaptureDeviceDemo.h"


class VideoCaptureFactoryGlue : public AVE::VideoCaptureFactory {
public:
	VideoCaptureFactoryGlue();
	virtual ~VideoCaptureFactoryGlue();
	virtual VideoCaptureDeviceBase* Create(const char* device_id) override;
	virtual void Destroy(VideoCaptureDeviceBase *vc) override;

protected:
	VideoCaptureDeviceGlue *device_;
};
