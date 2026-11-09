/******************************************************************************
 * Project File:    StartScreen.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The start/welcome screen to the app.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>
#include "appmodel.h"


namespace Ui { class StartScreen; }

class StartScreen : public QWidget{
    Q_OBJECT

public:
    explicit StartScreen(AppModel *model, QWidget *parent = nullptr);
    ~StartScreen();

private:
    Ui::StartScreen *ui;
    AppModel *model = nullptr;
};

#endif // STARTSCREEN_H
