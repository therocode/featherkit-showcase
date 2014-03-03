#include "antapplication.h"

AntApplication::AntApplication()
    :   input(messageBus),
        antManager(messageBus),
        physics(messageBus),
        window(new fea::SDL2WindowBackend()),
        renderer(messageBus)
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
    /*
    antManager.createAnt(true, false, glm::vec2(100.0f, 300.0f));
    antManager.createAnt(true, true, glm::vec2(600.0f, 600.0f));
    antManager.createAnt(true, false, glm::vec2(150.0f, 300.0f));
    antManager.createAnt(true, true, glm::vec2(200.0f, 300.0f));
    antManager.createAnt(true, false, glm::vec2(300.0f, 300.0f));
    antManager.createAnt(true, true, glm::vec2(100.0f, 300.0f));
    antManager.createAnt(true, true, glm::vec2(500.0f, 600.0f));
    antManager.createAnt(true, true, glm::vec2(450.0f, 600.0f));
    antManager.createAnt(true, false, glm::vec2(700.0f, 600.0f));
    antManager.createAnt(true, false, glm::vec2(650.0f, 600.0f));
    */
}
