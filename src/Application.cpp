#include "Application.h"
#include "./Physics/Constants.h"

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();
    
    Particle* smallBall = new Particle(50, 100, 1.0);
    smallBall->radius = 4;
    particles.push_back(smallBall);

    Particle* bigBall = new Particle(100, 100, 3.0);
    bigBall->radius = 12;
    particles.push_back(bigBall);

    liquid.x = 0;
    liquid.y = Graphics::Height() / 2;
    liquid.width = Graphics::Width();
    liquid.height = Graphics::Height() / 2;
}

void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
        running = false;
        break;
        case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
          running = false;
        break;
    }
  }
}

void Application::Update() {
  static int timePreviousFrame;
  int timeToWait = MILLIS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
  if(timeToWait > 0) {
    SDL_Delay(timeToWait);
  }

  float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0F;
  if(deltaTime > 0.016) {
    deltaTime = 0.016;
  }
  timePreviousFrame = SDL_GetTicks();

  Vec2 wind = Vec2(0.2 * PIXELS_PER_METER, 0.0);
  Vec2 weight = Vec2(0.0, 9.8 * PIXELS_PER_METER);
  for(auto particle: particles) {
    particle->AddForce(wind);
    particle->AddForce(weight * particle->mass);
  }

  for(auto particle: particles) {
    particle->Integrate(deltaTime);
  }


  for(auto particle: particles) {
    if(particle->position.y + particle->radius > Graphics::Height()) {
      particle->velocity.y = -particle->velocity.y;
      particle->position.y = Graphics::Height() - particle->radius;
    }
    if(particle->position.y - particle->radius <= 0) {
      particle->velocity.y = -particle->velocity.y;
      particle->position.y = particle->radius;
    }
    if(particle->position.x + particle->radius > Graphics::Width()) {
      particle->velocity.x = -particle->velocity.x;
      particle->position.x = Graphics::Width() - particle->radius;
    }
    if(particle->position.x - particle->radius <= 0) {
      particle->velocity.x = -particle->velocity.x;
      particle->position.x = particle->radius;
    }
  }

}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
  Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.y, 0xFF6E3713);

for(auto particle: particles) {
    Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
  }

  Graphics::RenderFrame();
}

void Application::Destroy() {
  for(auto particle: particles) {
    delete particle;
  }

  Graphics::CloseWindow();
}
