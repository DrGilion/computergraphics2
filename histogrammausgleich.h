#ifndef HISTOGRAMMAUSGLEICH_H
#define HISTOGRAMMAUSGLEICH_H

#include "functionbox.h"

#include <memory>
#include <QImage>
#include <QVBoxLayout>
#include <QPushButton>



class HistogrammAusgleich : public FunctionBox{
    Q_OBJECT
public:
    HistogrammAusgleich();
private:
    std::unique_ptr<QVBoxLayout> layout{new QVBoxLayout(this)};
    std::unique_ptr<QPushButton> loadPictureButton{new QPushButton(QString("Referenzbild laden"))};
    FlexImage* referenceImage;
    std::unique_ptr<QLabel> referencePicture{new QLabel()};
    std::unique_ptr<QPushButton> linearEqualizationButton{new QPushButton(QString("Linearer Histogrammausgleich"))};
    std::unique_ptr<QPushButton> piecewiseLinearEqualizationButton{new QPushButton(QString("stückweiser linearer Histogrammausgleich"))};
    std::unique_ptr<QPushButton> successiveEqualizationButton{new QPushButton(QString("sukzessiver Histogrammausgleich"))};
private slots:
    void loadReferencePicture();
    void linearEqualization();
    void piecewiseLinearEqualization();
    void successiveEqualization();
};

#endif // HISTOGRAMMAUSGLEICH_H
