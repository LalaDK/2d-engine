#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Shape.h"

bool Application::IsRunning()
{
    return running;
}

void Application::Setup()
{
    running = Graphics::OpenWindow();
    pushForce = Vec2(0.0, 0.0);

    Body* body = new Body(CircleShape(50), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);
    body->radius = 4;
    bodies.push_back(body);
}

void Application::Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
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

    for(auto body: bodies)
    {
        body->AddForce(weight * body->mass);
        body->AddForce(pushForce);

        Vec2 drag = Force::GenerateDragForce(*body, 0.0005);
        body->AddForce(drag);
    }

    for(auto body: bodies)
    {
        body->Integrate(deltaTime);
    }


    for(auto body: bodies)
    {
        if(body->position.y + body->radius > Graphics::Height())
        {
            body->velocity.y = -body->velocity.y;
            body->position.y = Graphics::Height() - body->radius;
        }
        if(body->position.y - body->radius <= 0)
        {
            body->velocity.y = -body->velocity.y;
            body->position.y = body->radius;
        }
        if(body->position.x + body->radius > Graphics::Width())
        {
            body->velocity.x = -body->velocity.x;
            body->position.x = Graphics::Width() - body->radius;
        }
        if(body->position.x - body->radius <= 0)
        {
            body->velocity.x = -body->velocity.x;
            body->position.x = body->radius;
        }
    }

}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    for(auto body: bodies)
    {
        Graphics::DrawFillCircle(body->position.x, body->position.y, body->radius, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}

void Application::Destroy()
{
    for(auto body: bodies)
    {
        delete body;
    }

    Graphics::CloseWindow();
}
