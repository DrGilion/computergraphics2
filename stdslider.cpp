#include "stdslider.h"


StdSlider::StdSlider(int min, int max, int tickinterval, int singlestep,Qt::Orientation orientation) : QSlider(orientation){
    this->setMinimum(min);
    this->setMaximum(max);
    this->setTickPosition(QSlider::TicksBelow);
    this->setTickInterval(tickinterval);
    this->setSingleStep(singlestep);
    QObject::connect(this,QSlider::valueChanged,this,[&](int val){this->setToolTip(QString("Aktueller Wert: %1").arg(val));});
}
