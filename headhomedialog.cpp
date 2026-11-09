/******************************************************************************
 * Project File:    HeadHomeDialog.cpp
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      Head home dialogue implementation
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#include "headhomedialog.h"


HeadHomeDialog::HeadHomeDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::HeadHomeDialog){
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setModal(true);

    setStyleSheet(R"(
    QLabel#exitSpeechText {
        color: #f7f0de;
        font-family: "Press Start 2P";
        font-size: 18pt;          /* bigger */
        font-weight: 700;         /* heavier */
        letter-spacing: 1px;      /* a bit more presence */
        padding: 8px 12px;
        qproperty-wordWrap: true;
        qproperty-alignment: AlignHCenter;
    }

        QPushButton#quitButton {
            color: #ffffff;
            font-family: "Segoe UI", "SF Pro Text", "Arial", sans-serif;
            font-size: 12pt;
            font-weight: 600;
            letter-spacing: 1px;
            padding: 6px 28px;
            min-height: 32px;
            background-color: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #3b65b3,
                stop:1 #2b4f8a
            );
            border: 2px solid #ffffff;
            border-radius: 8px;
            border-top-color: #dfe9ff;
            border-left-color: #dfe9ff;
            border-right-color: #1b315c;
            border-bottom-color: #1b315c;
            outline: none;
        }

        QPushButton#quitButton:hover {
            background-color: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #4a79cf,
                stop:1 #355fa4
            );
        }

        QPushButton#quitButton:pressed {
            background-color: #223a66;
            padding-top: 8px;
            padding-bottom: 4px;
        }
    )");

    ui->warningLabel->setText(
        "Are you sure you want to head home?\n"
        "All progress will be lost."
        );
    ui->warningLabel->setObjectName("exitSpeechText");
    ui->stayButton->setText("STAY ON MOUNTAIN");
    ui->quitButton->setText("HEAD HOME");
    ui->stayButton->setObjectName("quitButton");
    ui->quitButton->setObjectName("quitButton");

    connect(ui->stayButton, &QPushButton::clicked, this, [this](){
        reject();
    });
    connect(ui->quitButton, &QPushButton::clicked, this, [this](){
        accept();
    });
}

HeadHomeDialog::~HeadHomeDialog(){
    delete ui;
}
