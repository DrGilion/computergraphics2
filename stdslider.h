#ifndef STDSLIDER_H
#define STDSLIDER_H

#include <QSlider>

class StdSlider : public QSlider{
    Q_OBJECT
public:
    StdSlider(int min, int max,int tickinterval = 10, int singlestep = 1, Qt::Orientation orientation = Qt::Horizontal);
};

#endif // STDSLIDER_H
