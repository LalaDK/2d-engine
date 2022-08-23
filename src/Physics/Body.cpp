#include "Body.h"
#include <iostream>
#include <math.h>

Body::Body(const Shape& shape, float x, float y, float mass) {
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->velocity = Vec2(0, 0);
    this->acceleration = Vec2(0, 0);
    this->rotation = 0.0;
    this->angularAcceleration = 0.0;
    this->angularVelocity = 0.0;
    this->sumForces = Vec2(0,0);
    this->sumTorque = 0.0;
    this->restitution = 1.0;


    this->mass = mass;
    if(mass != 0.0) {
        this->invMass = 1.0 / this->mass;
    } else {
        this->invMass = 0.0;
    }

    I = shape.GetMomentOfInertia() * mass;
    if(I != 0.0) {
        this->invI = 1.0 / I;
    } else {
        this->invI = 0.0;
    }
    std::cout << "Body constructor called!" << std::endl;
}

Body::~Body() {
    delete shape;
    std::cout << "Body destructor called!" << std::endl;
}

bool Body::IsStatic() const {
    const float epsilon = 0.005f;
    return fabs(invMass - 0.0) < epsilon;
};

void Body::IntegrateLinear(float dt) {
    if(IsStatic()) {
        return;
    }
    acceleration = sumForces * invMass;
    velocity += acceleration * dt;
    position += velocity * dt;
    ClearForces();
}

void Body::IntegrateAngular(float dt) {
    if(IsStatic()) {
        return;
    }
    angularAcceleration = sumTorque * invI;
    angularVelocity += angularAcceleration * dt;
    rotation += angularVelocity * dt;
    ClearTorque();
}

void Body::AddForce(const Vec2 &force) {
    sumForces += force;
}

void Body::AddTorque(float torque) {
    sumTorque += torque;
}

void Body::ClearForces() {
    this->sumForces = Vec2(0.0, 0.0);
}

void Body::ClearTorque() {
    this->sumTorque = 0.0;
}

void Body::ApplyImpulse(const Vec2& j) {
    if(IsStatic()) {
        return;
    }

    velocity+= j * invMass;
}
