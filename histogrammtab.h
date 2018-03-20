#ifndef HISTOGRAMMTAB_H
#define HISTOGRAMMTAB_H

#include <memory>
#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "histogramm.h"

class HistogrammTab : public QWidget{
    Q_OBJECT
private:
    std::unique_ptr<QVBoxLayout> layout{new QVBoxLayout(this)};

    std::unique_ptr<QHBoxLayout> histLayout{new QHBoxLayout()};
    std::unique_ptr<Histogramm> blackHist{new Histogramm()};
    std::unique_ptr<Histogramm> rHist{new Histogramm(qRgb(255,0,0))};
    std::unique_ptr<Histogramm> gHist{new Histogramm(qRgb(0,255,0))};
    std::unique_ptr<Histogramm> bHist{new Histogramm(qRgb(0,0,255))};

    std::unique_ptr<QHBoxLayout> cumulHistLayout{new QHBoxLayout()};
    std::unique_ptr<Histogramm> blackHistCumul{new Histogramm(true)};
    std::unique_ptr<Histogramm> rHistCumul{new Histogramm(qRgb(255,0,0),true)};
    std::unique_ptr<Histogramm> gHistCumul{new Histogramm(qRgb(0,255,0),true)};
    std::unique_ptr<Histogramm> bHistCumul{new Histogramm(qRgb(0,0,255),true)};

public:
    HistogrammTab();

public slots:
    void updateHistogramms();
};

#endif // HISTOGRAMMTAB_H
