#ifndef HISTOGRAMM_H
#define HISTOGRAMM_H

#include <array>
#include <QImage>
#include <QLabel>

class Histogramm : public QLabel{
public:
    Histogramm(bool pcumul = false);
    Histogramm(QRgb pcolor, bool pcumul = false);
    Histogramm(std::array<int,256> value, QRgb pcolor, bool pcumul = false);
    void drawPlot();
    void drawBlack();
    void refresh();
    void resetWith(std::array<int,256> value);
private:
    QImage* plot;
    QRgb color = qRgb(255,255,255);
    bool cumul;
    std::array<int,256> colorscale{{}};
    int maxValue;
    void init();
};

#endif // HISTOGRAMM_H
