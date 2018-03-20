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

#include <QtGui>
#include <QResizeEvent>
#include <iostream>

#include "imageviewer.h"
#include "histogramm.h"
#include "edge_detection_USM.h"
#include "filterlinear.h"
#include "geradendetektor.h"
#include "histogrammausgleich.h"
#include "pictureinfo.h"
#include "redcross.h"
#include "filtertest.h"
#include "miscoperations.h"

using namespace std;


ImageViewer::ImageViewer(){
    image = 0;

    startLogging();

    generateMainGui();
    renewLogging();

    createActions();
    createMenus();

    resize(1600, 600);
}

/****************************************************************************************
 *
 *  mit dieser Methode können sie sich pro Aufgabe ein  Tab anlegen, in der die Ein-
 *  stellungen per Slider, Button, Checkbox etc. gemacht werden und die zu implemen-
 *  tierenden Algorithmen gestatet werden.
 *
 *****************************************************************************************/

void ImageViewer::generateControlPanels() {
    struct Clearer{
        static void remove(QLayout* layout){
            QLayoutItem* child;
            while(layout->count()!=0){
                auto child = layout->takeAt(0);
                if(child->layout() != 0){
                    remove(child->layout());
                }else if(child->widget() != 0){
                    delete child->widget();
                }
                delete child;
            }
        }
    };
    if(editorLayout->count()>0){
        //editorLayout->clear();
        Clearer::remove(editorLayout);


    }
//zeroth tab: Basic Operations
    editorLayout->addWidget(new MiscOperations());

// first tab: Kreuz malen
    editorLayout->addWidget(new RedCross());

// second tab: Helligkeit & Kontrast
    editorLayout->addWidget(new PictureInfo());

//third tab: Histogrammausgleich
    editorLayout->addWidget(new HistogrammAusgleich());

//fourth tab: Linearer Filter
    editorLayout->addWidget(new FilterLinear());

//fifth tab: Kantendetektion
    editorLayout->addWidget(new EdgeDetectionUSM());

//sixth tab: USM-Filter
    editorLayout->addWidget(new GeradenDetektor());

//Filter Tab: Verschiedene Filter testen
    editorLayout->addWidget(new FilterTest());
}

/****************************************************************************************
 *
 *   ab hier kommen technische Details, die nicht notwenig für das Verständnis und die
 *   Bearbeitung sind.
 *
 *
 *****************************************************************************************/

void ImageViewer::startLogging() {
//LogFile
    logFile.open("log.txt", std::ios::out);
    logFile << "Logging: \n" << std::endl;
}

void ImageViewer::renewLogging() {
    QFile file("log.txt"); // Create a file handle for the file named
    QString line;
    file.open(QIODevice::ReadOnly); // Open the file

    QTextStream stream(&file); // Set the stream to read from myFile
    logBrowser->clear();
    while (!stream.atEnd()) {

        line = stream.readLine(); // this reads a line (QString) from the file
        logBrowser->append(line);
    }
}

void ImageViewer::updateImageDisplay() {
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    emit imageChanged();
}

void ImageViewer::generateMainGui() {
    /* Editor widget */
    editor = new QWidget;
    editorLayout = new QVBoxLayout(editor);
    editorLayout->setSizeConstraint(QLayout::SetMinimumSize);
    editorLayout->setObjectName("editorLayout");
    editorScrollArea = new QScrollArea;
    editorScrollArea->setWidget(editor);
    editorScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    editorScrollArea->setWidgetResizable(true);

    /* Center widget */
    centralwidget = new QWidget(this);
    centralwidget->setObjectName("centralwidget");

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    /* Center widget */
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);

    setCentralWidget(scrollArea);

    /* HBox layout */
    QGridLayout* gLayout = new QGridLayout(centralwidget);
    gLayout->setObjectName("hboxLayout");
    gLayout->addWidget(new QLabel(), 1, 1);
    gLayout->setVerticalSpacing(50);
    gLayout->addWidget(editorScrollArea, 2, 1);
    gLayout->addWidget(scrollArea, 2, 2);

    int minHeight = 100;
    int maxHeight = 200;
    infoArea = new QTabWidget(this);

    logBrowser = new QTextEdit();
    logBrowser->setMinimumHeight(minHeight);
    //logBrowser->setMaximumHeight(maxHeight);
    logBrowser->setMinimumWidth(width());
    logBrowser->setMaximumWidth(width());
    infoArea->addTab(logBrowser,QString("Log-Browser"));

    histogrammTab = new HistogrammTab();
    histogrammTab->setMinimumHeight(minHeight);
    histogrammTab->setMinimumWidth(width());
    histScroll = new QScrollArea;
    histScroll->setWidget(histogrammTab);
    histScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    histScroll->setWidgetResizable(false);
    QObject::connect(this,ImageViewer::imageChanged,histogrammTab,HistogrammTab::updateHistogramms);
    infoArea->addTab(histScroll,QString("Histogramme"));

    infoTab = new InfoTab();
    infoTab->setMinimumHeight(minHeight);
    infoTab->setMaximumHeight(maxHeight);
    infoTab->setMinimumWidth(width());
    infoTab->setMaximumWidth(width());
    QObject::connect(this,ImageViewer::imageChanged,infoTab,InfoTab::updateTab);
    infoArea->addTab(infoTab,QString("Weitere informationen"));

    gLayout->addWidget(infoArea, 3, 1, 1, 2);
    gLayout->setVerticalSpacing(50);
}

void ImageViewer::print() {
    Q_ASSERT(imageLabel->pixmap());
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *imageLabel->pixmap());
    }
#endif
}

void ImageViewer::open() {
    /*if (image != NULL) {
        delete image;
        image = NULL;
    }*/

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
            QDir::currentPath());
    if (!fileName.isEmpty()) {
        image = new FlexImage(fileName);
        Controller::instance()->clearImageStorage();
        Controller::instance()->pushImage(image);
        if (image->isNull()) {
            QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(fileName));
            return;
        }

        generateControlPanels();
        scaleFactor = 1.0;


        updateImageDisplay();

        printAct->setEnabled(true);
        fitToWindowAct->setEnabled(true);
        updateActions();

        if (!fitToWindowAct->isChecked())
            imageLabel->adjustSize();

        setWindowFilePath(fileName);
        logFile << "geladen: " << fileName.toStdString().c_str() << std::endl;

        renewLogging();
    }
}

void ImageViewer::save(){
    QString filename = QFileDialog::getSaveFileName(this,tr("save File"),QDir::currentPath(),QString("png"));
    image->save(filename);
}

void ImageViewer::resizeEvent(QResizeEvent * event) {
    QMainWindow::resizeEvent(event);
    centralwidget->setMinimumWidth(width());
    centralwidget->setMinimumHeight(height());
    centralwidget->setMaximumWidth(width());
    centralwidget->setMaximumHeight(height());
    logBrowser->setMinimumWidth(width());
    logBrowser->setMaximumWidth(width());
}

void ImageViewer::zoomIn() {
    scaleImage(1.25);
}

void ImageViewer::zoomOut() {
    scaleImage(0.8);
}

//selbstgemacht
void ImageViewer::restoreOriginal(){
    Controller::instance()->restoreOriginalImage();
}

//selbstgemacht
void ImageViewer::restorePreviousImage(){
    Controller::instance()->restorePreviousImage();
}

void ImageViewer::normalSize() {
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::fitToWindow() {
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    editorScrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}

void ImageViewer::about() {
    QMessageBox::about(this, tr("About Image Viewer"),
            tr(
                    "<p>The <b>Image Viewer</b> example shows how to combine QLabel "
                            "and QScrollArea to display an image. QLabel is typically used "
                            "for displaying a text, but it can also display an image. "
                            "QScrollArea provides a scrolling view around another widget. "
                            "If the child widget exceeds the size of the frame, QScrollArea "
                            "automatically provides scroll bars. </p><p>The example "
                            "demonstrates how QLabel's ability to scale its contents "
                            "(QLabel::scaledContents), and QScrollArea's ability to "
                            "automatically resize its contents "
                            "(QScrollArea::widgetResizable), can be used to implement "
                            "zooming and scaling features. </p><p>In addition the example "
                            "shows how to use QPainter to print an image.</p>"));
}

void ImageViewer::createActions() {
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save..."),this);
    connect(saveAct,SIGNAL(triggered()),this,SLOT(save()));
    saveAct->setEnabled(true);

    //selbstgemacht
    restoreAct = new QAction(tr("&Restore to Original"), this);
    restoreAct->setEnabled(true);
    connect(restoreAct ,SIGNAL(triggered()), this, SLOT(restoreOriginal()));

    //selbstgemacht
    restorePreviousAct = new QAction(tr("&Restore to Previous"), this);
    restorePreviousAct->setShortcut(tr("Ctrl+Z"));
    restorePreviousAct->setEnabled(true);
    connect(restorePreviousAct ,SIGNAL(triggered()), this, SLOT(restorePreviousImage()));

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcut(tr("Ctrl+P"));
    printAct->setEnabled(false);
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void ImageViewer::createMenus() {
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(restoreAct);
    viewMenu->addAction(restorePreviousAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(helpMenu);
}

void ImageViewer::updateActions() {
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor) {
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor) {
    scrollBar->setValue(
            int(
                    factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep() / 2)));
}
