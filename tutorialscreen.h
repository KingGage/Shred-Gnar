/******************************************************************************
 * Project File:    TutorialScreen.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The tutorial screen explains the premise of the application.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef TUTORIALSCREEN_H
#define TUTORIALSCREEN_H

#include <QWidget>
#include <QFile>
#include <QFont>
#include <QTimer>

#include "appmodel.h"


namespace Ui { class TutorialScreen; }

class TutorialScreen : public QWidget {
    Q_OBJECT

public:
    explicit TutorialScreen(AppModel *model, QWidget *parent = nullptr);
    ~TutorialScreen();

private:
    int textIndex = 0;
    QTimer *typingTimer;
    QString currentFullText;
    int charIndex;
    int typingSpeed;
    QStringList imagePaths;
    QVector<QString> dialogues;

    Ui::TutorialScreen *ui;
    AppModel *model = nullptr;

    ///
    /// \brief updateText - updates the dialogue text and begins the text scroll animation.
    ///
    void updateText();

    ///
    /// \brief readDialogue - reads from the file and gets the dialogue paragraph strings ready for display.
    /// \param filename
    ///
    void readTutorialDialogue(const QString &filename);

signals:
    ///
    /// \brief nextClicked - progresses to the next set of dialogue or the next page of the app.
    ///
    void nextClicked();

public slots:
    ///
    /// \brief onStartDialogue - starts the first set of dialogue when the page is shown.
    ///
    void onStartDialogue();

private slots:
    ///
    /// \brief showNext - shows the next set of dialogue by updating the text then starting text scroll animation.
    ///
    void showNext();
    ///
    /// \brief typeNextCharacter - helper method to get the text scroll animation.
    ///
    void typeNextCharacter();
};

#endif // TUTORIALSCREEN_H
