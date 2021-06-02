#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"
#include<math.h>
class AnimalMovement : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    std::string animNames[8];
    int frame=0;
    Vector2D old;
    int rotation=0;//0 up,1 right,2 down,3 left
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
        old = transform->position;
        for(int i=0;i<8;i++)
            animNames[i] = sprite->animNames[i];
    }

    void update() override
    {
        float moveLength;
        int state=rand()%100;

        frame++;
        if((abs(old.x-transform->position.x)<=5)&&(abs(old.y-transform->position.y)<=5)) {
            if(frame%50==0){
                if(state>90) {//moving random
                    Vector2D movementOffset = Vector2D(rand() % 300 - 150, rand() % 300 - 150);
                    moveLength = sqrt(movementOffset.x * movementOffset.x +
                                      movementOffset.y * movementOffset.y);
                    movementOffset.x /= moveLength;
                    movementOffset.y /= moveLength;

                    transform->velocity = movementOffset;
                    if (abs(movementOffset.x) > abs(movementOffset.y)) { //vertical sau orizontal
                        if (movementOffset.x >= 0) {//right
                            sprite->Play(animNames[2]);//"ChickenWR"
                            rotation=2;
                        } else {
                            sprite->Play(animNames[3]);//"ChickenWL"
                            rotation=3;
                        }
                    } else {
                        if (movementOffset.y <= 0) {//up
                            sprite->Play(animNames[0]);//"ChickenWU"
                            rotation=0;
                        } else {
                            sprite->Play(animNames[1]);//"ChickenWD"
                            rotation=1;
                        }
                    }
                }else if(state >30) {//stoping
                    transform->velocity = Vector2D(0,0);
                }else{//eating
                    transform->velocity = Vector2D(0,0);
                    switch (rotation) {
                        case 0:
                            sprite->Play(animNames[6]);
                            break;
                        case 1:
                            sprite->Play(animNames[7]);
                            break;
                        case 2:
                            sprite->Play(animNames[4]);
                            break;
                        case 3:
                            sprite->Play(animNames[5]);
                            break;
                    }
                    
                }
            }
            old=transform->position;
        }
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
};