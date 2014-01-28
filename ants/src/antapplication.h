#include "inputhandler.h"
#include "imageloader.h"
#include "rainhandler.h"
#include "interpolator.h"
#include <featherkit/render2d.h>
#include <featherkit/structure.h>
#include <featherkit/util/window/sdl2/sdl2windowbackend.h>

class AntApplication : public fea::Application
{
    public:
        AntApplication();
        void loop() override;
        void setup(const std::vector<std::string>& args) override;
        void destroy() override;

    private:
        InputHandler input;
};

AntApplication::AntApplication()
{
}

void AntApplication::loop()
{
    input.inputLoop();
}

void AntApplication::setup(const std::vector<std::string>& args)
{
    (void)args;
}

void AntApplication::destroy()
{
}

