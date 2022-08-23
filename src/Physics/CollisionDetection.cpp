#include "CollisionDetection.h"
#include <limits>

bool CollisionDetection::IsColliding(Body* a, Body *b, Contact& contact) {
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;
    bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
    bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;

    if(aIsCircle && bIsCircle) {
        return IsCollidingCircleCircle(a, b, contact);
    }
    if(aIsPolygon && bIsPolygon) {
        return IsCollidingPolygonPolygon(a, b, contact);
    }

    return false;
};

float FindMinSeperation(const PolygonShape& a, const PolygonShape& b) {
    float seperation = std::numeric_limits<float>::lowest();

    for(int i = 0; i < a.worldVertices.size(); i++) {
        Vec2 va = a.worldVertices[i];
        Vec2 normal = a.EdgeAt(i).Normal();

        float minSep = std::numeric_limits<float>::max();
        for(int j = 0; j < b.worldVertices.size(); j++) {
            Vec2 vb = b.worldVertices[j];

            minSep = std::min(minSep, (vb - va).Dot(normal));
            seperation= std::max(seperation, minSep);
        }
    }

    return seperation;
};

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body *b, Contact &contact) {
    const PolygonShape* aPolygonShape = (PolygonShape*) a->shape;
    const PolygonShape* bPolygonShape = (PolygonShape*) b->shape;
    if(FindMinSeperation(*aPolygonShape, *bPolygonShape) >= 0) {
        return false;
    }
    if(FindMinSeperation(*bPolygonShape, *aPolygonShape) >= 0) {
        return false;
    }

   return true;
};


bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body *b, Contact& contact) {
    CircleShape* aCircleShape = (CircleShape*) a->shape;
    CircleShape* bCircleShape = (CircleShape*) b->shape;

    const Vec2 ab = b->position - a->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;

    bool IsColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);

    if(!IsColliding) {
        return false;
    }

    contact.a = a;
    contact.b = b;
    contact.normal = ab;
    contact.normal.Normalize();

    contact.start = b->position - contact.normal * bCircleShape->radius;
    contact.end = a->position + contact.normal * aCircleShape->radius;

    contact.depth = (contact.end - contact.start).Magnitude();

    return true;

};
