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

LJGolbalConfigManager::LJGolbalConfigManager()
    :m_videoCaptureResolution(1280,720)
    ,m_videoEncodeResolution(1280,720)
{
}
