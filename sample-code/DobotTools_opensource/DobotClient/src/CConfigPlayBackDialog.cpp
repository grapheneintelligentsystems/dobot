/*
  CConfigPlayBackDialog.cpp - Playback parameter config dialog
  Copyright (c) 2015-2016 Yuejiang Technology Co., Ltd. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "CConfigPlayBackDialog.h"
#include <QtWidgets>
#include "COptions.h"
#include "DobotClient.h"

CConfigPlayBackDialog::CConfigPlayBackDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("PlayBack params");
    setFixedSize(412, 400);
    initLayout();
}

CConfigPlayBackDialog::~CConfigPlayBackDialog()
{

}

void CConfigPlayBackDialog::initLayout(void)
{
    static const char *strLut[] = {
        "JointVelocity",
        "JointAcceleration",
        "ServoVelocity",
        "ServoAcceleration",
        "LinearVelocity",
        "LinearAcceleration",
        "pauseTime",
        "jumpHeight",
        "VelocityRatio",
        "AccelerationRatio"
    };
    static const char *unitLut[] = {
        "°/s",
        "°/(s·s)",
        "°/s",
        "°/(s·s)",
        "mm/s",
        "mm/(s·s)",
        "s",
        "cm",
        "%",
        "%"
    };
    static const char *toolTipLut[] = {
        "Velocity of single joint",
        "Acceleration of single joint",
        "Velocity of the servo",
        "Acceleration of the servo",
        "Velocity of single axis",
        "Acceleration of single axis",
        "Pause time between two playback points",
        "Jump height when use JUMP motion style",
        "Ratio of the velocity",
        "Ratio of the acceleration"
    };

    QFont font("微软雅黑", 8);

    COptions::PlayBackParams playBackParams;
    COptions::instance()->getPlayBackParams(&playBackParams);
    float *params = (float *)&playBackParams;

    int i = 0;
    for (i = 0; i < 8; i++) {
        QLabel *label = new QLabel(this);
        label->move(10, 10 + i * 40);
        label->resize(150, 30);
        label->setFont(font);
        label->setText(strLut[i]);

        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->move(180, 10 + i * 40);
        lineEdit->resize(100, 30);
        lineEdit->setFont(font);
        lineEdit->setToolTip(toolTipLut[i]);
        lineEdit->setText(QString("%1").arg(params[i]));
        paramWidgetList << lineEdit;

        label = new QLabel(this);
        label->move(300, 10 + i * 40);
        label->resize(50, 20);
        label->setFont(font);
        label->setText(unitLut[i]);
    }

    QPushButton *sendBtn = new QPushButton(this);
    sendBtn->move(280, 10 + i * 40);
    sendBtn->resize(120, 50);
    sendBtn->setFont(font);
    sendBtn->setText("Send");
    connect(sendBtn, SIGNAL(clicked()), this, SLOT(onSendBtnClicked()));
}

void CConfigPlayBackDialog::onSendBtnClicked(void)
{
    COptions::PlayBackParams playBackParams;
    float *params = (float *)&playBackParams;

    int i = 0;
    for (i = 0; i < 8; i++) {
        QLineEdit *lineEdit = (QLineEdit *)paramWidgetList.at(i);
        params[i] = lineEdit->text().toFloat();
    }
    COptions::instance()->setPlayBackParams(&playBackParams);
    DobotClient::instance()->configPlayBackStaticParams(
                playBackParams.jointVelocity,
                playBackParams.jointAcceleration,
                playBackParams.servoVelocity,
                playBackParams.servoAcceleration,
                playBackParams.linearVelocity,
                playBackParams.linearAcceleration,
                playBackParams.pauseTime,
                playBackParams.jumpHeight
                );

    accept();
}
