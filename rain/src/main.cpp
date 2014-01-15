#include "imageloader.h"
#include <featherkit/render2d.h>
#include <featherkit/userinterface.h>
#include <featherkit/util/window/sdl/sdlwindowbackend.h>
#include <featherkit/util/input/sdl/sdlinputbackend.h>
#include "rainhandler.h"

int main()
{
    fea::Window window(new fea::util::SDLWindowBackend());
    window.create(fea::VideoMode(800, 600, 32), "Rain");

    fea::InputHandler input(new fea::util::SDLInputBackend());

    fea::Texture groundTexture;
    fea::Texture skyTexture;
    fea::Texture rainTexture;
    fea::Texture rainSplashTexture;

    ImageLoader loader;
    groundTexture.create(800, 600, loader.loadImage("raindata/textures/ground.png", 800, 600).data(), false, true);
    skyTexture.create(800, 600, loader.loadImage("raindata/textures/sky.png", 800, 600).data());
    rainTexture.create(20, 20, loader.loadImage("raindata/textures/raindrop.png", 20, 20).data());
    rainSplashTexture.create(13, 13, loader.loadImage("raindata/textures/rainsplash.png", 13, 13).data());

    fea::Quad ground(800, 600);
    ground.setTexture(groundTexture);
    fea::Quad sky(800, 600);
    sky.setTexture(skyTexture);

    fea::Renderer2D renderer(fea::Viewport(800, 600, 0, 0, fea::Camera(800.0f/2.0f, 600.0f/2.0f)));
    renderer.setup();

    RainHandler rainHandler;
    fea::Quad rainDrop(20.0f, 20.0f);
    rainDrop.setTexture(rainTexture);
    fea::Quad rainSplash(13.0f, 13.0f);
    rainSplash.setTexture(rainSplashTexture);

    bool quit = false;
    while(!quit)
    {
        input.processEvents();
        fea::Event event;

        while(input.pollEvent(event))
        {
            if(event.type == fea::Event::KEYPRESSED)
            {
                if(event.key.code == fea::Keyboard::Q || event.key.code == fea::Keyboard::ESCAPE)
                    quit = true;
            }
        }

        rainHandler.update(groundTexture);

        auto collisions = rainHandler.getCollisions();

        for(const auto& collision : collisions)
            groundTexture.setPixels([&] (uint32_t w, uint32_t h, uint8_t* data) 
            {
                (void)h;
                uint32_t startX = collision.x - 6;
                uint32_t startY = collision.y - 6;

                for(uint32_t x = 0; x < 12; x++)
                {
                    for(uint32_t y = 0; y < 12; y++)
                    {
                        if(glm::distance(glm::vec2(startX + x, startY + y), collision) < 6.0f)
                        data[((startX + x) + (startY + y) * w) * 4 + 3] = 0;
                    }
                }
            });

        if(collisions.size() > 0)
            groundTexture.update();

        renderer.clear();
        renderer.queue(sky);
        renderer.queue(ground);
        for(auto position : rainHandler.getRaindropPositions())
        {
            rainDrop.setPosition(position);
            renderer.queue(rainDrop);
        }

        auto splashPositions = rainHandler.getRainsplashPositions();
        auto splashSizes = rainHandler.getRainsplashSizes();

        for(uint32_t i = 0; i < splashPositions.size(); i++)
        {
            rainSplash.setPosition(splashPositions[i]);
            rainSplash.setScale({splashSizes[i], splashSizes[i]});
            renderer.queue(rainSplash);
        }

        renderer.render();
        window.swapBuffers();
    }
}
