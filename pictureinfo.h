#ifndef PICTUREINFO_H
#define PICTUREINFO_H

#include <memory>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSlider>
#include <QSpinBox>

#include "functionbox.h"
#include "stdslider.h"
#include "fleximage.h"
#include "histogramm.h"



class PictureInfo : public FunctionBox{
    Q_OBJECT
public:
    PictureInfo();
private:
    std::unique_ptr<QFormLayout> layout{new QFormLayout(this)};

    //Helligkeit
    std::unique_ptr<QSlider> brightSlider{new StdSlider(-255,255)};
    std::unique_ptr<QPushButton> brightButton{new QPushButton(QString("Helligkeit anpassen"))};
    std::unique_ptr<QHBoxLayout> secondaryLayout{new QHBoxLayout()};

    //Kontrast
    std::unique_ptr<QFormLayout> contrastLayout{new QFormLayout()};
    std::unique_ptr<QSlider> contrastSlider{new StdSlider(0,200)};
    std::unique_ptr<QPushButton> contrastButton{new QPushButton(QString("Kontrast anpassen"))};
    std::unique_ptr<QPushButton> autoContrastButton{new QPushButton(QString("Automatische Kontrastanpassung"))};

    //Robuster Kontrast
    std::unique_ptr<QFormLayout> robustContrastLayout{new QFormLayout()};
    std::unique_ptr<QSpinBox> slow{new QSpinBox()};
    std::unique_ptr<QSpinBox> shigh{new QSpinBox()};
    std::unique_ptr<QPushButton> robustContrastButton{new QPushButton(QString("Robuste Kontrastanpassung"))};

private slots:
    void applyBrightness();
    void applyContrast();
    void applyAutomaticContrast();
    void applyRobustContrast();
};

#endif // PICTUREINFO_H
