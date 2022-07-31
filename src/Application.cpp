#include "Application.h"
#include "./Physics/Constants.h"

bool Application::IsRunning()
{
    return running;
}

void Application::Setup()
{
    running = Graphics::OpenWindow();
    pushForce = Vec2(0.0, 0.0);
    anchor = Vec2(Graphics::Width() / 2, 40);
    for(int i = 0; i < 10; i++) {
      Particle* bob = new Particle(Graphics::Width() / 2.0 + (i* 50), 100 + (i * 50), 1.0);
      bob->radius = 6;
      particles.push_back(bob);
    }
}

void Application::Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            int x, y;
            SDL_GetMouseState(&x, &y);
            Particle* ball;
            ball = new Particle(x, y, 1.0);
            ball->radius = 5;
            particles.push_back(ball);
            break;
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_UP)
            {
                pushForce.y = -50 * PIXELS_PER_METER;
            }
            if(event.key.keysym.sym == SDLK_DOWN)
            {
                pushForce.y = 50 * PIXELS_PER_METER;
            }
            if(event.key.keysym.sym == SDLK_LEFT)
            {
                pushForce.x = -50 * PIXELS_PER_METER;
            }
            if(event.key.keysym.sym == SDLK_RIGHT)
            {
                pushForce.x = 50 * PIXELS_PER_METER;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
            break;
        case SDL_KEYUP:
            if(event.key.keysym.sym == SDLK_UP)
            {
                pushForce.y = 0;
            }
            if(event.key.keysym.sym == SDLK_DOWN)
            {
                pushForce.y = 0;
            }
            if(event.key.keysym.sym == SDLK_LEFT)
            {
                pushForce.x = 0;
            }
            if(event.key.keysym.sym == SDLK_RIGHT)
            {
                pushForce.x = 0;
            }
            break;
        }
    }
}

void Application::Update()
{
    static int timePreviousFrame;
    int timeToWait = MILLIS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if(timeToWait > 0)
    {
        SDL_Delay(timeToWait);
    }

    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0F;
    if(deltaTime > 0.016)
    {
        deltaTime = 0.016;
    }
    timePreviousFrame = SDL_GetTicks();

    Vec2 weight = Vec2(0.0, 9.8 * PIXELS_PER_METER);

    Vec2 springForceAnchor = Force::GenerateSpringForce(*particles[0], anchor, 15, 300);
    particles[0]->AddForce(springForceAnchor);

    for(auto particle: particles)
    {
      particle->AddForce(weight * particle->mass);
      particle->AddForce(pushForce);

      Vec2 drag = Force::GenerateDragForce(*particle, 0.002);
      particle->AddForce(drag);
    }

    for(int i = 1; i < 10; i++) {
      Vec2 springForce = Force::GenerateSpringForce(*particles[i], *particles[i-1], 15, 300);
      particles[i]->AddForce(springForce);
      particles[i-1]->AddForce(-springForce);
    }

    //Vec2 attraction = Force::GenerateGravitationalForce(*particles[0], *particles[1], 1000.0, 5, 50);
    //particles[0]->AddForce(attraction);
    //particles[1]->AddForce(-attraction);


    for(auto particle: particles)
    {
        particle->Integrate(deltaTime);
    }


    for(auto particle: particles)
    {
        if(particle->position.y + particle->radius > Graphics::Height())
        {
            particle->velocity.y = -particle->velocity.y;
            particle->position.y = Graphics::Height() - particle->radius;
        }
        if(particle->position.y - particle->radius <= 0)
        {
            particle->velocity.y = -particle->velocity.y;
            particle->position.y = particle->radius;
        }
        if(particle->position.x + particle->radius > Graphics::Width())
        {
            particle->velocity.x = -particle->velocity.x;
            particle->position.x = Graphics::Width() - particle->radius;
        }
        if(particle->position.x - particle->radius <= 0)
        {
            particle->velocity.x = -particle->velocity.x;
            particle->position.x = particle->radius;
        }
    }

}

void Application::Render()
{
    Graphics::ClearScreen(0xFF056263);


    Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF0000FF);
    for(auto particle: particles)
    {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    }

    Graphics::RenderFrame();
}

void Application::Destroy()
{
    for(auto particle: particles)
    {
        delete particle;
    }

    Graphics::CloseWindow();
}
