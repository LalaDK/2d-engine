#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Shape.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <iostream>

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();
    pushForce = Vec2(0.0, 0.0);

    Body* boxA = new Body(BoxShape(200, 200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);
    Body* boxB= new Body(BoxShape(200, 200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);
    boxA->angularVelocity= 0.4;
    boxB->angularVelocity= 0.1;
    bodies.push_back(boxA);
    bodies.push_back(boxB);

    //Body *bigBall = new Body(CircleShape(100), 100, 100, 1.0);
    //Body *smallBall = new Body(CircleShape(50), 500, 100, 2.0);
    //bodies.push_back(bigBall);
    //bodies.push_back(smallBall);
}

void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_UP) {
                pushForce.y = -50 * PIXELS_PER_METER;
            }
            if (event.key.keysym.sym == SDLK_DOWN) {
                pushForce.y = 50 * PIXELS_PER_METER;
            }
            if (event.key.keysym.sym == SDLK_LEFT) {
                pushForce.x = -50 * PIXELS_PER_METER;
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                pushForce.x = 50 * PIXELS_PER_METER;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_UP) {
                pushForce.y = 0;
            }
            if (event.key.keysym.sym == SDLK_DOWN) {
                pushForce.y = 0;
            }
            if (event.key.keysym.sym == SDLK_LEFT) {
                pushForce.x = 0;
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                pushForce.x = 0;
            }
            break;
        case SDL_MOUSEMOTION:
            int x, y;
            SDL_GetMouseState(&x, &y);
            bodies[0]->position.x = x;
            bodies[0]->position.y = y;
            break;
        }
    }
}

void Application::Update() {
    Graphics::ClearScreen(0xFF056263);
    static int timePreviousFrame;
    int timeToWait = MILLIS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0) {
        SDL_Delay(timeToWait);
    }

    // Calculate the deltatime in seconds
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0F;
    if (deltaTime > 0.016) {
        deltaTime = 0.016;
    }

    // Set the time of the current frame to be used in the next one
    timePreviousFrame = SDL_GetTicks();

    // Apply forces to bodies
    for (auto body : bodies) {
        // Apply the weight force
        //Vec2 weight = Vec2(0.0, 9.8 * PIXELS_PER_METER);
        //body->AddForce(weight * body->mass);

        // Apply the wind force
        //Vec2 wind = Vec2(5.0 * PIXELS_PER_METER, 0);
        //body->AddForce(wind);
    }

    for (auto body : bodies) {
        body->IntegrateLinear(deltaTime);
        body->IntegrateAngular(deltaTime);
        bool isPolygon =
            body->shape->GetType() == POLYGON || body->shape->GetType() == BOX;
        if (isPolygon) {
            PolygonShape *polygonShape = (PolygonShape *)body->shape;
            polygonShape->UpdateVertices(body->rotation, body->position);
        }
    }

    // Check all the rigidbodies with the other rigidbodies for collision
    for(int i = 0; i <= bodies.size() -1; i++) {
        for(int j = i + 1;  j < bodies.size(); j++) {
            Body* a = bodies[i];
            Body* b = bodies[j];

            a->IsColliding = false;
            b->IsColliding = false;

            Contact contact;

            if(CollisionDetection::IsColliding(a, b, contact)) {
                //contact.ResolveCollision();
                Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
                Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15, contact.start.y + contact.normal.y * 15, 0xFF0000FF);

                a->IsColliding = true;
                b->IsColliding = true;
            }
        }
    }

    for (auto body : bodies) {
        if (body->shape->GetType() == CIRCLE) {
            CircleShape *circleShape = (CircleShape *)body->shape;

            if (body->position.y + circleShape->radius > Graphics::Height()) {
                body->velocity.y = -body->velocity.y;
                body->position.y = Graphics::Height() - circleShape->radius;
            }
            if (body->position.y - circleShape->radius <= 0) {
                body->velocity.y = -body->velocity.y;
                body->position.y = circleShape->radius;
            }
            if (body->position.x + circleShape->radius > Graphics::Width()) {
                body->velocity.x = -body->velocity.x;
                body->position.x = Graphics::Width() - circleShape->radius;
            }
            if (body->position.x - circleShape->radius <= 0) {
                body->velocity.x = -body->velocity.x;
                body->position.x = circleShape->radius;
            }
        }
    }
}

void Application::Render() {

    for (auto body : bodies) {
        Uint32 color = body->IsColliding ? 0xFF0000FF : 0xFFFFFFFF;

        if (body->shape->GetType() == CIRCLE) {
            CircleShape *circleShape = (CircleShape *)body->shape;
            Graphics::DrawCircle(body->position.x, body->position.y,
                                 circleShape->radius, body->rotation, color);
        }
        if (body->shape->GetType() == BOX) {
            BoxShape *boxShape = (BoxShape *)body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y,
                                  boxShape->worldVertices, color);
        }
    }

    Graphics::RenderFrame();
}

void Application::Destroy() {
    for (auto body : bodies) {
        delete body;
    }

    Graphics::CloseWindow();
}
