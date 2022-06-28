#include "threadpool.h"
#include<QDebug>
ThreadPool::ThreadPool(QObject *parent) : QObject(parent),m_init(0)
{

}
ThreadPool::~ThreadPool() //停止所有线程，并释放资源
{
    QThread * tmp;
    for (auto it = m_threadSize.begin(); it != m_threadSize.end(); ++it)
    {
        tmp = it.key();
        tmp->exit();
        tmp->wait(3000);
        delete tmp;
    }
}
ThreadPool &ThreadPool::getClass()
{
    static  ThreadPool pool;
      return pool;
}
QThread * ThreadPool::getThread(ThreadType t,unsigned int m)
{
     if(!m_init)
       {
         initThreadType(t,m);
       }
        if(m_type==fixedThread)
          return findThreadSize();
      else
          return findHandleSize();
}
void ThreadPool::initThreadType(ThreadType type,unsigned int max)
{
          if(!m_init)
            {
              this->m_type=type;
              this->m_max=max;
              if(this->m_max==0)
                {
                  if(type==fixedThread)
                    this->m_max=10;
                   else
                    this->m_max=1000;
                }
                if(type==fixedThread)
                  this->openThread();
                else
                {
                    QThread * p = new QThread;
                    m_threadSize.insert(p,0);
                    p->start();
                }
            }
              m_init=1;
          return;
}
void ThreadPool::openThread()
{
   QThread *p;
   for(unsigned int i=0;i<m_max;i++)
     {
       p=new QThread;
       m_threadSize.insert(p,i);
       p->start();
     }
}
QThread * ThreadPool::findHandleSize() //查找到线程里的连接数小于最大值就返回查找到的，找不到就新建一个线程
{
    for (auto it  = m_threadSize.begin();it != m_threadSize.end() ;++it)
    {
        if (it.value() <m_max)
        {
            ++it.value() ;
            return it.key();
        }
    }
    static int i;
    i++;
    QThread * tmp = new QThread;
    m_threadSize.insert(tmp,i);
    tmp->start();
    return tmp;
}

QThread *ThreadPool::findThreadSize() //遍历查找所有线程中连接数最小的那个，返回
{
    auto it = m_threadSize.begin();
    auto ite = m_threadSize.begin();
    for (++it ; it != m_threadSize.end(); ++it)
    {
        if (it.value() < ite.value())
        {
            ite = it;
        }
    }
    ite.value() ++;
    return ite.key();
}
void ThreadPool::removeThread(QThread * thread)
{
    auto t = m_threadSize.find(thread);
    if (t != m_threadSize.end())
    {
        t.value() --;
        if (m_type == fixedThreadConnect && t.value() == 0 && m_threadSize.size() > 1)
        {
            m_threadSize.remove(thread);
            thread->exit();
            thread->wait(3000);
            delete thread;
        }
    }
}
void ThreadPool::removeAll()
{
  QThread * tmp;
  for (auto it = m_threadSize.begin(); it != m_threadSize.end(); ++it)
  {
      tmp = it.key();
      tmp->exit();
      tmp->wait(3000);
      delete tmp;
  }
}
void ThreadPool::printfThread()
{
  QString show;
  qDebug()<<"线程池线程为主线程"<<QThread::currentThreadId();
  qDebug()<<"threadSize内容"<<m_threadSize;
  qDebug()<<"最大连接数"<<m_max;
  if(m_type==fixedThread)
  {
    show="THNUM类型,一个处理配一个线程";
  }
   else
  {
      show="CONN类型,一个线程,配Max个处理";
  }
  qDebug()<<show;
  if(m_init)
  {
      show="true 初始化完成";
  }
  else
  {
      show="false 未初始化";
  }
  qDebug()<<show;
}
