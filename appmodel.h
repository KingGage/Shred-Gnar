/******************************************************************************
 * Project File:    AppModel.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The AppModel handles the state of the ShredGnar app, the users progress,
 *      and spinning up the actual game.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QRandomGenerator>

#include "ski.h"


class AppModel : public QObject{
    Q_OBJECT

public:
    enum GameState{
        TUTORIAL,
        GAME_LOOP,
        Ski_Shop,
        END_GAME
    };

    enum Weather{
        GROOMER,
        POW,
        ICY
    };

    enum SkiType{
        ALLMOUNTAIN,
        CARVING,
        FREERIDE,
        MOGUL,
        PARK,
        POWDER,
        RACE,
        TOURING,
    };

    explicit AppModel(QObject *parent = nullptr);
    void resetApplication();

private:
    GameState gamePhase;
    std::size_t tutorialStep = 0;
    std::size_t gameStep = 0;
    std::vector<QString> tutorialScreens;
    std::vector<QString> gameCycleScreens;
    std::map<SkiType, Ski> skisList;
    Ski* currentSki;
    int playerPoints;

    Weather nextWeather;
    void setupGameTutorial();

    ///
    /// \brief updateSkiPerformance - calculates the performance of a ski based on
    ///     conditions and ski dimensions.
    ///
    void updateSkiPerformance();

signals:
    void currentSelectedSkiChanged(Ski* ski);
    void weatherUpdate(const Weather& forecastedWeather);
    void skiPerformanceUpdate(const Ski::SkiPerformance& perf);
    void displayScreen(QString screenName);
    void skiUnlocked(const QString& skiType);
    void skiLocked(const QString& skiType);
    void notEnoughPoints();
    void displayPlayerPoints(int playerPoints);


public slots:
    ///
    /// \brief onContinueClicked - display the next screen in the current game phase.
    ///
    void onContinueClicked();

    ///
    /// \brief onSkiSelect - handle user selcting a new ski from the shop
    /// \param skiType - type of ski selected in the ui
    ///
    void onSkiSelect(const SkiType& skiType);

    ///
    /// \brief onPurchaseCurrentSki - handle unlocking the ski if locked and the user
    ///     has enough points.
    ///
    void onPurchaseCurrentSki();

    ///
    /// \brief startGame - start the
    ///
    void startGame();

    ///
    /// \brief onGameEnd - handle the game ending
    /// \param points - points earned from gameplay
    ///
    void onGameEnd(int points);

    ///
    /// \brief forecastWeather - determine what the next weather condition is randomly.
    ///
    void forecastWeather();

    ///
    /// \brief getDefaultSki
    ///
    Ski* getDefaultSki();
};

#endif // APPMODEL_H
