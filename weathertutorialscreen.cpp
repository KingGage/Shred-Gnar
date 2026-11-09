/******************************************************************************
 * Project File:    WeatherTutorialScreen.cpp
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The weather tutorial screen implementation.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#include "weathertutorialscreen.h"
#include "ui_weathertutorialscreen.h"


WeatherTutorialScreen::WeatherTutorialScreen(AppModel *model, QWidget *parent)
    : QWidget(parent), ui(new Ui::WeatherTutorialScreen){
    ui->setupUi(this);

    connect(ui->nextButton, &QPushButton::clicked,
            this, &WeatherTutorialScreen::showNext);
    connect(this, &WeatherTutorialScreen::nextClicked,
           model, &AppModel::onContinueClicked);

    typingTimer = new QTimer(this);
    connect(typingTimer, &QTimer::timeout,
            this, &WeatherTutorialScreen::typeNextCharacter);

    textIndex = 0;
    charIndex = 0;
    typingSpeed = 30;

    readTutorialDialogue(":/dialogues/a9_Resources/dialogues/weatherTutorial.txt");
    jonnyImagePaths = {
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyGesture.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyGesture.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyGesture.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",

    };

    groomerSkis = {
        ":/skiSprites/a9_Resources/skiSprites/raceSki.png",
        ":/skiSprites/a9_Resources/skiSprites/carvingSki.png",
        ":/skiSprites/a9_Resources/skiSprites/mogulSki.png",
        ":/skiSprites/a9_Resources/skiSprites/allMountainSki.png"
    };

    powderSkis = {
        ":/skiSprites/a9_Resources/skiSprites/powderSki.png",
        ":/skiSprites/a9_Resources/skiSprites/bigMountainSki.png"
    };
    icySkis = {
        ":/skiSprites/a9_Resources/skiSprites/allMountainSki.png",
        ":/skiSprites/a9_Resources/skiSprites/bigMountainSki.png",
        ":/skiSprites/a9_Resources/skiSprites/touringSki.png"
    };

    ui->textBrowser->setStyleSheet(
        "QTextBrowser {"
        "background-color: transparent;"
        "border: 2px solid transparent;"
        "padding: 10px;"
        "font-family: 'Retro Gaming';"
        "font-size: 16pt;"
        "font-weight: bold;"
        "color: black;"
        "}"
    );

    ui->skiSpriteList->setStyleSheet(
        "QListWidget {"
        "  background: transparent;"
        "  border: none;"
        "}"
        "QListWidget::item {"
        "  background: transparent;"
        "}"
    );

    ui->skiSpriteList->setViewMode(QListView::IconMode);
    ui->skiSpriteList->setFlow(QListView::LeftToRight);
    ui->skiSpriteList->setWrapping(false);

    ui->skiSpriteList->setIconSize(QSize(60, 180));
    ui->skiSpriteList->setGridSize(QSize(80, 200));
    ui->skiSpriteList->setSpacing(10);
    ui->skiSpriteList->setFrameStyle(QFrame::NoFrame);
}


WeatherTutorialScreen::~WeatherTutorialScreen(){
    delete ui;
}


void WeatherTutorialScreen::onStartDialogue(){
    updateTextBrowser();
}

void WeatherTutorialScreen::readTutorialDialogue(const QString &filename){
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filename << file.errorString();
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    QStringList paragraphs = content.split("\n\n");
    dialogues.clear();

    for (const QString &p : paragraphs) {
        QString trimmed = p.trimmed();
        if (!trimmed.isEmpty()) {
            dialogues.append(trimmed);
        }
    }

    textIndex = 0;
}


void WeatherTutorialScreen::showNext(){
    if (typingTimer->isActive()){
        typingTimer->stop();
        charIndex = currentFullText.length();
        ui->textBrowser->setHtml(currentFullText);
        return;
    }

    textIndex++;

    if (textIndex > dialogues.size() - 1){
        emit nextClicked();
        textIndex = 0;
    } else {
        updateTextBrowser();
    }
}


void WeatherTutorialScreen::updateTextBrowser(){
    typingTimer->stop();
    currentFullText = dialogues[textIndex];
    charIndex = 0;
    ui->textBrowser->clear();

    // Update Jonny
    if (textIndex < jonnyImagePaths.size()){
        QPixmap pixmap(jonnyImagePaths[textIndex]);
        ui->jonnySprite->setPixmap(pixmap);
        ui->jonnySprite->setScaledContents(true);
    }
    if (textIndex < weatherIconStart || textIndex >= weatherIconEnd){
        ui->weatherSprite->setVisible(false);
        ui->weatherIconHolder->setVisible(false);
        ui->skiSpriteContainer->setVisible(false);
        ui->skiSpriteList->clear();
    } else {
        // Decide which weather this textIndex belongs to
        weatherState newWeather;

        if (textIndex >= groomerStart && textIndex < powderStart){
            newWeather = Groomer;
        } else if (textIndex >= powderStart && textIndex < icyStart){
            newWeather = Powder;
        } else {
            newWeather = Icy;
        }

        //If weather changed, clear skis and update state
        if (newWeather != currentWeatherState){
            currentWeatherState = newWeather;
            ui->skiSpriteList->clear();
        }

        //Update weather icon + currentSkiStack
        getCurrentDisplaySprites();
        ui->weatherSprite->setVisible(true);

        //Decide how many skis should be visible for THIS weather
        int targetCount = 0;

        if (currentWeatherState == Groomer){
            if (textIndex >= groomerSkiDisplay){
                targetCount = textIndex - groomerSkiDisplay + 1;
            }
            if (targetCount > groomerSkis.size()){
                targetCount = groomerSkis.size();
            }
            if (targetCount < 0){
                targetCount = 0;
            }

            // Only add *new* skis
            int currentCount = ui->skiSpriteList->count();
            for (int i = currentCount; i < targetCount; ++i){
                addSkiSprite(groomerSkis[i]);
            }

        } else if (currentWeatherState == Powder){
            if (textIndex >= powderSkiDisplay){
                targetCount = textIndex - powderSkiDisplay + 1;
            }
            if (targetCount > powderSkis.size()){
                targetCount = powderSkis.size();
            }
            if (targetCount < 0) {
                targetCount = 0;
            }

            int currentCount = ui->skiSpriteList->count();
            for (int i = currentCount; i < targetCount; ++i){
                addSkiSprite(powderSkis[i]);
            }

        } else if (currentWeatherState == Icy){
            if (textIndex >= icySkiDisplay) {
                targetCount = textIndex - icySkiDisplay + 1;
            }
            if (targetCount > icySkis.size()){
                targetCount = icySkis.size();
            }
            if (targetCount < 0){
                targetCount = 0;
            }

            int currentCount = ui->skiSpriteList->count();
            for (int i = currentCount; i < targetCount; ++i){
                addSkiSprite(icySkis[i]);
            }
        } else {
        ui->weatherSprite->setVisible(false);
        ui->weatherIconHolder->setVisible(false);
        ui->skiSpriteContainer->setVisible(false);
        ui->skiSpriteList->clear();
        }}

    typingTimer->start(typingSpeed);
}


void WeatherTutorialScreen::typeNextCharacter(){
    if (charIndex >= currentFullText.length()){
        typingTimer->stop();
        return;
    }

    QString partialText = currentFullText.left(charIndex + 1);
    ui->textBrowser->setHtml(partialText);
    charIndex++;
}


void WeatherTutorialScreen::getCurrentDisplaySprites(){
    if(currentWeatherState == Groomer){
        QPixmap pixmap(":/otherSprites/a9_Resources/otherSprites/SunnyDayIcon.png");
        ui->weatherSprite->setPixmap(pixmap);
        ui->weatherSprite->setScaledContents(true);
        currentSkiStack = groomerSkis;
        ui->weatherIconHolder->setVisible(true);
        ui->skiSpriteContainer->setVisible(true);
    } else if(currentWeatherState == Powder){
        QPixmap pixmap(":/otherSprites/a9_Resources/otherSprites/PowderDayIcon.png");
        ui->weatherSprite->setPixmap(pixmap);
        ui->weatherSprite->setScaledContents(true);
        ui->weatherIconHolder->setVisible(true);
        ui->skiSpriteContainer->setVisible(true);
        currentSkiStack = powderSkis;
    } else if(currentWeatherState == Icy){
        QPixmap pixmap(":/otherSprites/a9_Resources/otherSprites/IcyDayIcon.png");
        ui->weatherSprite->setPixmap(pixmap);
        ui->weatherSprite->setScaledContents(true);
        ui->weatherIconHolder->setVisible(true);
        ui->skiSpriteContainer->setVisible(true);
        currentSkiStack = icySkis;
    }
}


void WeatherTutorialScreen::switchWeather(weatherState nextWeather){
    currentWeatherState = nextWeather;
}


void WeatherTutorialScreen::addSkiSprite(const QString &resourcePath){
    QPixmap pix(resourcePath);
    QPixmap scaled = pix.scaled(60, 180,
                                Qt::IgnoreAspectRatio,
                                Qt::SmoothTransformation);
    QIcon icon(scaled);

    auto *item = new QListWidgetItem();
    item->setIcon(icon);
    item->setSizeHint(QSize(80, 200));

    ui->skiSpriteList->addItem(item);
}

