#include "filterlinear.h"
#include "imageviewer.h"
#include "filter.h"
#include "controller.h"

#include <vector>
#include <QIntValidator>
#include <QComboBox>
#include <QHeaderView>

FilterLinear::FilterLinear() : FunctionBox("Bild Filtern"){
    for(int i = 3; i < 10 ; i+=2) sizeBox->addItem(QString::number(i));
    initTable();
    QObject::connect(sizeButton.get(), QPushButton::clicked, this, FilterLinear::initTable );
    layout->addRow("Größe: ",sizeBox.get());
    layout->addRow(sizeButton.get());

    for(auto& entry : edgeFunctionMap) cornerMethodField->addItem(entry.first);
    layout->addRow("Randbehandlung: ",cornerMethodField.get());

    layout->addRow(filtertable.get());


    layout->addRow(filterButton.get());
    QObject::connect(filterButton.get(), QPushButton::clicked, this, FilterLinear::filterLinearly );
}

void FilterLinear::initTable(){
    int size = sizeBox->currentText().toInt();
    if(size != filtertable->columnCount()){
        filtertable->setRowCount(size);
        filtertable->setColumnCount(size);

        //calculating ideal size
        int w = filtertable->verticalHeader()->width() + 4; // +4 seems to be needed
        for (int i = 0; i < size; i++)
            w += filtertable->columnWidth(i); // seems to include gridline (on my machine)
        int h = filtertable->horizontalHeader()->height() + 4;
        for (int i = 0; i < size; i++)
            h += filtertable->rowHeight(i);
        QSize newSize(w, h);
        filtertable->setMaximumSize(newSize);
        filtertable->setMinimumSize(filtertable->maximumSize());
        //filtertable->setValidator(new QIntValidator(-99,99));
    }
}

void FilterLinear::filterLinearly(){
    EdgeAlgorithm edge = edgeFunctionMap[cornerMethodField->currentText()];
    std::vector<std::vector<int>> input;
    Filter filter(input);
    Controller::instance()->filterLinear(filter,edge);
}
