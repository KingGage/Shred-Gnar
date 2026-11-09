/******************************************************************************
 * Project File:    GameScreen.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Ethan Laynor
 * Last Updated:    December 12, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The game view that handles all the game view logic such as key inputs
 *      and objects scrolling
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QMap>
#include <QTimer>
#include <QPropertyAnimation>
#include <QLabel>
#include <QMovie>
#include <QGraphicsColorizeEffect>


#include "appmodel.h"
#include "gamemodel.h"
#include "gameobject.h"


namespace Ui {
class GameScreen;
}

class GameScreen : public QWidget
{
    Q_OBJECT

public:
    explicit GameScreen(AppModel *model, GameModel *gameModel, QWidget *parent = nullptr);
    ~GameScreen();

    ///
    /// \brief resetAndStartGame - resets the game state and starts a new game
    ///
    void resetAndStartGame();

protected:
    ///
    /// \brief keyPressEvent - handles keyboard input for skier control
    /// \param event - the key event containing the pressed key
    ///
    void keyPressEvent(QKeyEvent *event) override;

signals:
    ///
    /// \brief gameOver - emitted when the game ends
    /// \param score - the final score achieved by the player
    ///
    void gameOver(int score);

private slots:
    ///
    /// \brief onSkierMoved - updates the skier's visual position
    /// \param pos - the new position of the skier
    ///
    void onSkierMoved(QPointF pos);

    ///
    /// \brief onTurnAngleChanged - updates the skier sprite based on turn angle
    /// \param angle - the current turn angle
    ///
    void onTurnAngleChanged(int angle);

    ///
    /// \brief onBackgroundScrolled - scrolls the background to create movement
    /// \param offset - the scroll offset amount
    ///
    void onBackgroundScrolled(int offset);

    ///
    /// \brief onScoreChanged - updates the score display
    /// \param score - the current score
    ///
    void onScoreChanged(int score);

    ///
    /// \brief onGameOver - handles game over logic and weather forecasting
    /// \param finalScore - the final score when the game ends
    ///
    void onGameOver(int finalScore);

    ///
    /// \brief onZoneChange - updates the zone display when entering a new zone
    /// \param zone - the zone number (0=Green, 1=Blue, 2=Black)
    ///
    void onZoneChange(int zone);

    ///
    /// \brief onObjectSpawned - adds a visual representation of a spawned object
    /// \param object - the game object that was spawned
    ///
    void onObjectSpawned(GameObject* object);

    ///
    /// \brief onObjectRemoved - removes the visual representation of an object
    /// \param object - the game object that was removed
    ///
    void onObjectRemoved(GameObject* object);

    ///
    /// \brief onSkierCrashed - handles crash animation and visual effects
    ///
    void onSkierCrashed();

    ///
    /// \brief onSkierJumped - changes skier sprite to jumping animation
    ///
    void onSkierJumped();

    ///
    /// \brief onSkierLanded - changes skier sprite back to normal after landing
    ///
    void onSkierLanded();

private:
    Ui::GameScreen *ui;
    AppModel *appModel = nullptr;
    GameModel *gameModel;

    // Graphics objects
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsPixmapItem *skier;
    QGraphicsPixmapItem *background1;
    QGraphicsPixmapItem *background2;
    QGraphicsPixmapItem *zoneSign;
    QGraphicsPixmapItem *zoneDisplay;
    QGraphicsPixmapItem *zoneBackground;
    QGraphicsPixmapItem *scoreBackground;
    QGraphicsPixmapItem *weatherBackround;

    QGraphicsTextItem *scoreLabel;
    QGraphicsTextItem *instructions;

    QPixmap skierSprites[8];
    QPixmap zoneSignSprites[3];
    QPixmap backgroundSprites[3];  // 0=Groomer, 1=Powder, 2=Icy
    QPixmap weatherIconSprites[3]; // 0=Groomer, 1=Powder, 2=Icy
    QGraphicsPixmapItem *weatherDisplay;

    AppModel::Weather currentWeather;

    QPixmap treeSmallSprite;
    QPixmap treeLargeSprite;

    QPixmap rockSprite;
    QPixmap cliffDropSprite;

    QPixmap powderSprite;
    QPixmap snowPileSprite;

    QPixmap crashTextSprite;

    QGraphicsPixmapItem *crashText;
    QTimer *crashAnimationTimer;
    int crashAnimationFrame;

    QMap<GameObject*, QGraphicsPixmapItem*> objectItems;

    ///
    /// \brief setupView - initializes the graphics scene and loads sprites
    ///
    void setupView();

    ///
    /// \brief updateSkierSprite - updates the skier sprite based on turn angle
    /// \param angle - the current turn angle
    ///
    void updateSkierSprite(int angle);

    ///
    /// \brief onWeatherUpdate - updates the background and weather display
    /// \param weather - the current weather condition
    ///
    void onWeatherUpdate(AppModel::Weather weather);

    ///
    /// \brief animateCrashText - starts the crash text animation
    ///
    void animateCrashText();

    ///
    /// \brief updateCrashAnimation - updates the crash text animation frame
    ///
    void updateCrashAnimation();

    int pendingGameOverScore = -1;
    int pendingCrashScore = -1;

    QLabel *snowAnimationLabel;
    QMovie *snowAnimation;

    QPixmap jumpSprite;
    QPixmap skierJumpSprite;

    QPixmap pointsTextSprite;
    QGraphicsPixmapItem *pointsText;
    QTimer *pointsAnimationTimer;
    int pointsAnimationFrame;

    ///
    /// \brief animatePointsText - starts the points earned animation
    /// \param points - the number of points earned
    ///
    void animatePointsText(int points);

    ///
    /// \brief updatePointsAnimation - updates the points text animation frame
    ///
    void updatePointsAnimation();

    QGraphicsPixmapItem *instructionOverlay;
    bool gameStarted;
};

#endif
