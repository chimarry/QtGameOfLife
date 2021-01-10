#ifndef IMAGETHREAD_H
#define IMAGETHREAD_H

#include <QThread>

class ImageThread:public QThread
{
public:
    ImageThread();
    void run();
};

#endif // IMAGETHREAD_H
