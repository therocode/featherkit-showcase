#include "imageloader.h"
#include <featherkit/render2d.h>
#include <featherkit/userinterface.h>
#include <featherkit/util/window/sdl/sdlwindowbackend.h>
#include <featherkit/util/input/sdl/sdlinputbackend.h>
#include "rainhandler.h"
#include "interpolator.h"

enum {UP, LEFT, RIGHT, DOWN};

int main()
{
    fea::Window window(new fea::util::SDLWindowBackend());
    window.create(fea::VideoMode(800, 600, 32), "Rain");

    fea::InputHandler input(new fea::util::SDLInputBackend());

    fea::Texture groundTexture;
    fea::Texture groundbackTexture;
    fea::Texture skyTexture;
    fea::Texture rainTexture;
    fea::Texture rainSplashTexture;
    fea::Texture backgroundTexture;

    ImageLoader loader;
    groundTexture.create(800, 600, loader.loadImage("raindata/textures/ground.png", 800, 600).data(), false, true);
    groundbackTexture.create(800, 600, loader.loadImage("raindata/textures/groundback.png", 800, 600).data());
    skyTexture.create(800, 600, loader.loadImage("raindata/textures/sky.png", 800, 600).data());
    backgroundTexture.create(300, 600, loader.loadImage("raindata/textures/background.png", 300, 600).data());
    rainTexture.create(20, 20, loader.loadImage("raindata/textures/raindrop.png", 20, 20).data());
    rainSplashTexture.create(13, 13, loader.loadImage("raindata/textures/rainsplash.png", 13, 13).data());

    fea::Quad ground(1600, 1200);
    ground.setTexture(groundTexture);
    fea::Quad groundback(1600, 1200);
    groundback.setTexture(groundbackTexture);
    fea::Quad sky(800, 600);
    sky.setTexture(skyTexture);
    sky.setParallax(0.0f);
    sky.setPosition(-400, -300);
    fea::RepeatedQuad background(1800, 1000);
    background.setTileSize(300, 600);
    background.setParallax(0.5f);
    background.setPosition(-800.0f, 100.0f);
    background.setTexture(backgroundTexture);
    

    glm::vec2 camPos(800.0f/2.0f, 600.0f/2.0f + 300.0f);
    Interpolator cameraPosition(camPos);

    fea::Renderer2D renderer(fea::Viewport(800, 600, 0, 0, fea::Camera(cameraPosition.getPosition())));
    renderer.setup();

    RainHandler rainHandler;
    fea::Quad rainDrop(20.0f, 20.0f);
    rainDrop.setTexture(rainTexture);
    fea::Quad rainSplash(13.0f, 13.0f);
    rainSplash.setTexture(rainSplashTexture);

    bool directions[4] = {false, false , false, false};

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
                else if(event.key.code == fea::Keyboard::W)
                    directions[UP] = true;
                else if(event.key.code == fea::Keyboard::S)
                    directions[DOWN] = true;
                else if(event.key.code == fea::Keyboard::A)
                    directions[LEFT] = true;
                else if(event.key.code == fea::Keyboard::D)
                    directions[RIGHT] = true;
            }
            else if(event.type == fea::Event::KEYRELEASED)
            {
                if(event.key.code == fea::Keyboard::W)
                    directions[UP] = false;
                else if(event.key.code == fea::Keyboard::S)
                    directions[DOWN] = false;
                else if(event.key.code == fea::Keyboard::A)
                    directions[LEFT] = false;
                else if(event.key.code == fea::Keyboard::D)
                    directions[RIGHT] = false;
            }
        }

        if(directions[UP])
            camPos.y -= 12;
        if(directions[DOWN])
            camPos.y += 12;
        if(directions[LEFT])
            camPos.x -= 12;
        if(directions[RIGHT])
            camPos.x += 12;

        if(camPos.x < 400.0f)
            camPos.x = 400.0f;
        else if(camPos.x > 1200.0f)
            camPos.x = 1200.0f;
        if(camPos.y < 300.0f)
            camPos.y = 300.0f;
        else if(camPos.y > 900.0f)
            camPos.y = 900.0f;

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

        cameraPosition.setPosition(camPos);
        cameraPosition.update();
        renderer.getViewport().getCamera().setPosition(cameraPosition.getPosition());

        renderer.clear();
        renderer.queue(sky);
        renderer.queue(background);
        renderer.queue(groundback);
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
