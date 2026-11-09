/******************************************************************************
 * Project File:    SkiShopScreen.cpp
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      Implementation of ski shop screen.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#include "skishopscreen.h"


SkiShop::SkiShop(AppModel *model, QWidget *parent)
    : QWidget(parent), ui(new Ui::SkiShop){
    ui->setupUi(this);
    Ski* defaultSki = model->getDefaultSki();
    ui->dataWidget->setVisible(true);
    ui->skiDisplayed->setPixmap(defaultSki->getImagePath());
    ui->name->setText(defaultSki->getName());
    ui->flex->setText(QString::number(defaultSki->getFlex()));
    ui->width->setText(QString::number(defaultSki->getWidth()) + " mm");
    ui->rocker->setText(QString::number(defaultSki->getRocker()));
    currentSkiDescription = defaultSki->getDescription();
    ui->price->setText(QString::number(defaultSki->getCostPoints()));

    ui->groomerRating->setValue(static_cast<int>(defaultSki->getGroomerScore() * 100));
    ui->powderRating->setValue(static_cast<int>(defaultSki->getPowderScore() * 100));
    ui->iceRating->setValue(static_cast<int>(defaultSki->getIcyScore() * 100));


    // Hide the weather icon
    ui->weatherIcon->hide();
    ui->weatherIconBackground->hide();

    ui->textBrowser->setStyleSheet(
        "QTextBrowser {"
        "background: black;"
        "color: white;"
        "font-family: 'Retro Gaming';"
        "font-size: 10pt;"
        "}"
    );

    connect(model, &AppModel::weatherUpdate,
            this, &SkiShop::onWeatherUpdate);

    connect(ui->shredGnarButton, &QPushButton::clicked,
            model, &AppModel::startGame);

    connect(ui->buySkiButton, &QPushButton::clicked,
            model, &AppModel::onPurchaseCurrentSki);

    connect(ui->allMountainSkiButton, &QPushButton::clicked,
            this, [this](){ emit skiSelected(AppModel::SkiType::ALLMOUNTAIN); });
    connect(ui->powderSkiButton, &QPushButton::clicked,
            this, [this](){ emit skiSelected(AppModel::SkiType::POWDER); });
    connect(ui->bigMountainSkiButton, &QPushButton::clicked,
            this, [this](){ emit skiSelected(AppModel::SkiType::FREERIDE); });
    connect(ui->mogulSkiButton, &QPushButton::clicked,
            this, [this](){ emit skiSelected(AppModel::SkiType::MOGUL); });
    connect(ui->carvingSkiButton, &QPushButton::clicked,
            this, [this](){ emit skiSelected(AppModel::SkiType::CARVING); });
    connect(ui->parkSkiButton, &QPushButton::clicked,
            this, [this](){ emit skiSelected(AppModel::SkiType::PARK); });
    connect(ui->raceSkiButton, &QPushButton::clicked,
            this, [this](){ emit skiSelected(AppModel::SkiType::RACE); });
    connect(ui->touringSkiButton, &QPushButton::clicked,
            this, [this](){ emit skiSelected(AppModel::SkiType::TOURING); });
    connect(model, &AppModel::notEnoughPoints,
            this, &SkiShop::onNotEnoughPoints);

    connect(model, &AppModel::currentSelectedSkiChanged,
            this, &SkiShop::selectedSkiChanged);

    connect(model, &AppModel::skiUnlocked,
            this, &SkiShop::onSkiUnlocked);

    connect(model, &AppModel::skiLocked,
            this, &SkiShop::onSkiLocked);

    connect(model, &AppModel::displayPlayerPoints,
            this, &SkiShop::updatePlayerPoints);

    typingTimer = new QTimer(this);

    connect(typingTimer, &QTimer::timeout,
            this, &SkiShop::typeNextCharacter);

    dialoguesPaths = {":/dialogues/a9_Resources/dialogues/skiShopTutorial.txt",
                       ":/dialogues/a9_Resources/dialogues/skiShopWeatherTutorial.txt",
                      ":/dialogues/a9_Resources/dialogues/skiShop.txt"};
}


SkiShop::~SkiShop(){
    delete ui;
}


void SkiShop::selectedSkiChanged(Ski* ski){
    ui->skiDisplayed->setPixmap(ski->getImagePath());
    ui->name->setText(ski->getName());
    ui->flex->setText(QString::number(ski->getFlex()));
    ui->width->setText(QString::number(ski->getWidth()) + " mm");
    ui->rocker->setText(QString::number(ski->getRocker()));
    currentSkiDescription = ski->getDescription();
    ui->price->setText(QString::number(ski->getCostPoints()));
    updateText();

    ui->groomerRating->setValue(static_cast<int>(ski->getGroomerScore() * 100));
    ui->powderRating->setValue(static_cast<int>(ski->getPowderScore() * 100));
    ui->iceRating->setValue(static_cast<int>(ski->getIcyScore() * 100));
}


void SkiShop::onSkiUnlocked(const QString& skiType){
    if (skiType == "Powder"){
        ui->powderSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/powderSki.png"));
    }
    else if (skiType == "Freeride"){
        ui->bigMountainSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/bigMountainSki.png"));
    }
    else if (skiType == "Mogul"){
        ui->mogulSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/mogulSki.png"));
    }
    else if (skiType == "Carving"){
        ui->carvingSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/carvingSki.png"));
    }
    else if (skiType == "Park"){
        ui->parkSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/parkSki.png"));
    }
    else if (skiType == "Race"){
        ui->raceSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/raceSki.png"));
    }
    else if (skiType == "Touring"){
        ui->touringSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/touringSki.png"));
    }
}


void SkiShop::onSkiLocked(const QString& skiType){
    if (skiType == "Powder"){
        ui->powderSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/LockedGreenSkis.png"));
    }
    else if (skiType == "Freeride"){
        ui->bigMountainSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/LockedPinkSkis.png"));
    }
    else if (skiType == "Mogul"){
        ui->mogulSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/LockedBlueOrangeSkis.png"));
    }
    else if (skiType == "Carving"){
        ui->carvingSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/LockedPurpleCyanSkis.png"));
    }
    else if (skiType == "Park"){
        ui->parkSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/LockedYellowSkis.png"));
    }
    else if (skiType == "Race"){
        ui->raceSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/LockedSuperSkis.png"));
    }
    else if (skiType == "Touring"){
        ui->touringSkiButton->setIcon(QIcon(":/skiSprites/a9_Resources/skiSprites/LockedGreenLightningSkis.png"));
    }
}


void SkiShop::readDialogue() {
    QString filename = dialoguesPaths[dialogueIndex];
    if(dialogueIndex < 2){
        dialogueIndex++;
        qDebug() << "dialogue index increased";
    }
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


void SkiShop::onNotEnoughPoints(){
    QString original = ui->buySkiButton->styleSheet();
    QString flashStyle = original;

    flashStyle.replace(
        "background-color: qlineargradient(\n"
        "        x1:0, y1:0, x2:0, y2:1,\n"
        "        stop:0 #243764,\n"
        "        stop:1 #1B2C56\n"
        "    );",
        "background-color: #d4022c;"
    );

    ui->buySkiButton->setStyleSheet(flashStyle);

    QTimer::singleShot(200, [this, original]() {
        ui->buySkiButton->setStyleSheet(original);
    });
}


void SkiShop::onShowWeatherIcons(){
    qDebug() << "showing";
    ui->weatherIcon->show();
    ui->weatherIconBackground->show();
}


void SkiShop::onWeatherUpdate(const AppModel::Weather& forecastedWeather){
    switch(forecastedWeather){
        case(AppModel::GROOMER):
            ui->weatherIcon->setPixmap(QPixmap(":/otherSprites/a9_Resources/otherSprites/SunnyDayIcon.png"));
        case(AppModel::POW):
            ui->weatherIcon->setPixmap(QPixmap(":/otherSprites/a9_Resources/otherSprites/PowderDayIcon.png"));
        case(AppModel::ICY):
            ui->weatherIcon->setPixmap(QPixmap(":/otherSprites/a9_Resources/otherSprites/IcyDayIcon.png"));
    }
}


void SkiShop::updatePlayerPoints(int points) {
    ui->pointsTotal->setText(QString::number(points));
}


void SkiShop::handelChangeStates() {
    currentSkiDescription = nullptr;
    readDialogue();
    updateText();
}


void SkiShop::showNext() {
    if (typingTimer->isActive()) {
        typingTimer->stop();
        charIndex = currentFullText.length();
        ui->textBrowser->setHtml(currentFullText);
        return;
    }

    textIndex++;

    if (textIndex > dialogues.size() - 1) {
        textIndex = 0;
    } else {
        updateText();
    }
}


void SkiShop::updateText() {
    typingTimer->stop();

    if(currentSkiDescription != nullptr)
        currentFullText = currentSkiDescription;
    else currentFullText = dialogues[textIndex];

    charIndex = 0;
    ui->textBrowser->clear();
    typingTimer->start(typingSpeed);
}


void SkiShop::typeNextCharacter() {
    if (charIndex >= currentFullText.length()) {
        typingTimer->stop();
        return;
    }

    QString partialText = currentFullText.left(charIndex + 1);
    ui->textBrowser->setHtml(partialText);
    charIndex++;
}
