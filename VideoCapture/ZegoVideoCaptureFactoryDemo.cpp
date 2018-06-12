#include "ZegoVideoCaptureFactoryDemo.h"

VideoCaptureFactoryGlue::VideoCaptureFactoryGlue() : device_(0) {
}

VideoCaptureFactoryGlue::~VideoCaptureFactoryGlue() {
}

AVE::VideoCaptureDeviceBase* VideoCaptureFactoryGlue::Create(const char* device_id) {
	device_ = new VideoCaptureDeviceGlue();
	return device_;
}

void VideoCaptureFactoryGlue::Destroy(AVE::VideoCaptureDeviceBase *vc) {
	if (auto p = dynamic_cast<VideoCaptureDeviceGlue*>(vc)) {
		delete p;
	}
}