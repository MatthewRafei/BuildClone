#include <iostream>
#include <SDL2/SDL.h>

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
			    1280, 720,
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

void handle_input(SDL_Event &event, bool &running)
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
      break;
    }
  }
}

void render(SDL_Window *window, SDL_Surface *win_surface)
{
  SDL_FillRect(win_surface, nullptr, SDL_MapRGB(win_surface->format, 240, 240, 240));
  SDL_UpdateWindowSurface(window);
}

// void update(Uint32 delta_time)
// {
//   // TODO
// }

int main(int argc, char** args)
{
  
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
    handle_input(event, running);
    render(window, win_surface); // delta_time
  }	

  sdl_quit(window);
  return 0;
}
