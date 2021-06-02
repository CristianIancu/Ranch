#pragma once

#include "../Game.h"
#include "ECS.h"
#include "../LTexture.h"
//#include "Components.h"
//Manager Game::manager;
class EggSpawner : public Component
{

public:
    Manager manager;
    AssetManager* assets;
    std::vector<Entity*> eggs;
    TransformComponent *transform;
    int frame=0;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        assets = new AssetManager(&manager);
    }
    void spawnEgg(){
        auto& egg(manager.addEntity());
        assets->AddTexture("egg","products/egg.png");
        egg.addComponent<TransformComponent>(transform->position.x, transform->position.y, 12 , 15, 4,0);
        egg.addComponent<SpriteComponent>("egg");
        egg.addGroup(Game::groupEggs);
        eggs = manager.getGroup(Game::groupEggs);
        manager.update();
    }
    void update() override
    {
        frame++;
        if(frame%800==0) {
            spawnEgg();
            manager.refresh();

        }

        /*
        Vector2D movementOffset = Vector2D(rand() % 200 - 100, rand() % 200 - 100);
        transform->position += movementOffset;
        if (abs(movementOffset.x) > abs(movementOffset.y)) { //vertical sau orizontal
            if (movementOffset.x >= 0) {//right
                sprite->Play("ChickenWR");
            } else {
                sprite->Play("ChickenWL");
            }
        } else {
            if (movementOffset.y <= 0) {//up
                sprite->Play("ChickenWU");
            } else {
                sprite->Play("ChickenWD");
            }
        }
*/
        /*
        if ((Game::direction.x != 0) || (Game::direction.y != 0)) {
            transform->velocity.x = Game::direction.x;
            transform->velocity.y = Game::direction.y;
            if ((Game::direction.x < 0)&&(currentRotation > 0)) { //change direction
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                currentRotation = -1;
            }
            if ((Game::direction.x > 0)&&(currentRotation < 0)) { //change direction
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                currentRotation = 1;
            }
            sprite->Play("Walk");
        }
        else{
            transform->velocity.x = Game::direction.x;
            transform->velocity.y = Game::direction.y;
            sprite->Play("Idle");
        }*/
    }
    void draw() override{
        for (auto& e : eggs)
        {

        }
    }
};