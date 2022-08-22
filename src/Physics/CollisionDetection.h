#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Body.h"
#include "Contact.h"

struct CollisionDetection {
    static bool IsColliding(Body* a, Body* b, Contact& contact);
    static bool IsCollidingCircleCircle(Body* a, Body* b, Contact& contact);
};

#endif