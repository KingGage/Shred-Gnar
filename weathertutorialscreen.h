/******************************************************************************
 * Project File:    WeatherTutorialScreen.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The weather tutorial screen explains to the user weather conditions.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef WEATHERTUTORIALSCREEN_H
#define WEATHERTUTORIALSCREEN_H

#include <QWidget>
#include <QFile>
#include <QFont>
#include <QTimer>

#include "appmodel.h"


namespace Ui {
class WeatherTutorialScreen;
}

class WeatherTutorialScreen : public QWidget
{
    Q_OBJECT

private:
    ///
    /// \brief textIndex index of current text
    ///
    int textIndex = 0;

    ///
    /// \brief weatherIconStart index at which the weather icon appears
    ///
    int weatherIconStart = 3;

    ///
    /// \brief weatherIconEnd index at which the weather icon disapears
    ///
    int weatherIconEnd = 18;

    ///
    /// \brief groomerStart index at which the groomer days are talked about
    ///
    int groomerStart = 3;

    ///
    /// \brief groomerSkiDisplay index at which groomer skis display
    ///
    int groomerSkiDisplay = 4;

    ///
    /// \brief powderStart index at which powder days are talked about
    ///
    int powderStart = 8;

    ///
    /// \brief powderSkiDisplay index at which powder skis display
    ///
    int powderSkiDisplay = 10;

    ///
    /// \brief icyStart index at which icy days are talked about
    ///
    int icyStart = 13;

    ///
    /// \brief icySkiDisplay index at which icy skis displays
    ///
    int icySkiDisplay = 15;

    ///
    /// \brief typingTimer timer to track typing
    ///
    QTimer *typingTimer;

    ///
    /// \brief currentFullText current full text
    ///
    QString currentFullText;

    ///
    /// \brief charIndex index of current character
    ///
    int charIndex;

    ///
    /// \brief typingSpeed speed at which text appears
    ///
    int typingSpeed;

    ///
    /// \brief jonnyImagePaths image path for jonny sprites
    ///
    QStringList jonnyImagePaths;

    ///
    /// \brief weatherImagePaths image path of weather sprites
    ///
    QStringList weatherImagePaths;

    ///
    /// \brief The weatherState enum which weather is currently being explained
    ///
    enum weatherState{Groomer, Powder, Icy};

    ///
    /// \brief currentSkiStack current stack of skis to be displayed
    ///
    QStringList currentSkiStack;

    ///
    /// \brief groomerSkis list of groomer skis
    ///
    QStringList groomerSkis;

    ///
    /// \brief powderSkis list of powder skis
    ///
    QStringList powderSkis;

    ///
    /// \brief icySkis list of icy skis
    ///
    QStringList icySkis;

    ///
    /// \brief currentWeatherState current weather condition being explained
    ///
    weatherState currentWeatherState;

    ///
    /// \brief dialogues list of dialogues jonny will read
    ///
    QVector<QString> dialogues;

public:
    ///
    /// \brief WeatherTutorialScreen screen to walk through weather conditions
    /// \param model App model
    /// \param parent
    ///
    explicit WeatherTutorialScreen(AppModel *model, QWidget *parent = nullptr);
    ~WeatherTutorialScreen();

signals:
    ///
    /// \brief nextClicked moves to the next dialogue
    ///
    void nextClicked();

public slots:
    ///
    /// \brief onStartDialogue when the dialogue starts
    ///
    void onStartDialogue();

private slots:
    ///
    /// \brief showNext moves to next screen
    ///
    void showNext();

    ///
    /// \brief typeNextCharacter type the next character in dialogue
    ///
    void typeNextCharacter();

private:
    Ui::WeatherTutorialScreen *ui;

    ///
    /// \brief updateTextBrowser update text box
    ///
    void updateTextBrowser();

    ///
    /// \brief readTutorialDialogue take in the file with the given dialogue
    /// \param filename name of file
    ///
    void readTutorialDialogue(const QString &filename);

    ///
    /// \brief getCurrentDisplaySprites get the sprites for the given weather condition
    ///
    void getCurrentDisplaySprites();

    ///
    /// \brief switchWeather switch which weather condition is being talked about
    /// \param nextWeather the next weather being discussed
    ///
    void switchWeather(weatherState nextWeather);

    ///
    /// \brief addSkiSprite add a ski to the display
    /// \param resourcePath path of sprite
    ///
    void addSkiSprite(const QString &resourcePath);
};

#endif // WEATHERTUTORIALSCREEN_H
