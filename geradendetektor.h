#ifndef GERADENDETEKTOR_H
#define GERADENDETEKTOR_H

#include "functionbox.h"

#include <memory>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

class GeradenDetektor : public FunctionBox{
public:
    GeradenDetektor();
private:
    std::unique_ptr<QFormLayout> layout{new QFormLayout(this)};
    std::unique_ptr<QLineEdit> aStepsField{new QLineEdit()};
    std::unique_ptr<QLineEdit> rStepsField{new QLineEdit()};
    std::unique_ptr<QLineEdit> contrastField{new QLineEdit()};
    std::unique_ptr<QLineEdit> blackWhiteField{new QLineEdit()};
    std::unique_ptr<QPushButton> detectLinesButton{new QPushButton(QString("Gerade detektieren"))};
private slots:
    void detectLines();
};

#endif // GERADENDETEKTOR_H
