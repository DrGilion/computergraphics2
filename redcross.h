#ifndef REDCROSS_H
#define REDCROSS_H

#include "fleximage.h"
#include "functionbox.h"
#include "stdslider.h"
#include "controller.h"

#include <memory>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QGroupBox>

class RedCross : public FunctionBox{
    Q_OBJECT
public:
    RedCross();
    std::unique_ptr<QFormLayout> layout{new QFormLayout(this)};
    std::unique_ptr<QSpinBox> sizeBox{new QSpinBox()};
    std::unique_ptr<QSlider> redSlider{new StdSlider(0,255)};
    std::unique_ptr<QSlider> greenSlider{new StdSlider(0,255)};
    std::unique_ptr<QSlider> blueSlider{new StdSlider(0,255)};
    std::unique_ptr<QPushButton> drawButton{new QPushButton(QString("Kreuz malen"))};

private slots:
    void drawCross();
};

#endif // REDCROSS_H
