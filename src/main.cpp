#include <iostream>
#include <SDL2/SDL.h>

#include "render/render.hpp"
#include "game/player.hpp"

// TODO: Vector normalization needed or not lol straffing 
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

void handle_input(SDL_Event &event, bool &running, Player &player, float &thrustX, float &thrustY)
{
  const float moveSpeed = 0.5f;  // Thrust per input
  const float rotSpeed = 0.05f;  // Rotation speed in radians

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
        running = false;
        break;

      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_q:
            running = false;
            break;

          case SDLK_w: // Forward
            thrustX += std::cos(player.dir) * moveSpeed;
            thrustY += std::sin(player.dir) * moveSpeed;
            break;

          case SDLK_s: // Backward
            thrustX -= std::cos(player.dir) * moveSpeed;
            thrustY -= std::sin(player.dir) * moveSpeed;
            break;

          case SDLK_a: // Rotate left
            player.dir -= rotSpeed;
            break;

          case SDLK_d: // Rotate right
            player.dir += rotSpeed;
            break;
        }
        break;
    }
  }
}


void update(SDL_Window *window, Player &player, float thrustX, float thrustY)
{
  // Push the updated surface to the screen
  SDL_UpdateWindowSurface(window);

  // Max player speed
  // Move into player
  const float maxSpeed = 0.2f;

  // Apply thrust vector to player based on input
  float speed = std::sqrt(thrustX * thrustX + thrustY * thrustY);

  // Clamp to max speed
  if (speed > maxSpeed) {
    float scale = maxSpeed / speed;
    thrustX *= scale;
    thrustY *= scale;
  }

  // Apply movement
  player.x += thrustX;
  player.y += thrustY;

  // Friction to slow down naturally
  // Move into player
  const float friction = 10.0f;
  thrustX *= friction;
  thrustY *= friction;
  
}

int main(int argc, char** args)
{
  
  Player player = {
    5.5f, 2.5f,  // x, y — starting at open space
    0.0f, 0.0f,  // x, y - Velocity
    0.0005f,    // thrust amount
    0.2f,        // facing east (0 radians)
    degToRad(120),// FOV
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

    // CLEAN
    float thrustX = 0.0f;
    float thrustY = 0.0f;
    
    handle_input(event, running, player, thrustX, thrustY);
    update(window, player, thrustX, thrustY);
    render(window, win_surface, player);
    
  }	
  sdl_quit(window);
  return 0;
}
