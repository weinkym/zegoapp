#ifndef LJUSERVIEWWIDGET_H
#define LJUSERVIEWWIDGET_H

#include <QWidget>

namespace Ui {
class LJUserViewWidget;
}

class LJUserViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LJUserViewWidget(QWidget *parent = 0);
    ~LJUserViewWidget();
    void setStreamId(const QString &streamId);
    void *getViewId();
    void setContent(const QString &content);
    QString getStreamId();

private:
    Ui::LJUserViewWidget *ui;
    QString m_streamId;
};

#endif // LJUSERVIEWWIDGET_H
