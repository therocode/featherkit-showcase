#pragma once
#include "messages.h"
#include "physicsbody.h"
#include <featherkit/render2d.hpp>

class Physics
    :   public fea::MessageReceiver<DirtTextureSetMessage>,
        public fea::MessageReceiver<AntCreationMessage>,
        public fea::MessageReceiver<AntDeletionMessage>,
        public fea::MessageReceiver<AntStartedDiggingMessage>,
        public fea::MessageReceiver<AntStoppedDiggingMessage>
{
    public:
        Physics(fea::MessageBus& bus);
        ~Physics();

        void update();

        virtual void handleMessage(const DirtTextureSetMessage& mess) override;
        virtual void handleMessage(const AntCreationMessage& mess) override;
        virtual void handleMessage(const AntDeletionMessage& mess) override;
        virtual void handleMessage(const AntStartedDiggingMessage& mess) override;
        virtual void handleMessage(const AntStoppedDiggingMessage& mess) override;

    private:
        fea::MessageBus& messageBus;
        glm::vec2 gravity;
        //float thresholdAngle;

        void addVelocity(PhysicsBody& body, size_t id);
        void addFalling(PhysicsBody& body);
        void terrainCheck(PhysicsBody& body);
        bool terrainCollisionAt(glm::vec2 pos);

        const fea::Texture* dirtTexture;
        std::unordered_map<size_t, PhysicsBody> ants;
        std::unordered_map<size_t, glm::vec2> targetPositions;

        glm::vec2 rotatePoint(glm::vec2 pointToRotate, float degreesToRotate, glm::vec2 pointOfOrigin);
};
