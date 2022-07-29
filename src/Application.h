#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Particle.h"
#include "./Physics/Force.h"
#include <vector>

class Application {
    private:
        bool running = false;
        std::vector<Particle*> particles;
        SDL_Rect liquid;
        Vec2 pushForce;

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif
