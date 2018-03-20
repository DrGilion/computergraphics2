#include "geradendetektor.h"
#include "imageviewer.h"

GeradenDetektor::GeradenDetektor() : FunctionBox("Geraden detektieren") {
    layout->addRow("a-Steps: ",aStepsField.get());
    layout->addRow("r-Steps: ",rStepsField.get());
    layout->addRow("Kontrast: ",contrastField.get());
    layout->addRow("Schwarz-Weiß-Grenze: ",blackWhiteField.get());
    layout->addRow(detectLinesButton.get());
    QObject::connect(detectLinesButton.get(), QPushButton::clicked, this, GeradenDetektor::detectLines );
}

void GeradenDetektor::detectLines(){

}
