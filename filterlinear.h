#ifndef FILTERLINEAR_H
#define FILTERLINEAR_H

#include "functionbox.h"
#include "controller.h"

#include <memory>
#include <QFormLayout>
#include <QComboBox>
#include <QTableWidget>

enum class EdgeAlgorithm : int;

class FilterLinear : public FunctionBox{
    Q_OBJECT
public:
    FilterLinear();
private:
    std::unique_ptr<QFormLayout> layout{new QFormLayout(this)};

    std::unique_ptr<QComboBox> sizeBox{new QComboBox()};
    std::unique_ptr<QPushButton> sizeButton{new QPushButton(QString("Filtergroesse anpassen"))};
    std::unique_ptr<QComboBox> cornerMethodField{new QComboBox()};
    std::map<QString,EdgeAlgorithm> edgeFunctionMap{{QString("Zero Padding"),EdgeAlgorithm::ZEROPADDING}};
    //2d array of fields
    std::unique_ptr<QTableWidget> filtertable{new QTableWidget(3,3)};


    std::unique_ptr<QPushButton> filterButton{new QPushButton(QString("Linear filtern"))};
private slots:
    void filterLinearly();
    void initTable();
};

#endif // FILTERLINEAR_H
