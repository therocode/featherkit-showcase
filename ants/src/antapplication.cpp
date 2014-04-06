#include "antapplication.h"

AntApplication::AntApplication()
    :   input(messageBus),
        antManager(messageBus),
        physics(messageBus),
        window(new fea::SDL2WindowBackend()),
        renderer(messageBus),
        digging(messageBus)
{
    messageBus.addSubscriber<QuitMessage>(*this);
    window.create(fea::VideoMode(800, 600, 32), "ants");
    renderer.setup();
    createInitialAnts();
}

AntApplication::~AntApplication()
{
    messageBus.removeSubscriber<QuitMessage>(*this);
}

void AntApplication::loop()
{
    input.inputLoop();
    antManager.update();
    physics.update();
    digging.update();
    renderer.render();
    window.swapBuffers();
}

void AntApplication::setup(const std::vector<std::string>& args)
{
    (void)args;
}

void AntApplication::destroy()
{
    window.close();              
}

void AntApplication::handleMessage(const QuitMessage& mess)
{
    (void)mess;
    quit();
}

void AntApplication::createInitialAnts()
{
    // digger ant! :) //
    size_t diggerAntId = antManager.createAnt(true, true, glm::vec2(346.0f, 952.0f), 3.5f);
    messageBus.send(DiggerAntCreatedMessage(diggerAntId));
    //antManager.createAnt(true, true, glm::vec2(600.0f, 600.0f));
}
