#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class TouchController : public Component
{
public:
    int currentRotation=1;//right;
	TransformComponent *transform;
	SpriteComponent *sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
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
		}
	}
};