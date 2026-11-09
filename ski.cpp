/******************************************************************************
 * Project File:    Ski.cpp
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      Ski class implementation
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#include "ski.h"


Ski::Ski() : name(""), width(-1), flex(-1), rocker(-1), desc(""){}


Ski::Ski(QString name, int width, int flex, int rocker, int skiCostPoints, QString desc, QString imagePath)
    : name(name), width(width), flex(flex), rocker(rocker), skiCostPoints(skiCostPoints), locked(true), desc(desc), imagePath(imagePath){
    calculatePerformance();
}


void Ski::lock(){
    this->locked = true;
}


void Ski::unlock(){
    this->locked = false;
}


bool Ski::isLocked(){
    return this->locked;
}


QString Ski::getName(){
    return this->name;
}


int Ski::getWidth(){
    return this->width;
};


int Ski::getFlex(){
    return this->flex;
};


int Ski::getRocker(){
    return this->rocker;
};


QString Ski::getDescription(){
    return this->desc;
}


QString Ski::getImagePath(){
    return this->imagePath;
}


int Ski::getCostPoints(){
    return this->skiCostPoints;
}


void Ski::calculatePerformance(){
    // GROOMER PERFORMANCE (Hardpack/Corduroy)
    // Ideal: Narrow (75-85), Stiff (8-10), Low rocker (1-3)
    // Race, Carving, and Mogul skis should excel here

    double widthPenalty = qMax(0.0, (width - 85.0) / 100.0);  // Penalty for being too wide
    double flexBonus = qMax(0.0, (flex - 6.0) / 4.0);  // Reward stiffness above 6
    double rockerPenalty = rocker / 12.0;  // Penalty for rocker

    double groomerScore = 1.0 - widthPenalty + flexBonus - rockerPenalty;

    groomerPerformance.speedMultiplier = 0.80 + (groomerScore * 0.45);
    groomerPerformance.turningMultiplier = 0.75 + (groomerScore * 0.55);


    // POWDER PERFORMANCE (Deep Snow)
    // Ideal: Wide (110-120), Medium flex (5-7), High rocker (8-10)
    // Powder, Freeride, and Touring skis should excel here

    double widthBonus = qMax(0.0, (width - 95.0) / 50.0);  // Reward width above 95
    double flexScore = 1.0 - qAbs(flex - 6.0) / 6.0;  // Peak at flex 6
    double rockerBonus = qMax(0.0, (rocker - 5.0) / 5.0);  // Reward rocker above 5

    double powderScore = widthBonus + (flexScore * 0.5) + rockerBonus;

    powderPerformance.speedMultiplier = 0.75 + (powderScore * 0.40);
    powderPerformance.turningMultiplier = 0.70 + (powderScore * 0.50);


    // ICY PERFORMANCE (Hard/Icy Conditions)
    // Ideal: Narrow (70-80), Very stiff (8-10), Minimal rocker (1-2)
    // Race, Carving, and Mogul skis should dominate here

    double icyWidthPenalty = qMax(0.0, (width - 80.0) / 80.0);  // Penalty for width
    double icyFlexBonus = qMax(0.0, (flex - 7.0) / 3.0);  // Strong reward for stiffness
    double icyRockerPenalty = (rocker - 1.0) / 10.0;  // Strong penalty for rocker

    double icyScore = 1.0 - icyWidthPenalty + (icyFlexBonus * 1.2) - (icyRockerPenalty * 1.5);

    icyPerformance.speedMultiplier = 0.80 + (icyScore * 0.45);
    icyPerformance.turningMultiplier = 0.75 + (icyScore * 0.55);


    // Keep values in a reasonable range
    groomerPerformance.speedMultiplier = qBound(0.75, groomerPerformance.speedMultiplier, 1.30);
    groomerPerformance.turningMultiplier = qBound(0.70, groomerPerformance.turningMultiplier, 1.35);

    powderPerformance.speedMultiplier = qBound(0.75, powderPerformance.speedMultiplier, 1.25);
    powderPerformance.turningMultiplier = qBound(0.70, powderPerformance.turningMultiplier, 1.30);

    icyPerformance.speedMultiplier = qBound(0.75, icyPerformance.speedMultiplier, 1.30);
    icyPerformance.turningMultiplier = qBound(0.70, icyPerformance.turningMultiplier, 1.35);
}


Ski::SkiPerformance Ski::getGroomerPerformance(){
    return groomerPerformance;
}


Ski::SkiPerformance Ski::getPowderPerformance(){
    return powderPerformance;
}


Ski::SkiPerformance Ski::getIcyPerformance(){
    return icyPerformance;
}


double Ski::getGroomerScore() {
    calculatePerformance();

    // Combine with slight weight toward turning (more important on groomers)
    double combined = (groomerPerformance.speedMultiplier * 0.45) +
                      (groomerPerformance.turningMultiplier * 0.55);

    double normalized = (combined - 0.725) / (1.325 - 0.725);
    return qBound(0.0, normalized, 1.0);
}


double Ski::getPowderScore() {
    calculatePerformance();

    // In powder, speed matters more (staying afloat)
    double combined = (powderPerformance.speedMultiplier * 0.55) +
                      (powderPerformance.turningMultiplier * 0.45);

    double normalized = (combined - 0.725) / (1.275 - 0.725);
    return qBound(0.0, normalized, 1.0);
}


double Ski::getIcyScore() {
    calculatePerformance();

    // On ice, turning/edge hold is critical
    double combined = (icyPerformance.speedMultiplier * 0.40) +
                      (icyPerformance.turningMultiplier * 0.60);

    double normalized = (combined - 0.725) / (1.325 - 0.725);
    return qBound(0.0, normalized, 1.0);
}
