#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <QThread>
#include<QHash>
#include<QDebug>

class ThreadPool : public QObject
{
  Q_OBJECT
public:
  enum ThreadType
  {
      fixedThreadConnect, //固定每个线程处理多少事情
      fixedThread, //固定线程数
  };
           ~ThreadPool();
           static ThreadPool & getClass();//返回实体
           QThread * getThread(ThreadType t= fixedThreadConnect,unsigned int m=3);//取出线程
           void removeThread(QThread *);//连接断开，线程计数减一
           void removeAll();//删除全部线程
           void printfThread();//打印线程需要显示的信息
protected:
               void openThread();//创建并开启线程
               QThread * findThreadSize();//固定线程数的查找
               QThread *findHandleSize();//固定连接数查找
signals:
private:
       void initThreadType(ThreadType type ,unsigned int max);//初始化线程管理的方式
       explicit ThreadPool(QObject *parent = nullptr);
      bool m_init;//线程初始化标志位
      ThreadType m_type;//线程类型
      unsigned int m_max;//最大连接数
      QHash<QThread *, unsigned int> m_threadSize;//保存每个线程的数目
};

#endif // THREADPOOL_H
