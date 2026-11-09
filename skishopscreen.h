/******************************************************************************
 * Project File:    SkiShopScreen.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The ui screen for the ski shop in the application.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef SKISHOPSCREEN_H
#define SKISHOPSCREEN_H

#include <QWidget>
#include <QFile>
#include <QFont>
#include <QTimer>
#include <QDebug>

#include "ui_skishopscreen.h"
#include "appmodel.h"


namespace Ui { class SkiShop; }

class SkiShop : public QWidget{
    Q_OBJECT

public:
    explicit SkiShop(AppModel *model, QWidget *parent = nullptr);
    ~SkiShop();

private:
    Ui::SkiShop *ui;
    AppModel *model = nullptr;

    ///
    /// \brief defaultSki
    ///
    Ski* defaultSki;

    ///
    /// \brief updateText update text in dialogue
    ///
    void updateText();

    ///
    /// \brief isSkiShopTutorial
    ///
    bool isSkiShopTutorial = true;

    ///
    /// \brief isWeatherTutorial if the weather tutorial has happened
    ///
    bool isWeatherTutorial = false;

    ///
    /// \brief isWelcomeBack is standard welcome back screen
    ///
    bool isWelcomeBack = false;

    ///
    /// \brief textIndex index in dialogue
    ///
    int textIndex = 0;

    ///
    /// \brief dialogueIndex which dialogue we have
    ///
    int dialogueIndex;

    ///
    /// \brief typingTimer timer to track scrolling text
    ///
    QTimer *typingTimer;

    ///
    /// \brief currentFullText full dialogue for given screen
    ///
    QString currentFullText;

    ///
    /// \brief dialoguesPaths paths to dialogue resources
    ///
    QVector<QString> dialoguesPaths;

    ///
    /// \brief dialogues
    ///
    QVector<QString> dialogues;

    ///
    /// \brief charIndex index in text line
    ///
    int charIndex;

    ///
    /// \brief typingSpeed scrolling speed
    ///
    int typingSpeed;

    ///
    /// \brief currentSkiDescription description of current ski selected
    ///
    QString currentSkiDescription = nullptr;

signals:
    ///
    /// \brief skiSelected emits when ski button is clicked
    /// \param skiType which ski was selected
    ///
    void skiSelected(const AppModel::SkiType& skiType);

    ///
    /// \brief shredGnarClicked when button is clicked to progress the game
    ///
    void shredGnarClicked();

public slots:
    ///
    /// \brief selectedSkiChanged updates all fields in ui when ski is changed
    /// \param ski reference to current ski with all it's data
    ///
    void selectedSkiChanged(Ski* ski);

    ///
    /// \brief onSkiUnlocked when a new ski is unlocked
    /// \param skiType which ski is unlocked
    ///
    void onSkiUnlocked(const QString& skiType);

    ///
    /// \brief onSkiLocked when a ski is set back to locked
    /// \param skiType which ski is locked
    ///
    void onSkiLocked(const QString& skiType);

    ///
    /// \brief readDialogue read the current dialogue screen
    ///
    void readDialogue();

    ///
    /// \brief onNotEnoughPoints when a player tries to unlock a ski it cannot afford
    ///
    void onNotEnoughPoints();

    ///
    /// \brief onShowWeatherIcons when the weather icon needs to be showed
    ///
    void onShowWeatherIcons();

    ///
    /// \brief onWeatherUpdate when the current weather changes
    /// \param forecastedWeather which weather it's changing to
    ///
    void onWeatherUpdate(const AppModel::Weather& forecastedWeather);

    ///
    /// \brief updatePlayerPoints update points after a ski is purchased
    /// \param points amount of points it decreases by
    ///
    void updatePlayerPoints(int points);

    ///
    /// \brief handelChangeStates
    ///
    void handelChangeStates();

private slots:
    ///
    /// \brief showNext
    ///
    void showNext();

    ///
    /// \brief typeNextCharacter next character typed in text dialogue
    ///
    void typeNextCharacter();
};

#endif // SKISHOPSCREEN_H
