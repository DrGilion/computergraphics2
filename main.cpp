#include <QApplication>

#include "controller.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    Controller* control = Controller::instance();
    control->start();
    return app.exec();
}
