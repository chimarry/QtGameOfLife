#include "connector.h"
#include<iostream>
#include<string>
#include <chrono>
#include <thread>
#include <QThread>
using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds
Connector::Connector()
{

}

void Connector::change(const ImageChanger& imageChanger,const QString& q){
    imageChanger.setValue(q);
}

void Connector::connect(const ImageChanger& imageChanger, int x){
    QList<QThread *> runnables;
     for(int i = 3;i>1;--i){
      runnables.append()
       sleep_until(system_clock::now() + seconds(1));
       std::string s  =  std::to_string(i);
       QString q = QString::fromStdString("C:\\Users\\Marija\\Desktop\\Everything\\Wallpapers\\"+ s+".jpg");
       std::thread t1(change, std::ref(imageChanger), std::ref(q));
      }
}
