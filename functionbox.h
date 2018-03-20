#ifndef FUNCTIONBOX_H
#define FUNCTIONBOX_H

#include <QGroupBox>

#include "controller.h"
class Controller;

//ACHTUNG: Falls undefined reference to vtable = rechts-klick auf Projekt ->run qmake
class FunctionBox : public QGroupBox{
public:
    FunctionBox(const char* title = "Ihr Algorithmus hier" );

};

#endif // FUNCTIONBOX_H
