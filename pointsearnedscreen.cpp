/******************************************************************************
 * Project File:    PointsEarnedScreen.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      PointsEarnedScreen implementation.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#include "pointsearnedscreen.h"


PointsEarnedScreen::PointsEarnedScreen(AppModel *model, GameModel *gameModel, QWidget *parent)
    : QWidget(parent), ui(new Ui::PointsEarnedScreen){
    ui->setupUi(this);
    connect(ui->endGameButton, &QPushButton::clicked,
            this, &PointsEarnedScreen::headHomeClicked);

    connect(ui->toSkiShopButton, &QPushButton::clicked,
            model, &AppModel::onContinueClicked);

    connect(gameModel, &GameModel::pointsOnCrash,
            this, &PointsEarnedScreen::displayPastGamePoints);

    connect(gameModel, &GameModel::gameOverSignal,
            this, &PointsEarnedScreen::displayPastGamePoints);

    connect(model, &AppModel::displayPlayerPoints,
            this, &PointsEarnedScreen::displayTotalGamePoints);
}


PointsEarnedScreen::~PointsEarnedScreen(){
    delete ui;
}


void PointsEarnedScreen::headHomeClicked(){
    HeadHomeDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted){
        emit endGameClicked();
    }
}


void PointsEarnedScreen::displayPastGamePoints(int points){
    ui->pastPoints->setText("+ " + QString::number(points));
}


void PointsEarnedScreen::displayTotalGamePoints(int points){
    ui ->totalPoints->setText("TOTAL : " + QString::number(points));
}
