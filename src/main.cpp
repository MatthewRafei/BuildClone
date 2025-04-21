#include <iostream>
#include <SDL2/SDL.h>

#include "render/render.hpp"
#include "game/player.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

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

void handle_input(SDL_Event &event, bool &running, Player &player)
{
  while(SDL_PollEvent(&event)){
    switch(event.type){
    case SDL_QUIT:
      running = 0;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_q){
	running = 0;
      }
      if (event.key.keysym.sym == SDLK_a){
	player.dir += 0.1;
      }
      if (event.key.keysym.sym == SDLK_d){
	player.dir -= 0.1;
      }
      if (event.key.keysym.sym == SDLK_w){
	player.x += 0.1;
      }
      if (event.key.keysym.sym == SDLK_s){
	player.x -= 0.1;
      }
      break;
    }
  }
}

// void update(Uint32 delta_time)
// {
//   // TODO
// }

int main(int argc, char** args)
{
  // x, y, angle, fov, health
Player player = { 5.5f, 2.5f,  // x, y — starting at open space
                  0.2f,        // facing east (0 radians)
                  1.047f,      // fov 60 degrees
                  100.0f       // health
};

  
  SDL_Surface *win_surface = nullptr;
  SDL_Window *window = nullptr;

  if(!sdl_init(window, win_surface)){
    return 1;
  }

  // Get time in milliseconds since SDL lib was init
  // Uint32 last_time = SDL_GetTicks();

  SDL_Event event;
  bool running = true;

  // Game Loop
  while(running){
    // Uint32 current_time = GetTicks();
    // float delta_time = (current_time - last_time) / 1000.0f // find a better way to do this later
    handle_input(event, running, player);
    //update(delta_time);
    render(window, win_surface, player);
    
  }	

  sdl_quit(window);
  return 0;
}
