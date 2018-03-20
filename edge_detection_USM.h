#ifndef EDGE_DETECTION_USM_H
#define EDGE_DETECTION_USM_H

#include "functionbox.h"

#include <memory>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>



class EdgeDetectionUSM : public FunctionBox{
public:
    EdgeDetectionUSM();
private:
    std::unique_ptr<QFormLayout> layout{new QFormLayout(this)};
    std::unique_ptr<QLineEdit> gaussRadiusField{new QLineEdit()};
    std::unique_ptr<QLineEdit> sigmaField{new QLineEdit()};

    std::unique_ptr<QHBoxLayout> secondaryLayout{new QHBoxLayout()};

    //for canny edge detection
    std::unique_ptr<QFormLayout> cannyEdgeLayout{new QFormLayout()};
    std::unique_ptr<QLineEdit> hysteresisLowField{new QLineEdit()};
    std::unique_ptr<QLineEdit> hysteresisHighField{new QLineEdit()};
    std::unique_ptr<QPushButton> cannyEdgeButton{new QPushButton(QString("Canny Edge Detection"))};

    //for usm filtering
    std::unique_ptr<QFormLayout> usmLayout{new QFormLayout()};
    std::unique_ptr<QLineEdit> alphaField{new QLineEdit()};
    std::unique_ptr<QPushButton> usmFilterButton{new QPushButton(QString("USM-Filter"))};
private slots:
    void canny_edge();
    void USM_filter();
};

#endif // EDGE_DETECTION_USM_H
