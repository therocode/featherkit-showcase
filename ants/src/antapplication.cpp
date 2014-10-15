#include "antapplication.h"
#include "anttype.h"

AntApplication::AntApplication()
    :   input(messageBus),
        antManager(messageBus),
        physics(messageBus),
        window(new fea::SDLWindowBackend()),
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
    size_t diggerAntId = antManager.createAnt(AntType::DIGGING, true, glm::vec2(320.0f, 1124.0f), 5.0f);
    messageBus.send(DiggerAntCreatedMessage({diggerAntId}));
    antManager.createAnt(AntType::NORMAL, false, glm::vec2(967.0f, 787.0f), (float)(rand() % 25 + 10)/10.0f);
    antManager.createAnt(AntType::NORMAL, false, glm::vec2(800.0f, 762.0f), (float)(rand() % 25 + 10)/10.0f);
    antManager.createAnt(AntType::NORMAL, false, glm::vec2(600.0f, 804.0f), (float)(rand() % 25 + 10)/10.0f);
    antManager.createAnt(AntType::NORMAL, false, glm::vec2(463.0f, 680.0f), (float)(rand() % 25 + 10)/10.0f);
}
