#include <QVBoxLayout>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QFont>
#include <QPropertyAnimation>
#include <QGraphicsColorizeEffect>

#include "gamescreen.h"
#include "ui_gamescreen.h"

GameScreen::GameScreen(AppModel *model, GameModel *gameModel, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameScreen)
    , appModel(model)
    , gameModel(gameModel)
    , crashAnimationTimer(nullptr)
    , crashAnimationFrame(0)
    , pointsAnimationTimer(nullptr)
    , pointsAnimationFrame(0)
    , pendingCrashScore(-1)
{
    ui->setupUi(this);

    connect(this, &GameScreen::gameOver,
            model, &AppModel::onContinueClicked);

    // connect to update the current ski used in game.
    connect(model, &AppModel::skiPerformanceUpdate,
            gameModel, &GameModel::onSkiPerformanceUpdate);

    // connect to update the weather condition.
    connect(model, &AppModel::skiPerformanceUpdate,
            gameModel, &GameModel::onSkiPerformanceUpdate);

    setupView();

    connect(gameModel, &GameModel::skierMoved,
            this, &GameScreen::onSkierMoved);
    connect(gameModel, &GameModel::turnAngleChanged,
            this, &GameScreen::onTurnAngleChanged);
    connect(gameModel, &GameModel::backgroundScrolled,
            this, &GameScreen::onBackgroundScrolled);
    connect(gameModel, &GameModel::scoreChanged,
            this, &GameScreen::onScoreChanged);
    connect(gameModel, &GameModel::gameOverSignal,
            this, &GameScreen::onGameOver);
    connect(gameModel, &GameModel::newZone,
            this, &GameScreen::onZoneChange);

    connect(gameModel, &GameModel::objectSpawned,
            this, &GameScreen::onObjectSpawned);
    connect(gameModel, &GameModel::objectRemoved,
            this, &GameScreen::onObjectRemoved);
    connect(gameModel, &GameModel::skierCrashed,
            this, &GameScreen::onSkierCrashed);

    connect(gameModel, &GameModel::pointsOnCrash,
            this, [this](int points) {
                pendingCrashScore = points;
            });

    // All inputs will be 'focused' on GameScreen now.
    setFocusPolicy(Qt::StrongFocus);

    connect(model, &AppModel::weatherUpdate,
            this, &GameScreen::onWeatherUpdate);

    crashAnimationTimer = new QTimer(this);
    connect(crashAnimationTimer, &QTimer::timeout,
            this, &GameScreen::updateCrashAnimation);

    connect(gameModel, &GameModel::skierJumped,
            this, &GameScreen::onSkierJumped);
    connect(gameModel, &GameModel::skierLanded,
            this, &GameScreen::onSkierLanded);
    connect(gameModel, &GameModel::pointsEarned,
            this, &GameScreen::animatePointsText);

    pointsAnimationTimer = new QTimer(this);
    connect(pointsAnimationTimer, &QTimer::timeout,
            this, &GameScreen::updatePointsAnimation);
}

GameScreen::~GameScreen()
{
    if (snowAnimation) {
        snowAnimation->stop();
        delete snowAnimation;
    }
    if (snowAnimationLabel) {
        delete snowAnimationLabel;
    }

    delete ui;
}

void GameScreen::resetAndStartGame()
{

    // Remove all obstacles
    for (auto object : objectItems.values()) {
        scene->removeItem(object);
        delete object;
    }
    objectItems.clear();

    // Reset zone display
    QPixmap scaledZoneSign = zoneSignSprites[0].scaled(43, 43, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    zoneDisplay->setPixmap(scaledZoneSign);

    // Reset zone sign to starting position
    // zoneSign->setPixmap(zoneSignSprites[0]);
    zoneSign->setPos(550, 750);

    // Update background and weather display based on current weather
    background1->setPixmap(backgroundSprites[static_cast<int>(currentWeather)]);
    background2->setPixmap(backgroundSprites[static_cast<int>(currentWeather)]);

    background1->setPos(0, 0);
    background2->setPos(0, 750);

    QPixmap scaledWeatherIcon = weatherIconSprites[static_cast<int>(currentWeather)]
                                    .scaled(43, 43, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    weatherDisplay->setPixmap(scaledWeatherIcon);

    skier->setPixmap(skierSprites[2]);
    skier->setPos(337.5, 150);

    // Hide crash and points if text image was still visible
    crashText->setVisible(false);
    crashText->setScale(1.0);
    pointsText->setVisible(false);
    pointsText->setScale(1.0);


    if (crashAnimationTimer->isActive()) {
        crashAnimationTimer->stop();
    }

    if (pointsAnimationTimer->isActive()) {
        pointsAnimationTimer->stop();
    }

    pendingGameOverScore  = -1;
    pendingCrashScore = -1;
    crashAnimationFrame = 0;
    pointsAnimationFrame = 0;

    scoreLabel->setPlainText("Score: 0");

    instructions->show();
    instructionOverlay->setVisible(true);
    gameStarted = false;

    gameModel->resetGame();
    setFocus();
}


void GameScreen::setupView()
{
    // Create the scene with 750x750 dimensions
    scene = new QGraphicsScene(0, 0, 750, 750, this);

    // Creates the view
    view = new QGraphicsView(scene, this);
    view->setFixedSize(750, 750);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocusPolicy(Qt::NoFocus);
    scene->setSceneRect(0, 0, 750, 750);

    // Add view to the layout
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    if (mainLayout) {
        mainLayout->addWidget(view);
        qDebug() << "View added to existing layout";
    } else {
        // Create new layout if none exists
        mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->addWidget(view);
        qDebug() << "Created new layout and added view";
    }

    // ========== Loading and adding to the scene by Z-Value ==========

    // ==== Z - Value: -1 (Background Images - Furthest in the back) ====

    // Load background
    QPixmap backgroundPixmap(":/backgroundSprites/a9_Resources/backgroundResources/GameBackgroundUpdated.png");
    qDebug() << "Background loaded:" << !backgroundPixmap.isNull();

    // Scaling and Initializing Background Image to the scene (seamless scrolling)
    backgroundSprites[0] = QPixmap(":/backgroundSprites/a9_Resources/backgroundResources/GameBackgroundUpdated.png")
                               .scaled(750, 750, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    backgroundSprites[1] = QPixmap(":/backgroundSprites/a9_Resources/backgroundResources/GameBackgroundPowder.png")
                               .scaled(750, 750, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    backgroundSprites[2] = QPixmap(":/backgroundSprites/a9_Resources/backgroundResources/GameBackgroundIcy.png")
                               .scaled(750, 750, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    currentWeather = AppModel::GROOMER;

    // Setting the position of the backgrouds (first one is on screen - the second one starts above)
    background1 = scene->addPixmap(backgroundSprites[0]);
    background1->setPos(0, 0);
    background1->setZValue(-1);

    // Background behind everything
    background2 = scene->addPixmap(backgroundSprites[0]);
    background2->setPos(0, 750);
    background2->setZValue(-1);

    // ==== Z - Value: 5 (Zone Sign and Obstacles) ====

    // Scaling and Initializing Zone Sign Sprites
    zoneSignSprites[0] = QPixmap(":/otherSprites/a9_Resources/otherSprites/GreenSign.png")
                             .scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    zoneSignSprites[1] = QPixmap(":/otherSprites/a9_Resources/otherSprites/BlueSign.png")
                             .scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    zoneSignSprites[2] = QPixmap(":/otherSprites/a9_Resources/BlackSign.png")
                             .scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    zoneSign = scene->addPixmap(zoneSignSprites[0]);
    zoneSign->setPos(550,750);
    zoneSign->setZValue(5);

    // Load, Scale and Initialize Obstacles Sprites
    treeSmallSprite = QPixmap(":/obstacleSprites/a9_Resources/obstacleSprites/Tree1.png")
                          .scaled(36, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    treeLargeSprite = QPixmap(":/obstacleSprites/a9_Resources/obstacleSprites/Tree2.png")
                          .scaled(60, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    rockSprite = QPixmap(":/obstacleSprites/a9_Resources/obstacleSprites/Rock1.png")
                     .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    cliffDropSprite = QPixmap(":/obstacleSprites/a9_Resources/obstacleSprites/CliffDrop.png")
                          .scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    powderSprite = QPixmap(":/obstacleSprites/a9_Resources/obstacleSprites/Powder.png")
                       .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    snowPileSprite = QPixmap(":/obstacleSprites/a9_Resources/obstacleSprites/SnowPile.png")
                         .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    jumpSprite = QPixmap(":/obstacleSprites/a9_Resources/obstacleSprites/Jump.png")
                     .scaled(80, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // ==== Z - Value: 10 (Skier) ====

    // Load, Scale and Initialize Skier Sprites
    skierSprites[0] = QPixmap(":/skierSprites/a9_Resources/skierSprites/SkierLeft2.png")
                          .scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    skierSprites[1] = QPixmap(":/skierSprites/a9_Resources/skierSprites/SkierLeft3.png")
                          .scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    skierSprites[2] = QPixmap(":/skierSprites/a9_Resources/skierSprites/SkierStraight.png")
                          .scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    skierSprites[3] = QPixmap(":/skierSprites/a9_Resources/skierSprites/SkierRight3.png")
                          .scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    skierSprites[4] = QPixmap(":/skierSprites/a9_Resources/skierSprites/SkierRight2.png")
                          .scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    skierSprites[5] = QPixmap(":/skierSprites/a9_Resources/skierSprites/CrashedSkierLeft.png")
                          .scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    skierSprites[6] = QPixmap(":/skierSprites/a9_Resources/skierSprites/SnowCoveredSkier.png")
                          .scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    skierSprites[7] = QPixmap(":/skierSprites/a9_Resources/skierSprites/TempTrickSkier.png")
                          .scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Adding Skier Image to the Scene
    skier = scene->addPixmap(skierSprites[2]);
    skier->setPos(gameModel->getSkierPosition());
    skier->setZValue(10);


    // ==== Z - Value: 15 (Crash Animation) ====

    crashTextSprite = QPixmap(":/obstacleSprites/a9_Resources/obstacleSprites/CRASH!.png");
    crashText = scene->addPixmap(crashTextSprite);
    crashText->setZValue(15);
    crashText->setVisible(false);
    crashText->setTransformOriginPoint(crashText->boundingRect().center());

    // Add points text
    pointsTextSprite = QPixmap(":/obstacleSprites/a9_Resources/obstacleSprites/+500!.png");
    pointsText = scene->addPixmap(pointsTextSprite);
    pointsText->setZValue(15);
    pointsText->setVisible(false);
    pointsText->setTransformOriginPoint(pointsText->boundingRect().center());


    // ==== Z - Value: 100 (Snow Particles Animation) ====

    snowAnimationLabel = new QLabel(view);
    snowAnimationLabel->setFixedSize(750, 750);
    snowAnimationLabel->setGeometry(0, 0, 750, 750);
    snowAnimationLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    snowAnimationLabel->setStyleSheet("background: transparent;");
    snowAnimationLabel->setScaledContents(false);

    // Load and play the GIF
    snowAnimation = new QMovie(":/backgroundSprites/a9_Resources/backgroundResources/BigSnowFall.gif");
    snowAnimation->setScaledSize(QSize(750, 750));
    snowAnimationLabel->setMovie(snowAnimation);
    snowAnimation->start();

    // Raise it above the scene but allow interaction
    snowAnimationLabel->raise();
    snowAnimationLabel->show();

    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
    effect->setColor(QColor(180, 180, 255));  // light blue tint
    effect->setStrength(500);                 // subtle tint
    snowAnimationLabel->setGraphicsEffect(effect);

    // ==== Z - Value: 105 (*UI* Background Frames) ====

    // Create the score frame / background
    QPixmap scoreFrame(":/backgroundSprites/a9_Resources/backgroundResources/ScoreFrame.png");
    scoreFrame = scoreFrame.scaled(300, 50);
    scoreBackground = scene->addPixmap(scoreFrame);
    scoreBackground->setPos(5, 5);
    scoreBackground->setZValue(105);

    // Create the zone frame / background
    QPixmap zoneFrame(":/backgroundSprites/a9_Resources/backgroundResources/ScoreFrame.png");
    zoneFrame = zoneFrame.scaled(50, 50);
    zoneBackground = scene->addPixmap(zoneFrame);
    zoneBackground->setPos(650, 5);
    zoneBackground->setZValue(105);

    // Create the weather frame / background
    QPixmap weatherFrame(":/backgroundSprites/a9_Resources/backgroundResources/WeatherFrame.png");
    weatherFrame = weatherFrame.scaled(50, 50);
    weatherBackround = scene->addPixmap(weatherFrame);
    weatherBackround->setPos(375, 5);
    weatherBackround->setZValue(105);

    // ==== Z - Value: 110 (*UI* Content - Score and Zone Display) ====

    // Create the score label display
    scoreLabel=new QGraphicsTextItem();
    scoreLabel->setDefaultTextColor(Qt::black);
    scoreLabel->setFont(QFont("Retro Gaming",20));
    scoreLabel->setPlainText("Score: 0");
    scene->addItem(scoreLabel);
    scoreLabel->setPos(45,10);
    scoreLabel->setZValue(110);

    // Create the zone label display (scaled to fix in background image)
    QPixmap scaledZoneSign = zoneSignSprites[0].scaled(43, 43, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    zoneDisplay = scene->addPixmap(scaledZoneSign);
    zoneDisplay->setPos(653, 9);
    zoneDisplay->setZValue(110);

    // Load weather icon sprites
    weatherIconSprites[0] = QPixmap(":/otherSprites/a9_Resources/otherSprites/SunnyDayIcon.png");
    weatherIconSprites[1] = QPixmap(":/otherSprites/a9_Resources/otherSprites/PowderDayIcon.png");
    weatherIconSprites[2] = QPixmap(":/otherSprites/a9_Resources/otherSprites/IcyDayIcon.png");

    // Create the weather display (scaled to fit in background frame)
    QPixmap scaledWeatherIcon = weatherIconSprites[0].scaled(43, 43, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    weatherDisplay = scene->addPixmap(scaledWeatherIcon);
    weatherDisplay->setPos(378, 9);
    weatherDisplay->setZValue(110);

    // ==== Z - Value: 115 (Game Instructions) ====

    //Set up the instruction label
    instructions =new QGraphicsTextItem("Press Any Key to Continue");
    instructions->setDefaultTextColor(Qt::black);
    instructions->setFont(QFont("Retro Gaming",20));
    instructions->setZValue(115);
    scene->addItem(instructions);
    instructions->setPos(150, 650);

    QPixmap instructionPixmap(":/backgroundSprites/a9_Resources/backgroundResources/Instruction.png");
    instructionOverlay = scene->addPixmap(instructionPixmap.scaled(600, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    instructionOverlay->setPos(75, 75);
    instructionOverlay->setZValue(115);
    instructionOverlay->setVisible(true);

    gameStarted = false;
}

void GameScreen::keyPressEvent(QKeyEvent *event)
{

    if (!gameStarted) {
        instructions->hide();

        instructionOverlay->setVisible(false);
        gameStarted = true;
        gameModel->startGame();
        return;
    }

    switch(event->key()) {
        case Qt::Key_Left:
        case Qt::Key_A:
            gameModel->turnLeft();
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            gameModel->turnRight();
            break;
        case Qt::Key_Up:
        case Qt::Key_W:
        case Qt::Key_Space:
        case Qt::Key_Down:
            gameModel->goStraight();
            break;
    }

    QWidget::keyPressEvent(event);
}

void GameScreen::onSkierMoved(QPointF newPosition)
{
    skier->setPos(newPosition);
}

void GameScreen::onTurnAngleChanged(int angle)
{
    if (!gameModel->isCrashed()) {
        updateSkierSprite(angle);
    }}

void GameScreen::updateSkierSprite(int angle)
{
    // Angle [-2 to 2] to index [0 to 4]
    int index = angle + 2;

    if (index >= 0 && index < 5) {
        skier->setPixmap(skierSprites[index]);
    }
}

void GameScreen::onBackgroundScrolled(int offset)
{
    // Moving BOTH backgrounds
    background1->moveBy(0, offset);
    background2->moveBy(0, offset);

    // Move Zone Sign
    zoneSign->moveBy(0,offset);


    for (auto it = objectItems.begin(); it != objectItems.end(); ++it) {
        GameObject* object = it.key();
        QGraphicsPixmapItem* item = it.value();
        QPointF newPos = object->getPosition();
        item->setPos(newPos);
    }

    // Seamless Scrolling - When a backgrounds goes off top, move to bottom
    // Check if background1 has scrolled completely off the top
    if (background1->y() <= -750) {
        // Reposition it above background2
        background1->setPos(0, background2->y() +750);
    }

    // Check if background2 has scrolled completely off the top
    if (background2->y() <= -750 ) {
        // Reposition it above background1
        background2->setPos(0, background1->y() + 750);
    }

}

void GameScreen::onScoreChanged(int score)
{
    scoreLabel->setPlainText("Score: "+QString::number(score));
}

void GameScreen::onGameOver(int finalScore)
{

    // First, update game end with the crash score (this triggers weather forecast)
    if (pendingCrashScore >= 0) {
        appModel->onGameEnd(pendingCrashScore);
        pendingCrashScore = -1;  // Reset
    }

    // Store the score but don't emit gameOver signal yet
    pendingGameOverScore = finalScore;
}


void GameScreen::onZoneChange(int zone){

    if (zone < 0 || zone >= 3) {
        zone = qBound(0, zone, 2);
    }

    QPixmap scaledSign = zoneSignSprites[zone].scaled(43, 43, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    zoneSign->setPixmap(zoneSignSprites[zone]);
    zoneDisplay->setPos(653, 9);
    zoneDisplay->setPixmap(scaledSign);
}

void GameScreen::onObjectSpawned(GameObject* object)
{
    // Choose sprite based on object type
    QPixmap sprite;
    switch(object->getType()) {
        case GameObject::SMALLTREE:
            sprite = treeSmallSprite;
            break;
        case GameObject::LARGETREE:
            sprite = treeLargeSprite;
            break;
        case GameObject::ROCK:
            sprite = rockSprite;
            break;
        case GameObject::CLIFF:
            sprite = cliffDropSprite;
            break;
        case GameObject::POWDER:
            sprite = powderSprite;
            break;
        case GameObject::SNOWPILE:
            sprite = snowPileSprite;
            break;
        case GameObject::JUMP:
            sprite = jumpSprite;
            break;
        default:
            sprite = treeSmallSprite;
            break;
    }

    // Create graphics item
    QGraphicsPixmapItem* item = scene->addPixmap(sprite);
    item->setPos(object->getPosition());
    item->setZValue(5);

    // Store in map for tracking
    objectItems[object] = item;

}

void GameScreen::onObjectRemoved(GameObject* object)
{
    // Remove from scene and cleanup
    if (objectItems.contains(object)) {
        QGraphicsPixmapItem* item = objectItems[object];
        scene->removeItem(item);
        delete item;
        objectItems.remove(object);
    }
}

void GameScreen::onSkierCrashed()
{
    // Change to crash sprite
    skier->setPixmap(skierSprites[5]);
    animateCrashText();
}

void GameScreen::onWeatherUpdate(AppModel::Weather weather)
{
    currentWeather = weather;

    // Update background images

    background1->setPixmap(backgroundSprites[static_cast<int>(weather)]);
    background2->setPixmap(backgroundSprites[static_cast<int>(weather)]);

    // Update weather icon display
    QPixmap scaledWeatherIcon = weatherIconSprites[static_cast<int>(weather)]
                                    .scaled(43, 43, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    weatherDisplay->setPixmap(scaledWeatherIcon);

    qDebug() << "Weather updated to:" << weather;

        snowAnimationLabel->show();
        snowAnimation->start();
}

void GameScreen::animateCrashText()
{
    // Position the crash text above the skier's head
    QPointF skierPos = skier->pos();
    qreal crashTextWidth = crashText->boundingRect().width();
    qreal crashTextHeight = crashText->boundingRect().height();

    // Center the text above the skier
    crashText->setPos(skierPos.x() + 37.5 - crashTextWidth/2,
                      skierPos.y() - crashTextHeight - 10);

    // Make visible
    crashText->setVisible(true);
    crashText->setScale(0.1);  // Start very small

    // Reset animation
    crashAnimationFrame = 0;
    crashAnimationTimer->start(20);  // Update every 20ms (50 FPS)

    qDebug() << "Crash text animation started";
}

void GameScreen::updateCrashAnimation()
{
    crashAnimationFrame++;

    if (pendingGameOverScore >= 0 && crashAnimationFrame >= 40) {
        // Animation complete
        crashAnimationTimer->stop();
        crashText->setVisible(false);
        crashText->setScale(1.0);
        qDebug() << "Crash animation complete";

        qDebug() << "Emitting delayed gameOver signal with score:" << pendingGameOverScore;
        emit gameOver(pendingGameOverScore);
        pendingGameOverScore = -1;  // Reset
        return;
    }

    int animFrames = 40;
    qreal progress = qMin(1.0, static_cast<qreal>(crashAnimationFrame) / animFrames);
    qreal scale;

    if (progress < 0.5) {
        // First half: grow from 0.1 to 1.5
        scale = 0.1 + (1.4 * (progress * 2.0));
    } else {
        // Second half: shrink from 1.5 to 0.5
        scale = 1.5 - (1.0 * ((progress - 0.5) * 2.0));
    }

    // Add bounce effect during growth
    if (progress < 0.5) {
        qreal bounce = sin(progress * 3.14159 * 4) * 0.1 * (1.0 - progress * 2.0);
        scale += bounce;
    }

    crashText->setScale(scale);
}

void GameScreen::onSkierJumped()
{
    // Change to jumping sprite
    skier->setPixmap(skierSprites[7]);
}

void GameScreen::onSkierLanded()
{
    // Return to straight sprite
    skier->setPixmap(skierSprites[2]);
}

void GameScreen::animatePointsText(int points)
{
    // Position the points text above the skier's head
    QPointF skierPos = skier->pos();
    qreal pointsTextWidth = pointsText->boundingRect().width();
    qreal pointsTextHeight = pointsText->boundingRect().height();

    // Center the text above the skier
    pointsText->setPos(skierPos.x() + 37.5 - pointsTextWidth/2,
                       skierPos.y() - pointsTextHeight - 10);

    // Make visible
    pointsText->setVisible(true);
    pointsText->setScale(0.1);  // Start very small

    // Reset animation
    pointsAnimationFrame = 0;
    pointsAnimationTimer->start(20);  // Update every 20ms (50 FPS)

    qDebug() << "Points animation started:" << points;
}

void GameScreen::updatePointsAnimation()
{
    const int totalFrames = 40;  // 40 frames * 20ms = 800ms total
    pointsAnimationFrame++;

    if (pointsAnimationFrame >= totalFrames) {
        // Animation complete
        pointsAnimationTimer->stop();
        pointsText->setVisible(false);
        pointsText->setScale(1.0);
        qDebug() << "Points animation complete";
        return;
    }

    // Calculate scale based on frame (0.0 to 1.0 progress)
    qreal progress = static_cast<qreal>(pointsAnimationFrame) / totalFrames;
    qreal scale;

    if (progress < 0.5) {
        // First half: grow from 0.1 to 1.5
        scale = 0.1 + (1.4 * (progress * 2.0));
    } else {
        // Second half: shrink from 1.5 to 0.5
        scale = 1.5 - (1.0 * ((progress - 0.5) * 2.0));
    }

    // Add bounce effect during growth
    if (progress < 0.5) {
        qreal bounce = sin(progress * 3.14159 * 4) * 0.1 * (1.0 - progress * 2.0);
        scale += bounce;
    }

    pointsText->setScale(scale);
}
