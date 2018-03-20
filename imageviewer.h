/****************************************************************************
 **
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QPrinter>
#include <QPrintDialog>
#include <QMenuBar>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTextEdit>
#include <QSlider>
#include <QPushButton>
#include <QTabWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QMenu>

#include "histogrammtab.h"
#include "infotab.h"
#include "functionbox.h"
#include "fleximage.h"
#include "controller.h"
#include "fstream"


class ImageViewer: public QMainWindow {
Q_OBJECT

signals:
    void imageChanged();
    // hier können weitere GUI Objekte hin wie Buttons Slider etc.
private slots:
    // hier können weitere als SLOTS definierte Funktionen hin, die auf Knopfdruck etc. aufgerufen werden.
    void open();
    void save();
    void restoreOriginal();
    void restorePreviousImage();
    void print();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();

public:
    ImageViewer();
    bool loadFile(const QString &);
    void updateImageDisplay();
    FlexImage* image;
    std::fstream logFile;
    void renewLogging();

protected:

    void resizeEvent(QResizeEvent * event);

private:
    void generateControlPanels();

    // Ab hier technische Details die nicht für das Verständnis notwendig sind.
    void startLogging();
    void generateMainGui();
    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QWidget* editor;
    QScrollArea* editorScrollArea;
    QVBoxLayout* editorLayout;
    QTabWidget* infoArea;
    QTextEdit* logBrowser;
    HistogrammTab* histogrammTab;
    QScrollArea* histScroll;
    InfoTab* infoTab;
    QWidget* centralwidget;
    QLabel* imageLabel;
    QScrollArea* scrollArea;
    double scaleFactor;
    QImage* saveImage;


#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif

    QAction *openAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    QAction* restoreAct;
    QAction* restorePreviousAct;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
};
#endif
