#include "ljgolbalconfigmanager.h"

LJGolbalConfigManager *LJGolbalConfigManager::m_instance = NULL;
LJGolbalConfigManager *LJGolbalConfigManager::getInstance()
{
    if(!m_instance)
    {
        m_instance = new LJGolbalConfigManager;
    }
    return m_instance;
}

QSize LJGolbalConfigManager::getVideoCaptureResolution()
{
    return m_videoCaptureResolution;
}

QSize LJGolbalConfigManager::getVideoEncodeResolution()
{
    return m_videoEncodeResolution;
}

int LJGolbalConfigManager::getFPS()
{
    return m_fps;
}

void LJGolbalConfigManager::setFPS(int fps)
{
    m_fps = fps;
}

QString LJGolbalConfigManager::getVideoPath()
{
    return m_videoPath;
}

void LJGolbalConfigManager::setVideoPath(const QString &path)
{
    C_LOG_INFO(QString("path=%1").arg(path));
    m_videoPath = path;
}

QString LJGolbalConfigManager::getImagePath()
{
    QString videoPath = QDir::toNativeSeparators(QApplication::applicationDirPath()+QString("/%1P.jpg").arg(m_sizeType));
    return videoPath;
}

void LJGolbalConfigManager::setSizeType(int size)
{
    if(size == 720 || size == 480 || size == 360)
    {
        m_sizeType = size;
    }
    C_LOG_INFO(QString("m_sizeType=%1").arg(m_sizeType));
}

LJGolbalConfigManager::DataType LJGolbalConfigManager::getDataType()
{
    return m_dataType;
}

void LJGolbalConfigManager::setDataType(LJGolbalConfigManager::DataType type)
{
    m_dataType = type;
}

LJGolbalConfigManager::LJGolbalConfigManager()
    :m_videoCaptureResolution(1280,720)
    ,m_videoEncodeResolution(1280,720)
    ,m_fps(10)
    ,m_sizeType(720)
    ,m_dataType(DATA_TYPE_IMAGE)
//    :m_videoCaptureResolution(720,480)
//    ,m_videoEncodeResolution(720,480)
{
}
