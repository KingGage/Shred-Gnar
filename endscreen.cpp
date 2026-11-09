/******************************************************************************
 * Project File:    EndScreen.cpp
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      End screen view implementation.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#include "endscreen.h"
#include "ui_endscreen.h"


EndScreen::EndScreen(QWidget *parent)
    : QWidget(parent), ui(new Ui::EndScreen){
    ui->setupUi(this);

    ui->textBox->setStyleSheet(
        "QTextBrowser {"
            "background-color: transparent;"
            "color: black;"
            "font-family: 'Retro Gaming';"
            "font-size: 16pt;"
            "font-weight: bold;"
        "}"
    );

    charIndex = 0;
    typingSpeed = 30;

    dialogue = "Thanks for hanging with me on the mountain today. "
               "Let’s link up again soon—there’s always more snow to slash. "
               "Later, shred-dog!";

    typingTimer = new QTimer(this);

    connect(typingTimer, &QTimer::timeout,
            this, &EndScreen::typeNextCharacter);
    connect(ui->playAgainButton, &QPushButton::clicked,
            this, &EndScreen::playAgainButtonClicked);
    connect(ui->quitButton, &QPushButton::clicked,
            this, &EndScreen::quitButtonClicked);
    connect(ui->quitButton, &QPushButton::clicked,
            qApp, &QCoreApplication::quit);
}


EndScreen::~EndScreen(){
    delete ui;
}


void EndScreen::onStartDialogue(){
    updateText();
}


void EndScreen::updateText(){
    // Stop any previous typing animation
    typingTimer->stop();

    // Store the full text we want to display
    charIndex = 0;

    // Clear the text browser
    ui->textBox->clear();

    // Start the typing animation
    typingTimer->start(typingSpeed);
}


void EndScreen::typeNextCharacter(){
    if (charIndex >= dialogue.length()){
        typingTimer->stop();
        return;
    }

    // Get the text up to current character
    QString partialText = dialogue.left(charIndex + 1);

    // Display the partial text
    ui->textBox->setHtml(partialText);

    // Move to next character
    charIndex++;
}
