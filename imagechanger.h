#ifndef IMAGECHANGER_H
#define IMAGECHANGER_H
#include <QObject>

class ImageChanger:public QObject
{
Q_OBJECT
public:
    ImageChanger();
public slots:
    void setValue( QString value) const;

signals:
    void valueChanged( QString newValue) const;
};

#endif // IMAGECHANGER_H
