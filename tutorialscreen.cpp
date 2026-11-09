/******************************************************************************
 * Project File:    TutorialScreen.cpp
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The tutorial screen implementation.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#include "tutorialscreen.h"
#include "ui_tutorialscreen.h"
#include <QTimer>


TutorialScreen::TutorialScreen(AppModel *model, QWidget *parent)
    : QWidget(parent), ui(new Ui::TutorialScreen){
    ui->setupUi(this);

    connect(ui->nextButton, &QPushButton::clicked,
            this, &TutorialScreen::showNext);

    connect(this, &TutorialScreen::nextClicked,
            model, &AppModel::onContinueClicked);

    typingTimer = new QTimer(this);

    connect(typingTimer, &QTimer::timeout,
            this, &TutorialScreen::typeNextCharacter);

    textIndex = 0;
    charIndex = 0;
    typingSpeed = 30;

    readTutorialDialogue(":/dialogues/a9_Resources/dialogues/tutorial.txt");
    imagePaths = {
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyGesture.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyStanding.png",
        ":/jonnySprites/a9_Resources/jonnySprites/JonnyCheer.png"
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
}


TutorialScreen::~TutorialScreen(){
    delete ui;
}


void TutorialScreen::onStartDialogue(){
    updateText();
}


void TutorialScreen::readTutorialDialogue(const QString &filename){
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open file:" << filename << file.errorString();
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    QStringList paragraphs = content.split("\n\n");
    dialogues.clear();

    for (const QString &p : paragraphs){
        QString trimmed = p.trimmed();
        if (!trimmed.isEmpty()) {
            dialogues.append(trimmed);
        }
    }

    textIndex = 0;
}


void TutorialScreen::showNext(){
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
        updateText();
    }
}


void TutorialScreen::updateText(){
    typingTimer->stop();
    currentFullText = dialogues[textIndex];
    charIndex = 0;
    ui->textBrowser->clear();

    if (textIndex < imagePaths.size()) {
        QPixmap pixmap(imagePaths[textIndex]);
        ui->tutorialJonny->setPixmap(pixmap);
        ui->tutorialJonny->setScaledContents(true);
    }

    typingTimer->start(typingSpeed);
}


void TutorialScreen::typeNextCharacter() {
    if (charIndex >= currentFullText.length()) {
        typingTimer->stop();
        return;
    }

    QString partialText = currentFullText.left(charIndex + 1);
    ui->textBrowser->setHtml(partialText);
    charIndex++;
}
