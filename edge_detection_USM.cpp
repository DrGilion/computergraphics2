#define _USE_MATH_DEFINES
#include <cmath>

#include "edge_detection_USM.h"
#include "imageviewer.h"



EdgeDetectionUSM::EdgeDetectionUSM() : FunctionBox("Kantendetektion mittels Canny Edge Algorithmus"){
    layout->addRow("Gauss-Radius: ",gaussRadiusField.get());
    layout->addRow("Sigma: ",sigmaField.get());

    cannyEdgeLayout->addRow("Hysteresis Low: ",hysteresisLowField.get());
    cannyEdgeLayout->addRow("Hysteresis High: ",hysteresisHighField.get());
    cannyEdgeLayout->addRow(cannyEdgeButton.get());
    QObject::connect(cannyEdgeButton.get(), QPushButton::clicked, this, EdgeDetectionUSM::canny_edge );
    secondaryLayout->addLayout(cannyEdgeLayout.get());

    usmLayout->addRow("Alpha: ",alphaField.get());
    usmLayout->addRow(usmFilterButton.get());
    QObject::connect(usmFilterButton.get(), QPushButton::clicked, this, EdgeDetectionUSM::USM_filter );
    secondaryLayout->addLayout(usmLayout.get());

    layout->addRow(secondaryLayout.get());
}

void EdgeDetectionUSM::canny_edge(){

}

void EdgeDetectionUSM::USM_filter(){

}
