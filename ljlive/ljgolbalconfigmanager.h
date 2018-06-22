#ifndef LJGOLBALCONFIGMANAGER_H
#define LJGOLBALCONFIGMANAGER_H

#include <QSize>
#include <QString>
#include <QApplication>
#include <QDir>
#include "clogsetting.h"

class LJGolbalConfigManager
{
public:
    enum DataType{
        DATA_TYPE_IMAGE,
        DATA_TYPE_VIDEO,
        DATA_TYPE_YUV,
    };

    static LJGolbalConfigManager *getInstance();
    QSize getVideoCaptureResolution();
    QSize getVideoEncodeResolution();
    int getFPS();
    void setFPS(int fps);
    QString getVideoPath();
    void setVideoPath(const QString &path);
    QString getImagePath();
    void setSizeType(int size);

    DataType getDataType();
    void setDataType(DataType type);

protected:
    LJGolbalConfigManager();

private:
    static LJGolbalConfigManager* m_instance;
    QSize m_videoCaptureResolution;
    QSize m_videoEncodeResolution;
    int m_fps;
    int m_sizeType;
    DataType m_dataType;
    QString m_videoPath;
};

#endif // LJGOLBALCONFIGMANAGER_H
