#ifndef MISCOPERATIONS_H
#define MISCOPERATIONS_H

#include <memory>
#include <QVBoxLayout>
#include <QPushButton>
#include "functionbox.h"

class MiscOperations : public FunctionBox{
    Q_OBJECT
public:
    MiscOperations();
    std::unique_ptr<QVBoxLayout> layout{new QVBoxLayout(this)};
    std::unique_ptr<QPushButton> toGrayButton{new QPushButton(QString("Schwarz-Weiss filtern"))};
    std::unique_ptr<QPushButton> invertButton{new QPushButton(QString("Farben invertieren"))};

private slots:
    void toGray();
    void invert();
};

#endif // MISCOPERATIONS_H
