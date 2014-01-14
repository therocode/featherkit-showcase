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

    ImageLoader loader;
    groundTexture.create(800, 600, loader.loadImage("raindata/textures/ground.png", 800, 600).data(), false, true);
    skyTexture.create(800, 600, loader.loadImage("raindata/textures/sky.png", 800, 600).data());

    fea::Quad ground(800, 600);
    ground.setTexture(groundTexture);
    fea::Quad sky(800, 600);
    sky.setTexture(skyTexture);

    fea::Renderer2D renderer(fea::Viewport(800, 600, 0, 0, fea::Camera(800.0f/2.0f, 600.0f/2.0f)));
    renderer.setup();

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

        renderer.clear();
        renderer.queue(sky);
        renderer.queue(ground);
        renderer.render();
        window.swapBuffers();
    }
}
