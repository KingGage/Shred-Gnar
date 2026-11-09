/******************************************************************************
 * Project File:    TerminologyScreen.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The terminology screen ui which teaches the user about ski terms.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef TERMINOLOGYSCREEN_H
#define TERMINOLOGYSCREEN_H

#include <QDialog>
#include <QFile>
#include <QDebug>
#include "appmodel.h"


namespace Ui { class TerminologyScreen; }

class TerminologyScreen : public QDialog {
    Q_OBJECT
public:
    explicit TerminologyScreen(AppModel* model, QWidget *parent = nullptr);
    ~TerminologyScreen();

private:
    Ui::TerminologyScreen *ui;
    int textIndex = 0;
    QTimer *typingTimer;
    QString currentFullText;
    QVector<QString> dialogues;
    int charIndex;
    int typingSpeed;
    QStringList imagePaths;

    ///
    /// \brief updateText - updates the text and begins text scrolling animation.
    ///
    void updateText();
    ///
    /// \brief readDialogue - reads from the file and gets the dialogue paragraph strings ready for display.
    /// \param filename
    ///
    void readDialogue(const QString &filename);

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

#endif // TERMINOLOGYSCREEN_H
