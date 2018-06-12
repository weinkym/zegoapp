#include "clogsetting.h"
#include <QThread>

CLogFuncHelper::CLogFuncHelper(const QString &functionInfo):m_functionInfo(functionInfo)
{
    m_functionInfo=QString("[function:%1][threadId:%2]").arg(functionInfo).arg((long long)QThread::currentThreadId());
    C_LOG_INFO(QString("[Enter]%1").arg(m_functionInfo));
    m_time = QTime::currentTime();
}

CLogFuncHelper::~CLogFuncHelper()
{
    C_LOG_INFO(QString("[Leave]%1[consuming:%2ms]").arg(m_functionInfo).arg(m_time.msecsTo(QTime::currentTime())));
}
