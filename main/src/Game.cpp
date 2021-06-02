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
int frame=0;
auto& player(manager.addEntity());
auto& chicken(manager.addEntity());
auto& cow(manager.addEntity());
auto& sheep(manager.addEntity());
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
    assets->AddTexture("egg", "products/egg.png");
    assets->AddTexture("chicken","animals/chicken.png");
    assets->AddTexture("cow","animals/cow.png");
    assets->AddTexture("sheep","animals/sheep.png");
    assets->AddTexture("pig","animals/pig.png");
	//assets->AddTexture("chicken_walk","animals/chicken_walk.png");
	//assets->AddTexture("chicken_eat","animals/chicken_eat.png");
    //assets->AddTexture("cow_walk","animals/cow_walk.png");
    //assets->AddTexture("cow_eat","animals/cow_eat.png");

	//assets->AddTexture("cow","animals/cow_walk.png");
    //assets->AddTexture("pig","animals/pig_walk.png");
	assets->AddFont("arial", "arial.ttf", 16);

	map = new Map("terrain", 3, 32);

	//ecs implementation

	map->LoadMap(40, 30);

    //all the animations
    Animation playerAnimations[2],chickenAnimations[8],cowAnimations[8],sheepAnimations[8],pigAnimations[8];
    //player animations
    /*``
	Animation playerIdle = Animation(0, 3, 100);
	Animation playerWalk = Animation(1, 8, 100);
    playerAnimations[0] = playerIdle;
    playerAnimations[1] = playerWalk;
    //chicken animations

    Animation chickenWalkUp    = Animation( 0 , 4 , 100 );
    Animation chickenWalkDown  = Animation( 2 , 4 , 100 );
    Animation chickenWalkRight = Animation( 3 , 4 , 100 );
    Animation chickenWalkLeft  = Animation( 1 , 4 , 100 );
    chickenWalkAnimations[0] = chickenWalkUp;
    chickenWalkAnimations[1] = chickenWalkDown;
    chickenWalkAnimations[2] = chickenWalkRight;
    chickenWalkAnimations[3] = chickenWalkLeft;
    */
    //player animations
    playerAnimations[0] = Animation(0, 3, 100);
    playerAnimations[1] = Animation(1, 8, 100);
    //chicken animations
    chickenAnimations[0] = Animation( 0 , 4 , 100 );
    chickenAnimations[1] = Animation( 2 , 4 , 100 );
    chickenAnimations[2] = Animation( 3 , 4 , 100 );
    chickenAnimations[3] = Animation( 1 , 4 , 100 );
    chickenAnimations[4] = Animation( 4 , 4 , 100 );
    chickenAnimations[5] = Animation( 6 , 4 , 100 );
    chickenAnimations[6] = Animation( 7 , 4 , 100 );
    chickenAnimations[7] = Animation( 5 , 4 , 100 );
    //cow animations
    cowAnimations[0] = Animation( 0 , 4 , 100 );
    cowAnimations[1] = Animation( 2 , 4 , 100 );
    cowAnimations[2] = Animation( 3 , 4 , 100 );
    cowAnimations[3] = Animation( 1 , 4 , 100 );
    cowAnimations[4] = Animation( 4 , 4 , 100 );
    cowAnimations[5] = Animation( 6 , 4 , 100 );
    cowAnimations[6] = Animation( 7 , 4 , 100 );
    cowAnimations[7] = Animation( 5 , 4 , 100 );
    //sheep animations
    sheepAnimations[0] = Animation( 0 , 4 , 100 );
    sheepAnimations[1] = Animation( 2 , 4 , 100 );
    sheepAnimations[2] = Animation( 3 , 4 , 100 );
    sheepAnimations[3] = Animation( 1 , 4 , 100 );
    sheepAnimations[4] = Animation( 4 , 4 , 100 );
    sheepAnimations[5] = Animation( 6 , 4 , 100 );
    sheepAnimations[6] = Animation( 7 , 4 , 100 );
    sheepAnimations[7] = Animation( 5 , 4 , 100 );
    //pig animations
    pigAnimations[0] = Animation( 0 , 4 , 100 );
    pigAnimations[1] = Animation( 2 , 4 , 100 );
    pigAnimations[2] = Animation( 3 , 4 , 100 );
    pigAnimations[3] = Animation( 1 , 4 , 100 );
    pigAnimations[4] = Animation( 4 , 4 , 100 );
    pigAnimations[5] = Animation( 6 , 4 , 100 );
    pigAnimations[6] = Animation( 7 , 4 , 100 );
    pigAnimations[7] = Animation( 5 , 4 , 100 );

            //animations names
    //std::string i="Idle",w="Walk",cwu="ChickenWU",cwd="ChickenWD",cwr="ChickenWR",cwl="ChickenWL";
    //std::string playerAnimNames[2];playerAnimNames[0]=i;playerAnimNames[1]=w;
    /*std::string chickenWANames[4];
    chickenWANames[0] = cwu;
    chickenWANames[1] = cwd;
    chickenWANames[2] = cwr;
    chickenWANames[3] = cwl;
     */
    std::string playerAnimNames[2];
    playerAnimNames[0]="Idle";
    playerAnimNames[1]="Walk";

    std::string chickenANames[8];
    chickenANames[0] = "ChickenWU";
    chickenANames[1] = "ChickenWD";
    chickenANames[2] = "ChickenWR";
    chickenANames[3] = "ChickenWL";
    chickenANames[4] = "ChickenEU";
    chickenANames[5] = "ChickenED";
    chickenANames[6] = "ChickenER";
    chickenANames[7] = "ChickenEL";

    std::string cowANames[8];
    cowANames[0] = "CowWU";
    cowANames[1] = "CowWD";
    cowANames[2] = "CowWR";
    cowANames[3] = "CowWL";
    cowANames[4] = "CowEU";
    cowANames[5] = "CowED";
    cowANames[6] = "CowER";
    cowANames[7] = "CowEL";

    std::string sheepANames[8];
    sheepANames[0] = "SheepWU";
    sheepANames[1] = "SheepWD";
    sheepANames[2] = "SheepWR";
    sheepANames[3] = "SheepWL";
    sheepANames[4] = "SheepEU";
    sheepANames[5] = "SheepED";
    sheepANames[6] = "SheepER";
    sheepANames[7] = "SheepEL";

    std::string pigANames[8];
    pigANames[0] = "PigWU";
    pigANames[1] = "PigWD";
    pigANames[2] = "PigWR";
    pigANames[3] = "PigWL";
    pigANames[4] = "PigEU";
    pigANames[5] = "PigED";
    pigANames[6] = "PigER";
    pigANames[7] = "PigEL";

	player.addComponent<TransformComponent>(800.0f, 640.0f, 32 , 32, 6,3);
    chicken.addComponent<TransformComponent>(800.0f, 640.0f, 32 , 32, 3,1);
    cow.addComponent<TransformComponent>(800.0f, 640.0f, 128 , 128, 3,1);
    sheep.addComponent<TransformComponent>(800.0f, 640.0f, 128 , 128, 3,1);
    pig.addComponent<TransformComponent>(800.0f, 640.0f, 128 , 128, 3,1);
	//chicken.addComponent<EggSpawner>();


	//animations.emplace("Idle", idle);
	//animations.emplace("Walk", walk);

	player.addComponent<SpriteComponent>("player", playerAnimNames, playerAnimations,2,0);
	chicken.addComponent<SpriteComponent>("chicken",chickenANames,chickenAnimations,4,0);
	chicken.addComponent<AnimalMovement>();
    cow.addComponent<SpriteComponent>("cow",cowANames,cowAnimations,4,0);
    cow.addComponent<AnimalMovement>();
    sheep.addComponent<SpriteComponent>("sheep",sheepANames,sheepAnimations,4,0);
    sheep.addComponent<AnimalMovement>();
    pig.addComponent<SpriteComponent>("pig",pigANames,pigAnimations,4,0);
    pig.addComponent<AnimalMovement>();

	//player.getComponent<SpriteComponent>().add()
	player.addComponent<TouchController>();
	player.addComponent<ColliderComponent>("player");


	player.addGroup(groupPlayers);
    chicken.addGroup(groupChickens);
    cow.addGroup(groupCows);
    sheep.addGroup(groupSheeps);
    pig.addGroup(groupPigs);
	SDL_Color white = { 255, 255, 255, 255 };
	
	label.addComponent<UILabel>(10, 10, "Test String", "arial", white);


}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& chickens(manager.getGroup(Game::groupChickens));
auto& cows(manager.getGroup(Game::groupCows));
auto& sheeps(manager.getGroup(Game::groupSheeps));
auto& pigs(manager.getGroup(Game::groupPigs));

auto& colliders(manager.getGroup(Game::groupColliders));
auto& eggs(manager.getGroup(Game::groupEggs));
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
    frame++;
	//adding eggs
	if(frame % 400==0) {
        for (auto &chicken: chickens) {
            auto &egg(manager.addEntity());
            Vector2D eggPosition;
            eggPosition = chicken->getComponent<TransformComponent>().position;
            egg.addComponent<TransformComponent>(eggPosition.x, eggPosition.y, 12, 15, 3, 0);
            egg.addComponent<SpriteComponent>("egg");
            egg.addGroup(groupEggs);
        }
        eggs = manager.getGroup(Game::groupEggs);
    }

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
    for(auto& e: eggs){
        e->draw();

    }
    for (auto& c : chickens)
    {
        c->draw();
    }

    for (auto& c : cows)
    {
        c->draw();
    }
    for (auto& c : sheeps)
    {
        c->draw();
    }
    for (auto& c : pigs)
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