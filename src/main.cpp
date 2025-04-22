#include <iostream>
#include <SDL2/SDL.h>

#include "render/render.hpp"
#include "game/player.hpp"

// TODO: Create more robust screen resolution system. Sometimes Segfaults if screen size changes. 
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

inline float degToRad(float degrees) {
    return degrees * M_PI / 180.0f;
}

inline float radToDeg(float radians) {
    return radians * 180.0f / M_PI;
}

void sdl_quit(SDL_Window *window)
{
  SDL_DestroyWindow(window);
  SDL_Quit();
}

bool sdl_init(SDL_Window *&window, SDL_Surface *&win_surface)
{ 
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
    std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
    return false;
  } 

  window = SDL_CreateWindow("BuildClone",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    SCREEN_WIDTH, SCREEN_HEIGHT, // SCREEN WIDTH AND HEIGHT
			    SDL_WINDOW_SHOWN);

  if (!window) {
    std::cout << "Error creating window: " << SDL_GetError()  << std::endl;
    return false;
  }

  win_surface = SDL_GetWindowSurface( window );

  if (!win_surface) {
    std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
    return false;
  }
  return true;
}

void handle_input(SDL_Event &event, bool &running, Player &player, float delta_time)
{
  // while(SDL_PollEvent(&event)){
  //   switch(event.type){
  //   case SDL_QUIT:
  //     running = 0;
  //     break;
  //   case SDL_KEYDOWN:
  //     if (event.key.keysym.sym == SDLK_q){
  // 	running = 0;
  //     }
  //     ////////////////////////////////////
  //     // W.A.S.D 
  //     ////////////////////////////////////
  //     if (event.key.keysym.sym == SDLK_w){
  // 	player.velocityY += sin(player.dir) * player.thrust * delta_time;
  // 	player.y += player.velocityY;
  //     }
  //     if (event.key.keysym.sym == SDLK_a){
  // 	player.velocityX += cos(player.dir) * player.thrust * delta_time;
  // 	player.x += player.velocityX;
  //     }
  //     if (event.key.keysym.sym == SDLK_s){
  // 	// -thrust may not be the way to go
  // 	player.velocityY += sin(player.dir) * -player.thrust * delta_time;
  // 	player.y += player.velocityY;
  //     }
  //     if (event.key.keysym.sym == SDLK_d){
  // 	player.velocityX += cos(player.dir) * -player.thrust * delta_time;
  // 	player.x += player.velocityX;
  //     }
  //     ////////////////////////////////////////
  //     if (event.key.keysym.sym == SDLK_RIGHT){
  // 	player.dir += 0.1;
  //     }
  //     if (event.key.keysym.sym == SDLK_LEFT){
  // 	player.dir -= 0.1;
  //     }
  //     break;
  //   }
  // }

  



  
}

void update(SDL_Window *window)
{
  // Push the updated surface to the screen
  SDL_UpdateWindowSurface(window);
}

int main(int argc, char** args)
{
  
  Player player = {
    5.5f, 2.5f,  // x, y — starting at open space
    0.0f, 0.0f,  // x, y - Velocity
    0.00005f,    // thrust amount
    0.2f,        // facing east (0 radians)
    degToRad(90),// FOV
    100.0f       // health
  };

  
  SDL_Surface *win_surface = nullptr;
  SDL_Window *window = nullptr;

  if(!sdl_init(window, win_surface)){
    return 1;
  }
  
  // Get time in milliseconds since SDL lib was init
  Uint32 last_time = SDL_GetTicks();

  SDL_Event event;
  bool running = true;

  // Game Loop
  while(running){
    // Delta time calc
    Uint32 current_time = SDL_GetTicks();
    float delta_time = (current_time - last_time) / 1000.0f;
    
    handle_input(event, running, player, delta_time);
    update(window);
    render(window, win_surface, player);
    
  }	
  sdl_quit(window);
  return 0;
}
