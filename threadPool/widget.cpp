#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //线程池用的是moveThread的方式

    //获取线程  （参数1初始化管理线程池）(参数2设置线程的数量)
    //第一种模式是 固定线程，
    auto th = ThreadPool::getClass().getThread(ThreadPool::ThreadType::fixedThread,3);
    auto thh = ThreadPool::getClass().getThread(ThreadPool::ThreadType::fixedThread,3);
    work *c = new work("1号");
    work *cc= new work("2号");
    c->moveToThread(th);//移入线程
    cc->moveToThread(thh);//移入线程
    //用信号控制子线程 处理 (如果不用信号调用，次方法失效，数据处理会在主线程)
    connect(this,&Widget::startThread,c,&work::run);//第一种模式
    connect(this,&Widget::startThread,cc,&work::run);//第一种模式
    emit startThread(); //打开信号
    disconnect(this,&Widget::startThread,c,&work::run);//第一种模式
    disconnect(this,&Widget::startThread,cc,&work::run);//第一种模式

    //第二种模式是 固定线程

//    auto th1 = ThreadPool::getClass().getThread(ThreadPool::ThreadType::fixedThreadConnect,3);
//    auto th2 = ThreadPool::getClass().getThread(ThreadPool::ThreadType::fixedThreadConnect,3);
//    work *c1 = new work("3号");
//    work *cc1= new work("4号");
//    c1->moveToThread(th1);//移入线程
//    cc1->moveToThread(th2);//移入线程
//    //用信号控制子线程 处理 (如果不用信号调用，次方法失效，数据处理会在主线程)

//    emit startThread(); //打开信号
//    disconnect(this,&Widget::startThread,c1,&work::run);
//    disconnect(this,&Widget::startThread,cc1,&work::run);


    qDebug() << "主线程" << QThread::currentThreadId();

}

Widget::~Widget()
{
    delete ui;
}

