/******************************************************************************
 * Project File:    GameObject.h
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      The game object is any object in the game.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QPointF>
#include <QRectF>


class GameObject{
public:
    ///
    /// \brief The Obstacle enum - different types of obstacles/objects in the game
    ///
    enum Obstacle {
        SMALLTREE,
        LARGETREE,
        ROCK,
        CLIFF,
        SNOWPILE,
        POWDER,
        JUMP,
        GREENCIRCLESIGN,
        BLUESQUARESIGN,
        BLACKDIAMONDSIGN
    };

    ///
    /// \brief GameObject - constructor
    /// \param type - the type of obstacle
    /// \param box - rectangle containing position and dimensions
    ///
    GameObject(Obstacle type, const QRectF& box);

    ///
    /// \brief getType - returns the type of this object
    /// \return Obstacle enum value
    ///
    Obstacle getType() const;

    ///
    /// \brief getBoundingRect - returns the collision rectangle
    /// \return QRectF containing position and size
    ///
    QRectF getBoundingRect() const;

    ///
    /// \brief getPosition - returns the top-left position
    /// \return QPointF position
    ///
    QPointF getPosition() const;

    ///
    /// \brief setPosition - moves the object to a new position
    /// \param pos - new top-left position
    ///
    void setPosition(const QPointF& pos);

    ///
    /// \brief isOffScreen - checks if object has scrolled off the top
    /// \param screenHeight - height of the game screen (default 750)
    /// \return true if completely off screen
    ///
    bool isOffScreen() const;

private:
    Obstacle obstacleType;
    QRectF boundingBox;
};

#endif // GAMEOBJECT_H
