#include "imagechanger.h"

ImageChanger::ImageChanger()
{
}
void ImageChanger::setValue(QString value) const
{
    emit valueChanged(value);
}
