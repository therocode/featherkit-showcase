#pragma once
#include "messages.h"
#include "physicsbody.h"
#include <featherkit/render2d.h>

class Physics
    :   public fea::MessageReceiver<DirtTextureSetMessage>
{
    public:
        Physics(fea::MessageBus& bus);
        ~Physics();

        void update();

        virtual void handleMessage(const DirtTextureSetMessage& mess) override;

    private:
        fea::MessageBus& messageBus;
        glm::vec2 gravity;
        //float thresholdAngle;

        void addVelocity(PhysicsBody& body);
        void addFalling(PhysicsBody& body);
        void terrainCheck(PhysicsBody& body);
        bool terrainCollisionAt(glm::vec2 pos);

        const fea::Texture* dirtTexture;
        std::vector<PhysicsBody> ants;

        glm::vec2 rotatePoint(glm::vec2 pointToRotate, float degreesToRotate, glm::vec2 pointOfOrigin);
};
