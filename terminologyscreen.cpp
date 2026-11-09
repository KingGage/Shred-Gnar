/******************************************************************************
 * Project File:    TerminologyScreen.cpp
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The terminology screen implementation.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#include "terminologyscreen.h"
#include "ui_terminologyscreen.h"
#include <QTimer>

TerminologyScreen::TerminologyScreen(AppModel* model, QWidget *parent)
    : QDialog(parent), ui(new Ui::TerminologyScreen) {
    ui->setupUi(this);

    connect(ui->nextButton, &QPushButton::clicked,
            this, &TerminologyScreen::showNext);

    connect(this, &TerminologyScreen::nextClicked,
            model, &AppModel::onContinueClicked);

    typingTimer = new QTimer(this);

    connect(typingTimer, &QTimer::timeout,
            this, &TerminologyScreen::typeNextCharacter);

    textIndex = 0;
    charIndex = 0;
    typingSpeed = 30;

    readDialogue(":/dialogues/a9_Resources/dialogues/terminology.txt");

    imagePaths = {
        ":/skiSprites/a9_Resources/skiSprites/DiagramSki.png",
        ":/skiSprites/a9_Resources/skiSprites/DiagramSki.png",
        ":/skiSprites/a9_Resources/skiSprites/DiagramSki.png",
        ":/skiSprites/a9_Resources/skiSprites/DiagramUnderfoot.png",
        ":/skiSprites/a9_Resources/skiSprites/DiagramRocker.png",
        ":/skiSprites/a9_Resources/skiSprites/DiagramFlex.png",
        ":/skiSprites/a9_Resources/skiSprites/DiagramSki.png",
        ":/skiSprites/a9_Resources/skiSprites/DiagramSki.png"
    };

    ui->textBox->setStyleSheet(
        "QTextBrowser {"
            "background-color: transparent;"
            "color: black;"
            "font-family: 'Retro Gaming';"
            "font-size: 16pt;"
            "font-weight: bold;"
        "}"
    );
}


TerminologyScreen::~TerminologyScreen() {
    delete ui;
}


void TerminologyScreen::onStartDialogue(){
    updateText();
}


void TerminologyScreen::readDialogue(const QString &filename){
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

    // for (const QString &p : paragraphs){
    //     QString trimmed = p.trimmed();
    //     if (!trimmed.isEmpty()) {
    //         dialogues.append(trimmed);
    //     }
    // }

    for (int i = 0; i < paragraphs.size(); i++){
        QString trimmed = paragraphs[i].trimmed();
        if (!trimmed.isEmpty()) {
            dialogues.append(trimmed);
        }
    }

    textIndex = 0;
}


void TerminologyScreen::showNext(){
    if (typingTimer->isActive()){
        typingTimer->stop();
        charIndex = currentFullText.length();
        ui->textBox->setHtml(currentFullText);
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


void TerminologyScreen::updateText(){
    typingTimer->stop();
    currentFullText = dialogues[textIndex];
    charIndex = 0;
    ui->textBox->clear();

    if (textIndex < imagePaths.size()) {
        QPixmap pixmap(imagePaths[textIndex]);
        ui->diagram->setPixmap(pixmap);
        ui->diagram->setScaledContents(true);
    }

    typingTimer->start(typingSpeed);
}


void TerminologyScreen::typeNextCharacter() {
    if (charIndex >= currentFullText.length()) {
        typingTimer->stop();
        return;
    }

    QString partialText = currentFullText.left(charIndex + 1);
    ui->textBox->setHtml(partialText);
    charIndex++;
}
