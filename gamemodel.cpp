/******************************************************************************
 * Project File:    GameModel.cpp
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


#include "gamemodel.h"


GameModel::GameModel(QObject *parent)
    : QObject{parent}{
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout,
            this, &GameModel::updateGame);
    currentSkiPerformance.speedMultiplier = 1.0;
    currentSkiPerformance.turningMultiplier = 1.0;
}

GameModel::~GameModel(){
    // Reseting objects
    qDeleteAll(objects);
    objects.clear();
}


void GameModel::startGame(){
    resetGame();
    gameTimer->start(25);
}

void GameModel::pauseGame(){
    gameTimer->stop();
}

void GameModel::resetGame(){
    skierPosition = QPointF(337.5, 150);
    currentTurnAngle = 0;
    currentSpeed = -1;
    score = 0;
    gameOver = false;
    crashed = false;
    backgroundOffset = 0;
    distanceScore = 0;
    distance = 0;
    trickScore = 0;
    zoneScore = 0;
    currentZone = 0;
    frameCounter = 0;
    crashTimer = 0;

    isJumping = false;
    jumpTimer = 0;
    totalScore = 0;

    qDeleteAll(objects);
    objects.clear();

    currentSkiPerformance.speedMultiplier = 1.0;
    currentSkiPerformance.turningMultiplier = 1.0;

    emit skierMoved(skierPosition);
    emit turnAngleChanged(currentTurnAngle);
    emit scoreChanged(score);
    emit backgroundScrolled(backgroundOffset);
}

void GameModel::turnLeft(){
    if (!crashed && currentTurnAngle > -2) {
        currentTurnAngle--;
        emit turnAngleChanged(currentTurnAngle);
    }
}

void GameModel::turnRight(){
    if (!crashed && currentTurnAngle < 2) {
        currentTurnAngle++;
        emit turnAngleChanged(currentTurnAngle);
    }
}

void GameModel::goStraight(){
    if(!crashed){
        currentTurnAngle = 0;
        emit turnAngleChanged(currentTurnAngle);
    }
}

void GameModel::updateGame(){
    if (gameOver) {
        return;
    }

    if(crashed){
        crashTimer++;
        if (crashTimer >= CRASH_DURATION) {
            gameOver = true;
            gameTimer->stop();
            emit gameOverSignal(totalScore);
        }
        return;
    }

    // Handle jumping animation
    if (isJumping) {
        jumpTimer++;

        // Calculate jump arc (parabola)
        float jumpProgress = static_cast<float>(jumpTimer) / JUMP_DURATION;
        float jumpHeight = sin(jumpProgress * 3.14159f) * 80.0f;

        skierPosition.setY(jumpStartY - jumpHeight);
        emit skierMoved(skierPosition);

        if (jumpTimer >= JUMP_DURATION) {
            // Land
            isJumping = false;
            jumpTimer = 0;
            skierPosition.setY(jumpStartY);
            emit skierMoved(skierPosition);
            emit skierLanded();
        }
    }

    frameCounter++;

    // Update speed based on turn angle (slower when turning sharply) and ski performance
    double baseSpeed = -15 + 5 * std::abs(currentTurnAngle);
    currentSpeed = baseSpeed * currentSkiPerformance.speedMultiplier;

    // Update skier horizontal position based on turn angle and ski performance
    double baseTurning = currentTurnAngle * 5;
    int horizontalMove = baseTurning * currentSkiPerformance.turningMultiplier;


    //Checks to see if the zone needs to be updated
    if (tutorialPhase == FIRST_GAME || tutorialPhase == SECOND_GAME) {
        // Fast zone progression for tutorial
        if (currentZone == 0 && distance > 100) {
            GameModel::updateZone();
        }
        else if (currentZone == 1 && distance > 300) {
            GameModel::updateZone();
        }
    } else {
        // Normal zone progression for post-tutorial
        if (currentZone == 0 && distance > 5000) {
            GameModel::updateZone();
        }
        else if (currentZone == 1 && distance > 25000) {
            GameModel::updateZone();
        }
    }

    // Apply horizontal movement to skier
    skierPosition.setX(skierPosition.x() + horizontalMove);
    emit skierMoved(skierPosition);

    // Check boundaries
    checkBounds();

    //Check gameover status
    if(gameOver){
        return;
    }

    // Update background movement
    backgroundOffset += currentSpeed;
    emit backgroundScrolled(currentSpeed);

    distance -= currentSpeed;
    distanceScore -= currentSpeed*(currentZone+1);
    updateScore();

    spawnObstacles();
    updateObstacles();
    checkCollisions();
}


void GameModel::checkBounds()
{
    // Check if skier went out of bounds
    if (skierPosition.x() < 0 || skierPosition.x() + 75 > 750) {
        // Treat out-of-bounds like a crash - trigger crash animation
        handleCrash();
    }
}

void GameModel::updateScore(){
    totalScore = (distanceScore / 50) + zoneScore + trickScore;
    emit scoreChanged(totalScore);
}

void GameModel::updateZone(){
    currentZone++;
    zoneScore += 500;
    emit newZone(currentZone);
}

int GameModel::getSpawnInterval() const{
    // More frequent spawning in tutorial phases
    if (tutorialPhase == FIRST_GAME || tutorialPhase == SECOND_GAME) {
        switch(currentZone) {
        case 0: return 20;  // Much faster than normal (80)
        case 1: return 10;  // Faster than normal (20)
        case 2: return 3;   // Faster than normal (5)
        default: return 20;
        }
    } else {
        // Normal spawn rates for post-tutorial
        switch(currentZone) {
        case 0: return 80;
        case 1: return 20;
        case 2: return 5;
        default: return 80;
        }
    }
}

void GameModel::spawnObstacles(){
    int interval = getSpawnInterval();

    if (frameCounter % interval == 0) {
        // Random x position (with margins to avoid edges)
        int x = QRandomGenerator::global()->bounded(50, 650);

        // Spawn below the visible area
        int y = 750;

        GameObject::Obstacle type;
        int width, height;

        // Zone-specific obstacle spawning
        switch(currentZone) {
        case 0:  // Green zone - only trees (small and large)
        {
            bool isLarge = QRandomGenerator::global()->bounded(2) == 0;
            type = isLarge ? GameObject::LARGETREE : GameObject::SMALLTREE;
            width = isLarge ? 80 : 36;
            height = isLarge ? 100 : 48;
            break;
        }
        case 1:  // Blue zone - trees, rocks, and JUMPS
        {
            int randNum = QRandomGenerator::global()->bounded(5);
            if (randNum == 0) {
                type = GameObject::SMALLTREE;
                width = 36;
                height = 48;
            } else if (randNum == 1) {
                type = GameObject::LARGETREE;
                width = 80;
                height = 100;
            } else if (randNum == 2) {
                type = GameObject::ROCK;
                width = 50;
                height = 50;
            } else if (randNum == 3) {
                type = GameObject::CLIFF;
                width = 100;
                height = 100;
            } else {
                type = GameObject::JUMP;
                width = 80;
                height = 40;
            }
            break;
        }
        case 2:  // Black zone - trees, rocks, snow piles, and JUMPS
        default:
        {
            int randNum = QRandomGenerator::global()->bounded(7);
            if (randNum == 0) {
                type = GameObject::SMALLTREE;
                width = 36;
                height = 48;
            } else if (randNum == 1) {
                type = GameObject::LARGETREE;
                width = 80;
                height = 100;
            } else if (randNum == 2 ) {
                type = GameObject::ROCK;
                width = 50;
                height = 50;
            } else if (randNum == 3) {
                type = GameObject::CLIFF;
                width = 50;
                height = 50;
            } else if (randNum == 4) {
                type = GameObject::SNOWPILE;
                width = 70;
                height = 40;
            } else if (randNum == 5) {
                type = GameObject::POWDER;
                width = 70;
                height = 40;
            } else {
                type = GameObject::JUMP;
                width = 80;
                height = 40;
            }
            break;
        }
        }

        // Create QRectF with position and dimensions
        QRectF boundingBox(x, y, width, height);

        // Create object with the bounding box
        GameObject* object = new GameObject(type, boundingBox);
        objects.append(object);
        emit objectSpawned(object);
    }
}


void GameModel::updateObstacles(){
    // Move all obstacles upward with the scrolling speed
    for (GameObject* object : objects) {
        QPointF pos = object->getPosition();
        pos.setY(pos.y() + currentSpeed);
        object->setPosition(pos);
    }

    // Remove obstacles that are off screen
    QVector<GameObject*> toRemove;
    for (GameObject* object : objects) {
        if (object->isOffScreen()) {
            toRemove.append(object);
        }
    }

    for (GameObject* object : toRemove) {
        objects.removeOne(object);
        emit objectRemoved(object);
        delete object;
    }
}

void GameModel::checkCollisions(){
    for (const GameObject* object : objects) {
        if (object->getType() == GameObject::JUMP) {
            // Check collision with jump
            if (checkCollision(object) && !isJumping) {
                handleJump(object);
            }
        } else {
            // Regular obstacle - only crash if not jumping
            if (checkCollision(object) && !isJumping) {
                handleCrash();
                return;
            }
        }
    }
}

bool GameModel::checkCollision(const GameObject* object){
    // Create bounding rectangles for skier and object
    QRectF skierRect(skierPosition.x(), skierPosition.y(), 75, 75);
    QRectF objectRect = object->getBoundingRect();

    // Apply "padding" to make collision detection more forgiving towards the use
    const double PADDING = 10.0;
    skierRect = skierRect.adjusted(PADDING, PADDING, -PADDING, -PADDING);
    objectRect = objectRect.adjusted(PADDING, PADDING, -PADDING, -PADDING);

    // Use Qt's built-in intersection test
    return skierRect.intersects(objectRect);
}

void GameModel::handleCrash(){
    if (!crashed) {
        crashed = true;
        currentSpeed = 0;
        crashTimer = 0;
        emit skierCrashed();
        emit pointsOnCrash(totalScore);
    }
}

void GameModel::onSkiPerformanceUpdate(const Ski::SkiPerformance& perf){
    currentSkiPerformance = perf;
}

void GameModel::setTutorialPhase(TutorialPhase phase){
    tutorialPhase = phase;
}


void GameModel::handleJump(const GameObject* jumpObject){
    if (!isJumping) {
        isJumping = true;
        jumpTimer = 0;
        jumpStartY = skierPosition.y();

        // Award points
        int jumpPoints = 500;
        totalScore += jumpPoints;
        trickScore += jumpPoints;

        emit skierJumped();
        emit pointsEarned(jumpPoints);
        emit scoreChanged(totalScore);
    }
}
