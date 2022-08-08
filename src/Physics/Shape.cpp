#include "Shape.h"
#include <iostream>

CircleShape::CircleShape(float radius) {
    this->radius = radius;
    std::cout << "CircleShape constructor called!" << std::endl;
};

CircleShape::~CircleShape() {
    std::cout << "CircleShape destructor called!" << std::endl;
};

ShapeType CircleShape::GetType() const {
    return CIRCLE;
};

Shape* CircleShape::Clone() const {
    return new CircleShape(radius);
};

PolygonShape::~PolygonShape() {
    // TODO ...
};
Shape* PolygonShape::Clone() const {
    return new PolygonShape(vertices);
}
PolygonShape::PolygonShape(const std::vector<Vec2> vertices) {
    // TODO ...
};

ShapeType PolygonShape::GetType() const {
    return POLYGON;
};
BoxShape::BoxShape(float width, float height) {
    // TODO ...
};

BoxShape::~BoxShape() {
    // TODO ...
};
ShapeType BoxShape::GetType() const {
    return BOX;
};

Shape* BoxShape::Clone() const {
    return new BoxShape(width, height);
}

