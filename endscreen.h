/******************************************************************************
 * Project File:    EndScreen.h
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


#ifndef ENDSCREEN_H
#define ENDSCREEN_H

#include <QWidget>
#include <QTimer>

#include "appmodel.h"


namespace Ui { class EndScreen; }

class EndScreen : public QWidget{
    Q_OBJECT

public:
    explicit EndScreen(QWidget *parent = nullptr);
    ~EndScreen();

private:
    ///
    /// \brief ui
    ///
    Ui::EndScreen *ui;

    ///
    /// \brief model App model
    ///
    AppModel *model = nullptr;

    ///
    /// \brief textIndex index of text
    ///
    int textIndex = 0;

    ///
    /// \brief typingTimer timer to track typing of dialuge
    ///
    QTimer *typingTimer;

    ///
    /// \brief dialogue text for dialogue of game
    ///
    QString dialogue;

    ///
    /// \brief charIndex character index in the current
    ///
    int charIndex;

    ///
    /// \brief typingSpeed how fast the text appears
    ///
    int typingSpeed;

    ///
    /// \brief imagePaths of sprites
    ///
    QStringList imagePaths;

    ///
    /// \brief updateText update the text
    ///
    void updateText();

signals:
    ///
    /// \brief playAgainButtonClicked when play again button is pushed, it resets the game and takes you back to the beginning
    ///
    void playAgainButtonClicked();

    ///
    /// \brief quitButtonClicked
    ///
    void quitButtonClicked();

public slots:
    ///
    /// \brief onStartDialogue when dialogue starts
    ///
    void onStartDialogue();

private slots:
    ///
    /// \brief typeNextCharacter type next character in text sequence
    ///
    void typeNextCharacter();
};

#endif // ENDSCREEN_H
