/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
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
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"

#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QSpacerItem>
#include <QQuaternion>
QQuaternion q1,q2,qd;

//! [0]
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//! [0]
    ui->setupUi(this);
    console = new Console;
    console->setEnabled(false);

    ui->bottomWidget->setLayout(new QVBoxLayout);
    ui->bottomWidget->layout()->addWidget(console);
    ui->bottomWidget->setDisabled(true);
//! [1]
    serial = new QSerialPort(this);
//! [1]
    settings = new SettingsDialog;

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    status = new QLabel;
    QLabel *link = new QLabel;
    ui->statusBar->addWidget(status);
    ui->statusBar->addWidget(link);
    link->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    link->setOpenExternalLinks(true);
    link->setText("<a href='https://github.com/m-fi/Myo-Arduino'>MyoArduino</a>");

    //remove resizability and set window title
    ui->statusBar->setSizeGripEnabled( 0 );
    this->setFixedSize(maximumWidth(), maximumHeight());
    this->setWindowTitle("MyoArduino GUI");

    initActionsConnections();

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);

//! [2]
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
//! [2]
    //connect(console, &Console::getData, this, &MainWindow::writeData);

//! [3]
}
//! [3]

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

//! [4]
void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        console->setEnabled(true);
        console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}
//! [4]

//! [5]
void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}
//! [5]

void MainWindow::about()
{
    QMessageBox _about(this);
    _about.setWindowTitle("About MyoArduino GUI");
    _about.setTextFormat(Qt::RichText);//needed for links to work
    _about.setText(tr("The MyoArduino GUI is part of the MyoArduino project, "
                      "which can be found @ <a href='https://github.com/m-fi/Myo-Arduino'>"
                      "https://github.com/m-fi/Myo-Arduino</a>."
                      ));
    _about.exec();
}

//! [6]
void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
//! [6]

//! [7]
void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    console->putData(data);

    getQuat();
}

//! [7]
//This function parses the text received through comm port communication
//The Console textbox acts as a buffer; from which we chop off older data and parse the newer data
//This function is specifically built to find and store the quaternion data sent via comm port
//from an Arduino running MyoArduino
//Could be rewritten to find other data from the buffer
void MainWindow::getQuat()
{
    QString str;
    //get the text from the comm port console
    str = console->toPlainText();
    //check how long the string is
    //we don't want to parse more than one line
    //51 is the largest number of chars we could have based on:
    //q2 - x: 140.88 ,y: -170.44 ,z: -133.15 ,w: -143.14
    //but not all strings will have coords in the hundreds e.g.:
    //qd - x: 0.01 ,y: -0.05 ,z: -0.02 ,w: 1.00
    if(str.length() > 51)
    {
        //remove all chars except the last 200 (don't want to parse a really long string)
        str.remove(0, str.length()-200);
    }

    //make sure that the string only has the data we're expecting from a quat
    QRegExp rxStr("(\\q[12d].{6}\\-?\\d{1,3}\\.\\d{2}.{5}\\-?\\d{1,3}\\.\\d{2}.{5}\\-?\\d{1,3}\\.\\d{2}.{5}\\-?\\d{1,3}\\.\\d{2})");
    QStringList strFilter;
    int pos1 = 0;
    while ((pos1 = rxStr.indexIn(str, pos1)) != -1) {
        strFilter << rxStr.cap(1);
        pos1 += rxStr.matchedLength();
    }

    //parse the string for numbers matching the regexp format set
    QRegExp rxQuat("(\\-?\\d{1,3}\\.\\d{2})");
    QStringList list;
    int pos2 = 0;
    for(int i = 0; i<strFilter.size(); i++)
    {
        pos2 = 0;
        while ((pos2 = rxQuat.indexIn(strFilter[i], pos2)) != -1) {
            list << rxQuat.cap(1);
            pos2 += rxQuat.matchedLength();
        }
    }

    //sort the data collected and store it in a quaternion object
    //each member of the strFilter array has been parsed for 4 float numbers
    //these floats are currently stored as a string list e.g.
    //stringlist[0]-[3] is x,y,z,w
    //stringlist[4]-[7] is also stored as x,y,z,w
    float x,y,z,w;
    int line=0;
    for(int count=0;count<strFilter.size()*4;count++)
    {
        x=((QString)list[count]).toFloat();
        y=((QString)list[count+1]).toFloat();
        z=((QString)list[count+2]).toFloat();
        w=((QString)list[count+3]).toFloat();

        //check which quat we received out of three expected
        if(strFilter[line].contains("q1", Qt::CaseInsensitive))
            {
            q1.setVector(x,y,z);
            q1.setScalar(w);
        } else if(strFilter[line].contains("q2", Qt::CaseInsensitive))
            {
            q2.setVector(x,y,z);
            q2.setScalar(w);
        } else if(strFilter[line].contains("qd", Qt::CaseInsensitive))
            {
            qd.setVector(x,y,z);
            qd.setScalar(w);
        }
        line++;
        count+=3;
    }
}

//! [8]
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
//! [8]

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionConfigure, &QAction::triggered, settings, &MainWindow::show);
    connect(ui->actionClear, &QAction::triggered, console, &Console::clear);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}
