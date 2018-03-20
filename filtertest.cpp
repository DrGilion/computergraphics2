#include "imageviewer.h"
#include "filtertest.h"
#include "filter.h"


#include <QVBoxLayout>

FilterTest::FilterTest() : FunctionBox("Filter ausprobieren"){
    options->addItem("Sobel Horizontal");
    options->addItem("Sobel Vertical");
    options->addItem("LaPlace");
    options->addItem("Gauss 3x3");
    layout->addRow("Filter: ",options.get());
    layout->addRow(filterButton.get());
    QObject::connect(filterButton.get(), QPushButton::clicked, this, FilterTest::filter );
}

void FilterTest::filter(){

}
