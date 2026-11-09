/******************************************************************************
 * Project File:    Ski.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The ski class holds stats for different types of skis
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef SKI_H
#define SKI_H

#include <QString>


class Ski {

public:
    struct SkiPerformance {
        double speedMultiplier = 1.0;    // Affects up/down speed
        double turningMultiplier = 1.0;  // Affects left/right speed
    };

    Ski();

    ///
    /// \brief Ski - constructor
    /// \param name - name of the ski
    /// \param width - underfoot measurement
    /// \param flex - scaled 1 to 10 (stiffest)
    /// \param rocker - scaled 1 (none) to 10 (full)
    /// \param skiCostPoints - how many points the ski costs
    /// \param desc - description
    /// \param imagePath - where the ski sprite image is stored
    ///
    Ski(QString name, int width, int flex, int rocker, int skiCostPoints, QString desc, QString imagePath);

    ///
    /// \brief calculatePerformance sets the values for how ski preforms in differnt conditions.
    ///
    void calculatePerformance();

    ///
    /// \brief getName returns the name of the ski
    /// \return the name of the ski
    ///
    QString getName();

    ///
    /// \brief getWidth returns the width of the ski
    /// \return the width of the ski
    ///
    int getWidth();

    ///
    /// \brief getFlex returns the flex of the ski
    /// \return the flex of the ski
    ///
    int getFlex();

    ///
    /// \brief getRocker returns the rocker of the ski
    /// \return the rocker of the ski
    ///
    int getRocker();

    ///
    /// \brief getCostPoints returns the amount of points the ski costs.
    /// \return the cost of the ski
    ///
    int getCostPoints();

    ///
    /// \brief isLocked tells weither the ski has been purchased in the shop
    /// \return false if ski is locked
    ///
    bool isLocked();

    ///
    /// \brief getDescription returns teh descriptoin of the ski
    /// \return the ski description
    ///
    QString getDescription();

    ///
    /// \brief getImagePath returns the path the the ski sprite image
    /// \return the path to image
    ///
    QString getImagePath();

    ///
    /// \brief lock makes it so you can not purchase the ski
    ///
    void lock();

    ///
    /// \brief unlock allows you to purchase the ski.
    ///
    void unlock();


    SkiPerformance getGroomerPerformance();
    SkiPerformance getPowderPerformance();
    SkiPerformance getIcyPerformance();

    ///
    /// \brief getGroomerScore returns the groomer score
    /// \return how good the ski preforms in groomer conditions.
    ///
    double getGroomerScore();

    ///
    /// \brief getPowderScore returns the powder score
    /// \return how good the ski preforms in powder conditions.
    ///
    double getPowderScore();

    ///
    /// \brief getIcyScore returns the icy score
    /// \return how good the ski preforms in icy conditions.
    ///
    double getIcyScore();

private:
    QString name;
    int width;
    int flex;
    int rocker;
    int skiCostPoints;
    bool locked;
    QString imagePath;

    SkiPerformance groomerPerformance;
    SkiPerformance powderPerformance;
    SkiPerformance icyPerformance;

    ///
    /// \brief desc - brief description of the skis characteristics
    ///
    QString desc;
};

#endif // SKI_H
