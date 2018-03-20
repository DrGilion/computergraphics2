#ifndef INFOTAB_H
#define INFOTAB_H

#include <QWidget>
#include <QLabel>
#include <QFormLayout>
#include <memory>

class InfoTab : public QWidget{
    Q_OBJECT
public:
    InfoTab();
    std::unique_ptr<QFormLayout> layout{new QFormLayout(this)};

    std::unique_ptr<QLabel> fConstGrayLabel{new QLabel()};
    std::unique_ptr<QLabel> fConstRedLabel{new QLabel()};
    std::unique_ptr<QLabel> fConstGreenLabel{new QLabel()};
    std::unique_ptr<QLabel> fConstBlueLabel{new QLabel()};

    std::unique_ptr<QLabel> varianzGrayLabel{new QLabel()};
    std::unique_ptr<QLabel> varianzRedLabel{new QLabel()};
    std::unique_ptr<QLabel> varianzGreenLabel{new QLabel()};
    std::unique_ptr<QLabel> varianzBlueLabel{new QLabel()};
public slots:
    void updateTab();
};

#endif // INFOTAB_H
