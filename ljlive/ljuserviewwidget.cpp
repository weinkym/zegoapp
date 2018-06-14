#include "ljuserviewwidget.h"
#include "ui_ljuserviewwidget.h"

LJUserViewWidget::LJUserViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LJUserViewWidget)
{
    ui->setupUi(this);
}

LJUserViewWidget::~LJUserViewWidget()
{
    delete ui;
}

void LJUserViewWidget::setStreamId(const QString &streamId)
{
    m_streamId = streamId;
}

void *LJUserViewWidget::getViewId()
{
    return (void*)ui->widgetView->winId();
}

void LJUserViewWidget::setContent(const QString &content)
{
    ui->labelName->setText(content);
}

QString LJUserViewWidget::getStreamId()
{
    return m_streamId;
}
