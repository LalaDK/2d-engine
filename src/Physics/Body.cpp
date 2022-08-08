#include "Body.h"
#include <iostream>

Body::Body(const Shape& shape, float x, float y, float mass) {
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->mass = mass;
    if(mass != 0.0)
    {
        this->invMass = 1.0 / this->mass;
    }
    else
    {
        this->invMass = 0.0;
    }
    std::cout << "Body constructor called!" << std::endl;
}

Body::~Body()
{
    delete shape;
    std::cout << "Body destructor called!" << std::endl;
}

void Body::Integrate(float dt)
{
    acceleration = sumForces * invMass;
    velocity += acceleration * dt;
    position += velocity * dt;
    ClearForces();
}

void Body::AddForce(const Vec2 &force)
{
    sumForces += force;
}

void Body::ClearForces()
{
    this->sumForces = Vec2(0.0, 0.0);
}
