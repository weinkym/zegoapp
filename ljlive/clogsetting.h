#ifndef CLOGSETTING_H
#define CLOGSETTING_H
#include <QDebug>
#include <QTime>
//#include "util/base.h"

class CLogFuncHelper
{
public:
    CLogFuncHelper(const QString &funcionName);
    ~CLogFuncHelper();
private:
    QString m_functionInfo;
    QTime m_time;
};
#define C_LOG_FUNCTION CLogFuncHelper logFuncHelper(Q_FUNC_INFO)

#define C_LOG_DATE QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss ")
#define C_LOG_FILE_INFO (QString(" [file = ") + QString(__FILE__) + QString(" ,Line = ") + QString::number(__LINE__) + QString("]"))

#define C_LOG_CONTENT(log) QString("===LJLIVE===")+C_LOG_DATE+(log)+C_LOG_FILE_INFO

#ifdef Q_OS_WIN
#define C_CONTENT_DEBUG(log) qDebug()<<(C_LOG_CONTENT(log))
#else
#define C_CONTENT_DEBUG(log)
#endif

//#define C_LOG_TEST(log) {C_CONTENT_DEBUG(log);blog(LOG_DEBUG,"%s",(C_LOG_CONTENT(log)).toLocal8Bit().data());}
//#define C_LOG_DEBUG(log) {C_CONTENT_DEBUG(log);blog(LOG_INFO,"%s",(C_LOG_CONTENT(log)).toLocal8Bit().data());}
//#define C_LOG_INFO(log) {C_CONTENT_DEBUG(log);blog(LOG_INFO,"%s",(C_LOG_CONTENT(log)).toLocal8Bit().data());}
//#define C_LOG_WARNING(log) {C_CONTENT_DEBUG(log);blog(LOG_WARNING,"%s",(C_LOG_CONTENT(log)).toLocal8Bit().data());}
//#define C_LOG_ERROR(log) {C_CONTENT_DEBUG(log);blog(LOG_ERROR,"%s",(C_LOG_CONTENT(log)).toLocal8Bit().data());}

#define C_LOG_TEST(log) {C_CONTENT_DEBUG(log);}
#define C_LOG_DEBUG(log) {C_CONTENT_DEBUG(log);}
#define C_LOG_INFO(log) {C_CONTENT_DEBUG(log);}
#define C_LOG_WARNING(log) {C_CONTENT_DEBUG(log);}
#define C_LOG_ERROR(log) {C_CONTENT_DEBUG(log);}

#define C_LOG_TEST_TT C_LOG_TEST(QString("TTTTTTTTTTTTTTTV"));
#define C_LOG_TEST_LINE C_LOG_TEST(QString("LINE=%1").arg(__LINE__));

#define C_BOOL_VALUE_TO_STRING(value) (value ? "TRUE":"FALSE")

#define C_VALUE_LOG_TEST_BOOL(value) C_LOG_TEST(QString(#value"=%1").arg(C_BOOL_VALUE_TO_STRING(value)))
#define C_VALUE_LOG_DEBUG_BOOL(value) C_LOG_DEBUG(QString(#value"=%1").arg(C_BOOL_VALUE_TO_STRING(value)))
#define C_VALUE_LOG_INFO_BOOL(value) C_LOG_INFO(QString(#value"=%1").arg(C_BOOL_VALUE_TO_STRING(value)))
#define C_VALUE_LOG_WARNING_BOOL(value) C_LOG_WARNING(QString(#value"=%1").arg(C_BOOL_VALUE_TO_STRING(value)))
#define C_VALUE_LOG_ERROR_BOOL(value) C_LOG_ERROR(QString(#value"=%1").arg(C_BOOL_VALUE_TO_STRING(value)))

//#define C_VALUE_LOG_TEST(value) C_LOG_TEST(QString(#value"=%1").arg(value))
//#define C_VALUE_LOG_DEBUG(value) C_LOG_DEBUG(QString(#value"=%1").arg(value))
#define C_VALUE_LOG_INFO(value) C_LOG_INFO(QString(#value"=%1").arg(value))
//#define C_VALUE_LOG_WARNING(value) C_LOG_WARNING(QString(#value"=%1").arg(value))
//#define C_VALUE_LOG_ERROR(value) C_LOG_ERROR(QString(#value"=%1").arg(value))

#endif // CLOGSETTING_H
