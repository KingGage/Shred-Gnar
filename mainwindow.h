/******************************************************************************
 * Project File:    MainWindow.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      Main controller/view for the application, stores pointers to all uis
 *      and models.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFontDatabase>

#include "ui_mainwindow.h"
#include "appmodel.h"
#include "startscreen.h"
#include "tutorialscreen.h"
#include "terminologyscreen.h"
#include "skishopscreen.h"
#include "gamescreen.h"
#include "pointsearnedscreen.h"
#include "endscreen.h"
#include "weathertutorialscreen.h"
#include "gamemodel.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(AppModel *model, GameModel *gameModel, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    AppModel *model;
    StartScreen *startScreen;
    TutorialScreen *tutorialScreen;
    TerminologyScreen *terminologyScreen;
    SkiShop *skiShopScreen;
    WeatherTutorialScreen *weatherScreen;
    GameScreen *gameScreen;
    PointsEarnedScreen *pointsEarnedScreen;
    EndScreen *endScreen;

signals:
    ///
    /// \brief startDialogue - start the dialogue scrolling timers when they are
    ///     visible.
    ///
    void startDialogue();

    ///
    /// \brief showWeatherIcons - show the weather in the ski shop after the
    ///     weather conditions explanation.
    ///
    void showWeatherIcons();

    ///
    /// \brief switchSkiShopDialogue - update the dialogue in the ski shop.
    ///
    void switchSkiShopDialogue();

private slots:
    void onDisplayScreen(const QString& screenName);
};

#endif // MAINWINDOW_H
