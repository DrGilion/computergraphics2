#ifndef FILTERTEST_H
#define FILTERTEST_H

#include "functionbox.h"

#include <memory>
#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>



class FilterTest : public FunctionBox{
    Q_OBJECT
public:
    FilterTest();
private:
    std::unique_ptr<QFormLayout> layout{new QFormLayout(this)};
    std::unique_ptr<QComboBox> options{new QComboBox()};
    /*Filter* (*filters[4])(void) ={
            &Filter::SobelHorizontal,
            &Filter::SobelVertical,
            &Filter::LaPlace,
            &Filter::Gauss3
    };*/
    std::unique_ptr<QPushButton> filterButton{new QPushButton(QString("Filter ausprobieren"))};
private slots:
    void filter();
};

#endif // FILTERTEST_H
