/******************************************************************************
 * Project File:    StartScreen.cpp
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      Start screen implementation.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#include "startscreen.h"
#include "ui_startscreen.h"


StartScreen::StartScreen(AppModel *model, QWidget *parent)
    : QWidget(parent), ui(new Ui::StartScreen){
    ui->setupUi(this);

    connect(ui->startButton, &QPushButton::clicked,
            model, &AppModel::onContinueClicked);
}


StartScreen::~StartScreen(){
    delete ui;
}
