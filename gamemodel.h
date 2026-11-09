/******************************************************************************
 * Project File:    GameModel.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The end screen view which prompts the user if they want to quit the
 *      application or start a new game.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QTimer>
#include <QPointF>
#include <QRandomGenerator>
#include <QRectF>

#include "gameobject.h"
#include "ski.h"


class GameModel : public QObject {

    Q_OBJECT

public:

    enum TutorialPhase {
        FIRST_GAME,      // Hide all UI, fast zones, lots of obstacles
        SECOND_GAME,     // Show score and zone, fast zones, lots of obstacles
        POST_TUTORIAL    // Show all UI, normal gameplay
    };

    explicit GameModel(QObject *parent = nullptr);
    ~GameModel();

    QPointF getSkierPosition() const { return skierPosition; }
    int getTurnAngle() const { return currentTurnAngle; }
    int getScore() const { return score; }
    bool isGameOver() const { return gameOver; }
    bool isCrashed() const { return crashed; }

    const QVector<GameObject*>& getObjects() const { return objects; }

    void turnLeft();
    void turnRight();
    void goStraight();
    void startGame();
    void pauseGame();
    void resetGame();

    void setTutorialPhase(TutorialPhase phase);
    TutorialPhase getTutorialPhase() const { return tutorialPhase; }

signals:
    void skierMoved(QPointF newPosition);
    void turnAngleChanged(int angle);
    void scoreChanged(int newScore);
    void gameOverSignal(int finalScore);
    void backgroundScrolled(int offset);

    ///
    /// \brief newZone - signals when the player enters a new zone.
    /// \param zone - zone entered.
    ///
    void newZone(int zone);

    void objectSpawned(GameObject* object);
    void objectRemoved(GameObject* object);

    void skierCrashed();

    void skierJumped();
    void skierLanded();

    void pointsOnCrash(int points);
    void pointsEarned(int points);

private slots:
    void updateGame();

public slots:
    ///
    /// \brief onSkiPerformanceUpdate - adjust the ski performance factors.
    /// \param perf - performance of the ski.
    ///
    void onSkiPerformanceUpdate(const Ski::SkiPerformance& perf);

private:
    QPointF skierPosition{337.5, 150};
    int currentTurnAngle = 0;
    int currentSpeed = -1;
    int score = 0;
    bool gameOver = false;
    bool crashed = false;
    int backgroundOffset = 0;
    int totalScore = 0;
    int distanceScore = 0;
    int distance=0;
    int trickScore = 0;
    int zoneScore = 0;
    int currentZone = 0;

    QTimer *gameTimer;

    ///
    /// \brief objects - objects in the game.
    ///
    QVector<GameObject*> objects;

    int frameCounter = 0;

    ///
    /// \brief crashTimer - how long the game will wait after a crash.
    ///
    int crashTimer = 0;
    static const int CRASH_DURATION = 60;

    void checkBounds();
    void updateScore();
    void updateZone();

    void spawnObstacles();
    void updateObstacles();
    void checkCollisions();
    bool checkCollision(const GameObject* object);
    void handleCrash();
    int getSpawnInterval() const;

    Ski::SkiPerformance currentSkiPerformance;
    TutorialPhase tutorialPhase = POST_TUTORIAL;

    bool isJumping = false;
    int jumpTimer = 0;
    static const int JUMP_DURATION = 40;
    int jumpStartY = 150;

    void handleJump(const GameObject* jumpObject);
};

#endif // GAMEMODEL_H
