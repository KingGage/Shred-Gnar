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
 *      Main window implementation
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#include "mainwindow.h"


MainWindow::MainWindow(AppModel *model, GameModel *gameModel, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QFontDatabase::addApplicationFont(":/fonts/a9_Resources/fonts/RetroGaming.ttf");

    startScreen        = new StartScreen(model, this);
    tutorialScreen     = new TutorialScreen(model, this);
    terminologyScreen  = new TerminologyScreen(model, this);
    weatherScreen      = new WeatherTutorialScreen(model, this);
    skiShopScreen      = new SkiShop(model, this);
    gameScreen         = new GameScreen(model, gameModel, this);
    pointsEarnedScreen = new PointsEarnedScreen(model, gameModel, this);
    endScreen          = new EndScreen(this);

    ui->stackedWidget->addWidget(startScreen);
    ui->stackedWidget->addWidget(tutorialScreen);
    ui->stackedWidget->addWidget(terminologyScreen);
    ui->stackedWidget->addWidget(weatherScreen);
    ui->stackedWidget->addWidget(skiShopScreen);
    ui->stackedWidget->addWidget(gameScreen);
    ui->stackedWidget->addWidget(pointsEarnedScreen);
    ui->stackedWidget->addWidget(endScreen);
    ui->stackedWidget->setCurrentWidget(startScreen);

    // Connect to start the dialogues
    connect(this, &MainWindow::startDialogue,
            terminologyScreen, &TerminologyScreen::onStartDialogue);
    connect(this, &MainWindow::startDialogue,
            tutorialScreen, &TutorialScreen::onStartDialogue);
    connect(this, &MainWindow::switchSkiShopDialogue,
            skiShopScreen, &SkiShop::handelChangeStates);
    connect(this, &MainWindow::startDialogue,
            weatherScreen, &WeatherTutorialScreen::onStartDialogue);
    connect(this, &MainWindow::startDialogue,
            endScreen, &EndScreen::onStartDialogue);

    // Link up the buttons and signals to the app model
    connect(skiShopScreen, &SkiShop::skiSelected,
            model, &AppModel::onSkiSelect);
    connect(model, &AppModel::displayScreen,
            this, &MainWindow::onDisplayScreen);

    connect(pointsEarnedScreen, &PointsEarnedScreen::endGameClicked,
            this, [this](){
                emit startDialogue();
                this->ui->stackedWidget->setCurrentWidget(endScreen);
            });
    connect(endScreen, &EndScreen::playAgainButtonClicked,
            this, [this, model](){
                this->ui->stackedWidget->setCurrentWidget(startScreen);
                model->resetApplication();
            });

    // Link up to show the weather icons
    connect(this, &MainWindow::showWeatherIcons,
            skiShopScreen, &SkiShop::onShowWeatherIcons);
    // connect(gameModel, &GameModel::pointsOnCrash,
    //         model, &AppModel::forecastWeather);

    // connect(gameModel, &GameModel::gameOverSignal,
    //         model, &AppModel::forecastWeather);

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::onDisplayScreen(const QString& screenName){
    if (screenName == "startScreen") {
        ui->stackedWidget->setCurrentWidget(startScreen);
    } else if (screenName == "tutorialScreen") {
        ui->stackedWidget->setCurrentWidget(tutorialScreen);
        emit startDialogue();
    } else if (screenName == "skiShopScreen"){
        ui->stackedWidget->setCurrentWidget(skiShopScreen);
        emit switchSkiShopDialogue();
    } else if (screenName == "gameScreen") {
        ui->stackedWidget->setCurrentWidget(gameScreen);
        gameScreen->resetAndStartGame();
    } else if (screenName == "terminologyScreen") {
        ui->stackedWidget->setCurrentWidget(terminologyScreen);
        emit startDialogue();
    } else if (screenName == "pointsEarnedScreen") {
        ui->stackedWidget->setCurrentWidget(pointsEarnedScreen);
    } else if (screenName == "weatherScreen"){
        ui->stackedWidget->setCurrentWidget(weatherScreen);
        emit startDialogue();
        emit showWeatherIcons();
    }
}
