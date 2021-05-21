#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>
#include "LTexture.h"

Map* map;
Manager manager;

LTexture bigCircleInput;
LTexture smallCircleInput;
Vector2D Game::direction = Vector2D(0,0);
SDL_Point pointA, pointB, circle,outerCircle;

bool touchStart=false;



SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0,0,800,640 };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& cow(manager.addEntity());
auto& pig(manager.addEntity());
auto& label(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}
//Creating joystick

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;
    circle.x = outerCircle.x = 250;
    circle.y = outerCircle.y = 830;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}
    bigCircleInput.loadFromFile( "outer_circle.png" ,renderer);
    smallCircleInput.loadFromFile( "inner_circle.png",renderer);


	assets->AddTexture("terrain", "terrain_ss.png");
	assets->AddTexture("player", "player_anims.png");
    assets->AddTexture("cow","animals/cow_walk.png");
    assets->AddTexture("pig","animals/pig_walk.png");
	assets->AddFont("arial", "arial.ttf", 16);

	map = new Map("terrain", 3, 32);

	//ecs implementation

	map->LoadMap(40, 30);


	Animation playerIdle = Animation(0, 3, 100);
	Animation playerWalk = Animation(1, 8, 100);
	player.addComponent<TransformComponent>(800.0f, 640.0f, 32 , 32, 4);
	player.addComponent<SpriteComponent>("player", true);
	//player.getComponent<SpriteComponent>().add()
	player.addComponent<TouchController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	SDL_Color white = { 255, 255, 255, 255 };
	
	label.addComponent<UILabel>(10, 10, "Test String", "arial", white);


}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));

void Game::handleEvents()
{
	
	SDL_PollEvent(&event);
    std::stringstream s1,s2,s3;

    SDL_Point moveLocation;

	int gScreenRect_x=2340;
	int gScreenRect_y=1080;

    SDL_Point offset;

    float offsetLength;

    switch (event.type)
	{
	    case SDL_FINGERDOWN:
			pointA.x = event.tfinger.x * gScreenRect_x;
			pointA.y = event.tfinger.y * gScreenRect_y;
            circle.x=pointA.x;  circle.y=pointA.y;
            outerCircle.x=pointA.x;  outerCircle.y=pointA.y;

            break;
	    case SDL_FINGERMOTION:
	        touchStart = true;
			pointB.x = event.tfinger.x * gScreenRect_x;
			pointB.y = event.tfinger.y * gScreenRect_y;

            offset.x = pointB.x - pointA.x; offset.y = pointB.y - pointA.y;
            offsetLength= sqrt(1.0*offset.x*offset.x + 1.0*offset.y*offset.y);
            direction.x = offset.x / offsetLength; direction.y = offset.y / offsetLength;
            circle.x = pointA.x + direction.x*bigCircleInput.getWidth()/2; circle.y = pointA.y + direction.y*bigCircleInput.getHeight()/2;

	        break;
		case SDL_FINGERUP:
		    touchStart= false;
            direction = Vector2D(0,0);
			circle.x = pointA.x + direction.x*bigCircleInput.getWidth()/2; circle.y = pointA.y + direction.y*bigCircleInput.getHeight()/2;

			break;
        case SDL_QUIT :
            isRunning = false;
            break;
        default:
            break;
	}
}



void Game::update()
{
    std::stringstream s1,s2,s3;
    if(touchStart){

    }


    //input render
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	player.getComponent<TransformComponent>().velocity.x = direction.x;
	player.getComponent<TransformComponent>().velocity.y = direction.y;


	manager.refresh();
	manager.update();


	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - 400);
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - 320);

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;
}

void Game::render()
{
	SDL_RenderClear(renderer);


	for (auto& t : tiles)
	{
		t->draw();
	}

	for (auto& c : colliders)
	{
		c->draw();
	}

	for (auto& p : players)
	{
		p->draw();
	}


	label.draw();
    bigCircleInput.render( outerCircle.x-bigCircleInput.getWidth()/2, outerCircle.y-bigCircleInput.getHeight()/2,renderer);
    smallCircleInput.render( circle.x - smallCircleInput.getWidth()/2,circle.y - smallCircleInput.getHeight()/2,renderer);
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}