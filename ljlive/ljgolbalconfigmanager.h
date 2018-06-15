#ifndef LJGOLBALCONFIGMANAGER_H
#define LJGOLBALCONFIGMANAGER_H

#include <QSize>

class LJGolbalConfigManager
{
public:
    static LJGolbalConfigManager *getInstance();
    QSize getVideoCaptureResolution();
    QSize getVideoEncodeResolution();
    int getFPS();

protected:
    LJGolbalConfigManager();

private:
    static LJGolbalConfigManager* m_instance;
    QSize m_videoCaptureResolution;
    QSize m_videoEncodeResolution;
    int m_fps;
};

#endif // LJGOLBALCONFIGMANAGER_H
