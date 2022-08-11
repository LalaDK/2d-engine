#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Shape.h"

bool Application::IsRunning() { return running; }

void Application::Setup() {
  running = Graphics::OpenWindow();
  pushForce = Vec2(0.0, 0.0);

  Body *box = new Body(BoxShape(200, 100), Graphics::Width() / 2.0,
                       Graphics::Height() / 2.0, 1.0);
  bodies.push_back(box);
}

void Application::Input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_MOUSEBUTTONDOWN:
      break;
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
    }
  }
}

void Application::Update() {
  static int timePreviousFrame;
  int timeToWait = MILLIS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
  if (timeToWait > 0) {
    SDL_Delay(timeToWait);
  }

  float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0F;
  if (deltaTime > 0.016) {
    deltaTime = 0.016;
  }
  timePreviousFrame = SDL_GetTicks();

  for (auto body : bodies) {
    // Vec2 weight = Vec2(0.0, 9.8 * PIXELS_PER_METER);
    //  body->AddForce(weight * body->mass);

    float torque = 200;
    body->AddTorque(torque);

    body->AddForce(pushForce);

    // Vec2 drag = Force::GenerateDragForce(*body, 0.0005);
    // body->AddForce(drag);
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
  Graphics::ClearScreen(0xFF056263);

  for (auto body : bodies) {
    if (body->shape->GetType() == CIRCLE) {
      CircleShape *circleShape = (CircleShape *)body->shape;
      Graphics::DrawCircle(body->position.x, body->position.y,
                           circleShape->radius, body->rotation, 0xFFFFFFFF);
    }
    if (body->shape->GetType() == BOX) {
      BoxShape *boxShape = (BoxShape *)body->shape;
      Graphics::DrawPolygon(body->position.x, body->position.y,
                            boxShape->worldVertices, 0xFFFFFFFF);
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
