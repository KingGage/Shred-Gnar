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
 *      The points earned screen that shows up after gameplay.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef POINTSEARNEDSCREEN_H
#define POINTSEARNEDSCREEN_H

#include <QWidget>

#include "appmodel.h"
#include "gamemodel.h"
#include "ui_pointsearnedscreen.h"
#include "headhomedialog.h"


namespace Ui { class PointsEarnedScreen; }

class PointsEarnedScreen : public QWidget{
    Q_OBJECT

public:
    explicit PointsEarnedScreen(AppModel *model, GameModel *gameModel, QWidget *parent = nullptr);
    ~PointsEarnedScreen();

private:
    Ui::PointsEarnedScreen *ui;
    AppModel *model = nullptr;

signals:
    ///
    /// \brief endGameClicked - move to headhomedialogue screen to quit the app.
    ///
    void endGameClicked();

public slots:
    ///
    /// \brief headHomeClicked - Emits endGameClicked to progress game flow.
    ///
    void headHomeClicked();
    ///
    /// \brief displayPastGamePoints - displays the points earned on the most recent game.
    /// \param points
    ///
    void displayPastGamePoints(int points);
    ///
    /// \brief displayTotalGamePoints - displays the total game points of the player.
    /// \param points
    ///
    void displayTotalGamePoints(int points);
};

#endif // POINTSEARNEDSCREEN_H
