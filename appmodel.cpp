/******************************************************************************
 * Project File:    AppModel.cpp
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      Model class implementation
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#include "appmodel.h"


AppModel::AppModel(QObject *parent)
    : QObject(parent), gamePhase(TUTORIAL), tutorialStep(0), playerPoints(1000){

    tutorialScreens = {
        "startScreen",
        "tutorialScreen",
        "gameScreen",       // First groomer game
        "terminologyScreen",
        "skiShopScreen",    // Ski shop dialogue
        "gameScreen",       // Second groomer game
        "pointsEarnedScreen",
        "weatherScreen",
        "skiShopScreen",    // Should now have the weather
        "gameScreen"        // Powder game
    };

    gameCycleScreens = {
        "skiShopScreen",
        "gameScreen",
        "pointsEarnedScreen"
    };

    nextWeather = GROOMER;

    skisList[ALLMOUNTAIN] = Ski("All Mountain", 100, 6, 6, 1000, "The do-everything workhorse. Running 90-105mm underfoot, these handle groomers, chop, crud, and moderate powder without breaking a sweat. Balanced flex and moderate rocker/camber combo make them versatile enough for whatever the mountain throws at you. Perfect for riders who want one ski to rule them all.", ":/skiSprites/a9_Resources/skiSprites/allMountainSki.png");
    skisList[CARVING] = Ski("Carving",80, 9, 1, 1000, "Purpose-built for laying down trenches on corduroy. Medium width (75-90mm underfoot) with deep sidecut and full camber for maximum edge contact. Stiffer than all-mountain skis and reward clean technique with razor-sharp precision. These are for folks who get up early to chase freshly groomed runs.", ":/skiSprites/a9_Resources/skiSprites/carvingSki.png");
    skisList[FREERIDE] = Ski("Freeride", 110, 8, 8, 1000, "Burly chargers for steep lines and variable conditions. Wider than all-mountain (100-115mm) but more directional than powder skis. Stiff construction handles speed and chunder while providing stability when you're sending it off cliffs. Built tough for riders who push limits.", ":/skiSprites/a9_Resources/skiSprites/bigMountainSki.png");
    skisList[MOGUL] = Ski("Mogul", 70, 7, 2, 1000, "Short, narrow, and nimble skis (70-85mm underfoot) built for navigating those zipper lines down mogul fields. Quick edge-to-edge response and a softer flex pattern let you absorb all those bumps without getting bucked off the mountain. Shorter length makes rapid-fire turns easier when you're dancing through tight troughs.", ":/skiSprites/a9_Resources/skiSprites/mogulSki.png");
    skisList[PARK] = Ski("Park", 95, 2, 8, 1000, "Twin-tipped and playful, built for hitting jumps, rails, and riding switch. These run narrower (85-95mm underfoot) with a symmetrical shape so you can land backwards without eating it. Softer flex makes them buttery for presses and forgiving on landings. Durable construction because, let's face it, rails are not kind to your bases.", ":/skiSprites/a9_Resources/skiSprites/parkSki.png");
    skisList[POWDER] = Ski("Powder", 115, 6, 9, 1000, "Wide, floaty boards designed for deep snow days. These bad boys typically run 110-130mm underfoot with plenty of rocker to keep your tips up. The extra width gives you surface area so you're surfing on top of the snow instead of sinking like a rock. Usually have a wider tip and tail for that effortless float.", ":/skiSprites/a9_Resources/skiSprites/powderSki.png");
    skisList[RACE] = Ski("Race", 75, 9, 2, 1000, "Narrow speed demons (65-75mm underfoot) built for precision on hardpack. Aggressive sidecut and camber underfoot mean they carve turns like they're on rails. Stiff flex rewards aggressive skiing but demands proper technique. These are all about edge grip and quick turn transitions on groomed runs.", ":/skiSprites/a9_Resources/skiSprites/raceSki.png");
    skisList[TOURING] = Ski("Touring", 100, 8, 7, 1000, "Lightweight skis designed for earning your turns. These run 90-105mm underfoot with a focus on keeping weight down for the uphill without sacrificing too much performance on the descent. Often feature rockered tips for variable snow and a balanced flex that won't exhaust you on the climb but still rips on the way down.", ":/skiSprites/a9_Resources/skiSprites/touringSki.png");

    currentSki = &skisList[ALLMOUNTAIN]; // starter ski
    skisList[ALLMOUNTAIN].unlock();
}

void AppModel::resetApplication(){
    gamePhase = TUTORIAL;
    nextWeather = GROOMER;
    currentSki = &skisList[ALLMOUNTAIN];
    tutorialStep = 0;
    gameStep = 0;
    for (auto& pair : skisList){
        pair.second.lock();
        emit skiLocked(pair.second.getName());
    }
    skisList[ALLMOUNTAIN].unlock();
}

void AppModel::forecastWeather(){
    if (gamePhase == TUTORIAL){
        return;
    }
    nextWeather = static_cast<Weather>(QRandomGenerator::global()->bounded(3));
    emit weatherUpdate(nextWeather);
}

void AppModel::startGame(){
    if (currentSki->isLocked()){
        return; // don't allow game start if a locked ski is selected
    }

    updateSkiPerformance();
    emit weatherUpdate(nextWeather);
    onContinueClicked(); // to change to the game screen
}

void AppModel::onContinueClicked(){
    if (gamePhase == TUTORIAL){
        tutorialStep++;
        if (tutorialStep >= tutorialScreens.size()){
            gamePhase = GAME_LOOP;
            emit displayScreen("pointsEarnedScreen");
        } else {
            if (tutorialStep == 2 || tutorialStep == 5){
                nextWeather = GROOMER;
            } else if (tutorialStep == 9){
                nextWeather = POW;
            }
            emit displayScreen(tutorialScreens[tutorialStep]);
        }
    } else if (gamePhase == GAME_LOOP){
        emit displayScreen(gameCycleScreens[gameStep % gameCycleScreens.size()]);
        gameStep++;
    }
}

void AppModel::onSkiSelect(const SkiType& skiType){
    this->currentSki = &skisList[skiType];
    emit currentSelectedSkiChanged(currentSki);
}

void AppModel::onPurchaseCurrentSki(){
    if (!currentSki->isLocked()){
        return;
    }

    int price = currentSki->getCostPoints();
    if (playerPoints >= price){
        currentSki->unlock();
        playerPoints -= price;
        emit skiUnlocked(currentSki->getName());
        emit displayPlayerPoints(playerPoints);
    } else {
        emit notEnoughPoints();
    }
}

void AppModel::onGameEnd(int points){
    playerPoints += points;
    forecastWeather();
    emit displayPlayerPoints(playerPoints);
}

void AppModel::updateSkiPerformance(){
    Ski::SkiPerformance currentPerformance;
    currentSki->calculatePerformance();

    switch(nextWeather){
    case GROOMER:
        currentPerformance = currentSki->getGroomerPerformance();
        break;
    case POW:
        currentPerformance = currentSki->getPowderPerformance();
        break;
    case ICY:
        currentPerformance = currentSki->getIcyPerformance();
        break;
    }

    emit skiPerformanceUpdate(currentPerformance);
}

Ski* AppModel::getDefaultSki(){
    return &skisList[ALLMOUNTAIN];
}
