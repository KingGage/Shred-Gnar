/******************************************************************************
 * Project File:    GameObject.cpp
 * Team Name:       FANTASTIC 4++
 * Team Members:    Rachel Gundersen, Jacob Wardle, Brandon Flickinger,
 *                  Ethan Laynor, Gage Hansen
 * Assignment:      A9: Shred Gnar
 * Reviewer:        Brandon Flickinger
 * Last Updated:    December 8, 2025
 *
 * Description:
 *      ----------------------------------------------------------------------
 *      Game object implementation.
 *      ----------------------------------------------------------------------
 *
 *****************************************************************************/


#include "gameobject.h"


GameObject::GameObject(Obstacle type, const QRectF& box)
    : obstacleType(type), boundingBox(box) {}

GameObject::Obstacle GameObject::getType() const{
    return obstacleType;
}

QRectF GameObject::getBoundingRect() const{
    return boundingBox;
}

QPointF GameObject::getPosition() const{
    return boundingBox.topLeft();
}

void GameObject::setPosition(const QPointF& pos){
    boundingBox.moveTo(pos);
}

bool GameObject::isOffScreen() const{
    return boundingBox.bottom() < 0;
}
