#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <threadpool.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
signals :
       void startThread();
private:
    Ui::Widget *ui;
};


class work : public QObject
{
    Q_OBJECT
private:
   QString m_name;
public:
    work(const QString & name,QObject *p =nullptr):QObject(p) { m_name = name; }
    void run()const
    {
       qDebug() << m_name <<"的工作线程 work =" << QThread::currentThreadId();
    }
};

#endif // WIDGET_H
