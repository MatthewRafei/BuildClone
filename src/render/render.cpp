#include <SDL2/SDL.h>
#include <cmath>

#include "../map/map.hpp"
#include "../game/player.hpp"

// https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)
// https://www.doomworld.com/forum/topic/146286-why-is-software-rendering-so-different-from-hardware-rendering/

void render(SDL_Window *window, SDL_Surface *win_surface, const Player &player)
{
  // floor and sky color
  SDL_FillRect(win_surface, nullptr, SDL_MapRGB(win_surface->format, 240, 240, 240));
  
  int screen_width = 0;
  int screen_height = 0;

  // This function modifies parameters to return value
  SDL_GetWindowSize(window, &screen_width, &screen_height);

  // Calculate rays and direction
  for(int x = 0; x < screen_width; ++x) {
    
    float cameraX = 2 * x / float(screen_width) - 1; // -1 to 1 across the screen
    
    float direction = player.dir + cameraX * (player.fov / 2.0f);
    
    float rayDirX = std::cos(direction);
    float rayDirY = std::sin(direction);

    int mapX = player.x;
    int mapY = player.y;

    float deltaDistX = std::abs(1.0f / rayDirX);
    float deltaDistY = std::abs(1.0f / rayDirY);

    int stepX = 0;
    int stepY = 0;
    float sideDistX = 0.0;
    float sideDistY = 0.0;

    // X
    if(rayDirX < 0) {
      stepX = -1;
      sideDistX = (player.x - mapX) * deltaDistX;
    }
    else {
      stepX = 1;
      sideDistX = (mapX + 1.0f - player.x) * deltaDistX;
    }

    // Y
    if(rayDirY < 0) {
      stepY = -1;
      sideDistY = (player.y - mapY) * deltaDistY;
    }
    else {
      stepY = 1;
      sideDistY = (mapY + 1.0f - player.y) * deltaDistY;
    }

    bool hit = false;
    int side = 0; // 0 = x-side, 1 = y-side

    // Keep calculating until we hit wall
    while(!hit) {
      if(sideDistX < sideDistY) {
	sideDistX += deltaDistX;
	mapX += stepX;
	side = 0;
      }
      else {
	sideDistY += deltaDistY;
	mapY += stepY;
	side = 1;
      }

      if(map[mapY][mapX] > 0) {
	hit = true;
      }
    }

    // Calculate distance to wall
    float perpWallDist = 0.0;
      
    if (side == 0)
      perpWallDist = (mapX - player.x + (1 - stepX) / 2) / rayDirX;
    else
      perpWallDist = (mapY - player.y + (1 - stepY) / 2) / rayDirY;

    // Calculate height of line to draw
    int lineHeight = (int)(screen_height / perpWallDist);

    // Determine start and end y values for the vertical line
    int drawStart = -lineHeight / 2 + screen_height / 2;
    if (drawStart < 0) drawStart = 0;

    int drawEnd = lineHeight / 2 + screen_height / 2;
    if (drawEnd >= screen_height) drawEnd = screen_height - 1;

    Uint32 wallColor = SDL_MapRGB(win_surface->format, 100, 100, 100);
    if (side == 1) {
      wallColor = SDL_MapRGB(win_surface->format, 70, 70, 70); // Darken if Y-side
    }

    for (int y = drawStart; y < drawEnd; ++y) {
      Uint32 *pixels = (Uint32*)win_surface->pixels;
      pixels[y * (win_surface->pitch / 4) + x] = wallColor;
    }a
  }
  SDL_UpdateWindowSurface(window);
}
