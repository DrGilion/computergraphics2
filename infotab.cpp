#include "infotab.h"
#include "fleximage.h"
#include "controller.h"

InfoTab::InfoTab() : QWidget(){
    layout->addRow("fConst(Grau): ",fConstGrayLabel.get());
    layout->addRow("fConst(Rot): ",fConstRedLabel.get());
    layout->addRow("fConst(Grün): ",fConstGreenLabel.get());
    layout->addRow("fConst(Blau): ",fConstBlueLabel.get());
    layout->addRow("Varianz(Grau): ",varianzGrayLabel.get());
    layout->addRow("Varianz(Rot): ",varianzRedLabel.get());
    layout->addRow("Varianz(Grün): ",varianzGreenLabel.get());
    layout->addRow("Varianz(Blau): ",varianzBlueLabel.get());
}

void InfoTab::updateTab(){
    FlexImage* pic = Controller::instance()->getLastImage();
    fConstGrayLabel->setText(QString("%1").arg(pic->calculateGrayFConst()));
    fConstRedLabel->setText(QString("%1").arg(pic->calculateRedFConst()));
    fConstGreenLabel->setText(QString("%1").arg(pic->calculateGreenFConst()));
    fConstBlueLabel->setText(QString("%1").arg(pic->calculateBlueFConst()));
    varianzGrayLabel->setText(QString("%1").arg(pic->calculateGrayVarianz()));
    varianzRedLabel->setText(QString("%1").arg(pic->calculateRedVarianz()));
    varianzGreenLabel->setText(QString("%1").arg(pic->calculateGreenVarianz()));
    varianzBlueLabel->setText(QString("%1").arg(pic->calculateBlueVarianz()));
}
