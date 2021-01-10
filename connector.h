#ifndef CONNECTOR_H
#define CONNECTOR_H

#include<qobject.h>
#include<mainwindow.h>
#include<imagechanger.h>

class Connector
{
public:
    Connector();
    void connect(const ImageChanger&, int);
    static void change(const ImageChanger&,const QString&);

private:
    ImageChanger _imageChanger;
;};

#endif // CONNECTOR_H
