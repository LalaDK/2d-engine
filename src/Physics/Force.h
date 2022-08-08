#ifndef FORCE_H
#define FORCE_H

#include "./Vec2.h"
#include "./Body.h"

struct Force
{
    static Vec2 GenerateDragForce(const Body& body, float k);
    static Vec2 GenerateFrictionForce(const Body& body, float k);
    static Vec2 GenerateGravitationalForce(const Body& bodyA, const Body& bodyB, float G, float minDistance, float maxDistance);
    static Vec2 GenerateSpringForce(const Body& body, Vec2 anchor, float restLength, float k);
    static Vec2 GenerateSpringForce(const Body& a, const Body& b, float restLength, float k);
};

#endif
